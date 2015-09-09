/**
  @file encoder.c
  
  This component implements All Video encoder. (MPEG4/AVC/H.263)

  Copyright (C) 2009-2011 Telechips Inc.

  Date: 2011/02/25 13:33:29
  Author $Author: B070371 (ZzaU)
*/


#include "encoder.h"
#include "venc.h"
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

#define LOG_TAG	"DIRECT_ENC"

#define	LOGD		printf
#define	LOGE		printf
#define	LOGI		printf

static int DEBUG_ON  = 1;
#define DBug_MSG(msg...)	if (DEBUG_ON) { LOGD( ": " msg);/* sleep(1);*/}
#define DBG_LINE()	printf("[%s][%d]\n", __FUNCTION__, __LINE__);

//#define CHANGE_BITRATE  //to change bitrate.
#ifdef CHANGE_BITRATE
#define REQUEST_INTRAR_EFRESH  //to request I-Frame.
#endif

//#define	TCC_VENC_MUTEX

#ifdef  TCC_VENC_MUTEX
#define LOCK(x)		pthread_mutex_lock(x)
#define UNLOCK(x)	pthread_mutex_unlock(x)
#else
#define LOCK(x)		
#define UNLOCK(x)	

#endif

/*
	when vpu clock assigned to max, sequence header initialization are frequently failed. so we retry initialization until succeed. this is just a stopgap.
*/
#define VPU_SEQUENCE_HEADER_INIT_NAX_RETRY_COUNT		10


/***********************************************************/
//INTERNAL VARIABLE
/***********************************************************/
//INTERNAL VARIABLE
typedef struct enc_private_data 
{
//enc operation
	venc_init_t	gsVEncInit;
	venc_seq_header_t gsVEncSeqHeader;
	venc_input_t gsVEncInput;
	venc_output_t gsVEncOutput;
	unsigned char isVPUClosed;

//info
	unsigned char iConfigDataFlag;
	unsigned char video_coding_type;
	int	curr_frameRate;
	int	curr_targetKbps;	
	int	use_NalStartCode;
	unsigned int total_count;
	int	bChromaInterleaved; /* 1:chroma interleaved 0:chroma saparate */
	
//error process
	unsigned char isEncError;
	int retry_count;

//for venc_k
	void* venc_context;

	int	tid;
}tENC_PRIVATE;

//for multi-thread safety
//static pthread_mutex_t gsEncMutex;
static int number_of_encoder = 0; //number of opened encoder.
/***********************************************************/
//EXTERNAL FUNCTION

/*!
 ***********************************************************************
 * \brief
 *		ENCODER_INIT	: initial function of video encoder
 * \param
 *		[in] pInit			: pointer of encoder initial parameters 
 * \return
 *		If successful, ENCODER_INIT returns 0 or plus. Otherwise, it returns a minus value.
 ***********************************************************************
 */
void* ENCODER_INIT(tENC_INIT_PARAMS *pInit)
{
	tENC_PRIVATE *enc_private = NULL;
	int ret = 0;
	int priv_address;

	if(number_of_encoder == 0)
	{
	
#ifdef  TCC_VENC_MUTEX	
		pthread_mutex_init(&gsEncMutex, NULL);
		printf("Global Mutex Init!!!\n");
#endif
	}

	number_of_encoder++;
	
	LOCK(&gsEncMutex);	
	
	enc_private = calloc(1, sizeof(tENC_PRIVATE));
	if(enc_private == NULL)
	{
		return NULL;
	}

//variable init
	memset(enc_private, 0x00, sizeof(tENC_PRIVATE));
	enc_private->isVPUClosed = 1;
	enc_private->tid = number_of_encoder;

	DBug_MSG("Encoder ID:%d\n", enc_private->tid);
	
	switch(pInit->codecFormat)
	{
		case CODEC_FORMAT_H263:	 enc_private->video_coding_type = STD_H263;  	break;
		case CODEC_FORMAT_MPEG4: enc_private->video_coding_type = STD_MPEG4;	break;
		case CODEC_FORMAT_H264:  enc_private->video_coding_type = STD_AVC;		break;
		default:
			return NULL;
	}

	enc_private->gsVEncInit.m_iBitstreamFormat	= enc_private->video_coding_type;
	enc_private->gsVEncInit.m_iPicWidth		= pInit->picWidth;
	enc_private->gsVEncInit.m_iPicHeight 		= pInit->picHeight;
	enc_private->gsVEncInit.m_iFrameRate 		= enc_private->curr_frameRate  = pInit->frameRate;
	enc_private->gsVEncInit.m_iTargetKbps		= enc_private->curr_targetKbps = pInit->targetKbps;
	enc_private->gsVEncInit.m_iKeyInterval		= pInit->keyFrameInterval;
	enc_private->gsVEncInit.m_iAvcFastEncoding	= 0;
	enc_private->gsVEncInit.m_iSliceMode		= pInit->sliceMode;
	enc_private->gsVEncInit.m_iSliceSizeMode	= pInit->sliceSizeMode;
	enc_private->gsVEncInit.m_iSliceSize		= pInit->sliceSize * 8; // to change to bits.
	enc_private->gsVEncInit.m_bChromaInterleaved = pInit->bChromaInterleaved;
	enc_private->gsVEncInit.m_iEncodingQualityLevel = pInit->bEncodingQualityLevel;
	enc_private->gsVEncInit.encoder_id			= enc_private->tid;
	
	enc_private->bChromaInterleaved				= pInit->bChromaInterleaved;
	enc_private->use_NalStartCode 				= pInit->use_NalStartCode;
	
	enc_private->total_count = 0;
	enc_private->retry_count = 0;
	
//	LOGI("ENC Info :: %dx%d, %d bps, %d interval, %d Kbps\n", enc_private->gsVEncInit.m_iPicWidth, enc_private->gsVEncInit.m_iPicHeight, 
//					enc_private->gsVEncInit.m_iFrameRate, enc_private->gsVEncInit.m_iKeyInterval, enc_private->gsVEncInit.m_iTargetKbps);

	//init
	if(1)
	{
		venc_init_t* enc = &enc_private->gsVEncInit; 
		printf("VPU init =================================================================\n");
		printf("[id:%d]m_RegBaseVirtualAddr:%d\n", enc_private->tid, enc->m_RegBaseVirtualAddr);
		printf("[id:%d]m_iPicWidth:%d\n", enc_private->tid,enc->m_iPicWidth);
		printf("[id:%d]m_iPicHeight:%d\n", enc_private->tid,enc->m_iPicHeight);
		printf("[id:%d]m_iFrameRate:%d\n", enc_private->tid,enc->m_iFrameRate);
		printf("[id:%d]m_iTargetKbps:%d\n", enc_private->tid,enc->m_iTargetKbps);
		printf("[id:%d]m_iKeyInterval:%d\n", enc_private->tid,enc->m_iKeyInterval);
		printf("[id:%d]m_iAvcFastEncoding:%d\n", enc_private->tid,enc->m_iAvcFastEncoding);
		printf("[id:%d]m_BitstreamBufferAddr:%d\n", enc_private->tid,enc->m_BitstreamBufferAddr);
		printf("[id:%d]m_BitstreamBufferAddr_VA:%d\n", enc_private->tid,enc->m_BitstreamBufferAddr_VA);
		printf("[id:%d]m_iBitstreamBufferSize:%d\n", enc_private->tid,enc->m_iBitstreamBufferSize);
		printf("[id:%d]m_bChromaInterleaved:%d\n", enc_private->tid,enc->m_bChromaInterleaved);
		printf("[id:%d]m_iEncodingQualityLevel:%d\n", enc_private->tid,enc->m_iEncodingQualityLevel);
		printf("[id:%d]m_iSliceMode:%d\n", enc_private->tid,enc->m_iSliceMode);
		printf("[id:%d]m_iSliceSizeMode:%d\n", enc_private->tid,enc->m_iSliceSizeMode);
		printf("[id:%d]m_iSliceSize:%d\n", enc_private->tid,enc->m_iSliceSize);
		printf("\n====================================================================\n");
	}
	
	if((ret = venc_vpu(VENC_INIT, &priv_address, &enc_private->gsVEncInit, (void*)&enc_private->use_NalStartCode )) < 0 )
	{
		LOGE( "[%s][%d][Err:%d][id:%d] VENC_INIT failed", __FUNCTION__, __LINE__, ret, enc_private->tid);

		if(ret != -VPU_ENV_INIT_ERROR)
		{
			venc_vpu( VENC_CLOSE, enc_private->venc_context, NULL, NULL );
			enc_private->isVPUClosed = 1;
		}

		UNLOCK(&gsEncMutex);
		return NULL;
	}

	enc_private->venc_context = (void*)priv_address;
	enc_private->isVPUClosed = 0;

	UNLOCK(&gsEncMutex);

	return enc_private;
}

/*!
 ***********************************************************************
 * \brief
 *		ENCODER_CLOSE	: close function of video encoder
 ***********************************************************************
 */
int ENCODER_CLOSE(void* enc_context)
{
	int ret= 0;
	tENC_PRIVATE* enc_private = (tENC_PRIVATE*)enc_context;

	LOCK(&gsEncMutex);
	
	if(	enc_private->isVPUClosed == 0)
	{
		if( (ret = venc_vpu( VENC_CLOSE, enc_private->venc_context, NULL, NULL )) < 0 )
		{
			LOGE( "[%s][%d][Err:%d][id:%d] VENC_CLOSE failed\n", __FUNCTION__, __LINE__, ret, enc_private->tid);
		}
		enc_private->isVPUClosed = 1;
	}
	
	if(enc_private)
	{
		free(enc_private);
		enc_private = NULL;
	}	

	number_of_encoder--;
	UNLOCK(&gsEncMutex);
	
	if(number_of_encoder == 0)
	{
#ifdef  TCC_VENC_MUTEX		
		pthread_mutex_destroy(&gsEncMutex);
		printf("Global Mutex Destroy!!\n");
#endif
		
	}
	return ret;
}

/*!
 ***********************************************************************
 * \brief
 *		ENCODER_ENC	: encode function of video encoder
 * \param
 *		[in] pInput			: pointer of encoder frame input parameters  
 * \param
 *		[out] pOutput			: pointer of encoder frame output parameters  
 * \return
 *		If successful, ENCODER_ENC returns 0 or plus. Otherwise, it returns a minus value.
 ***********************************************************************
 */
int ENCODER_ENC(void* enc_context, tENC_FRAME_INPUT *pInput, tENC_FRAME_OUTPUT *pOutput)
{	
//	int i;
	signed int ret = -1;
//	unsigned int *InputCurrBuffer;
    tENC_PRIVATE* enc_private = NULL;

VPU_SEQUENCE_RETRY:

	LOCK(&gsEncMutex);

    enc_private = (tENC_PRIVATE*)enc_context;

	memset(pOutput, 0x00, sizeof(tENC_FRAME_OUTPUT));

	if(enc_private->isEncError == 1)
	{
		int priv_address;

		if(enc_private->isVPUClosed == 0)
		{	
			venc_vpu( VENC_CLOSE, enc_private->venc_context, NULL, NULL );
			enc_private->venc_context = NULL;
			enc_private->isVPUClosed = 1;
		}
		
		LOGD("[id:%d] Restore Encode\n", enc_private->tid);
		if( (ret = venc_vpu(VENC_INIT, &priv_address, &enc_private->gsVEncInit, (void*)&enc_private->use_NalStartCode )) < 0 )
		{
			DBug_MSG( "[%s][%d][Err:%d][id:%d] VENC_INIT failed\n", __FUNCTION__, __LINE__, ret, enc_private->tid);
			if(ret != (-VPU_ENV_INIT_ERROR))
			{
				enc_private->venc_context = (void*)priv_address;
				enc_private->isVPUClosed = 0;
			}
			
			goto ERR_PROCESS;
		}
		
		enc_private->venc_context = (void*)priv_address;
		enc_private->isVPUClosed = 0;
		enc_private->iConfigDataFlag = 0;
		enc_private->isEncError = 0;
	}

	if(enc_private->isVPUClosed == 1)
	{
		DBug_MSG("[id:%d]Vpu already is closed because of Error!!\n", enc_private->tid);
		goto ERR_PROCESS;
	}

//Send the first output buffer such as CodecSpecificData
	if(enc_private->iConfigDataFlag == 0)
	{
		enc_private->gsVEncSeqHeader.m_SeqHeaderBuffer[PA]	= 0;
		enc_private->gsVEncSeqHeader.m_SeqHeaderBuffer[VA]	= 0;
		enc_private->gsVEncSeqHeader.m_iSeqHeaderBufferSize = 0;
		
		if(enc_private->video_coding_type == STD_MPEG4) 
		{		
			ret = venc_vpu( VENC_SEQ_HEADER, enc_private->venc_context, &enc_private->gsVEncSeqHeader, (void *)pInput->muxType  );
			if( ret < 0 )
			{
				LOGD( "[Err%d][id:%d VENC_SEQ_HEADER failed\n", ret, enc_private->tid);
				UNLOCK(&gsEncMutex);
				goto RETRY_SEQUENCE;
			}

			pOutput->outputStreamAddr = enc_private->gsVEncSeqHeader.m_pSeqHeaderOut;
			pOutput->headerLen = enc_private->gsVEncSeqHeader.m_iSeqHeaderOutSize;
			pOutput->nTimeStamp = 0;
			//pOutputBuffer->nFlags |= OMX_BUFFERFLAG_CODECCONFIG;
			//pOutputBuffer->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
			DBug_MSG("[id:%d] VOL Header :: Length = %d !!\n", enc_private->tid, pOutput->headerLen);
		}
		else if(enc_private->video_coding_type == STD_AVC) 
		{
			ret = venc_vpu( VENC_SEQ_HEADER, enc_private->venc_context, &enc_private->gsVEncSeqHeader, (void *)pInput->muxType  );
			if( ret < 0 )
			{
				LOGD( "[Err%d][id:%d] VENC_SPS_SEQ_HEADER failed, ctx:0x%p\n", ret, enc_private->tid, enc_private->venc_context);
				UNLOCK(&gsEncMutex);
				goto RETRY_SEQUENCE;
			}
			DBug_MSG("[id:%d] SPS - %d !!\n", enc_private->tid, enc_private->gsVEncSeqHeader.m_iSeqHeaderOutSize);		

			pOutput->outputStreamAddr = enc_private->gsVEncSeqHeader.m_pSeqHeaderOut;
			pOutput->headerLen = enc_private->gsVEncSeqHeader.m_iSeqHeaderOutSize;
			pOutput->nTimeStamp = 0;
			//pOutputBuffer->nFlags |= OMX_BUFFERFLAG_CODECCONFIG;
			//pOutputBuffer->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
			ret = venc_vpu( VENC_SEQ_HEADER, enc_private->venc_context, &enc_private->gsVEncSeqHeader, (void *)pInput->muxType  );
			if( ret < 0 )
			{
				LOGD( "[Err%d][id:%d] VENC_PPS_SEQ_HEADER failed, ctx:0x%p\n", ret, enc_private->tid, enc_private->venc_context);
				UNLOCK(&gsEncMutex);
				goto RETRY_SEQUENCE;
			}
			DBug_MSG("[id:%d]PPS - %d !!\n", enc_private->tid, enc_private->gsVEncSeqHeader.m_iSeqHeaderOutSize);		
			pOutput->headerLen +=  enc_private->gsVEncSeqHeader.m_iSeqHeaderOutSize;
			DBug_MSG("[id:%d]CodecConfig Data :: %d !!\n", enc_private->tid, pOutput->headerLen);
		}
		
		if(0) //sequence header
		{
			int ii;
			printf("[id:%d] len:%d\n", enc_private->tid, enc_private->gsVEncSeqHeader.m_iSeqHeaderOutSize);
			for(ii=0; ii<pOutput->headerLen; ii++)
			{
				printf("0x%x ", pOutput->outputStreamAddr[ii]);
			}
			printf("\n");
		}
		enc_private->iConfigDataFlag = 1;
	}
	else
	{
		enc_private->gsVEncInput.m_bCbCrInterleaved = enc_private->bChromaInterleaved;

		enc_private->gsVEncInput.m_pInputY 		 	= (unsigned char*)pInput->inputStreamAddr;
		enc_private->gsVEncInput.m_pInputCbCr[0] 	= enc_private->gsVEncInput.m_pInputY + (enc_private->gsVEncInit.m_iPicWidth*enc_private->gsVEncInit.m_iPicHeight);
		enc_private->gsVEncInput.m_pInputCbCr[1] 	= enc_private->gsVEncInput.m_pInputCbCr[0] + (enc_private->gsVEncInit.m_iPicWidth*enc_private->gsVEncInit.m_iPicHeight/4);
		
#ifdef CHANGE_BITRATE
		unsigned int change_period = 25;
		if(enc_private->total_count == change_period*10)
			enc_private->total_count = change_period;

		if(enc_private->total_count == change_period)
		{
			enc_private->VideoConfigBitRateType.nEncodeBitrate = 896*1024;
		}
		else if(enc_private->total_count == change_period*2)
		{
			enc_private->VideoConfigBitRateType.nEncodeBitrate = 768*1024;
		}
		else if(enc_private->total_count == change_period*3)
		{
			enc_private->VideoConfigBitRateType.nEncodeBitrate = 640*1024;
		}
		else if(enc_private->total_count == change_period*4)
		{
			enc_private->VideoConfigBitRateType.nEncodeBitrate = 512*1024;
		}
		else if(enc_private->total_count == change_period*5)
		{
			enc_private->VideoConfigBitRateType.nEncodeBitrate = 384*1024;
		}
		else if(enc_private->total_count == change_period*6)
		{
			enc_private->VideoConfigBitRateType.nEncodeBitrate = 256*1024;
		}
		else if(enc_private->total_count == change_period*7)
		{
			enc_private->VideoConfigBitRateType.nEncodeBitrate = 192*1024;
		}
		else if(enc_private->total_count == change_period*8)
		{
			enc_private->VideoConfigBitRateType.nEncodeBitrate = 128*1024;
		}
		else if(enc_private->total_count == change_period*9)
		{
			enc_private->VideoConfigBitRateType.nEncodeBitrate = 64*1024;
		}

#ifdef REQUEST_INTRAR_EFRESH
		if(enc_private->total_count%17 == 0 ){
			enc_private->VideoIFrame.IntraRefreshVOP = 1;
		}
#endif
#endif
		enc_private->total_count++;	
		
#ifdef REMOVE_RC_AUTO_SKIP
		enc_private->gsVEncInput.m_iChangeRcParamFlag = (0x20 | 0x1);
#else
		enc_private->gsVEncInput.m_iChangeRcParamFlag = 0;
#endif
	
		if(pInput->targetKbps != 0 && pInput->targetKbps != enc_private->curr_targetKbps)
		{
			enc_private->curr_targetKbps = pInput->targetKbps;
			
			enc_private->gsVEncInput.m_iChangeRcParamFlag |= 0x2;
			enc_private->gsVEncInput.m_iChangeTargetKbps = enc_private->curr_targetKbps;
			DBug_MSG("[%d]Bitrate- Change(%d kbps)\n", enc_private->tid, enc_private->gsVEncInput.m_iChangeTargetKbps);
		}

		if(pInput->frameRate != 0 && pInput->frameRate != enc_private->curr_frameRate)
		{
			enc_private->curr_frameRate = pInput->frameRate;
			
			enc_private->gsVEncInput.m_iChangeRcParamFlag |= 0x4;
			enc_private->gsVEncInput.m_iChangeFrameRate = enc_private->curr_frameRate;
			enc_private->gsVEncInput.m_iChangeKeyInterval = enc_private->curr_frameRate;
			DBug_MSG("[%d]FrameRate- Change(%d fps), keyframe interval:%d\n", enc_private->tid, enc_private->gsVEncInput.m_iChangeFrameRate, enc_private->gsVEncInput.m_iChangeKeyInterval);
		}
		else
		{
			enc_private->gsVEncInput.m_iChangeFrameRate = 0;
			enc_private->gsVEncInput.m_iChangeKeyInterval = 0;
		}

		if(pInput->isForceIFrame){
			enc_private->gsVEncInput.request_IntraFrame = 1;
			DBug_MSG("[%d]IntraRefreshVOP\n", enc_private->tid);
		}
		else{
			enc_private->gsVEncInput.request_IntraFrame = 0;
		}

		ret = venc_vpu( VENC_ENCODE, enc_private->venc_context, &enc_private->gsVEncInput, &enc_private->gsVEncOutput );
		if(ret < 0)
		{
			LOGE( "[%s][%d][id:%d] retry VENC_ENCODE\n", __FUNCTION__, __LINE__, enc_private->tid);
			goto ERR_PROCESS;
		}

		pOutput->frameLen = enc_private->gsVEncOutput.m_iBitstreamOutSize;
		pOutput->outputStreamAddr = enc_private->gsVEncOutput.m_pBitstreamOut;
		pOutput->picType = enc_private->gsVEncOutput.m_iPicType;		
		pOutput->nTimeStamp = pInput->nTimeStamp;

		pOutput->m_iSliceCount = enc_private->gsVEncOutput.m_iSliceCount;
		pOutput->m_pSliceInfo = enc_private->gsVEncOutput.m_pSliceInfo;

#if 0
		if(pOutput->m_iSliceCount > 1 && DEBUG_ON == 1)
		{
			unsigned char *p = pOutput->outputStreamAddr;
			unsigned int *pSliceSize = pOutput->m_pSliceInfo;
			unsigned int total_bytes = 0;
			
			for( i=0; i<pOutput->m_iSliceCount; i++ ) 
			{
				if(enc_private->total_count < 5 )
				{
					DBug_MSG( "[id:%d][%2d/%2d] %d bits, %d bytes ", enc_private->tid, i, pOutput->m_iSliceCount, pSliceSize[i]*8, pSliceSize[i] );
					DBug_MSG( " 	 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", p[total_bytes+0], p[total_bytes+1], p[total_bytes+2], 
							 p[total_bytes+3], p[total_bytes+4], p[total_bytes+5], p[total_bytes+6], p[total_bytes+7]);
					total_bytes += pSliceSize[i];
					DBug_MSG( "  ~	 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", p[total_bytes-8], p[total_bytes-7], p[total_bytes-6], 
							 p[total_bytes-5], p[total_bytes-4], p[total_bytes-3], p[total_bytes-2], p[total_bytes-1]);
				}
			}
		}
#endif
	
#if 1
		 // IFrame (SyncFrame)
		if( (enc_private->gsVEncOutput.m_iPicType == VENC_PIC_TYPE_I) && pOutput->frameLen > 0)
		{
			//DBug_MSG("[id:%d]I-Frame for Sync :: Frm_size = %d !!\n", enc_private->tid, pOutput->frameLen);

			//This flag is set when the buffer content contains a coded sync frame 
			//pOutputBuffer->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
		}
#endif
		//Attach the end of frame flag while sending out the last piece of output buffer
		//pOutputBuffer->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
	}

	UNLOCK(&gsEncMutex);
	
	return 0;
	
ERR_PROCESS:
	enc_private->isEncError = 1;
	UNLOCK(&gsEncMutex);
	return ret;

RETRY_SEQUENCE:
	DBug_MSG("[%s][%d][id:%d] retry count:%d, ctx:0x%p\n\n", __FUNCTION__, __LINE__, enc_private->tid, enc_private->retry_count, enc_private->venc_context);

	enc_private->isEncError = 1;

	if(enc_private->retry_count++ < VPU_SEQUENCE_HEADER_INIT_NAX_RETRY_COUNT)
	{
		DBug_MSG("[%s][%d][id:%d] retry count:%d\n", __FUNCTION__, __LINE__, enc_private->tid, enc_private->retry_count);

		enc_private->iConfigDataFlag = 0;
		goto VPU_SEQUENCE_RETRY;
	}
	return -1;
}

unsigned char* ENCODER_BITSTREAM_BUFF_ADDR(void* enc_context, unsigned int index)
{
	unsigned char* ret_addr = NULL;
	tENC_PRIVATE* enc_private = NULL;

	LOCK(&gsEncMutex);
    enc_private = (tENC_PRIVATE*)enc_context;	
	ret_addr = venc_getBitstreamBufAddr(enc_private->venc_context, index);
	UNLOCK(&gsEncMutex);
	
	return ret_addr;
}

unsigned char* ENCODER_OUT_BUFF_PADDR(void* enc_context, unsigned char *convert_addr, unsigned int base_index)
{
	unsigned char* ret_addr = NULL;
	tENC_PRIVATE* enc_private = NULL;

	LOCK(&gsEncMutex);
    enc_private = (tENC_PRIVATE*)enc_context;
	ret_addr = venc_getStreamOutPhyAddr(enc_private->venc_context, convert_addr, base_index);
	UNLOCK(&gsEncMutex);
	
	return ret_addr;
}



