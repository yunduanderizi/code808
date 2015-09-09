#pragma once
#ifdef PRODUCT_ENDEAVOUR

/*----------------------------------------------------------------------------------------------|
|  Company              : Group Sense PDA Limited                                               |
|  Department           : R&D                                                                   |
|  Section              : SFW                                                                   |
|  Project code         : BD00                                                                  |
|  Module structure     : CamDrv_imagetools.h                                                   |
|  File revision        : NAVMAN BSP V1.0                                                       |
|  Last update date     : 2005.08.05                                                            |
|                                                                                               |
|  History              : V1.0: Camera driver initial release                                   |
|----------------------------------------------------------------------------------------------*/

#include <windows.h>

#ifndef __CAMDRV_IMAGETOOLS_H_
#define __CAMDRV_IMAGETOOLS_H_

#define CAMDRV_CAMERA_OVERLAY2 0
#define CAMERA_PREVIEW_SAMEAS_CAPTURE 1

#define FastRGB(Red, Green, Blue) (((UINT16)(Red)   & 0xF8) << 8 | ((UINT16)(Green) & 0xFC) << 3 | ((UINT16)(Blue)  & 0xF8) >> 3 )
 
//For write data to Overlay begin
typedef struct
{
	unsigned long OverlayHeight;	// app fills this in
	unsigned long OverlayWidth;		// app fills this in
	unsigned long X_Position;		// app fills this in
	unsigned long Y_Position;		// app fills this in
	unsigned long Format;			// app fills this in
	unsigned long DegradeBaseFrame; // app fills this in
	unsigned long CH2_Y;			// driver fills this in
	unsigned long CH3_Cb;			// driver fills this in
	unsigned long CH4_Cr;			// driver fills this in
	unsigned long OverlayBPP;		// driver fills this in
	unsigned long TmpBPP;			// driver fills this in
	unsigned long ch2_size;			// driver fills this in
	unsigned long ch3_size;			// driver fills this in
	unsigned long ch4_size;			// driver fills this in
} CAMERA_OVERLAY_T, *P_CAMERA_OVERLAY_T;

// GSPDA OEM escape code base	refer to ltp400wq.h
#define ESCAPECODEBASE				 100000
#define OVERLAY2_ENABLE				 (ESCAPECODEBASE + 6)
#define OVERLAY2_DISABLE			 (ESCAPECODEBASE + 7)
#define OVERLAY1_ENABLE				 (ESCAPECODEBASE + 8)
#define OVERLAY1_DISABLE			 (ESCAPECODEBASE + 9)
#define GET_OVERLAY1_ADDRESS		 (ESCAPECODEBASE + 10)
#define GET_OVERLAY2_ADDRESS		 (ESCAPECODEBASE + 11)

//refer to xllp_lcd.h
#define O_BPP_16 			0x4
#define FORMAT_RGB			0x0
#define FORMAT_PLANAR_422	0x3
//For write data to Overlay end

//function declare
int RGB2PreviewData(UINT16*pRGBData, UINT16 *pPreviewData);
int YCbCr2RGBPreviewData(UINT8 *pY, UINT8 *pCb, UINT8 *pCr, UINT16 *pPreviewData);
int CameraInitDirectDrawLCDFrame(void);

#if CAMDRV_CAMERA_OVERLAY2
int CameraInitOverlay2(void);
int CameraEnableOverlay2(void);
int CameraDisableOverlay2(void);
int YCbCrZoomOutX(UINT8 *pY, UINT8 *pCb, UINT8 *pCr);
#endif	//CAMDRV_CAMERA_OVERLAY2
#endif //__CAMDRV_IMAGETOOLS_H_

#endif //PRODUCT_ENDEAVOUR
