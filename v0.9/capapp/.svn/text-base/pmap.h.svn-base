/******************************************************************************
 *
*  (C)Copyright All Rights Reserved by Telechips Inc.
 *
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
 *
*   FileName    : pmap.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef __PMAP_H
#define __PMAP_H
/******************************************************************************
* include 
******************************************************************************/


// The defined Pmap file name is show with " cat /proc/pmap"
// Current Pmap list is below

/******************************************************************************
* defines 
******************************************************************************/

#define PATH_PROC_PMAP	"/proc/pmap"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



#define TCC_PMAP_PMEM			"pmem"
#define TCC_PMAP_VIQE			"viqe"
//#define TCC_PMAP_CAMERA		"camera"
#define TCC_PMAP_CAMERA		"ump_reserved"
#define TCC_PMAP_OVERLAY		"overlay"
#define TCC_PMAP_OVERLAY1		"overlay1"
#define TCC_PMAP_EXT_CAMERA	"ext_camera"
#define TCC_PMAP_VIDEO			"video"
#define TCC_PMAP_FB_VIDEO		"fb_video"
#define TCC_PMAP_FB_SCALE		"fb_scale"
#define TCC_PMAP_FB_SCALE0		"fb_scale0"
#define TCC_PMAP_FB_SCALE1		"fb_scale1"
#define TCC_PMAP_FB_G2D0		"fb_g2d0"
#define TCC_PMAP_FB_G2D1		"fb_g2d1"
#define TCC_PMAP_VIDEO_DUAL	"video_dual"
#define TCC_PMAP_JPEG_HEADER	"jpeg_header"
#define TCC_PMAP_JPEG_RAW		"jpeg_raw"
#define TCC_PMAP_JPEG_STREAM	"jpeg_stream"
#define TCC_PMAP_TOTAL			"total"





/******************************************************************************
* typedefs & structure
******************************************************************************/
typedef struct 
{
    unsigned int base;
    unsigned int size;
} pmap_t;

/******************************************************************************
* globals
******************************************************************************/


/******************************************************************************
* locals
******************************************************************************/

/******************************************************************************
* declarations
******************************************************************************/
int pmap_get_info(const char *name, pmap_t *mem);

#endif //__PMAP_H



