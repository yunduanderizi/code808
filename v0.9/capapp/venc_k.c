/*!
 ***********************************************************************
 \par Copyright
 \verbatim
  ________  _____           _____   _____           ____  ____   ____
     /     /       /       /       /       /     /   /    /   \ /
    /     /___    /       /___    /       /____ /   /    /____/ \___
   /     /       /       /       /       /     /   /    /           \
  /     /_____  /_____  /_____  /_____  /     / _ /_  _/_      _____/

  Copyright (c) 2009 Telechips Inc.
  Korad Bldg, 1000-12 Daechi-dong, Kangnam-Ku, Seoul, Korea
 \endverbatim
 ***********************************************************************
 */
/*!
 ***********************************************************************
 *
 * \file
 *		venc.c
 * \date
 *		2009/07/23
 * \author
 *		AV algorithm group(AValgorithm@telechips.com)
 * \brief
 *		video encoder
 *
 ***********************************************************************
 */
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>         // O_RDWR
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>

#include "venc.h"
#include "cdk_core.h"
#include "tcc_vpu_ioctl.h"

#define DBG_LINE()		printf("[%s][%d]\n", __FUNCTION__, __LINE__);

#define VPU_MGR_NAME	"/dev/vpu_mgr"
static int max_width = 0;
static int max_height = 0;
static pthread_mutex_t mgr_mutex;
static int mgr_mutex_count = 0;
#define MGR_LOCK()		pthread_mutex_lock(&mgr_mutex)
#define MGR_UNLOCK()	pthread_mutex_unlock(&mgr_mutex)

#define TCC_VPU_INPUT_BUF_SIZE 		(1024 * 1024)
#define STABILITY_GAP (512)

//#define VPU_ADD_SEQ_HEADER
#ifdef VPU_ADD_SEQ_HEADER
/* if you'd like to add sequence header in front of encoded data automatically, just use this definition! */
#endif

typedef struct tagVENC_Context
{
	unsigned char *seq_backup;
	unsigned int seq_len;
    unsigned long large_stream_buff_size;
	int gsBitWorkBufSize;
	codec_addr_t gsBitWorkBufAddr[3];

	codec_addr_t gsFrameBufAddr[3];
	unsigned int gsFrameBufSize;

	unsigned int gsMESearchBufSize;
	codec_addr_t gsMESearchBufAddr[3];

	VENC_INIT_t gsVpuEncInit_Info;
	VENC_PUT_HEADER_t gsVpuEncPutHeader_Info;
	VENC_SET_BUFFER_t gsVpuEncBuffer_Info;
	VENC_ENCODE_t gsVpuEncInOut_Info;

	unsigned int gsiSeqHeaderSize;
	unsigned int gsiSeqHeaderCnt;
	unsigned int gsiSzSeqHeader[3];
	unsigned int gsiFrameIdx;

	int gsBitstreamBufSize;
	codec_addr_t gsBitstreamBufAddr[3];
	unsigned int encoded_buf_size;
	codec_addr_t encoded_buf_base_pos[3];
	codec_addr_t encoded_buf_end_pos[3];
	codec_addr_t encoded_buf_cur_pos[3];
	unsigned int bAvcUsedNALStart;

	int vpu_enc_fd;
	int vpu_mgr_fd;
	int encoder_id;
	int mux_type;
}venc_context_t;

#define HEX_1MB		(0x100000)


/************************************************************************/
/* TEST and Debugging                                               								 */
/************************************************************************/
//static int DEBUG_ON	= 1;
//#define DPRINTF(msg...)	  printf( ": " msg);
//#define DSTATUS(msg...)	  if (DEBUG_ON) { printf( ": " msg);}
#define DBUG_FLIP(msg...) if (DEBUG_ON) { printf( ": " msg);}
#define DPRINTF_FRAME(msg...) printf(": " msg);

#define	LOGD		DSTATUS
#define	LOGE		DPRINTF
#define	LOGI		printf

static void vpu_env_close(venc_context_t* enc_ctx);

static int vpu_all_drv_reset_needed = 0;

//=========================================================================
// VENC DRIVER MANAGER
//=========================================================================
struct venc_drvs
{
	int drv_fd;
	char* dev_name;
};

#define NUM_OF_ENCODER_DRV		2
static struct venc_drvs venc_drv[NUM_OF_ENCODER_DRV] =
{
	{-1, "/dev/vpu_enc"}
	,{-1, "/dev/vpu_enc2"}
#if 0	
	,{-1, "/dev/vpu_enc3"}
	,{-1, "/dev/vpu_enc4"}
#endif	
};

static int vpu_num_of_used_drv(void)
{
	int count = 0;
	int tt = 0;

	while(tt < NUM_OF_ENCODER_DRV)
	{
		if(venc_drv[tt].drv_fd >= 0)
			count++;

		tt++;
	}

	DSTATUS("[%s][%d] num of used venc driver : %d\n", __FUNCTION__, __LINE__, count);
	return count;
}

#if 0
static unsigned long vpu_get_stream_buffer_size(int width, int height)
{
	if((width * height) <= (320*240))
	{
		return 0x80000; // 0.5Mb
	}
	else if((width * height) <= (720*480))
	{
		return 0x100000; //1Mb
	}
	else if((width * height) <= (1280*720))
	{
		return 0x200000; //2Mb
	}
	else
	{
		return 0x400000; //4Mb
	}
}
#endif


static int vpu_encoder_drv_open(void)
{
	int ret;
	int tt = 0;

	while(tt < NUM_OF_ENCODER_DRV)
	{
		if(venc_drv[tt].drv_fd == -1)
		{
			venc_drv[tt].drv_fd = 1;
			ret = open(venc_drv[tt].dev_name, O_RDWR);
			if(ret < 0)
			{
				venc_drv[tt].drv_fd = -1;
				LOGE("%s open error :%s\n", venc_drv[tt].dev_name, strerror(errno));
			}
			else
			{
				venc_drv[tt].drv_fd = ret;
				DSTATUS("[%s][%d]%s open succeed index:%d, fd:%d\n", __FUNCTION__, __LINE__, venc_drv[tt].dev_name, tt, venc_drv[tt].drv_fd);
			}

			return ret;
		}

		tt++;
	}
    return -1;
}

static int vpu_encoder_drv_close(int fd)
{
	int ret;
	int tt = 0;

	ret = close(fd);
	
	while(tt < NUM_OF_ENCODER_DRV)
	{
		if(venc_drv[tt].drv_fd == fd)
		{
			venc_drv[tt].drv_fd = -1;
			DSTATUS("[%s][%d] index:%d, fd:%d\n", __FUNCTION__, __LINE__, tt, fd);
			if(ret < 0)
			{
				LOGE("%s close error[%s]", venc_drv[tt].dev_name, strerror(errno));
			}
			
			break;
		}

		tt++;
	}
	return ret;
}
//=========================================================================



static void *vpu_sys_malloc_physical_addr(int venc_fd, unsigned int *remap_addr, int uiSize, Buffer_Type type)
{
	MEM_ALLOC_INFO_t alloc_mem;

	memset(&alloc_mem, 0x00, sizeof(MEM_ALLOC_INFO_t));

	alloc_mem.request_size = uiSize;
	alloc_mem.buffer_type = type;
	ioctl(venc_fd, V_ENC_ALLOC_MEMORY, &alloc_mem);

	if(remap_addr != NULL)
		*remap_addr = alloc_mem.kernel_remap_addr;

	return (void*)( alloc_mem.phy_addr );
}

static void *vpu_sys_malloc_virtual_addr(int venc_fd, int* pDev, codec_addr_t pPtr, int uiSize)
{
    if(!pDev)
    {
        printf("maybe NULL pdev!\n");
    }
    
	return (void *)mmap(NULL, uiSize, PROT_READ | PROT_WRITE, MAP_SHARED, venc_fd, pPtr);
}

static int vpu_sys_free_virtual_addr(int* pDev, void* pPtr, int uiSize)
{
    if(!pDev)
    {
        printf("maybe NULL pdev!\n");
    }
    
	return munmap((void*)pPtr, uiSize);
}

#if 0
static unsigned int vpu_sys_remain_memory_size(int mgr_fd)
{
	unsigned int sz_freeed_mem;

	ioctl(mgr_fd, VPU_GET_FREEMEM_SIZE, &sz_freeed_mem);

	return sz_freeed_mem;
}
#endif

static void vpu_update_sizeinfo(int mgr_fd, unsigned int image_width, unsigned int image_height)
{
	CONTENTS_INFO info;

	memset(&info, 0x00, sizeof(CONTENTS_INFO));
	info.type = VPU_ENC;
	info.width = image_width;
	info.height = image_height;
	ioctl(mgr_fd, VPU_SET_CLK, &info);

	return;
}

static int vpu_env_open(venc_context_t* enc_ctx, unsigned int image_width, unsigned int image_height)
{
	DSTATUS("[%s][%d] id:%d ----\n", __FUNCTION__, __LINE__, enc_ctx->encoder_id);
	
	if(vpu_num_of_used_drv() == NUM_OF_ENCODER_DRV)
	{
		LOGE("all the encoder drivers are already occupied.\n");
		goto err;
	}
	
	enc_ctx->vpu_enc_fd = vpu_encoder_drv_open();
	if(enc_ctx->vpu_enc_fd < 0)
	{
		goto err;
	}

	enc_ctx->vpu_mgr_fd = open(VPU_MGR_NAME, O_RDWR | O_NDELAY);
	if(enc_ctx->vpu_mgr_fd < 0)
	{
		LOGE("%s open error[%s]!!", VPU_MGR_NAME, strerror(errno));
		vpu_encoder_drv_close(enc_ctx->vpu_enc_fd);
		enc_ctx->vpu_enc_fd = -1;
		goto err;
	}

	
	max_width += image_width;
	max_height += image_height;
	vpu_update_sizeinfo(enc_ctx->vpu_mgr_fd, max_width, max_height);
	
	enc_ctx->gsiFrameIdx = 0;
	enc_ctx->gsiSeqHeaderCnt = 0;

	memset(&enc_ctx->gsVpuEncInit_Info.gsVpuEncInit,		0, sizeof(enc_ctx->gsVpuEncInit_Info.gsVpuEncInit) );
	memset(&enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput,		0, sizeof(enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput) );
	memset(&enc_ctx->gsVpuEncInit_Info.gsVpuEncInitialInfo,	0, sizeof(enc_ctx->gsVpuEncInit_Info.gsVpuEncInitialInfo) );
	memset(&enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput,		0, sizeof(enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput) );

	memset(enc_ctx->gsBitstreamBufAddr, 0x00, sizeof(enc_ctx->gsBitstreamBufAddr));
	memset(enc_ctx->gsBitWorkBufAddr, 0x00, sizeof(enc_ctx->gsBitWorkBufAddr));
	memset(enc_ctx->gsMESearchBufAddr, 0x00, sizeof(enc_ctx->gsMESearchBufAddr));
	memset(enc_ctx->gsFrameBufAddr, 0x00, sizeof(enc_ctx->gsFrameBufAddr));
	memset(enc_ctx->encoded_buf_base_pos, 0x00, sizeof(enc_ctx->encoded_buf_base_pos));

#ifdef VPU_ADD_SEQ_HEADER
	enc_ctx->seq_backup = NULL;
	enc_ctx->seq_len = 0;
#endif

	DSTATUS("[%s][%d] id:%d ++++ \n", __FUNCTION__, __LINE__, enc_ctx->encoder_id);
	
	return 0;

err:
	LOGE("vpu_env_open error");
	
	return -1;

}


static void vpu_env_close(venc_context_t* enc_ctx)
{
	int ret;
//	int nb_drv;

#ifdef VPU_ADD_SEQ_HEADER	
	if(enc_ctx->seq_backup != NULL)
	{
		free(enc_ctx->seq_backup);
		enc_ctx->seq_backup = NULL;
		enc_ctx->seq_len = 0;
	}
#endif
	DSTATUS("[%s][%d] id:%d ----\n", __FUNCTION__, __LINE__, enc_ctx->encoder_id);
	
	max_width -= enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iPicWidth;
	max_height -= enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iPicHeight;
	if(max_width < 64 || max_height < 64)
	{
		max_width = 64;
		max_height = 64;
	}

	vpu_update_sizeinfo(enc_ctx->vpu_mgr_fd, max_width, max_height);
	
	if(enc_ctx->vpu_enc_fd >= 0)
	{
		vpu_encoder_drv_close(enc_ctx->vpu_enc_fd);
	}
	enc_ctx->vpu_enc_fd = -1;

	if(enc_ctx->vpu_mgr_fd >= 0)
	{
		ret = close(enc_ctx->vpu_mgr_fd);
		if(ret < 0)
		{
			LOGE("%s close error[%s]", VPU_MGR_NAME, strerror(errno));
		}
	}
	enc_ctx->vpu_mgr_fd = -1;	
	
	DSTATUS("[%s][%d] id:%d ++++, remain n drv:%d\n", __FUNCTION__, __LINE__, enc_ctx->encoder_id, vpu_num_of_used_drv());
}

int put_data_size( unsigned int *pData, int putSize)
{
	unsigned int *DestAddr;
	unsigned char	*lOutPutBuffer = NULL;

	DestAddr = (unsigned int *)pData;
	lOutPutBuffer = (unsigned char *)pData;

	if(DestAddr[0] == 0x01000000)
	{
		lOutPutBuffer[0] =  (putSize >> 24)& 0x000000FF;
		lOutPutBuffer[1] =  (putSize >> 16)& 0x000000FF;
		lOutPutBuffer[2] =  (putSize >> 8)& 0x000000FF;
		lOutPutBuffer[3] =  (putSize)& 0x000000FF;
	}
	else
	{
		// error code를 추가해야 함. vpu data가 비정상 적이거나
		// pData의 address 가 잘못 되었을 경우
		//printf("__%d__ %s nal statart code error\n", __LINE__, __func__);

	}

    return 0;
}

static int venc_cmd_process(venc_context_t* enc_ctx, int cmd, void* args)
{
	int ret;
	int success = 0;
	struct pollfd tcc_event[1];

	if(enc_ctx->vpu_enc_fd < 0)
	{
		DSTATUS("vpu ioctl bad descripter, fd:%d\n", enc_ctx->vpu_enc_fd);
		return RETCODE_INVALID_HANDLE;
	}
	
	if(ioctl(enc_ctx->vpu_enc_fd, cmd, args) < 0)
	{
		DSTATUS("vpu ioctl err[%s] : cmd = 0x%x\n", strerror(errno), cmd);
	}

	while (1) {
		int ret;
		memset(tcc_event, 0, sizeof(tcc_event));
		tcc_event[0].fd = enc_ctx->vpu_enc_fd;
		tcc_event[0].events = POLLIN;
		ret = poll((struct pollfd *)&tcc_event, 1, 3000); // 100 msec
		if (ret < 0) {
			DSTATUS("vpu poll error\n");
			break;
		}else if (ret == 0) {
			DSTATUS("vpu poll timeout\n");
			break;
		}else if (ret > 0) {
			if (tcc_event[0].revents & POLLERR) {
				DSTATUS("vpu poll POLLERR\n");
				break;
			} else if (tcc_event[0].revents & POLLIN) {
				success = 1;
				break;
			}
		}
	}
	/* todo */
	if(!success)
	{
		DSTATUS("VENC command poll timeout.!!\n");
		return RETCODE_CODEC_EXIT;
	}

	switch(cmd)
	{
		case V_ENC_INIT:
			{
			 	VENC_INIT_t* init_info = args;

				ioctl(enc_ctx->vpu_enc_fd, V_ENC_INIT_RESULT, args);
				ret = init_info->result;
			}
			break;

		case V_ENC_PUT_HEADER:
			{
			 	VENC_PUT_HEADER_t* buff_info = args;

				ioctl(enc_ctx->vpu_enc_fd, V_ENC_PUT_HEADER_RESULT, args);
				ret = buff_info->result;
			}
			break;

		case V_ENC_ENCODE:
			{
			 	VENC_ENCODE_t* encoded_info = args;

				ioctl(enc_ctx->vpu_enc_fd, V_ENC_ENCODE_RESULT, args);
				ret = encoded_info->result;
			}
			break;

		case V_ENC_REG_FRAME_BUFFER:
		case V_ENC_CLOSE:
		default:
			ioctl(enc_ctx->vpu_enc_fd, V_ENC_GENERAL_RESULT, &ret);
			break;
	}

	if(!success || (ret == RETCODE_CODEC_EXIT))
	{
		if(cmd == V_ENC_ENCODE)
		{
			DSTATUS("VENC has to reset.!! 0x%x\n", ret);
//			ioctl(vpu_mgr_fd, VPU_HW_RESET, (void*)NULL);
			vpu_all_drv_reset_needed = 1;
			return RETCODE_CODEC_EXIT;
		}
	}
	
	if(vpu_all_drv_reset_needed == 1 && cmd != V_ENC_INIT  && cmd != V_ENC_REG_FRAME_BUFFER && cmd != V_ENC_CLOSE)
	{
		DSTATUS(" CMD:%d vpu all VENC drv have to reset.!! ret:0x%x\n", cmd, ret);
		return RETCODE_CODEC_EXIT;
	}
	
	return ret;
}

int venc_vpu( int iOpCode, int* pHandle, void* pParam1, void* pParam2 )
{
	venc_context_t* enc_ctx = NULL;
	int ret = 0;

	//DSTATUS("iOpCode:%d, pHandle:0x%x\n", iOpCode, pHandle);

	if( iOpCode == VENC_INIT )
	{
		venc_init_t* p_init_param = (venc_init_t*)pParam1;

		mgr_mutex_count++;
		if(mgr_mutex_count == 1)
		{
			pthread_mutex_init(&mgr_mutex, NULL);
			DSTATUS("[VENC_K] manager mutex initialized!!\n");
		}

		MGR_LOCK();
		
		enc_ctx = (venc_context_t*)calloc(1, sizeof(venc_context_t));
		if(enc_ctx == NULL)
		{
			MGR_UNLOCK();
			return -VPU_ENV_INIT_ERROR;
		}

		enc_ctx->encoder_id = p_init_param->encoder_id;		
		if(vpu_env_open(enc_ctx, p_init_param->m_iPicWidth, p_init_param->m_iPicHeight ) < 0)
		{
			free(enc_ctx);
			MGR_UNLOCK();
			return -VPU_ENV_INIT_ERROR;
		}

		*pHandle = (int)enc_ctx;
		enc_ctx->bAvcUsedNALStart = *((unsigned int*)pParam2);
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_RegBaseVirtualAddr = (unsigned int)NULL;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iBitstreamFormat = p_init_param->m_iBitstreamFormat;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iPicWidth = p_init_param->m_iPicWidth;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iPicHeight = p_init_param->m_iPicHeight;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iFrameRate = p_init_param->m_iFrameRate;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iTargetKbps = p_init_param->m_iTargetKbps;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iKeyInterval = p_init_param->m_iKeyInterval;// only first picture is I

		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iUseSpecificRcOption = 1;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iAvcFastEncoding 	= p_init_param->m_iAvcFastEncoding;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iPicQpY 			= -1;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iIntraMBRefresh 	= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iDeblkDisable 		= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iDeblkAlpha 		= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iDeblkBeta 		= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iDeblkChQpOffset 	= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iConstrainedIntra 	= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iVbvBufferSize 	= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iSearchRange 		= 2;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iPVMDisable 		= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iWeightIntraCost 	= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iRCIntervalMode 	= 1;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iRCIntervalMBNum 	= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_stRcInit.m_iEncQualityLevel	= p_init_param->m_iEncodingQualityLevel; /* 1 ~ 35 */
		
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_bEnableVideoCache				= 0;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_bCbCrInterleaveMode			= p_init_param->m_bChromaInterleaved;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_uiEncOptFlags					= 0; //(1 << 10 );

		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_bEnableVideoCache	= 0;

		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_Memcpy = (void* (*) ( void*, const void*, unsigned int ))memcpy;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_Memset = (void  (*) ( void*, int, unsigned int ))memset;
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_Interrupt = (int  (*) ( void ))NULL;

		//enc_ctx->large_stream_buff_size = vpu_get_stream_buffer_size(p_init_param->m_iPicWidth, p_init_param->m_iPicHeight);
		enc_ctx->encoded_buf_size = (enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iTargetKbps/8 /*KB/s*/) * 1024/*Byte*/ * 4 + HEX_1MB;
		
		//------------------------------------------------------------
		//! [x] bitstream buffer for each VPU decoder
		//------------------------------------------------------------
		
    	enc_ctx->gsBitstreamBufSize = enc_ctx->encoded_buf_size;
		enc_ctx->gsBitstreamBufSize = ALIGNED_BUFF( enc_ctx->gsBitstreamBufSize, 4*1024 );
		enc_ctx->gsBitstreamBufAddr[PA] = (codec_addr_t)vpu_sys_malloc_physical_addr(enc_ctx->vpu_enc_fd, &enc_ctx->gsBitstreamBufAddr[K_VA], enc_ctx->gsBitstreamBufSize, 0 );
		if(enc_ctx->gsBitstreamBufAddr[PA] == 0)
		{
			LOGE( "[VENC:%d] bitstream_buf_addr[PA] malloc() failed \n", enc_ctx->encoder_id);
			MGR_UNLOCK();
			return -1;
		}
       	DSTATUS( "[VENC:%d] bitstream_buf_addr[PA] = 0x%x, size:0x%x(%.2fMb) \n", enc_ctx->encoder_id, (codec_addr_t)enc_ctx->gsBitstreamBufAddr[PA], enc_ctx->gsBitstreamBufSize, (float)enc_ctx->gsBitstreamBufSize/(float)HEX_1MB);

		enc_ctx->gsBitstreamBufAddr[VA] = (codec_addr_t)vpu_sys_malloc_virtual_addr(enc_ctx->vpu_enc_fd, NULL, enc_ctx->gsBitstreamBufAddr[PA], enc_ctx->gsBitstreamBufSize );
		if(enc_ctx->gsBitstreamBufAddr[VA] == 0)
		{
			LOGE( "[VENC:%d] bitstream_buf_addr[VA] malloc() failed \n", enc_ctx->encoder_id);
			MGR_UNLOCK();
			return -1;
		}
		//memset( (void*)gsBitstreamBufAddr[VA], 0x00 , gsBitstreamBufSize);
		DSTATUS("[VENC:%d] bitstream_buf_addr[VA] = 0x%x, 0x%x \n", enc_ctx->encoder_id, (codec_addr_t)enc_ctx->gsBitstreamBufAddr[VA], enc_ctx->gsBitstreamBufSize );

		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_BitstreamBufferAddr = enc_ctx->gsBitstreamBufAddr[PA];
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_BitstreamBufferAddr_VA = enc_ctx->gsBitstreamBufAddr[K_VA];
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iBitstreamBufferSize = enc_ctx->gsBitstreamBufSize;

		//------------------------------------------------------------
		//! [x] code buffer, work buffer and parameter buffer for VPU
		//------------------------------------------------------------
		enc_ctx->gsBitWorkBufSize = WORK_CODE_PARA_BUF_SIZE;
		enc_ctx->gsBitWorkBufSize = ALIGNED_BUFF(enc_ctx->gsBitWorkBufSize, 4*1024);
		enc_ctx->gsBitWorkBufAddr[PA] = (codec_addr_t)vpu_sys_malloc_physical_addr(enc_ctx->vpu_enc_fd, &enc_ctx->gsBitWorkBufAddr[K_VA], enc_ctx->gsBitWorkBufSize, BUFFER_WORK  );
		if(enc_ctx->gsBitWorkBufAddr[PA] == 0 )
		{
			LOGE( "[VENC:%d] gsBitWorkBufAddr[PA] malloc() failed \n", enc_ctx->encoder_id);
			MGR_UNLOCK();
			return -1;
		}
		
		DSTATUS("[VENC:%d] gsBitWorkBufAddr[PA] = 0x%x, size:0x%x(%.2fMb)\n", enc_ctx->encoder_id, (codec_addr_t)enc_ctx->gsBitWorkBufAddr[PA], enc_ctx->gsBitWorkBufSize, (float)enc_ctx->gsBitWorkBufSize/(float)HEX_1MB);
		enc_ctx->gsBitWorkBufAddr[VA] = (codec_addr_t)vpu_sys_malloc_virtual_addr(enc_ctx->vpu_enc_fd, NULL, enc_ctx->gsBitWorkBufAddr[PA], enc_ctx->gsBitWorkBufSize);
		if(enc_ctx->gsBitWorkBufAddr[VA] == 0)
		{
			LOGE( "[VENC] gsBitWorkBufAddr[VA] malloc() failed \n");
			MGR_UNLOCK();
			return -1;
		}
		DSTATUS("[VENC:%d] gsBitWorkBufAddr[VA] = 0x%x, 0x%x \n", enc_ctx->encoder_id, (codec_addr_t)enc_ctx->gsBitWorkBufAddr[VA], enc_ctx->gsBitWorkBufSize);

		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_BitWorkAddr[PA]		= enc_ctx->gsBitWorkBufAddr[PA];
		enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_BitWorkAddr[VA]		= enc_ctx->gsBitWorkBufAddr[K_VA];

		ret = venc_cmd_process(enc_ctx, V_ENC_INIT, &enc_ctx->gsVpuEncInit_Info);
		if( ret != RETCODE_SUCCESS )
		{
			LOGE( "[VENC:%d,Err:%d] venc_vpu VPU_ENC_INIT failed \n", enc_ctx->encoder_id, ret );
			MGR_UNLOCK();
			return -VPU_ENCODER_CMD_INIT_FAILED;
		}
		DSTATUS("[VENC:%d] venc_vpu VPU_ENC_INIT ok! ret %d\n", enc_ctx->encoder_id, ret);

		//------------------------------------------------------------
		//! [x] Register frame buffers requested by the encoder.
		//------------------------------------------------------------
		enc_ctx->gsFrameBufSize = enc_ctx->gsVpuEncInit_Info.gsVpuEncInitialInfo.m_iMinFrameBufferCount * enc_ctx->gsVpuEncInit_Info.gsVpuEncInitialInfo.m_iMinFrameBufferSize;
		enc_ctx->gsFrameBufSize = ALIGNED_BUFF(enc_ctx->gsFrameBufSize, 4*1024);
		enc_ctx->gsFrameBufAddr[PA] = (codec_addr_t)vpu_sys_malloc_physical_addr(enc_ctx->vpu_enc_fd, &enc_ctx->gsFrameBufAddr[K_VA], enc_ctx->gsFrameBufSize, 0);
		if(enc_ctx->gsFrameBufAddr[PA] == 0)
		{
			LOGE( "[VENC:%d,Err:%d] venc_vpu gsFrameBufAddr[PA](0x%x) alloc failed \n", enc_ctx->encoder_id, ret, enc_ctx->gsFrameBufSize );
			MGR_UNLOCK();
			return -ret;
		}

		DSTATUS("[VENC:%d] min buffer count:%d, min buffer size:0x%x(%.2fMb)\n", enc_ctx->encoder_id, enc_ctx->gsVpuEncInit_Info.gsVpuEncInitialInfo.m_iMinFrameBufferCount, enc_ctx->gsVpuEncInit_Info.gsVpuEncInitialInfo.m_iMinFrameBufferSize, (float)enc_ctx->gsVpuEncInit_Info.gsVpuEncInitialInfo.m_iMinFrameBufferSize/(float)HEX_1MB);
		DSTATUS("[VENC:%d] gsFrameBufAddr[PA] = 0x%x, size:0x%x(%.2fMb)\n", enc_ctx->encoder_id, (codec_addr_t)enc_ctx->gsFrameBufAddr[PA], enc_ctx->gsFrameBufSize, (float)enc_ctx->gsFrameBufSize/(float)HEX_1MB);
		enc_ctx->gsFrameBufAddr[VA] = (codec_addr_t)vpu_sys_malloc_virtual_addr(enc_ctx->vpu_enc_fd, NULL, enc_ctx->gsFrameBufAddr[PA], enc_ctx->gsFrameBufSize);
		if(enc_ctx->gsFrameBufAddr[VA] == 0)
		{
			LOGE( "[VENC:%d,Err:%d] venc_vpu gsFrameBufAddr[VA] alloc failed \n", enc_ctx->encoder_id, ret );
			MGR_UNLOCK();
			return -ret;
		}
		DSTATUS("[VENC:%d] gsFrameBufAddr[VA] = 0x%x, gsFrameBufAddr[K_VA] = 0x%x \n", enc_ctx->encoder_id, (codec_addr_t)enc_ctx->gsFrameBufAddr[VA], enc_ctx->gsFrameBufAddr[K_VA] );

		enc_ctx->gsVpuEncBuffer_Info.gsVpuEncBuffer.m_FrameBufferStartAddr[PA] = enc_ctx->gsFrameBufAddr[PA];
		enc_ctx->gsVpuEncBuffer_Info.gsVpuEncBuffer.m_FrameBufferStartAddr[VA] = enc_ctx->gsFrameBufAddr[K_VA];

		ret = venc_cmd_process(enc_ctx, V_ENC_REG_FRAME_BUFFER, &enc_ctx->gsVpuEncBuffer_Info);  // register frame buffer
		if( ret != RETCODE_SUCCESS )
		{
			LOGE( "[VENC:%d,Err:%d] venc_vpu VPU_ENC_REG_FRAME_BUFFER failed \n", enc_ctx->encoder_id, ret );
			MGR_UNLOCK();
			return -ret;
		}

		enc_ctx->encoded_buf_base_pos[PA] 	= enc_ctx->gsBitstreamBufAddr[PA];
		enc_ctx->encoded_buf_base_pos[VA] 	= enc_ctx->gsBitstreamBufAddr[VA];
		enc_ctx->encoded_buf_base_pos[K_VA]	= enc_ctx->gsBitstreamBufAddr[K_VA];
		
		enc_ctx->encoded_buf_cur_pos[PA] 	= enc_ctx->encoded_buf_base_pos[PA];
		enc_ctx->encoded_buf_cur_pos[VA] 	= enc_ctx->encoded_buf_base_pos[VA];
		enc_ctx->encoded_buf_cur_pos[K_VA] 	= enc_ctx->encoded_buf_base_pos[K_VA];
		
		enc_ctx->encoded_buf_end_pos[PA] 	= enc_ctx->encoded_buf_base_pos[PA] + enc_ctx->encoded_buf_size;
		enc_ctx->encoded_buf_end_pos[VA] 	= enc_ctx->encoded_buf_base_pos[VA] + enc_ctx->encoded_buf_size;
		enc_ctx->encoded_buf_end_pos[K_VA] 	= enc_ctx->encoded_buf_base_pos[K_VA] + enc_ctx->encoded_buf_size;

		//printf("[%s][%d] encoded_buf_base_pos base_pos[VA]:0x%x\n", __FUNCTION__, __LINE__, enc_ctx->encoded_buf_base_pos[VA]);

		//DSTATUS("OUT-Buffer ::	%d Kbps, %d fps, %d sec !!\n", enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iTargetKbps, enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iFrameRate, (VIDEO_ENC_BUFFER_COUNT/enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iFrameRate +3 /*add 3sec*/ ));
		//DSTATUS("               PA = 0x%x, VA = 0x%x, size:0x%x(%.2fMb)\n", enc_ctx->encoded_buf_base_pos[PA], enc_ctx->encoded_buf_base_pos[VA],	enc_ctx->encoded_buf_size, (float)enc_ctx->encoded_buf_size/(float)HEX_1MB);

		vpu_all_drv_reset_needed = 0;
		DSTATUS("[VENC:%d] venc_vpu VPU_ENC_REG_FRAME_BUFFER ok! \n", enc_ctx->encoder_id);
		MGR_UNLOCK();
	}
	else if( iOpCode == VENC_SEQ_HEADER )
	{
		venc_context_t* enc_ctx = (venc_context_t*)pHandle;
		venc_seq_header_t* p_seq_param = (venc_seq_header_t*)pParam1;
		codec_addr_t m_SeqHeaderBuffer_VA = 0;

		if(enc_ctx == NULL)
        	return -RETCODE_INVALID_HANDLE;

		//enc_ctx->mux_type = ((int*)pParam2);
        enc_ctx->mux_type = (int)((int*)pParam2);
		if(p_seq_param->m_SeqHeaderBuffer[PA] == 0)
		{
			DSTATUS( "[VENC:%d] gspSeqHeader_Buffer = 0x%x, 0x%x \n", enc_ctx->encoder_id, (codec_addr_t)enc_ctx->gsBitstreamBufAddr[PA], enc_ctx->gsBitstreamBufSize);
			p_seq_param->m_SeqHeaderBuffer[PA] = enc_ctx->encoded_buf_base_pos[PA];
			p_seq_param->m_SeqHeaderBuffer[VA] = enc_ctx->encoded_buf_base_pos[K_VA];
			p_seq_param->m_iSeqHeaderBufferSize = enc_ctx->encoded_buf_size; //FIXME:
		}
		
		if( enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iBitstreamFormat == STD_MPEG4 )
		{
			enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderType = MPEG4_VOL_HEADER;
			enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_HeaderAddr  = p_seq_param->m_SeqHeaderBuffer[PA];
			enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize = p_seq_param->m_iSeqHeaderBufferSize;

			DSTATUS( "[VENC:%d] VPU_ENC_PUT_HEADER for MPEG4_VOL_HEADER \n", enc_ctx->encoder_id);
			ret = venc_cmd_process(enc_ctx, V_ENC_PUT_HEADER, &enc_ctx->gsVpuEncPutHeader_Info);

			if( ret != RETCODE_SUCCESS )
			{
				DSTATUS( "[VENC:%d:Err:%d] venc_vpu MPEG4_VOL_HEADER failed \n", enc_ctx->encoder_id, ret );
				return -ret;
			}
		}
		else if( enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iBitstreamFormat == STD_AVC )
		{
			if(enc_ctx->gsiSeqHeaderCnt == 0)
			{
				m_SeqHeaderBuffer_VA = enc_ctx->gsBitstreamBufAddr[VA];
				
				enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderType = AVC_SPS_RBSP;
				enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_HeaderAddr  = p_seq_param->m_SeqHeaderBuffer[PA];
				enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize = p_seq_param->m_iSeqHeaderBufferSize;

				DSTATUS( "[VENC:%d] VPU_ENC_PUT_HEADER for AVC_SPS_RBSP \n", enc_ctx->encoder_id);
				ret = venc_cmd_process(enc_ctx, V_ENC_PUT_HEADER, &enc_ctx->gsVpuEncPutHeader_Info);
				if( ret != RETCODE_SUCCESS )
				{
					LOGE( "[VENC:%d:Err:%d] venc_vpu AVC_SPS_RBSP failed \n", enc_ctx->encoder_id, ret );
					return -ret;
				}

				if(enc_ctx->bAvcUsedNALStart)
				{
					unsigned char* buffer = (unsigned char*)m_SeqHeaderBuffer_VA;
					DPRINTF_FRAME("[VENC:%d]SPS(%d) :: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", enc_ctx->encoder_id, 
										enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize,
                        				(unsigned int)buffer[0], (unsigned int)buffer[1], (unsigned int)buffer[2], (unsigned int)buffer[3], (unsigned int)buffer[4]);
					if(enc_ctx->mux_type)	//	mp4
					{
						put_data_size( (unsigned int *)m_SeqHeaderBuffer_VA, enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize-4);
					}
				}
				else
				{
					enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize -= 4;
					memmove((void *)m_SeqHeaderBuffer_VA, (void *)m_SeqHeaderBuffer_VA+4, enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize);
				}

				enc_ctx->gsiSzSeqHeader[enc_ctx->gsiSeqHeaderCnt] = enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize;
			}
			else
			{
				unsigned int tmp_pps_addr[2];
				tmp_pps_addr[PA] = enc_ctx->encoded_buf_base_pos[PA];
				tmp_pps_addr[VA] = enc_ctx->encoded_buf_base_pos[VA];

				m_SeqHeaderBuffer_VA = ALIGNED_BUFF(tmp_pps_addr[VA] + enc_ctx->gsiSzSeqHeader[enc_ctx->gsiSeqHeaderCnt-1], 4*1024);

				enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderType = AVC_PPS_RBSP;
				enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_HeaderAddr  = ALIGNED_BUFF(tmp_pps_addr[PA] + enc_ctx->gsiSzSeqHeader[enc_ctx->gsiSeqHeaderCnt-1], 4*1024);
				enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize = p_seq_param->m_iSeqHeaderBufferSize;

				DSTATUS( "[VENC:%d] VPU_ENC_PUT_HEADER for AVC_PPS_RBSP \n", enc_ctx->encoder_id);
				ret = venc_cmd_process(enc_ctx, V_ENC_PUT_HEADER, &enc_ctx->gsVpuEncPutHeader_Info);
				if( ret != RETCODE_SUCCESS )
				{
					LOGE( "[VENC:%d:Err:%d] venc_vpu AVC_SPS_RBSP failed \n", enc_ctx->encoder_id, ret );
					return -ret;
				}

				if(enc_ctx->bAvcUsedNALStart)
				{
					unsigned char* buffer = (unsigned char*)m_SeqHeaderBuffer_VA;
					DPRINTF_FRAME("[VENC:%d]PPS(%d) :: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", enc_ctx->encoder_id, enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize,
											(unsigned int)buffer[0], (unsigned int)buffer[1], (unsigned int)buffer[2], (unsigned int)buffer[3], (unsigned int)buffer[4]);
															
					if(enc_ctx->mux_type)	//	mp4
					{
						put_data_size( (unsigned int *)m_SeqHeaderBuffer_VA, enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize-4);
					}					
				}
				else
				{
					enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize -= 4;
					memmove((void *)m_SeqHeaderBuffer_VA, (void *)m_SeqHeaderBuffer_VA+4, enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize-4);
				}

				//attaching the pps behind the sps
				memmove((void*)(enc_ctx->encoded_buf_cur_pos[VA] + enc_ctx->gsiSzSeqHeader[enc_ctx->gsiSeqHeaderCnt-1]), (void*)m_SeqHeaderBuffer_VA, enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize);
				enc_ctx->gsiSzSeqHeader[enc_ctx->gsiSeqHeaderCnt] = enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize;
			}
		} //end if

		// output
		p_seq_param->m_pSeqHeaderOut = (unsigned char*)enc_ctx->encoded_buf_cur_pos[VA];

		if( enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iBitstreamFormat == STD_MPEG4 )
		{
#ifdef VPU_ADD_SEQ_HEADER		
			if(enc_ctx->seq_backup == NULL)
			{
				enc_ctx->seq_backup = (unsigned char*)malloc(enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize + (STABILITY_GAP*2));
			}
			memcpy(enc_ctx->seq_backup, (void *)enc_ctx->encoded_buf_cur_pos[VA], enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize);
			enc_ctx->seq_len = enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize;
#endif

			enc_ctx->encoded_buf_cur_pos[PA] += ALIGNED_BUFF(enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize + (STABILITY_GAP*2), 4*1024);
			enc_ctx->encoded_buf_cur_pos[VA] += ALIGNED_BUFF(enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize + (STABILITY_GAP*2), 4*1024);
			enc_ctx->encoded_buf_cur_pos[K_VA] += ALIGNED_BUFF(enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize + (STABILITY_GAP*2), 4*1024);
		}
		else
		{
			if(enc_ctx->gsiSeqHeaderCnt == 1)
			{
#ifdef VPU_ADD_SEQ_HEADER			
				if(enc_ctx->seq_backup == NULL)
				{
					enc_ctx->seq_backup = (unsigned char*)malloc(enc_ctx->gsiSzSeqHeader[0] + enc_ctx->gsiSzSeqHeader[1] + (STABILITY_GAP*2));
				}
				
				memcpy(enc_ctx->seq_backup, (void *)enc_ctx->encoded_buf_cur_pos[VA], enc_ctx->gsiSzSeqHeader[0] + enc_ctx->gsiSzSeqHeader[1]);
				enc_ctx->seq_len = enc_ctx->gsiSzSeqHeader[0] + enc_ctx->gsiSzSeqHeader[1];
#endif
				enc_ctx->encoded_buf_cur_pos[PA] += ALIGNED_BUFF(enc_ctx->gsiSzSeqHeader[0] + enc_ctx->gsiSzSeqHeader[1] + (STABILITY_GAP*2), 4*1024);
				enc_ctx->encoded_buf_cur_pos[VA] += ALIGNED_BUFF(enc_ctx->gsiSzSeqHeader[0] + enc_ctx->gsiSzSeqHeader[1] + (STABILITY_GAP*2), 4*1024);
				enc_ctx->encoded_buf_cur_pos[K_VA] += ALIGNED_BUFF(enc_ctx->gsiSzSeqHeader[0] + enc_ctx->gsiSzSeqHeader[1] + (STABILITY_GAP*2), 4*1024);
			}
		}

		p_seq_param->m_iSeqHeaderOutSize = enc_ctx->gsVpuEncPutHeader_Info.gsVpuEncHeader.m_iHeaderSize;
		enc_ctx->gsiSeqHeaderCnt++;
	}
	else if( iOpCode == VENC_ENCODE )
	{
		venc_context_t* enc_ctx = (venc_context_t*)pHandle;
		venc_input_t* p_input_param = (venc_input_t*)pParam1;
		venc_output_t* p_output_param = (venc_output_t*)pParam2;

		if(enc_ctx == NULL)
        	return -RETCODE_INVALID_HANDLE;

		if(p_input_param->m_iChangeFrameRate != 0)
		{
			enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_iChangeRcParamFlag = p_input_param->m_iChangeRcParamFlag;
			enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_iChangeFrameRate = p_input_param->m_iChangeFrameRate;
		}		

		if(p_input_param->m_iChangeKeyInterval != 0)
		{
			enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_iChangeKeyInterval = p_input_param->m_iChangeKeyInterval;
			enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iKeyInterval = p_input_param->m_iChangeKeyInterval;
		}		
			
		if(enc_ctx->gsiFrameIdx >= (unsigned int)enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iKeyInterval)
		{
			enc_ctx->gsiFrameIdx = 0;
			p_input_param->request_IntraFrame = 1; //make IDR frame
		}

		if(p_input_param->request_IntraFrame == 1)
		{
			enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_iForceIPicture = 1;//set 1 For IDR-Type I-Frame without P-Frame!!
		}
		else
		{
			enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_iForceIPicture = 0;
		}

		enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_iSkipPicture = 0;
		if( enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iTargetKbps == 0 ) // no rate control
		{
			enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_iQuantParam = 23;
		}
		else
		{
			enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_iQuantParam = 10;
		}


		/*
			output buffer setting
			if reach the end address, cur_address have to be initialized to base_pos
		*/
		if((enc_ctx->encoded_buf_cur_pos[PA] + HEX_1MB) > enc_ctx->encoded_buf_end_pos[PA])
		{
			enc_ctx->encoded_buf_cur_pos[PA] = enc_ctx->encoded_buf_base_pos[PA];
			enc_ctx->encoded_buf_cur_pos[VA] = enc_ctx->encoded_buf_base_pos[VA];
			enc_ctx->encoded_buf_cur_pos[K_VA] = enc_ctx->encoded_buf_base_pos[K_VA];
		}

#ifdef VPU_ADD_SEQ_HEADER
		if(/*p_input_param->request_IntraFrame == 1*/
			(enc_ctx->gsiFrameIdx != 0 && ((enc_ctx->gsiFrameIdx%enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iFrameRate) == 0)) &&
			enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iSliceMode != 1)
		{
			/* make spaces in front of encoded buffer */
			enc_ctx->encoded_buf_cur_pos[PA] = enc_ctx->encoded_buf_base_pos[PA] + ALIGNED_BUFF(enc_ctx->seq_len, 4*1024);
			enc_ctx->encoded_buf_cur_pos[VA] = enc_ctx->encoded_buf_base_pos[VA] + ALIGNED_BUFF(enc_ctx->seq_len, 4*1024);
			enc_ctx->encoded_buf_cur_pos[K_VA] = enc_ctx->encoded_buf_base_pos[K_VA] + ALIGNED_BUFF(enc_ctx->seq_len, 4*1024);
		}
#endif

		/* output address */
		enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_BitstreamBufferAddr =  (codec_addr_t)enc_ctx->encoded_buf_cur_pos[PA];
		enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_iBitstreamBufferSize = enc_ctx->encoded_buf_end_pos[PA] - enc_ctx->encoded_buf_cur_pos[PA];

		enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_PicYAddr = (codec_addr_t)p_input_param->m_pInputY;
		enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_PicCbAddr = (codec_addr_t)p_input_param->m_pInputCbCr[0];
		enc_ctx->gsVpuEncInOut_Info.gsVpuEncInput.m_PicCrAddr = (codec_addr_t)p_input_param->m_pInputCbCr[1];

//		LOGD(" 0x%x-0x%x-0x%x, %d-%d-%d, %d-%d-%d, 0x%x-%d", gsVpuEncInOut_Info.gsVpuEncInput.m_PicYAddr, gsVpuEncInOut_Info.gsVpuEncInput.m_PicCbAddr, gsVpuEncInOut_Info.gsVpuEncInput.m_PicCrAddr,
//				gsVpuEncInOut_Info.gsVpuEncInput.m_iForceIPicture, gsVpuEncInOut_Info.gsVpuEncInput.m_iSkipPicture, gsVpuEncInOut_Info.gsVpuEncInput.m_iQuantParam,
//				gsVpuEncInOut_Info.gsVpuEncInput.m_iChangeRcParamFlag, gsVpuEncInOut_Info.gsVpuEncInput.m_iChangeTargetKbps, gsVpuEncInOut_Info.gsVpuEncInput.m_iChangeFrameRate,
//				gsVpuEncInOut_Info.gsVpuEncInput.m_BitstreamBufferAddr, gsVpuEncInOut_Info.gsVpuEncInput.m_iBitstreamBufferSize);
		ret = venc_cmd_process(enc_ctx, V_ENC_ENCODE, &enc_ctx->gsVpuEncInOut_Info);

//		LOGD("systemtime:: encoded frame");

		if( ret != RETCODE_SUCCESS )
		{
			if( ret == RETCODE_WRAP_AROUND )
			{
				DSTATUS("[VENC:%d] Warning!! BitStream buffer wrap arounded. prepare more large buffer = %d \n", enc_ctx->encoder_id, ret );
			}

			if(ret == RETCODE_CODEC_EXIT)
			{
				DSTATUS("[VENC:%d] VPU RESET is needed, ret:%d \n", enc_ctx->encoder_id, ret );
			}
			
			DSTATUS("[VENC:%d:Err:%d]fd:%d VPU_ENC_ENCODE failed \n", enc_ctx->encoder_id, ret, enc_ctx->vpu_enc_fd);
			return -ret;
		}

		// output
		if( enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iBitstreamFormat == STD_AVC)
		{
			// modified by shmin for M2TS
			if(enc_ctx->bAvcUsedNALStart)
			{
				p_output_param->m_pBitstreamOut 	= (unsigned char*)enc_ctx->encoded_buf_cur_pos[VA];
				p_output_param->m_iBitstreamOutSize = enc_ctx->gsVpuEncInOut_Info.gsVpuEncOutput.m_iBitstreamOutSize;

				if( enc_ctx->mux_type)	//	mp4
				{
					put_data_size( (unsigned int *)p_output_param->m_pBitstreamOut, p_output_param->m_iBitstreamOutSize-4);
				}
			}
			else
			{
				p_output_param->m_pBitstreamOut 	= (unsigned char*)enc_ctx->encoded_buf_cur_pos[VA];
				p_output_param->m_pBitstreamOut 	+= 4;
				p_output_param->m_iBitstreamOutSize = enc_ctx->gsVpuEncInOut_Info.gsVpuEncOutput.m_iBitstreamOutSize - 4;
			}
		}
		else
		{
			p_output_param->m_pBitstreamOut 	= (unsigned char*)enc_ctx->encoded_buf_cur_pos[VA];
			p_output_param->m_iBitstreamOutSize = enc_ctx->gsVpuEncInOut_Info.gsVpuEncOutput.m_iBitstreamOutSize;
		}

       	//  printf("[VENC_K] m_pBitstreamOut : 0x%x, size:%d bytes\n", (unsigned int)p_output_param->m_pBitstreamOut,  p_output_param->m_iBitstreamOutSize);

#ifdef VPU_ADD_SEQ_HEADER
		if(/*p_input_param->request_IntraFrame == 1*/
			enc_ctx->gsiFrameIdx != 0 && ((enc_ctx->gsiFrameIdx%enc_ctx->gsVpuEncInit_Info.gsVpuEncInit.m_iFrameRate) == 0)
			)
		{
			/* copy sequence header */
			p_output_param->m_pBitstreamOut -= enc_ctx->seq_len;
			memcpy(p_output_param->m_pBitstreamOut, enc_ctx->seq_backup, enc_ctx->seq_len);
			p_output_param->m_iBitstreamOutSize += enc_ctx->seq_len;
		}
#endif

        /* increase cur_pos address */
		enc_ctx->encoded_buf_cur_pos[PA] += ALIGNED_BUFF(enc_ctx->gsVpuEncInOut_Info.gsVpuEncOutput.m_iBitstreamOutSize + (STABILITY_GAP*2), 4*1024);
		enc_ctx->encoded_buf_cur_pos[VA] += ALIGNED_BUFF(enc_ctx->gsVpuEncInOut_Info.gsVpuEncOutput.m_iBitstreamOutSize + (STABILITY_GAP*2), 4*1024);
		enc_ctx->encoded_buf_cur_pos[K_VA] += ALIGNED_BUFF(enc_ctx->gsVpuEncInOut_Info.gsVpuEncOutput.m_iBitstreamOutSize + (STABILITY_GAP*2), 4*1024);

		p_output_param->m_iPicType = enc_ctx->gsVpuEncInOut_Info.gsVpuEncOutput.m_iPicType;

		enc_ctx->gsiFrameIdx++;
	}
	else if( iOpCode == VENC_CLOSE )
	{
		int temp_id;
		venc_context_t* enc_ctx = (venc_context_t*)pHandle;
        if(enc_ctx == NULL)
        	return -RETCODE_INVALID_HANDLE;

		MGR_LOCK();

		ret = venc_cmd_process(enc_ctx, V_ENC_CLOSE, NULL);
		if( ret != RETCODE_SUCCESS )
		{
			DSTATUS( "[VENC:%d] VPU_ENC_CLOSE failed Error code is 0x%x \n", enc_ctx->encoder_id, ret );
		}

		if( enc_ctx->gsBitstreamBufAddr[VA] )
		{
			if(vpu_sys_free_virtual_addr( NULL, (void*)enc_ctx->gsBitstreamBufAddr[VA], enc_ctx->gsBitstreamBufSize ) >= 0)
				enc_ctx->gsBitstreamBufAddr[VA] = 0;
		}

		if( enc_ctx->gsBitWorkBufAddr[VA] )
		{
			if(vpu_sys_free_virtual_addr( NULL, (void*)enc_ctx->gsBitWorkBufAddr[VA], enc_ctx->gsBitWorkBufSize ) >= 0)
				enc_ctx->gsBitWorkBufAddr[VA] = 0;
		}
		if( enc_ctx->gsMESearchBufAddr[VA] )
		{
			if(vpu_sys_free_virtual_addr( NULL, (void*)enc_ctx->gsMESearchBufAddr[VA], enc_ctx->gsMESearchBufSize ) >= 0)
				enc_ctx->gsMESearchBufAddr[VA] = 0;
		}
		if( enc_ctx->gsFrameBufAddr[VA] )
		{
			if(vpu_sys_free_virtual_addr( NULL, (void*)enc_ctx->gsFrameBufAddr[VA], enc_ctx->gsFrameBufSize ) >= 0)
				enc_ctx->gsFrameBufAddr[VA] = 0;
		}

		vpu_env_close(enc_ctx);
		enc_ctx->gsiFrameIdx = 0;
		temp_id = enc_ctx->encoder_id;
		
		free(enc_ctx);
		 
		MGR_UNLOCK();
		
		mgr_mutex_count--;
		if(mgr_mutex_count == 0)
		{
			pthread_mutex_destroy(&mgr_mutex);
			DSTATUS("[VENC:%d] manager mutex destroyed!!\n", temp_id);
		}
	
        DSTATUS( "[VENC:%d] VPU_ENC_CLOSE end --- \n", temp_id);       
	}
	else
	{
		DSTATUS( "[VENC] Invalid Operation!!\n" );
		return -ret;
	}

	return ret;
}

unsigned char *venc_getBitstreamBufAddr(void* pHandle, unsigned int index)
{
	venc_context_t* enc_ctx = (venc_context_t*)pHandle;
	unsigned char *pBitstreamBufAddr = NULL;

	if (index == PA)
	{
		pBitstreamBufAddr = (unsigned char *)enc_ctx->gsBitstreamBufAddr[PA];
	}
	else if (index == VA)
	{
		pBitstreamBufAddr = (unsigned char *)enc_ctx->gsBitstreamBufAddr[VA];
	}
	else /* default : PA */
	{
		pBitstreamBufAddr = (unsigned char *)enc_ctx->gsBitstreamBufAddr[PA];
	}

	return pBitstreamBufAddr;
}


unsigned char *venc_getStreamOutPhyAddr(void* pHandle, unsigned char *convert_addr, unsigned int base_index)
{
	venc_context_t* enc_ctx = (venc_context_t*)pHandle;
	unsigned char *pBaseAddr;
	unsigned char *pTargetBaseAddr = NULL;
	unsigned int szAddrGap = 0;

	pTargetBaseAddr = (unsigned char*)enc_ctx->encoded_buf_base_pos[PA];

	if (base_index == K_VA)
	{
		pBaseAddr = (unsigned char*)enc_ctx->encoded_buf_base_pos[K_VA];
	}
	else /* default : VA */
	{
		pBaseAddr = (unsigned char*)enc_ctx->encoded_buf_base_pos[VA];
	}

	szAddrGap = convert_addr - pBaseAddr;

	return (pTargetBaseAddr+szAddrGap);
}

int vpu_hw_reset(int mgr_fd)
{
	DSTATUS("[%s][%d] VPU H/W reset\n", __FUNCTION__, __LINE__);
	return ioctl(mgr_fd, VPU_HW_RESET, (void*)NULL);
}
