


/*********************************************************
	Project:		ST FST(TDA7540)
	File name:	radio.h
	Description:	
	Write/Modify:	Perry Liang
	Time:		2006-1-24- 15:35:43
*********************************************************/
extern void RadioInit(void);
extern void RadioOn(void);
extern void RadioOff(void);
extern void RadioMain(void);
extern void RadioSeek(void);
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
extern void SeekOff(void);
extern void RadioMute(unsigned char type);
extern void PresetOrder(void);
extern signed char ChangeTV2(unsigned char inputbuff);
extern unsigned char ChangeToTV2(signed char inputbuff);
extern unsigned char GetTV2(unsigned int inputfreq);
extern unsigned char GetSMeter (void);
extern unsigned char CheckIFC(unsigned int times,unsigned int all);
extern unsigned char Radio_PLL_FMISet(unsigned int FMfreq);
extern unsigned char Radio_PLL_AMISet(unsigned int AMfreq);
#ifndef NOT_SAVE_BAND_AF
extern void RDSSaveBandAF(void);
extern void RDSLoadBandAF(void);
#endif

#if (MCU_STM8)
#if (STM8_80PIN)
#define PD_AFS		PC_DDR0
#define PCR1_AFS	PC_CR10
#define PCR2_AFS	PC_CR20
#define Pin_AFS		PC_ODR0

#define PD_SSTOP	PH_DDR7
#define PCR1_SSTOP	PH_CR17
#define PCR2_SSTOP	PH_CR27
#define Pin_SSTOP	PH_ODR7
#define PinIn_SSTOP	PH_IDR7

#define PD_FSU		PE_DDR6
#define PCR1_FSU	PE_CR16
#define PCR2_FSU	PE_CR26
#define AD_FSU		AIN9
#elif (STM8_64PIN)
#define PD_AFS		PE_DDR5
#define PCR1_AFS	PE_CR15
#define PCR2_AFS	PE_CR25
#define Pin_AFS		PE_ODR5

#define PD_SSTOP	PD_DDR4
#define PCR1_SSTOP	PD_CR14
#define PCR2_SSTOP	PD_CR24
#define Pin_SSTOP	PD_ODR4
#define PinIn_SSTOP	PD_IDR4

#define PD_FSU		PE_DDR6
#define PCR1_FSU	PE_CR16
#define PCR2_FSU	PE_CR26
#define AD_FSU		AIN9

#endif
#else
#endif


#define FM_BAND_NUM 3
#define AM_BAND_NUM 3

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
	UCharField ReadBuff;
	unsigned int TestPointFreq;
	signed char TestPointVal;
	signed int Slope;
	signed int Slope2;

	unsigned int MaxFreq;
	unsigned int MinFreq;
	unsigned char Step;
	unsigned char SeekStep;
	unsigned char Area;
	signed char Direction;

	unsigned char Smeter;
	
	unsigned F_ForceSetBuff:1;
	unsigned F_GoodStation:1;
	unsigned F_TunerDispReq:1;
	unsigned F_ManualSeek:1;
	unsigned F_LocDx:1;
	unsigned F_Scan:1;
	unsigned F_TDA7541:1;//---1: tda7541; 0: tda7540

	FMBandStuct FMBand[FM_BAND_NUM];
	AMBandStuct AMBand[AM_BAND_NUM];
} RadioStuct;

ext RadioStuct Radio;
ext unsigned char RadioWaitTimer;
ext unsigned char RadioWorkTimer10ms;
ext unsigned char RadioWorkTimer;
ext unsigned char RadioSeekMuteTimer;

/*typedef struct Preset_stuct {
	unsigned int Freq;
	unsigned char Smeter;
} PresetStuct;

ext PresetStuct TempPreset[6];*/
ext unsigned char TempPresetNum;
ext unsigned char TempPresetSMMin;
ext unsigned char TempPresetSMMinNum;
#if TDA7540_TUNER
ext unsigned char SaveSmeterSlider;
#endif
#if TDA7541_TUNER
ext unsigned char SaveFSWOffset;
#endif

ext unsigned char SeekKeyTimer;

ext unsigned char FmIdx;
ext unsigned char AmIdx;

ext unsigned char DB10;


#define F_TunerISSON				Radio.ReadBuff.field.B0
#define F_TunerISS80kHz				Radio.ReadBuff.field.B1
#define F_TunerStereo				Radio.ReadBuff.field.B2
#define F_TunerPLLLock				Radio.ReadBuff.field.B3
#define F_TunerFieldStrengthLow		Radio.ReadBuff.field.B4
#define F_TunerAdjacentChannel		Radio.ReadBuff.field.B5
#define F_TunerDeviationStrong		Radio.ReadBuff.field.B6
#define F_TunerDeviation				Radio.ReadBuff.field.B7

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
#define RADIO_WORK_SEEKREQ		0x11
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

//#define RADIO_IN_SEARCH			(Radio.WorkMode==RADIO_WORK_ASWAIT ||Radio.WorkMode==RADIO_WORK_SEEKWAIT ||Radio.WorkMode==RADIO_WORK_SEEKNEXT ||Radio.WorkMode==RADIO_WORK_SEEKWAITRDS ||Radio.WorkMode==RADIO_WORK_SEEKWAITPI)
#define RADIO_IN_SEEK				((Radio.WorkMode>>4)==1)
#define RADIO_IN_AS					(Radio.WorkMode>0x25)
#define RADIO_IN_PS					((Radio.WorkMode>0x20)&&(Radio.WorkMode<0x24))
#define RADIO_IN_SEARCH			(RADIO_IN_SEEK||RADIO_IN_AS)


#define AREA_USA			0
#define AREA_LATIN			1
#define AREA_EUROPE		2
#define AREA_OIRT			3
#define AREA_JAPAN			4

#define LW_FREQ						300
#define AM_FREQ						2000
#define FM_FREQ						2000
#define FM_WB_START_FREQ			2300
#define FM_WB_STOP_FREQ			2600
#define FM_NORMAL_START_FREQ		8000
#define FM_OIRT_START_FREQ			6000
#define FM_OIRT_STOP_FREQ			7500
#define FM_JAPAN_START_FREQ		7500
#define FM_JAPAN_STOP_FREQ		9100
#define FreqInFM(x)					(x>AM_FREQ)
#define FreqInAM(x)					(x<FM_FREQ)
#define FreqInNormalFM(x)			(x>FM_NORMAL_START_FREQ)
#define FreqInOirtFM(x)				((x>FM_OIRT_START_FREQ && x<FM_OIRT_STOP_FREQ)&&Radio.Area==AREA_OIRT)
#define FreqInJapanFM(x)				(x>FM_JAPAN_START_FREQ && x<FM_JAPAN_STOP_FREQ&&Radio.Area==AREA_JAPAN)
#define FreqInWBFM(x)				(x>FM_WB_START_FREQ && x<FM_WB_STOP_FREQ)
#define FreqInMWAM(x)				(x>LW_FREQ && x<FM_FREQ)
#define FreqInLWAM(x)				(x<LW_FREQ)

#define TunerEepromAddress			0xA0
ext unsigned char EepromBuff[53];
//ext unsigned char EepromBuff[110];
//ext unsigned char OtherPointBuff[7];

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

//====================================================//
//================ TDA7540 register bit define======================//
//====================================================//
//For PLL current Tun_Buff[0]
//Bit3~0
#define PLL_HICURRENT_0MA       			0x00
#define PLL_HICURRENT_0_5mA     		0x01
#define PLL_HICURRENT_1MA       			0x02
#define PLL_HICURRENT_1_5mA     		0x03
#define PLL_HICURRENT_2MA       			0x04
#define PLL_HICURRENT_2_5mA     		0x05
#define PLL_HICURRENT_3MA       			0x06
#define PLL_HICURRENT_3_5mA     		0x07
#define PLL_HICURRENT_4MA       			0x08
#define PLL_HICURRENT_4_5mA     		0x09
#define PLL_HICURRENT_5MA       			0x0A
#define PLL_HICURRENT_5_5mA     		0x0B
#define PLL_HICURRENT_6MA       			0x0C
#define PLL_HICURRENT_6_5mA     		0x0D
#define PLL_HICURRENT_7MA       			0x0E
#define PLL_HICURRENT_7_5mA     		0x0F
//Bit 5~4
#define PLL_LOWCURRENT_0uA      			0x00
#define PLL_LOWCURRENT_50uA     		0x10
#define PLL_LOWCURRENT_100uA    		0x20
#define PLL_LOWCURRENT_150uA    		0x30
//Bit6
#define PLL_LOWCURRENT_MODE     		0x00
#define PLL_HIGHCURRENT_MODE    		0x40
//Bit7
#define PLL_FMSEEK_ON           			0x00
#define PLL_FMSEEK_OFF          			0x80

//Sample time and IFC   Tun_Buff[5]
#define PLL_TSAMPLE_FM20_48_AM128	0x00
#define PLL_TSAMPLE_FM10_24_AM64    	0x01
#define PLL_TSAMPLE_FM5_12_AM32      	0x02
#define PLL_TSAMPLE_FM2_56_AM16      	0x03
#define PLL_TSAMPLE_FM1_28_AM8        	0x04
#define PLL_TSAMPLE_FM0_64_AM4        	0x05
#define PLL_TSAMPLE_FM0_32_AM2        	0x06
#define PLL_TSAMPLE_FM0_16_AM1        	0x07

#define PLL_IFC_DISABLE                       	0x00
#define PLL_IFC_ENABLE                        	0x08

#define PLL_IFC_FMMODE                    		0x10
#define PLL_IFC_AM450K                         	0x20
#define PLL_IFC_AM10_7M                      	0x30

#define DISABLE_MUTEONHOLD               	0x00
#define ENABLE_MUTEONHOLD                	0x40

#define PLL_LOCK_DISABLE                    	0x00
#define PLL_LOCK_ENABLE                     		0x80

//center freqency and delta window   Tun_Buff[5]
#define PLL_IFC_CENTER_10_7MHZ     		0x0F
#define PLL_IFC_CENTER_450KHZ      		0x01

#define PLL_IFC_EW_FM6_25_AM1K     		0x60
#define PLL_IFC_EW_FM12_5_AM2K     		0x80
#define PLL_IFC_EW_FM25_AM4K       		0xA0
#define PLL_IFC_EW_FM50_AM8K       		0xC0
#define PLL_IFC_EW_FM100_AM16K     		0xE0


//For reference freqency
#define PLL_REFERECE_50KHz      			0x20     
#define PLL_REFERECE_25KHz      			0x28
#define PLL_REFERECE_10KHz      			0x30
#define PLL_REFERECE_9KHz       			0x38
#define PLL_REFERECE_2KHz       			0x00

//For SD threshold
#define SSTOP_IFC                      			0x00
#define SSTOP_IFC_0_89V          			0x10    // IFC=H & FSU>0.89V
#define SSTOP_IFC_1_16V           			0x20    // IFC=H & FSU>1.16V
#define SSTOP_IFC_1_43V           			0x30    // IFC=H & FSU>1.43V
#define SSTOP_IFC_1_7V             			0x40    // IFC=H & FSU>1.7V
#define SSTOP_IFC_1_97V           			0x50
#define SSTOP_IFC_2_24V           			0x60
#define SSTOP_IFC_2_51V           			0x70
#define SSTOP_IFC_2_78V           			0x80
#define SSTOP_IFC_3_05V           			0x90
#define SSTOP_IFC_3_32V           			0xA0
#define SSTOP_IFC_3_59V           			0xB0
#define SSTOP_IFC_3_86V           			0xC0
#define SSTOP_IFC_4_13V           			0xD0
#define SSTOP_IFC_4_40V           			0xE0
#define SSTOP_IFC_4_67V           			0xF0


//====================================================//
//================ TDA7541 register bit define==================//
//====================================================//
//For Tun_Buff[0]
//Bit2~0
#define TDA7541_PLL_HICURRENT_0MA       		0x00
#define TDA7541_PLL_HICURRENT_0_5mA     		0x01
#define TDA7541_PLL_HICURRENT_1MA       		0x02
#define TDA7541_PLL_HICURRENT_1_5mA     		0x03
#define TDA7541_PLL_HICURRENT_2MA       		0x04
#define TDA7541_PLL_HICURRENT_2_5mA     		0x05
#define TDA7541_PLL_HICURRENT_3MA       		0x06
#define TDA7541_PLL_HICURRENT_3_5mA     		0x07
//Bit3
#define TDA7541_PLL_LOCK_DISABLE				0x00
#define TDA7541_PLL_LOCK_ENABLE				0x08
//Bit4
#define TDA7541_STEREO_MUTE_DISABLE			0x00
#define TDA7541_STEREO_MUTE_ENABLE			0x10
//Bit5
#define TDA7541_LOCAL_DISABLE					0x00
#define TDA7541_LOCAL_ENABLE					0x20
//Bit6
#define TDA7541_ASFC_NORMAL					0x00//weak signal mute and AGC normal mode in FM
#define TDA7541_ASFC_HOLD						0x40//weak signal mute and AGC on hold in FM
//Bit7
#define TDA7541_SEEK_OFF						0x00
#define TDA7541_SEEK_ON						0x80

//Sample time and IFC   Tun_Buff[4]
//Bit0
#define TDA7541_PLL_TV_TRACK					0x00
#define TDA7541_PLL_TV_INDEPEDENT				0x02
//Bit1
#define TDA7541_PLL_TVOFFSET_DISABLE			0x00
#define TDA7541_PLL_TVOFFSET_ENABLE			0x02
//Bit4~3
#define TDA7541_PLL_TSAMPLE_FM20_48_AM128	0x00
#define TDA7541_PLL_TSAMPLE_FM10_24_AM64	0x08
#define TDA7541_PLL_TSAMPLE_FM5_12_AM32		0x10
#define TDA7541_PLL_TSAMPLE_FM2_56_AM16		0x18
//---expand mode for sampling time
#define TDA7541_PLL_TSAMPLE_FM1_28_AM8		0x00
#define TDA7541_PLL_TSAMPLE_FM0_64_AM4		0x08
#define TDA7541_PLL_TSAMPLE_FM0_32_AM2		0x10
#define TDA7541_PLL_TSAMPLE_FM0_16_AM1		0x18

//Bit6~5
#define TDA7541_PLL_IFC_EW_FM12_5_AM2K		0x00
#define TDA7541_PLL_IFC_EW_FM25_AM4K			0x20
#define TDA7541_PLL_IFC_EW_FM50_AM8K			0x40
#define TDA7541_PLL_IFC_EW_FM100_AM16K		0x60
//---expand mode for error window
#define TDA7541_PLL_IFC_EW_FM6_25_AM1K		0x60
//Bit7
#define TDA7541_AMMODE						0x00
#define TDA7541_FMMODE						0x80


//VCO & Pll reference freqency Tun_Buff[5]
//Bit1~0
#define TDA7541_VCO_DIVIDER_NOTVALID			0x00
#define TDA7541_VCO_DIVIDER_2					0x01
#define TDA7541_VCO_DIVIDER_3					0x02
#define TDA7541_VCO_DIVIDER_ORIGINAL			0x03
//Bit2
#define TDA7541_VCO_PHASE_0					0x00
#define TDA7541_VCO_PHASE_180					0x04
//Bit5~3
#define TDA7541_PLL_REFERECE_2KHz				0x18
#define TDA7541_PLL_REFERECE_9KHz				0x20
#define TDA7541_PLL_REFERECE_10KHz			0x28
#define TDA7541_PLL_REFERECE_25KHz			0x30
#define TDA7541_PLL_REFERECE_50KHz			0x38
//Bit7~6
#define TDA7541_AM_PREDIVIDER_10				0x00
#define TDA7541_AM_PREDIVIDER_8				0x40
#define TDA7541_AM_PREDIVIDER_6				0x80
#define TDA7541_AM_PREDIVIDER_4				0xc0

//For SSTOP threshold Tun_Buff[10]
#define TDA7541_SSTOP_IFC						0x00
#define TDA7541_SSTOP_IFC_FM0_5_AM1_1		0x10
#define TDA7541_SSTOP_IFC_FM0_7_AM1_4		0x20
#define TDA7541_SSTOP_IFC_FM0_9_AM1_7		0x30
#define TDA7541_SSTOP_IFC_FM1_1_AM2_0		0x40
#define TDA7541_SSTOP_IFC_FM1_3_AM2_3		0x50
#define TDA7541_SSTOP_IFC_FM1_5_AM2_6		0x60
#define TDA7541_SSTOP_IFC_FM1_7_AM2_9		0x70
#define TDA7541_SSTOP_IFC_FM1_9_AM3_2		0x80
#define TDA7541_SSTOP_IFC_FM2_1_AM3_5		0x90
#define TDA7541_SSTOP_IFC_FM2_3_AM3_8		0xA0
#define TDA7541_SSTOP_IFC_FM2_5_AM4_1		0xB0
#define TDA7541_SSTOP_IFC_FM2_7_AM4_4		0xC0
#define TDA7541_SSTOP_IFC_FM2_9_AM4_7		0xD0
#define TDA7541_SSTOP_IFC_FM3_1_AM5_0		0xE0
#define TDA7541_SSTOP_IFC_FM3_2_AM5_3		0xF0

//Tun_Buff[11]
#define TDA7541_WB_DISABLE					0x00
#define TDA7541_WB_ENABLE						0x02

//Tun_Buff[14]
//Bit5~4
#define TDA7541_IF2Q_NOR_AMQ28_FMQ28		0x00
#define TDA7541_IF2Q_R5k_FMQ3					0x10
#define TDA7541_IF2Q_R4_1k_FMQ2_5			0x20
#define TDA7541_IF2Q_R3_3k_FMQ2_05			0x30
//Bit7~6
#define TDA7541_IF2_GAIN_9						0x00
#define TDA7541_IF2_GAIN_12					0x40
#define TDA7541_IF2_GAIN_17					0x80
#define TDA7541_IF2_GAIN_21					0xC0

//Tun_Buff[23]
//Bit5
#define TDA7541_STD_VCO_OFF					0x00
#define TDA7541_STD_VCO_ON					0x20

//Tun_Buff[20]
//Bit6---MP FAST
#define TDA7541_MPTC_ENABLE					0x00
#define TDA7541_MPTC_DISABLE					0x40


/*********************************************************
				File End
*********************************************************/

