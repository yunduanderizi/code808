#pragma once
#ifdef PRODUCT_ENDEAVOUR

/*----------------------------------------------------------------------------------------------|
|  Company              : Group Sense PDA Limited                                               |
|  Department           : R&D                                                                   |
|  Section              : SFW                                                                   |
|  Project code         : BD00                                                                  |
|  Module structure     : PUBLIC\COMMON\OAK\CSP\ARM\INTEL\PXA27X\XLLP\INC\xllp_camera_macro.h   |
|  File revision        : NAVMAN BSP V1.0                                                       |
|  Last update date     : 2005.09.27                                                            |
|                                                                                               |
|  History              : V1.0: Camera driver initial release                                   |
|                        V1.01: add night mode                                                  |
|                        V1.02: add effect mode                                                 |
|----------------------------------------------------------------------------------------------*/

#ifndef _XLLP_CAMERA_MACRO_H__
#define _XLLP_CAMERA_MACRO_H__

// Camera capture sizes
#define kCamCapture_160x120		10		// photo capture at 160x120 (QQVGA) Only for smart-phone.
#define kCamCapture_320x240		20		// photo capture at 320x240 (QVGA)
#define kCamCapture_640x480		30		// photo capture at 640x480 (VGA)
#define kCamCapture_1280x960		40		// photo capture at 1280x960(1.3M pixel)
#define kCamCapture_1280x1024		50		// photo capture at 1280x1024(1.3M pixel)
#define kMP4Capture_176x144		60		// Mpeg4 capture at 176x144 (QCIF)#define 


// Camera zoom in levels
#define kZoom_1X				1		// Zoom X1 (No zoom)
#define kZoom_2X				2		// Zoom X2
#define kZoom_4X				4		// Zoom X4 (Reserved, only use for smart-phone)
#define kZoom_8X				8		// Zoom X8 (Reserved, only use for smart-phone)

// Camera width & height
// 1M3 pixel
#define kCamWidth_1280		1280
#define kCamHeight_1024		1024
#define kCamHeight_960		960

// VGA
#define kCamWidth_640		640
#define kCamHeight_480		480
// QVGA
#define kCamWidth_320		320
#define kCamHeight_240		240
// QQVGA
#define kCamWidth_160		160
#define kCamHeight_120		120
// QQQVGA
#define kCamWidth_80		80
#define kCamHeight_60		60
// CIF
#define kCamWidth_352		352
#define kCamHeight_288		288
// QCIF
#define kCamWidth_176		176
#define kCamHeight_144		144

// Max camera width & height
#define kCamMaxWidth		kCamWidth_1280
#define kCamMaxHeight		kCamHeight_1024

// AE (auto exposure) feature definitions
typedef enum 
{
    CamAE_Auto ,    			// auto exposure
    CamAE_Manual,   			// manual exposure
    CamAE_Total
}CamAEFeature;

// ME (manual exposure) feature definition
typedef enum
{
	CamME_Exposure_N4,			// Brightness -4
	CamME_Exposure_N3,			// Brightness -3
	CamME_Exposure_N2,			// Brightness -2
	CamME_Exposure_N1,			// Brightness -1
	CamME_Exposure_0,			// Brightness  0
	CamME_Exposure_P1,			// Brightness +1
	CamME_Exposure_P2,			// Brightness +2
	CamME_Exposure_P3,			// Brightness +3
	CamME_Exposure_P4,			// Brightness +4
	CamME_Total
}	
CamMEFeature;	

// AWB (Auto White Balance)
typedef enum
{
    CamAWB_Auto,				// Auto
    CamAWB_Sunny,				// Sunny
    CamAWB_Cloudy,				// Cloudy
    CamAWB_Fluorescent,			// Fluorescent
    CamAWB_Tungsten,			// Tungsten
//GSPDA 18/08/2005 Remove night environment. it was replaced with other night mode.
//    CamAWB_Night,				// Night mode
    CamAWB_CloudySunny,		// Cloudy and sunny    
    CamAWB_Total				// (Upper Limit)
}CamAWBFeature;

// Image Effects (Color effects)
typedef enum
{
	CamEF_NormalColor,		// Normal Color
	CamEF_BlackAndWhite,	// Black & White
	CamEF_Sepia,			// Sepia
	CamEF_Blush,			// Blush
	CamEF_Negative,		// Negative Color
	CamEF_NegativeBW,		// Negative B/W image
	CamEF_Greenish,		// Greenish image
	CamEF_Reddish,			// Reddish image
	CamEF_Antique,			// Antique image
	CamEF_SharpNormal,	// Sharp Normal
	CamEF_SharpSoft,		// Sharp Soft
	CamEF_SharpSharp,		// Sharp Sharp
	CamEF_Total			// (Upper Limit)
}CamEFFeature;

//Error information for Camera

enum{
    CamNoErr				    = 0x8000,
    CamNotConfigErr			    ,
    CamInvalidIDErr			    ,
    CamParaErr				    ,
    CamOpenErr				    ,
    CamWidthSetErr			    ,
    CamHeightSetErr			    ,
    CamSaveBufferSizeErr	    ,
    CamScaleNotSupportErr	    ,
    CamNoMemoryErr			    ,
    CamCaptureSizeErr		    ,
    CamPreviewModeErr		    ,
    CamZoomLevelErr			    ,
    CamZoomNotSupportErr	    ,
    CamOutputFormatErr		    ,
    CamOutputWindowErr		    ,
    CamMultiShotNotSupportErr	,
    CamInvalidStateErr			,
    CamInvalidInvokeErr			,
    CamCMOSSensorFailureErr     ,
    CamNoDriverFunctionErr		
};
#endif

#endif //PRODUCT_ENDEAVOUR
