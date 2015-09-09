#pragma once
/***********************************************************
MiTAC Int'l Corp. 2006
NAME: Camera.h
DESC: header file for Camera
Version: R00_000_20070627

************************************************************/

#ifndef __CAMERA_APIMITAC2_H__
#define __CAMERA_APIMITAC2_H__

#if defined(HAS_FEATURE_CAMERA_HARDWARE) && (FEATURE_CAMERA_HARDWARE==2)

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

#define U8 unsigned char
#define U16 unsigned short
#define U32 unsigned int

//-----------structure for communication with application -----------
//-------------------structure for comunication with application
typedef struct _CAM_INFO
{
  U16 Size;                           // size of CAM_INFO
  U16 nSize_x_src;                    // original input image size = nSize_x_src * nSize_y_src  
  U16 nSize_y_src;
  U16 nSize_x_pre;                    // preview image size = nSize_x_pre * nSize_y_pre
  U16 nSize_y_pre;
  U16 nSize_x_cap;                    // capture image size = nSize_x_cap * nSize_y_cap
  U16 nSize_y_cap;
  U16 nPos_x;
  U16 nPos_y;
  U16 nWinHorOfst;
  U16 nWinVerOfst;
  U16 mirror_rotation_codec;
  U16 mirror_rotation_preview;
  U16 status_cap;
  U16 status_pre;
  U32 display_virtual_address;    // display frame buffer virtual addres
  U8 Reserved;
  U16 status_outputformat;
  BYTE OperationMode;                 //preview, capture or standby
  U16 nWinHorOfst2;
  U16 nWinVerOfst2;
}CAM_INFO, *PCAM_INFO;


typedef struct _CAM_POS_SIZE {
  U16 Size; // size of CAM_SIZE
  U16 nPos_x;     // preview start postion: (nPos_x, nPos_y)
  U16 nPos_y;
  U16 nSize_x_pr; // preview image size = (nSize_x_pr*nSize_y_pr)
  U16 nSize_y_pr;
  U16 nSize_x_cp; // capture image size = (nSize_x_cp*nSize_x_cp)
  U16 nSize_y_cp;
  U16 Reserved1;
  U16 Reserved2;
  U8 Reserved3;
  U8 Reserved4;
} CAM_POS_SIZE, *PCAM_POS_SIZE;

//-----------IO control code description-----------------------------

// get camera information
// pBufIn = PCAM_INFO, dwLenIn = sizeof(CAM_INFO)
#define IOCTL_CAM_GET_INFO		CTL_CODE( FILE_DEVICE_VIDEO, 1, METHOD_OUT_DIRECT,FILE_ANY_ACCESS)

// set preview position 
// pBufIn = PCAM_POS_SIZE, dwLenIn = sizeof(CAM_POS_SIZE)
#define IOCTL_CAM_SET_POS_SIZE	CTL_CODE( FILE_DEVICE_VIDEO, 2, METHOD_NEITHER,FILE_ANY_ACCESS)
/* Note:
The resolutions supported are as below:
Preview resolutions: 128x96, 176x144, 320x240, 336x252
Capture resolutions: 128x96, 176x144, 320x240, 336x252, 400x300, 640x480, 800x600, 1280x960, 1600x1200
*/


// start preview
#define IOCTL_CAM_START			CTL_CODE( FILE_DEVICE_VIDEO, 3, METHOD_NEITHER,FILE_ANY_ACCESS)

// stop preview
#define IOCTL_CAM_STOP			CTL_CODE( FILE_DEVICE_VIDEO, 4, METHOD_NEITHER,FILE_ANY_ACCESS)

// copy camera image
// pBufIn = 1: copy and stop preview and capture
// dwLenIn = sizeof(BYTE)
// pBufOut will store image data, dwLenOut = size of pBufOut
#define IOCTL_CAM_COPY_CAPTURE	CTL_CODE( FILE_DEVICE_VIDEO, 5, METHOD_OUT_DIRECT,FILE_ANY_ACCESS)

// set brightness
// pBufIn = PBYTE, dwLenIn = sizeof(BYTE)
// value from 0 to 7.
#define IOCTL_CAM_SETBRIGHTNESS CTL_CODE( FILE_DEVICE_VIDEO, 6, METHOD_NEITHER,FILE_ANY_ACCESS)

//Set saturation
#define IOCTL_CAM_SETSATURATION      CTL_CODE(FILE_DEVICE_VIDEO, 7, METHOD_NEITHER, FILE_ANY_ACCESS)

// copy camera last frame preview image
// pBufOut will store image data, dwLenOut = size of pBufOut
// format will be RGB:565.
#define IOCTL_CAM_COPY_PREVIEW	CTL_CODE( FILE_DEVICE_VIDEO, 8, METHOD_IN_DIRECT,FILE_ANY_ACCESS)

//refresh the preview 
#define IOCTL_CAM_REFRESH		CTL_CODE( FILE_DEVICE_VIDEO, 9, METHOD_OUT_DIRECT,FILE_ANY_ACCESS)

// set zoom
// pBufIn = PBYTE, dwLenIn = sizeof(BYTE)
// 0: x1  1: x1.25  2: x2  3: x2.5  4: x4
#define CAM_IOCTL_ZOOM CTL_CODE( FILE_DEVICE_VIDEO, 10, METHOD_IN_DIRECT,FILE_ANY_ACCESS)

// set digital effect
// pBufIn = PBYTE, dwLenIn = sizeof(BYTE)
// 0: Normal, 1: Mono, 2: Sepia
#define IOCTL_CAM_SETDIGITALEFFECT CTL_CODE( FILE_DEVICE_VIDEO, 11, METHOD_IN_DIRECT,FILE_ANY_ACCESS)


// set operation mode
// pBufIn = PBYTE, dwLenIn = sizeof(BYTE)
// 0: Preview, 1: Capture, 2: Video
#define IOCTL_CAM_SETOPERATIONMODE CTL_CODE( FILE_DEVICE_VIDEO, 12, METHOD_IN_DIRECT,FILE_ANY_ACCESS)

// set flick detect
// pBufIn = PBYTE, dwLenIn = sizeof(BYTE)
// 1: 50 Hz (on), 2: 60 Hz (on)
#define IOCTL_CAM_SETFLICKDETECT CTL_CODE( FILE_DEVICE_VIDEO, 13, METHOD_IN_DIRECT,FILE_ANY_ACCESS)

// set scenery mode
// pBufIn = PBYTE, dwLenIn = sizeof(BYTE)
// 0: Normal, 1: Sport, 2: Night
#define IOCTL_CAM_SETSCENARYMODE CTL_CODE( FILE_DEVICE_VIDEO, 14, METHOD_IN_DIRECT,FILE_ANY_ACCESS)


//Set/Stop AutoFocus
// pBufIn = PBYTE, dwLenIn = sizeof(BYTE)
// 0: Stop AutoFocus, 1: Set AutoFocus
#define IOCTL_CAM_SETAUTOFOCUS CTL_CODE( FILE_DEVICE_VIDEO, 15, METHOD_IN_DIRECT,FILE_ANY_ACCESS)
/* If you have an AF button, you should set autofocus if the button is pressed 
 * then stop autofocus when the button is released. 
 * If you have no AF button, you need set autofocus before changing 
 * the operation mode to CAPTURE mode, then stop AF after retrieving the image data.
 */

//Write Registers through IIC (only for test IIC function)
#define IOCTL_CAM_REG_WRITE          CTL_CODE(FILE_DEVICE_VIDEO, 25, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

//Read Registers through IIC (only for test IIC function)
#define IOCTL_CAM_REG_READ           CTL_CODE(FILE_DEVICE_VIDEO, 26, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

//-------------------------------------------------------------------


// set White balance coordinate
// pBufIn = PBYTE, dwLenIn = sizeof(BYTE)
// 0: Auto, 1: Indoor, 2: Outdoor
#define IOCTL_CAM_SETWBCOORD	CTL_CODE( FILE_DEVICE_VIDEO, 22, METHOD_IN_DIRECT,FILE_ANY_ACCESS)

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
#endif //__CAMERA_APIMITAC2_H__
