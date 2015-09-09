#include "JT808MessageHelper.h"
#include "ConfigManager.h"
#include "LogUtility.h"
#include "ConnectionManager.h"
#include "VDRDeployParaTable.h"
#include "Utility.h"
#include <dbusServer.h>
#include <dbusApplication.h>
#include "JT808DbusMessageHandler.h"
#include "NaviDbusMessage.h"
#include <time.h>
#include "McuMessageHandler.h"
#include "GeneralTimer.h"
#include "JT8082DbusMessage.h"
#include <sys/time.h> 
#include "VDRBaseConfigTable.h"

#define DeadTime 3600
extern int shutdown_flag;
int TimeToDev=0;
int NightOrDay;
extern DBusServerC    *g_pDbusServer;
extern int sendhello;
extern int sendloc;
extern unsigned short heart_retry_time;//心跳间隔
extern unsigned short loc_retry_time;//位置信息间隔
extern bool stop_m_helloTimer;
extern unsigned int AccStatus;
extern GeneralTimerItem *systemoff;
extern McuMesageHandler mcumessagehandle;
extern unsigned short seq_last;
unsigned short tmp_seq=0;
extern unsigned char  connect_tmp;
extern int DeadBlock_flag;
unsigned char	connect_Id;

static void JT808TimerHandler(string &name, void *data, void *data2)
{
    JT808MessageHandler *handler = (JT808MessageHandler *)data;
    if(handler != NULL)
    {
        handler->HandleTimer(name, data2);
    }
}

JT808MessageHandler::JT808MessageHandler()
{
    printf("JT808MessageHandler::JT808MessageHandler!\n" );
    m_helloTimer = NULL;
    m_queueTimer = NULL;
    m_authfailure = 0;
    m_retryTimer = NULL;
    m_retryRegister = NULL;
    auth_failure = 0;
    m_Timer_Deadzone = NULL;
    RefreshConfig();
    GetPhoneNumber();
    m_waitingQueue.set_capacity(JT808PacketFifo::capacity_type(JT808_MAX_WAITING_QUEUE));
    m_waitingQueue_reply.set_capacity(JT808PacketFifo::capacity_type(JT808_MAX_WAITING_QUEUE));
    m_waitingQueue_ack.set_capacity(JT808PacketFifo::capacity_type(JT808_MAX_WAITING_QUEUE));
    m_rxPacket.set_capacity(JT808Packet::capacity_type(JT808_MAX_PACKET_SIZE, JT808_MIN_PACKET_SIZE));
    ClearRxPacket();
    m_seq = 0;
    ConnectionManager::Instance()->AddEventHandler(this);
    ConnectionManager::Instance()->SetPacketHandler(this);
    
    if(pthread_mutex_init(&m_queueMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler: mutex init failed.");
        return;
    }
    if(pthread_mutex_init(&m_packetMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler: mutex init failed.");
        return;
    }
    
    m_registered = false;
    m_authorized = false;
    s_multipacket= false;
     reply_flag=0;
	//VDRDeployParaTable *table = new VDRDeployParaTable();
    VDRBaseConfigTable * table = new VDRBaseConfigTable();
    string authToken = "";
    if(table->GetStringValue((int)VDR_BASE_STATUS_D7, authToken) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler get authToken failed");
    }
    
    if(authToken.length() > 0)
    {
        m_registered = true;
    }

    delete table;
    
    
    JT808MessageHelper::Init(this);
}

JT808MessageHandler::~JT808MessageHandler()
{
    DestroyTimer(m_helloTimer);
    DestroyTimer(m_queueTimer);
    DestroyTimer(m_retryTimer);
    DestroyTimer(m_retryRegister);
    DestroyTimer(m_retryTimer_location);
    DestroyTimer(m_Timer_Deadzone);
    ClearQueue();
    ClearRxPacket();
    pthread_mutex_destroy(&m_queueMutex);
    pthread_mutex_destroy(&m_packetMutex);
}

void JT808MessageHandler::ClearQueue()
{
    pthread_mutex_lock(&m_queueMutex);
#if 0
	    for(JT808PacketFifo::iterator it = m_waitingQueue_reply.begin(); it != m_waitingQueue_reply.end(); it ++)
    {
        StruJT808Packet *packet = *it;
        DestroyPacket(packet);
    }
    m_waitingQueue_reply.clear();
#endif 
    for(JT808PacketFifo::iterator it = m_waitingQueue.begin(); it != m_waitingQueue.end(); it ++)
    {
        StruJT808Packet *packet = *it;
        DestroyPacket(packet);
    }
    m_waitingQueue.clear();
  
    pthread_mutex_unlock(&m_queueMutex);
}

int JT808MessageHandler::HandleHelloTimer()
{
	printf("=============start the hellotimer============\n");
	StartTimer(m_helloTimer);
}

void JT808MessageHandler::RefreshConfig()
{
    ConfigManager::Refresh();
    m_helloTimeout = ConfigManager::GetHeartBeatInterval();
    m_retryTimeout = ConfigManager::GetTcpRetryTimeout();
    m_retryTimes = ConfigManager::GetTcpRetryTimes();
    LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::RefreshConfig Hello Timeout %d", m_helloTimeout);
    LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::RefreshConfig Retry Timeout %d", m_retryTimeout);
    LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::RefreshConfig Retry Times %d", m_retryTimes);
	m_DeadTimeout = DeadTime;
    if(m_helloTimer)
    {
        m_helloTimer->timeout = TIMER_SEC_TO_MSEC(m_helloTimeout);
    }
    else
    {
        m_helloTimer = CreateTimer(JT808_HELLO_TIMER_NAME, TIMER_SEC_TO_MSEC(m_helloTimeout), this, JT808TimerHandler, true);
    }
    if(m_queueTimer)
    {
        m_queueTimer->timeout = TIMER_SEC_TO_MSEC(JT808_QUEUE_TIME_OUT);
    }
    else
    {
        m_queueTimer = CreateTimer(JT808_QUEUE_TIMER_NAME, TIMER_SEC_TO_MSEC(JT808_QUEUE_TIME_OUT), this, JT808TimerHandler, false);
    }
    if(m_retryTimer)
    {
        m_retryTimer->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
    }
    else
    {
        m_retryTimer = CreateTimer(JT808_RETRY_TIMER_NAME, TIMER_SEC_TO_MSEC(m_retryTimeout), this, JT808TimerHandler, false);
    }
    if(m_retryRegister)
    {
        m_retryRegister->timeout = TIMER_SEC_TO_MSEC(JT808_RETRY_REGISTER_TIME);
    }
    else
    {
        m_retryRegister= CreateTimer(JT808_RETRY_REGISTER_NAME, TIMER_SEC_TO_MSEC(JT808_RETRY_REGISTER_TIME), this, JT808TimerHandler, false);
    }
    if(m_retryTimer_location)
    {
        m_retryTimer_location->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
    }
    else
    {
        m_retryTimer_location = CreateTimer(JT808_RETRY_TIMER_NAME_LOCATION, TIMER_SEC_TO_MSEC(m_retryTimeout), this, JT808TimerHandler, false);
    }
    if(m_Timer_Deadzone)
    {
        m_Timer_Deadzone->timeout = TIMER_SEC_TO_MSEC(m_DeadTimeout );
    }
    else
    {
        m_Timer_Deadzone = CreateTimer(JT808_DEAD_ZONE_TIMER, TIMER_SEC_TO_MSEC(m_DeadTimeout ), this, JT808TimerHandler, false);
    }
}
void JT808MessageHandler::ResetFactory()
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ResetFactory()!");
    m_registered = false;
	VDRDeployParaTable *table1 = new VDRDeployParaTable();
	string auth="";
	if(table1->SetIntValue(VDR_DEPLOY_PARA_DEPLOY_MILEAGE,0)<0)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG,"JT808MessageHandler::ResetFactory set mileage faild.");
		delete table1;
		return ;
	}
	if(table1->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::ResetFactory set auth faild.");
		delete table1;
		return;
	}
	table1->Commit();
	delete table1;

	m_helloTimeout = ConfigManager::GetHeartBeatInterval();
	m_retryTimeout = ConfigManager::GetTcpRetryTimeout();
	m_retryTimes = ConfigManager::GetTcpRetryTimes();
	LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::RefreshConfig Hello Timeout %d", m_helloTimeout);
	LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::RefreshConfig Retry Timeout %d", m_retryTimeout);
	LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::RefreshConfig Retry Times %d", m_retryTimes);
	
	StopTimer(m_helloTimer);
	StopTimer(m_queueTimer);
	//StopTimer(m_retryTimer);
	StopTimer(m_retryRegister);
 	StopTimer(m_retryTimer_location); 
	
	m_helloTimer->timeout=TIMER_SEC_TO_MSEC(m_helloTimeout);
	m_queueTimer->timeout=TIMER_SEC_TO_MSEC(JT808_QUEUE_TIME_OUT);
	m_retryTimer->timeout=TIMER_SEC_TO_MSEC(m_retryTimeout);
	m_retryRegister->timeout=TIMER_SEC_TO_MSEC(JT808_RETRY_REGISTER_TIME);
	m_retryTimer_location->timeout=TIMER_SEC_TO_MSEC(m_retryTimeout);
	
	StartTimer(m_helloTimer);
	StartTimer(m_queueTimer);
	//StartTimer(m_retryTimer);
	StartTimer(m_retryRegister);
	StartTimer(m_retryTimer_location);
	ConnectionManager::Instance()->Notify(ENUM_CONNECTION_STATE_DISCONNECTED);	
	ConnectionManager::Instance()->Reconnect();
}
void JT808MessageHandler::GetPhoneNumber()
{
    string phoneNumber;
    VDRDeployParaTable *table = new VDRDeployParaTable();
    int arraylen = sizeof(m_phoneNumber);
    int maxlen = arraylen * 2;
   /* 
    for(int i = 0; i < (int)sizeof(m_phoneNumber); i ++)
    {
        m_phoneNumber[i] = 0x00;
    }
   */
    if(table->GetStringValue((int)VDR_DEPLOY_PARA_PHONE_NUMBER, phoneNumber) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::GetPhoneNumber failed");
        delete table;
        return;
    }
    
    LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::GetPhoneNumber %s", phoneNumber.c_str());
    
    delete table;
    
    int len = phoneNumber.length();
    
    int offset = maxlen - len;
    if(offset < 0)
    {
        offset = 0;
    }
    
    for(int i = 0; i < maxlen; i ++)
    {
        unsigned char bcd1;
        unsigned char bcd2;
        if(offset > i)
        {
            bcd1 = 0x0;
        }
        else
        {
            bcd1 = CharToBCD(phoneNumber[i - offset]);
        }
        i ++;
        if((maxlen - i) > len)
        {
            bcd2 = 0x0;
        }
        else
        {
            bcd2 = CharToBCD(phoneNumber[i - offset]);
        }
        m_phoneNumber[i/2] = (unsigned char)((bcd1 << 4) |(bcd2));
    }

    return;
}

int JT808MessageHandler::ConnectionEventHandler(ENUM_CONNECTION_EVENT state, ENUM_CONNECTION_ID conn)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ConnectionEventHandler state %x, conn %x.", (int)state, (int)conn);
	VDRBaseConfigTable * table = new VDRBaseConfigTable();
    string authToken = "";
    if(table->GetStringValue((int)VDR_BASE_STATUS_D7, authToken) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler get authToken failed");
    }
    
    if(authToken.length() > 0)
    {
        m_registered = true;
    }else
    {
	    m_registered = false;

	}
	if(state == ENUM_CONNECTION_STATE_CONNECTED)
    {
		printf("if(state == ENUM_CONNECTION_STATE_CONNECTED)  \n");
		if(conn == ENUM_CONNECTION_ID_MASTER)
        {
			connect_Id=ENUM_CONNECTION_ID_MASTER;
			//SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_CONNECTED, (int)CONN_STATUS_NTF_DISCONNECT);
        }
        else
        {
            //SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_CONNECTED);
	}

	if((conn == ENUM_CONNECTION_ID_MASTER) || (conn == ENUM_CONNECTION_ID_SECONDARY))
	{
		printf("(conn == ENUM_CONNECTION_ID_MASTER) || (conn == ENUM_CONNECTION_ID_SECONDARY) \n");
		if(ConfigManager::m_control == 0)
		{
		    
			printf("ConfigManager::m_control************************************************** == 0\n");
			JT808MessageHelper::SendAuth();
		}
		else if(m_registered)
		{
			if(ConfigManager::GetAuthFlag())
			{
				printf("ConfigManager::GetAuthFlag()**********************************************8 0\n");
				JT808MessageHelper::SendAuth();
                
			}
		}
		else
		{
			printf("fei -------------------------- SendRegister\n");
			JT808MessageHelper::SendRegister(1, NULL);
		}
	}
    }
    if(state == ENUM_CONNECTION_STATE_CONNECTTING)
    {
        if(conn == ENUM_CONNECTION_ID_MASTER)
        {
            //SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_CONNECTING, (int)CONN_STATUS_NTF_DISCONNECT);
        }
        else
        {
           //SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_CONNECTING);
        }
    }
    else if(state == ENUM_CONNECTION_STATE_DISCONNECTED)
    {
        SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_DISCONNECT);
        StopTimer(m_helloTimer);
        StopTimer(m_queueTimer);
        StopTimer(m_retryTimer);
        StopTimer(m_retryTimer_location);
        ClearQueue();
        ClearRxPacket();
        m_authorized = false;
        s_multipacket= false;
    }
    
    return 0;
}

int JT808MessageHandler::PacketHandler(unsigned char *buf, int len)
{
    printf( "xxh--------JT808MessageHandler::PacketHandler buf len = %d\n",len);
	for(int i=0;i<=len;i++)
	{
		printf("%02x ",buf[i]);
		if((i+1)%15==0)
			printf("\n");
	}
   for(int i = 0; i < len; i ++)
    {
        unsigned char ch = buf[i];
		
        if(m_rxPacket.size() >= JT808_MAX_PACKET_SIZE)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PacketHandler buffer overflow.");
            ClearRxPacket();
            continue;
        }

        if(ch == JT808_PACKET_FLAG)
        {
            if(m_packet_start)
            {
                m_rxPacket.push_back(ch);
                if(m_rxPacket.size() < JT808_MIN_PACKET_SIZE)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PacketHandler too short.");
                    ClearRxPacket();
                    continue;
                }
                HandlePacket();
                ClearRxPacket();
            }
            else
            {
                m_rxPacket.push_back(ch);
                m_packet_start = true;
                m_packet_escape = false;
            }
            continue;
        }
        
        if(!m_packet_start)
        {
            continue;
        }
        
        if(ch == JT808_PACKET_ESCAPE)
        {
            if(m_packet_escape)
            {
                m_rxPacket.push_back(JT808_PACKET_ESCAPE);
            }
            m_packet_escape = true;
            continue;
        }
        
        if(m_packet_escape)
        {
            if(ch == 0x01)
            {
                m_rxPacket.push_back(JT808_PACKET_ESCAPE);
            }
            else if(ch == 0x02)
            {
                m_rxPacket.push_back(JT808_PACKET_FLAG);
            }
            else
            {
                m_rxPacket.push_back(JT808_PACKET_ESCAPE);
                m_rxPacket.push_back(ch);
            }
            m_packet_escape = false;
            continue;
        }
        
        m_rxPacket.push_back(ch);
    }
    
    return 0;
}

void JT808MessageHandler::HandlePacket()
{
	printf( "xie-------------------------------------JT808MessageHandler::HandlePacket()");
	unsigned char phonenum_tmp[6];
	int count;
	printf("\n");
	for ( count=0;count<6;count++)
	{
		phonenum_tmp[count]=(char)m_rxPacket[5+count];
	}
	printf("\n");
	for ( count=0;count<6;count++)
	{
		if(m_phoneNumber[count] ==phonenum_tmp[count])
		{	
			printf("%02x ",m_phoneNumber[count]);
		}
		else 
		{
			printf("%02x ",m_phoneNumber[count]);
			printf("The PhoneNumgber  Is  Wrong  \n");
			return;
		}
	
	}
	

    
    unsigned short id = JT808_GET_SHORT(m_rxPacket, 1);
    unsigned short attr = JT808_GET_SHORT(m_rxPacket, 3);
    unsigned short seq = JT808_GET_SHORT(m_rxPacket, 11);
    unsigned short packetNum;
    unsigned short sum;
    
    int len = attr & 0x3FF;
    bool segment = (attr >> 13) & 0x1;
    int crypto = (attr >> 10) & 0x7;
    int body;
    unsigned char checksum = 0;
    int packetsize = m_rxPacket.size();
    
    if(segment)
    {
        body = 17;

	sum = JT808_GET_SHORT(m_rxPacket,13);
	packetNum = JT808_GET_SHORT(m_rxPacket,15);
    }
    else
    {
        body = 13;
    }
    if(crypto != JT808_CRYTO_NONE)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::HandlePacket crypto not support(%d).", crypto);
        return;
    }
    
    if((body==13) && ((packetsize - 13 - 2) != len))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::HandlePacket length not matching(%d, %d).", packetsize, len);
        return;
    }
    if((body==17) && ((packetsize - 17 -2) != len))
    {
    	LogUtility::Log(LOG_LEVEL_ERROR,"JT808MessageHandler::HandlePacket length not matching(%d, %d).", packetsize,len);
	return;
    }
    for(int i = 1; i < packetsize - 2; i ++)
    {
        checksum ^= m_rxPacket[i];
    }
    
    if(checksum != m_rxPacket[packetsize - 2])
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::HandlePacket checksum not matching(%02x, %02x).", checksum, m_rxPacket[packetsize - 2]);
        return;
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "xxh---JT808MessageHandler::HandlePacket check pass(%04x, %d).", id, len);

    switch(id)
    {
        case JT808_PACKET_PLATFORM_COMMON_REP:
            ProcessCommonReply(body, len);
            break;
        case JT808_PACKET_ID_REGISTER_REP:
            ProcessRegisterReply(body, len);
            break;
        case JT808_PACKET_RESEND_REQUEST:
          // JT808MessageHelper ::HandlePacketResendRequest(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_PARAMETERS:
            JT808MessageHelper::HandleSetParameters(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_QUERY_PARAMETERS:
            //JT808MessageHelper::SendAck(seq,id,ENUM_JT808_COMMON_OK);
            JT808MessageHelper::SendParameters(seq);
            break;
        case JT808_PACKET_QUERY_LIST_PARAMETERS:
	    JT808MessageHelper::HandleRequryTermParameters(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_TERM_CONTROL:
            JT808MessageHelper::HandleTermControl(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_QUERY_ATTRIBUTE:
            //JT808MessageHelper::SendAck(seq,id,ENUM_JT808_COMMON_OK);
            JT808MessageHelper::SendAttribute(seq);
            break;
        case JT808_PACKET_UPDATE_TERM:
           // JT808MessageHelper::HandleTermUpdate(m_rxPacket, body, len, seq, sum, packetNum);
            break;
        case JT808_PACKET_LOC_QUERY:
            //JT808MessageHelper::SendAck(seq,id,ENUM_JT808_COMMON_OK);
            JT808MessageHelper::SendLocationQueryResp(seq);
            break;
        case JT808_PACKET_TEMP_LOC_CONTROL:
            JT808MessageHelper::HandleTempLocControl(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MANUAL_CONFIRM:
            JT808MessageHelper::HandleAlarmConfirm(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_TEXT_MESSAGE:
            JT808MessageHelper::HandleTextMessage(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_EVENT:
            JT808MessageHelper::HandleEvent(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_QUESTION:
            JT808MessageHelper::HandleQuestion(m_rxPacket, body, len, seq);
            break;
	case JT808_PACKET_INFOMATION_SET:
	    JT808MessageHelper::HandleInfomationSet(m_rxPacket, body, len, seq);
	    break;
	case JT808_PACKET_INFOMATION_SERVICE:
	    JT808MessageHelper::HandleInfomationService(m_rxPacket, body, len, seq);
	    break;
        case JT808_PACKET_CALL_BACK:
	    JT808MessageHelper::SendAck(seq, (unsigned short)JT808_PACKET_CALL_BACK, (int)ENUM_JT808_COMMON_OK);
            JT808MessageHelper::HandleCallBack(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_PHONE_BOOK:
            JT808MessageHelper::HandlePhoneBook(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_CAR_CONTROL:
            JT808MessageHelper::HandleCarControl(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_CIRCLE:
            JT808MessageHelper::HandleSetCircle(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_DELETE_CIRCLE:
            JT808MessageHelper::HandleDeleteCircle(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_RECTANGLE:
            JT808MessageHelper::HandleSetRectangle(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_DELETE_RECTANGLE:
            JT808MessageHelper::HandleDeleteRectangle(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_POLYGON:
            JT808MessageHelper::HandleSetPolygon(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_DELETE_POLYGON:
            JT808MessageHelper::HandleDeletePolygon(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_SET_LINE:
            JT808MessageHelper::HandleSetLine(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_DELETE_LINE:
            JT808MessageHelper::HandleDeleteLine(m_rxPacket, body, len, seq);
            break;
	case JT808_PACKET_DATA_ACQ:			    
            JT808MessageHelper::HandleDataAcq(m_rxPacket, body, len, seq);
            break;
	case JT808_PACKET_PARA_SET:			    
            JT808MessageHelper::HandleParaSet(m_rxPacket, body, len, seq);
	    break;
	case JT808_PACKET_QUERY_DRIVER_INFO:              
            //JT808MessageHelper::SendAck(seq,id,ENUM_JT808_COMMON_OK);
           // JT808MessageHelper::SendDriverInfo(0);
	    break;
	case JT808_PACKET_CAMERA_START:
           // JT808MessageHelper::HandleCameraStart(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_RECORD_START:
          //  JT808MessageHelper::HandleRecordStart(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_INDEX_QUERY:
           // JT808MessageHelper::HandleQueryMediaIndex(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_SINGLE_INDEX_QUERY:
            //JT808MessageHelper::HandleQueryMediaSingleIndex(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_DATA_RSP:
            //JT808MessageHelper::HandlePacketMediaDataRsp(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_UPLOAD:
            //JT808MessageHelper::HandlePacketMediaUpload(m_rxPacket, body, len, seq);
	    break;
	case JT808_PACKET_DOWN_TRANSPARENT_TRANSMISSION:  
            JT808MessageHelper::HandlePacketDownTransparentTransmission(m_rxPacket, body, len, seq);
	    break;
        default:
            LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandlePacket unknown message.");
            break;
    }
}

void JT808MessageHandler::ProcessCommonReply(int start, int len)
{
    if(len != 5)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::ProcessCommonReply len invalid.");
        return;
    }
    
    int seq = JT808_GET_SHORT(m_rxPacket, start);
    int id = JT808_GET_SHORT(m_rxPacket, start+2);
    int result = JT808_GET_BYTE(m_rxPacket, start+4);
    if(id == JT808_PACKET_ID_AUTH){
	    if(result == 0)
	    {
		    auth_failure = 0;
		    m_authfailure = 1;
		    reply_flag=0;
    		PopQueue(id, seq);
			StopTimer(m_retryTimer);
		    StopTimer(m_retryRegister);
		    printf("if(state == ENUM_CONNECTION_STATE_CONNECTED)  \n");

		if(connect_Id== ENUM_CONNECTION_ID_MASTER)
	        {
	        	
	            		SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_CONNECTED, (int)CONN_STATUS_NTF_DISCONNECT);
				printf("SendJT808ConncetionStatusNotify 1111111111111111111\n");
		}
	        else
	        {
	            		SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_CONNECTED);
				printf("SendJT808ConncetionStatusNotify 122222222222222222222222\n");
		}
		
		
	    }
	    else
	    {
		    auth_failure++;
		    if(auth_failure >= 3)
		    {
			    PopQueue(id, seq);

       			   
			    string auth="";
			    VDRDeployParaTable *table = new VDRDeployParaTable();
			    if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
			    {
				    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
				    delete table;
				    return;
			    }
			    table->Commit();
			    delete table;

			    auth_failure = 0;
			    m_authfailure = 0;
			    m_registered = 0;
			    m_authorized = 0;
			    JT808MessageHelper::SendRegister(1, NULL);
			    return;
		    }
	    }
    	    //if(m_seq>1)StartTimer(m_queueTimer);
    }
    else
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ProcessCommonReply %d, %04x, %d.", seq, id, result);
   
	    PopQueue(id, seq);
    
    //if(id==JT808_PACKET_ID_AUTH && m_seq>1)StartTimer(m_queueTimer);
    JT808MessageHelper::HandleCommonReply(id, result);
}

void JT808MessageHandler::ProcessRegisterReply(int start, int len)
{
    string auth = "";
    if(len < 3)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::ProcessRegisterReply len invalid.");
        return;
    }
    
    int seq = JT808_GET_SHORT(m_rxPacket, start);
    int result = JT808_GET_BYTE(m_rxPacket, start+2);

    for(int i = 3; i < len; i ++)
    {
        auth.push_back(m_rxPacket[i + start]);
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ProcessRegisterReply %d, %d, %s.", seq, result, auth.c_str());
    
    if(result == 0){
    PopQueue(JT808_PACKET_ID_REGISTER, seq);
	    ConnectionManager::Instance()->errornum_register = 0; 
    }else if(ConnectionManager::Instance()->errornum_register++ >=2) 
    {
	PopQueue(JT808_PACKET_ID_REGISTER, seq);
	StopTimer(m_retryRegister);
	ConnectionManager::Instance()->Reconnect();
    }

    JT808MessageHelper::HandleRegisterResult(result, auth);
}

void JT808MessageHandler::PopQueue(int id, int seq)
{
	printf("---------------------PopQueue  seq = %x\n", seq);
	pthread_mutex_lock(&m_queueMutex);
	printf("---------------------PopQueue  id= %d seq = %x\n",id, seq);
	StruJT808Packet *packet;
#if 0

    if((id == JT808_PACKET_LOC_REPORT) || (id == JT808_PACKET_LOC_MULTI_REPORT)){
    //if(id == JT808_PACKET_LOC_MULTI_REPORT){
	    if(m_waitingQueue_location.size() <= 0)
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue location waiting queue empty.");
		    pthread_mutex_unlock(&m_queueMutex);
		    return;
	    }

	    StruJT808Packet *packet = *(m_waitingQueue_location.begin());
	    if((packet->id == id) && (packet->seq == seq))
	    {
		    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PopQueue location correct resp.");
		    m_waitingQueue_location.pop_front();
		    DestroyPacket(packet);
		    StopTimer(m_retryTimer_location);
		    if(m_waitingQueue_location.size() > 0)
		    {
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PopQueue location start queue timer");
			    StartTimer(m_queueTimer);
		    }
	    }
	    else
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue location incorrect resp(%d, %04x).", packet->seq, packet->id);
	    }
#endif	
		if (m_waitingQueue_reply.size()>=1)
	 	{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PopQueue m_waitingQueue_reply reply_flag =%d .",reply_flag);
			packet = *(m_waitingQueue_reply.begin());
			printf("PopQueue::m_waitingQueue_reply.size()==%d \n",m_waitingQueue_reply.size());
		       LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler:: pop m_waitingQueue_reply retry %d  resp(%d, %04x).", packet->retry,packet->seq, packet->id);
		    if((packet->retry == 0) && (packet->id == id) && (packet->seq == seq)&&reply_flag==1)
		    {
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::pop m_waitingQueue_reply correct resp.");
				m_waitingQueue_reply.pop_front();
				reply_flag=0;
				StartTimer(m_queueTimer);
			    DestroyPacket(packet);
				pthread_mutex_unlock(&m_queueMutex);
				return;
		    }
		    else
		    {
			    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler:: pop m_waitingQueue_Blindarea incorrect resp(%d, %04x).", packet->seq, packet->id);
		    }
			
		}
	    if(m_waitingQueue.size() <= 0)
	    {
		    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue waiting queue empty.");
		    pthread_mutex_unlock(&m_queueMutex);
		    return;
	    }

	    packet = *(m_waitingQueue.begin());
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::incorrect resp(%d, %d, %04x).", packet->retry, packet->seq, packet->id);
	    if((packet->retry >= 1) && (packet->id == id) && (packet->seq == seq))
	    {
		    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PopQueue correct resp.");
		    m_waitingQueue.pop_front();
		    DestroyPacket(packet);
		    StopTimer(m_retryTimer);
			StartTimer(m_queueTimer);
		
	    }
	    else
	    {
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue incorrect resp(%d, %04x).", packet->seq, packet->id);
	    }
    
    pthread_mutex_unlock(&m_queueMutex);
}

void JT808MessageHandler::PopQueue(int index)
{
	pthread_mutex_lock(&m_queueMutex);
	if(m_waitingQueue.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "jt808messagehandler::popqueue waiting queue empty.");
		pthread_mutex_unlock(&m_queueMutex);
		return;
	}

	StruJT808Packet *packet = *(m_waitingQueue.begin());
	JT808Packet &jt_packet=*(packet->packetarray[0]);
	if(JT808_GET_LONG(jt_packet, 17)== index)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::popqueue correct resp.");
		m_waitingQueue.pop_front();
		DestroyPacket(packet);
		StopTimer(m_retryTimer);
		if(m_waitingQueue.size() > 0)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::popqueue start queue timer");
			StartTimer(m_queueTimer);
		}
	}
	else
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "jt808messagehandler::popqueue incorrect resp(%d, %04x).", packet->seq, JT808_GET_LONG(jt_packet, 0));
	}
	pthread_mutex_unlock(&m_queueMutex);

}
void JT808MessageHandler::HandleTimer(string &name, void *data)
{
	string phonenumber= "";
    printf( "xie----------HandleTimer name=%s\n", name.c_str() );
    LogUtility::Log(LOG_LEVEL_INFO, "JT808MessageHandler::HandleTimer." );
	if(name == JT808_HELLO_TIMER_NAME)
	{
		if(connect_tmp==1)
			SendHello();
		else 
			StartTimer(m_helloTimer);
	}
	else if(name == JT808_QUEUE_TIMER_NAME)
	{
		SendQueue();
	}
	else if(name == JT808_RETRY_TIMER_NAME)
	{
		HandleRetryTimer(data);
	}
	else if(name == JT808_RETRY_TIMER_NAME_LOCATION)
	{
		HandleRetryTimerLocation(data);
	}
	else if(name == JT808_RETRY_REGISTER_NAME)
	{
		HandleRetryTimer(data);
	}
	else if(name == JT808_DEAD_ZONE_TIMER )
	{
	    SendJT808CallBackNotify( 3, phonenumber);
		sleep(3);
		printf("==============begin eth0 up================\n");
		ConnectionManager::Instance()->b_zone = false;
		DeadBlock_flag=0;
		ConnectionManager::Instance()->Start();

	}
}

void JT808MessageHandler::HandleRetryTimer(void *data)
{
	pthread_mutex_lock(&m_queueMutex);
	StruJT808Packet *timer_packet = (StruJT808Packet *)data;
	if(m_waitingQueue.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandleRetryTimer empty");
		pthread_mutex_unlock(&m_queueMutex);
		return ;
	}

	StruJT808Packet *packet = *(m_waitingQueue.begin());
	if(timer_packet != packet)
	{
		LogUtility::Log(LOG_LEVEL_WARN, "JT808MessageHandler::HandleRetryTimer out of sync");
		pthread_mutex_unlock(&m_queueMutex);
		return ;
	}
	LogUtility::Log(LOG_LEVEL_DEBUG, "%d::*%d*****%d****%d",packet->retry, packet->seq, packet->id, packet->send_index);
#if 0
	if (packet->id==JT808_PACKET_ID_HELLO)	
	{
		printf("packet->id ======%d  packet->retry==%d m_retryTimes==%d \n",packet->id,packet->retry,m_retryTimes);
		if(packet->retry >= m_retryTimes)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandleRetryTimer %d reach limit", packet->id);
			ConnectionManager::Instance()->Reconnect();
		}
	}
#endif
	if((m_authfailure != 0)&&((packet->id == JT808_PACKET_ID_AUTH) ? (packet->retry > 9) : (packet->retry > m_retryTimes)))
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandleRetryTimer %d reach limit", packet->id);
		m_waitingQueue.pop_front();
		if(packet->id == JT808_PACKET_ID_AUTH){
			JT808MessageHelper::SendRegister(1, NULL);
		}
		if (packet->id == JT808_PACKET_ID_REGISTER&&packet->retry>=4)
		{
			ConnectionManager::Instance()->Reconnect();
		}
			
		
		/*不应该在这个地方重连，应该放在收到后台3次注册失败的地方
		else {
			ConnectionManager::Instance()->Reconnect();
		}
		*/
	}
	else
	{
		SendPacket(packet);
		packet->retry ++;
	}
	pthread_mutex_unlock(&m_queueMutex);
}
void JT808MessageHandler::HandleRetryTimerLocation(void *data)
{
	pthread_mutex_lock(&m_queueMutex);
#if 0
	StruJT808Packet *timer_packet = (StruJT808Packet *)data;
	if(m_waitingQueue_location.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandleRetryTimer empty");
		pthread_mutex_unlock(&m_queueMutex);
		return ;
	}

	StruJT808Packet *packet = *(m_waitingQueue_location.begin());
	if(timer_packet != packet)
	{
		LogUtility::Log(LOG_LEVEL_WARN, "JT808MessageHandler::HandleRetryTimer out of sync");
		pthread_mutex_unlock(&m_queueMutex);
		return ;
	}
	if(packet->retry >= m_retryTimes)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::HandleRetryTimer %d reach limit", packet->id);
		m_waitingQueue_location.pop_front();
		DestroyPacket(packet);
       	       ConnectionManager::connection_bei= 2;
		ConnectionManager::Instance()->Reconnect();
	}
	else
	{
		SendPacket(packet);
		packet->retry ++;
	}
#endif
	pthread_mutex_unlock(&m_queueMutex);
}

void JT808MessageHandler::SendHello()
{
    //sleep(1);
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendHello entry");
    StruJT808Packet *packet = CreatePacket();
    packet->reply = true;

	
    SendBody(packet, JT808_PACKET_ID_HELLO, JT808_CRYTO_NONE);
}

void JT808MessageHandler::SendBody(StruJT808Packet *packet, unsigned short id, ENUM_JT808_CRYTO crypto)
{
    printf("xie-----JT808MessageHandler::SendBody id=id=0x%x\n", id );
	struct timeval tv; 
	m_helloTimeout = ConfigManager::GetHeartBeatInterval();
    m_retryTimeout = ConfigManager::GetTcpRetryTimeout();
    if(ConnectionManager::Instance()->m_connId == ENUM_CONNECTION_ID_IC)
    {
        if(id != JT808_PACKET_UP_TRANSPARENT_TRANSMISSION)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::current state is connect to ic");
            DestroyPacket(packet);
            return;
        }
    }
    else
    {
        if(!m_registered || !m_authorized)
        {
            if((id != JT808_PACKET_LOC_MULTI_REPORT ) && (id != JT808_PACKET_ID_QUICK_REGISTER) && (id != JT808_PACKET_ID_REGISTER) && (id != JT808_PACKET_ID_AUTH) && (id!=JT808_PACKET_LOC_REPORT))
            {
                LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendBody not ready(%d, %d)", m_registered, m_authorized);
                DestroyPacket(packet);
                return;
            }
        }
     }

 #if 0        
            if(!m_registered )
        	{
                printf("ENTER SendBody3  ID=====%d\n\n ",id);



                

                if( (id != JT808_PACKET_ID_QUICK_REGISTER) && (id != JT808_PACKET_ID_REGISTER) )
        		{
        			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendBody not ready(%d, %d)", m_registered, m_authorized);
        			DestroyPacket(packet);
        			return;
        		}
        	}else if( !m_authorized )
            {
                if((id != JT808_PACKET_LOC_MULTI_REPORT ) && (id != JT808_PACKET_ID_QUICK_REGISTER) && (id != JT808_PACKET_ID_REGISTER) && (id != JT808_PACKET_ID_AUTH) && (id!=JT808_PACKET_LOC_REPORT))
        		{
        			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendBody not ready(%d, %d)", m_registered, m_authorized);
        			DestroyPacket(packet);
        			return;
        		}
            }
       
#endif
     
    if(packet->packetarray.size())
    {
		vector<JT808Packet*> &packetarray = packet->packetarray;
	    for(int i = 0; i < (int)packetarray.size(); i ++)
	    {
            if(packetarray[i]->size() > JT808_SEGMENT_SIZE)
		    {
			    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendBody segment array find big packet(%d)", i);
			    DestroyPacket(packet);
			    return;
		    }
           
		    if(i == 0) packet->seq = m_seq;
                   printf("enter SystemOffHandleHeartLocPacket1   \n ");
			mcumessagehandle.get_utc_now_time(&tv);
	        int ret;
		   ret =  PacketHead(packet,packetarray[i], (unsigned short)id, crypto, packet->segment_id[i]);
			if(ret < 0)
			{
				// add dbus to ui  
				printf("------------------------PacketHead < 0\n");
				return ;
			}
			mcumessagehandle.SystemOffHandleHeartLocPacket(packet,(unsigned int )tv.tv_sec);
		    PacketCheckSumFlag(packetarray[i]);
	    }
       	packet->segment_id.clear();
        packet->send_index=0;
        PushQueue(packet);
    }
    else
    {
    	printf("enter SystemOffHandleHeartLocPacket2   \n ");
        JT808Packet &content = packet->packet;
        if(content.size() > JT808_SEGMENT_SIZE)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendBody segment not support");
            DestroyPacket(packet);
            return;
        }
	mcumessagehandle.get_utc_now_time(&tv);
        PacketHead(packet, (unsigned short)id, crypto);
	mcumessagehandle.SystemOffHandleHeartLocPacket(packet,(unsigned int )tv.tv_sec);
        PacketCheckSumFlag(packet);
        PushQueue(packet);
    }
}

int JT808MessageHandler::PushQueue(StruJT808Packet *packet)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "xie------JT808MessageHandler::PushQueue id=0x%x-----\n",packet->id);
    
	pthread_mutex_lock(&m_queueMutex);
	switch(packet->id){
		case JT808_PACKET_TERM_COMMON_REP:
		case JT808_PACKET_LOC_QUERY_REP:
		case JT808_PACKET_ANSWER:
		case JT808_PACKET_QUERY_PARAMETERS_REP:
		case JT808_PACKET_CAMERA_START_RSP:
		case JT808_PACKET_MEDIA_INDEX_QUERY_RSP:
		case JT808_PACKET_DATA_SEND:
		case JT808_PACKET_MEDIA_DATA:
		case JT808_PACKET_UPDATE_TERM_REP:
		case JT808_PACKET_QUERY_ATTRIBUTE_REP:
		case JT808_PACKET_RESEND_REQUEST:
		case JT808_PACKET_ID_HELLO:
			{
				printf( "xie-printf--------JT808MessageHandler::PushQueue id=0x%x-----\n",packet->id );
				if(packet->packetarray.size() >0)
                		{
					if(m_waitingQueue.size() >= JT808_MAX_WAITING_QUEUE)
					{
						LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue full");
						pthread_mutex_unlock(&m_queueMutex);
						DestroyPacket(packet);
						return -1;
					}
			
						m_waitingQueue.push_back(packet);
						pthread_mutex_unlock(&m_queueMutex);
						if(m_waitingQueue.size() == 1)
						{
							LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue empty");
							StartTimer(m_queueTimer);
						}
					break;
				}
			}
				if(m_waitingQueue_ack.size() >= JT808_MAX_WAITING_QUEUE)
				{
					LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue   m_waitingQueue_ack full");
					pthread_mutex_unlock(&m_queueMutex);
					DestroyPacket(packet);
					return -1;
				}
				m_waitingQueue_ack.push_back(packet);
				pthread_mutex_unlock(&m_queueMutex);
				if(m_waitingQueue_ack.size() == 1)
				{
					printf("ConnectionManager::Instance()->m_connected==%d \n",ConnectionManager::Instance()->m_connected);
					if (ConnectionManager::Instance()->m_connected==true)
					{
						LogUtility::Log(LOG_LEVEL_DEBUG, "Jt808messagehandler::pushqueue empty  m_waitingQueue_ack");
						//StartTimer(m_queueTimer_100);
						StartTimer(m_queueTimer);
					}
			    }
				break;

		case JT808_PACKET_LOC_REPORT:
		case JT808_PACKET_UP_TRANSPARENT_TRANSMISSION:
		case JT808_PACKET_LOC_MULTI_REPORT: 
		case JT808_PACKET_DRIVER_INFO:
			{
				  printf( "xie-printf--------JT808MessageHandler::PushQueue id=0x%x-----\n",packet->id );
				if(m_waitingQueue_reply.size() >= JT808_MAX_WAITING_QUEUE)
				{
					LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue full");
					pthread_mutex_unlock(&m_queueMutex);
					DestroyPacket(packet);
					return -1;
				}
			
				m_waitingQueue_reply.push_back(packet);
				pthread_mutex_unlock(&m_queueMutex);
				 if(m_waitingQueue_reply.size()== 1)
				 {
				 	if (ConnectionManager::Instance()->m_connected==true)
					{
						LogUtility::Log(LOG_LEVEL_DEBUG, "Jt808messagehandler::pushqueue empty  ROAD_TEST");
						StartTimer(m_queueTimer);
				 	}
				 }
			
			break;
		  }

	default :
				printf( "xie-printf--------JT808MessageHandler::PushQueue id=0x%x-----\n",packet->id );
				if(m_waitingQueue.size() >= JT808_MAX_WAITING_QUEUE)
				{
					LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue full");
					pthread_mutex_unlock(&m_queueMutex);
					DestroyPacket(packet);
					return -1;
				}
		
				m_waitingQueue.push_back(packet);
				if(m_waitingQueue.size() == 1)
				{
					LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue empty");
					StartTimer(m_queueTimer);
				}
			
			pthread_mutex_unlock(&m_queueMutex);
			break;
	}
	return 0;
}

void JT808MessageHandler::SendQueue()
{
	pthread_mutex_lock(&m_queueMutex);
	if(m_authorized==true&&s_multipacket==false)
	{
		if (m_waitingQueue_ack.size()<=0)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendQueue0 empty");
		}
		else
		{
			StruJT808Packet *packet = *(m_waitingQueue_ack.begin());
			SendPacket(packet);
			pthread_mutex_unlock(&m_queueMutex);
			return;
		}
		if(m_waitingQueue_reply.size() <= 0)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendQueue0 empty");
		}
		else
		{
			printf(" ----------------else send m_waitingQueue_reply  reply_flag==%d \n",reply_flag);
			StruJT808Packet *packet = *(m_waitingQueue_reply.begin());
			if (reply_flag==0)
			{
				SendPacket(packet);
				reply_flag=1;
				pthread_mutex_unlock(&m_queueMutex);
				return;
			}
		}
		
	}
	if(m_waitingQueue.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendQueue m_waitingQueue empty");
	}
	else
	{
		StruJT808Packet *packet = *(m_waitingQueue.begin());
		printf("sendqueue::m_waitingQueue.size()==%d\n",m_waitingQueue.size());
		LogUtility::Log(LOG_LEVEL_DEBUG, "%d::*%d*****%d****%d",packet->retry, packet->seq, packet->id, packet->send_index);
		if(packet->retry == 0){
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::*******%d****%d  empty",packet->id,packet->send_index);
			SendPacket(packet);
			if((packet->send_index >= (int)packet->packetarray.size()) && (packet -> id == 0x0700))
			{
				packet->retry++;
				packet->send_index =0;
				m_waitingQueue.pop_front();
				DestroyPacket(packet);
			//}else if(packet->id != JT808_PACKET_DATA_SEND){
			}
			else 
			{
				if(packet->packetarray.size()==0)packet->retry ++;
				else if(packet->send_index == packet->packetarray.size())packet->retry++;
			}
			pthread_mutex_unlock(&m_queueMutex);
			return;
		}
		else if(!(packet->segment_id.empty())){
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::reup ****%d****%d  empty",packet->id,packet->send_index);
			SendPacket(packet);
			pthread_mutex_unlock(&m_queueMutex);
			return;
		}
		else
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::cann't send **%d****%d  empty",packet->id,packet->retry);

	}
	pthread_mutex_unlock(&m_queueMutex);
	return;
}

void JT808MessageHandler::SendPacket(StruJT808Packet *packet)
{
	printf("----------------------SendPacket::packet->id==%d packet->seq==%d ",packet->id,packet->seq);
	if (shutdown_flag==1&&packet->id==JT808_PACKET_LOC_REPORT)
	{
		printf("111111111111111111111111111111111111111111111111111111111111111111111111111111\n");
		return;
	}

	m_send_len = 0;
	int ret=0;
	vector<JT808Packet*> &packetarray = packet->packetarray;
	if(packetarray.size())
	{
		if(packet->retry == 0){
			int i = 0;
			if(s_multipacket==false)s_multipacket=true;
			while((i < (int)JT808_SEND_SEGMENT_NUM_ONCE) && (packet->send_index < (int)packetarray.size()))
			{
					if(SendContent(*packetarray[packet->send_index]) < 0)
					{
						LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendPacket failed to send %d packet", packet->send_index);
						ConnectionManager::Instance()->Reconnect();
						return;
					}
					i ++;
					packet->send_index ++;
					printf("current index is %d \n",packet->send_index);
			}

			if(packet->send_index >= (int)packetarray.size())
			{
#if 0
				if(packet -> id == 0x0700)
				{
					m_waitingQueue.pop_front();
					DestroyPacket(packet);
				}
#endif
				if(s_multipacket== true) s_multipacket= false;
				//m_retryTimer->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
				//m_retryTimer->data2 = packet;
				//StartTimer(m_retryTimer);
				if(m_waitingQueue_reply.size() > 0)
				{
					LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket not empty");
					StartTimer(m_queueTimer);
				}
			}
			else
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket array not over");
				StartTimer(m_queueTimer);
			}
		}else if(!(packet->segment_id.empty())){
			if(s_multipacket==false)s_multipacket=true;
			if(SendContent(*packetarray[packet->segment_id[packet->send_index]]) < 0)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendPacket failed to send %d packet", packet->send_index);
				ConnectionManager::Instance()->Reconnect();
				return;
			}
			packet->send_index ++;
			printf("reup current index is %d \n",packet->send_index);
			if(packet->send_index >= (int)packet->segment_id.size())
			{
				packet->send_index = 0;
				packet->segment_id.clear();
				if(s_multipacket== true) s_multipacket= false;
				if(m_waitingQueue_reply.size() > 0)
				{
					LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket not empty");
					StartTimer(m_queueTimer);
				}
			}
			else
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket array not over");
				StartTimer(m_queueTimer);
			}

		}else {
			if(SendContent(*packetarray[(int)packetarray.size()-1]) < 0)
			{
				LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendPacket failed to send %d packet", packet->send_index);
				ConnectionManager::Instance()->Reconnect();
				return;
			}
			m_retryTimer->timeout = m_retryTimer->timeout * (packet->retry + 1);
			m_retryTimer->data2 = packet;
			StartTimer(m_retryTimer);
			if(m_waitingQueue_reply.size() > 0)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket not empty");
				StartTimer(m_queueTimer);
			}
		}
	}
	else
	{
		if( ret=SendContent(packet->packet) < 0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendPacket failed to send packet");
			ConnectionManager::Instance()->Reconnect();
			return;
		}
		if(packet->reply)
		{
			if (m_waitingQueue_ack.size() > 0)
			{
				if ((*m_waitingQueue_ack.begin())->id=packet->id)
				{
					printf("------------------------m_waitingQueue_ack.pop_front()\n");
					StartTimer(m_queueTimer);
					m_waitingQueue_ack.pop_front();
					DestroyPacket(packet);
				}
			}
			printf("SendPacket::sendloc  %d sendhello  %d \n",sendloc,sendhello);
			
#if 0		
			if(packet->id == JT808_PACKET_LOC_MULTI_REPORT)
			{
				m_waitingQueue_location.pop_front();
			}else 
				m_waitingQueue_reply.pop_front();
			if(m_waitingQueue_reply.size() > 0)
			{
				DestroyPacket(packet);
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket11 not empty");
				StartTimer(m_queueTimer);
				return ;
			}
			if(m_waitingQueue.size() > 0 && ((*m_waitingQueue.begin())->id!=packet->id))
			{
				DestroyPacket(packet);
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket31 not empty");
				StartTimer(m_queueTimer);
				return ;
			}


#endif

		}
		else
		{
			printf("SendPacket2::sendloc  %d sendhello  %d \n",sendloc,sendhello);
			if(packet->id==JT808_PACKET_LOC_REPORT)
			{
				if(packet->retry == 0)
				{
					m_retryTimer_location->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
				}
				else
				{
					m_retryTimer_location->timeout = m_retryTimer_location->timeout * (packet->retry + 1);
				}

				m_retryTimer_location->data2 = packet;
				StartTimer(m_retryTimer_location);
			}
			else 
			{
				
				if((packet->id == JT808_PACKET_ID_REGISTER)){
					StopTimer(m_retryRegister);
					m_retryRegister->data2 = packet;
					if(packet->retry == 0)
					{
						m_retryTimer->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
					}
					else
					{
						m_retryTimer->timeout = m_retryTimer->timeout * (packet->retry + 1);
	                  
					}
					StartTimer(m_retryRegister);
					return ;
				}
				if(packet->id == JT808_PACKET_ID_AUTH)
				{
					StopTimer(m_retryRegister);
					m_retryRegister->timeout = TIMER_SEC_TO_MSEC(JT808_RETRY_REGISTER_TIME);
					m_retryRegister->data2 = packet;
					m_retryRegister->start_interval.tv_sec = 0;
					m_retryRegister->start_interval.tv_usec = 0;
					StartTimer(m_retryRegister);
					return ;
				}
				if(packet->retry == 0)
				{
					m_retryTimer->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
				}
				else
				{
					m_retryTimer->timeout = m_retryTimer->timeout * (packet->retry + 1);
                  
				}
             			printf("m_retryTimer->timeout =====%d\n",m_retryTimer->timeout );
				m_retryTimer->data2 = packet;
				//StartTimer(m_retryTimer);
			}

			if(m_waitingQueue_reply.size() > 0)
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket1 not empty");
				StartTimer(m_queueTimer);
				return ;
			}
		
			if(m_waitingQueue.size() > 0 && ((*m_waitingQueue.begin())->id!=packet->id)&&((*m_waitingQueue.begin())->retry==0))
			{
				LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendPacket3 not empty %d",(*m_waitingQueue.begin())->id);
				StartTimer(m_queueTimer);
				return ;
			}

		}
    }
    
    return;
}

int JT808MessageHandler::SendContent(JT808Packet &content)
{
    int ret = 0;
    
    m_send_len = 0;

    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::SendContent entry(%d)", content.size());
    JT808Packet::const_array_range ar = content.array_one();
    if(ar.second > 0)
    {
        memcpy(m_send_buffer, ar.first, ar.second);
        m_send_len += ar.second;
    }
    
    ar = content.array_two();
    if(ar.second > 0)
    {
        memcpy(m_send_buffer + m_send_len, ar.first, ar.second);
        m_send_len += ar.second;
    }
    
    ret = ConnectionManager::Instance()->Send(m_send_buffer, m_send_len);
	printf("SendConnect::ret===%d  m_send_len====%d  \n",ret,m_send_len);
    if(ret != m_send_len)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::SendContent failed");
        return -1;
    }
    
    return m_send_len;
}

int JT808MessageHandler::PacketHead(StruJT808Packet *thepacket, unsigned short id, ENUM_JT808_CRYTO crypto)
{
    unsigned short len;
    unsigned short attr = 0;
    m_seq=tmp_seq;
    JT808Packet &packet = thepacket->packet;
    
    len = (unsigned short)packet.size();
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PacketHead Count len %d", len);

    
    packet.push_front((unsigned char)((unsigned char)(m_seq & 0xFF)));
    packet.push_front((unsigned char)((unsigned char)((m_seq >> 8) & 0xFF)));

    pthread_mutex_lock(&m_packetMutex);
  
	if (shutdown_flag==1&&id==JT808_PACKET_LOC_REPORT)
		seq_last=m_seq;
    thepacket->seq = m_seq;
    thepacket->id = id;
	printf(" id  %d  fei  m_seq============%d    tmp_seq==================%d     seq_last    %d \n ",id,m_seq,tmp_seq,seq_last);
    m_seq ++;
    tmp_seq=m_seq;
    pthread_mutex_unlock(&m_packetMutex);
    int ret=0;
	if( (ret = strlen((char *)m_phoneNumber))==0)
	   {
		  printf("m_phoneNumber  len = 0 ret = %d\n",ret);
		  return -1;
	  }
    
		
    for(int i = sizeof(m_phoneNumber); i > 0; i --)
    {
        packet.push_front(m_phoneNumber[i-1]);
    }

    if(thepacket->total_segment)
    {
        attr |= (1 << 13);
    }
    
    if(crypto == JT808_CRYTO_RSA)
    {
        attr |= (1 << 10);
    }
    attr |= (len & 0x3FF);
    packet.push_front((unsigned char)((unsigned char)(attr & 0xFF)));
    packet.push_front((unsigned char)((unsigned char)((attr >> 8) & 0xFF)));
    packet.push_front((unsigned char)((unsigned char)(id & 0xFF)));
    packet.push_front((unsigned char)((unsigned char)((id >> 8) & 0xFF)));
	return 0;
}
int JT808MessageHandler::PacketHead(StruJT808Packet *packet , JT808Packet *thepacket, unsigned short id, ENUM_JT808_CRYTO crypto, int index)
{
    unsigned short len;
    unsigned short attr = 0;

    pthread_mutex_lock(&m_packetMutex);
    m_seq=tmp_seq;
    
    len = (unsigned short)thepacket->size();
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PacketHead len %d", len);

    if(packet->total_segment)
    {
        thepacket->push_front((unsigned char)((unsigned char)(index & 0xFF)));
        thepacket->push_front((unsigned char)((unsigned char)((index >> 8) & 0xFF)));
        thepacket->push_front((unsigned char)((unsigned char)(packet->total_segment & 0xFF)));
        thepacket->push_front((unsigned char)((unsigned char)((packet->total_segment >> 8) & 0xFF)));
    }
    
       thepacket->push_front((unsigned char)((unsigned char)(m_seq & 0xFF)));
       thepacket->push_front((unsigned char)((unsigned char)((m_seq >> 8) & 0xFF)));

   
    
	printf("second PacketHead   m_seq============%d    tmp_seq==================%d \n ",m_seq,tmp_seq);
	seq_last=m_seq;
       packet->seq = m_seq;
       packet->id = id;

    m_seq ++;
	tmp_seq=m_seq;
   
    int ret;
	if((ret == strlen((char *)m_phoneNumber)) == 0)
	{   
		printf("------------------------- m_phoneNumber  ret = %d\n",ret );
		return -1;
	}
		
    for(int i = sizeof(m_phoneNumber); i > 0; i --)
    {
        thepacket->push_front(m_phoneNumber[i-1]);
    }

    if(packet->total_segment)
    {
        attr |= (1 << 13);
    }
    
    if(crypto == JT808_CRYTO_RSA)
    {
        attr |= (1 << 10);
    }
    attr |= (len & 0x3FF);
    thepacket->push_front((unsigned char)((unsigned char)(attr & 0xFF)));
    thepacket->push_front((unsigned char)((unsigned char)((attr >> 8) & 0xFF)));

    thepacket->push_front((unsigned char)((unsigned char)(id & 0xFF)));
    thepacket->push_front((unsigned char)((unsigned char)((id >> 8) & 0xFF)));
   pthread_mutex_unlock(&m_packetMutex);
	return 0;
}

void JT808MessageHandler::PacketCheckSumFlag(StruJT808Packet *thepacket)
{
    unsigned char checksum = 0;
    
    JT808Packet &packet = thepacket->packet;
    
    for(JT808Packet::iterator it = packet.begin(); it != packet.end(); it ++)
    {
        checksum ^= (*it);
    }
    
    packet.push_back(checksum);

    for(JT808Packet::iterator it = packet.begin(); it != packet.end(); it ++)
    {
	    if(*it==JT808_PACKET_FLAG)
	    {
		    *it=JT808_PACKET_ESCAPE;
		    if(++it==packet.end())
		    {
			    packet.push_back(JT808Value(0x02));
			    break;
		    }
		    else
			    it=packet.insert(it,JT808Value(0x02));
		    continue;
	    }
	    if(*it==JT808_PACKET_ESCAPE)
	    {
		    if(++it==packet.end())
		    {
			    packet.push_back(JT808Value(0x01));
			    break;
		    }
		    else
			    it=packet.insert(it,JT808Value(0x01));
	    }
    }

    packet.push_front(JT808_PACKET_FLAG);
    packet.push_back(JT808_PACKET_FLAG);
}
void JT808MessageHandler::PacketCheckSumFlag(JT808Packet *thepacket)
{
    unsigned char checksum = 0;
    
    
    for(JT808Packet::iterator it = thepacket->begin(); it != thepacket->end(); it ++)
    {
        checksum ^= (*it);
    }
    
    thepacket->push_back(checksum);

    for(JT808Packet::iterator it = thepacket->begin(); it != thepacket->end(); it ++)
    {
	    if(*it==JT808_PACKET_FLAG)
	    {
		    *it=JT808_PACKET_ESCAPE;
		    if(++it==thepacket->end())
		    {
			    thepacket->push_back(JT808Value(0x02));
			    break;
		    }
		    else
			    it=thepacket->insert(it,JT808Value(0x02));
		    continue;
	    }
	    if(*it==JT808_PACKET_ESCAPE)
	    {
		    if(++it==thepacket->end())
		    {
			    thepacket->push_back(JT808Value(0x01));
			    break;
		    }
		    else
			    it=thepacket->insert(it,JT808Value(0x01));
	    }
    }

    thepacket->push_front(JT808_PACKET_FLAG);
    thepacket->push_back(JT808_PACKET_FLAG);
}
void JT808MessageHandler::RSendMessage(unsigned short req, map<int, int> &idlist)
{
	pthread_mutex_lock(&m_queueMutex);
	if(m_waitingQueue.size() <= 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::ReUp empty.");
		pthread_mutex_unlock(&m_queueMutex);
		return;
	}

	StruJT808Packet *packet = *(m_waitingQueue.begin());
	if((packet->retry >= 1) && (packet->seq == req))
	{
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ReUp correct resp.");
		map<int, int>::const_iterator map_it= idlist.begin();
		while(map_it != idlist.end())
		{
			packet->segment_id.push_back( map_it->first - 1);
			++map_it;
		}
		if(m_waitingQueue.size() > 0)
		{
			LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ReUp start queue timer");
			StartTimer(m_queueTimer);
		}
	}
	else
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::PopQueue incorrect resp(%d, %04x).", packet->seq, packet->id);
		//StopTimer(m_queueTimer);
	}
    	pthread_mutex_unlock(&m_queueMutex);

}
void JT808MessageHandler::HandleConnect()
{
	StartTimer(m_queueTimer);
}
void JT808MessageHandler::HandleShutDownReboot(int flag)
{
	if (flag==1)
	{		
			StopTimer(m_helloTimer);
			printf("111111111111111111111111111111111111111111111111111111111111111111\n");
			sleep(4);
			mcumessagehandle.WriteOneServerInformation();
			printf("即		将		关		机1  systemoff\n");
			sleep(50);
			system("sync");
			system("accoff.sh 1 &");
			return ;
	}
	else if (flag==2)
	{
			StopTimer(m_helloTimer);
			//JT808AccOffDbusNotifyC  Notify;
			//Notify.flag=2;
			//g_pDbusServer->sendNotify(&Notify);
			printf("即		将		重		启2\n");
			sleep(5);
			system("sync");
			system("accoff.sh 2 &");
			return;
	}else if (flag==3)
	{
			StopTimer(m_helloTimer);
			//JT808AccOffDbusNotifyC  Notify;
			//Notify.flag=1;
			//g_pDbusServer->sendNotify(&Notify);
			//mcumessagehandle.SendPowerOffCheckTime();
			mcumessagehandle.WriteOneServerInformation();
			printf("即		将		关		机1  systemoff\n");
			sleep(5);
			system("sync");
			system("accoff.sh 3 &");
			return ;
	}
	else 
		return;
}
void JT808MessageHandler::HandleRegister()
{
	ConnectionManager::Instance()->NetBreak(2);	 
	m_authorized=false;
	m_registered=false;
	string auth=""; 
	VDRDeployParaTable *table = new VDRDeployParaTable();
	if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
		delete table;
		return;
	}
	table->Commit();
	delete table;
	ConfigManager::Refresh();
	ConnectionManager::Instance()->Reconnect();

}
int JT808MessageHandler::RefreshGetLocTime(int status)
{
	int day_flag;//0 day 1 night
	int loc_time=0;
	int heart_time=0;
	char buf[10];
	char buffer[1];
	int loctime_night;//默认如果是数据库为空默认2小时
	struct tm *date;
	time_t timep; 
	time(&timep); 
	date=gmtime(&timep); 
	//char * PathName="/nand4/dayornight";
	
	
	VDRRunParaTable *table = new VDRRunParaTable();
	
	if(table->GetIntValue(VDR_PARA_SLEEP_REPORT_INTERVAL, loc_time)<0);
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler get loc_retry_time failed");
	}
	if(table->GetIntValue(VDR_PARA_HEART_BEAT_INTERVAL, heart_time)<0);
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler get heart_retry_time failed");
	}
	
	printf("loc_time=%d heart_time=%d \n",loc_time,heart_time);
	heart_retry_time=(unsigned short)heart_time;
	VDRBaseConfigTable  *baseconfigtable = new VDRBaseConfigTable();
	vector<unsigned char> IntervalTime;
   	IntervalTime.clear();
    baseconfigtable->GetSequenceValue(VDR_BASE_STATUS_D2, IntervalTime);
	if (IntervalTime.empty())
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler get loc_retry_time failed");
		loc_retry_time=loc_time;
		return -1;
	}
	
	loctime_night=IntervalTime[0]<<24|IntervalTime[1]<<16|IntervalTime[2]<<8|IntervalTime[3];
	printf ("loctime_night ====%d \n",loctime_night);

	int hour1=BCD_TO_VALUE(IntervalTime[4]);
	int minute1=BCD_TO_VALUE(IntervalTime[5]);
	int hour2=BCD_TO_VALUE(IntervalTime[6]);
	int minute2=BCD_TO_VALUE(IntervalTime[7]);

	printf ("hour1=%d  minute1=%d hour2=%d  minute2=%d \n",hour1,minute1,hour2,minute2);
	printf("%d  %d  %d  \n",(1900+date->tm_year), (1+date->tm_mon),date->tm_mday); 
	printf(" %d;%d;%d\n", date->tm_hour, date->tm_min, date->tm_sec);

	
	if (hour1<=hour2)
	{
		if (date->tm_hour>hour1&&date->tm_hour<hour2)
		{
			loc_retry_time=loc_time;
			day_flag=0;
			printf("1is the day loc_retry_time==%d   \n",loc_retry_time);
		}
		else if (date->tm_hour==hour1&&date->tm_hour==hour2)
		{
			if ( minute1<=date->tm_min&&minute2>=date->tm_min)
			{	
				loc_retry_time=loc_time;
				day_flag=0;
				printf("2is the day loc_retry_time==%d   \n",loc_retry_time);
			}
			else
			{
				loc_retry_time=loctime_night;
				day_flag=1;
				printf("2is the night loc_retry_time==%d   \n",loc_retry_time);
			}
		}
		else if (date->tm_hour==hour1&&date->tm_min>=minute1)
		{	
			loc_retry_time=loc_time;
			day_flag=0;
			printf("3is the day loc_retry_time==%d   \n",loc_retry_time);
		}
		else if (date->tm_hour==hour2&&date->tm_min<=minute2)
		{	
			loc_retry_time=loc_time;
			day_flag=0;
			printf("4is the day loc_retry_time==%d   \n",loc_retry_time);
		}
		else
		{	
			loc_retry_time=loctime_night;
			day_flag=1;
			printf("5is the night loc_retry_time==%d   \n",loc_retry_time);
		}
	}
	else
	{
		loc_retry_time=loc_time;
		day_flag=0;
	}
#if 0
	if ((status==1&&AccStatus==3)||(status==0&&AccStatus==2))
	{
		FILE *pFile = fopen(PathName,"r");
		if(pFile==NULL)
		{
			printf("no this file\n");
			TimeToDev=loc_retry_time;
			fclose(pFile);
		}
		else
		{ 
			printf("this  is  the  file   *************\n");
			fread(buffer,1,2,pFile);
			int dayornight=atoi(buffer);
			printf("dayornight======================%d \n",dayornight);
			if (dayornight==1)
			{
				TimeToDev=loctime_night;
			}
			else 
			{
				TimeToDev=loc_time;
			}
			printf("TimeToDev=============%d \n",TimeToDev);
			
		}

		if (pFile==NULL)
		{
			FILE *pFile1 = fopen(PathName,"w+");
			if (day_flag==1)
			{
				fwrite("1",1,strlen("1"),pFile1);
				fclose(pFile1);
			}
			else 
			{	
				fwrite("0",1,strlen("0"),pFile1);
				fclose(pFile1);
			}
		}else
		{
			FILE *pFile2 = fopen(PathName,"w+");
			printf("day_flag========================%d \n",day_flag);
			if (day_flag==1)
			{	
				fwrite("1",1,strlen("1"),pFile2);
				fclose(pFile2);
			}
			else 
			{	
				fwrite("0",1,strlen("0"),pFile2);
				fclose(pFile2);
			}
		}
	}
	
	printf ("loc_retry_time===================%d \n",loc_retry_time);
   #endif
	return 0;
}




