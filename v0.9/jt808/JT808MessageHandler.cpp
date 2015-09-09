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
#include "JT808DbusMessage.h"
#include <sys/time.h> 
#include "VDRBaseConfigTable.h"
#include <boost/locale.hpp>

#define DeadTime 3600
int loc_seq=0;
int seq_emergency;
extern int emergency_notify;
extern int shutdown_flag;
int TimeToDev=0;
extern char * Main_Server_To_Mcu;
extern char * Second_Server_To_Mcu;
extern unsigned short Port_To_Mcu;
extern int Lougout;
extern DBusServerC    *g_pDbusServer;
extern unsigned short heart_retry_time;//心跳间隔
extern unsigned short loc_retry_time;//位置信息间隔
extern unsigned int AccStatus;
extern GeneralTimerItem *systemoff;
extern McuMesageHandler mcumessagehandle;
extern unsigned short seq_last;
unsigned short tmp_seq=0;
extern unsigned char  connect_tmp;
extern int DeadBlock_flag;
extern int SendDataFlag;
extern unsigned short heart_retry_time;
extern unsigned short loc_retry_time;
unsigned char connectphone_tmp=0;
int flag_emergency=0;
extern unsigned int  loc_night_sleep;
extern unsigned int  loc_day_sleep;
extern unsigned char day_time[4];

unsigned char	connect_Id;
string main_server="jt1.gghypt.net";
string second_server="jt2.gghypt.net";
int port=7008;


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
	VDRDeployParaTable *table = new VDRDeployParaTable();
    string authToken = "";
    if(table->GetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, authToken) < 0)
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
	//StartTimer(m_retryRegister);
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

	if(state == ENUM_CONNECTION_STATE_CONNECTED)
    {
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
		if(ConfigManager::m_control == 0){
			printf("ConfigManager::m_control   0\n");
			JT808MessageHelper::SendAuth();
		}else if(m_registered){
			if(ConfigManager::GetAuthFlag())
			{
				printf("ConfigManager::GetAuthFlag()  0\n");
				JT808MessageHelper::SendAuth();
                
			}
		}else
			JT808MessageHelper::SendRegister(1, NULL);
	}
    }
    if(state == ENUM_CONNECTION_STATE_CONNECTTING)
    {
		printf("11111111111111111111111111111111111111111111111111111111111111111111111111111111   ENUM_CONNECTION_STATE_CONNECTTING \n");
        if(conn == ENUM_CONNECTION_ID_MASTER)
        {
            SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_CONNECTING, (int)CONN_STATUS_NTF_DISCONNECT);
        }
        else
        {
            SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_CONNECTING);
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
	unsigned char phonenum_tmp[6];
	int count;
    unsigned short id = JT808_GET_SHORT(m_rxPacket, 1);
	unsigned short attr = JT808_GET_SHORT(m_rxPacket, 3);
    unsigned short seq = JT808_GET_SHORT(m_rxPacket, 11);
    unsigned short packetNum;
    unsigned short sum;
	
		for ( count=0;count<6;count++)
		{
			phonenum_tmp[count]=(char)m_rxPacket[5+count];
		}
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
            JT808MessageHelper::HandlePacketResendRequest(m_rxPacket, body, len, seq);
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
            JT808MessageHelper::HandleTermUpdate(m_rxPacket, body, len, seq, sum, packetNum);
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
            JT808MessageHelper::SendDriverInfo(0);
	    break;
	case JT808_PACKET_CAMERA_START:
            JT808MessageHelper::HandleCameraStart(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_RECORD_START:
            JT808MessageHelper::HandleRecordStart(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_INDEX_QUERY:
            JT808MessageHelper::HandleQueryMediaIndex(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_SINGLE_INDEX_QUERY:
            JT808MessageHelper::HandleQueryMediaSingleIndex(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_DATA_RSP:
            JT808MessageHelper::HandlePacketMediaDataRsp(m_rxPacket, body, len, seq);
            break;
        case JT808_PACKET_MEDIA_UPLOAD:
            JT808MessageHelper::HandlePacketMediaUpload(m_rxPacket, body, len, seq);
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
		    VDRRunParaTable *table = new VDRRunParaTable();
		  
		    if(table->GetStringValue((int)VDR_PARA_MAIN_SERVER_ADDRESS, main_server) < 0)
		    {
			    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult get auth token failed");
			    delete table;
			    return;
		    }
			if(table->GetStringValue((int)VDR_PARA_SECOND_SERVER_ADDRESS, second_server) < 0)
		    {
			    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult get auth token failed");
			    delete table;
			    return;
		    }
			if(table->GetIntValue((int)VDR_PARA_TCP_PORT, port) < 0)
		    {
			    LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult get auth token failed");
			    delete table;
			    return;
		    }
			printf("main_server  %s   second_server  %s  port  %d \n",main_server.c_str(),second_server.c_str(),port);
			delete table;
		    auth_failure = 0;
		    m_authfailure = 1;
			reply_flag=0;
    		PopQueue(id, seq);
		    StopTimer(m_retryRegister);
			printf("if(state == ENUM_CONNECTION_STATE_CONNECTED)  \n");
			if(connect_Id== ENUM_CONNECTION_ID_MASTER)
	        {
	        	connectphone_tmp=1;
	            SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_CONNECTED, (int)CONN_STATUS_NTF_DISCONNECT);
				printf("SendJT808ConncetionStatusNotify 1111111111111111111\n");
			}
	        else
	        {
	            SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_CONNECTED);
				printf("SendJT808ConncetionStatusNotify 122222222222222222222222\n");
			}
		
	    }else
	    {
		    auth_failure++;
			printf("******************************88auth_failure                 %d   &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n",auth_failure);
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
    {
		LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::ProcessCommonReply %d, %04x, %d.", seq, id, result);
   
	    PopQueue(id, seq);
    }
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
	printf("loc_seq   %d   shutdown_flag   %d  id   %d   seq   %d \n",loc_seq,shutdown_flag,id,seq);
	printf("PopQueue::emergency_notify   %d  flag_emergency %d   \n",emergency_notify,flag_emergency);
	if (loc_seq==seq&&shutdown_flag==1&&id ==JT808_PACKET_LOC_REPORT)
	{
		system("sync");
		system("accoff.sh 1 &");
	}
	if (seq_emergency==seq&&id ==JT808_PACKET_LOC_REPORT&&emergency_notify==1&&flag_emergency==0)
	{
		flag_emergency=1;
		JT808EmergencyNotifyC notify;
		g_pDbusServer->sendNotify(&notify);
	}
	pthread_mutex_lock(&m_queueMutex);
	printf("---------------------PopQueue  id= %d seq = %d\n",id, seq);
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
			reply_flag=0;
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
		ConnectionManager::Instance()->Connect();

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
	int ret;
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
        if(!m_registered || !m_authorized )
        {
            if((id != JT808_PACKET_LOC_MULTI_REPORT ) && (id != JT808_PACKET_ID_QUICK_REGISTER) && (id != JT808_PACKET_ID_REGISTER) && (id != JT808_PACKET_ID_AUTH) && (id!=JT808_PACKET_LOC_REPORT))
            {
            /*	if (id ==JT808_PACKET_LOC_REPORT && SendDataFlag==0)
            			{
					printf("The  Connect  is  not   OK  \n");
					return;
				} */
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
        ret=PacketHead(packet, (unsigned short)id, crypto);
		if(ret < 0)
		{
				// add dbus to ui  
				printf("------------------------PacketHead < 0\n");
				return ;
		}
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
				  printf( "xie-printf--------JT808MessageHandler::PushQueue id=0x%x-----222\n",packet->id );
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
				printf( "xie-printf--------JT808MessageHandler::PushQueue id=0x%x-----111\n",packet->id );
				if(m_waitingQueue.size() >= JT808_MAX_WAITING_QUEUE)
				{
					LogUtility::Log(LOG_LEVEL_DEBUG, "jt808messagehandler::pushqueue full");
					pthread_mutex_unlock(&m_queueMutex);
					DestroyPacket(packet);
					return -1;
				}
				printf("m_waitingQueue.size()    %d \n",m_waitingQueue.size());
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
	printf("SendPacket::packet->id==%d packet->seq==%d ",packet->id,packet->seq);
	if (shutdown_flag==1&&packet->id==JT808_PACKET_LOC_REPORT)
	{
		printf("111111111111111111111111111111111111111111111111111111111111111111111111111111\n");
		return;
	}
	if (emergency_notify==1&&packet->id==JT808_PACKET_LOC_REPORT)
	{
			seq_emergency=packet->seq;
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
#if 0
			StopTimer(m_retryTimer);
			m_retryTimer->data2 = packet;
			if(packet->retry == 0)
			{
				m_retryTimer->timeout = TIMER_SEC_TO_MSEC(m_retryTimeout);
			}
			else
			{
				m_retryTimer->timeout = m_retryTimer->timeout * (packet->retry + 1);
			}
			StartTimer(m_retryTimer);
			
		
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
			
			if(packet->id==JT808_PACKET_LOC_REPORT)
			{
				printf("this is locreport\n");
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
						if (m_retryTimer->timeout>=TIMER_SEC_TO_MSEC(1200))
						{
							ConnectionManager::Instance()->NetBreak(1);	 
							m_authorized=false;
							m_registered=false;
							ConnectionManager::Instance()->Reconnect();
							return;
						}
	                  
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
	pthread_mutex_lock(&m_packetMutex);
    m_seq=tmp_seq;
    JT808Packet &packet = thepacket->packet;
    
    len = (unsigned short)packet.size();
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHandler::PacketHead Count len %d", len);

    
    packet.push_front((unsigned char)((unsigned char)(m_seq & 0xFF)));
    packet.push_front((unsigned char)((unsigned char)((m_seq >> 8) & 0xFF)));

   
   

	printf("PacketHead::m_seq  %d    tmp_seq %d \n ",m_seq,tmp_seq);
	if (shutdown_flag==1&&id==JT808_PACKET_LOC_REPORT)
		seq_last=m_seq;
    thepacket->seq = m_seq;
    thepacket->id = id;
	printf(" main  id  %d   m_seq   %d    tmp_seq    %d     seq_last    %d \n ",id,m_seq,tmp_seq,seq_last);
    m_seq ++;
    tmp_seq=m_seq;
    
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
	pthread_mutex_unlock(&m_packetMutex);
	return 0;
}
int JT808MessageHandler::PacketHead(StruJT808Packet *packet , JT808Packet *thepacket, unsigned short id, ENUM_JT808_CRYTO crypto, int index)
{
    unsigned short len;
    unsigned short attr = 0;
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

    pthread_mutex_lock(&m_packetMutex);
    
	printf("main PacketHead   m_seq  %d   tmp_seq  %d \n ",m_seq,tmp_seq);
	seq_last=m_seq;
    packet->seq = m_seq;
    packet->id = id;

    m_seq ++;
	tmp_seq=m_seq;
    pthread_mutex_unlock(&m_packetMutex);
    int ret;
	if((ret == strlen((char *)m_phoneNumber)) == 0)
	{   
		printf("m_phoneNumber  ret = %d\n",ret );
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
			JT808AccOffDbusNotifyC  Notify;
			Notify.flag=0;
			g_pDbusServer->sendNotify(&Notify);
			sleep(4);
			RefreshGetLocTime(0);
			mcumessagehandle.WriteOneServerInformation();
			mcumessagehandle.SendPowerOffCheckTime();
			mcumessagehandle.SendSystemOffInfo();
			HandleALLParametersTOMcu();
			printf("即		将		关		机1  systemoff\n");
			sleep(5);
			system("sync");
			system("accoff.sh 1 &");
			return ;
	}
	else if (flag==2)
	{		
			StopTimer(m_helloTimer);
			JT808AccOffDbusNotifyC  Notify;
			Notify.flag=2;
			g_pDbusServer->sendNotify(&Notify);
			printf("即		将		重		启2\n");
			sleep(2);
			system("sync");
			system("accoff.sh 2 &");
			return;
	}else if (flag==3)
	{
			StopTimer(m_helloTimer);
			RefreshGetLocTime(0);
			JT808AccOffDbusNotifyC  Notify;
			Notify.flag=0;
			g_pDbusServer->sendNotify(&Notify);
			HandleALLParametersTOMcu();
			mcumessagehandle.SendPowerOffCheckTime();
			mcumessagehandle.SendSystemOffInfo();
			printf("即		将		关		机1  systemoff\n");
			sleep(2);
			system("sync");
			system("accoff.sh 3 &");
			return ;
	}
	else 
		return;
}
void JT808MessageHandler::HandleRegister()
{
	JT808ParModifyNotifyC Notify;
	Notify.type = 1 ;
	g_pDbusServer->sendNotify(&Notify);
	if (Lougout==1)
	{
		Lougout=0;
		ConnectionManager::Instance()->Start();
	}
	ConnectionManager::Instance()->NetBreak(1);	 
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
	ConnectionManager::Instance()->Notify(ENUM_CONNECTION_STATE_DISCONNECTED);	
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
	memset(day_time,0,4);
	loc_night_sleep=loctime_night;
	loc_day_sleep=loc_time;
	for (int i=0;i<4;i++)
	{
		day_time[i]=IntervalTime[i+4];
	}
	int hour1=BCD_TO_VALUE(IntervalTime[4]);
	int minute1=BCD_TO_VALUE(IntervalTime[5]);
	int hour2=BCD_TO_VALUE(IntervalTime[6]);
	int minute2=BCD_TO_VALUE(IntervalTime[7]);
	
	printf ("hour1=%d  minute1=%d hour2=%d  minute2=%d \n",hour1,minute1,hour2,minute2);
	printf("%d  %d  %d  \n",(1900+date->tm_year), (1+date->tm_mon),date->tm_mday); 
	printf(" %d;%d;%d\n", date->tm_hour, date->tm_min, date->tm_sec);

#if 0
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
		if (hour1>hour2)
		{
			if (date->tm_hour<hour1&&hour2<date->tm_hour)
			{
				loc_retry_time=loctime_night;
				printf("6is the night loc_retry_time==%d   \n",loc_retry_time);
			}
			else 
			{
				if (hour1==date->tm_hour)
				{
					if (date->tm_min>=minute1)
					{
						loc_retry_time=loc_time;
						printf("7is the day loc_retry_time==%d   \n",loc_retry_time);
					}
					else{
						loc_retry_time=loctime_night;
						printf("8is the night loc_retry_time==%d   \n",loc_retry_time);
					}
				}
				else if(hour2==date->tm_hour)
				{
					if (date->tm_min<=minute2)
					{
						loc_retry_time=loc_time;
						printf("9is the day loc_retry_time==%d   \n",loc_retry_time);
					}
					else{
						loc_retry_time=loctime_night;
						printf("10is the night loc_retry_time==%d   \n",loc_retry_time);
					}
				}
				else if (hour1<date->tm_hour&&date->tm_hour<24)
				{
					loc_retry_time=loc_time;
					printf("11is the day loc_retry_time==%d   \n",loc_retry_time);
				}
				else if (0<date->tm_hour&&date->tm_hour<hour2)
				{
					loc_retry_time=loc_time;
					printf("12is the day loc_retry_time==%d   \n",loc_retry_time);
				}
				else{ 
					loc_retry_time=loctime_night;
					printf("13is the night loc_retry_time==%d   \n",loc_retry_time);
				}
			}
		}
		
	}
#endif
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
void  JT808MessageHandler::HandleALLParametersTOMcu()
{
	printf("HandleALLParametersTOMcu::enter HandleALLParametersTOMcu \n");
	unsigned char buf_data[663];
	int len=JT808MessageHelper::GetParametersTOMcu(buf_data);
	mcumessagehandle.WriteParaterToMcu(buf_data,len);
	return;
}
void JT808MessageHandler::GetStringFromPara(unsigned char * buf,int* num,string para)
{
	char * buf_tmp;
	*num=*num+30;
	printf("  num         GetStringFromPara    **********************  %d \n",*num);
	memcpy((char *)para.c_str(),(char * )(buf+*num),sizeof(char)*30);
	printf("%s   \n",para.c_str());
	
}
void  JT808MessageHandler::GetPhoneFromPara(unsigned char * buf,int* num,string para)
{
	char * buf_tmp;
	*num=*num+16;
	printf("  num     GetPhoneFromPara        **********************  %d \n",*num);
	memcpy((char *)para.c_str(),(char * )(buf+*num),sizeof(char)*16);
	printf("GetPhoneFromPara  %s   \n",para.c_str());
	
}
void  JT808MessageHandler::GetPlateFromPara(unsigned char * buf,int* num,string para)
{
	char * buf_tmp;
	*num=*num+20;
	printf("  num  GetPlateFromPara           **********************  %d \n",*num);
	buf_tmp = (char * )malloc(sizeof(char)*20);
	memcpy((char *)para.c_str(),(char * )(buf+*num),sizeof(char)*20);
	printf("GetPlateFromPara  %s   \n",para.c_str());
	
}


int  JT808MessageHandler::HandleParaMcutoCpu(unsigned char * buf,int len)
{

	if(len != len )
	{
		printf("The Para Len  Is  Wrong  len  %d  \n",len);
		return -1;
	}
	
	int i ;
	int num=0;
	for(i=0;i<663;i++)
	{
		printf("%02x ",buf[i]);
		if ((i+1)%16==0)
			printf("\n");
	}
	int size= sizeof (PARASET);

	//PARASET * parameter=(PARASET * )buf;
	PARA1 *para1 =(PARA1 *)(buf);
	PARA2 *para2 =(PARA2 *)(buf+306);
	PARA3 *para3 =(PARA3 *)(buf+386);
	PARA4 *para4 =(PARA4 *)(buf+344);
	PARA5 *para5 =(PARA5 *)(buf+398);
	PARA6 *para6 =(PARA6 *)(buf+478);

	printf("size ======================================== %d \n",size);

	HandleSetTheParaFromMcu(para1,para2,para3,para4,para5,para6);
	return 0;

}
int JT808MessageHandler::ReverseBytes(int value)
{  
	return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |(value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}

int  JT808MessageHandler::HandleSetTheParaFromMcu(PARA1 * paraset1,PARA2 * paraset2,PARA3 * paraset3,PARA4 * paraset4,PARA5 * paraset5,PARA6 * paraset6)
{

	printf("HandleSetTheParaFromMcu\n");
	printf("papaset.HeartInterval  %d \n",paraset1->HeartInterval);
	for (int i =0;i<4;i++)
	{
		printf("date %02x \n",paraset4->date[i]);
	}
	printf("NightSleepLocIngerval  %d \n",ReverseBytes(paraset4->NightSleepLocIngerval));
	printf("Loc_Strategy  %d \n",ReverseBytes(paraset4->Loc_Strategy));
	printf("Loc_Plan  %d \n",ReverseBytes(paraset4->Loc_Plan));
	printf("IC_TCP_Port  %d \n",ReverseBytes(paraset2->IC_TCP_Port));
	printf("IC_UDP_Port  %d \n",ReverseBytes(paraset2->IC_UDP_Port));
	printf("NoLoginLocInterval  %d \n",ReverseBytes(paraset4->NoLoginLocInterval));
	printf("SleepLocInterval  %d \n",ReverseBytes(paraset4->SleepLocInterval));
	printf("EmergencyLocInterval  %d \n",ReverseBytes(paraset4->EmergencyLocInterval));
	printf("DefaultLocInterval  %d \n",ReverseBytes(paraset4->DefaultLocInterval));
	printf("NightSleepLocIngerval  %d \n",(ReverseBytes(paraset4->NightSleepLocIngerval)));
	printf("DefaultDistanceInterval  %d \n",ReverseBytes(paraset4->DefaultDistanceInterval));
	printf("NoLoginDistanceInterval  %d \n",ReverseBytes(paraset4->NoLoginDistanceInterval));
	printf("SleepDistalceInterval  %d \n",ReverseBytes(paraset4->SleepDistalceInterval));
	printf("EmergencyDistanceInterval  %d \n",ReverseBytes(paraset4->EmergencyDistanceInterval));
	printf("Angle  %d \n",ReverseBytes(paraset4->Angle));


	
	printf("FenceRadius  %d \n",((paraset3->FenceRadius & 0xFFU) << 8 | (paraset3->FenceRadius & 0xFF00U) >> 8));
	printf("\n");
	for (int i=0;i<16;i++)
	{
		
			printf("%02x ",ReverseBytes(paraset5->HelpPhone[i]));	
	}
	printf("\n");
	printf("\n");
	for (int i=0;i<16;i++)
	{
		
			printf("%02x ",ReverseBytes(paraset5->ResetPhone[i]));	
	}
	printf("\n");
	printf("\n");
	for (int i=0;i<16;i++)
	{
		
			printf("%02x ",ReverseBytes(paraset5->RestFactoryPhone[i]));	
	}
	printf("\n");
		printf("MonitorPlatform   \n");
	printf("\n");
	for (int i=0;i<16;i++)
	{
		
			printf("%02x ",ReverseBytes(paraset5->MonitorPlatform[i]));	
	}
	printf("\n");
	printf("ReciveSMSPlatform   \n");
	printf("\n");
	for (int i=0;i<16;i++)
	{
		
			printf("%02x ",ReverseBytes(paraset5->ReciveSMSPlatform[i]));	
	}	
	printf("\n");
	printf("MonitorPlatformSMSPhone   \n");
	for (int i=0;i<16;i++)
	{
		
			printf("%02x ",ReverseBytes(paraset6->MonitorPlatformSMSPhone[i]));	
	}


	
	printf("\n");
	printf("AnswerStrategy  %d \n",ReverseBytes(paraset6->AnswerStrategy));		
	printf("LongAnswerTime  %d \n",ReverseBytes(paraset6->LongAnswerTime));
	printf("MonthAnswerTime  %d \n",ReverseBytes(paraset6->MonthAnswerTime));







	
	printf("AlarmShielding  %d \n",ReverseBytes(paraset6->AlarmShielding));
	printf("SendSMSFlag	%d \n",ReverseBytes(paraset6->SendSMSFlag));
	printf("AlarmShotFlag	%d \n",ReverseBytes(paraset6->AlarmShotFlag));
	printf("AlarmSaveFlag	%d \n",ReverseBytes(paraset6->AlarmSaveFlag));
	printf("KeyAlarmFlag	%d \n",ReverseBytes(paraset6->KeyAlarmFlag));
	printf("HighSpeed	%d \n",ReverseBytes(paraset6->HighSpeed));
	printf("OverSpeedTime	%d \n",ReverseBytes(paraset6->OverSpeedTime));
	printf("DriveTime	%d \n",ReverseBytes(paraset6->DriveTime));
	printf("DayCumulativeTime	%d \n",ReverseBytes(paraset6->DayCumulativeTime));
	printf("LeastRestTime	%d \n",ReverseBytes(paraset6->LeastRestTime));
	printf("LongParkingTime	%d \n",ReverseBytes(paraset6->LongParkingTime));
	
	printf("OverSpeedPoor %d \n",((paraset6->OverSpeedPoor & 0xFFU) << 8 | (paraset6->OverSpeedPoor & 0xFF00U) >> 8));
	printf("tired %d \n",((paraset6->tired & 0xFFU) << 8 | (paraset6->tired & 0xFF00U) >> 8));
	printf("CollisionPara %d \n",((paraset6->CollisionPara & 0xFFU) << 8 | (paraset6->CollisionPara & 0xFF00U) >> 8));
	printf("RolloverAlarmPara %d \n",((paraset6->RolloverAlarmPara & 0xFFU) << 8 | (paraset6->RolloverAlarmPara & 0xFF00U) >> 8));

	
	printf("PlateColor  %d\n ",paraset6->PlateColor);
	
		
	printf("TimerTakephotoControl	%d \n",ReverseBytes(paraset6->TimerTakephotoControl));
	printf("DistanceTakePhone	%d \n",ReverseBytes(paraset6->DistanceTakePhone));

	printf("PictureQuality  %d \n",ReverseBytes(paraset6->PictureQuality));
	printf("Light  %d \n",ReverseBytes(paraset6->Light));
	printf("Contrast  %d \n",ReverseBytes(paraset6->Contrast));
	printf("Satruation  %d\n ",ReverseBytes(paraset6->Satruation));
	printf("chroma	%d \n",ReverseBytes(paraset6->chroma));
	printf("Mileage	%d \n",ReverseBytes(paraset6->Mileage));

printf("ProvienceID %d\n ",((paraset6->ProvienceID & 0xFFU) << 8 | (paraset6->ProvienceID & 0xFF00U) >> 8));
printf("CityID %d \n",((paraset6->CityID & 0xFFU) << 8 | (paraset6->CityID & 0xFF00U) >> 8));
printf("GNSSmodel  %d \n",paraset6->GNSSmodel);
printf("GNSSBaudRate  %d\n ",paraset6->GNSSBaudRate);
printf("GNSSOutFrequency  %d \n",paraset6->GNSSOutFrequency);
printf("GNSSNMEAFrequency  %d \n",ReverseBytes(paraset6->GNSSNMEAFrequency));
printf("GNSSPoistingMethod  %d \n",paraset6->GNSSPoistingMethod);
printf("GNSSTime  %d \n",ReverseBytes(paraset6->GNSSTime));
printf("CAN1CollectTime  %d\n ",ReverseBytes(paraset6->CAN1CollectTime));
printf("CAN1UploadTime %d \n",((paraset6->CAN1UploadTime & 0xFFU) << 8 | (paraset6->CAN1UploadTime & 0xFF00U) >> 8));
printf("CAN2UploadTime  %d\n ",((paraset6->CAN2UploadTime & 0xFFU) << 8 | (paraset6->CAN2UploadTime & 0xFF00U) >> 8));
	int flag=0;
	int flag_setinter=0;
	int flag_settime=0;
#if 1
	VDRRunParaTable *table = new VDRRunParaTable();
	VDRDeployParaTable *deploytable = new VDRDeployParaTable();
	VDRBaseConfigTable  *baseconfigtable = new VDRBaseConfigTable();
	SQLiteSequence value_sleep_get(8);
	SQLiteSequence value_sleep_set(8);
    baseconfigtable->GetSequenceValue(VDR_BASE_STATUS_D2, value_sleep_get);

	for (int i=0;i<4;i++)
	{
		if (value_sleep_get[i+4]!=paraset4->date[i])
		{
			printf("value_sleep_set %d %02x IntervalTime %d %02x \n",i,value_sleep_get[i],i,paraset4->date[i]);	
			flag_settime=1;
		}
	}
	int loctime_night=value_sleep_get[0]<<24|value_sleep_get[1]<<16|value_sleep_get[2]<<8|value_sleep_get[3];
	if (loctime_night!=ReverseBytes(paraset4->NightSleepLocIngerval))
	{
		printf("loctime_night  %d ReverseBytes(paraset4->NightSleepLocIngerval) %d \n",loctime_night,ReverseBytes(paraset4->NightSleepLocIngerval));
		flag_setinter=1;
	}
	if (flag_setinter||flag_settime)
	{
		value_sleep_set[0]=(unsigned char)(ReverseBytes(paraset4->NightSleepLocIngerval)>>24);
		value_sleep_set[1]=(unsigned char)(ReverseBytes(paraset4->NightSleepLocIngerval)>>16);
		value_sleep_set[2]=(unsigned char)(ReverseBytes(paraset4->NightSleepLocIngerval)>>8);
		value_sleep_set[3]=(unsigned char)(ReverseBytes(paraset4->NightSleepLocIngerval));
		for (int i=0;i<4;i++)
		{
			value_sleep_set[i+4]=paraset4->date[i];
		}
		for (int i=0;i<8;i++)
		{
			printf("value_sleep_set[%d ] %02x \n",i,value_sleep_set[i]);
		}
		baseconfigtable->SetSequenceValue(VDR_BASE_STATUS_D2, value_sleep_set);
		baseconfigtable->Commit();
		delete baseconfigtable;
	}
	printf("flag_setinter   %d  flag_settime  %d \n",flag_setinter,flag_settime);
	int Province_ID;
	int City_ID;
	int Plate_Color;
	char * Plate_ID_tmp;
	int flag3=0;
	Plate_ID_tmp=(char * )malloc(20*sizeof(char));

	 string value_plate = "";
			
   int heart_time;
   if(table->GetIntValue(VDR_PARA_HEART_BEAT_INTERVAL, heart_time)<0);
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler get heart_retry_time failed");
	}
	
	printf(" heart_time=%d \n",heart_time);
	heart_retry_time=(unsigned short)heart_time;
	
	if(deploytable->GetIntValue((int)VDR_DEPLOY_PARA_PROVINCE_ID,Province_ID)<0)
	{
		printf("get   VDR_DEPLOY_PARA_PROVINCE_ID wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(deploytable->GetIntValue((int)VDR_DEPLOY_PARA_CITY_ID,City_ID)<0)
	{
		printf("get   VDR_DEPLOY_PARA_CITY_ID wrong  \n");
		return -1;
	}	
	usleep(5*1000);
	if(deploytable->GetStringValue((int)VDR_DEPLOY_PARA_PLATE_ID,value_plate)<0)
	{
		printf("get   VDR_DEPLOY_PARA_PLATE_ID wrong  \n");
		return -1;
	}
	string Plate_ID = boost::locale::conv::from_utf<char>(value_plate, JT808_CHAR_SET);
	printf("Plate_ID          %s \n",Plate_ID.c_str());
	
	memset(Plate_ID_tmp,0,20);
	memcpy(Plate_ID_tmp,Plate_ID.c_str(),Plate_ID.length());
	if(deploytable->GetIntValue((int)VDR_DEPLOY_PARA_PLATE_COLOR,Plate_Color)<0)
	{
		printf("get   VDR_DEPLOY_PARA_PLATE_COLOR wrong  \n");
		return -1;
	}

	if (Province_ID !=((paraset6->ProvienceID & 0xFFU) << 8 | (paraset6->ProvienceID & 0xFF00U) >> 8))
	{
		printf("Province_ID     %d \n",Province_ID);
		flag3=1;
	}
	if (City_ID !=((paraset6->CityID & 0xFFU) << 8 | (paraset6->CityID & 0xFF00U) >> 8))
	{
		printf("City_ID     %d \n",City_ID);
		flag3=1;
	}
	for (int i=0;i<20;i++)
	{
		if (Plate_ID_tmp[i]!=paraset6->Plate[i])
		{
			
			flag3=1;
			printf("Plate_ID_tmp  %02x    paraset6->Plate   %02x   i  %d \n",Plate_ID_tmp[i],  paraset6->Plate[i],i);
		}
	}
		printf("\n******************Plate_ID_tmp*****************\n");
	for (int i=0;i<20;i++)
	{
		printf("%02x ",Plate_ID_tmp[i]);
	}
	printf("\n*****************paraset6->Plate******************\n");

	for (int i=0;i<20;i++)
	{
		printf("%02x ",paraset6->Plate[i]);
	}
	printf("\n***********************************\n");
	if (Plate_Color != (int)paraset6->PlateColor)
	{
		printf("Plate_Color     %d \n",Plate_Color);
		flag3=1;
	}
	
	if(table->SetIntValue(VDR_PARA_HEART_BEAT_INTERVAL,ReverseBytes(paraset1->HeartInterval))<0)
	{
		printf("set   VDR_PARA_HEART_BEAT_INTERVAL wrong  \n");
		return -1;
	}
	if(table->SetIntValue((int)VDR_PARA_TCP_REPLY_TIMEOUT,ReverseBytes(paraset1->TcpTimeout))<0)
	{
		printf("set   VDR_PARA_TCP_REPLY_TIMEOUT wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_TCP_RETRY_TIMES,ReverseBytes(paraset1->TcpRetry))<0)
	{
		printf("set   VDR_PARA_TCP_RETRY_TIMES wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_UDP_REPLY_TIMEOUT,ReverseBytes(paraset1->UdpTimeout))<0)
	{
		printf("set   VDR_PARA_UDP_REPLY_TIMEOUT wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_UDP_RETRY_TIMES,ReverseBytes(paraset1->UdpRetry))<0)
	{
		printf("set   VDR_PARA_UDP_RETRY_TIMES wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_SMS_REPLY_TIMEOUT,ReverseBytes(paraset1->SMSTimeout))<0)
	{
		printf("set   VDR_PARA_SMS_REPLY_TIMEOUT wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_SMS_RETRY_TIMES,ReverseBytes(paraset1->SMSRetry))<0)
	{
		printf("set   VDR_PARA_SMS_RETRY_TIMES wrong  \n");
		return -1;
	}
	//string
	usleep(5*1000);
	printf("\n222222222222222222222\n");
	if(table->SetStringValue((int)VDR_PARA_MAIN_SERVER_APN,paraset1->Main_APN)<0)
	{
		printf("set   VDR_PARA_MAIN_SERVER_APN wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_MAIN_SERVER_USERNAME,paraset1->Main_APN_login)<0)
	{
		printf("set   VDR_PARA_MAIN_SERVER_USERNAME wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_MAIN_SERVER_PASSWORD,paraset1->Main_APN_SN)<0)
	{
		printf("set   VDR_PARA_MAIN_SERVER_PASSWORD wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_MAIN_SERVER_ADDRESS,paraset1->Main_Server)<0)
	{
		printf("set   VDR_PARA_MAIN_SERVER_ADDRESS wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_SECOND_SERVER_APN,paraset1->Second_APN)<0)
	{
		printf("set   VDR_PARA_SECOND_SERVER_APN wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_SECOND_SERVER_USERNAME,paraset1->Second_APN_login)<0)
	{
		printf("set   VDR_PARA_SECOND_SERVER_USERNAME wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_SECOND_SERVER_PASSWORD,paraset1->Second_APN_SN)<0)
	{
		printf("set   VDR_PARA_SECOND_SERVER_PASSWORD wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_SECOND_SERVER_ADDRESS,paraset1->Second_Server)<0)
	{
		printf("set   VDR_PARA_SECOND_SERVER_ADDRESS wrong  \n");
		return -1;
	}
	usleep(5*1000);
	//int 
	if(table->SetIntValue((int)VDR_PARA_TCP_PORT,ReverseBytes(paraset1->TCP_Port))<0)
	{
		printf("set   VDR_PARA_TCP_PORT wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_UDP_PORT,ReverseBytes(paraset1->UDP_Port))<0)
	{
		printf("set   VDR_PARA_UDP_PORT wrong  \n");
		return -1;
	}
	usleep(5*1000);
	//位置汇报策略
	if(table->SetIntValue((int)VDR_PARA_LOC_REPORT_POLICY,ReverseBytes(paraset4->Loc_Strategy))<0)
	{
		printf("set   VDR_PARA_LOC_REPORT_POLICY wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_LOC_REPORT_SCENARIO,ReverseBytes(paraset4->Loc_Plan))<0)
	{
		printf("set   VDR_PARA_LOC_REPORT_SCENARIO wrong  \n");
		return -1;
	}

	if(table->SetIntValue((int)VDR_PARA_NOLOGIN_REPORT_INTERVAL,ReverseBytes(paraset4->NoLoginLocInterval))<0)
	{
		printf("set   VDR_PARA_NOLOGIN_REPORT_INTERVAL wrong  \n");
		return -1;
	}

	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_SLEEP_REPORT_INTERVAL,ReverseBytes(paraset4->SleepLocInterval))<0)
	{
		printf("set   VDR_PARA_SLEEP_REPORT_INTERVAL wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_EMERGENCY_REPORT_INTERVAL,ReverseBytes(paraset4->EmergencyLocInterval))<0)
	{
		printf("set   VDR_PARA_EMERGENCY_REPORT_INTERVAL wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_DEFAULT_REPORT_INTERVAL,ReverseBytes(paraset4->DefaultLocInterval))<0)
	{
		printf("set   VDR_PARA_EMERGENCY_REPORT_INTERVAL wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_DEFAULT_REPORT_DISTANCE,ReverseBytes(paraset4->DefaultDistanceInterval))<0)
	{
		printf("set   VDR_PARA_DEFAULT_REPORT_DISTANCE wrong  \n");
		return -1;
	}
	usleep(5*1000);
	

	if(table->SetIntValue((int)VDR_PARA_NOLOGIN_REPORT_DISTANCE,ReverseBytes(paraset4->NoLoginDistanceInterval))<0)
	{
		printf("set   VDR_PARA_NOLOGIN_REPORT_DISTANCE wrong  \n");
		return -1;
	}
	if(table->SetIntValue((int)VDR_PARA_SLEEP_REPORT_DISTANCE,ReverseBytes(paraset4->SleepDistalceInterval))<0)
	{
		printf("set   VDR_PARA_NOLOGIN_REPORT_DISTANCE wrong  \n");
		return -1;
	}
	if(table->SetIntValue((int)VDR_PARA_EMERGENCY_REPORT_DISTANCE,ReverseBytes(paraset4->EmergencyDistanceInterval))<0)
	{
		printf("set   VDR_PARA_EMERGENCY_REPORT_DISTANCE wrong  \n");
		return -1;
	}
	if(table->SetIntValue((int)VDR_PARA_TURN_ANGLE,ReverseBytes(paraset4->Angle))<0)
	{
		printf("set   VDR_PARA_TURN_ANGLE wrong  \n");
		return -1;
	}







	usleep(5*1000);
	//string
	if(table->SetStringValue((int)VDR_PARA_PLATFORM_TEL,paraset5->HelpPhone)<0)
	{
		printf("set   VDR_PARA_PLATFORM_TEL wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_RESET_TEL,paraset5->ResetPhone)<0)
	{
		printf("set   VDR_PARA_RESET_TEL wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_FACTORY_RESET_TEL,paraset5->RestFactoryPhone)<0)
	{
		printf("set   VDR_PARA_PLATFORM_SMS wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_PLATFORM_SMS,paraset5->MonitorPlatform)<0)
	{
		printf("set   VDR_PARA_PLATFORM_SMS wrong  \n");
		return -1;
	}
	usleep(5*1000);

	if(table->SetStringValue((int)VDR_PARA_REPORT_SMS,paraset5->ReciveSMSPlatform)<0)
	{
		printf("set   VDR_PARA_PLATFORM_TEL wrong  \n");
		return -1;
	}

	// int 
	if(table->SetIntValue((int)VDR_PARA_CALL_ANSWER_POLICY,ReverseBytes(paraset6->AnswerStrategy))<0)
	{
		printf("set   VDR_PARA_CALL_ANSWER_POLICY wrong  \n");
		return -1;
	}
	if(table->SetIntValue((int)VDR_PARA_CALL_DURATION_ONCE,ReverseBytes(paraset6->LongAnswerTime))<0)
	{
		printf("set   VDR_PARA_CALL_DURATION_ONCE wrong  \n");
		return -1;
	}	
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_CALL_DURATION_MONTH,ReverseBytes(paraset6->MonthAnswerTime))<0)
	{
		printf("set   VDR_PARA_CALL_DURATION_MONTH wrong  \n");
		return -1;
	}
	//string 
	if(table->SetStringValue((int)VDR_PARA_MONITOR_TEL,paraset6->SetMonitorPhone)<0)
	{
		printf("set   VDR_PARA_PLATFORM_TEL wrong  \n");
		return -1;
	}	
	usleep(5*1000);
	if(table->SetStringValue((int)VDR_PARA_MONITOR_SMS,paraset6->MonitorPlatformSMSPhone)<0)
	{
		printf("set   VDR_PARA_PLATFORM_TEL wrong  \n");
		return -1;
	}
	// int usleep(5*1000);
	usleep(5*1000);
	
	if(table->SetIntValue((int)VDR_PARA_WARNING_MASK,ReverseBytes(paraset6->AlarmShielding))<0)
	{
		printf("set   VDR_PARA_WARNING_MASK wrong  \n");
		return -1;
	}	
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_WARNING_SMS_MASK,ReverseBytes(paraset6->SendSMSFlag))<0)
	{
		printf("set   VDR_PARA_WARNING_SMS_MASK wrong  \n");
		return -1;
	}	
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_WARNING_PHOTO_MASK,ReverseBytes(paraset6->AlarmShotFlag))<0)
	{
		printf("set   VDR_PARA_WARNING_PHOTO_MASK wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_WARNING_PHOTOSAVE_MASK,ReverseBytes(paraset6->AlarmSaveFlag))<0)
	{
		printf("set   VDR_PARA_WARNING_PHOTOSAVE_MASK wrong  \n");
		return -1;
	}	
	usleep(5*1000);

	if(table->SetIntValue((int)VDR_PARA_WARNING_KEY_MASK,ReverseBytes(paraset6->KeyAlarmFlag))<0)
	{
		printf("set   VDR_PARA_WARNING_KEY_MASK wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_MAX_SPEED,ReverseBytes(paraset6->HighSpeed))<0)
	{
		printf("set   VDR_PARA_MAX_SPEED wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_OVERSPEED_DURATION,ReverseBytes(paraset6->OverSpeedTime))<0)
	{
		printf("set   VDR_PARA_WARNING_KEY_MASK wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_CONT_DRIVE_THRESHOLD,ReverseBytes(paraset6->DriveTime))<0)
	{
		printf("set   VDR_PARA_WARNING_KEY_MASK wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_DAY_DRIVE_THRESHOLD,ReverseBytes(paraset6->DayCumulativeTime))<0)
	{
		printf("set   VDR_PARA_DAY_DRIVE_THRESHOLD wrong  \n");
		return -1;
	}	
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_REST_MIN,ReverseBytes(paraset6->LeastRestTime))<0)
	{
		printf("set   VDR_PARA_REST_MIN wrong  \n");
		return -1;
	}	
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_PARKING_MAX,ReverseBytes(paraset6->LongParkingTime))<0)
	{
		printf("set   VDR_PARA_PARKING_MAX wrong  \n");
		return -1;
	}
	usleep(5*1000);
	// photo quality
	if(table->SetIntValue((int)VDR_PARA_PHOTO_QUALITY,ReverseBytes(paraset6->PictureQuality))<0)
	{
		printf("set   VDR_PARA_PHOTO_QUALITY wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_PHOTO_BRIGHTNESS,ReverseBytes(paraset6->Light))<0)
	{
		printf("set   VDR_PARA_PHOTO_BRIGHTNESS wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_PHOTO_CONTRAST,ReverseBytes(paraset6->Contrast))<0)
	{
		printf("set   VDR_PARA_PHOTO_CONTRAST wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_PHOTO_SATUATION,ReverseBytes(paraset6->Satruation))<0)
	{
		printf("set   VDR_PARA_PHOTO_SATUATION wrong  \n");
		return -1;
	}
	
	if(table->SetIntValue((int)VDR_PARA_PHOTO_CHROMA,ReverseBytes(paraset6->chroma))<0)
	{
		printf("set   VDR_PARA_PHOTO_CHROMA wrong  \n");
		return -1;
	}
	// fence radius 
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_FENCE_RADIUS,((paraset3->FenceRadius & 0xFFU) << 8 | (paraset3->FenceRadius & 0xFF00U) >> 8))<0)
	{
		printf("set   VDR_PARA_FENCE_RADIUS wrong  \n");
		return -1;
	}
	// deploy  
	usleep(5*1000);
	if(deploytable->SetIntValue((int)VDR_DEPLOY_PARA_PROVINCE_ID,(paraset6->ProvienceID & 0xFFU) << 8 | (paraset6->ProvienceID & 0xFF00U) >> 8)<0)
	{
		printf("set   VDR_DEPLOY_PARA_PROVINCE_ID wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(deploytable->SetIntValue((int)VDR_DEPLOY_PARA_CITY_ID,(paraset6->CityID & 0xFFU) << 8 | (paraset6->CityID & 0xFF00U) >> 8)<0)
	{
		printf("set   VDR_DEPLOY_PARA_CITY_ID wrong  \n");
		return -1;
	}	
	usleep(5*1000);
	 string utf_value = boost::locale::conv::to_utf<char>(paraset6->Plate, JT808_CHAR_SET);
	if(deploytable->SetStringValue((int)VDR_DEPLOY_PARA_PLATE_ID,utf_value)<0)
	{
		printf("set   VDR_DEPLOY_PARA_PLATE_ID wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(deploytable->SetIntValue((int)VDR_DEPLOY_PARA_PLATE_COLOR,(paraset6->PlateColor))<0)
	{
		printf("set   VDR_DEPLOY_PARA_PLATE_COLOR wrong  \n");
		return -1;
	}
	//table    GNSS  setting

	if(table->SetIntValue((int)VDR_PARA_GNSS_LOC_MODE,(paraset6->GNSSmodel))<0)
	{
		printf("set   VDR_PARA_GNSS_LOC_MODE wrong  \n");
		return -1;
	}

	
	if(table->SetIntValue((int)VDR_PARA_GNSS_BAUD_RATE,(paraset6->GNSSBaudRate))<0)
	{
		printf("set   VDR_PARA_GNSS_BAUD_RATE wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_GNSS_DATA_OUT_RATE,(paraset6->GNSSOutFrequency))<0)
	{
		printf("set   VDR_PARA_GNSS_DATA_OUT_RATE wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_GNSS_DATA_COLLECT_RATE,ReverseBytes(paraset6->GNSSNMEAFrequency))<0)
	{
		printf("set   VDR_PARA_GNSS_DATA_COLLECT_RATE wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_GNSS_DATA_UPLOAD_MODE,(paraset6->GNSSPoistingMethod))<0)
	{
		printf("set   VDR_PARA_GNSS_DATA_UPLOAD_MODE wrong  \n");
		return -1;
	}
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_GNSS_DATA_UPLOAD_SET,ReverseBytes(paraset6->GNSSTime))<0)
	{
		printf("set   VDR_PARA_GNSS_DATA_UPLOAD_SET wrong  \n");
		return -1;
	}
	// can  setting 
	usleep(5*1000);
	if(table->SetIntValue((int)VDR_PARA_CAN_1_COLLECT_INTERVAL,ReverseBytes(paraset6->CAN1CollectTime))<0)
	{
		printf("set   VDR_PARA_CAN_1_COLLECT_INTERVAL wrong  \n");
		return -1;
	}
	if(table->SetIntValue((int)VDR_PARA_CAN_1_UPLOAD_INTERVAL,((paraset6->CAN1UploadTime & 0xFFU) << 8 | (paraset6->CAN1UploadTime & 0xFF00U) >> 8))<0)
	{
		printf("set   VDR_PARA_CAN_1_UPLOAD_INTERVAL wrong  \n");
		return -1;
	}
	if(table->SetIntValue((int)VDR_PARA_CAN_2_COLLECT_INTERVAL,ReverseBytes((short)paraset6->CAN2CollectTime))<0)
	{
		printf("set   VDR_PARA_CAN_2_COLLECT_INTERVAL wrong  \n");
		return -1;
	}
	if(table->SetIntValue((int)VDR_PARA_CAN_2_UPLOAD_INTERVAL,((paraset6->CAN2UploadTime & 0xFFU) << 8 | (paraset6->CAN2UploadTime & 0xFF00U) >> 8))<0)
	{
		printf("set   VDR_PARA_CAN_2_UPLOAD_INTERVAL wrong  \n");
		return -1;
	}
	
/*
	if(table->SetIntValue((int)VDR_PARA_CAN_ID_COLLECT_SET,paraset->CANSetting)<0)
	{
		printf("set   VDR_PARA_CAN_ID_COLLECT_SET wrong  \n");
		return -1;
	}
	*/

	printf("11111111111111111111111111111111111111111111111111111111  end \n");
	table->Commit();
	deploytable->Commit();
	delete table;
	delete deploytable;
	for (int i=0;i<30;i++)
	{
		if (Main_Server_To_Mcu[i]!=paraset1->Main_Server[i])
		{
			flag=1;
		}
	}
	for (int i=0;i<30;i++)
	{
		if (Second_Server_To_Mcu[i]!=paraset1->Second_Server[i])
		{
			flag=0;
		}
	}

	if (Port_To_Mcu != ReverseBytes(paraset1->TCP_Port))
	{
		flag=1;
		if (ReverseBytes(paraset1->TCP_Port)<0)
			flag=0;
		printf("Port_To_Mcu  %d  paraset1->TCP_Port  %d \n ",Port_To_Mcu,ReverseBytes(paraset1->TCP_Port));
	}
	if (heart_retry_time != ReverseBytes(paraset1->HeartInterval))
	{
		printf("HeartInterval  %d  heart_retry_time  %d \n",ReverseBytes(paraset1->HeartInterval),heart_retry_time);
		flag=2;
	}
	
	printf("flag===========%d =====flag3==============================================================%d \n",flag,flag3);
	if (flag==1||flag3==1)
	{
			ConnectionManager::Instance()->NetBreak(1);
			RefreshConfig();
			m_registered = false;
            m_authorized = false;
         	 string auth="";
            VDRDeployParaTable *table = new VDRDeployParaTable();
            if(table->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth) < 0)
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
                delete table;
                return -1 ;
            }
            table->Commit();
            delete table;
			ConnectionManager::Instance()->Notify(ENUM_CONNECTION_STATE_DISCONNECTED);	
			ConnectionManager::Instance()->Reconnect();
	}
	if (flag3==1)
	{
		string auth=""; 
		VDRBaseConfigTable *baseconfig = new VDRBaseConfigTable();
		if(baseconfig->SetStringValue((int)VDR_BASE_STATUS_D7, auth) < 0)
		{
			LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult save auth token failed");
			delete baseconfig;
			return -1;
		}
		baseconfig->Commit();
		delete baseconfig;
		
		printf("2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222\n");
	}
	if (flag==2)
	{
		RefreshConfig();
	}
	int flag1=0;
	flag1=flag1|0x03;
	SendJT808SettingUpdateNotify(flag1);
	SendJT808GnssUpdateNotify(paraset6->GNSSmodel,paraset6->GNSSBaudRate,paraset6->GNSSOutFrequency,paraset6->GNSSNMEAFrequency);
	//delete accelerationtable;
	//delete baseconfigtable;
#endif
}
void  JT808MessageHandler::HandleEnterDeadZone()
{
	SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_DISCONNECT);
    StopTimer(m_helloTimer);
	StopTimer(m_retryTimer);
}



