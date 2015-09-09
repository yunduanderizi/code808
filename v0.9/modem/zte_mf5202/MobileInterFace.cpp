#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "MobileInterFace.h"
#include "phoneMonitorVoiceReportBase.h"
#include "phoneConnection.h"
#include "voicePhoneInitThread.h"
#include "voicePhoneWorkThread.h"
#include "videoPhoneMonitor.h"
#include "tcpWorkThread.h"
#include "debugMonitor.h"
#include "md5.h"
#include "utility/Utility.h"
#include "phoneMessage.h"


PhoneMonitorVoiceInterface::PhoneMonitorVoiceInterface()
{
	m_phoneStatus =PHONE_STATUS_NOT_INIT;
	m_callStatus  =CALL_IDLE;
	m_phoneNumber[0]=0;
	signal = Strength_0;
	simstate = -1;

	m_pPhoneConnection =new PhoneConnectionC();
	m_pInitThread =new VoicePhoneInitThreadC();
	m_pWorkThread =new VoicePhoneWorkThreadC();
	m_videoPhone  =new VideoPhoneMonitorC();
	m_ptcpThread = new tcpWorkThreadC();

	connect(m_pInitThread, SIGNAL(phoneInitedSignal()), this, SIGNAL(phoneInitedSignal()));//for bug:calling,auto-system reboot suddently because of unpredictable factor,calling is going on
}
PhoneMonitorVoiceInterface::~PhoneMonitorVoiceInterface()
{
}

#ifdef PHONE_MODULE_ZTEMF5200_AUTH
void PhoneMonitorVoiceInterface::getMd5Data(char *input, char *output)
{
	const char *data = input;
	unsigned char data_MD5[32] = {0};
	unsigned char data_MD5_char[33];
	memset(&data_MD5_char,0,sizeof(data_MD5_char));
	
	MD5_CTX context;
	MD5Init_UI(&context);
	MD5Update(&context, (unsigned char *)data, 16);
	MD5Final(data_MD5, &context);
	HextoASC((char *)data_MD5_char,data_MD5,sizeof(data_MD5));//将16进制的数转换为ASC
	
	strcpy(output, (char *)data_MD5_char);
}

int PhoneMonitorVoiceInterface::authentication()
{
	QMutexLocker locker(&m_mutex);
	char sendBuffer[200];
    char recvBuffer[200];
    int ret = 0;

    sprintf(sendBuffer, "AT+ZGKEY?\r\n");

	ret = m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);
	
	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL) && (strstr(recvBuffer, "+ZGKEY:") != NULL))
	{
		char cpTmp1[50];
		char cpTmp2[50];
		char *pTmp1 = cpTmp1;
		char *pTmp2 = NULL;
		char randStr[32];
		char outputStr[50];
		strcpy(cpTmp1, recvBuffer);

		if(strsep(&pTmp1, ":") != NULL)
		{
			strsep(&pTmp1, " ");
			pTmp2 = strsep(&pTmp1, "OK");
			strcpy(randStr, pTmp2);

			getMd5Data(randStr, outputStr);	

			memset(sendBuffer,0,sizeof(sendBuffer));
			sprintf(sendBuffer, "AT+ZVERIFY=\"%s\"\r\n", outputStr);

			ret = m_pPhoneConnection->sendAtCommand(sendBuffer, recvBuffer,200,2000);
			
			if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
			{
				IBD_PRINTF(DEBUG_EMERGENCY, "Ztemf5200 authentication success.");
				return PHONE_MONITOR_SUCCESS;
			}
		}
	}
	IBD_PRINTF(DEBUG_EMERGENCY, "Ztemf5200 authentication failed!");
	
	return PHONE_MONITOR_FAIL;
}
#endif

int PhoneMonitorVoiceInterface::start( PhoneMonitorVoiceReportBase *reportHandle )
{
	m_phoneStatus =PHONE_STATUS_NOT_INIT;
	m_callStatus  =CALL_IDLE;

	m_pCallback   =reportHandle;

	if(m_pPhoneConnection->openDev("/dev/ttyUSB2")<0)
		return -1;

	QThread::start();
#ifdef PHONE_MODULE_ZTEMF5200_AUTH	
	if(authentication() < 0)
	{
		printf("AUTH fail!!!\n");
		//m_pPhoneConnection->closeDev("/dev/ttyUSB2");
		return -1;
	}
#endif
	m_phoneStatus =PHONE_STATUS_INITTING;

	m_pInitThread->startInit(this);
	m_pWorkThread->init(this);
	m_ptcpThread->init(this);

//	m_videoPhone->init(reportHandle);
        getOperator();
	
	return PHONE_MONITOR_SUCCESS;
}

int PhoneMonitorVoiceInterface::stop()
{
	///xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	IBD_PRINTF(DEBUG_INFOMATION, " Function not supported Now ! \n")
	return PHONE_MONITOR_FAIL;
}




int PhoneMonitorVoiceInterface::dial(const char* cpNumber)
{
	if (strlen(cpNumber) > PHONE_MONITOR_NUMBER_LEN)
		return PHONE_MONITOR_PHONE_NUMBER_INVALID;

	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	if(m_callStatus!=CALL_IDLE)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer, "ATD%s;\r\n",cpNumber);

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);
        printf("ret is 000000000000000   %d;\r\n recvBuffer is %s\r\n",ret,recvBuffer);

	if(ret >= 0 && strstr(recvBuffer,"OK") != NULL && strstr(recvBuffer,"END") == NULL)
	{
		m_callStatus =CALL_CALLING;
		
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::pickUp()
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	if(m_callStatus!=CALL_COMING_CALL)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"ATA\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL  ||  strstr(recvBuffer, "BEGIN") != NULL))
	{
		m_callStatus =CALL_TALKING;
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::hangUp()
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	m_callStatus =CALL_IDLE;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CHUP\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL  ||  strstr(recvBuffer, "END") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::rtDial(char cbyte)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	if(m_callStatus!=CALL_TALKING)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+VTS=%c\r\n",cbyte);

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}



bool PhoneMonitorVoiceInterface::init3gfun()
{
	return true;
}

bool PhoneMonitorVoiceInterface::close3gfun()
{
	return true; 
}

int PhoneMonitorVoiceInterface::FaceTime_Dial(char* cpNumber)
{
	return m_videoPhone->dial(cpNumber);
}

bool PhoneMonitorVoiceInterface::FaceTime_PickUp()
{	
	return m_videoPhone->pickUp();
}

bool PhoneMonitorVoiceInterface::FaceTime_HangUp()
{
	return m_videoPhone->hangUp();
}

int PhoneMonitorVoiceInterface::sendSMS(const char* pcContent, const char* pcNumber)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned int iCurrentTime = tv.tv_sec;

	int contentlen = strlen(pcContent);
	if(contentlen == 0)
	{
		char content[2];
		content[0] = 32;
		content[1] = '\0';
		m_pWorkThread->appendSendMsg(pcNumber, content, iCurrentTime);

		return PHONE_MONITOR_SUCCESS;
	}

	m_pWorkThread->appendSendMsg(pcNumber, pcContent, iCurrentTime);
	
	return PHONE_MONITOR_SUCCESS;
}

int PhoneMonitorVoiceInterface::sendSMSWithId(const char* pcContent, const char* pcNumber, unsigned int* id)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned int iCurrentTime = tv.tv_sec;
	*id = iCurrentTime;
		
	int contentlen = strlen(pcContent);
	if(contentlen == 0)
	{
		char content[2];
		content[0] = 32;
		content[1] = '\0';
		m_pWorkThread->appendSendMsg(pcNumber, content, iCurrentTime);

		return PHONE_MONITOR_SUCCESS;
	}

	m_pWorkThread->appendSendMsg(pcNumber, pcContent, iCurrentTime);
	
	return PHONE_MONITOR_SUCCESS;
}

int PhoneMonitorVoiceInterface::getRSSI()
{
	char *p=NULL;
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return -1;

	char sendBuffer[200];
        char recvBuffer[256];

	int iSigValue = -1;
        int ret = 0;
        static int iTmpSigValue = iSigValue;
 
		int csstate = 0;

        sprintf(sendBuffer,"AT+CSQ\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);
	
	if(ret >= 0 && ((p=strstr(recvBuffer, "+CSQ:")) != NULL))
	{
		char cpTmp[300];
		char *cpSig =NULL;

		//strcpy(cpTmp,recvBuffer);
		strcpy(cpTmp,p);
		p=cpTmp;

		if(strsep(&p, ":") != NULL)
		{
			if((cpSig = strsep(&p,",")) != NULL)
			{
				iSigValue = atoi(cpSig);
			}
		}
	}
        else if(ret >= 0)
        {   
                iSigValue = iTmpSigValue;
        }   
#ifdef _8925_8D1
        signal = Strength_0;
	if( iSigValue < 3 || iSigValue > 31 )
		signal = Strength_0;
	else if( iSigValue >= 3 && iSigValue < 7 )
		signal = Strength_1;
	else if( iSigValue >= 7 && iSigValue < 10 )
		signal = Strength_2;
	else if( iSigValue >= 10 && iSigValue < 13 )
		signal = Strength_3;
	else if( iSigValue >= 13 && iSigValue < 16 )
		signal = Strength_4;
	else if( iSigValue >= 16 && iSigValue <= 31 )
		signal = Strength_5;

        /*sprintf(sendBuffer,"AT+CCALR?\r\n");
	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);
	if(ret >= 0 && ((p=strstr(recvBuffer, "+CCALR: 1")) != NULL))
        {
                m_pCallback->toCSQSigAndSIM(signal, 1);
        }
        else
        {
                m_pCallback->toCSQSigAndSIM(signal, 0);
                return -1;
        }*/
        sprintf(sendBuffer,"AT+CSMINS?\r\n");
	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);
	if(ret >= 0 && ((p=strstr(recvBuffer, "+CSMINS:")) != NULL))
        {
		char cpTmp[300];
		char *cpSig =NULL;
		int csstate = 0;
        
		strcpy(cpTmp,p);
		p=cpTmp;

		if(strsep(&p, ":") != NULL)
		{
			if((cpSig = strsep(&p,",")) != NULL)
			{
				csstate = atoi(p);
			}
		}
                if(csstate == 0)
                {
                    qWarning("SimState getRSSI 0------------------------no sim");
                    simstate = 0;
                    m_pCallback->toCSQSigAndSIM(signal, 0);// no sim card
                    return -1;
                }
                else if(csstate == 1)
                {
                    simstate=1;
                }
        }
        else
        {
                //simstate = 0;
                //qWarning("SimState getRSSI 0------------------------2");
                //m_pCallback->toCSQSigAndSIM(signal, 0);
                return -1;
        }

        sprintf(sendBuffer,"AT+CREG?\r\n");
	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);
	if(ret >= 0 && ((p=strstr(recvBuffer, "+CREG:")) != NULL))
        {
		char cpTmp[300];
		char *cpSig =NULL;
		int csstate = 0;
        
		strcpy(cpTmp,p);
		p=cpTmp;

		if(strsep(&p, ":") != NULL)
		{
			if((cpSig = strsep(&p,",")) != NULL)
			{
				csstate = atoi(p);
			}
		}
                if(!(csstate == 1 ||csstate == 5))
                {
                    simstate = 1;
                    qWarning("SimState getRSSI 1------------------------not registered");
                    m_pCallback->toCSQSigAndSIM(signal, 1);//sim card ok.not registered
                    return -1;
                }
                else
                {
                    //simstate=2;
                    //qWarning("SimState getRSSI 2------------------------registered");
                    //m_pCallback->toCSQSigAndSIM(signal, 2);//sim card ok.registered
                    //return -1;
                }
        }
        else
        {
                simstate = 1;
                qWarning("SimState getRSSI 1------------------------2");
                m_pCallback->toCSQSigAndSIM(signal, 1);//sim card ok.not registered
                return -1;
        }

		
#endif
        iTmpSigValue = iSigValue;

	return iSigValue;
}

int PhoneMonitorVoiceInterface::getGprs()
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;
    int iGPRSStatus = 0;

    sprintf(sendBuffer,"AT+CGATT?\r\n");

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);
    if((ret >=0) && (!findIntegerInString(recvBuffer, "+CGATT:", "", 0, &iGPRSStatus)))
    {
		return iGPRSStatus;
    }

    return 0;
}

int PhoneMonitorVoiceInterface::getOperator()
{
        QMutexLocker locker(&m_mutex);

        char sendBuffer[200];
        char recvBuffer[256];

        int ret = 0;
        int iGPRSStatus = 0;

        sprintf(sendBuffer,"AT+COPS?\r\n");

        ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);
        if(ret >= 0)
        {
                if(strstr(recvBuffer, "UNICOM") != NULL)
                        m_pCallback->toOperator(0);
                else if(strstr(recvBuffer, "CHINA MOBILE") != NULL)
                        m_pCallback->toOperator(1);
                else if(strstr(recvBuffer, "TELECOM") != NULL)
                        m_pCallback->toOperator(2);
                else
                        m_pCallback->toOperator(3);
        }
        else
        {
                m_pCallback->toOperator(3);//connect ok
        }

        return 0;
}

int PhoneMonitorVoiceInterface::connectLink()
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIICR\r\n");

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,10000);

    if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
    {
        return 0;
    }

    return -1;
}

int PhoneMonitorVoiceInterface::getIpStatus()
{
	QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIPSTATUS\r\n");

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,5000);
    if(ret >= 0)
    {
		if(strstr(recvBuffer, "IP STATUS") != NULL)
			return 1;
		else if(strstr(recvBuffer, "IP PROCESSING") != NULL)
			return 2;
		else
			return 0;
    }

    return -1;
}
int PhoneMonitorVoiceInterface::getTcpStatus(int conNum)
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIPSTATUS=%d\r\n", conNum);

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,5000);
    if(ret >= 0 && (strstr(recvBuffer, "CONNECTED") != NULL))
    {
		return 1;
    }

    return 0;
}
int PhoneMonitorVoiceInterface::get_LocalIP()
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIFSR\r\n");
    
	m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,500);

	return 0;
}

int PhoneMonitorVoiceInterface::setPowerDown(int mode)
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    sprintf(sendBuffer,"AT+CPOWD=%d\r\n", mode);
	m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255, 1);

	return 0;
}

int PhoneMonitorVoiceInterface::setCfun(int flag, int rst, int timeout)
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CFUN=%d,%d\r\n", flag, rst);
    
	m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255, timeout);

	return 0;
}

int PhoneMonitorVoiceInterface::setAPN(int apn)
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    int ret = 0;
    char sendBuffer[200];
    char recvBuffer[256];
	char sapn[20];

	if(apn == APN_UNINET){
		strcpy(sapn, "UNINET");
	}else if(apn == APN_CTNET){
		strcpy(sapn, "CTNET");
	}else{
		strcpy(sapn, "CMNET");
	}
	
	sprintf(sendBuffer,"AT+CSTT=\"%s\"\r\n", sapn);

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,5000);
    if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
    {
		return 0;
    }

    return -1;
}

int PhoneMonitorVoiceInterface::connectTcp(int conNum, char *server, int port)
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIPSTART=%d,\"TCP\",\"%s\",\"%d\"\r\n", conNum, server, port);
    
	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,5*1000);
    if(ret >= 0 && (strstr(recvBuffer, "CONNECT OK") != NULL))
    {
		return 0;
    }

    return -1;
}

int PhoneMonitorVoiceInterface::setFastSend(int fast)
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIPQSEND=%d\r\n", fast);

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);

    if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
    {
        return 0;
    }

    return -1;
}

int PhoneMonitorVoiceInterface::getSendLength(int conNum)
{
    int send_length = 0;
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];
    char str[100];

    int ret = 0;

    sprintf(str,"+CIPSEND: %d,", conNum);
    sprintf(sendBuffer,"AT+CIPSEND?\r\n");

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);
	if((ret >=0) && strstr(recvBuffer, "OK") && (!findIntegerInString(recvBuffer, str, ",", 1, &send_length)))
	{
        return send_length;
    }
    return 0;
}

int PhoneMonitorVoiceInterface::closeTcp(int conNum)
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIPCLOSE=%d\r\n", conNum);

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,5000);

    if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
    {
        return 0;
    }

    return -1;
}

int PhoneMonitorVoiceInterface::closeLink()
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIPSHUT\r\n");

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,10*1000);

    if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
    {
        return 0;
    }

    return -1;
}

int PhoneMonitorVoiceInterface::setMux(int mux)
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIPMUX=%d\r\n", mux);

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);

    if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
    {
        return 0;
    }

    return -1;
}

int PhoneMonitorVoiceInterface::setTransMode(int trans)
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIPMODE=%d\r\n", trans);

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);

    if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
    {
        return 0;
    }

    return -1;
}

int PhoneMonitorVoiceInterface::setRxMode()
{
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[256];

    int ret = 0;

    sprintf(sendBuffer,"AT+CIPRXGET=1\r\n");

    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,255,2000);

    if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
    {
        return 0;
    }

    return -1;
}

int PhoneMonitorVoiceInterface::getRxData(int conNum, unsigned char *buffer, int len)
{
    char *p=NULL;
    int rx_length = 730;
	int conNumRx;
    QMutexLocker locker(&m_mutex);
    if(m_phoneStatus !=PHONE_STATUS_INITED)
        return 0;

    char sendBuffer[200];
    char recvBuffer[2048];

    int ret = 0;
    if(len < rx_length)
        rx_length = len;

    sprintf(sendBuffer,"AT+CIPRXGET=3,%d,%d\r\n", conNum, rx_length);
    ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,2048,2000);
    if((ret >=0) && (!findIntegerInString(recvBuffer, "+CIPRXGET: 3", ",", 2, &rx_length)))
    {

		if(!strstr(recvBuffer, "+CIPRXGET: 1") && !strstr(recvBuffer, "OK"))
			return 0;
		if(!findIntegerInString(recvBuffer, "+CIPRXGET: 1", ",",1 , &conNumRx)){
			m_ptcpThread->data_receive(conNumRx);
		}

    	int i;
        if(rx_length > len)
            return 0;
        p = strstr(recvBuffer, "+CIPRXGET: 3");
        while(p && *p && ((*p) != '\n'))
		    p++;
		p++;
        if(!p || !(*p))
			return 0;

        for(i = 0; i < rx_length; i ++, p+= 2)
        {
            if(!(*p) || !isBCDChar(*p))
                break;
            if(!(*(p+1)) || !isBCDChar(*(p+1)))
                break;
            buffer[i] = CharToBCD(*p) * 16 + CharToBCD(*(p+1));
        }
        if(i < rx_length)
            return 0;
        return rx_length;
    }
    return 0;
}

int PhoneMonitorVoiceInterface::getSignalStrength(SignalStrength &signal,int &signal_strength)
{
	int ipstate;
	int newsignum=getRSSI();
	if( newsignum < 0 )
		newsignum = 0;
	
	if( newsignum < 3 || newsignum > 31 )
		signal = Strength_0;
	else if( newsignum >= 3 && newsignum < 7 )
		signal = Strength_1;
	else if( newsignum >= 7 && newsignum < 10 )
		signal = Strength_2;
	else if( newsignum >= 10 && newsignum < 13 )
		signal = Strength_3;
	else if( newsignum >= 13 && newsignum < 16 )
		signal = Strength_4;
	else if( newsignum >= 16 && newsignum <= 31 )
		signal = Strength_5;
	signal_strength = newsignum;

	m_ptcpThread->reportTcpStates();
	ipstate =  getIpStatus();
	m_ptcpThread->ip_connected(ipstate);//connect to server


	return 0;
}

int PhoneMonitorVoiceInterface::queryCallState(void)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	//m_callStatus =CALL_IDLE;

	char sendBuffer[200];
    char recvBuffer[200];
    int ret = 0;

    sprintf(sendBuffer,"AT+CPAS\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL  ||  strstr(recvBuffer, "END") != NULL))
	{
		//return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}

	if(strstr(recvBuffer, "3") != NULL || strstr(recvBuffer, "4") != NULL )
	{
		return PHONE_STATUS_CALLING;
	}
	else
	{
	}

	return 0;

	//return m_callStatus;
}

signed char PhoneMonitorVoiceInterface::getNetworkType()
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return -1;

	char sendBuffer[200];
        char recvBuffer[512];
        int ret = 0;

        sprintf(sendBuffer,"AT+ZPAS?\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,500,1000);

	if(ret<0)
		return -1;
	
	if((strstr(recvBuffer, "GPRS") != NULL))
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

int PhoneMonitorVoiceInterface::getNetworkTypeAndServiceType(NetworkType &net, ServiceType &service)
{
	net = No_Network;
#ifndef _8925_8D1
	service = No_Service;
#endif
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return -1;

	char sendBuffer[200];
       char recvBuffer[512];
       int ret = 0;

#ifdef _8925_8D1
        sprintf(sendBuffer,"AT+CGATT?\r\n");
#else
        sprintf(sendBuffer,"AT+ZPAS?\r\n");
#endif

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,500,1000);

	if(ret<0)
		return -1;

#ifdef _8925_8D1
        if((strstr(recvBuffer, "+CGATT: 1") != NULL))
        {
                net = Network_2G;
                qWarning("SimState 3 getNetworkTypeAndServiceType------------------------connect ok ! %d",simstate);
                if(simstate >= 1 )
                {
                    simstate = 2;
                    m_pCallback->toCSQSigAndSIM(signal, 2);//connect ok
                }
        }
        else if((strstr(recvBuffer, "+CGATT: 0") != NULL))
        {
            qWarning("SimState 2 getNetworkTypeAndServiceType------------------------not connected ! %d",simstate);
                if(simstate >= 1 )
                {
                    simstate = 1;
                    m_pCallback->toCSQSigAndSIM(signal, 1);//registered,not conneted
                }
        }

	return 0;
#endif



	if((strstr(recvBuffer, "GPRS") != NULL))
	{
		net = Network_2G;
	}
	else if((strstr(recvBuffer, "EDGE") != NULL))
	{
		net = Network_2G;
	}
	else if((strstr(recvBuffer, "GSM") != NULL))
	{
		net = Network_2G;
	}
	else if((strstr(recvBuffer, "No Service") != NULL))
	{
		net = No_Network;
	}
	else if((strstr(recvBuffer, "Limited Service") != NULL))
	{
		net = Limited_Network;
	}
	else if((strstr(recvBuffer, "HSDPA") != NULL))
	{
		net = Network_3G;
	}
	else if((strstr(recvBuffer, "HSUPA") != NULL))
	{
		net = Network_3G;
	}
	else if((strstr(recvBuffer, "UMTS") != NULL))
	{
		net = Network_3G;
	}
	else
	{
		net = No_Network;
		IBD_PRINTF(DEBUG_EMERGENCY, "ZPAS error value [%s]\n", recvBuffer);
	}
	
	if((strstr(recvBuffer, "CS_ONLY") != NULL))
	{
		service = Phone_Only;

	}
	else if((strstr(recvBuffer, "PS_ONLY") != NULL))
	{
		service = Net_Only;
	}
	else if((strstr(recvBuffer, "CS_PS") != NULL))
	{
		service = Phone_And_Net;
	}
	else if((strstr(recvBuffer, "CAMPED") != NULL))
	{
		service = No_Service;
	}
	else
	{
		service = No_Service;
		IBD_PRINTF(DEBUG_EMERGENCY, "ZPAS error value [%s]\n", recvBuffer);
	}

	return 0;
}

int PhoneMonitorVoiceInterface::getSKVol()
{
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	return PHONE_MONITOR_FAIL;
}

int PhoneMonitorVoiceInterface::setSKVol(int val)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CLVL=%d\r\n", val);

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}


bool PhoneMonitorVoiceInterface::setMicVol(int val)
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED){
		return PHONE_MONITOR_FAIL;
	}
	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CMVL=%d\r\n", val);

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}


int PhoneMonitorVoiceInterface::muteMic()
{
	QMutexLocker locker(&m_mutex);

	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CMUT=1\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);
	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::unMuteMic()
{
	QMutexLocker locker(&m_mutex);
	if(m_phoneStatus !=PHONE_STATUS_INITED)
		return PHONE_MONITOR_FAIL;

	char sendBuffer[200];
        char recvBuffer[200];
        int ret = 0;

        sprintf(sendBuffer,"AT+CMUT=0\r\n");

	ret =m_pPhoneConnection->sendAtCommand(sendBuffer,recvBuffer,200,2000);

	if(ret >= 0 && (strstr(recvBuffer, "OK") != NULL))
	{
		return PHONE_MONITOR_SUCCESS;
	}
	else
	{
		return PHONE_MONITOR_FAIL;
	}
}

int PhoneMonitorVoiceInterface::getMicSatus()
{
	return PHONE_MONITOR_FAIL;
}

void PhoneMonitorVoiceInterface::run()
{
	char recvBuffer[1024];
	int conNum;

	while(1)
	{
        if(m_pPhoneConnection->getNextIndication(recvBuffer,1024)<0)
		{
			IBD_PRINTF(DEBUG_EMERGENCY,"voice phone device error.");
			sleep(1);
			continue;
		}

		if((!findIntegerInString(recvBuffer, "+CIPRXGET: ", ",",1 , &conNum)))//得到一条短消息
		{
			m_ptcpThread->data_receive(conNum);
		}

		//if(strstr(recvBuffer, "CLOSED") != NULL)//TCP CLOSED
		//{
		//	m_ptcpThread->tcp_connected(0);
		//}

        //从收到+CMTI解析短信条数ID，否则从recvbuffer解析\", 中间会夹着其他查询指令返回信息
        //[zhenyubin 2015/09/06]
        char *cmtiStr=strstr(recvBuffer, "+CMTI");
        if(cmtiStr != NULL)//得到一条短消息
		{
			char* tmp;
			int iSmsId;

            if((tmp = strstr(cmtiStr, "\",")) == NULL)//未找到","
			{
				IBD_PRINTF(DEBUG_INFOMATION, "not found '\",' error...\n")
				continue;	
			}	
			else
			{
                if((iSmsId = atoi(tmp+2)) < 0)//解析出短消息的ID
				{
					IBD_PRINTF(DEBUG_INFOMATION, "Get Sms Id error...\n")
					continue;
				}
			}
			m_pWorkThread->appendRecvMsg(iSmsId);//将短消息ID放入队列，等待处理		
		}
		if(strstr(recvBuffer, "CLIP"))
		{
			if(strstr(recvBuffer, "SYNC"))
			{
				char* cpTmp=recvBuffer;

                                if(strsep(&cpTmp, "\"") != NULL)
                                {
                                        char *cpPhoneNumber;
                                        if((cpPhoneNumber = strsep(&cpTmp, "\"")) != NULL)
                                        {
						strncpy(m_phoneNumber,cpPhoneNumber,30);
                                                m_phoneNumber[29]=0;

						m_videoPhone->waitIncommingCall();
                                        }
                                }

			}
			else
			{
				char* cpTmp =recvBuffer;

				if(strsep(&cpTmp, "\"") != NULL)
				{
					char *number=strsep(&cpTmp, "\"");
					if(number != NULL)
					{
						QMutexLocker locker(&m_mutex);
						if(m_callStatus!=CALL_COMING_CALL)
						{
							m_callStatus =CALL_COMING_CALL;

							strncpy(m_phoneNumber,number,30);
							m_phoneNumber[30-1] =0;
							m_pCallback->toNewCall(m_phoneNumber);
						}
					}
				}
			}
		}
#ifdef _8925_8D1
		else if(strstr(recvBuffer, "NO CARRIER") != NULL)
#else
		else if(strstr(recvBuffer, "VOICE NO CARRIER") != NULL)
#endif
		{
#ifndef _8925_8D1
                        //without codes below,when hung up,over_init()(exists in PHONE-UI-CODES) will run twice
                        if(strstr(recvBuffer, ":") == NULL)//for bug : run APP "ppp-on", calling ,the opposition hung up,run AT+CPAS,will get result: 4(state:talking)
                        {                                                                       //VOICE NO CARRIER 
                                                                                                //                      : hung up by self
                                m_pCallback->toCallHangUpBySelf();
                                continue;                                               //VOICE NO CARRIER : ERROR-CODE
                        }                                                                       //                      : hung up because of errors
                        else
                        {   
                        }
#endif
            //关闭声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(false);
			QMutexLocker locker(&m_mutex);
			m_pCallback->toCallHangUp(PHONE_MONITOR_TIMEOUT_HANG);
			printf("*** m_videoPhone->hangUp ***\n");
			m_videoPhone->hangUp();
            m_callStatus =CALL_IDLE;
		}
		else if(strstr(recvBuffer, "VOICE CALL: END") != NULL)  
		{
            //关闭声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(false);
			QMutexLocker locker(&m_mutex);
			m_pCallback->toCallHangUp(PHONE_MONITOR_OPPOSITE_HANG);
            m_callStatus =CALL_IDLE;
		}
		else if(strstr(recvBuffer, "HANGUP") != NULL) ////靠靠靠靠靠靠靠靠靠HANGUP
		{	
            //关闭声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(false);
			QMutexLocker locker(&m_mutex);
			m_pCallback->toCallHangUp(PHONE_MONITOR_OPPOSITE_HANG);
            m_callStatus =CALL_IDLE;
		}
        else if(strstr(recvBuffer, "NO ANSWER") != NULL) //移动卡一直不接挂断时会报这个 [zhenyubin 2015/07/13]
        {
            //关闭声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(false);
            QMutexLocker locker(&m_mutex);
            m_pCallback->toCallHangUp(PHONE_MONITOR_OPPOSITE_HANG);
            m_callStatus =CALL_IDLE;
        }
        else if(strstr(recvBuffer, "BUSY") != NULL) //移动卡挂断时会报这个 [zhenyubin 2015/07/13]
        {
            //关闭声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(false);
            QMutexLocker locker(&m_mutex);
            m_pCallback->toCallHangUp(PHONE_MONITOR_OPPOSITE_HANG);
            m_callStatus =CALL_IDLE;
        }
		
		else if(strstr(recvBuffer, "CSDSTOPRING") != NULL)
		{
			continue;
		}
		else if(strstr(recvBuffer, "STOPRING") != NULL)
		{
            //关闭声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(false);
			QMutexLocker locker(&(m_mutex));
			m_pCallback->toMissCall(m_phoneNumber);
			m_phoneNumber[0]=0;
            m_callStatus= CALL_IDLE;
		}
		else if(strstr(recvBuffer, "MISSED") != NULL)
		{
            //关闭声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(false);
			QMutexLocker locker(&(m_mutex));
			m_pCallback->toMissCall(m_phoneNumber);
			m_phoneNumber[0]=0;
            m_callStatus= CALL_IDLE;
		}
		else if(strstr(recvBuffer, "NO DIALTONE") != NULL)
		{
            //关闭声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(false);
			QMutexLocker locker(&m_mutex);
			m_pCallback->toCallHangUp(PHONE_MONITOR_TIMEOUT_HANG);
            m_callStatus =CALL_IDLE;
		}
		else if(strstr(recvBuffer, "RINGBACK") != NULL)
		{
			char *p=strstr(recvBuffer, "RINGBACK");

			if(strncmp(p,"RINGBACKLOCAL",strlen("RINGBACKLOCAL"))!=0)
			{
                //打开声音，防止延迟 [zhenyubin 2015/08/13]
                m_pCallback->closeOrOpenPhoneAudio(true);
				QMutexLocker locker(&m_mutex);
				m_pCallback->toRing(m_phoneNumber);
			}
			continue;
		}
#ifdef _8925_8D1
                else if(strstr(recvBuffer, "MO RING") != NULL)
                {
                        //打开声音，防止延迟 [zhenyubin 2015/08/13]
                        m_pCallback->closeOrOpenPhoneAudio(true);
                        QMutexLocker locker(&m_mutex);
                        m_pCallback->toRing(m_phoneNumber);
                }
                else if(strstr(recvBuffer, "MO CONNECTED") != NULL)
                {
            //打开声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(true);
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_TALKING;
			m_pCallback->toCallConnect();
                }
#endif
		else if(strstr(recvBuffer, "BEGIN") != NULL)
		{
            //打开声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(true);
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_TALKING;
			m_pCallback->toCallConnect();
		}
#ifndef _8925_8D1
		else if(strstr(recvBuffer, "CONNECT") != NULL)
		{
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_TALKING;
			m_pCallback->toCallConnect();
		}
#endif
		else if(strstr(recvBuffer, "ANSWER") != NULL)
		{
            //打开声音，防止延迟 [zhenyubin 2015/08/13]
            m_pCallback->closeOrOpenPhoneAudio(true);
			QMutexLocker locker(&(m_mutex));
			m_callStatus= CALL_TALKING;
			m_pCallback->toCallConnect();
		}
		else if(strstr(recvBuffer, "+ZEND") != NULL)//出现此字段说明模块不能再使用，原因目前不清楚，ZTE怀疑是SIM卡报出的错误
		{
		}
	}
}

