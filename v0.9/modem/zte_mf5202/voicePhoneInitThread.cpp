#include "phoneConnection.h"
#include "voicePhoneInitThread.h"
#include "voicePhoneWorkThread.h"
#include "MobileInterFace.h"
#include "videoPhoneMonitor.h"


static const char *phoneInitCommandList[]=
{
	"AT\r\n",
#ifdef _8925_8D1
	"ATE0\r\n",
	"AT+IPR=115200\r\n",
	"AT&W\r\n",
	"AT+CFGRI=1\r\n",   //Indicate Ri When Using URC - for waking up sim800
#else
	"ATE1\r\n",
	"\032",           //SetREBUILD
	"AT+CCWA=1,0\r\n",	//SetCCWA
#endif
//	"AT+CHUP\r\n",	  //SetHANGUP
	"AT+CSCS=\"UCS2\"\r\n",  //SetCSCS
	"AT+CMGF=0\r\n",    //
//	"AT+CNMI=2,1\r\n",
#ifdef _8925_8D1
    "AT+MORING=1\r\n",
	"AT+CNMI=2,1,0,1,0\r\n",
#else
	"AT+CNMI=2,1,0,2,0\r\n",
#endif
	"AT+CLIP=1\r\n",
	"AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n",
	"AT+CLVL=5\r\n",
//	"AT+WAKEUPSMS=UTSCMDWAKE\r\n",
#ifdef _8925_8D1
	"AT+CBST=4,0,0\r\n",
#else
	"AT+CBST=134,1,0\r\n",
	"AT+ZSNT=0,0,0\r\n",
#endif
	"AT+CRC=1\r\n",
#ifndef _8925_8D1
	"AT+ZGINIT\r\n",
	"AT+ZGMODE=3\r\n",
	"AT+ZGQOS=50,255\r\n",
	"AT+ZGFIXRATE=2147483647,0\r\n",
        "AT+ZGRUN=2\r\n",
        "AT+ZAUDIO_HUNGUP_DELAY=100\r\n" //增加延迟100ms挂断电话
#endif
};


void VoicePhoneInitThreadC::startInit(PhoneMonitorVoiceInterface *voicePhone)
{
	m_voicePhone =voicePhone;
	start();
}

void VoicePhoneInitThreadC::run()
{
	char resultBuffer[100];
	for(int i=0;i<(int)(sizeof(phoneInitCommandList)/sizeof(const char *));i++)
	{
		int result =m_voicePhone->m_pPhoneConnection->sendAtCommand(phoneInitCommandList[i],resultBuffer,100,10000);

		if(result<0)
		{
			;//printf("Send AT cmd:[%s] error\n",phoneInitCommandList[i]);
		}
		else if(strstr(resultBuffer,"OK")==NULL)
		{
			;//printf("Send AT cmd:[%s] respose error. res=[%s]\n",resultBuffer);
		}
		else
		{
			;//printf("Send AT cmd:[%s] success\n",phoneInitCommandList[i]);
		}
	}

	m_voicePhone->m_mutex.lock();
	m_voicePhone->m_phoneStatus =PHONE_STATUS_INITED;
	emit phoneInitedSignal();//for bug:calling,auto-system reboot suddently because of unpredictable factor,calling is going on
	//fprintf(stderr,"\n$$$$$$4...line:%d,%s,%s...\n\n",__LINE__,__FUNCTION__,__FILE__);
	m_voicePhone->m_mutex.unlock();

	m_voicePhone->m_videoPhone->init(m_voicePhone->m_pCallback);
	m_voicePhone->m_pWorkThread->readPreviousMsg();
}


