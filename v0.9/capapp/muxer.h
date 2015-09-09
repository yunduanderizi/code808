/*!
 ***********************************************************************
 \par Copyright
 \verbatim
  ________  _____           _____   _____           ____  ____   ____		
     /     /       /       /       /       /     /   /    /   \ /			
    /     /___    /       /___    /       /____ /   /    /____/ \___			
   /     /       /       /       /       /     /   /    /           \		
  /     /_____  /_____  /_____  /_____  /     / _ /_  _/_      _____/ 		
   																				
  Copyright (c) 2008-2009 Telechips Inc.
  Korad Bldg, 1000-12 Daechi-dong, Kangnam-Ku, Seoul, Korea					
 \endverbatim
 ***********************************************************************
 */
/*!
 ***********************************************************************
 *
 * \file
 *		muxer.h
 * \date
 *		2010/02/03
 * \author
 *		Song-Taek Lim(AValgorithm@telechips.com) 
 * \brief
 *		mp4 muxer project main api	(prototypes for mp4_muxer.c)
 * \version
 *		1.5.2(2009/06/17)
 *
 ***********************************************************************
 */
 
#ifndef MUXER_H_
#define MUXER_H_
typedef enum
{
	CCD_NULL=0,
	CCD_MPEG4_MODE, 	//  1
	CCD_H263_MODE,		//  2	
	CCD_H264_MODE,		//  3
	CCD_AAC_MODE,		//  4
	CCD_MP3_MODE,		//  5
	CCD_PCM_MODE,		//  6
	CCD_AMRNB_MODE,	//  7
	CCD_QCELP_MODE,	//  8
	CCD_EVRC_MODE,		//  9
	CCD_NO_VIDEO_MODE,	//  10
	CCD_NO_AUDIO_MODE	//  11
}CCD_MODE;

typedef enum
{
	STREAM_NULL=0,
	STREAM_VIDEO, 		//   1
	STREAM_AUDIO,		// 2
	STREAM_NUM			// 3
}STREAM_TYPE;
#endif

