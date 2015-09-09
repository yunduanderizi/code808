/*********************************************************
	Project:		ST FST(TDA7540)
	File name:	PUBLIC.H
	Description:
	Write/Modify:	Perry
	Time:		2005-11-21- 12:19:56
*********************************************************/
#define EON_ENABLE
#define RT_ENABLE
//#define TMC_OUTPUT
//#define NOT_SAVE_BAND_AF
#define CDMODE_ENABLE
#define TDA7719
//#define NAVMODE_ENABLE
//#define BTMODE_ENABLE
#define BASE_VERSION "   ST CRN V2.1 "

#ifdef NAVMODE_ENABLE
	#define BASE_VERSION1 BASE_VERSION"n"
#else
	#define BASE_VERSION1 BASE_VERSION" "
#endif

/********************************************************************************************
* Set MCU_STM8=1, MCU_ST7=0,STM8_80PIN=1, STM8_64PIN=0,to select 80pin of STM8
* Set MCU_STM8=1, MCU_ST7=0,STM8_80PIN=0, STM8_64PIN=1,to select 64pin of STM8
* Set MCU_STM8=0, MCU_ST7=1,STM8_80PIN=X, STM8_64PIN=X,to select 64pin of ST7Audio,or ST72321
*********************************************************************************************/
#define MCU_STM8		1
#define MCU_ST7			0
#define STM8_80PIN		1
#define STM8_64PIN		0

/********************************************************************************************
// Set HIT_TUNER=1,TDA7540_TUNER=X,TDA7541_TUNER=X, to select HIT_TUNER
// Set HIT_TUNER=0,TDA7540_TUNER=1,TDA7541_TUNER=0, to select TDA7540 TUNER
// Set HIT_TUNER=0,TDA7540_TUNER=0,TDA7541_TUNER=1, to select TDA7541 TUNER
// Set HIT_TUNER=0,TDA7540_TUNER=1,TDA7541_TUNER=1, Let software to auto_config TDA7540 or TDA7541 tuner
*********************************************************************************************/
#define HIT_TUNER 		1
#define TDA7540_TUNER 	1
#define TDA7541_TUNER 	1

#define HIT_44
//#define  HIT_64


#if (HIT_TUNER)
#ifdef  HIT_44
	#define VERSION BASE_VERSION1"h44"
#else
      	#define VERSION BASE_VERSION1"h64"
#endif
#else
	#define VERSION BASE_VERSION1" "
#endif



/**********************************************************************
	HIT communication  configure
***********************************************************************/
//#define HIT_SPI_BOOT		1
#define HIT_I2C_C2_BOOT		1
//#define HIT_I2C_C8_BOOT	1

/**********************************************************************
	HIT application configure
***********************************************************************/
#define HIT_FM_NARROWBAND		0		/*HIT module type: 1---narrow band module with varicap, 0---wide band module without varicap*/
#define HIT_ADA					0		/*HIT silicon type: 1---HIT ADA silicon version(new), 0---HIT ABB silicon version(old)*/
#define HIT_ADB                 1

/**********************************************************************
	HIT seek method configure
---switch between usage automatic seek method and usage assisted manual seek method for HIT
***********************************************************************/
//#define AUTOSEEK




#if (HIT_TUNER)
#include "radio_hit.h"
#else
#include "radio.h"
#endif

/*********************************************************
				File End
*********************************************************/
