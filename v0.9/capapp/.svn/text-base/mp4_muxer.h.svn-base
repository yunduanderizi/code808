/*!
 ***********************************************************************
 \par Copyright
 \verbatim
  ________  _____           _____   _____           ____  ____   ____		
     /     /       /       /       /       /     /   /    /   \ /			
    /     /___    /       /___    /       /____ /   /    /____/ \___			
   /     /       /       /       /       /     /   /    /           \		
  /     /_____  /_____  /_____  /_____  /     / _ /_  _/_      _____/ 		
   																				
  Copyright (c) 2009-2011 Telechips Inc.
  19~23 Floor, Luther Building, 7-20 Sincheon-dong, Songpa-gu, Seoul, Korea 
 \endverbatim
 ***********************************************************************
 */
/*!
 ***********************************************************************
 *
 * \file
 *		mp4_muxer.h
 * \date
 *		2008/09/29
 * \author
 *		A/V algorithm group (AValgorithm@telechips.com) 
 * \brief
 *		mp4 muxer project main api	(prototypes for mp4_muxer.c)
 * \version
 *		1.1.4(2010/01/11) : added a structure mp4_mux_ftypbox_t
 *		1.1.3(2009/11/16)
 *		1.1.2(2009/10/08)
 *		1.1.1(2009/09/23)
 *		1.1.0(2009/06/17)
 *		1.0.1(2009/02/19)
 *		1.0.0(2009/11/27)
 *
 ***********************************************************************
 */
#ifndef _MP4_MUXER_H_
#define _MP4_MUXER_H_

#include "av_common_mux.h"

#define MP4MUXOP_GET_VERSION 3000 //!< get version of the mp4 muxer
#define MP4MUXOP_PUT_USERDATA 3001

typedef av_mux_handle_t mp4_mux_handle_t; //!< the type of a muxer's handle
typedef av_mux_result_t mp4_mux_result_t; //!< the type of a muxer's result

//! Muxer Initialize - input
typedef struct mp4_mux_init_t 
{
	// common
	av_memory_func_t	 m_stMemoryFuncs;		//!< [in] callback functions for memory
	av_file_func_t		 m_stFileFuncs;			//!< [in] callback functions for file
	char				*m_pszOutFileName;		//!< [in] output file name
	unsigned long		 m_ulStreamTypeAll;		//!< [in] ( MUXTYPE_VIDEO | MUXTYPE_AUDIO )

	// mp4 specific info.
	//! muxer's additional options
#define MP4MUXOPT_ONLY_32BITS_OFFSET	(1<<0)	// this means that muxer will be use only 32bit offset during file writing.(stco instead of co64). 
												// therefore muxer will be managed normally up to 2GB.
#define MP4MUXOPT_WRITE_CTTS				(1<<1)	// this means that muxer write ctts-box, set if b-frames exist(not used in TCC_ONBOARD)
#define MP4MUXOPT_OPEN_FROM_FILE_HANDLE	(1<<2)	// this means that muxer process mp4-file from the opened file-handle, in case m_pszOutFileName must be set the opened file-handle by host.
#define MP4MUXOPT_WRITE_USERDATA_PER_FRAME	(1<<3)
	unsigned long		 m_ulOptions;			//!< [in] option

	//! Cache setting
	struct {
		unsigned char	*m_pbyWriteCache;			//!< [in] allocated write cache(buffer) pointer( for mp4 file, video and audio tmp file )
		unsigned long	 m_ulWriteCacheSize;		//!< [in] cache(buffer) size for file writing
		unsigned long	 m_ulTmpFileVideoCacheSize;	//!< [in] cache(buffer) size for video temp file writing
		unsigned long	 m_ulTmpFileAudioCacheSize;	//!< [in] cache(buffer) size for audio temp file writing

		//! for temp file writing(mp4 info)
		char			*m_pszTmpFilePath;			//!< [in] pointer to temp file path name( (ex) "C:/tmp_folder" ), max 240 bytes
													//!< NULL -->current folder
		/*!< [in] for speed up
			 TCC append,SysGr Function: 
				TC_STAT TC_AppendhandleByCluster(TC_U32 uIndex1, TC_U32 uIndex2, TC_U32 uOption); 
			 this function is used for direct copy(appending) from temp file to output file.
			 if this function is NULL, general operation will be used.
		 */
		int (*m_pfnAppendByCluster) (unsigned int uiHandle1, unsigned int uiHandle2, unsigned int uiMode); 
	} m_stCache;
} mp4_mux_init_t;


/*!
 ============================================================
 *
 *	File/Video/Audio/Subtitle Information
 *		- information about video, audio or subtitle into one file.
 *
 ============================================================
*/

typedef struct mp4_mux_userdata_put_t
{
	unsigned long  m_ulUserBoxId;		//!< box id(FourCC): (ex) uuid
	unsigned char *m_pbyUserBoxData;	//!< pointer to userdata except for m_ulUserBoxId
	unsigned long  m_ulUserBoxSize;		//!< size of m_pbyUserBoxData
} mp4_mux_userdata_put_t;

typedef struct mp4_mux_ftypbox_t
{
	unsigned long  m_ulMajorBrand;			//!< is a brand identifier: FourCC
	unsigned long  m_ulMinorVersion;		//!< is an informative integer for the minor version of the major brand.
	unsigned long *m_pulCompatibleBrands;	//!< is a list, to the end of the box, of brands
	unsigned long  m_ulCompatibleBrandTotal;//!< total number of m_pulCompatibleBrands
} mp4_mux_ftypbox_t;

//! file information
typedef struct mp4_mux_file_info_t
{
	struct {
		long	m_lYear;			//!< year
		long	m_lMonth;			//!< months since January	- [0,11]
		long	m_lDayOfMonth;		//!< day of the month		- [1,31]
		long	m_lDayOfWeek;		//!< days since Sunday		- [0, 6]
		long	m_lHour;			//!< hours since midnight	- [0,23]
		long	m_lMin;				//!< minutes after the hour	- [0,59]
		long	m_lSec;				//!< seconds after the minute- [0,59]
	} m_stTime;						//!< [in] time information.

	mp4_mux_userdata_put_t	*m_pstUserDataList; //!< [in] pointer to array of structure mp4_mux_userdata_put_t
	unsigned long			 m_ulUserDataTotal;	//!< [in] total number of array of structure m_pstUserDataList
	mp4_mux_ftypbox_t		*m_pstFtypBox;		//!< [in] if you want to change ftypbox, you must allocate and fill it. (NULL is default)
} mp4_mux_file_info_t;

//! video information
typedef struct mp4_mux_video_info_t
{
	// common info
	unsigned long	 m_ulStreamID;
	unsigned long	 m_ulWidth;
	unsigned long	 m_ulHeight;
	unsigned long	 m_ulFrameRate;
	unsigned long	 m_ulFourCC;
	unsigned char	*m_pbyCodecPrivate;
	unsigned long	 m_ulCodecPrivateSize;
} mp4_mux_video_info_t;

//! audio information
typedef struct mp4_mux_audio_info_t
{
	// common info
	unsigned long	 m_ulStreamID;
	unsigned long	 m_ulSamplePerSec;
	unsigned long	 m_ulBitsPerSample;
	unsigned long	 m_ulChannels;
	unsigned long	 m_ulFormatTag;
	unsigned char	*m_pbyCodecPrivate;
	unsigned long	 m_ulCodecPrivateSize;
	char			 m_szLanguage[4];		//!< ISO 639-2 language code
} mp4_mux_audio_info_t;

//! Muxer Information - input
typedef struct mp4_mux_putinfo_t
{
	mp4_mux_file_info_t		m_stFileInfo;
	mp4_mux_video_info_t	m_stVideoInfo;
	mp4_mux_audio_info_t	m_stAudioInfo;
} mp4_mux_putinfo_t;


/*============================================================
 *	Operation: Put Stream
 =============================================================
 *	PARAMS:
 *		- opCode:	MUXOP_PUT_STREAM
 *		- handle:	(av_mux_handle_t*)pHandle
 *		- param1:	(av_mux_putstream_t*)in
 *		- param2:	(av_mux_outstream_t*)out
 *	RETURNS:
 *		- DMXRET_SUCCESS / DMXRET_FAILED / DMXERR_END_OF_STREAM
 ============================================================*/

//! put stream structure
typedef struct mp4_mux_putstream_t
{
	unsigned char  *m_pbyStreamData;		//!< [in] a pointer to stream data
	unsigned long	m_ulStreamDataSize;		//!< [in] a size to stream data
	unsigned long	m_ulStreamType;			//!< [in] a type of stream

	// if B-frames do not exist, m_ulDTS is equal to m_ulPTS
	unsigned long	m_ulDTS;				//!< [in] decoding timestamp of stream, must be in ascending order.
	unsigned long	m_ulPTS;				//!< [in] presentation timestamp of stream
#define MP4MUXFLAG_KEY 0x01	// key frame info
	unsigned long	m_ulFlags;				//!< [in] flags
} mp4_mux_putstream_t;

//! out stream structure
typedef struct mp4_mux_outstream_t
{
	S64			m_llEstiFileSize;		//!< [out] estimated file size
} mp4_mux_outstream_t;

//! Muxer Close - input
typedef struct mp4_mux_close_t 
{
	/*! Muxer will use this memory for file writing instead of m_pbyWriteCache 
	    this memory must be greater than m_ulWriteCacheSize. 
		if m_pbyWriteExtensionCache is zero, m_pbyWriteCache will be used.
		also if m_pfnAppendByCluster exists, m_pbyWriteExtensionCache and m_ulWriteExtensionCacheSize is ignored.*/
	unsigned char	*m_pbyWriteExtensionCache;		//!< [in] extension cache(buffer) for file writing
	unsigned long	 m_ulWriteExtensionCacheSize;	//!< [in] length of extension cache(buffer)
} mp4_mux_close_t;

/*!
 ============================================================
 *
 *	Get Errors
 *
 ============================================================
*/
typedef struct mp4_mux_error_t
{
	long	 m_lErrCode;
	char	*m_pszErrStatus;
	S64		 m_llLastStatus;
} mp4_mux_error_t;

/*!
 ***********************************************************************
 * \brief
 *		TCC_MP4_MUX		: main api function of mp4 muxer
 * \param
 *		[in]Op			: muxer operation 
 * \param
 *		[in,out]pHandle	: mp4's handle
 * \param
 *		[in]pParam1		: init or input parameter
 * \param
 *		[in,out]pParam2	: output or information parameter
 * \return
 *		If successful, TCC_MP4_MUX returns 0 or plus. Otherwise, it returns a minus value.
 ***********************************************************************
 */
mp4_mux_result_t 
TCC_MP4_MUX( int Op, mp4_mux_handle_t* pMp4Handle, void* pParam1, void* pParam2 );

#endif//_MP4_MUXER_H_
