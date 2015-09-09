/*********************************************************
	Project:		ST HIT(TDA7705)
	File name:	radio.c
	Description:
	Write/Modify:	Yete HUANG
	Time:		April-2008
*********************************************************/
#include "PUBLIC.H"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "io.h"
#include "radio_hit_data.c"

#define debug 0
#define dprintf(msg...) if (debug) { printf( "radio_hit: " msg); }
  


extern void gpio_init();
extern void tda7703_RSTN_set(int high);



/*********************************************
	Function:		LoadArea
	Description:
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:38:56
*********************************************/
void LoadArea(void)
{
	unsigned char i,j;
	for(i=0;i<BAND_NUM;i++)
	{

		if(i<3)
		{
			for(j=0;j<6;j++)
				Radio.FMBand[i].MemFreq[j]=Area[Radio.Area].Band[i].MemFreq[j];
			Radio.FMBand[i].BandFreq=Area[Radio.Area].Band[i].BandFreq;
			Radio.FMBand[i].CurrentMem=Area[Radio.Area].Band[i].CurrentMem;
		}
		else
		{
			for(j=0;j<6;j++)
				Radio.AMBand[i-3].MemFreq[j]=Area[Radio.Area].Band[i].MemFreq[j];
			Radio.AMBand[i-3].BandFreq=Area[Radio.Area].Band[i].BandFreq;
			Radio.AMBand[i-3].CurrentMem=Area[Radio.Area].Band[i].CurrentMem;
		}
	}

	Radio.MaxFreq=Area[Radio.Area].FMMaxFreq;
	Radio.MinFreq=Area[Radio.Area].FMMinFreq;
	Radio.Step=Area[Radio.Area].FMStep;
	Radio.SeekStep=Area[Radio.Area].FMSeekStep;

	Radio.CurrentBand=0;
}

void Wait1ms(int num)
{
	usleep(num * 1000);
}
void Wait10us(int num)
{
	usleep(num * 10);
}

 /*********************************************
	Function:		RadioInit
	Description:
	Write/Modify:	Yete HUANG
	Time:		2007-10-24
*********************************************/
void RadioInit(void)
{

/*
pin configure:
Reset: RSTN(mcu out(master),RDSint(mcu out), MISO(SPI)(mcu out)
RDS: AFS(mcu out),RDSINT(mcu in)
I2C/SPI: SCL(I2C), SDA(I2C)/MOSI(SPI),MISO(SPI),CS(SPI),
*/
	
	Radio.Area=AREA_USA;

	LoadArea();
	Radio.Freq=Radio.FMBand[0].BandFreq;

	Radio.CurrentBand=0;
	FmIdx = 0;
	AmIdx = 0;

	io2w_init();
	gpio_init();
	//RDSSetTuner(TUNER_HIT);
}

//void RadioWork(void)
//{
//	unsigned char i,j;
//
//	switch(Radio.WorkMode)
//	{
//		case RADIO_WORK_IDLE:
//			if(System.DispMode.Current!=DISPMODE_RADIO)
//				break;
//
//			if(F_HalfSecondOut)
//			{
//				if(Radio.F_QualDisplay)
//				{
//					HIT_GetCFQuality(3);
//					//read out some status for debug
//					HIT_CmdReadMem(0x01a2e4, 1); //read out ifbw
//					Radio.IFBW = Radio.HITDataBuff[5];
//					Wait10us(5);
//					HIT_CmdReadTDSR();
//
//				}
//			}
//			break;
//
//		case RADIO_WORK_SEEKREQ:
//			Radio.F_TunerDispReq=1;
//			Radio.WorkMode=RADIO_WORK_SEEKWAIT;
//			Radio.BackupFreq=Radio.Freq;
//			RadioMute(SOFTON);
//			if(Radio.Direction>0)
//			{
//				if(Radio.Freq%10==5 && FreqInFM(Radio.Freq)&&!FreqInWBFM(Radio.Freq))
//				{
//					Radio.Freq-=5;//avoid the frequency(87.55,89.55...) at first time
//					RadioSetFreq(Radio.Freq);
//
//				}
//			}
//			else
//			{
//				if(Radio.Freq%10==5 && FreqInFM(Radio.Freq)&&!FreqInWBFM(Radio.Freq))
//				{
//					Radio.Freq+=5;
//					RadioSetFreq(Radio.Freq);
//				}
//			}
//			Radio.BackupFreq=Radio.Freq;
//			SeekOn();
//			NavigationShow(NAV_TUNER_SEEK_BEGIN);
//			RadioWorkTimer10ms=10;
//			//break;//!!!!!!!!
//
//		case RADIO_WORK_SEEKWAIT:
//			if(RadioWorkTimer10ms>0)break;
//			HIT_CmdReadSeekStatus();
//			if(Radio.F_SeekFullCycle)
//			{
//				if(F_PTYEnable)
//				{
//					F_NoPTY=1;
//					PTYSelectTimer=50;
//					System.DispMode.Requst=DISPMODE_RDS;
//					RDSDispMode=RDSDISP_PTYSELECT;
//				}
//				Radio.WorkMode=RADIO_WORK_SEEKOK;
//				Radio.F_Scan=0;
//			}
//			else if(Radio.F_GoodStation)
//			{
//				if((FreqInFM(Radio.Freq)&&!FreqInJapanFM(Radio.Freq)&&!FreqInWBFM(Radio.Freq))&&(F_AFEnable||F_TAEnable||F_PTYEnable||F_PISeek))
//				{
//					if(FreqInNormalFM(Radio.Freq)&&Radio.Freq%10==0)
//					{
//						Radio.WorkMode=RADIO_WORK_SEEKWAITRDS;
//						RadioWorkTimer10ms=150;
//						RDSDataInit();//Need further confirm
//					}
//					else
//						Radio.WorkMode=RADIO_WORK_SEEKNEXT;
//				}
//				else
//				{
//					Radio.WorkMode=RADIO_WORK_SEEKOK;
//				}
//				break;
//			}
//
//			RadioWorkTimer10ms=1;
//			break;
//
//		case RADIO_WORK_SEEKNEXT:
//			SeekOff(SeekContinue);
//			Radio.WorkMode=RADIO_WORK_SEEKWAIT;
//			RadioWorkTimer10ms=10;
//			break;
//
//		case RADIO_WORK_SEEKWAITRDS:
//			if(RDSGetDataBlock())
//			{
//				Radio.WorkMode=RADIO_WORK_SEEKWAITPI;
//				RadioWorkTimer10ms=100;
//				//Radio.WorkMode=RADIO_WORK_SEEKOKWAIT;
//				RDSResetPISeekTimer();
//				break;
//			}
//			if(RadioWorkTimer10ms==0)
//			{
//				Radio.WorkMode=RADIO_WORK_SEEKNEXT;
//			}
//			break;
//		case RADIO_WORK_SEEKWAITPI:
//			if(RDSFlagPINew())
//			{
//
//				if(F_TAEnable)
//				{
//					Radio.WorkMode=RADIO_WORK_SEEKWAITTP;
//					RadioWorkTimer10ms=9;
//				}
//				else if(F_PTYEnable)
//				{
//					Radio.WorkMode=RADIO_WORK_SEEKWAITPTY;
//					PTYSelectTimer=100;
//					RadioWorkTimer10ms=9;
//				}
//				else if(F_PISeek)
//				{
//					if(RDSFlagPIJudge())
//					{
//						Radio.WorkMode=RADIO_WORK_SEEKOK;
//						F_PISeek=0;
//						RDSDataInit();
//					}
//					else
//					{
//						Radio.WorkMode=RADIO_WORK_SEEKNEXT;
//					}
//				}
//				else
//					Radio.WorkMode=RADIO_WORK_SEEKOK;
//
//				break;
//			}
//			if(RadioWorkTimer10ms==0)
//			{
//				Radio.WorkMode=RADIO_WORK_SEEKNEXT;
//			}
//			break;
//		case RADIO_WORK_SEEKWAITTP:
//			if(RDSFlagTP()||RDSFlagTA())
//			{
//				Radio.WorkMode=RADIO_WORK_SEEKOK;
//				#ifdef DEBUG_MODE
//				DebugShow("Radio.Freq=",Radio.Freq," Wait TP OK!");
//				#endif
//				break;
//			}
//			if(RadioWorkTimer10ms==0)
//			{
//				Radio.WorkMode=RADIO_WORK_SEEKNEXT;
//			}
//			break;
//		case RADIO_WORK_SEEKWAITPTY:
//			if(RDSFlagPTYNew()&&(RDSGetPTY()==PTYSelectType))
//			{
//				Radio.WorkMode=RADIO_WORK_SEEKOK;
//				PTYSelectTimer=1;
//				break;
//			}
//			if(RadioWorkTimer10ms==0)
//			{
//				Radio.WorkMode=RADIO_WORK_SEEKNEXT;
//			}
//			break;
//
//		case RADIO_WORK_SEEKOK:
//			F_PISeek=0;
//			Radio.WorkMode=RADIO_WORK_IDLE;
//			SeekOff(SeekStopUnmuted);
//			//RadioSetFreq(Radio.Freq);
//			RadioMute(SOFTOFF);
//			Radio.F_TunerDispReq=1;
//			NavigationShow(NAV_TUNER_CURRENT_FREQ);
//			NavigationShow(NAV_TUNER_SEEK_END);
//			if(Radio.F_Scan)
//			{
//				Radio.WorkMode=RADIO_WORK_SCANWAIT;
//				RadioWorkTimer=50;
//			}
//			break;
//
//		case RADIO_WORK_SCANWAIT:
//			if(RadioWorkTimer>0)break;
//			Radio.WorkMode=RADIO_WORK_SEEKNEXT;
//			break;
//
//		case RADIO_WORK_SEEKBREAK:
//			F_PISeek=0;
//			Radio.F_GoodStation=0;
//			Radio.WorkMode=RADIO_WORK_IDLE;
//			SeekOff(SeekStopUnmuted);
//			RadioSetFreq(Radio.Freq);
//			RadioMute(SOFTOFF);
//			Radio.F_TunerDispReq=1;
//			NavigationShow(NAV_TUNER_CURRENT_FREQ);
//			NavigationShow(NAV_TUNER_SEEK_END);
//			break;
//
//		case RADIO_WORK_PSREQ://display the each preset memory frequency during 5 seconds
//			Radio.WorkMode=RADIO_WORK_PSWAIT;
//			if(FreqInFM(Radio.Freq))
//			{
//				Radio.FMBand[FmIdx].CurrentMem=0;
//			}
//			else
//			{
//				Radio.AMBand[AmIdx].CurrentMem=0;
//			}
//			NavigationShow(NAV_TUNER_PS_BEGIN);
//			RadioWorkTimer=0;
//			break;
//		case RADIO_WORK_PSWAIT:
//			if(RadioWorkTimer>0)break;
//			RadioWorkTimer=50;
//			if(FreqInFM(Radio.Freq))
//			{
//				if(Radio.FMBand[FmIdx].CurrentMem==6)
//				{
//					Radio.FMBand[FmIdx].CurrentMem=1;
//					Radio.WorkMode=RADIO_WORK_PSEND;
//				}
//				else
//					Radio.FMBand[FmIdx].CurrentMem++;
//			}
//			else
//			{
//				if(Radio.AMBand[AmIdx].CurrentMem==6)
//				{
//					Radio.AMBand[AmIdx].CurrentMem=1;
//					Radio.WorkMode=RADIO_WORK_PSEND;
//				}
//				else
//					Radio.AMBand[AmIdx].CurrentMem++;
//			}
//			BreakAFCheck();
//			RadioMute(SOFTON);
//			if(FreqInFM(Radio.Freq))
//			{
//				RadioSetFreq(Radio.FMBand[FmIdx].MemFreq[Radio.FMBand[FmIdx].CurrentMem-1]);
//			}
//			else
//			{
//				RadioSetFreq(Radio.AMBand[AmIdx].MemFreq[Radio.AMBand[AmIdx].CurrentMem-1]);
//			}
//			RadioMute(SOFTOFF);
//			NavigationShow(NAV_TUNER_CURRENT_FREQ);
//			System.DispMode.Requst=DISPMODE_RADIO;
//			RDSDispMode=RDSDISP_IDLE;
//			break;
//		case RADIO_WORK_PSEND:
//			RadioWorkTimer=0;
//			NavigationShow(NAV_TUNER_PS_END);
//			Radio.WorkMode=RADIO_WORK_IDLE;
//			break;
//
//		case RADIO_WORK_ASPSBREAK:
//			NavigationShow(NAV_TUNER_PSAS_BREAK);
//			NavigationShow(NAV_TUNER_CURRENT_FREQ);
//			Radio.WorkMode=RADIO_WORK_IDLE;
//			SeekOff(SeekStopUnmuted);
//			RadioSetFreq(Radio.Freq);
//			RadioMute(SOFTOFF);
//			RadioWorkTimer=0;
//			Radio.F_TunerDispReq=1;
//			break;
//
//		case RADIO_WORK_ASREQ:
//			if(FreqInFM(Radio.Freq)&&Radio.Area==AREA_OIRT)
//				Radio.BackupFreq=Area[AREA_OIRT].FMMinFreqOirt;
//			else
//				Radio.BackupFreq=Radio.MinFreq;
//			Radio.ReqFreq=Radio.BackupFreq;
//			RadioSetFreq(Radio.ReqFreq);
//			Radio.Direction=1;
//			if(FreqInFM(Radio.Freq))
//			{
//				for(i=0;i<6;i++)
//				{
//					Radio.FMBand[FmIdx].MemFreq[i]=Radio.BackupFreq;
//					Radio.FMBand[FmIdx].MemSmeter[i]=0;
//					Radio.FMBand[FmIdx].PI[i]=0;
//					Radio.FMBand[FmIdx].MemAFNum[i]=1;
//					for(j=0;j<8;j++)
//						Radio.FMBand[FmIdx].MemAFList[i][j]=0xFF;
//				}
//			}
//			else
//			{
//				for(i=0;i<6;i++)
//				{
//					Radio.AMBand[AmIdx].MemFreq[i]=Radio.BackupFreq;
//					Radio.AMBand[AmIdx].MemSmeter[i]=0;
//					//Radio.AMBand[FmIdx].PI[i]=0;
//					//Radio.AMBand[FmIdx].MemAFNum[i]=1;
//					//for(j=0;j<8;j++)
//						//Radio.FMBand[FmIdx].MemAFList[i][j]=0xFF;
//				}
//			}
//			SeekOn();
//			RadioMute(SOFTON);
//			NavigationShow(NAV_TUNER_AS_BEGIN);
//			Radio.WorkMode=RADIO_WORK_ASWAIT;
//			TempPresetNum=0;
//			Radio.F_TunerDispReq=1;
//			RadioWorkTimer10ms=10;
//			break;
//
//		case RADIO_WORK_ASWAIT:
//			if(RadioWorkTimer10ms>0) break;
//			HIT_CmdReadSeekStatus();
//			if (Radio.F_SeekFullCycle)
//			{
//				Radio.WorkMode=RADIO_WORK_ASEND;
//
//			}
//			else if(Radio.F_GoodStation)
//			{
//				if((FreqInFM(Radio.Freq)&&!FreqInJapanFM(Radio.Freq)&&!FreqInWBFM(Radio.Freq))&&(F_AFEnable||F_TAEnable))
//				{
//					Radio.WorkMode=RADIO_WORK_ASWAITRDS;
//					RadioWorkTimer10ms=150;
//					RDSDataInit();
//					break;
//				}
//
//				else
//				{
//					PresetOrder();
//					Radio.WorkMode=RADIO_WORK_ASNEXT;
//
//				}
//			}
//
//			RadioWorkTimer10ms=1;
//			break;
//
//		case RADIO_WORK_ASNEXT:
//			SeekOff(SeekContinue);
//			RadioWorkTimer10ms=10;
//			Radio.WorkMode=RADIO_WORK_ASWAIT;
//			break;
//
//		case RADIO_WORK_ASWAITRDS:
//			if(RDSGetDataBlock())
//			{
//				Radio.WorkMode=RADIO_WORK_ASWAITPI;
//				RadioWorkTimer10ms=100;
//				break;
//			}
//			if(RadioWorkTimer10ms==0)
//				Radio.WorkMode=RADIO_WORK_ASNEXT;
//			break;
//
//		case RADIO_WORK_ASWAITPI:
//			if(RDSFlagPINew())
//			{
//				if(FreqInFM(Radio.Freq))
//				{
//					for(i=0,j=0;i<TempPresetNum;i++)
//					{
//						if(RDSGetPI()==Radio.FMBand[FmIdx].PI[i])
//						{
//							j=1;
//							break;
//						}
//					}
//				}
//				if(j)
//				{
//					Radio.WorkMode=RADIO_WORK_ASNEXT;
//				}
//				else
//				{
//					Radio.WorkMode=RADIO_WORK_ASWAITAFLIST;
//					RadioWorkTimer10ms=250;
//				}
//				break;
//			}
//			if(RadioWorkTimer10ms==0)
//				Radio.WorkMode=RADIO_WORK_ASNEXT;
//			break;
//
//		case RADIO_WORK_ASWAITAFLIST:
//			if(RDSFlagAFListNew() || RadioWorkTimer10ms==0)
//			{
//				PresetOrder();
//				Radio.WorkMode=RADIO_WORK_ASNEXT;
//				break;
//			}
//			break;
//
//		case RADIO_WORK_ASEND:
//			//for(i=0;i<TempPresetNum;i++)
//			//	Radio.Band[Radio.CurrentBand].MemFreq[i]=TempPreset[i].Freq;
//			SeekOff(SeekStopUnmuted);
//			RadioSetFreq(Radio.Freq);
//			RadioMute(SOFTOFF);
//			NavigationShow(NAV_TUNER_AS_END);
//			Radio.WorkMode=RADIO_WORK_PSREQ;
//			break;
//
//		case RADIO_WORK_STARTUP:
//			if(hit_startup_fsm == HIT_STARTUP_IDLE)
//			{
//				hit_startup_fsm = HIT_STARTUP_REQ;
//			}
//			HIT_Startup_Seq();
//			break;
//	}
//
//}



void HIT_Change_Band( unsigned char  bandreq )
{
	dprintf("#########################HIT_Change_Band: %d################\n", bandreq);
	//Change to FM band from other band
	unsigned char i;
	if(bandreq == CHANGE_FM)
	{
			HIT_DSPMode(DSP_FM);
			HIT_CmdWriteBeCoeff( FMSWPatchData);
			HIT_CmdSetBand(FM);
			//HIT_CmdSetFEReg(FM,0,0);//num=0, send current internal values to FE registers (refresh)
			HIT_CmdSetFEReg(FM,0,4);/*use ctm define FE setting*/
			HIT_CmdSetFEReg(FM,4,4);
			HIT_CmdSetFEReg(FM,15,1);
			Wait1ms(50);
			HIT_CmdSetIRType(HybridIR_FM);
			Radio.F_TDBusy=1;
			do
			{
				HIT_CmdReadTDSR();
			}while(Radio.F_TDBusy);
			//SetAFParameter----if no RDS operation or use defaut value, this step also can be skipped
			HIT_CmdDynIS(0x1);//set dynamic image selection enable
			/*SetSeekParameter---Set by cmd,customer can modify the default with this cmd*/
			/*default parameter in SW:  Nb of cycle: 000708; TH on fsu: c00000; TH on ac: 100000; TH on detune:100000;TH on mp:7FFFFF; TH on global quality:000000*/
			//HIT_CmdSeekTH(0x500, 0xC30000, 0x300000, 0x060000, 0x100000, 0x000000);
			HIT_CmdSeekTH(0x500, 0xC80000, 0x500000, 0x060000, 0x150000, 0x000000);

			//if(F_AFEnable)
			//{
			//	Enable_RDS_Int();
			//}
			//HIT_CmdSetFreq(Radio.FMBand[FmIdx].BandFreq);
#ifdef HIT_44
			HIT_CmdSetDiss(Diss_FM_Manual, Diss_FM_Manual_BW1);
#else
			HIT_CmdSetDiss(Diss_FM_Manual, Diss_FM_Manual_BW4);
#endif
			Wait1ms(100);
			HIT_CmdSetDiss(Diss_FM_Auto, 0);
			HIT_CmdWriteBeCoeff( FMCustomWSPData);
			//RDSDataInit();
	//if(RDSDispMode!=RDSDISP_PTYSELECT&&RDSDispMode!=RDSDISP_TRAFFIC)
	//	RDSDispMode=RDSDISP_IDLE;
//
	}
	else if(bandreq == CHANGE_WX)
	{
			HIT_DSPMode(DSP_WX);
			HIT_CmdWriteBeCoeff( WXSWPatchData);
			HIT_CmdSetBand(WB);
			//HIT_CmdSetFEReg(FM,0,0);//num=0, send current internal values to FE registers (refresh)
			HIT_CmdSetFEReg(WB,0,4);/*use ctm define FE setting*/
			HIT_CmdSetFEReg(WB,4,4);
			HIT_CmdSetFEReg(WB,15,1);
			Wait1ms(50);
			HIT_CmdSetIRType(HybridIR_FM);
			Radio.F_TDBusy=1;
			do
			{
				HIT_CmdReadTDSR();
			}while(Radio.F_TDBusy);
			/*SetSeekParameter---Set by cmd,customer can modify the default with this cmd*/
			HIT_CmdDynIS(0x1);//set dynamic image selection enable
			/*default parameter in SW:  Nb of cycle: 000708; TH on fsu: c00000; TH on ac: 100000; TH on detune:100000;TH on mp:7FFFFF; TH on global quality:000000*/
			HIT_CmdSeekTH(0x500, 0xC30000, 0x300000, 0x060000, 0x100000, 0x000000);
			//HIT_CmdSetFreq(ndFreq);
			HIT_CmdSetDiss(Diss_WX_Neutral, 0);
			HIT_CmdWriteBeCoeff( WXCustomWSPData);
			//if(RDSDispMode!=RDSDISP_PTYSELECT&&RDSDispMode!=RDSDISP_TRAFFIC)
			//	RDSDispMode=RDSDISP_IDLE;
	}
	//Change to AM band from other band
	else if(bandreq == CHANGE_AM || bandreq == CHANGE_SW )
	{
			if(Radio.Area==AREA_EUROPE||Radio.Area==AREA_JAPAN)
			{
				HIT_DSPMode(DSP_AM_EuJp);
			}
			else
			{
				HIT_DSPMode(DSP_AM_Us);
			}
			//Disable_RDS_Int();
			HIT_CmdWriteBeCoeff( AMSWPatchData);
			if(bandreq == CHANGE_SW)
				HIT_CmdSetBand(SW);
			else
				HIT_CmdSetBand(AM);
			/*HIT_CmdSetFEReg(AM,0,0);num=0, send current internal values to FE registers (refresh)*/
			HIT_CmdSetFEReg(AM,0,4);/*use ctm define FE setting*/
			HIT_CmdSetFEReg(AM,4,4);
			HIT_CmdSetFEReg(AM,15,1);
			usleep(50*1000);
			/*Set AM IR*/
			HIT_CmdSetIRType(HybridIR_AM);
			do
			{
				HIT_CmdReadTDSR();
			}while(Radio.F_TDBusy);
			HIT_CmdDynIS(0x1);//set dynamic image selection enable
			// SetSeekParameter---Set by cmd,customer can modify the default with this cmd
			// AM_Smeter(0xa10000), when add 18dB dummy before AM ant, seek sensitivity set 32dB
			//HIT_CmdSeekTH(0x000700, 0xa10000, 0x7fffff, 0x100000, 0x7fffff, 0x000000);
			//set Smeter threshold as C1, this setting is only for ForYou!, stop thd is 30 db with 18db dummy
			HIT_CmdSeekTH(0x000700, 0xB60000, 0x7fffff, 0x100000, 0x7fffff, 0x000000);
			//HIT_CmdSeekTH(0x000700, AMSeekSmeterTH, 0x7fffff, 0x100000, 0x7fffff, 0x000000);
		//HIT_CmdSetFreq(Radio.AMBand[AmIdx].BandFreq);
		HIT_CmdSetDiss(Diss_AM_Neutral, 0);
		HIT_CmdWriteBeCoeff( AMCustomWSPData);
	}
	//if(!RADIO_IN_PS)
	//{
	//	if(FreqInFM(Radio.Freq))
	//	{
	//		for(i=0;i<6;i++)
	//		{
	//			if(Radio.Freq==Radio.FMBand[FmIdx].MemFreq[i])
	//			{
	//				Radio.FMBand[FmIdx].CurrentMem=i+1;
	//				break;
	//			}
	//			if(i==5)Radio.FMBand[FmIdx].CurrentMem=0;
	//		}
	//	}
	//	else
	//	{
	//		for(i=0;i<6;i++)
	//		{
	//			if(Radio.Freq==Radio.AMBand[AmIdx].MemFreq[i])
	//			{
	//				Radio.AMBand[AmIdx].CurrentMem=i+1;
	//				break;
	//			}
	//			if(i==5)Radio.AMBand[AmIdx].CurrentMem=0;
	//		}
	//	}
	//}
	//if(RDSDispMode!=RDSDISP_PTYSELECT&&RDSDispMode!=RDSDISP_TRAFFIC)
	//	RDSDispMode=RDSDISP_IDLE;
}

extern void tda7703_RSTN_set(int high);
void HIT_Startup_Seq()
{
	dprintf("################# HIT_Startup_Seq in.....\n");

	tda7703_RSTN_set(0);

	Wait10us(100);

	tda7703_RSTN_set(1);

	Wait10us(100);
          /*boot code---This step can be skipped with HIT Rom version*/

#ifdef HIT_EEPROM
          HIT_EEpromBootCodeDownload();
#else
          HIT_BootcodeDownload();
#endif
          /*set DSP to FM and then perform the alignment*/
          HIT_DSPMode(DSP_FM);

#if HIT_FM_NARROWBAND
          if (Radio.Area==AREA_EUROPE||Radio.Area==AREA_USA||Radio.Area==AREA_LATIN)
          {
               HIT_CmdStartAlign(AlignVCO_EUUSA);
          }
          else if(Radio.Area==AREA_JAPAN)
          {
               HIT_CmdStartAlign(AlignVCO_Japan);
          }
          else
          {
               HIT_CmdStartAlign(AlignVCO);
          }
#else
          HIT_CmdStartAlign(AlignVCO);
#endif

          //hit_startup_fsm=HIT_STARTUP_WAIT_VCO_FE_ALIGN;
          //RadioWorkTimer10ms=25;
		  usleep(25*1000);
       //   break;

     //case HIT_STARTUP_WAIT_VCO_FE_ALIGN:
         // if(RadioWorkTimer10ms!=0)break;
		 
          //HIT_CmdReadTDSR();
		  do{
			  HIT_CmdReadTDSR();
			  usleep(1000);
		  }while(Radio.F_TDBusy);
          //if(!Radio.F_TDBusy)
          //{
               //set FE FM parameters
               //HIT_CmdSetFEReg(FM,0,0);//num=0, send current internal values to FE registers (refresh)
               HIT_CmdSetFEReg(FM,0,4);/*use ctm define FE setting*/
               HIT_CmdSetFEReg(FM,4,4);
               HIT_CmdSetFEReg(FM,15,1);
               //hit_startup_fsm = HIT_STARTUP_WAIT_IRALIGN;
               //RadioWorkTimer10ms=5;
			   usleep(5*1000);
          //}
          //else
          //{
          //     RadioWorkTimer10ms=1;
          //}
          //break;
     //case HIT_STARTUP_WAIT_IRALIGN:
       //   if(RadioWorkTimer10ms!=0)break;
          HIT_CmdSetIRType(IRalignment);
          Radio.F_TDBusy=1;
          do
          {
               HIT_CmdReadTDSR();
          }while(Radio.F_TDBusy);
          //hit_startup_fsm = HIT_STARTUP_IRALIGNDONE;
          //break;

     //case HIT_STARTUP_IRALIGNDONE:
          //Radio.ReqFreq=Radio.Band[Radio.CurrentBand].BandFreq;
		  HIT_DSPMode(DSP_IDLE);

		  dprintf("222#################33 HIT_Startup_Seq out.....\n");
		  //if(!fm)
		  //{
		  //    HIT_Change_Band(CHANGE_FM);

		  //}
		  //else
		  //{
		  //    HIT_Change_Band(CHANGE_AM);
		  //}
		  //Radio.F_ForceSetBuff=1;
          //RadioSetFreq(Radio.ReqFreq);
          //RadioMute(SOFTOFF);
#ifndef NOT_SAVE_BAND_AF
         // RDSLoadBandAF();
#endif
          //Radio.F_TunerDispReq=1;

          //NavigationModeDisplay(WORKMODE_RADIO);
          //hit_startup_fsm = HIT_STARTUP_IDLE;
          //Radio.WorkMode=RADIO_WORK_IDLE;
        //  break;
	  //case HIT_STARTUP_IDLE:
	//	break;
     //}
}

void RadioSetFreq(unsigned int setfreq)
{
	Radio.Freq = setfreq;
	if(FreqInFM(setfreq) && !FreqInWBFM(setfreq))
	{
		Radio.F_TDBusy=1;
		do
		{
			HIT_CmdReadTDSR();
		}while(Radio.F_TDBusy);
		HIT_CmdSetFreq(setfreq);
#ifdef HIT_44
		HIT_CmdSetDiss(Diss_FM_Manual, Diss_FM_Manual_BW1);
#else
		HIT_CmdSetDiss(Diss_FM_Manual, Diss_FM_Manual_BW4);
#endif
		usleep(100*1000);
		HIT_CmdSetDiss(Diss_FM_Auto, 0);
	}
	else if(FreqInWBFM(setfreq))
	{
		HIT_CmdSetFreq(setfreq);
		HIT_CmdSetDiss(Diss_WX_Neutral, 0);
	}
	else if(FreqInAM(setfreq))//AM
	{
		HIT_CmdSetFreq(setfreq);
		usleep(100*1000);
		HIT_CmdSetDiss(Diss_AM_Neutral, 0);
	}
}

/*********************************************
	Function:		RadioFreqCal
	Description:
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:38:22
*********************************************/

/*********************************************
	Function:		RadioSave
	Description:
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:38:20
*********************************************/
//void RadioLoad(unsigned char pos)
//{
//	unsigned char i;
//	if((Radio.WorkMode&0xF0)==RADIO_WORK_ASPSMODE){
//		//Radio.WorkMode=RADIO_WORK_ASPSBREAK;
//			NavigationShow(NAV_TUNER_PSAS_BREAK);
//			NavigationShow(NAV_TUNER_CURRENT_FREQ);
//			Radio.WorkMode=RADIO_WORK_IDLE;
//			SeekOff(SeekStopUnmuted);
//			RadioSetFreq(Radio.Freq);
//			RadioMute(SOFTOFF);
//			RadioWorkTimer=0;
//			Radio.F_TunerDispReq=1;
//	}
//
//	if(RADIO_IN_SEEK){
//		//Radio.WorkMode=RADIO_WORK_SEEKBREAK;
//			F_PISeek=0;
//			Radio.F_GoodStation=0;
//			Radio.WorkMode=RADIO_WORK_IDLE;
//			SeekOff(SeekStopUnmuted);
//			Wait1ms(5);
//			//RadioSetFreq(Radio.Freq);
//			RadioMute(SOFTOFF);
//			//Radio.F_TunerDispReq=1;
//			NavigationShow(NAV_TUNER_CURRENT_FREQ);
//			NavigationShow(NAV_TUNER_SEEK_END);
//	}
//
//
//	RadioMute(SOFTON);
//	if(FreqInFM(Radio.Freq))
//	{
//		RadioSetFreq(Radio.FMBand[FmIdx].MemFreq[pos]);
//	}
//	else
//	{
//		RadioSetFreq(Radio.AMBand[AmIdx].MemFreq[pos]);
//	}
//	RadioMute(SOFTOFF);
//	if(FreqInFM(Radio.Freq))
//	{
//		Radio.FMBand[FmIdx].CurrentMem=pos+1;
//	}
//	else
//	{
//		Radio.AMBand[AmIdx].CurrentMem=pos+1;
//	}
//	if(F_AFEnable||F_TAEnable)
//	{
//		if(FreqInFM(Radio.Freq))
//		{
//			RDSSetPI(Radio.FMBand[FmIdx].PI[pos]);
//			RDSSetPIBackup(Radio.FMBand[FmIdx].PI[pos]);
//			RDSSetAFNum(Radio.FMBand[FmIdx].MemAFNum[pos]);
//			for(i=0;i<8;i++)
//				RDSSetAF(i,Radio.FMBand[FmIdx].MemAFList[pos][i]);
//			if(RDSGetAFNum()>1)
//			{
//				SetAFSearch();
//			}
//			RDSResetPISeekTimer();
//		}
//	}
//	Radio.F_TunerDispReq=1;
//
//
//	NavigationShow(NAV_TUNER_CURRENT_FREQ);
//}
//
//
//#ifndef NOT_SAVE_BAND_AF
//void RDSSaveBandAF(void)
//{
//	unsigned char i;
//
//	if(FreqInFM(Radio.Freq))
//	{
//		if(RDSGetAFNum()>1)
//		{
//			BreakAFSearch();
//			Radio.FMBand[FmIdx].BandPI=RDSGetPIBackup();
//			if(RDSGetAFNum()>8)
//				Radio.FMBand[FmIdx].BandAFNum=8;
//			else
//				Radio.FMBand[FmIdx].BandAFNum=RDSGetAFNum();
//			for(i=0;i<8;i++)
//				Radio.FMBand[FmIdx].BandAFList[i]=RDSGetAF(i);
//		}
//		else
//		{
//			Radio.FMBand[FmIdx].BandAFNum=1;
//			Radio.FMBand[FmIdx].BandAFList[0]=ChangFreq16BitTo8Bit(Radio.FMBand[FmIdx].BandFreq);
//			Radio.FMBand[FmIdx].BandPI=0;
//		}
//	}
//}
//
//void RDSLoadBandAF(void)
//{
//	unsigned char i;
//
//	if(F_AFEnable||F_TAEnable)
//	{
//		if(FreqInFM(Radio.Freq))
//		{
//			RDSSetPI(Radio.FMBand[FmIdx].BandPI);
//			RDSSetPIBackup(Radio.FMBand[FmIdx].BandPI);
//			RDSSetAFNum(Radio.FMBand[FmIdx].BandAFNum);
//			for(i=0;i<8;i++)
//				RDSSetAF(i,Radio.FMBand[FmIdx].BandAFList[i]);
//			if(RDSGetAFNum()>1)
//			{
//				SetAFSearch();
//			}
//			RDSResetPISeekTimer();
//		}
//	}
//	Radio.F_TunerDispReq=1;
//}
//
//#endif

/*********************************************
	Function:		SeekOn
	Description:	Activate auto seek
	Write/Modify:	Yete HUANG
	Time:		2008-2
*********************************************/
void SeekOn(void)
{
	if(FreqInFM(Radio.Freq))//FM
	{
		if (FreqInWBFM(Radio.Freq))
		{
#ifdef AUTOSEEK
						HIT_CmdStartAutoSeek((signed long)Radio.SeekStep*Radio.Direction, WaitCmdEnd);
#else
						HIT_CmdStartManualSeek((signed long)Radio.SeekStep*Radio.Direction);
#endif
		}
		else
		{
#ifdef AUTOSEEK
						HIT_CmdStartAutoSeek((signed long)(Radio.SeekStep*10)*Radio.Direction, WaitCmdEnd);
#else
						HIT_CmdStartManualSeek((signed long)(Radio.SeekStep*10)*Radio.Direction);
#endif
		}

	}
		//HIT_CmdStartAutoSeek((signed long)(Radio.SeekStep*10)*Radio.Direction, AutoEnd);
	else//AM
	{
		if (FreqInSWAM(Radio.Freq))
#ifdef AUTOSEEK
						HIT_CmdStartAutoSeek((signed long)(Radio.SeekStep*5)*Radio.Direction, WaitCmdEnd);
#else
						HIT_CmdStartManualSeek((signed long)(Radio.SeekStep*5)*Radio.Direction);
#endif
		else
#ifdef AUTOSEEK
						HIT_CmdStartAutoSeek((signed long)Radio.SeekStep*Radio.Direction, WaitCmdEnd);
#else
						HIT_CmdStartManualSeek((signed long)Radio.SeekStep*Radio.Direction);
#endif
	}
}

/*********************************************
	Function:		SeekOff
	Description:	exit auto seek or auto seek continue
	Write/Modify:	Yete HUANG
	Time:		2008-2
*********************************************/
void SeekOff(unsigned char SeekEndAction)
{
	HIT_CmdSeekEnd(SeekEndAction);
}



/*********************************************
	Function:		PresetOrder
	Description:
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:38:13
*********************************************/

#define PILOT_THS_ADDR 0X01B05D
void HIT_ForeceMono(void)
{
    static unsigned long pilotth;
    static char flag=0;
    if(flag == 0)  //mono
    {
    HIT_CmdReadMem(PILOT_THS_ADDR, 1);
    pilotth=Radio.HITDataBuff[3];
    pilotth <<= 8;
    pilotth |=Radio.HITDataBuff[4];
    pilotth <<= 8;
    pilotth |=Radio.HITDataBuff[5];
    HIT_CmdWriteMem(PILOT_THS_ADDR, 0x7fffff);
    HIT_CmdReadMem(PILOT_THS_ADDR, 1);
    flag=1;
    }
    else //restore original pilot threshold
    {
        HIT_CmdWriteMem(PILOT_THS_ADDR, pilotth);
        flag=0;
    }
}

/*********************************************
	Function:		HIT_DSPMode
	Description:	Set HIT DSP to FM,AM,IDLE,WX
	Write/Modify:	Yete HUANG
	Time:		March-2008
*********************************************/

void HIT_DSPMode(unsigned char DspWorkMode)
{
	Radio.HITDataBuff[3]=0x01;//Para 1
	Radio.HITDataBuff[4]=0xa0;
	Radio.HITDataBuff[5]=0x06;

	Radio.HITDataBuff[6]=0x0;//Para2,
	Radio.HITDataBuff[7]=0x0;
	Radio.HITDataBuff[8]=DspWorkMode;
	HIT_CmdWriteRead(CmdCode_WriteXmen,2,1);

}

/*********************************************
	Function:		HIT_BootcodeDownload
	Description:	Download the script file data
	Write/Modify:	Yete HUANG
	Time:		March-2008
*********************************************/
void HIT_BootcodeDownload(void)
{
	dprintf("####################HIT_BootcodeDownload in....\n");
	unsigned char num;
	unsigned int i,j, ret;
	i=0;
	while(I2CBootData[i]!=0xff)
	{
		num=I2CBootData[i];
		//I2C3Start();
		//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress
		//I2C3Out8Bit((I2CBootData[i+1]&0x01)|0xe0);//addr1
		//I2C3Out8Bit(I2CBootData[i+2]);
		//I2C3Out8Bit(I2CBootData[i+3]);
		//for(j=i+4;j<i+4*num+4;j++)
		//{
		//	//I2C3Out8Bit(I2CBootData[j]);
		//	dprintf("%d  ", I2CBootData[j]);
		//}
		//I2C3Stop();
		dprintf("num: %d\n", num);
		I2CBootData[i+1] = (I2CBootData[i+1]&0x01)|0xe0;
		ret = io2w_write( 3+num*4, I2CBootData+i+1);
		if(ret < 0)
			printf("########################HIT_BootcodeDownload io2w_write error....\n");
		i=i+4*num+4;
	}
}



/*********************************************
	Function:		HIT_GetAverageQuality
	Description:	Get the reception average quality(using FIR), AverageNum <256
	Write/Modify:	Yete HUANG
	Time:		2008-April
*********************************************/
void HIT_GetCFQuality (unsigned char AverageNum)
{
	/* use cmd to read back the quality information*/
	unsigned int temp1,temp2,temp3,temp4;
	unsigned char i;

	temp1=0;
	temp2=0;
	temp3=0;
	temp4=0;

	if(FreqInFM(Radio.Freq))
	{
		dprintf("@@@@@@@@@@@@@@@@@@@@@@@@HIT_GetCFQuality: FM\n");
		for(i=0; i<AverageNum; i++)
		{
			if(FreqInWBFM(Radio.Freq))
				//wb_fs_qu_det(0x01a095)
				HIT_CmdReadMem(0x01a095, 2);
			else
			//fm_fs_qu_det(0x01a03e),fm_mp_adj_dv(0x01a03f)
				HIT_CmdReadMem(0x01a03e, 2);
				temp1=temp1+Radio.HITDataBuff[3];
				temp2=temp2+Radio.HITDataBuff[5];
				temp3=temp3+Radio.HITDataBuff[6];
				temp4=temp4+Radio.HITDataBuff[7];
		}
		Radio.Smeter=temp1/AverageNum;
		Radio.Detuning=temp2/AverageNum;
		Radio.Multipath=temp3/AverageNum;
		Radio.AdjChannel=temp4/AverageNum;
		Wait10us(5);
	}
	else
	{
		dprintf("@@@@@@@@@@@@@@@@@@@@@@@@HIT_GetCFQuality: AM\n");
		for(i=0; i<AverageNum; i++)
		{
			//am_fs_qu_det(0x01a1c7),am_mp_adj_dv(0x01a1c8)
			HIT_CmdReadMem(0x01a1c7, 2);
			temp1=temp1+Radio.HITDataBuff[3];
			temp2=temp2+Radio.HITDataBuff[5];
			temp3=temp3+Radio.HITDataBuff[6];
			temp4=temp4+Radio.HITDataBuff[7];
		}
		Radio.Smeter=temp1/AverageNum;
		Radio.Detuning=temp2/AverageNum;
		Radio.Multipath=temp3/AverageNum;
		Radio.AdjChannel=temp4/AverageNum;
		Wait10us(5);

	}
    dprintf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@smeter[%d] detun[%d] multi[%d] adj[%d]@@@@@@@@@@@@@@@@@@@@@2\n", Radio.Smeter,Radio.Detuning,Radio.Multipath,Radio.AdjChannel);

}


/*********************************************
	Function:		HIT_CombinedQuality
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-May
*********************************************/
/*
unsigned char HIT_CombinedQuality (unsigned char fs, unsigned char det, unsigned char ac, unsigned char mp)
{
	float f_fs, f_det, f_ac, f_mp;
	float fator_ac, fator_mp;
	float quality;

	f_fs=(float)fs/255.0;
	f_det=(float)det/31.0;

	if(fs>DB25)
	{
		fator_ac=1.0;
	}
	else if(fs>DB10)
	{
		fator_ac=1.0/(25.0-10.0);
	}
	else
	{
		fator_ac=0.0;
	}
	f_ac=((float)ac*fator_ac)/127.0;

	if(fs>DB30)
	{
		fator_mp=1.0;
	}
	else if(fs>DB10)
	{
		fator_mp=1.0/(30.0-10.0);
	}
	else
	{
		fator_mp=0.0;
	}
	f_mp=((float)mp*fator_mp)/31.0;

	quality=f_fs*Radio.Qual_fs_factor-f_det*Radio.Qual_det_fator-f_ac*Radio.Qual_ac_factor-f_mp*Radio.Qual_mp_fator;

	return((unsigned char)(quality*255.0));

}
*/
/*********************************************
	Function:		HIT_CmdWriteRead
	Description:	Write to or read from HIT with command
	Write/Modify:	Yete HUANG
	Time:		2008-March
*********************************************/
void HIT_CmdWriteRead(unsigned char cmdcode,  unsigned char para_num, unsigned char rece_num)
{

	Radio.HITAddressBuff[0]=0x01;//Send command address 0x019000
	Radio.HITAddressBuff[1]=0x90;
	Radio.HITAddressBuff[2]=0x00;

	Radio.HITDataBuff[0]=(cmdcode&0xf0)>>4;
	Radio.HITDataBuff[1]=(cmdcode&0x0f)<<4;
	Radio.HITDataBuff[2]= (para_num+1) & 0x1f;

	HIT_DirectWrite(para_num+1, ModeXX4, 1);

	Radio.HITAddressBuff[2]=0x0a;//command read address 0x01900a
	HIT_DirectRead(rece_num, ModeXX4);

}

int HIT_I2C_Write(unsigned char HITAddressBuff[], unsigned char HITDataBuff[], int datalen, unsigned char flag)
{
	unsigned char HIT_Write_buff[50];
	int ret;

	dprintf("#####################HIT_I2C_Write --> datalen[%d], flag[%x]\n", datalen, flag);
	HITAddressBuff[0] = (HITAddressBuff[0]&0x01) | flag;

	memcpy(HIT_Write_buff, HITAddressBuff, 3);
	
	if(datalen > 0)
		memcpy(HIT_Write_buff+3, HITDataBuff, datalen);
	
	ret = io2w_write(datalen+3, HIT_Write_buff);
	if(ret < 0 )
		printf("###############HIT_I2C_Write io2w_write error!############\n");

	return ret;
}


/*********************************************
Function:		HIT_CmdWriteRead
Description:	Directly Write data to HIT
Write/Modify:	Yete HUANG
Time:		2008-March
 *********************************************/
void HIT_DirectWrite(unsigned char DataNum, unsigned char mode, unsigned char cmd)//bool cmd)
{
	unsigned int i,j;
	unsigned int checksum[4]={0,0,0,0};
	unsigned char check[3]={0,0,0};

	switch(mode)
		{
		case ModeXX1:
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress
		  	//I2C3Out8Bit((Radio.HITAddressBuff[0]&0x01)|0x80);//mode+addr1
		  	//I2C3Out8Bit(Radio.HITAddressBuff[1]);
			//I2C3Out8Bit(Radio.HITAddressBuff[2]);
		  	//I2C3Out8Bit(Radio.HITDataBuff[0]);
		  	//I2C3Out8Bit(Radio.HITDataBuff[1]);
		  	//I2C3Out8Bit(Radio.HITDataBuff[2]);
		  	//I2C3Out8Bit(Radio.HITDataBuff[3]);
			//I2C3Stop();
			HIT_I2C_Write(Radio.HITAddressBuff, Radio.HITDataBuff, 4, 0x80);
			break;
		case ModeXX2:
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress
		  	//I2C3Out8Bit((Radio.HITAddressBuff[0]&0x01)|0x90);//mode+addr1
		  	//I2C3Out8Bit(Radio.HITAddressBuff[1]);
			//I2C3Out8Bit(Radio.HITAddressBuff[2]);
		  	//I2C3Out8Bit(Radio.HITDataBuff[0]);
		  	//I2C3Out8Bit(Radio.HITDataBuff[1]);
		  	//I2C3Out8Bit(Radio.HITDataBuff[2]);
			//I2C3Stop();
			HIT_I2C_Write(Radio.HITAddressBuff, Radio.HITDataBuff, 3, 0x90);
			break;
		case ModeXX3:
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress
		  	//I2C3Out8Bit((Radio.HITAddressBuff[0]&0x01)|0xe0);//mode+addr1
		  	//I2C3Out8Bit(Radio.HITAddressBuff[1]);
			//I2C3Out8Bit(Radio.HITAddressBuff[2]);
			//for(i=0;i<DataNum;i++)
			//{
			//	I2C3Out8Bit(Radio.HITDataBuff[i*4]);
			//	I2C3Out8Bit(Radio.HITDataBuff[i*4+1]);
			//	I2C3Out8Bit(Radio.HITDataBuff[i*4+2]);
			//	I2C3Out8Bit(Radio.HITDataBuff[i*4+3]);
			//	if(cmd)
			//	{
			//		for(j=3; j>=1;j--)
			//		{
			//		 	checksum[j]=checksum[j]+Radio.HITDataBuff[i*4+j];
			//			if (checksum[j] >= 256)
			//			{
			//	                    checksum[j - 1] = checksum[j - 1] + 1;
			//	                    checksum[j] = checksum[j] - 256;
			//			}
			//		}
			//		 checksum[0]=checksum[0]+Radio.HITDataBuff[i*4];
			//		 if (checksum[0]>=256)
			//		 	checksum[0]=checksum[0]-256;

			//	}
			//}
			//if(cmd)
			//{
			//	I2C3Out8Bit(checksum[0]&0xff);
			//	I2C3Out8Bit(checksum[1]&0xff);
			//	I2C3Out8Bit(checksum[2]&0xff);
			//	I2C3Out8Bit(checksum[3]&0xff);

			//}
			//I2C3Stop();
			//if(flg_i2c_log)
			//{
			//	DEBUG_HIT_SENDSTRING("I2C W  ");
			//	DEBUG_HEX_HIT_OUTVAL(HITTUNER_I2C_ADDR);
			//	DEBUG_HEX_HIT_OUTVAL((Radio.HITAddressBuff[0]&0x01)|0xe0);
			//	DEBUG_HEX_HIT_OUTVAL(Radio.HITAddressBuff[1]);
			//	DEBUG_HEX_HIT_OUTVAL(Radio.HITAddressBuff[2]);
			//	DEBUG_HIT_SENDSTRING(" ");

			//	for(i=0;i<DataNum;i++)
			//	{
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*4]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*4+1]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*4+2]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*4+3]);
			//		DEBUG_HIT_SENDSTRING(" ");
			//	}
			//	if(cmd)
			//	{
			//		DEBUG_HEX_HIT_OUTVAL(checksum[0]&0xff);
			//		DEBUG_HEX_HIT_OUTVAL(checksum[1]&0xff);
			//		DEBUG_HEX_HIT_OUTVAL(checksum[2]&0xff);
			//		DEBUG_HEX_HIT_OUTVAL(checksum[3]&0xff);
			//	}
			//	DEBUG_HIT_SENDSTRING("\n");
			//}
			break;
		case ModeXX4:
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress
		  	//I2C3Out8Bit((Radio.HITAddressBuff[0]&0x01)|0xf0);//mode+addr1
		  	//I2C3Out8Bit(Radio.HITAddressBuff[1]);
			//I2C3Out8Bit(Radio.HITAddressBuff[2]);
			for(i=0;i<DataNum;i++)
			{
				//I2C3Out8Bit(Radio.HITDataBuff[i*3]);
				//I2C3Out8Bit(Radio.HITDataBuff[i*3+1]);
				//I2C3Out8Bit(Radio.HITDataBuff[i*3+2]);

				if(cmd)
				{
					for(j=2; j>=1;j--)
					{
					 	checksum[j]=checksum[j]+Radio.HITDataBuff[i*3+j];
						if (checksum[j] >= 256)
						{
				                    checksum[j - 1] = checksum[j - 1] + 1;
				                    checksum[j] = checksum[j] - 256;
						}
					}
					 checksum[0]=checksum[0]+Radio.HITDataBuff[i*3];
					 if (checksum[0]>=256)
					 	checksum[0]=checksum[0]-256;

				}
			}
			//if(cmd)
			//{
			//	I2C3Out8Bit(checksum[0]&0xff);
			//	I2C3Out8Bit(checksum[1]&0xff);
			//	I2C3Out8Bit(checksum[2]&0xff);
			//}
			//I2C3Stop();

			//check[0] = checksum[0]&0xff;
			//check[1] = checksum[1]&0xff;
			//check[2] = checksum[2]&0xff;
			dprintf("Datanum: %d :checksum:%x - %x - %x\n",DataNum, checksum[0], checksum[1], checksum[2]);
			*(Radio.HITDataBuff + (DataNum*3)) = checksum[0]&0xff;
			*(Radio.HITDataBuff + (DataNum*3) + 1) = checksum[1]&0xff;
			*(Radio.HITDataBuff + (DataNum*3) + 2) = checksum[2]&0xff;
			//memcpy(Radio.HITDataBuff + (DataNum*3), check, 3);
			HIT_I2C_Write(Radio.HITAddressBuff, Radio.HITDataBuff, DataNum*3 + 3, 0xf0);
			//if(flg_i2c_log)
			//{
			//	DEBUG_HIT_SENDSTRING("I2C W  ");
			//	DEBUG_HEX_HIT_OUTVAL(HITTUNER_I2C_ADDR);
			//	DEBUG_HEX_HIT_OUTVAL((Radio.HITAddressBuff[0]&0x01)|0xf0);
			//	DEBUG_HEX_HIT_OUTVAL(Radio.HITAddressBuff[1]);
			//	DEBUG_HEX_HIT_OUTVAL(Radio.HITAddressBuff[2]);
			//	DEBUG_HIT_SENDSTRING(" ");

			//	for(i=0;i<DataNum;i++)
			//	{
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*3]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*3+1]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*3+2]);
			//		DEBUG_HIT_SENDSTRING(" ");
			//	}
			//	if(cmd)
			//	{
			//		DEBUG_HEX_HIT_OUTVAL(checksum[0]&0xff);
			//		DEBUG_HEX_HIT_OUTVAL(checksum[1]&0xff);
			//		DEBUG_HEX_HIT_OUTVAL(checksum[2]&0xff);
			//	}
			//	DEBUG_HIT_SENDSTRING("\n");
			//}
			break;
		}

}
/*********************************************
	Function:		HIT_DirectRead
	Description:	Directly read data from HIT
	Write/Modify:	Yete HUANG
	Time:		2008-March
*********************************************/
void HIT_DirectRead(unsigned int DataNum, unsigned char mode)//bool cmd)
{
	unsigned int i;
	//DI;//asm sim;
	unsigned int tmp=DataNum;

	//I2C3Start();
	//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress/write

	switch(mode)
	{
		case ModeXX1:
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress/write
			//I2C3Out8Bit((Radio.HITAddressBuff[0]&0x01)|0x0);//mode+addr1
			//I2C3Out8Bit(Radio.HITAddressBuff[1]);
			//I2C3Out8Bit(Radio.HITAddressBuff[2]);
			//I2C3Stop();
			HIT_I2C_Write(Radio.HITAddressBuff, NULL, 0, 0);
			Wait10us(5);
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR+1);//chipaddress/read
			//Wait10us(5);
			//Radio.HITDataBuff[0]=I2C3In8Bit();
			//I2C3Ack();
			//Radio.HITDataBuff[1]=I2C3In8Bit();
			//I2C3Ack();
			//Radio.HITDataBuff[2]=I2C3In8Bit();
			//I2C3Ack();
			//Radio.HITDataBuff[3]=I2C3In8Bit();
			//I2C3Ack2();
			//I2C3Stop();
			io2w_read(4, Radio.HITDataBuff);
			break;

		case ModeXX2:
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress/write
			//I2C3Out8Bit((Radio.HITAddressBuff[0]&0x01)|0x10);//mode+addr1
			//I2C3Out8Bit(Radio.HITAddressBuff[1]);
			//I2C3Out8Bit(Radio.HITAddressBuff[2]);
			//I2C3Stop();
			HIT_I2C_Write(Radio.HITAddressBuff, NULL, 0, 0x10);
			Wait10us(5);
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR+1);//chipaddress/read
			//Wait10us(5);
			//Radio.HITDataBuff[0]=I2C3In8Bit();
			//I2C3Ack();
			//Radio.HITDataBuff[1]=I2C3In8Bit();
			//I2C3Ack();
			//Radio.HITDataBuff[2]=I2C3In8Bit();
			//I2C3Ack2();
			//I2C3Stop();
			io2w_read(4, Radio.HITDataBuff);
			break;

		case ModeXX3:
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress/write
			//I2C3Out8Bit((Radio.HITAddressBuff[0]&0x01)|0x60);//mode+addr1
			//I2C3Out8Bit(Radio.HITAddressBuff[1]);
			//I2C3Out8Bit(Radio.HITAddressBuff[2]);
			//I2C3Stop();
			//Wait10us(5);
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR+1);//chipaddress/read
			//Wait10us(5);
			//for(i=0;i<DataNum;i++)
			//{
			//	Radio.HITDataBuff[i*4]=I2C3In8Bit();
			//	I2C3Ack();
			//	Radio.HITDataBuff[i*4+1]=I2C3In8Bit();
			//	I2C3Ack();
			//	Radio.HITDataBuff[i*4+2]=I2C3In8Bit();
			//	I2C3Ack();
			//	Radio.HITDataBuff[i*4+3]=I2C3In8Bit();
			//	if(i==DataNum-1)
			//	{
			//		I2C3Ack2();
			//	}
			//	else
			//	{
			//		I2C3Ack();
			//	}
			//}
			//I2C3Stop();
			//if(flg_i2c_log)
			//{
			//	DEBUG_HIT_SENDSTRING("I2C W  ");
			//	DEBUG_HEX_HIT_OUTVAL(HITTUNER_I2C_ADDR);
			//	DEBUG_HEX_HIT_OUTVAL((Radio.HITAddressBuff[0]&0x01)|0x60);
			//	DEBUG_HEX_HIT_OUTVAL(Radio.HITAddressBuff[1]);
			//	DEBUG_HEX_HIT_OUTVAL(Radio.HITAddressBuff[2]);
			//	DEBUG_HIT_SENDSTRING("\n");
			//	DEBUG_HIT_SENDSTRING("I2C R  ");
			//	DEBUG_HEX_HIT_OUTVAL(HITTUNER_I2C_ADDR+1);
			//	DEBUG_HIT_SENDSTRING(" ");
			//	for(i=0;i<DataNum;i++)
			//	{
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*4]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*4+1]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*4+2]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*4+3]);
			//		DEBUG_HIT_SENDSTRING(" ");
			//	}
			//	DEBUG_HIT_SENDSTRING("\n");
			//}
			break;
		case ModeXX4:
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR);//chipaddress/write
			//I2C3Out8Bit((Radio.HITAddressBuff[0]&0x01)|0x70);//mode+addr1
			//I2C3Out8Bit(Radio.HITAddressBuff[1]);
			//I2C3Out8Bit(Radio.HITAddressBuff[2]);
			//I2C3Stop();
			HIT_I2C_Write(Radio.HITAddressBuff, NULL, 0, 0x70);
			Wait10us(5);
			//I2C3Start();
			//I2C3Out8Bit(HITTUNER_I2C_ADDR+1);//chipaddress/read
			//Wait10us(5);

			//for(i=0;i<tmp;i++)
			//{
			//	Radio.HITDataBuff[i*3]=I2C3In8Bit();
			//	I2C3Ack();
			//	Radio.HITDataBuff[i*3+1]=I2C3In8Bit();
			//	I2C3Ack();
			//	Radio.HITDataBuff[i*3+2]=I2C3In8Bit();
			//	if(i==tmp-1)
			//	{
			//		I2C3Ack2();
			//	}
			//	else
			//	{
			//		I2C3Ack();
			//	}
			//}
			//I2C3Stop();
			
			int ret;
			ret = io2w_read(tmp*3, Radio.HITDataBuff);
			if(ret < 0)
				dprintf("############read error...\n");
			//if(flg_i2c_log)
			//{
			//	DEBUG_HIT_SENDSTRING("I2C W  ");
			//	DEBUG_HEX_HIT_OUTVAL(HITTUNER_I2C_ADDR);
			//	DEBUG_HEX_HIT_OUTVAL((Radio.HITAddressBuff[0]&0x01)|0x70);
			//	DEBUG_HEX_HIT_OUTVAL(Radio.HITAddressBuff[1]);
			//	DEBUG_HEX_HIT_OUTVAL(Radio.HITAddressBuff[2]);
			//	DEBUG_HIT_SENDSTRING("\n");
			//	DEBUG_HIT_SENDSTRING("I2C R  ");
			//	DEBUG_HEX_HIT_OUTVAL(HITTUNER_I2C_ADDR+1);
			//	DEBUG_HIT_SENDSTRING(" ");
			//	for(i=0;i<tmp;i++)
			//	{
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*3]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*3+1]);
			//		DEBUG_HEX_HIT_OUTVAL(Radio.HITDataBuff[i*3+2]);
			//		DEBUG_HIT_SENDSTRING(" ");
			//	}
			//	DEBUG_HIT_SENDSTRING("\n");
			//}
			break;
	}

}


/*********************************************
Function:		HIT_CmdSetFreq
Description:	Tuned to specific frequency
Write/Modify:	Yete HUANG
Time:		2007-12
 *********************************************/
void HIT_CmdSetFreq(unsigned int setfreq)
{
	unsigned char cmdcode;
	unsigned long freq;
	cmdcode=CmdCode_SetFreq;
	if(FreqInWBFM(setfreq))
		freq=(unsigned long)setfreq+100000;
	else if(FreqInFM(setfreq))
		freq=(unsigned long)setfreq*10;
	else if(FreqInSWAM(setfreq))
		freq=(unsigned long)((setfreq-1384)*5);
	else
		freq=(unsigned long)setfreq;

	Radio.HITDataBuff[3]=(unsigned char)((freq&0xFF0000)>>16);//Para1
	Radio.HITDataBuff[4]=(unsigned char)((freq&0x00FF00)>>8);
	Radio.HITDataBuff[5]=(unsigned char)(freq&0x0000FF);

	HIT_CmdWriteRead(cmdcode,1,1);

}


/*********************************************
Function:		HIT_CmdSetFEReg
Description:	Set Front-end Registers. If the num=0, then just send cmd to FE registers to ROM-specified default values
Write/Modify:	Yete HUANG
Time:		2007-12
 *********************************************/
void HIT_CmdSetFEReg(unsigned char band, unsigned char start, unsigned char num)
{
	unsigned char cmdcode, i;
	cmdcode=CmdCode_SetFEReg;

	if(num==0)
	{
		//set FE registers to ROM-specified default values
		HIT_CmdWriteRead(cmdcode, 0,1);
	}
	else
	{
		//set FE registers to user-specified values, starting from specified subaddress
		Radio.HITDataBuff[3]=0x00;//para 1---Subaddress
		Radio.HITDataBuff[4]=0x00;
		Radio.HITDataBuff[5]=start;

		if(start==15)
		{
			if(band==FM)
			{
				Radio.HITDataBuff[6]=FMFEReg[8*3];
				Radio.HITDataBuff[7]=FMFEReg[8*3+1];
				Radio.HITDataBuff[8]=FMFEReg[8*3+2];
			}
			else if(band==WB)
			{
				Radio.HITDataBuff[6]=WXFEReg[8*3];
				Radio.HITDataBuff[7]=WXFEReg[8*3+1];
				Radio.HITDataBuff[8]=WXFEReg[8*3+2];
			}

			else
			{

				Radio.HITDataBuff[6]=AMFEReg[8*3];
				Radio.HITDataBuff[7]=AMFEReg[8*3+1];
				Radio.HITDataBuff[8]=AMFEReg[8*3+2];

			}
		}
		else
		{
			if(band==FM)
			{
				for(i=0; i<num; i++)
				{
					Radio.HITDataBuff[6+i*3]=FMFEReg[start*3+i*3];
					Radio.HITDataBuff[7+i*3]=FMFEReg[start*3+i*3+1];
					Radio.HITDataBuff[8+i*3]=FMFEReg[start*3+i*3+2];
				}
			}

			else if(band==WB)
			{
				for(i=0; i<num; i++)
				{
					Radio.HITDataBuff[6+i*3]=WXFEReg[start*3+i*3];
					Radio.HITDataBuff[7+i*3]=WXFEReg[start*3+i*3+1];
					Radio.HITDataBuff[8+i*3]=WXFEReg[start*3+i*3+2];
				}
			}


			else
			{
				for(i=0; i<num; i++)
				{
					Radio.HITDataBuff[6+i*3]=AMFEReg[start*3+i*3];
					Radio.HITDataBuff[7+i*3]=AMFEReg[start*3+i*3+1];
					Radio.HITDataBuff[8+i*3]=AMFEReg[start*3+i*3+2];
				}
			}
		}
		HIT_CmdWriteRead(cmdcode, num+1,1);
	}

}


/*********************************************
	Function:		HIT_CmdSetBand
	Description:	Set to band(FM/MW---USA/EUR/JP)
	Write/Modify:	Yete HUANG
	Time:		2007-12
*********************************************/
void HIT_CmdSetBand(unsigned char band)
{
	unsigned char cmdcode;
	unsigned long MinFreq,MaxFreq;
	cmdcode=CmdCode_SetBand;

	if (band==WB)
	{
		Radio.HITDataBuff[3]=0x00;//para 1
		Radio.HITDataBuff[4]=0x00;
		Radio.HITDataBuff[5]=0x01;

		Radio.HITDataBuff[6]=(unsigned char)((((unsigned long)Radio.MinFreq+100000)&0xFF0000)>>16);//Para 2
		Radio.HITDataBuff[7]=(unsigned char)((((unsigned long)Radio.MinFreq+100000)&0x00FF00)>>8);
		Radio.HITDataBuff[8]=(unsigned char)(((unsigned long)Radio.MinFreq+100000)&0x0000FF);

		Radio.HITDataBuff[9]=(unsigned char)((((unsigned long)Radio.MaxFreq+100000)&0xFF0000)>>16);//Para 3
		Radio.HITDataBuff[10]=(unsigned char)((((unsigned long)Radio.MaxFreq+100000)&0x00FF00)>>8);
		Radio.HITDataBuff[11]=(unsigned char)(((unsigned long)Radio.MaxFreq+100000)&0x0000FF);

		HIT_CmdWriteRead(cmdcode, 3, 1);

	}

	else if(band==FM)
	{
		Radio.HITDataBuff[3]=0x00;//para 1
		Radio.HITDataBuff[4]=0x00;
		Radio.HITDataBuff[5]=0x01;

		Radio.HITDataBuff[6]=(unsigned char)((((unsigned long)Radio.MinFreq*10)&0xFF0000)>>16);//Para 2
		Radio.HITDataBuff[7]=(unsigned char)((((unsigned long)Radio.MinFreq*10)&0x00FF00)>>8);
		Radio.HITDataBuff[8]=(unsigned char)(((unsigned long)Radio.MinFreq*10)&0x0000FF);

		Radio.HITDataBuff[9]=(unsigned char)((((unsigned long)Radio.MaxFreq*10)&0xFF0000)>>16);//Para 3
		Radio.HITDataBuff[10]=(unsigned char)((((unsigned long)Radio.MaxFreq*10)&0x00FF00)>>8);
		Radio.HITDataBuff[11]=(unsigned char)(((unsigned long)Radio.MaxFreq*10)&0x0000FF);

		HIT_CmdWriteRead(cmdcode, 3, 1);

	}

	else if (band==AM)
	{

		if (band==AM &&(Radio.Area==AREA_USA||Radio.Area==AREA_LATIN))
		{
		Radio.HITDataBuff[3]=0x00;//para 1
		Radio.HITDataBuff[4]=0x00;
			Radio.HITDataBuff[5]=0x03;

			Radio.HITDataBuff[6]=(unsigned char)(((unsigned long)Radio.MinFreq&0xFF0000)>>16);//Para 2
		Radio.HITDataBuff[7]=(unsigned char)(((unsigned long)Radio.MinFreq&0x00FF00)>>8);
		Radio.HITDataBuff[8]=(unsigned char)((unsigned long)Radio.MinFreq&0x0000FF);

			Radio.HITDataBuff[9]=(unsigned char)(((unsigned long)Radio.MaxFreq&0xFF0000)>>16);//Para 3
		Radio.HITDataBuff[10]=(unsigned char)(((unsigned long)Radio.MaxFreq&0x00FF00)>>8);
		Radio.HITDataBuff[11]=(unsigned char)((unsigned long)Radio.MaxFreq&0x0000FF);

			HIT_CmdWriteRead(cmdcode, 3,1);

		}

		else   // other area
		{
			Radio.HITDataBuff[3]=0x00;//para 1
			Radio.HITDataBuff[4]=0x00;
			Radio.HITDataBuff[5]=0x02;

			Radio.HITDataBuff[6]=(unsigned char)(((unsigned long)Radio.MinFreq&0xFF0000)>>16);//Para 2
			Radio.HITDataBuff[7]=(unsigned char)(((unsigned long)Radio.MinFreq&0x00FF00)>>8);
			Radio.HITDataBuff[8]=(unsigned char)((unsigned long)Radio.MinFreq&0x0000FF);

			Radio.HITDataBuff[9]=(unsigned char)(((unsigned long)Radio.MaxFreq&0xFF0000)>>16);//Para 3
			Radio.HITDataBuff[10]=(unsigned char)(((unsigned long)Radio.MaxFreq&0x00FF00)>>8);
			Radio.HITDataBuff[11]=(unsigned char)((unsigned long)Radio.MaxFreq&0x0000FF);

			HIT_CmdWriteRead(cmdcode, 3, 1);
		}
	}
	else if (band==SW)
	{
		Radio.HITDataBuff[3]=0x00;//para 1
		Radio.HITDataBuff[4]=0x00;
		Radio.HITDataBuff[5]=0x02;

		MinFreq=(unsigned long)((Radio.MinFreq-1384)*5);
		MaxFreq=(unsigned long)((Radio.MaxFreq-1384)*5);
		Radio.HITDataBuff[6]=(unsigned char)((MinFreq&0xFF0000)>>16);//Para 2
		Radio.HITDataBuff[7]=(unsigned char)((MinFreq&0x00FF00)>>8);
		Radio.HITDataBuff[8]=(unsigned char)(MinFreq&0x0000FF);

		Radio.HITDataBuff[9]=(unsigned char)((MaxFreq&0xFF0000)>>16);//Para 3
		Radio.HITDataBuff[10]=(unsigned char)((MaxFreq&0x00FF00)>>8);
		Radio.HITDataBuff[11]=(unsigned char)(MaxFreq&0x0000FF);

		HIT_CmdWriteRead(cmdcode, 3, 1);

	}
	else
	{

	}

}




/*********************************************
	Function:		HIT_CmdSeekTH
	Description:    It's used to program the measurement time and the thresholds of the station detector in seek mode. tmeas = M_CYC/45600 seconds
				fieldstrength /quality:  				high for good reception quality
				adjacent channel/detune/multipath:	low for good reception quality
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void HIT_CmdSeekTH(unsigned long MeasCycle, unsigned long fieldstrengthTH, unsigned long AdjTH, unsigned long DetuneTH, unsigned long MultipathTH, unsigned long QualityTH)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_SetSeekTH;

	Radio.HITDataBuff[3]=(unsigned char)((MeasCycle&0xFF0000)>>16);//Para1   Tmeas=MeasCycle/45600= MeasCycle*0.02193ms
	Radio.HITDataBuff[4]=(unsigned char)((MeasCycle&0x00FF00)>>8);
	Radio.HITDataBuff[5]=(unsigned char)(MeasCycle&0x0000FF);

	Radio.HITDataBuff[6]=(unsigned char)((fieldstrengthTH&0xFF0000)>>16);//Para2
	Radio.HITDataBuff[7]=(unsigned char)((fieldstrengthTH&0x00FF00)>>8);
	Radio.HITDataBuff[8]=(unsigned char)(fieldstrengthTH&0x0000FF);

	Radio.HITDataBuff[9]=(unsigned char)((AdjTH&0xFF0000)>>16);//Para3
	Radio.HITDataBuff[10]=(unsigned char)((AdjTH&0x00FF00)>>8);
	Radio.HITDataBuff[11]=(unsigned char)(AdjTH&0x0000FF);

	Radio.HITDataBuff[12]=(unsigned char)((DetuneTH&0xFF0000)>>16);//Para4
	Radio.HITDataBuff[13]=(unsigned char)((DetuneTH&0x00FF00)>>8);
	Radio.HITDataBuff[14]=(unsigned char)(DetuneTH&0x0000FF);

	Radio.HITDataBuff[15]=(unsigned char)((MultipathTH&0xFF0000)>>16);//Para5
	Radio.HITDataBuff[16]=(unsigned char)((MultipathTH&0x00FF00)>>8);
	Radio.HITDataBuff[17]=(unsigned char)(MultipathTH&0x0000FF);

	Radio.HITDataBuff[18]=(unsigned char)((QualityTH&0xFF0000)>>16);//Para6
	Radio.HITDataBuff[19]=(unsigned char)((QualityTH&0x00FF00)>>8);
	Radio.HITDataBuff[20]=(unsigned char)(QualityTH&0x0000FF);

	HIT_CmdWriteRead(cmdcode,6,1);


}

/*********************************************
	Function:		HIT_CmdStartManualSeek
	Description:    It's used to manual seek(A decrement can be achieved by sending a negative increment parameter.)
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void HIT_CmdStartManualSeek(signed long FreqIncrement)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_StartManuSeek;

	Radio.HITDataBuff[3]=(unsigned char)((FreqIncrement&0xFF0000)>>16);//Para1
	Radio.HITDataBuff[4]=(unsigned char)((FreqIncrement&0x00FF00)>>8);
	Radio.HITDataBuff[5]=(unsigned char)(FreqIncrement&0x0000FF);

	HIT_CmdWriteRead(cmdcode,1,1);
}

/*********************************************
	Function:		HIT_CmdStartAutoSeek
	Description:    The AutoSeek command starts an automatic seek cycle.
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void HIT_CmdStartAutoSeek(signed long FreqIncrement, unsigned char AutoSeekEnd)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_StartAutoSeek;

	Radio.HITDataBuff[3]=(unsigned char)((FreqIncrement&0xFF0000)>>16);//Para1
	Radio.HITDataBuff[4]=(unsigned char)((FreqIncrement&0x00FF00)>>8);
	Radio.HITDataBuff[5]=(unsigned char)(FreqIncrement&0x0000FF);

	Radio.HITDataBuff[6]=0x0;//Para2
	Radio.HITDataBuff[7]=0x0;
	Radio.HITDataBuff[8]=AutoSeekEnd;//AutoSeekEnd=1: automatically unmute the audio and leave the seek mode after a search stop condition has been detected

	HIT_CmdWriteRead(cmdcode,2,1);
}

/*********************************************
	Function:		HIT_CmdSeekEnd
	Description:    The SeekEnd command stops or continues an automatic or manual seek cycle.
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void HIT_CmdSeekEnd(unsigned char SeekEndAction)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_SeekEnd;

	Radio.HITDataBuff[3]=0x0;//Para1
	Radio.HITDataBuff[4]=0x0;
	Radio.HITDataBuff[5]=SeekEndAction&0x03;//Seek action code: 01-Continue, 10-StopMuted, 11-StopUnmuted

	HIT_CmdWriteRead(cmdcode,1,1);
}

/*********************************************
	Function:		HIT_CmdReadTDSR
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void HIT_CmdReadTDSR(void)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_ReadTDS;
	HIT_CmdWriteRead(cmdcode,0,1+1+1); //add read out checksum
	if(Radio.HITDataBuff[4]&0x04)
		Radio.F_AlignDone=1;
	else
		Radio.F_AlignDone=0;

	if(Radio.HITDataBuff[4]&0x08)
		Radio.IS=1;
	else
		Radio.IS=0;

	if(Radio.HITDataBuff[5]&0x40)
		Radio.F_TDBusy=1;
	else
		Radio.F_TDBusy=0;
	dprintf("====%x %x %x %x===== \n", Radio.HITDataBuff[0],Radio.HITDataBuff[1],Radio.HITDataBuff[2],Radio.HITDataBuff[5]);
}

/*********************************************
	Function:		HIT_CmdReadFEReg
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-02
*********************************************/
void HIT_CmdReadFEReg(unsigned char FESubaddress, unsigned char num)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_ReadFEReg;
	Radio.HITDataBuff[3]=0x0;//Para1
	Radio.HITDataBuff[4]=0x0;
	Radio.HITDataBuff[5]=FESubaddress;

	Radio.HITDataBuff[6]=0x0;//Para2
	Radio.HITDataBuff[7]=0x0;
	Radio.HITDataBuff[8]=num;

	HIT_CmdWriteRead(cmdcode,2,num+1);
}

/*********************************************
	Function:		HIT_CmdReadSeekStatus
	Description:	read out Seek status
	Write/Modify:	Yete HUANG
	Time:		2009-Nov.
*********************************************/
void HIT_CmdReadSeekStatus(void)
{
	unsigned char cmdcode;
	unsigned long frequency;

	cmdcode=CmdCode_ReadSeekStatus;
	Radio.HITDataBuff[3]=0x0;//Para1
	Radio.HITDataBuff[4]=0x0;
	Radio.HITDataBuff[5]=SeekStatus_RstQaul;
	//Radio.HITDataBuff[5]=SeekStatus_NoRstQaul;


	HIT_CmdWriteRead(cmdcode, 1, 1+2+2+1);
	Radio.Smeter=Radio.HITDataBuff[12];
	Radio.Detuning=Radio.HITDataBuff[14];
	Radio.Multipath=Radio.HITDataBuff[9];
	Radio.AdjChannel=Radio.HITDataBuff[10];
	frequency=((unsigned long)Radio.HITDataBuff[6]<<16)&0xFF0000;//RF frequency
	frequency=(((unsigned long)Radio.HITDataBuff[7]<<8)&0x00FF00) |frequency;
	frequency=((unsigned long)Radio.HITDataBuff[8]&0x0000FF) |frequency;
	if(FreqInWBFM(frequency-100000))
		Radio.Freq=frequency-100000;
	else if(FreqInFM(frequency/10))
		Radio.Freq=frequency/10;
	else if(FreqInSWAM(frequency/5+1384))
		Radio.Freq=frequency/5+1384;
	else
		Radio.Freq=frequency;


#ifdef AUTOSEEK
	HIT_CmdWriteRead(cmdcode, 1, 1+2+2);
	if(Radio.HITDataBuff[3]&SeekStatus_Seekok)
		Radio.F_GoodStation=1;
	else
		Radio.F_GoodStation=0;
	if(Radio.HITDataBuff[3]&SeekStatus_FullCircle)
		Radio.F_SeekFullCycle=1;
	else
		Radio.F_SeekFullCycle=0;

#else
	if((Radio.CurrentBand<FM_BAND_NUM && Radio.Smeter>= 195 && Radio.AdjChannel<=120 && Radio.Multipath<= 20 &&  Radio.Detuning<6)
		|| (Radio.CurrentBand>=FM_BAND_NUM && Radio.Smeter>= 164 && Radio.Detuning<16))//customized seek threshold with using assisted manual seek method
	{
		Radio.F_GoodStation=1;
	}
	else
	{
		Radio.F_GoodStation=0;
	}


	if(Radio.BackupFreq == Radio.Freq)
	{

		Radio.F_SeekFullCycle=1;
	}
	else
	{
		Radio.F_SeekFullCycle=0;
	}

	if(!Radio.F_GoodStation&&!Radio.F_SeekFullCycle)
	{
		HIT_CmdSeekEnd(SeekContinue);
		Wait1ms(20);
	}
#endif

	Radio.F_TunerDispReq=1;


}

/*********************************************
	Function:		HIT_CmdStartAlign
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-2
*********************************************/
void HIT_CmdStartAlign(unsigned char AlignMode)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_StartAlignment;
	Radio.HITDataBuff[3]=0x0;//Para1
	Radio.HITDataBuff[4]=0x0;
	Radio.HITDataBuff[5]=AlignMode&0x03;
	//Radio.HITDataBuff[5]=AlignMode&0x01;
	HIT_CmdWriteRead(cmdcode,1,1);

}
/*********************************************
	Function:		HIT_CmdSetIRType
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-3
*********************************************/
void HIT_CmdSetIRType(unsigned char type)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_IR;
	Radio.HITDataBuff[3]=0x0;//Para1
	Radio.HITDataBuff[4]=0x0;
	//if(band==FM)
	//	Radio.HITDataBuff[5]=OnlineIR_FM;
	//else
		Radio.HITDataBuff[5]=type;
	HIT_CmdWriteRead(cmdcode,1,1);
}

/*********************************************
	Function:		HIT_CmdSetDiss
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-3
*********************************************/
void HIT_CmdSetDiss(unsigned char mode, unsigned char bandwidth)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_SetDiss;
	Radio.HITDataBuff[3]=0x0;//Para1
	Radio.HITDataBuff[4]=0x0;
	Radio.HITDataBuff[5]=mode;

	if(mode==Diss_FM_Manual ||mode==Diss_FM_Special)
	{
		Radio.HITDataBuff[6]=0x0;//Para2
		Radio.HITDataBuff[7]=0x0;
		Radio.HITDataBuff[8]=bandwidth;
		HIT_CmdWriteRead(cmdcode,2,1);
	}
	else
	{
		HIT_CmdWriteRead(cmdcode,1,1);
	}
}


/*********************************************
	Function:		HIT_CmdDynIS
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-9
*********************************************/
void HIT_CmdDynIS(unsigned char mode)
{
	unsigned char cmdcode;
	cmdcode=CmdCode_SetDynIS;
	/*Para1----0x000000 means disable the dynamic image selection;
			     0x000001 means DynIS is enabled without the cyclic rechecking of the optimum injection side.
			     0x000002, the DynIS is enabled with the cyclic rechecking of the optimum injection side.*/
	/*no parameter----mode=0xff*/
	if (mode==0xff)
	{
		HIT_CmdWriteRead(cmdcode,0,1);
	}
	else
	{
	Radio.HITDataBuff[3]=0x0;
	Radio.HITDataBuff[4]=0x0;
	Radio.HITDataBuff[5]=mode;
	HIT_CmdWriteRead(cmdcode,1,1);

	}
}


/*********************************************
	Function:		HIT_StereoIndicator
	Description:
	Write/Modify:	Yete HUANG
	Time:		2008-May
*********************************************/
unsigned char HIT_StereoIndicator(void)
{
	HIT_CmdReadMem(0x01a0fa, 1);//fm_pilot_zm1(0x01a0fa)

#ifdef HIT_44
	if(Radio.HITDataBuff[3]==0x7f&&Radio.HITDataBuff[4]==0xfb&&Radio.HITDataBuff[5]==0x00)//means the pilot is higher than the threshold ST_PILOT_DET_TH
#else
	if(Radio.HITDataBuff[3]==0x7f&&Radio.HITDataBuff[4]==0xff&&Radio.HITDataBuff[5]==0xfc)//means the pilot is higher than the threshold ST_PILOT_DET_TH
#endif

	{
		return 1;
	}
	else
	{
		return 0;
	}

}


/**************************************************************************************

The memory space has to be divided as follows

Memory 000000 to 018FFF      = DMA memory            ----> use commands 0x03 (write) 0x00 (read)
Memory 019000 to 019FFF      = DSP dualport memory   ----> use commands 0x03 (write) 0x00 (read)
Memory 01a000 to 01aFFF      = DSP X memory          ----> use commands 0x04 (write) 0x01 (read)
Memory 01b000 to 01bFFF      = DSP Y memory          ----> use commands 0x05 (write) 0x02 (read)
Memory 01C000 to 01FFFF      = DSP P memory          ----> use commands 0x03 (write) 0x00 (read)

For example
to write to Pram memory, address 0x1c000 the 32bit data 12345678 it is possible to do a direct write as follows

SPI W  9801C000  12345678

But equivalently can be processed a script as follows:

01C000, 12345678, 8A

It will be translated in a write IP command (0x03) with data splitted in DATAH and DATAL

C 03 01C000 123456 345678

with  transmission of following sequence:

SPI W  F8019000  003004  01C000  123456  345678  487AD2 SPI W  7801900A
SPI R    003000


Where 01C000 =24 bit address
      123456 = 24 bit DATA_H
      345678 = 24 bit DATA_L

Note that bits 23_8 have to be indicated in both data H and data L The read routine has than to take the whole 32 bit word

So sending the command 0x00 read IP memory
C 00 01C000 2   we get

SPI W  F8019000  000003  01C000  000002  01C005 SPI W  7801900A
SPI R    000005  123456  345678

And from this read we have  123456 + 78  ==>  12345678 32bit data value


*******************************************************************************************/

unsigned char HIT_CmdWriteMem( unsigned long menAddr, unsigned long menData)
{
	unsigned char cmdcode;
    if( menAddr <= 0x019fff  || (menAddr >= 0x01c000 && menAddr <= 0x01ffff ))
    {
           cmdcode = CmdCode_WriteDMAmem;
    }
    else  if( menAddr >= 0x01a000 && menAddr <= 0x01afff)
    {
          cmdcode = CmdCode_WriteXmen;
    }
    else if( menAddr >= 0x01b000 && menAddr <= 0x01bfff )
    {
          cmdcode = CmdCode_WriteYmen;
    }

	if(CmdCode_WriteYmen == cmdcode || CmdCode_WriteXmen == cmdcode)
	{
		Radio.HITDataBuff[3]=(menAddr & 0x00ff0000)>>16;//Para 1
		Radio.HITDataBuff[4]=(menAddr & 0x0000ff00)>>8;
		Radio.HITDataBuff[5]=menAddr & 0x000000ff;
		Radio.HITDataBuff[6]=(menData & 0x00ff0000)>>16;//Para 1
		Radio.HITDataBuff[7]=(menData & 0x0000ff00)>>8;
		Radio.HITDataBuff[8]=menData & 0x000000ff;
		HIT_CmdWriteRead(cmdcode,2,1);
	}
	else if(CmdCode_WriteDMAmem == cmdcode)
	{
        Radio.HITDataBuff[3]=(menAddr & 0x00ff0000)>>16;    //Para 1  memory address
        Radio.HITDataBuff[4]=(menAddr & 0x0000ff00)>>8;
        Radio.HITDataBuff[5]=menAddr & 0x000000ff;
        Radio.HITDataBuff[6] = (menData & 0xff000000)>>24;//Para 2
        Radio.HITDataBuff[7] = (menData & 0x00ff0000)>>16;
        Radio.HITDataBuff[8] = (menData & 0x0000ff00)>>8;
        Radio.HITDataBuff[9] = Radio.HITDataBuff[7];
        Radio.HITDataBuff[10] = Radio.HITDataBuff[8] ;
        Radio.HITDataBuff[11] = menData & 0x000000ff;
        HIT_CmdWriteRead(cmdcode,3,1);
	}
	else
		;
	return 0;
}

unsigned char HIT_CmdReadMem(unsigned long menAddr, unsigned char Numword)
{
	unsigned char cmdcode;
        if( menAddr <= 0x019fff  || (menAddr >= 0x01c000 && menAddr <= 0x01ffff ))
        {
               cmdcode = CmdCode_ReadDMAmem;
        }
        else  if( menAddr >= 0x01a000 && menAddr <= 0x01afff)
        {
              cmdcode = CmdCode_ReadXmen;
        }
        else if( menAddr >= 0x01b000 && menAddr <= 0x01bfff )
        {
              cmdcode = CmdCode_ReadYmen;
        }else{
		
			dprintf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ cmdcode error...\n");
			return -1;
		
		}

	if(CmdCode_ReadYmen == cmdcode || CmdCode_ReadXmen == cmdcode)
	{
		Radio.HITDataBuff[3]=(menAddr & 0x00ff0000)>>16;//Para 1
		Radio.HITDataBuff[4]=(menAddr & 0x0000ff00)>>8;
		Radio.HITDataBuff[5]=menAddr & 0x000000ff;
		Radio.HITDataBuff[6]=0x0;//Para2, this vaule must be <=8
		Radio.HITDataBuff[7]=0x0;
		Radio.HITDataBuff[8]=Numword;
		HIT_CmdWriteRead(cmdcode,2,Numword+2);//command header+numword+checksum
	}
	else if(CmdCode_ReadDMAmem == cmdcode)
	{
		Radio.HITDataBuff[3]=(menAddr & 0x00ff0000)>>16;//Para 1
		Radio.HITDataBuff[4]=(menAddr & 0x0000ff00)>>8;
		Radio.HITDataBuff[5]=menAddr & 0x000000ff;
		Radio.HITDataBuff[6]=0x0;//Para2, this vaule must be <=8
		Radio.HITDataBuff[7]=0x0;
		//Radio.HITDataBuff[8]=Numword*2;
		//HIT_CmdWriteRead(cmdcode,2,Numword*2+1);//command header+numword
		Radio.HITDataBuff[8]=Numword;
		HIT_CmdWriteRead(cmdcode,2,Numword*2+2);//command header+numword+checksum

	}

	return 0;
}


void HIT_CmdWriteBeCoeff( unsigned char* p_be_coeff)
{
     unsigned char num, num_word;
     unsigned int i;
     unsigned char addr1, addr2, addr3;
     unsigned long addr;
     unsigned char cmdcode;
     unsigned char offset;
     unsigned long addr_range;
     while( *p_be_coeff != 0xff )
     {
          num = num_word = *p_be_coeff;
          addr1 = *(p_be_coeff+1);
          addr2 = *(p_be_coeff+2);
          addr3 = *(p_be_coeff+3);
          offset = 0;
          addr_range = *(p_be_coeff + 1);
          addr_range <<=8;
          addr_range |= *(p_be_coeff + 2);
          addr_range <<=8;
          addr_range |= *(p_be_coeff + 3);
          if( addr_range <= 0x019fff  || (addr_range >= 0x01c000 && addr_range <= 0x01ffff ))
          {
               cmdcode = CmdCode_WriteDMAmem;
          }
          else  if( addr_range >= 0x01a000 && addr_range <= 0x01afff)
          {
               cmdcode = CmdCode_WriteXmen;
          }
          else if( addr_range >= 0x01b000 && addr_range <= 0x01bfff )
          {
               cmdcode = CmdCode_WriteYmen;
          }

          if(cmdcode == CmdCode_WriteDMAmem)
          {
               for(i = 0; i < num; i++)
               {
                    Radio.HITDataBuff[3]=addr1;//Para 1  memory address
                    Radio.HITDataBuff[4]=addr2;
                    Radio.HITDataBuff[5]=addr3;
                    Radio.HITDataBuff[6] = *(p_be_coeff+4+i*4);//Para 2
                    Radio.HITDataBuff[7] = *(p_be_coeff+5+i*4);
                    Radio.HITDataBuff[8] = *(p_be_coeff+6+i*4);
                    Radio.HITDataBuff[9] = Radio.HITDataBuff[7];
                    Radio.HITDataBuff[10] = Radio.HITDataBuff[8] ;
                    Radio.HITDataBuff[11] = *(p_be_coeff+7+i*4);
                    HIT_CmdWriteRead(cmdcode,3,1);
                    addr = addr1;
                    addr <<=8;
                    addr |= addr2;
                    addr <<=8;
                    addr |= addr3;
                    addr++;
                    addr1 = (addr & 0x00ff0000)>>16;
                    addr2 = (addr & 0x0000ff00)>>8;
                    addr3 = addr & 0x000000ff;
               }
               p_be_coeff += num_word *4 +4;
          }
          else if(cmdcode == CmdCode_WriteXmen || cmdcode == CmdCode_WriteYmen)
          {
               while(num > 7)
               {
                    Radio.HITDataBuff[3] = addr1;       //Para 1  memory address
                    Radio.HITDataBuff[4] = addr2;
                    Radio.HITDataBuff[5] = addr3;
                    for(i=0; i<7; i++)
                    {
                         Radio.HITDataBuff[6+i*3] = *(p_be_coeff+4+i*4+offset);//Para 2
                         Radio.HITDataBuff[7+i*3] = *(p_be_coeff+5+i*4+offset);
                         Radio.HITDataBuff[8+i*3] = *(p_be_coeff+6+i*4+offset);
                    }
                    HIT_CmdWriteRead(cmdcode,8,1);
                    addr = addr1;
                    addr <<=8;
                    addr |= addr2;
                    addr <<=8;
                    addr |= addr3;
                    addr += 7;
                    addr1 = (addr & 0x00ff0000)>>16;
                    addr2 = (addr & 0x0000ff00)>>8;
                    addr3 = addr & 0x000000ff;
                    offset += 4*7;
                    num -= 7;
               }
               Radio.HITDataBuff[3]=addr1;//Para 1  memory address
               Radio.HITDataBuff[4]=addr2;
               Radio.HITDataBuff[5]=addr3;
               for(i=0; i<num; i++)
               {
                    Radio.HITDataBuff[6+i*3]=*(p_be_coeff+4+i*4+offset);//Para 2
                    Radio.HITDataBuff[7+i*3]=*(p_be_coeff+5+i*4+offset);
                    Radio.HITDataBuff[8+i*3]=*(p_be_coeff+6+i*4+offset);
               }
               HIT_CmdWriteRead(cmdcode,num+1,1);
               p_be_coeff += num_word *4 +4;
          }
     }
}

/*********************************************************
				File End
*********************************************************/

