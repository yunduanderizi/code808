#pragma once
#if 0 //def PRODUCT_TUI

/**
  @file
  Camera driver API for Kinpo's camera driver
*/

#ifndef __CAMERA_APIKINPO_H__
#define __CAMERA_APIKINPO_H__

#define MAX_FRAMES 3

//-----------IO control code description-----------------------------

#define SEN_OPENCAMERA				0x10
#define SEN_CLOSECAMERA				0x11

#define SEN_STARTPREVIEW			0x12
#define SEN_STOPPREVIEW				0x13

#define SEN_SET_CAM_POS_SIZE		0x14

#define SEN_CAPTUREIMAGE			0x15

#define SEN_SETWHITEBALANCE			0x16
#define SEN_GETWHITEBALANCE			0x17

#define SEN_SETEXPOSURE				0x18
#define SEN_GETEXPOSURE				0x19

#define SEN_SET_IMAGE_FORMAT		0x1a

#define SEN_SET_REGISTER			0x1b
#define SEN_GET_REGISTER			0x1c

///////////////////
typedef struct
{
	DWORD VirtAddr;	// Address of the 4K aligned DMA buffer, derived from pBuf.
	int size;	// size of the buffer
	int BufferID;		// a buffer ID used to identify this buffer to the driver
	DWORD *pY;
	DWORD *pCb;
	DWORD *pCr;
	DWORD *pBuf;  // Address of the DMA buffer returned from a call to malloc().
} CAMERA_DMA_BUFFER_INFO, *P_CAMERA_DMA_BUFFER_INFO;

//-----------structure for communication with application -----------
typedef struct _CAM_POS_SIZE
{
UINT16 Size; // size of CAM_POS_SIZE
UINT16 nPos_x; // preview start position: (nPos_x, nPos_y)
UINT16 nPos_y;
UINT16 nSize_x_pr; // preview image size = (nSize_x_pr*nSize_y_pr)
UINT16 nSize_y_pr;
UINT16 nSize_x_cp; // capture image size = (nSize_x_cp*nSize_x_cp)
UINT16 nSize_y_cp;
} CAM_POS_SIZE, *PCAM_POS_SIZE;
/////////////////////////

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

// wb setting mode
typedef enum _IQ_MANUALWB_MODE
{
	IQ_WB_AUTO              	= (char )0x00,
	IQ_WB_SUN               	= (char )0x01,
	IQ_WB_CLOUDY            	= (char )0x02,
	IQ_WB_TUNGSTEN          	= (char )0x03,
	IQ_WB_FLUORESCENT			= (char )0x04,  
	IQ_WB_FLASHLIGHT			= (char )0x05,      
	IQ_WB_NIGHT					= (char )0x06, 
} IQ_MANUALWB_MODE;

typedef struct _get_wb
{
	unsigned short wbMode;
	unsigned short Rgain;
	unsigned short Ggain;
	unsigned short Bgain;
}get_wb,*Pget_wb;


/* camera api */

// open the camera driver and initialise, allocate memory buffers safe to call if already open.
void CAM_OPEN();
/** close camera driver handle and free memory */
void CAM_CLOSE();
/** starts sending viewfinder images */
void CAM_PREVIEW_START();
/** stops sending viewfinder images */
void CAM_PREVIEW_STOP();
/* set preview size,preview position and capture size, */
void SET_CAM_POS_SIZE( CAM_POS_SIZE values);
/** takes raw data,and saves it */
void CAM_CAPTURE();
/** set white balance mode */
void CAM_SET_WHITEBALANCE(int wbtype);
/** get white balance mode and wb value */
void CAM_GET_WHITEBALANCE(int *wbtype,int *Rgain,int *Ggain,int *Bgain );
/** set lumilance level */
void CAM_SET_EXPOSURE(int exposurelevel);
/** get lumilance level */
void CAM_GET_EXPOSURE(int *exposurelevel);


#endif //__CAMERA_APIKINPO_H__

#endif //PRODUCT_TUI
