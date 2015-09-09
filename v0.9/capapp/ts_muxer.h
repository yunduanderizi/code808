/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : ts_muxer.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef	_TCC_TS_MUXER_H__
#define	_TCC_TS_MUXER_H__
/******************************************************************************
* include 
******************************************************************************/
#include "TCC_TS_MUXER.h"
#include "muxer.h"


/******************************************************************************
* defines 
******************************************************************************/



/******************************************************************************
* typedefs & structure
******************************************************************************/
typedef	struct	TCC_TS_Muxer_structure
{	
	tsmhandle_t		hTsm;
	long			lProgramID ;
	tsm_alloc_info_t	stAllocInfo;
	tsm_alloc_esid_t	stAllocEsID;
	tsm_write_t		stWriteInfo;

	long			lVideoID ;
	long			lAudioID ;
	int			First_Video;
	
}TCC_TS_Muxer_t;





/******************************************************************************
* globals
******************************************************************************/


/******************************************************************************
* locals
******************************************************************************/



/******************************************************************************
* declarations
******************************************************************************/
int  Mux_TCC_TS_Muxer_Init();
int  Mux_TCC_TS_Muxer_Open(unsigned short *FileName, unsigned char VideoCodecType, unsigned char AudioCodecType,
                                                        unsigned int Width, unsigned int Height);

int Mux_TCC_TS_Muxer_Close(void);                            
//int Mux_TCC_TS_Muxer_Frame(int streamType, void* pBuff, unsigned int buffSize, unsigned char KeyFrame, unsigned int dts);
int Mux_TCC_TS_Muxer_Frame(int streamType, void* pBuff, unsigned int buffSize, unsigned char KeyFrame, unsigned int dts ,char *MPEG4DSI,unsigned int MPEG4DSI_Size);

#endif // _TCC_TS_MUXER_H__

