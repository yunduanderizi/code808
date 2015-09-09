#pragma once
/***********************************************************
MiTAC Int'l Corp. 2006
NAME: Camera.h
DESC: header file for Camera
HISTORY:  2006.05.23 version:0.1

************************************************************/

#ifndef __CAMERA_APIMITAC_H__
#define __CAMERA_APIMITAC_H__

#if defined(HAS_FEATURE_CAMERA_HARDWARE) && (FEATURE_CAMERA_HARDWARE==1)

#ifdef _WIN32_WCE
#include "windev.h"
#endif
#define MAX_FRAMES 3


typedef struct
{
	DWORD VirtAddr;		// Address of the 4K aligned DMA buffer, derived from pBuf.
	int	 size;			// size of the buffer
	int BufferID;		// a buffer ID used to identify this buffer to the driver
	DWORD *pY;
	DWORD *pCb;
	DWORD *pCr;
	DWORD *pBuf;		// Address of the DMA buffer returned from a call to malloc().
} CAMERA_DMA_BUFFER_INFO, *P_CAMERA_DMA_BUFFER_INFO;

//-----------structure for communication with application -----------
typedef struct _CAM_INFO 
{ 
  UINT16 Size; // size of CAM_INFO 
  UINT16 nSize_x_src; // original input image size =(nSize_x_src*nSize_y_src) 
  UINT16 nSize_y_src; 
  UINT16 nSize_x_pr; // preview image size = (nSize_x_pr*nSize_y_pr) 
  UINT16 nSize_y_pr; 
  UINT16 nSize_x_cp; // capture image size = (nSize_x_cp*nSize_x_cp) 
  UINT16 nSize_y_cp; 
  UINT16 nPos_x; // preview start postion: (nPos_x, nPos_y) 
  UINT16 nPos_y; 
  UINT16 Reserved1; 
  UINT16 Reserved2; 
  UINT16 Reserved3; 
  UINT16 Reserved4; 
  UINT16 Reserved5; 
  UINT16 Reserved6; 
  UINT16 Reserved7; 
  UINT16 Reserved8; 
  UINT16 Reserved9; 
} CAM_INFO, *PCAM_INFO;


typedef struct _CAM_POS_SIZE 
{ 
  UINT16 Size; // size of CAM_POS_SIZE 
  UINT16 nPos_x; // preview start position: (nPos_x, nPos_y) 
  UINT16 nPos_y; 
  UINT16 nSize_x_pr; // preview image size = (nSize_x_pr*nSize_y_pr) 
  UINT16 nSize_y_pr; 
  UINT16 nSize_x_cp; // capture image size = (nSize_x_cp*nSize_x_cp) 
  UINT16 nSize_y_cp; 
  UINT16 Reserved1; 
  UINT16 Reserved2; 
  UINT8 Reserved3; 
  UINT8 Reserved4; 
} CAM_POS_SIZE, *PCAM_POS_SIZE;


//-----------IO control code description-----------------------------

// get camera information
// pBufIn = PCAM_INFO, dwLenIn = sizeof(CAM_INFO)
#define IOCTL_CAM_GET_INFO		CTL_CODE( FILE_DEVICE_VIDEO, 1, METHOD_OUT_DIRECT,FILE_ANY_ACCESS)

// set preview position 
// pBufIn = PCAM_POS_SIZE, dwLenIn = sizeof(CAM_POS_SIZE)
#define IOCTL_CAM_SET_POS_SIZE	CTL_CODE( FILE_DEVICE_VIDEO, 2, METHOD_NEITHER,FILE_ANY_ACCESS)

// start preview
#define IOCTL_CAM_START			CTL_CODE( FILE_DEVICE_VIDEO, 3, METHOD_NEITHER,FILE_ANY_ACCESS)

// stop preview
#define IOCTL_CAM_STOP			CTL_CODE( FILE_DEVICE_VIDEO, 4, METHOD_NEITHER,FILE_ANY_ACCESS)

// copy camera image
// pBufIn = 1: copy and stop preview and capture
// dwLenIn = sizeof(BYTE)
// pBufOut will store image data, dwLenOut = size of pBufOut
#define IOCTL_CAM_COPY			CTL_CODE( FILE_DEVICE_VIDEO, 5, METHOD_OUT_DIRECT,FILE_ANY_ACCESS)

// copy camera last frame preview image
// pBufOut will store image data, dwLenOut = size of pBufOut
#define IOCTL_CAM_COPY_PREVIEW	CTL_CODE( FILE_DEVICE_VIDEO, 8, METHOD_IN_DIRECT,FILE_ANY_ACCESS)

//refresh the preview 
#define IOCTL_CAM_REFRESH		CTL_CODE( FILE_DEVICE_VIDEO, 9, METHOD_OUT_DIRECT,FILE_ANY_ACCESS)

//-------------------------------------------------------------------

// set brightness
#define IOCTL_CAM_SETBRIGHTNESS CTL_CODE( FILE_DEVICE_VIDEO, 6, METHOD_NEITHER,FILE_ANY_ACCESS)

// set White balance coordinate
// pBufIn = PMODE, dwLenIn = sizeof(MODE)
#define IOCTL_CAM_SETWBCOORD	CTL_CODE( FILE_DEVICE_VIDEO, 22, METHOD_IN_DIRECT,FILE_ANY_ACCESS)

//-------------------------------------------------------------------

// set zoom
// pBufIn = PCAM_ZOOM, dwLenIn = sizeof(PCAM_ZOOM)
#define CAM_IOCTL_ZOOM			CTL_CODE( FILE_DEVICE_VIDEO, 10, METHOD_IN_DIRECT,FILE_ANY_ACCESS)

// enum for luminance offset
typedef enum LUMAOFFSET
{
  luma0=0,
  luma1=1,
  luma2=2,
  luma3=3,
  luma4=4,
  luma5=5,
  luma6=6,
  luma7=7
}_LUMAOFFSET, *PLUMAOFFSET;

typedef enum MODE
{
  auto_mode,
  Sunny_mode,
  Cloudy_mode,
  Fluorescent_mode,
  Incandescent_mode,
  Night_mode
}_MODE, *PMODE;

#endif // #if defined(HAS_FEATURE_CAMERA_HARDWARE)
#endif //__CAMERA_APIMITAC_H__
