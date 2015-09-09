/*********************************************************
	Project:		ST HIT Radio
	File name:	radio.h
	Description:
	Write/Modify:	Yete HUANG
	Time:		April-2008
*********************************************************/
extern void RadioInit(void);
extern void RadioOn(void);
extern void RadioOff(void);
extern void RadioMain(void);
extern void RadioSetFreq(unsigned int setfreq);
extern void RadioTunerSend(unsigned char start, unsigned char end);
extern void RadioTunerRead(void);
extern void RadioKey(void);
extern void RadioWork(void);
extern unsigned int  RadioFreqCal(unsigned char step);
extern void Radio10msTimer(void);
extern void Radio100msTimer(void);
extern void RadioDisplay(void);
extern void RadioSave(unsigned char pos);
extern void RadioLoad(unsigned char pos);
extern void SeekOn(void);
extern void SeekOff(unsigned char SeekEndAction);
extern void RadioMute(unsigned char type);
extern void PresetOrder(void);
#ifndef NOT_SAVE_BAND_AF
extern void RDSSaveBandAF(void);
extern void RDSLoadBandAF(void);
#endif

extern void HIT_BootcodeDownload(void);
extern void HIT_Startup(void);
extern void HIT_DSPMode(unsigned char DspWorkMode);
extern void HIT_GetCFQuality (unsigned char AverageNum);
extern void HIT_CmdWriteRead(unsigned char cmdcode,  unsigned char para_num, unsigned char rece_num);
extern void HIT_DirectWrite(unsigned char DataNum, unsigned char mode, unsigned char cmd);//bool cmd)
extern void HIT_DirectRead(unsigned int DataNum, unsigned char mode);
extern void HIT_CmdSetFreq(unsigned int setfreq);
extern void HIT_CmdSetFEReg(unsigned char band, unsigned char start, unsigned char num);
extern void HIT_CmdSetBand(unsigned char band);
extern void HIT_CmdSeekTH(unsigned long MeasCycle, unsigned long fieldstrengthTH, unsigned long AdjTH, unsigned long DetuneTH, unsigned long MultipathTH, unsigned long QualityTH);
extern void HIT_CmdStartManualSeek(signed long  FreqIncrement);
extern void HIT_CmdStartAutoSeek(signed long  FreqIncrement, unsigned char AutoSeekEnd);
extern void HIT_CmdSeekEnd(unsigned char SeekEndAction);
extern void HIT_CmdReadTDSR(void);
extern void HIT_CmdReadSeekStatus(void);
extern void HIT_CmdStartAlign(unsigned char RegionCode);
extern void HIT_CmdSetIRType(unsigned char type);
extern void HIT_CmdSetDiss(unsigned char mode, unsigned char bandwidth);
extern unsigned char HIT_CmdReadMem(unsigned long menAddr, unsigned char Numword);
extern unsigned char HIT_CmdWriteMem( unsigned long menAddr, unsigned long menData);
extern void  HIT_CmdDynIS(unsigned char mode);
extern void HIT_CmdSetFE_Reg(unsigned char band, unsigned char start, unsigned char num, unsigned char *preg);
extern void HIT_CmdWriteBeCoeff( unsigned char* p_be_coeff);
extern void HIT_CmdReadFEReg(unsigned char FESubaddress, unsigned char num);

extern unsigned char HIT_StereoIndicator(void);
//extern unsigned char HIT_CombinedQuality (unsigned char fs, unsigned char det, unsigned char ac, unsigned char mp);
extern void HIT_ForeceMono(void);

//HIT communcation functions
#ifdef HIT_SPI_BOOT
extern void SPIStart(void);
extern void SPIStop(void);
extern void SPIOut8Bit(unsigned char outbyte);
extern unsigned char SPIIn8Bit(void);
#else
extern void HIT_EEPromRead(unsigned char address,unsigned long begin,unsigned char length,unsigned char *p);
extern void I2C3Start(void);
extern void I2C3Stop(void);
extern unsigned char I2C3Out8Bit(unsigned char outbyte);
extern unsigned char I2C3In8Bit(void);
extern void I2C3Ack(void);
extern void I2C3Ack2(void);
#endif
extern void HIT_EEpromBootCodeDownload(void);


#define	EXT	extern
#define	ext	extern

#define FM_BAND_NUM 3
#define AM_BAND_NUM 3
#define BAND_NUM  6

typedef struct FMBand_stuct {
	unsigned int BandFreq;
	#ifndef NOT_SAVE_BAND_AF
	unsigned int BandPI;
	unsigned char BandAFList[8];
	unsigned char BandAFNum;
	#endif
	unsigned int MemFreq[6];
	unsigned char MemSmeter[6];
	unsigned int PI[6];
	unsigned char MemAFList[6][8];
	unsigned char MemAFNum[6];
	unsigned char CurrentMem;
} FMBandStuct;

typedef struct AMBand_stuct {
	unsigned int BandFreq;
	unsigned int MemFreq[6];
	unsigned char MemSmeter[6];
	unsigned char CurrentMem;
} AMBandStuct;

typedef struct Radio_stuct {
	unsigned char SeekMode;
	unsigned char WorkMode;
	unsigned char CurrentBand;
	unsigned int ReqFreq;
	unsigned int Freq;
	unsigned int BackupFreq;
	unsigned int ScanBackupFreq;
	unsigned char TunerBuff[40];
	//unsigned int TestPointFreq;
	//signed char TestPointVal;
	//signed int Slope;
	//signed int Slope2;
	FMBandStuct FMBand[FM_BAND_NUM];
	AMBandStuct AMBand[AM_BAND_NUM];

	unsigned int MaxFreq;
	unsigned int MinFreq;
	unsigned char Step;
	unsigned char SeekStep;
	unsigned char Area;
	signed char Direction;

	unsigned char Smeter;
	unsigned char Multipath;
	unsigned char AdjChannel;
	unsigned char Detuning;
	unsigned char HITAddressBuff[3];
	unsigned char HITDataBuff[40];
	unsigned char IFBW;
	unsigned char IS;

	unsigned F_ForceSetBuff:1;
	unsigned F_GoodStation:1;
	unsigned F_SeekFullCycle:1;
	unsigned F_AutoSeek:1;
	unsigned F_TunerDispReq:1;
	unsigned F_ManualSeek:1;
	unsigned F_LocDx:1;
	unsigned F_Scan:1;
	//unsigned F_HIT_SPIComm:1;// ---1: SPI; 0 I2C
	unsigned F_AlignDone:1;// 1: aligne done
	unsigned F_TDBusy:1;// 1---Tuner driver busy
	unsigned F_QualDisplay:1;

/*	float Quality;
	float Qual_fs_factor;
	float Qual_ac_factor;
	float Qual_mp_fator;
	float Qual_det_fator;
*/

} RadioStuct;

 RadioStuct Radio;
 unsigned char RadioWaitTimer;
 unsigned char RadioWorkTimer10ms;
 unsigned char RadioWorkTimer;
 unsigned char RadioSeekMuteTimer;
 unsigned char TempPresetNum;
 unsigned char TempPresetSMMin;
 unsigned char TempPresetSMMinNum;
 unsigned char SeekKeyTimer;
 unsigned char FmIdx;
 unsigned char AmIdx;
 unsigned char flg_i2c_log;
/*

*/


#define RADIO_SEEK_IDLE				0
#define RADIO_SEEK_INIT				1
#define RADIO_SEEK_CHANGE_FRE		2
#define RADIO_SEEK_AMFSU_WAIT	3
#define RADIO_SEEK_WAIT			4
#define RADIO_SEEK_CHECK			5
#define RADIO_SEEK_GOOD_STATION	6
#define RADIO_SEEK_BAD_STATION	7

#define RADIO_WORK_IDLE			0
#define RADIO_WORK_SEEKMODE		0x10
#define RADIO_WORK_SEEKREQ	0x11
#define RADIO_WORK_SEEKWAIT		0x12
#define RADIO_WORK_SEEKOK			0x13
#define RADIO_WORK_SEEKBREAK		0x14
#define RADIO_WORK_SEEKNEXT		0x15
#define RADIO_WORK_SEEKWAITRDS	0x16
#define RADIO_WORK_SEEKWAITPI		0x17
#define RADIO_WORK_SEEKWAITTP	0x18
#define RADIO_WORK_SEEKWAITPTY	0x19
#define RADIO_WORK_SCANWAIT		0x1A
#define RADIO_WORK_SEEKOKWAIT	0x1B


#define RADIO_WORK_ASPSMODE		0x20
#define RADIO_WORK_PSREQ			0x21
#define RADIO_WORK_PSWAIT			0x22
#define RADIO_WORK_PSEND			0x23
#define RADIO_WORK_ASPSBREAK		0x24
#define RADIO_WORK_ASREQ			0x25
#define RADIO_WORK_ASWAIT			0x26
#define RADIO_WORK_ASEND			0x27
#define RADIO_WORK_ASNEXT			0x28
#define RADIO_WORK_ASWAITRDS		0x29
#define RADIO_WORK_ASWAITPI		0x2A
#define RADIO_WORK_ASWAITAFLIST	0x2B

#define RADIO_WORK_STARTUP		0x30
#define RADIO_WORK_WAIT_VCO_FE_ALIGN		0x31
#define RADIO_WORK_VCO_FE_ALIGNDONE		0x32
#define RADIO_WORK_WAIT_IRALIGN		0x33
#define RADIO_WORK_IRALIGNDONE		0x34



//#define RADIO_IN_SEARCH			(Radio.WorkMode==RADIO_WORK_ASWAIT ||Radio.WorkMode==RADIO_WORK_SEEKWAIT ||Radio.WorkMode==RADIO_WORK_SEEKNEXT ||Radio.WorkMode==RADIO_WORK_SEEKWAITRDS ||Radio.WorkMode==RADIO_WORK_SEEKWAITPI)
#define RADIO_IN_SEEK				((Radio.WorkMode>>4)==1)
#define RADIO_IN_AS					(Radio.WorkMode>0x25&&Radio.WorkMode<0x30)
#define RADIO_IN_PS					((Radio.WorkMode>0x20)&&(Radio.WorkMode<0x24))
#define RADIO_IN_SEARCH			(RADIO_IN_SEEK||RADIO_IN_AS)


#define AREA_USA			0
#define AREA_LATIN			1
#define AREA_EUROPE		2
#define AREA_OIRT			3
#define AREA_JAPAN			4

#define LW_FREQ						300
#define AM_FREQ						6450
#define SW_FREQ                    1725

//#define FM_SW_START_FREQ			2300
//#define FM_SW_STOP_FREQ			18135

#define FM_FREQ						6450

#define FM_NORMAL_START_FREQ		8000
#define FM_OIRT_START_FREQ		6450
#define FM_OIRT_STOP_FREQ			7500
#define FM_JAPAN_START_FREQ		7500
#define FM_JAPAN_STOP_FREQ		9100

#define FM_WB_START_FREQ			62300
#define FM_WB_STOP_FREQ			62600

#define FreqInFM(x)				((x)>((AM_FREQ)))
#define FreqInAM(x)				((x)<((FM_FREQ)))
#define FreqInNormalFM(x)			(x>FM_NORMAL_START_FREQ && x<FM_WB_START_FREQ)
#define FreqInOirtFM(x)			((x>FM_OIRT_START_FREQ && x<FM_OIRT_STOP_FREQ)&&Radio.Area==AREA_OIRT)
#define FreqInJapanFM(x)			(x>FM_JAPAN_START_FREQ && x<FM_JAPAN_STOP_FREQ&&Radio.Area==AREA_JAPAN)
#define FreqInWBFM(x)				(x>FM_WB_START_FREQ && x<FM_WB_STOP_FREQ)
#define FreqInMWAM(x)				((x)>LW_FREQ && (x)<SW_FREQ)
#define FreqInLWAM(x)				((x)<LW_FREQ)
#define FreqInSWAM(x)				((x)<FM_FREQ && (x)>SW_FREQ)


#define HIT_STARTUP_IDLE 0X0
#define HIT_STARTUP_REQ 0X1
#define HIT_STARTUP_WAIT_VCO_FE_ALIGN 0X2
#define HIT_STARTUP_WAIT_IRALIGN 0X3
#define HIT_STARTUP_IRALIGNDONE 0X4
ext unsigned char hit_startup_fsm;
ext unsigned char flg_set_diss;
ext unsigned char diss_bw;
#define CHANGE_FM 1
#define CHANGE_AM 2
#define CHANGE_WX 3
#define CHANGE_SW 4
extern void HIT_Startup_Seq();
extern void HIT_Change_Band( unsigned char  bandreq );
#ifdef	DEBUGRADIO
ext unsigned char DebugBuffNum;
ext unsigned char DebugBuffPos;
#endif

//====================================================//
//=================== frequency  define======================//
//====================================================//
//FM frequency define
#define FM80_0MHZ         					8000
#define FM83_0MHZ         					8300
#define FM87_0MHZ         					8700
#define FM106_1MHZ						10610	// FM 106.1MHz
#define FM93_3MHZ						9330	// FM 93.3MHz
#define FM94_5MHZ						9450
#define FM98_6MHZ						9860
#define FM106_9MHZ						10690
#define FM98_1MHZ						9810
#define FM70_1MHZ           				7010
#define FM87_5MHZ						8750
#define FM89_5MHz           				8950
#define FM92_7MHz           				9270
#define FM96_1MHz           				9610
#define FM99_6MHz           				9960
#define FM103_1MHz         				10310
#define FM106_5MHz         				10650
#define FM108MHZ						10800
//ORIT
#define FM65_0MHZ						6500
#define FM67_0MHZ           				6700
#define FM71_0MHZ           				7100
#define FM74_0MHZ           				7400
//JAPAN
#define FM76_0MHZ						7600
#define FM80_0MHZ						8000
#define FM85_0MHZ           				8500
#define FM90_0MHZ						9000
//AM
#define AM530KHZ						530
#define AM900KHZ						900
#define AM720KHZ             				720
#define AM999KHZ						999
#define AM1404KHZ           				1404



#define FM					0
#define AM 					1
#define WB					2
#define SW					3

#ifdef HIT_I2C_C2_BOOT
#define HITTUNER_I2C_ADDR 		0xc2//write: c2; read c3
#else
#define HITTUNER_I2C_ADDR 		0xc8//write: c8; read c9
#endif


#define ModeXX1				0	//No autoinc, no burst, 32bit format
#define ModeXX2				1	//No autoinc, no burst, 24bit format
#define ModeXX3				2	//autoinc, burst, 32bit format
#define ModeXX4				3	//autoinc, burst, 24bit format

#define DSP_IDLE					0x00
#define DSP_FM					0x01
#define DSP_AM_EuJp				0x02
#define DSP_AM_Us				0x03
#define DSP_WX					0x04


//Command

#define CmdCode_ReadXmen		0x01
#define CmdCode_ReadYmen		0x02
#define CmdCode_WriteXmen		0x04
#define CmdCode_WriteYmen		0x05
#define CmdCode_ReadDMAmem    0x00
#define CmdCode_WriteDMAmem   0x03

#define CmdCode_SetFreq			0x08
#define CmdCode_SetFEReg		0x09
#define CmdCode_SetBand		0x0a
#define CmdCode_SetDynIS		0x0b
#define CmdCode_SetSeekTH		0x14
#define CmdCode_StartManuSeek	0x15

#define CmdCode_AFCheck		0x07
#define CmdCode_AFSwitch		0x10
#define CmdCode_AFStart			0x0d
#define CmdCode_AFMeasure		0x0e
#define CmdCode_AFEnd			0x0f

#define CmdCode_StartAlignment	0x0c
#define AlignVCO_EUUSA					0x00//87.5MHz-108MHz
#define AlignVCO_Japan					0x01//76MHz-90MHz
#define AlignVCO							0x02//only align VCO

#define CmdCode_StartAutoSeek	0x16
#define AutoEnd							0x01
#define WaitCmdEnd						0x00

#define CmdCode_SeekEnd		0x17
#define SeekContinue						0x01
#define SeekStopMuted					0x02
#define SeekStopUnmuted					0x03

#define CmdCode_ReadSeekStatus	0x18
#define SeekStatus_RstQaul				0x01
#define SeekStatus_NoRstQaul				0x00
#define SeekStatus_ManualSeek			0x01
#define SeekStatus_AutoSeek				0x02
#define SeekStatus_ASE					0x04
#define SeekStatus_Seekok                          0x01
#define SeekStatus_FullCircle                       0x02

#define CmdCode_ReadTDS		0x11
#define CmdCode_ReadFEReg		0x12
#define CmdCode_ReadRDSQual	0x13

#define CmdCode_IR				0x19
#define OnlineIR_FM						0x00
#define HybridIR_AM						0x01
#define HybridIR_FM						0x02
#define IRalignment						0x03


#define CmdCode_SetDiss			0x1a
#define Diss_FM_Auto						0x00
#define Diss_FM_Manual					0x01
#define Diss_FM_Special					0x02
#define Diss_AM_Neutral					0x0a
#define Diss_WX_Neutral					0x0b
//BW for manual and special FM mode
#define Diss_FM_Manual_BW0						0x00//(narrowest)
#define Diss_FM_Manual_BW1						0x01
#define Diss_FM_Manual_BW2						0x02
#define Diss_FM_Manual_BW3						0x03
#define Diss_FM_Manual_BW4						0x04
#define Diss_FM_Manual_BW5						0x05
#define Diss_FM_Manual_BW6						0x06//(widest)

#define BootData					0x00
#define FMPatchData				0x01
#define WXPatchData			0x02
#define AMPatchData			0x03
#define FMWSPData				0x04
#define AMWSPData				0x05
#define WXWSPData				0x06


#ifdef HIT_RADIO_DEBUG
#define DEBUG_HIT_OUTVAL(x)		M_SCI_OUTVAL(x)
#define DEBUG_HEX_HIT_OUTVAL(x)		M_SCI_OUT_HEX_VAL(x)
#define DEBUG_HIT_SENDSTRING(x)		SCI_SendString(x)
#else
#define DEBUG_HIT_OUTVAL(x)		
#define DEBUG_HEX_HIT_OUTVAL(x)	
#define DEBUG_HIT_SENDSTRING(x)	
#endif


/*********************************************************
				File End
*********************************************************/

