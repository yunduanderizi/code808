#pragma once
#ifdef PRODUCT_ENDEAVOUR

/*----------------------------------------------------------------------------------------------|
|  Company              : Group Sense PDA Limited                                               |
|  Department           : R&D                                                                   |
|  Section              : SFW                                                                   |
|  Project code         : BD00                                                                  |
|  Module structure     : camera_api.h                                                          |
|  File revision        : NAVMAN BSP V1.0                                                       |
|  Last update date     : 2005.09.27                                                            |
|                                                                                               |
|  History              :  V1.0: Camera API initial release                                     |
|                         V1.01: Support 1280x1024 resolution                                   |
|                         V1.02: add night mode                                                 |
|                         V1.03: add effect mode                                                |
|                                                                                               |
|-----------------------------------------------------------------------------------------------*/

#ifndef __CAMERA_MAINSTONEII_API_H__
#define __CAMERA_MAINSTONEII_API_H__

#define MAX_FRAMES 3

// Use the following for configuring YUV overlay format
#define FORMAT_RGB			0x0
#define FORMAT_PACKED_444	0x1
#define FORMAT_PLANAR_444	0x2
#define FORMAT_PLANAR_422	0x3
#define FORMAT_PLANAR_420	0x4

#define O_BPP_16 			0x4

typedef struct
{
	unsigned int VideoWidth;		// app fills this in -- reserved (set 0)
	unsigned int VideoHeight;		// app fills this in -- reserved (set 0)  
	unsigned long StillWidth;		// app fills this in
	unsigned long StillHeight;		// app fills this in
    unsigned int  CaptureSizes;     //Camera capture sizes, it is defined in xllp_Camera_macro.h  e.g.: //kCamCapture_1280x1024
	unsigned int PreviewX;		//app fills this in (the preview image x coordinate.
	unsigned int PreviewY;		//app fills this in (the preview image x coordinate.
	unsigned int PreviewWidth;		// The display region //it must be 80x60, 160x120, 320x240, 640x480, 1280x960
	unsigned int PreviewHeight;	// The display region
	unsigned int PreviewWinWidth;	 //for camera driver used (set 0)
	unsigned int PreviewWinHeight; //for camera driver used (set 0)
	unsigned int PreviewSubSample; //for camera driver used (set 0)
    unsigned int ZoomX;            // Zoom xx
	unsigned int CaptureFormat;	// app fills this in
	unsigned int FrameRate;		// app fills this in
	void *ring_buffer_address[MAX_FRAMES]; // driver fills this in
	BOOL	 bFrame_available;		// driver maintains this
	unsigned char subAddress;
	unsigned char *bufP;
	unsigned int AutoExposure;	 //app fills this in. exposure mode (auto or manual); the macro were defined in CamAEFeature
	unsigned int AutoExposeVal;	 //app fills this in. manual exposure value; the macro were defined in CamMEFeature
	unsigned int AutoWhiteBalance;	 //app fills this in. auto white balance. the macro were defined in CamAWBFeature
	BOOL bNightMode;	//capture in night environment. if yes, set 1, others is 0, default value is 0. the frame number is 4 in this mode.
	unsigned int Effect;	//image effect. e.g.:Black/white, Sepia, Bluish, Negative B/W , Greenish, Reddish, Antique ect.
  unsigned short *pPreviewData; // ptr to preview bmp buffer
} CAMERA_APP_T, *P_CAMERA_APP_T;

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

// Intel proprietary camera API IOCTLs
#define ESCAPECODEBASE							100000
#define CAMERA_INITIALIZE						(ESCAPECODEBASE + 12)
#define CAMERA_START_VIDEO_CAPTURE			(ESCAPECODEBASE + 13)	//reserved
#define CAMERA_STOP_VIDEO_CAPTURE			(ESCAPECODEBASE + 14)	//reserved
#define CAMERA_RESERVED						(ESCAPECODEBASE + 15)	//reserved
#define CAMERA_CAPTURE_STILL_IMAGE			(ESCAPECODEBASE + 16)
#define CAMERA_WRITE_REGISTER					(ESCAPECODEBASE + 17)	//reserved
#define CAMERA_READ_REGISTER					(ESCAPECODEBASE + 18)	//reserved
#define CAMERA_SHUTDOWN						(ESCAPECODEBASE + 19)
#define CAMERA_CHANGE_CAPTURE_FORMAT		(ESCAPECODEBASE + 20)
#define CAMERA_PREPARE_VIDEO_BUFFER			(ESCAPECODEBASE + 21)	//reserved
#define CAMERA_RESERVED1						(ESCAPECODEBASE + 22)	//reserved
#define CAMERA_SUBMIT_VIDEO_BUFFER			(ESCAPECODEBASE + 23)	//reserved
#define CAMERA_PREPARE_STILL_IMAGE_BUFFER	(ESCAPECODEBASE + 24)
#define CAMERA_SUBMIT_STILL_IMAGE_BUFFER	(ESCAPECODEBASE + 25)
#define CAMERA_SET_WHITE_BALANCE				(ESCAPECODEBASE + 26)
#define CAMERA_SET_EXPOSURE					(ESCAPECODEBASE + 27)
#define CAMERA_START_PREVIEW					(ESCAPECODEBASE + 28)
#define CAMERA_STOP_PREVIEW					(ESCAPECODEBASE + 29)
#define CAMERA_PREPARE_PREVIEW_BUFFER		(ESCAPECODEBASE + 30)	//Driver use
#define CAMERA_SUBMIT_PREVIEW_BUFFER		(ESCAPECODEBASE + 31)	//Driver use
#define CAMERA_RESUME_PREVIEW				(ESCAPECODEBASE + 32)
#define CAMERA_SET_NIGHT_ENVIRONMENT		(ESCAPECODEBASE + 33)
#define CAMERA_GET_PREVIEW_DATA				(ESCAPECODEBASE + 34)
#define CAMERA_SET_EFFECT						(ESCAPECODEBASE + 35)

BOOL CameraIoControl(
  HANDLE hDevice, 
  DWORD dwIoControlCode, 
  LPVOID lpInBuffer, 
  DWORD nInBufferSize, 
  LPVOID lpOutBuffer, 
  DWORD nOutBufferSize, 
  LPDWORD lpBytesReturned, 
  LPOVERLAPPED lpOverlapped
);
#endif //__CAMERA_MAINSTONEII_API_H__

#endif //PRODUCT_ENDEAVOUR
