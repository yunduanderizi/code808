#ifndef __TCC_TS_MUXER_H__
#define __TCC_TS_MUXER_H__

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//	
//	TODO
//
// 1. Append : CRC check

	
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//	
//	Definitions
//
#define ES_MAX			32
#define PROGRAM_MAX		16

#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)
typedef __int64		MUX_S64;
#else
typedef long long	MUX_S64;
#endif

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//	
//	Exported Types
//
/* API return typs */
typedef	long		tsmresult_t;
typedef	void*		tsmhandle_t;

//! TS type
typedef enum tstype_t
{
	TS_UNKNOWN = 0xFFFFFFFF,
	TS_NORMAL = 0,
	TS_M2TS,
	TS_DVHS,		//!< not used
	TS_FEC,
	TS_TYPEMAX,
} tstype_t;

//! Stream Type
typedef enum estype_t
{
	ES_UNKNOWN		= 0xFFFFFFFF,
	ES_VIDEO		= 1,
	ES_AUDIO		= 2,
	ES_GRAPHICS		= 4,
	ES_SUBTITLE		= 8
} estype_t;

//! video stream type
typedef enum ts_video_type_t
{
	TS_VIDEO_MPEG1					= 0x01,
	TS_VIDEO_MPEG2					= 0x02,	// BD-ROM P/S
	TS_VIDEO_MPEG4					= 0x10,
	TS_VIDEO_AVC					= 0x1B,	// BD-ROM P/S
	TS_VIDEO_VC1					= 0xEA,	// BD-ROM P/S
//	TS_VIDEO_DIRAC					= 0xD1,
} ts_video_type_t;

//! audio stream type
typedef enum ts_audio_type_t
{
	TS_AUDIO_MPEG1					= 0X03,
	TS_AUDIO_MPEG2					= 0X04,
	TS_AUDIO_AAC					= 0X0F,
	TS_AUDIO_HDMV_LPCM				= 0x80,	// BD-ROM P
	TS_AUDIO_DOLBY_DIGITAL			= 0x81,	// BD-ROM P
	TS_AUDIO_HDMV_DTS				= 0x82,	// BD-ROM P
	TS_AUDIO_DOLBY_LOSSLESS			= 0x83,	// BD-ROM P
	TS_AUDIO_DOLBY_DIGITAL_PLUS		= 0x84,	// BD-ROM P
	TS_AUDIO_DTS_HD_EXCEPT_XLL		= 0x85,	// BD-ROM P
	TS_AUDIO_DTS_HD_XLL				= 0x86,	// BD-ROM P
	TS_AUDIO_DTS					= 0x8A,
	TS_AUDIO_DOLBY_DIGITAL_PLUS_S	= 0xA1,	// BD-ROM S
	TS_AUDIO_DTS_HD_LBR				= 0xA2,	// BD-ROM S
} ts_audio_type_t;

//! chararcter encoding
typedef enum char_enc_t5
{
	ENC_UNICODE20,						//!< Unicode 2.0 (UTF-8 and UTF-16BE)
	ENC_SJIS,							//!< Shift-JIS
	ENC_KSC5601,						//!< KSC 5601-1987 (including KSC 5653)
	ENC_GB18030,						//!< GB18030-2000
	ENC_GB2312,							//!< GB2312
	ENC_BIG5,							//!< BIG5
	ENC_UNKNOWN = 0xFFFFFFFF
} char_enc_t;

//! memory management functions
typedef struct tsd_mem_funcs_t
{
	void* (*m_pfnMalloc			) ( unsigned int );									//!< malloc
	void  (*m_pfnFree			) ( void* );										//!< free
	int   (*m_pfnMemcmp			) ( const void*, const void*, unsigned int );		//!< memcmp
	void* (*m_pfnMemcpy			) ( void*, const void*, unsigned int );				//!< memcpy
	void* (*m_pfnMemmove		) ( void*, const void*, unsigned int );				//!< memmove
	void* (*m_pfnMemset			) ( void*, int, unsigned int );						//!< memset
	void* (*m_pfnRealloc		) ( void*, unsigned int );							//!< realloc
	int reserved[16-7];
} tsm_mem_funcs_t;

//! file management functions
typedef struct tsd_file_funcs_t
{
	void*		 (*m_pfnFopen	) ( const char*, const char* );						//!< fopen
	unsigned int (*m_pfnFread	) ( void*, unsigned int, unsigned int, void* );		//!< fread
	int			 (*m_pfnFseek	) ( void*, long, int );								//!< fseek
	long		 (*m_pfnFtell	) ( void* );										//!< ftell
	int			 (*m_pfnFclose	) ( void* );										//!< fclose
	unsigned int (*m_pfnFeof	) ( void* );										//!< feof
	unsigned int (*m_pfnFflush	) ( void* );										//!< fflush
	unsigned int (*m_pfnFwrite	) ( const void*, unsigned int, unsigned int, void* );//!< fwrite (Muxer only)
	int			 (*m_pfnUnlink	) ( const char* );									//!< _unlink (Muxer only)

#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)
	int			(*m_pfnFseek64)	(void*, __int64, int );							//!< __fseek64
	__int64		(*m_pfnFtell64)	(void*);										//!< __ftell64
#else
	int			(*m_pfnFseek64)	(void*, long long, int );						//!< fseeko
	long long	(*m_pfnFtell64)	(void*);										//!< ftello
#endif
	int reserved[16-11];
} tsm_file_funcs_t;



////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//	
//	Exported Constants
//

//! BOOL type constant values
#ifndef TRUE
    #define TRUE (1 == 1)
#endif
#ifndef FALSE
    #define FALSE (!TRUE)
#endif

#ifndef NULL
	#define NULL 0
#endif


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//
//	TS Muxer API
//
/*============================================================
	ROUTINE: 
		TCC_TS_MUX

	DESCRIPTION:
		Main function of the TS Muxer API

	PARAMS:
		- opCode: operation code (defined below)
		- handle: TS Muxer handle
		- param1: operation parameter 1 (generally used as input)
		- param2: operation parameter 2 (generally used as output)

	RETURNS:
		- TRUE: Operation success
		- FALSE: Operation failed
		- ERROR_CODE: defined below
============================================================*/
tsmresult_t TCC_TS_MUX(unsigned long opCode, tsmhandle_t handle, void *param1, void *param2);

/* opCode constants */
#define TSMOP_OPEN				0	
#define TSMOP_CLOSE				1	
#define TSMOP_WRITE_ES			2	// push TS data
#define TSMOP_WRITE_PCR			3	// push TS data
#define TSMOP_ALLOC_PROGRAMID	10	// allocate a program ID
#define TSMOP_ALLOC_ESID		11	// allocate a element stream ID
#define TSMOP_GET_LAST_ERROR	20	


/*============================================================
	Usage - Muxer Instance Open
==============================================================
	PARAMS:
		- opCode:		TSMOP_OPEN
		- handle:		NULL
		- param1(in):	(tsm_init_t*)in
		- param2(out):	(tsm_info_t*)out

		<< file io source >>
		param1:	in->szPath = "example.ts"
				in->emTsType = TS_NORMAL / TS_M2TS;
				in->stMemFuncs = <memory management functions>;
				in->stFileFuncs = <file management functions>;
	
	RETURNS:
		- TRUE / FALSE
============================================================*/
typedef struct tsm_es_info_t
{
	unsigned long	m_ulStreamID;			//
	unsigned long	m_ulStreamType;			// TS_VIDEO_xxx / TS_AUDIO_xxx
	char			m_szLanguageCode[4];	// ISO 639 Language Code

	//	by luciapra (V0.1.9b)
	char_enc_t		m_emCharEncType;		// used for text subtitle stream (TS_SUBTITLE_xxx)
	unsigned long	m_ulNextTimeStamp;
	unsigned char	reserved[12];
} tsm_es_info_t;

typedef struct tsm_program_info_t
{
	unsigned long	m_ulProgramID;			// program ID
	tsm_es_info_t  *m_pstEsInfoList;		// element stream info list
	long			m_lEsTotal;				// the number of element streams
} tsm_program_info_t;

/* ulOption */
#define TSMOPT_DEFAULT			0x0000	// default
#define TSMOPT_INSERT_AVC_AUD	0x0001	// for AVC(H.264) stream muxing
#define TSMOPT_APPEND			0x0002	// append at the end of file	(currently not supported)

//	by luciapra (V0.1.9b)
#define TSMOPT_MAKE_PSEUDO_PCR		0x0004	// make pseudo PCR (program clock reference)
#define TSMOPT_USE_EXTERNAL_PCR		0x0008	// use external input PCR (program clock reference)
#define TSMOPT_TS_STREAM_RECORD		0x0010	// use PTS/PCR value of source TS stream
#define TSMOPT_PACKET_ENCRYPTION	0x0020	// encrypt packet bytes


typedef struct tsm_init_t
{
	/* source information */
	char			   *m_pszOutputPath;		// source path
	tstype_t			m_emTsType;				// TS type	(currently TS_NORMAL supported only)
	unsigned long		m_ulOption;				// Demuxer options
	long				m_lPatUpdateIntervalMS;	// PAT/PMT output interval (milisecond)

	/* callback functions */
	tsm_mem_funcs_t		m_stMemFuncs;			// Memory management functions
	tsm_file_funcs_t	m_stFileFuncs;			// File management functions

	/* IO configuration */
	unsigned long		m_ulFIOBlockSize;		// File read/write block size (0: default 6114)

	//	by luciapra (V0.1.9b)
	/* PCR configuration */
	unsigned long		m_ulClockRefOffset;		// PCR time offset from PTS (for TSMOPT_MAKE_PSEUDO_PCR option)
	/* Packet Encryption */
	unsigned long		m_ulEncryptKey;			// encryption key value (ex: 0x12345678)
	long				m_lEncryptLength;		// encryption byte length (4 ~ 184)
} tsm_init_t;

typedef struct tsm_info_t
{
	tsmhandle_t		m_hTsm;					// TS Muxer Handle

	//	by luciapra (V0.1.9b)
	/* informations for TS appending (on TSMOPT_APPEND option) */
	tsm_program_info_t *m_pstProgramInfoList;	// information of previously muxed programs
	long				m_lProgramTotal;		// the number of previously muxed programs
} tsm_info_t;


//	by luciapra (V0.1.9b)
/*============================================================
	Usage - Write Element Stream
==============================================================
	PARAMS:
		- opCode:	TSMOP_WRITE_ES
		- handle:	<TS Muxer Handle>
		- param1:	(tsm_write_es_t* or tsm_write_es2_t*)in
		- param2:	(tsm_write_result_t*)out
	RETURNS:
		- TRUE / FALSE
============================================================*/
typedef struct tsm_write_es_t
{
	unsigned long	m_ulEsID;			// Packet ID of element stream
	long			m_lEsLength;		// length of element stream
	unsigned char  *m_pbyEsBuff;		// pointer of element stream buffer	(FRAME ALIGNED STREAM DATA !!)
	long			m_lTimeStamp;		// presentation time stamp
	long			m_lClockReference;	// used to generate transport stream PCR
	long			m_bIsKeyFrame;		// used to generate "H.264 AUD" and "transport stream PCR (program clock reference)"
} tsm_write_es_t;

typedef struct tsm_write_es2_t
{
	unsigned long	m_ulEsID;			// Packet ID of element stream
	long			m_lEsLength;		// length of element stream
	unsigned char  *m_pbyEsBuff;		// pointer of element stream buffer
	MUX_S64			m_llPTS;			// 33bit PTS
} tsm_write_es2_t;

typedef struct tsm_write_result_t
{
	long			m_lWriteBytes;
	long			m_lWriteTotal;		// total bytes
} tsm_write_result_t;

//	by luciapra (V0.1.9b)
/*============================================================
	Usage - Write PCR
==============================================================
	PARAMS:
		- opCode:	TSMOP_WRITE_PCR
		- handle:	<TS Muxer Handle>
		- param1:	(tsm_write_pcr_t*)in
		- param2:	(tsm_write_result_t*)out
	RETURNS:
		- TRUE / FALSE
============================================================*/
typedef struct tsm_write_pcr_t
{
	unsigned long	m_ulProgramID;
	MUX_S64			m_llPCR;		// 33bit PCR
	long			m_lPCRExt;		// 9bit PCR extension
} tsm_write_pcr_t;

/*============================================================
	Usage - Write TS Packet
==============================================================
	PARAMS:
		- opCode:	TSM_WRITE_TS
		- handle:	<TS Muxer Handle>
		- param1:	(tsm_write_t*)in
		- param2:	NULL
	RETURNS:
		- TRUE / FALSE
============================================================*/
typedef struct tsm_write_t
{
	unsigned long	m_ulEsID;			// Packet ID of element stream
	long			m_lEsLength;		// length of element stream
	unsigned char  *m_pbyEsBuff;		// pointer of element stream buffer	(FRAME ALIGNED STREAM DATA !!)
	long			m_lTimeStamp;		// presentation time stamp
	long			m_bIsKeyFrame;		// for generate H.264 AUD
} tsm_write_t;


/*============================================================
	Usage - Allocates a Program (step1)
==============================================================
	PARAMS:
		- opCode:	TSMOP_ALLOC_PROGRAMID
		- handle:	<TS Muxer Handle>
		- param1:	NULL
		- param2:	(tsm_alloc_info_t*)out
	RETURNS:
		- TRUE / FALSE
============================================================*/
typedef struct tsm_alloc_info_t
{
	unsigned long	m_ulAllocID;		// allocated ID (of Program or Element Stream)
} tsm_alloc_info_t;

/*============================================================
	Usage - Allocates a Element Stream (step2)
==============================================================
	PARAMS:
		- opCode:	TSMOP_ALLOC_ESID
		- handle:	<TS Muxer Handle>
		- param1:	(TSM_ALLOCES*)in
		- param2:	(TSM_ALLOCINFO*)out
	RETURNS:
		- TRUE / FALSE
============================================================*/
typedef struct tsm_alloc_esid_t
{
	unsigned long	m_ulProgramID;			// previously allocated program ID

	unsigned long	m_ulStreamType;			// TS_VIDEO_xxx / TS_AUDIO_xxx / TS_GRAPHICS_xxx / TS_SUBTITLE_xxx
	char			m_szLanguageCode[4];	// ISO 639 Language Code
	char_enc_t		m_emCharEncType;		// used for text subtitle stream (TS_SUBTITLE_xxx)

//	by luciapra (V0.1.9b)
#if 0	
	unsigned char	reserved[16];
#else
	long			m_bIsClockReference;	// PCR (program clock reference) elementary stream
	unsigned char	reserved[12];
#endif
} tsm_alloc_esid_t;


/*============================================================
	Usage - Get Last Error
==============================================================
	PARAMS:
		- opCode:	TSMOP_GET_LAST_ERROR
		- handle:	<TS Muxer Handle>
		- param1:	NULL
		- param2:	NULL
	RETURNS:
		- ERROR_CODE:	defined below
============================================================*/
/*============================================================
	ERROR_CODE constants
============================================================*/
#define TSMERR_NONE								0

// system error
#define TSMERR_BASE_SYSTEM_ERROR				0
#define TSMERR_SYSTEM_ERROR						(TSMERR_BASE_SYSTEM_ERROR - 1 )
#define TSMERR_INVALID_SYSTEM					(TSMERR_BASE_SYSTEM_ERROR - 2 )

//	by luciapra (V0.1.9b)
#define TSMERR_OUT_OF_MEMORY					(TSMERR_BASE_SYSTEM_ERROR - 3)
#define TSMERR_FILE_OPEN_FAILED					(TSMERR_BASE_SYSTEM_ERROR - 4)
#define TSMERR_FILE_WRITE_FAILED				(TSMERR_BASE_SYSTEM_ERROR - 5)
#define TSMERR_FILE_SEEK_FAILED					(TSMERR_BASE_SYSTEM_ERROR - 6)

// broken file error
#define TSMERR_BASE_BROKEN_FILE					(-1000)
#define TSMERR_BROKEN_FILE						(TSMERR_BASE_BROKEN_FILE - 0 )

//	by luciapra (V0.1.9b)
#define TSMERR_PAT_PARSING_FAILED				(TSMERR_BASE_BROKEN_FILE - 1)
#define TSMERR_PMT_PARSING_FAILED				(TSMERR_BASE_BROKEN_FILE - 2)
#define TSMERR_PAT_NOT_OCCURED					(TSMERR_BASE_BROKEN_FILE - 3)
#define TSMERR_PAT_NOT_FOUND					(TSMERR_BASE_BROKEN_FILE - 4)
#define TSMERR_PMT_NOT_FOUND					(TSMERR_BASE_BROKEN_FILE - 5)
#define TSMERR_COUNTINUITY_INFO_NOT_FOUND		(TSMERR_BASE_BROKEN_FILE - 6)
#define TSMERR_END_OF_STREAM					(TSMERR_BASE_BROKEN_FILE - 7)

// not supported format
#define TSMERR_BASE_NOT_SUPPORTED_FORMAT		(-3000)
#define TSMERR_NOT_SUPPORTED_FORMAT				(TSMERR_BASE_NOT_SUPPORTED_FORMAT - 0)

//	by luciapra (V0.1.9b)
#define TSMERR_TS_TYPE_NOT_SUPPORTED			(TSMERR_BASE_NOT_SUPPORTED_FORMAT - 1)

// invalid parameter error
#define TSMERR_BASE_INVALID_FUNC_PARAM			(-4000)
#define TSMERR_INVALID_FUNC_PARAM				(TSMERR_BASE_INVALID_FUNC_PARAM - 0 )

//	by luciapra (V0.1.9b)
#define TSMERR_INVALID_OP_CODE					(TSMERR_BASE_INVALID_FUNC_PARAM - 1)
#define TSMERR_INVALID_MUXER_HANDLE				(TSMERR_BASE_INVALID_FUNC_PARAM - 2)
#define TSMERR_INVALID_PROGRAM_ID				(TSMERR_BASE_INVALID_FUNC_PARAM - 3)
#define TSMERR_INVALID_STREAM_ID				(TSMERR_BASE_INVALID_FUNC_PARAM - 4)
#define TSMERR_INVALID_ENCRYPTION_LENGTH		(TSMERR_BASE_INVALID_FUNC_PARAM - 5)

// muxer internal error
#define TSMERR_BASE_MUXER_INTERNAL				(-10000)
#define TSMERR_MUXER_INTERNAL					(TSMERR_BASE_MUXER_INTERNAL - 0)


#define IS_SYSERROR(code)						(    0 >  code && code > -1000)
#define IS_FILEERROR(code)						(-1000 >= code && code > -2000)
#define IS_SEEKERROR(code)						(-2000 >= code && code > -3000)
#define IS_FORMATERROR(code)					(-3000 >= code && code > -4000)
#define IS_PARAMERROR(coce)						(-4000 >= code && code > -5000)
#define IS_INTERNALERROR(coce)					(-10000 >= code)


#ifdef __cplusplus
}
#endif
#endif //__TCC_TS_MUXER_H__
