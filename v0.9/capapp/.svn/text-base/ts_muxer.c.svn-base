#include "ts_muxer.h"

//#include "tcc_usb_trans_common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tcc_memory.h>

 
#define PCR_OPTION		TSMOPT_MAKE_PSEUDO_PCR		// generate pseudo PCR
#define PSEUDO_PCR_OFFSET	1500				// pseudo PCR offset from PTS
 
#define VIDEO_TYPE	TS_VIDEO_AVC
#define AUDIO_TYPE	TS_AUDIO_AAC
#define MUX_VIDEO	1
#define MUX_AUDIO	1
 
#define ENCRYPT_KEY			0xabcdef01
#define ENCRYPT_LEN			4

TCC_TS_Muxer_t Tcc_Ts_Muxer;


tsmresult_t ts_mux_open(tsmhandle_t *pHandle, char *pszFileName)
{
	tsmresult_t			ret;
	tsm_init_t		stMuxerInit;
	tsm_info_t		stMuxerInfo;
	tsm_mem_funcs_t		stMemFuncs = {TCC_malloc, TCC_free, memcmp, memcpy, NULL, memset, TCC_realloc};

	//	TCC 관련 함수로 mapping 해줘야 하며
	//	TCC_fclose -> TCC_ffclose 를 사용해야 함.
	tsm_file_funcs_t	stFileFuncs = {fopen, fread, fseek, ftell, fclose, feof, fflush, fwrite, 0, 0, 0};

	stMuxerInit.m_pszOutputPath = pszFileName;
	stMuxerInit.m_emTsType = TS_NORMAL;
	stMuxerInit.m_ulOption = TSMOPT_INSERT_AVC_AUD ;
	stMuxerInit.m_ulOption |= PCR_OPTION;

	stMuxerInit.m_stMemFuncs = stMemFuncs;
	stMuxerInit.m_stFileFuncs = stFileFuncs;


#if 0
	stMuxerInit.m_lPatUpdateIntervalMS = 100;
	stMuxerInit.m_ulFIOBlockSize = 6144;	//test
#else
	//	by luciapra (V0.1.9b)
	stMuxerInit.m_lPatUpdateIntervalMS = 500;	//	(200ms ~ 1000ms)
	stMuxerInit.m_ulFIOBlockSize = 256*1024;	//test
	//stMuxerInit.m_ulEncryptKey = ENCRYPT_KEY;
	//stMuxerInit.m_lEncryptLength = ENCRYPT_LEN;
#endif

#if PCR_OPTION == TSMOPT_MAKE_PSEUDO_PCR
	stMuxerInit.m_ulClockRefOffset = PSEUDO_PCR_OFFSET; //millisecond
#endif

	ret = TCC_TS_MUX(TSMOP_OPEN, NULL, &stMuxerInit, &stMuxerInfo);
	*pHandle = stMuxerInfo.m_hTsm;

	return ret;
}

tsmresult_t ts_mux_close(tsmhandle_t handle)
{
	return TCC_TS_MUX(TSMOP_CLOSE, handle, 0, 0);
}

tsmresult_t tsm_allocate_program_id(tsmhandle_t handle, tsm_alloc_info_t *pstAllocInfo)
{
	return TCC_TS_MUX(TSMOP_ALLOC_PROGRAMID, handle, 0, pstAllocInfo);
}

tsmresult_t tsm_allocate_es_id(tsmhandle_t handle, tsm_alloc_esid_t *pstAllocEsID, tsm_alloc_info_t *pstAllocInfo)
{
	return TCC_TS_MUX(TSMOP_ALLOC_ESID, handle, pstAllocEsID, pstAllocInfo);
}

tsmresult_t tsm_write_es(tsmhandle_t handle, tsm_write_t *pstWriteInfo, tsm_write_result_t	*pstWriteResult)
{
	return TCC_TS_MUX(TSMOP_WRITE_ES, handle, pstWriteInfo, pstWriteResult);
}

tsmresult_t tsm_get_lasterror(tsmhandle_t handle)
{
	return TCC_TS_MUX(TSMOP_GET_LAST_ERROR, handle, 0, 0);
}

//	by luciapra (V0.1.9b)
tsmresult_t tsm_write_ts(tsmhandle_t handle, tsm_write_es2_t *pstWriteInfo, tsm_write_result_t	*pstWriteResult)
{
	return TCC_TS_MUX(TSMOP_WRITE_ES, handle, pstWriteInfo, pstWriteResult);
}

//	by luciapra (V0.1.9b)
tsmresult_t tsm_write_pcr(tsmhandle_t handle, tsm_write_pcr_t *pstWriteInfo, tsm_write_result_t	*pstWriteResult)
{
	return TCC_TS_MUX(TSMOP_WRITE_PCR, handle, pstWriteInfo, pstWriteResult);
}



int  Mux_TCC_TS_Muxer_Init()
{	
	Tcc_Ts_Muxer.hTsm == NULL;
	Tcc_Ts_Muxer.lProgramID =0;

	Tcc_Ts_Muxer.lVideoID = 0;
	Tcc_Ts_Muxer.lAudioID = 0;
	Tcc_Ts_Muxer.First_Video = 1;

}



int  Mux_TCC_TS_Muxer_Open(unsigned short *FileName, unsigned char VideoCodecType, unsigned char AudioCodecType,
                                                        unsigned int Width, unsigned int Height)
{	

	Mux_TCC_TS_Muxer_Init();


	/* TS Muxer Open */
	ts_mux_open(&Tcc_Ts_Muxer.hTsm, FileName);

	
	if( Tcc_Ts_Muxer.hTsm == NULL )
	{
		printf("[MUXER_ERROR] %d \n", tsm_get_lasterror(0) );
		return FALSE;
	}

	/* TS Program Allocation (registration) */
	if( tsm_allocate_program_id(Tcc_Ts_Muxer.hTsm, &Tcc_Ts_Muxer.stAllocInfo) < 0 )
	{
		printf("[MUXER_ERROR] %d \n", tsm_get_lasterror(0) );
		ts_mux_close(Tcc_Ts_Muxer.hTsm);
		return FALSE;
	}

	Tcc_Ts_Muxer.lProgramID = Tcc_Ts_Muxer.stAllocInfo.m_ulAllocID;

	//if( MUX_VIDEO )
	{
		Tcc_Ts_Muxer.stAllocEsID.m_ulProgramID = Tcc_Ts_Muxer.lProgramID;
		Tcc_Ts_Muxer.stAllocEsID.m_ulStreamType = VIDEO_TYPE;
		Tcc_Ts_Muxer.stAllocEsID.m_szLanguageCode[0] = 0;

		//	by luciapra (V0.1.9b)
		Tcc_Ts_Muxer.stAllocEsID.m_bIsClockReference = 0;


		if( tsm_allocate_es_id(Tcc_Ts_Muxer.hTsm, &Tcc_Ts_Muxer.stAllocEsID, &Tcc_Ts_Muxer.stAllocInfo) < 0 )
		{
			printf("[MUXER_ERROR] %d \n", tsm_get_lasterror(0) );
			ts_mux_close(Tcc_Ts_Muxer.hTsm);
			return FALSE;
		}
		Tcc_Ts_Muxer.lVideoID = Tcc_Ts_Muxer.stAllocInfo.m_ulAllocID;
	}
	//if( MUX_AUDIO )
	{
		Tcc_Ts_Muxer.stAllocEsID.m_ulProgramID = Tcc_Ts_Muxer.lProgramID;
//		Tcc_Ts_Muxer.stAllocEsID.m_ulStreamType = AUDIO_TYPE;
		if (AudioCodecType == CCD_AAC_MODE)
			Tcc_Ts_Muxer.stAllocEsID.m_ulStreamType = TS_AUDIO_AAC;
		else if (AudioCodecType == CCD_MP3_MODE)
		{
			Tcc_Ts_Muxer.stAllocEsID.m_ulStreamType = TS_AUDIO_MPEG1;
//			Tcc_Ts_Muxer.stAllocEsID.m_ulStreamType = TS_AUDIO_MPEG2;
		}
		else if (AudioCodecType == CCD_PCM_MODE)
		{
			Tcc_Ts_Muxer.stAllocEsID.m_ulStreamType = TS_AUDIO_MPEG2;
		}
		else
			Tcc_Ts_Muxer.stAllocEsID.m_ulStreamType = TS_AUDIO_AAC;
		Tcc_Ts_Muxer.stAllocEsID.m_szLanguageCode[0] = 0;

		//	by luciapra (V0.1.9b)
		Tcc_Ts_Muxer.stAllocEsID.m_bIsClockReference = 1;	//	1일 경우 기준을 두는 셋팅

		if( tsm_allocate_es_id(Tcc_Ts_Muxer.hTsm, &Tcc_Ts_Muxer.stAllocEsID, &Tcc_Ts_Muxer.stAllocInfo) < 0 )
		{
			printf("[MUXER_ERROR] %d \n", tsm_get_lasterror(0) );
			ts_mux_close(Tcc_Ts_Muxer.hTsm);
			return FALSE;
		}
		Tcc_Ts_Muxer.lAudioID = Tcc_Ts_Muxer.stAllocInfo.m_ulAllocID;
	}


	return TRUE;
	
}

int Mux_TCC_TS_Muxer_Close(void)
{
	int retval = 0;

	if(Tcc_Ts_Muxer.hTsm) 
	{
		retval=ts_mux_close(Tcc_Ts_Muxer.hTsm);	
	}	
	else
	{
		return FALSE;			
	}	

	Mux_TCC_TS_Muxer_Init();
	return TRUE;	
}


//extern  unsigned int MPEG4DSI_Size;
//extern  unsigned char MPEG4DSI[256];

int Mux_TCC_TS_Muxer_Frame(int streamType, void* pBuff, unsigned int buffSize, unsigned char KeyFrame, unsigned int dts ,char *MPEG4DSI,unsigned int MPEG4DSI_Size)
{
	int retval = 0;
	void *temp_buffer;
	int	 temp_offset=0;

	//	by luciapra (V0.1.9b)
	tsm_write_result_t	stWriteResult;

	if(streamType == STREAM_VIDEO)
	{
		if( Tcc_Ts_Muxer.First_Video == 1)
		{
			temp_buffer=TCC_malloc(buffSize+MPEG4DSI_Size);
			memcpy(temp_buffer+temp_offset,MPEG4DSI,MPEG4DSI_Size);
			temp_offset+=MPEG4DSI_Size;
			
			memcpy(temp_buffer+temp_offset,pBuff,buffSize);
			temp_offset+=buffSize;

			Tcc_Ts_Muxer.stWriteInfo.m_ulEsID = Tcc_Ts_Muxer.lVideoID;
			Tcc_Ts_Muxer.stWriteInfo.m_pbyEsBuff = temp_buffer;
			Tcc_Ts_Muxer.stWriteInfo.m_lEsLength = temp_offset;
			Tcc_Ts_Muxer.stWriteInfo.m_lTimeStamp = dts;
			Tcc_Ts_Muxer.stWriteInfo.m_bIsKeyFrame = KeyFrame;

			//	by luciapra (V0.1.9b)
			retval=tsm_write_es(Tcc_Ts_Muxer.hTsm, &Tcc_Ts_Muxer.stWriteInfo, &stWriteResult);
			TCC_free(temp_buffer);

			Tcc_Ts_Muxer.First_Video=0;
			
		}
		else
		{	
			Tcc_Ts_Muxer.stWriteInfo.m_ulEsID = Tcc_Ts_Muxer.lVideoID;
			Tcc_Ts_Muxer.stWriteInfo.m_pbyEsBuff = pBuff;
			Tcc_Ts_Muxer.stWriteInfo.m_lEsLength = buffSize;
			Tcc_Ts_Muxer.stWriteInfo.m_lTimeStamp = dts;
			Tcc_Ts_Muxer.stWriteInfo.m_bIsKeyFrame = KeyFrame;
 			
			//	by luciapra (V0.1.9b)
			retval=tsm_write_es(Tcc_Ts_Muxer.hTsm, &Tcc_Ts_Muxer.stWriteInfo, &stWriteResult);
		}
	}
	else if(streamType == STREAM_AUDIO)	
	{

		Tcc_Ts_Muxer.stWriteInfo.m_ulEsID = Tcc_Ts_Muxer.lAudioID;
		Tcc_Ts_Muxer.stWriteInfo.m_pbyEsBuff = pBuff;
		Tcc_Ts_Muxer.stWriteInfo.m_lEsLength = buffSize;
		Tcc_Ts_Muxer.stWriteInfo.m_lTimeStamp = dts;

		//	by luciapra (V0.1.9b)
		retval=tsm_write_es(Tcc_Ts_Muxer.hTsm, &Tcc_Ts_Muxer.stWriteInfo, &stWriteResult);
	}
	return retval;
}


 
