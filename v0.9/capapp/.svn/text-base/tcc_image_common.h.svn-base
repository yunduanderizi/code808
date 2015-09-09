/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_image_common.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
/******************************************************************************
* include 
******************************************************************************/

#ifndef	_IMAGE_HEADER_H_
#define	_IMAGE_HEADER_H_

#include "main.h"
#include "Virtual.h"
#include <lcd_resolution.h>

/******************************************************************************
* defines 
******************************************************************************/
#ifndef MAX
/* MAX and MIN macros */
#define MAX(x,y) ((x > y) ? x:y)
#endif

#ifndef MIN
#define MIN(x,y) ((x > y) ? y:x)
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifdef PRINTF_DEBUG
#define IMAGE_DEBUG
#endif

#ifdef PRINTF_DEBUG
#ifdef IMAGE_DEBUG
#define	PRINTF printf
#endif
#else
#define PRINTF
#endif

//#define TCC_IMAGE_PERFORMANCE_INCLUDE

#ifdef TCC_IMAGE_PERFORMANCE_INCLUDE
extern unsigned int dec_count; 
#endif

#ifdef TCC_83XX_INCLUDE
#define 			PHYSICAL_REG_SIZE			1024*4
#define 			INT_BASE					0x90003000
#define 			CKC_BASE					0x90006000

#define			MAX_STREAM_BUF			100   // kbyte unit
#define 			IMAGE_PHY_MMAP_ADDR 		HW_MEM_BASE
#define 			IMAGE_PHY_MMAP_LEN 		(1024 * 1024 * 4)
#endif
#ifdef TCC_79XX_INCLUDE
#define 			PHYSICAL_REG_SIZE			1024*4
#define 			INT_BASE					0xF3001000
#define 			CKC_BASE					0xF3000000

#define 			IMAGE_PHY_MMAP_ADDR 		HW_MEM_BASE
#define 			IMAGE_PHY_MMAP_LEN 		(1024 * 1024 * 4)
#endif
#if	defined(TCC_89XX_INCLUDE) || defined(TCC_93XX_INCLUDE) || defined(TCC_88XX_INCLUDE)
#define 			PHYSICAL_REG_SIZE			1024*4
#if	defined(TCC_89XX_INCLUDE) || defined(TCC_88XX_INCLUDE)
#define 			INT_BASE					0xF0401000
#define 			CKC_BASE					0xF0400000
#endif
#define				MAX_STREAM_BUF			100   // kbyte unit
#define 			IMAGE_PHY_MMAP_ADDR 		0x45E00000
#define 			IMAGE_PHY_MMAP_LEN 		(1024 * 1024 * 30)
#endif

#define 			PHY_TEMPYUV_ADDR			IMAGE_PHY_MMAP_ADDR;
#define 			CLIP(x) 						(((x) < 0) ? 0 : (((x) > 255) ? 255 : (x)))

#define FBDEV_FILENAME  "/dev/fb0" 

#define IMAGE_DEFAULT_WIDTH		DEFAULT_WIDTH
#define IMAGE_DEFAULT_HEIGHT	DEFAULT_HEIGHT
#define IMAGE_DEC_WIDTH			DEFAULT_WIDTH*2
#define IMAGE_DEC_HEIGHT		DEFAULT_HEIGHT*2


enum
{
	CODEC_JPEG =0,
	CODEC_BMP,
	CODEC_PNG,
	CODEC_GIF,
	PHOTO_NUMCODECS
};

enum
{ 
	ZOOM_IN_LEV4 = 0,
	ZOOM_IN_LEV3,		
	ZOOM_IN_LEV2,	
	ZOOM_IN_LEV1,	
	ZOOM_NONE,
	ZOOM_OUT_LEV1,	
	ZOOM_OUT_LEV2,	
	ZOOM_OUT_LEV3,	
	ZOOM_OUT_LEV4,	
	ZOOM_VALUE_MAX
};	


enum
{ 
	ROTATE_NONE = 0,		
	ROTATE_NONE_R1,	//90
	ROTATE_NONE_R2,	//180
	ROTATE_NONE_R3,	//270	
	ROTATE_VALUE_MAX
};	


enum{
	PHY_MEMORY_NO_ERROR = 0,
	PHY_MEMORY_OPEN_ERR,	
	PHY_MEMORY_MALLOC_ERR,
	PHY_MEMORY_REG_ERROR 
};


enum{
	DEC_OUT_FORMAT_YUV = 0,
	DEC_OUT_FORMAT_RGB
};


#endif

