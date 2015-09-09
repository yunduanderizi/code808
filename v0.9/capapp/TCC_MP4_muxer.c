#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "TCC_MP4_muxer.h"

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

//#define CAMCORDER_BUFFER_SIZE 	CAPTURE_BUFF_SIZE+JPEG_DATA_BUFF_SIZE
#define MUX_WRITE_CACHE_SIZE          512*1024
#define MUX_VIDEO_CACHE_SIZE          512*4*1024
#define MUX_AUDIO_CACHE_SIZE          512*4*1024

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
unsigned char Mux_TCC_MP4_StreamType(unsigned char VideoCodecType, unsigned char AudioCodecType);


#define MUX_UUID_NUMBER	0
#if (MUX_UUID_NUMBER > 0)
static unsigned char sz_userdata[MUX_UUID_NUMBER][32] = {         
	{0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10},
	{0x8,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10},
	{0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10},
	{0x9,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10},
};
#endif


/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
int AVCodec_AAC_TCC_EncDSI(unsigned long usSampleRate, unsigned long ucChannels,unsigned char *bitstream, unsigned int *size)
{
	 const unsigned int usAACSRMap[] =    { 96000, 88200, 64000, 48000,
	          44100, 32000, 24000, 22050,
	          16000, 12000, 11025, 8000, 7350 };
	 unsigned sampling_frequency_index;
	 
	 for(sampling_frequency_index = 0; sampling_frequency_index < 13; sampling_frequency_index ++)
	 {
		  if(usSampleRate == usAACSRMap[sampling_frequency_index])
		  {
			   bitstream[0] = 0x10 | ((sampling_frequency_index & 0x0F) >> 1);
			   bitstream[1] = ((sampling_frequency_index & 0x01) << 7) | ((ucChannels & 0x0F) << 3);
			   *size = 2;
			   return 1;
		  }
	 }
	 
	 return -1;
}
 

/*
**---------------------------------------------------------------------------
**  Function Define
**---------------------------------------------------------------------------
*/
static int malloc_count = 0;
void* my_malloc(unsigned int size)
{
	malloc_count++;
	return malloc(size);
}

void* my_realloc(void *p, int size)
{
	
	return realloc(p, size);
}

void my_free(void* ptr)
{
	malloc_count--;
	//printf("[%s][%d] malloc count:%d\n", __FUNCTION__, __LINE__, malloc_count);
	return free(ptr);
}

TCC_MP4_MUXER_CONTEXT_t* Mux_TCC_MP4_EncOpen(unsigned char *FileName, unsigned char VideoCodecType, 
													unsigned char AudioCodecType, unsigned long	 SamplePerSec, 
													unsigned long BitsPerSample, unsigned long Channels,
                                                        unsigned int Width, unsigned int Height,char *MPEG4DSI,unsigned int MPEG4DSI_Size,
                                                        char *XMuxAACDSI,unsigned int XMuxAACDSI_Size, unsigned int isUserData)
{	
    if(XMuxAACDSI_Size || isUserData || XMuxAACDSI)
    {
        printf("isUserData XMuxAACDSI_Size");
    }
    
	int ret = 0;

	TCC_MP4_MUXER_CONTEXT_t* mp4_ctx = NULL;
	mp4_ctx = (TCC_MP4_MUXER_CONTEXT_t*)malloc(sizeof(TCC_MP4_MUXER_CONTEXT_t));
	if(mp4_ctx == NULL)
	{
		printf("3gp muxer init alloc failed!\n");
		return NULL;
	}
	
	memset(mp4_ctx, 0x00, sizeof(TCC_MP4_MUXER_CONTEXT_t));
	
	// callback functions for memory
	mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnMalloc 	=(void*(*) ( unsigned int ))my_malloc;
	mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnFree 		=(void (*) ( void* ))my_free;
	mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnMemcmp 	=(int(*) (const void*,const void*,unsigned int))memcmp;
	mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnMemcpy 	=(void*(*) ( void*, const void*, unsigned int ))memcpy;
	mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnMemmove 	=(void*(*) (void*, const void*, unsigned int))memmove;
	mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnMemset 	=(void*(*) ( void*, int, unsigned int ))memset;
	mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnRealloc 	=(void*(*) ( void*, unsigned int ))my_realloc;
	// callback functions for file
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFopen 		=(void*(*) (const char *, const char *))fopen;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFread 		=(unsigned int (*) (void*, unsigned int, unsigned int, void* ))fread;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFseek 		=(int(*) (void*, long, int ))fseek;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFtell 		=(long(*) (void* ))ftell;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFclose 		=(int(*) (void *))fclose;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFeof 		=(unsigned int(*) (void*))feof;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFflush 		=(unsigned int(*) (void*))fflush;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFwrite 		=(unsigned int(*) (const void*, unsigned int, unsigned int, void*))fwrite;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnUnlink 		=(int(*) (const char *))unlink;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFseek64 	=(int(*) (void*, S64, int ))fseeko;
	mp4_ctx->mux_init.m_stFileFuncs.m_pfnFtell64 	=(S64(*) (void* ))ftello;
	mp4_ctx->mux_init.m_pszOutFileName 				= (char *)FileName;
	// specific information
	mp4_ctx->mux_init.m_stCache.m_ulTmpFileVideoCacheSize = MUX_VIDEO_CACHE_SIZE;
	mp4_ctx->mux_init.m_stCache.m_ulTmpFileAudioCacheSize = MUX_AUDIO_CACHE_SIZE;
	mp4_ctx->mux_init.m_stCache.m_ulWriteCacheSize = MUX_WRITE_CACHE_SIZE;
	mp4_ctx->mux_init.m_stCache.m_pbyWriteCache = (unsigned char*) (*mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnMalloc)
															( mp4_ctx->mux_init.m_stCache.m_ulWriteCacheSize +
															mp4_ctx->mux_init.m_stCache.m_ulTmpFileAudioCacheSize +
															mp4_ctx->mux_init.m_stCache.m_ulTmpFileVideoCacheSize );

	//ASSERT(mux_init.m_stCache.m_pbyWriteCache);

	mp4_ctx->mux_init.m_stCache.m_pszTmpFilePath = NULL;
	mp4_ctx->mux_init.m_stCache.m_pfnAppendByCluster = NULL;
#ifdef FS_FILE_APPEND_SUPPORT
	mp4_ctx->mux_init.m_stCache.m_pfnAppendByCluster =(int(*) (unsigned int, unsigned int, unsigned int))CCD_PAL_AppendHandleByCluster;
#endif
	mp4_ctx->mux_init.m_ulStreamTypeAll = Mux_TCC_MP4_StreamType(VideoCodecType , AudioCodecType);

	if( isUserData)
	{
		mp4_ctx->mux_init.m_ulOptions |= MP4MUXOPT_WRITE_USERDATA_PER_FRAME;
		//printf("__%d__ %s, MP4MUXOPT_WRITE_USERDATA_PER_FRAME\n", __LINE__, __func__ );
	}


	// ex) Mon October 5, 16:05:18, 2009
	mp4_ctx->mux_putinfo.m_stFileInfo.m_stTime.m_lDayOfMonth = 5;
	mp4_ctx->mux_putinfo.m_stFileInfo.m_stTime.m_lMonth = 9;
	mp4_ctx->mux_putinfo.m_stFileInfo.m_stTime.m_lDayOfWeek = 1;
	mp4_ctx->mux_putinfo.m_stFileInfo.m_stTime.m_lHour = 17;
	mp4_ctx->mux_putinfo.m_stFileInfo.m_stTime.m_lMin = 5;
	mp4_ctx->mux_putinfo.m_stFileInfo.m_stTime.m_lSec = 18;
	mp4_ctx->mux_putinfo.m_stFileInfo.m_stTime.m_lYear = 2009;

	mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulWidth = Width;
	mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulHeight = Height;
	mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulFrameRate = 1000;
	if(VideoCodecType == CCD_MPEG4_MODE)
	{
		mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulFourCC = FOURCC_mp4v;
		mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulCodecPrivateSize = MPEG4DSI_Size;
		mp4_ctx->mux_putinfo.m_stVideoInfo.m_pbyCodecPrivate = (unsigned char *)MPEG4DSI;
	}
	else if(VideoCodecType == CCD_H263_MODE)
	{
		mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulFourCC = FOURCC_h263;
		mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulCodecPrivateSize = 0;
		mp4_ctx->mux_putinfo.m_stVideoInfo.m_pbyCodecPrivate = NULL;
	}
	else if(VideoCodecType == CCD_H264_MODE)
	{
		mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulFourCC = FOURCC_avc1;
		mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulCodecPrivateSize = MPEG4DSI_Size;
		mp4_ctx->mux_putinfo.m_stVideoInfo.m_pbyCodecPrivate = (unsigned char *)MPEG4DSI;
	}
	
	if(AudioCodecType == CCD_AAC_MODE)
	{	
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulSamplePerSec 	= SamplePerSec;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulBitsPerSample 	= BitsPerSample;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulChannels 	= Channels;
		
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulFormatTag = AV_AUDIO_AAC;
		
		AVCodec_AAC_TCC_EncDSI(mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulSamplePerSec, mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulChannels, mp4_ctx->MuxAudioDSI,&mp4_ctx->MuxAudioDSI_Size);
		
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulCodecPrivateSize = mp4_ctx->MuxAudioDSI_Size;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_pbyCodecPrivate = mp4_ctx->MuxAudioDSI;
	}
	else if(AudioCodecType == CCD_MP3_MODE)
	{	
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulSamplePerSec 	= SamplePerSec;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulBitsPerSample 	= BitsPerSample;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulChannels 	= Channels;
		
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulFormatTag = AV_AUDIO_MP3;
		
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulCodecPrivateSize = mp4_ctx->MuxAudioDSI_Size;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_pbyCodecPrivate = mp4_ctx->MuxAudioDSI;
	}
	else if(AudioCodecType == CCD_PCM_MODE)
	{	
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulSamplePerSec 	= SamplePerSec;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulBitsPerSample 	= BitsPerSample;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulChannels 	= Channels;
		
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulFormatTag = AV_AUDIO_MS_PCM;
		
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulCodecPrivateSize = mp4_ctx->MuxAudioDSI_Size;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_pbyCodecPrivate = mp4_ctx->MuxAudioDSI;
	}
	else if(AudioCodecType == CCD_AMRNB_MODE)
	{	
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulSamplePerSec = 8000;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulBitsPerSample = 16;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulChannels = 1;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulFormatTag = AV_AUDIO_AMR_NB;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulCodecPrivateSize = 0;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_pbyCodecPrivate = NULL;
	}
	else if(AudioCodecType == CCD_QCELP_MODE)
	{	
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulSamplePerSec = 8000;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulBitsPerSample = 16;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulChannels = 1;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulFormatTag = AV_AUDIO_QCELP;
		
		AVCodec_AAC_TCC_EncDSI(mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulSamplePerSec, mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulChannels, mp4_ctx->MuxAudioDSI,&mp4_ctx->MuxAudioDSI_Size);
		
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulCodecPrivateSize = mp4_ctx->MuxAudioDSI_Size;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_pbyCodecPrivate = mp4_ctx->MuxAudioDSI;

	}	
	else if(AudioCodecType == CCD_EVRC_MODE)
	{	
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulSamplePerSec = 8000;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulBitsPerSample = 16;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulChannels = 1;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulFormatTag = AV_AUDIO_EVRC;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_ulCodecPrivateSize = 0;
		mp4_ctx->mux_putinfo.m_stAudioInfo.m_pbyCodecPrivate = NULL;
	}	

#if 0
// put user data info
	#if (MUX_UUID_NUMBER > 0)
	{
		unsigned long udidx = 0;
		unsigned long alloc_size = 0;
		mp4_mux_file_info_t *pstFileInfo;

		pstFileInfo = &(mp4_ctx->mux_putinfo.m_stFileInfo);

		pstFileInfo->m_ulUserDataTotal = MUX_UUID_NUMBER;

		alloc_size = pstFileInfo->m_ulUserDataTotal * sizeof(mp4_mux_userdata_put_t);

		pstFileInfo->m_pstUserDataList = (mp4_mux_userdata_put_t*)(*mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnMalloc)(alloc_size);
		memset(pstFileInfo->m_pstUserDataList, 0, alloc_size );

		for( udidx = 0; udidx < pstFileInfo->m_ulUserDataTotal; ++udidx )
		{
			pstFileInfo->m_pstUserDataList[udidx].m_ulUserBoxId = FOURCC('u','u','i','d');
			pstFileInfo->m_pstUserDataList[udidx].m_ulUserBoxSize = sz_userdata[udidx][0];
			pstFileInfo->m_pstUserDataList[udidx].m_pbyUserBoxData = &sz_userdata[udidx][1];
		}
	}
	#endif

	//	user data setting start
	#define BOXTOTAL       3
	#define BOXSIZE        4

	if( mp4_ctx->mux_init.m_ulOptions & MP4MUXOPT_WRITE_USERDATA_PER_FRAME )
	{

		static unsigned char sz_userdata_subbox_id[BOXTOTAL][BOXSIZE] = 
		{
			{0x66, 0x69, 0x6e, 0x66}, // finf
			{0x67, 0x78, 0x79, 0x7a}, // gxyz
			{0x66, 0x72, 0x61, 0x6d}, // fram

		};

		unsigned long udidx = 0;
		unsigned long alloc_size = 0;

		mp4_ctx->mux_putinfo.m_stFileInfo.m_ulUserDataTotal = BOXTOTAL;
		alloc_size = mp4_ctx->mux_putinfo.m_stFileInfo.m_ulUserDataTotal*sizeof(mp4_mux_userdata_put_t);
		mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList = (mp4_mux_userdata_put_t*)malloc(alloc_size);
		memset( mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList, 0, alloc_size );


		for( udidx = 0; udidx < mp4_ctx->mux_putinfo.m_stFileInfo.m_ulUserDataTotal; ++udidx )
		{

			mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList[udidx].m_ulUserBoxId = FOURCC('u','d','t','a');
			mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList[udidx].m_ulUserBoxSize = BOXSIZE;
			mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList[udidx].m_pbyUserBoxData = 
				(unsigned char*)malloc(mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList[udidx].m_ulUserBoxSize);

			memcpy( mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList[udidx].m_pbyUserBoxData, sz_userdata_subbox_id[udidx],
					mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList[udidx].m_ulUserBoxSize );
		}

	}//	//	user data setting end
#endif

	ret = TCC_MP4_MUX( MUXOP_OPEN, &mp4_ctx->mux_handle, &mp4_ctx->mux_init, &mp4_ctx->mux_putinfo );

	if( ret < 0 )
	{
		TCC_MP4_MUX( MUXOP_GET_LASTERROR, &mp4_ctx->mux_handle, NULL, &mp4_ctx->mux_error );
		printf("[%s][%d] error:%s\n", __FUNCTION__, __LINE__, mp4_ctx->mux_error.m_pszErrStatus);
		return NULL;
	}

	mp4_ctx->state = MP4_STATE_READY;
	
	return mp4_ctx;
}

int Mux_TCC_MP4_EncClose(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx)
{
	int retval = 0;

	mp4_mux_close_t mux_close;

	if( mp4_ctx->mux_init.m_stCache.m_pfnAppendByCluster == NULL )
	{
		// close video/audio codec
		// free_memsize
		mux_close.m_ulWriteExtensionCacheSize = 1024*1024;
		mux_close.m_pbyWriteExtensionCache=(unsigned char*)(*mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnMalloc)(mux_close.m_ulWriteExtensionCacheSize);
	}
	retval = TCC_MP4_MUX( MUXOP_CLOSE, &mp4_ctx->mux_handle, &mux_close, NULL );
	if( retval < 0 )
	{
		TCC_MP4_MUX( MUXOP_GET_LASTERROR, &mp4_ctx->mux_handle, NULL, &mp4_ctx->mux_error );
		printf("[%s][%d] error:%s\n", __FUNCTION__, __LINE__, mp4_ctx->mux_error.m_pszErrStatus);
	}

	#ifdef FORMATTER_PERFORMANCE
		IO_TMR_FinishMEASURE(0);
		SaveMeasureValue(0);
	#endif

#if 0
#if (MUX_UUID_NUMBER > 0)
	if( pstFileInfo->m_pstUserDataList)
	{
		(*mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnFree)((void*)(mp4_ctx->m_pstUserDataList));
	}
#endif
#endif


	if( mp4_ctx->mux_init.m_ulOptions & MP4MUXOPT_WRITE_USERDATA_PER_FRAME )
	{
		unsigned int udidx;
		
		for( udidx = 0; udidx < mp4_ctx->mux_putinfo.m_stFileInfo.m_ulUserDataTotal; ++udidx )
			free( mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList[udidx].m_pbyUserBoxData);
	}
	

	if(retval > 0)
		retval = 0;

	if(mux_close.m_pbyWriteExtensionCache)
		(*mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnFree)((void*)(mux_close.m_pbyWriteExtensionCache));

	if(mp4_ctx->mux_init.m_stCache.m_pbyWriteCache)
		(*mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnFree)((void*)(mp4_ctx->mux_init.m_stCache.m_pbyWriteCache));

	if(mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList)
		(*mp4_ctx->mux_init.m_stMemoryFuncs.m_pfnFree)((void*)(mp4_ctx->mux_putinfo.m_stFileInfo.m_pstUserDataList));

	free( mp4_ctx);

	return retval;
}

//static int isFirst=1;

int Mux_TCC_MP4_EncFrame(	TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int streamType, void* pBuff, unsigned int buffSize, 
									unsigned char KeyFrame, unsigned int dts)
{
	int retval = 0;	
	if(mp4_ctx->state == MP4_STATE_READY)
	{
		unsigned char* ppbuf;
		ppbuf = (unsigned char*)pBuff;

		if(mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulFourCC == FOURCC_avc1)
		{
			if( (streamType == STREAM_VIDEO) && ((ppbuf[4] & 0x1F) == 7) ) // SPS
			{
				//printf("===  MP4 Mux State : SPS/PPS !!\n");
				mp4_ctx->state = MP4_STATE_AFTER_SEQUENCE_HEADER;
			}
			else
				return 0;
		}
		else
			mp4_ctx->state = MP4_STATE_AFTER_SEQUENCE_HEADER;
	}
	else if(mp4_ctx->state == MP4_STATE_AFTER_SEQUENCE_HEADER)
	{
		unsigned char* ppbuf;
		ppbuf = (unsigned char*)pBuff;

		if(mp4_ctx->mux_putinfo.m_stVideoInfo.m_ulFourCC == FOURCC_avc1)
		{
			if( (streamType == STREAM_VIDEO) && ((ppbuf[4] & 0x1F) == 5) ) // IDR-frame
			{
				//printf("=== MP4 Mux State : IDR !!\n");
				mp4_ctx->state = MP4_STATE_AFTER_FIRST_FRAME;
			}
			else
				return 0;
		}
		else
			mp4_ctx->state = MP4_STATE_AFTER_FIRST_FRAME;
	}
	
	mp4_ctx->putstream.m_pbyStreamData = pBuff;
	mp4_ctx->putstream.m_ulStreamDataSize = buffSize;
	mp4_ctx->putstream.m_ulDTS = dts;
	mp4_ctx->putstream.m_ulPTS = dts;
	mp4_ctx->putstream.m_ulFlags = KeyFrame;

	if(streamType == STREAM_VIDEO)
	{
		mp4_ctx->putstream.m_ulStreamType = MUXTYPE_VIDEO;
	}
	else if(streamType == STREAM_AUDIO)	
	{
		mp4_ctx->putstream.m_ulStreamType = MUXTYPE_AUDIO;
	}

	retval = TCC_MP4_MUX( MUXOP_PUT_STREAM, &mp4_ctx->mux_handle, &mp4_ctx->putstream, &mp4_ctx->outstream );
	if( retval < 0 )
	{
		TCC_MP4_MUX( MUXOP_GET_LASTERROR, &mp4_ctx->mux_handle, NULL, &mp4_ctx->mux_error );
		printf("[%s][%d] error:%s\n", __FUNCTION__, __LINE__, mp4_ctx->mux_error.m_pszErrStatus);
	}

#if 0
	if( streamType == STREAM_VIDEO && isFirst)
	{
		{
			// total, size
			FILEINFO fileinfo;

			mp4_mux_userdata_put_t st_userdata;
			memset( &st_userdata, 0, sizeof(mp4_mux_userdata_put_t) );
			memset( &fileinfo, 0, sizeof(FILEINFO) );

			st_userdata.m_ulUserBoxId = FOURCC('f','i','n','f');
			st_userdata.m_ulUserBoxSize = sizeof(FILEINFO);
			st_userdata.m_pbyUserBoxData = (unsigned char*)malloc(st_userdata.m_ulUserBoxSize);


			// set and put fileinfo
			memcpy( st_userdata.m_pbyUserBoxData, &fileinfo, st_userdata.m_ulUserBoxSize );

			retval = TCC_MP4_MUX( MP4MUXOP_PUT_USERDATA, &mp4_ctx->mux_handle, &st_userdata, &mp4_ctx->outstream );

			if( retval < 0 )
			{
				free( st_userdata.m_pbyUserBoxData);
				TCC_MP4_MUX( MUXOP_GET_LASTERROR, &mp4_ctx->mux_handle, NULL, &mp4_ctx->mux_error );
				printf( "[TESTER:%d] Error:%d, %s(%d) \n", retval, mp4_ctx->mux_error.m_lErrCode, mp4_ctx->mux_error.m_pszErrStatus, mp4_ctx->mux_error.m_llLastStatus );

				//	goto TESTER_EXIT;
			}
			isFirst = 0;
			free( st_userdata.m_pbyUserBoxData);
		}
	}
#endif

	if(retval > 0)
		retval = 0;

	return retval;
}

int Mux_TCC_MP4_EncGPSInfo(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int streamType, char *pGpsinfo, int size)
{
	int retval = 0;
	
	if( streamType == STREAM_VIDEO && pGpsinfo != NULL)
	{
		mp4_mux_userdata_put_t st_userdata;

		memset( &st_userdata, 0, sizeof(mp4_mux_userdata_put_t) );
		//	memset( &gpsinfo, 0, sizeof(GPSNSENSOR) );
#if 0
                printf ("\nRECEIVE!!!!!!!\n");
                printf ("latitude = %f\n", pGpsinfo->latitude);
                printf ("longitude = %f\n", pGpsinfo->longitude);
                printf ("altitude = %f\n", pGpsinfo->altitude);
                printf ("speed = %f\n", pGpsinfo->speed);
                printf ("course = %f\n", pGpsinfo->course);
                printf ("time = %f\n", pGpsinfo->time);
                printf ("geoid = %C\n", pGpsinfo->geoid);
                printf ("hdop = %f\n", pGpsinfo->hdop);
                printf ("satellites = %d\n", pGpsinfo->satellites);
                printf ("quality = %d\n", pGpsinfo->quality);
                printf ("valid = %C\n", pGpsinfo->valid);
                printf ("system = %d\n\n\n", pGpsinfo->system);
                printf ("gmt_time = %d %d %d %d\n",
                        pGpsinfo->gmt.Hour,
                        pGpsinfo->gmt.Min,
                        pGpsinfo->gmt.Sec,
                        pGpsinfo->gmt.HSec);
                printf ("date = %d %d %d\n", pGpsinfo->date.Year, pGpsinfo->date.Month, pGpsinfo->date.Day);
                printf ("time_stamp = %d %d\n", pGpsinfo->time_stamp_sec, pGpsinfo->time_stamp_usec);
#endif
		
		st_userdata.m_ulUserBoxId = FOURCC('f','r','a','m');	//	frame 마다 저장 식별자
		st_userdata.m_ulUserBoxSize = size;
		st_userdata.m_pbyUserBoxData = (unsigned char*)malloc(st_userdata.m_ulUserBoxSize);
		if( st_userdata.m_pbyUserBoxData == NULL)
		{
			printf("__%d__ %s mlloc fail\n", __LINE__, __func__);
			free( st_userdata.m_pbyUserBoxData);
			return 0;
		}
		
		// set and put gpsinfo
		memcpy( st_userdata.m_pbyUserBoxData, pGpsinfo, st_userdata.m_ulUserBoxSize );

		retval = TCC_MP4_MUX( MP4MUXOP_PUT_USERDATA, &mp4_ctx->mux_handle, &st_userdata, &mp4_ctx->outstream );

		if( retval < 0 )
		{
			TCC_MP4_MUX( MUXOP_GET_LASTERROR, &mp4_ctx->mux_handle, NULL, &mp4_ctx->mux_error );
			printf( "[TESTER:%d] Error:%ld, %s(%d) \n", retval, mp4_ctx->mux_error.m_lErrCode, mp4_ctx->mux_error.m_pszErrStatus, (int)mp4_ctx->mux_error.m_llLastStatus );
		}
		free( st_userdata.m_pbyUserBoxData);
	}

    return 0;

}

#if 0

int Mux_TCC_MP3_EncGSensorInfo(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int streamType, void *data, unsigned int sample_num)//tcc_sensor_accel_t *pGSensorinfo)
{
	int retval = 0;
	
	//if( streamType == STREAM_VIDEO && isGSensor)
	if( !isFirst)
	{
		mp4_mux_userdata_put_t st_userdata;
		tcc_sensor_accel_t *pGSensorinfo = (tcc_sensor_accel_t *)data;

		memset( &st_userdata, 0, sizeof(mp4_mux_userdata_put_t) );

		st_userdata.m_ulUserBoxId = FOURCC('g','x','y','z');
		st_userdata.m_ulUserBoxSize = sizeof(tcc_sensor_accel_t) * sample_num;

		st_userdata.m_pbyUserBoxData = (unsigned char*)TCC_malloc(st_userdata.m_ulUserBoxSize);
		if( st_userdata.m_pbyUserBoxData == NULL)
		{
			printf("__%d__ %s mlloc fail\n", __LINE__, __func__);
			return 0;
		}

		if( sample_num > 100)
		{
			free( st_userdata.m_pbyUserBoxData);
			printf("__%d__ %s illegal gsensor data\n", __LINE__, __func__);
			return 0;
		}

		// set and put fram
		memcpy( st_userdata.m_pbyUserBoxData, pGSensorinfo, st_userdata.m_ulUserBoxSize );

		retval = TCC_MP4_MUX( MP4MUXOP_PUT_USERDATA, &mp4_ctx->mux_handle, &st_userdata, &mp4_ctx->outstream );
		if( retval < 0 )
		{
			TCC_MP4_MUX( MUXOP_GET_LASTERROR, &mp4_ctx->mux_handle, NULL, &mp4_ctx->mux_error );
			printf( "[TESTER:%d] Error:%d, %s(%d) \n", retval, mp4_ctx->mux_error.m_lErrCode, mp4_ctx->mux_error.m_pszErrStatus, mp4_ctx->mux_error.m_llLastStatus );
		}
		free( st_userdata.m_pbyUserBoxData);
	}
}
/*
 *	Frame Info (fram)
 *	 - Video frame이 저장 될 때 동시에 0으로 저장
 *	 - G-Sensor 값이 설정 값보다 이상으로 올라가는 경우 1을 저장
 *	 - 하나의 파일에는 하나의 frame 정보가 1로 저장
 */
int Mux_TCC_MP3_EncGSensorDetected(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int isDetected)
{
	int retval = 0;
	
	//if( streamType == STREAM_VIDEO && isGSensor)
	{
		mp4_mux_userdata_put_t st_userdata;

		memset( &st_userdata, 0, sizeof(mp4_mux_userdata_put_t) );

		st_userdata.m_ulUserBoxId = FOURCC('f','r','a','m');
		st_userdata.m_ulUserBoxSize = 1; // 1byte
		st_userdata.m_pbyUserBoxData = (unsigned char*)TCC_malloc(st_userdata.m_ulUserBoxSize);
		if( st_userdata.m_pbyUserBoxData == NULL)
		{
			printf("__%d__ %s mlloc fail\n", __LINE__, __func__);
			return 0;
		}
	
		// set and put fram
		*st_userdata.m_pbyUserBoxData = isDetected; // 0 or 1
		
		retval = TCC_MP4_MUX( MP4MUXOP_PUT_USERDATA, &mp4_ctx->mux_handle, &st_userdata, &mp4_ctx->outstream );
		if( retval < 0 )
		{
			TCC_MP4_MUX( MUXOP_GET_LASTERROR, &mp4_ctx->mux_handle, NULL, &mp4_ctx->mux_error );
			printf( "[TESTER:%d] Error:%d, %s(%d) \n", retval, mp4_ctx->mux_error.m_lErrCode, mp4_ctx->mux_error.m_pszErrStatus, mp4_ctx->mux_error.m_llLastStatus );
		}
		free(st_userdata.m_pbyUserBoxData);
	}
}
#endif

long long Mux_TCC_MP4_GetFileSize(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx)
{
	return mp4_ctx->outstream.m_llEstiFileSize;
}

unsigned char Mux_TCC_MP4_StreamType(unsigned char VideoCodecType, unsigned char AudioCodecType)
{	 
	unsigned char StreamType = 0;

	if(VideoCodecType != CCD_NO_VIDEO_MODE
	 && AudioCodecType != CCD_NO_AUDIO_MODE)
	{
	 StreamType = ( MUXTYPE_VIDEO | MUXTYPE_AUDIO );
	 return StreamType;
	}
	else if(VideoCodecType == CCD_NO_VIDEO_MODE
	 && AudioCodecType != CCD_NO_AUDIO_MODE)
	{
	 StreamType =  MUXTYPE_AUDIO;
	 return StreamType;
	}
	else if(VideoCodecType != CCD_NO_VIDEO_MODE
	 && AudioCodecType == CCD_NO_AUDIO_MODE)
	{
	 StreamType =  MUXTYPE_VIDEO;
	 return StreamType;
	}

	return StreamType;
}




