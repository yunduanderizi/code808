#include "JT8082DbusMessage.h"
#include "JT808MessageHelper.h"
#include "../jt808/JT808DbusMessage.h"
#include "LogUtility.h"
#include "VDRRunParaTable.h"
#include "VDRDeployParaTable.h"
#include "ConnectionManager.h"
#include "JT808DbusMessageHandler.h"
#include "NaviDbusMessage.h"
#include "ConfigManager.h"
#include <stdlib.h>
#include <map>
#include "McuMessageHandler.h"
#include "JT808MessageHelper.h"
#define NHB
using namespace std;
extern JT808MessageHandler jt808;

extern DBusServerC    *g_pDbusServer;
#if 0
void JT808RegisterServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
	LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::JT808RegisterServiceHandler get register request");

	JT808RegisterDbusReqC   *pReq = (JT808RegisterDbusReqC *)request;
	if(pReq -> result == 0)
	{
		JT808MessageHelper::SendLogout();

	}else if(pReq -> result == 1){
		jt808.HandleRegister();
		//JT808MessageHelper::SendRegister(1, pReq -> buf);
		JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
		reply = pReply;
	}else if(pReq -> result == 2){
		JT808MessageHelper::SendRegister(2, pReq -> buf);
		JT808RegisterDbusRepC *pReply = new JT808RegisterDbusRepC();
		reply = pReply;
	}
    return;
}
#if 1

#endif
void JT808AuthenticationServiceHandler(DBusRequestC *, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808MessageHelper::JT808AuthenticationServiceHandler get register request");
    JT808MessageHelper::SendAuth();

    JT808AuthenticationDbusRepC *pReply=new JT808AuthenticationDbusRepC(); 
    reply = pReply;

    return;
}

void JT808AnswerServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808AnswerServiceHandler get answer request");
    JT808AnswerDbusReqC *pReq = (JT808AnswerDbusReqC *)request;
    
    JT808MessageHelper::SendAnswer((unsigned short)pReq->m_seq, (unsigned short)pReq->m_answer_id,pReq->time);

    JT808AnswerDbusRepC *pReply = new JT808AnswerDbusRepC();
    reply = pReply;

    return;
}
void JT808DriverInfoServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808DriverInfoServiceHandler get driver info request");
    JT808DriverInfoReqC *pReq = (JT808DriverInfoReqC *)request;
#if 0
    //turn on when go to beisanhuan
    //20130802
    while(1){
	    if ((jt808.m_authorized == true)&&( ConnectionManager::Instance()-> m_connId == ENUM_CONNECTION_ID_MASTER ))
	    {
		    JT808MessageHelper::SendDriverInfo(pReq->m_time_index);
		    return;
	    }
	    else 
		    sleep(1);
    }
#endif
    JT808MessageHelper::SendDriverInfo(pReq->m_time_index);
    
    JT808DriverInfoDbusRepC *pReply = new JT808DriverInfoDbusRepC();
    reply = pReply;
    
    return;
}
void JT808CameraServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808CameraServiceHandler get camera request");
    JT808CameraDbusReqC *pReq = (JT808CameraDbusReqC *)request;

    JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_PHOTO, pReq->m_tableIndex, pReq->trans_flag);

    JT808CameraDbusRepC *pReply = new JT808CameraDbusRepC();
    reply = pReply;
    
    return;
}
void JT808EventServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    static unsigned char up_event[]={'1','2','3','4'}; 
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808EventServiceHandler request");
 //   JT808CameraDbusReqC *pReq = (JT808CameraDbusReqC *)request;
    JT808EventReportDbusReqC *pReq = (JT808EventReportDbusReqC *)request;
    JT808MessageHelper::SendEvents(pReq->buff, pReq->sum);
    JT808EventReportDbusRepC *pReply = new JT808EventReportDbusRepC();
    reply = pReply;
    
    return;
}
void JT808VideoServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808CameraServiceHandler get camera request");
    JT808VideoDbusReqC *pReq = (JT808VideoDbusReqC *)request;

    JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_VIDEO, pReq->m_tableIndex,1);
    
    return;
}
void JT808RecordServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808RecordServiceHandler get record request");
    JT808RecordDbusReqC *pReq = (JT808RecordDbusReqC *)request;

    JT808MessageHelper::SendMedia(ENUM_JT808_MEDIA_AUDIO, pReq->m_tableIndex, 1);
    
    JT808RecordDbusRepC *pReply = new JT808RecordDbusRepC();
    reply = pReply;
    
    return;
}

void JT808ModifySettingServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    VDRRunParaTable *runtable = new VDRRunParaTable();
    VDRDeployParaTable *deploytable = new VDRDeployParaTable();
    int flag1 = false, flag2 = false;
    int ret = 0;
    int err = 0;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT808ModifySettingServiceHandler get modify setting request");
    JT808ModifySettingReqC *pReq = (JT808ModifySettingReqC *)request;

    if(pReq->m_main_server)
    {
        string main_server = pReq->m_main_server;
        string old_main_server;
        runtable->GetStringValue((int)VDR_PARA_MAIN_SERVER_ADDRESS, old_main_server);
        if(main_server != old_main_server)
        {
            runtable->SetStringValue((int)VDR_PARA_MAIN_SERVER_ADDRESS, main_server);
            flag1 = true;
        }
    }

    if(pReq->m_second_server)
    {
        string second_server = pReq->m_second_server;
        string old_second_server;
        runtable->GetStringValue((int)VDR_PARA_SECOND_SERVER_ADDRESS, old_second_server);
        if(second_server != old_second_server)
        {
            runtable->SetStringValue((int)VDR_PARA_SECOND_SERVER_ADDRESS, second_server);
            flag1 = true;
        }
    }
	if (pReq->m_apn)
	{
		string m_apn = pReq->m_apn;
        string m_apn_old;
        runtable->GetStringValue((int)VDR_PARA_MAIN_SERVER_APN, m_apn_old);
        if(m_apn != m_apn_old)
        {
            runtable->SetStringValue((int)VDR_PARA_MAIN_SERVER_APN, m_apn);
            flag1 = true;
        }
	}
    if(pReq->m_port >= 0)
    {
        int port = pReq->m_port;
        int old_port;
        runtable->GetIntValue((int)VDR_PARA_TCP_PORT, old_port);
        if(port != old_port)
        {
            runtable->SetIntValue((int)VDR_PARA_TCP_PORT, port);
            flag1 = true;
        }
    }
    if(pReq->m_port2 >= 0)
    {
    	int port2 = pReq->m_port2;
	int old_port2;
	runtable->GetIntValue((int)VDR_PARA_UDP_PORT,old_port2);
	if(port2 != old_port2)
	{
	    runtable->SetIntValue((int)VDR_PARA_UDP_PORT,port2);
	    flag1 = true;
	}
    }    
    if(flag1)
    {
        ret = runtable->Commit();
        if(ret < 0)
        {
            err = 1;
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808ModifySettingServiceHandler commit runtable failed");
        }
    }

    if(pReq->m_auth)
    {
        string auth = pReq->m_auth;
        string old_auth;
        deploytable->GetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, old_auth);
        if(auth != old_auth)
        {
            deploytable->SetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, auth);
            flag2 = true;
        }
    }

    if(flag2)
    {
        ret = deploytable->Commit();
        if(ret < 0)
        {
            err = 1;
            LogUtility::Log(LOG_LEVEL_ERROR, "JT808ModifySettingServiceHandler commit deploytable failed");
        }
    }

    JT808ModifySettingDbusRepC *pReply = new JT808ModifySettingDbusRepC();
    pReply->m_result = err;
    reply = pReply;

    delete runtable;
    delete deploytable;
    
    if(flag1 || flag2)
    {
		 ConfigManager::Refresh();
		 jt808.GetPhoneNumber();
		 ConnectionManager::Instance()->Notify(ENUM_CONNECTION_STATE_DISCONNECTED);	
		ConnectionManager::Instance()->Reconnect();
    }
    
    return;
}



void SendJT808RegisterNotify(int result)
{
    JT808RegisterDbusNotifyC notify;
    
    notify.result = result;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808AuthenticationNotify(int result)
{
    JT808AuthenticationDbusNotifyC notify;
    
    notify.result = result;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}
void SendJT808ConncetionStatusNotify(int main_status, int second_status)
{
    JT808ConnStatusDbusNotifyC notify;
    
    notify.m_main_status = main_status;
    notify.m_second_status = second_status;
    
   g_pDbusServer->sendNotify(&notify);
        
    return;
}
void SendJT808CameraStartNotify(StruCameraCommand &command)
{
   printf("camera :::::::::::::::: flag is %d\n ",command.flag);
    JT808CameraDbusNotifyC notify;
    
    notify.m_channel = command.chan;
    notify.m_command = command.command;
    notify.m_interval = command.interval;
    notify.m_flag = command.flag;
    notify.m_resolution = command.resolution;
    notify.m_quality = command.quality;
    notify.m_brightness = command.brightness;
    notify.m_saturation = command.saturation;
    notify.m_chroma = command.chroma;
    
    g_pDbusServer->sendNotify(&notify);
        
    return;
}
void SendJT808RecordStartNotify(StruRecordCommand &command)
{
    JT808RecordDbusNotifyC notify;
    
    notify.m_command = command.command;
    notify.m_time = command.duration;
    notify.m_flag = command.flag;
    notify.m_sample = command.sample;
    
    g_pDbusServer->sendNotify(&notify);
        
    return;
}
void RebootMachineHandler(DBusRequestC *request, DBusReplyC *&reply)
{
	if(request->getIdentifier()!=JT808_DBUS_REQ_ID_REBOOT)
        return;
	jt808.HandleShutDownReboot(2);
}
void ShutdownMachineHandler(DBusRequestC *request, DBusReplyC *&reply)
{
	if(request->getIdentifier()!=JT808_DBUS_REQ_ID_SHUTDOWN)
        return;
	printf("THE MACHINE IS GOING SHUTDOWN !\n");
	jt808.HandleShutDownReboot(3);

}
void SendJT808PolygonFenceUpdateNotify()
{
    JT808PolygonFenceUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808LinesFenceUpdateNotify()
{
    JT808LinesFenceUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808CircleFenceUpdateNotify()
{
    JT808CircleFenceUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808RectangleFenceUpdateNotify()
{
    JT808RectangleFenceUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

#endif
void SendJT808ConncetionStatusNotify(int main_status, int second_status)
{
    JT8082ConnStatusDbusNotifyC notify;
    
    notify.m_main_status = main_status;
    notify.m_second_status = second_status;
    
    g_pDbusServer->sendNotify(&notify);
    return;
}
void ShutdownMachineHandler(DBusRequestC *request, DBusReplyC *&reply)
{
	if(request->getIdentifier()!=JT808_DBUS_REQ_ID_SHUTDOWN)
        return;
	printf("THE MACHINE IS GOING SHUTDOWN !\n");
	jt808.HandleShutDownReboot(3);

}

void JT808InfomationDemandHandler(DBusRequestC *request,DBusReplyC *&reply)
{
	LogUtility::Log(LOG_LEVEL_DEBUG,"JT808MessageHelper::JT808InfomationDemandHandler get request");
	JT8082InfomationDbusReqC *pReq = (JT8082InfomationDbusReqC *)request;
	JT808MessageHelper::HandleInfomationDemand(pReq->sum,pReq->flag,pReq->buff);
	JT8082InfomationDbusRepC *pReply = new JT8082InfomationDbusRepC();
	reply=pReply;
	return;
}
void SendJT808SettingUpdateNotify(int result)
{
    printf("fei -----------------------------SendJT808SettingUpdateNotify\n");
    JT8082SettingUpdateDbusNotifyC notify;
    notify.flag = result;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}
void JT808OverDormancy(DBusRequestC *request, DBusReplyC *&reply)
{
	
}
void JT808TransmissionDataServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "JT8082TransmitServiceHandler get record request");
    JT8082DataTransmitReqC *pReq = (JT8082DataTransmitReqC *)request;
    if( pReq->type != 0x0B ){
	    if(pReq->type==0xF8)
	    {
	    	    JT808MessageHelper::SendTransmissionData_can(0xF8, pReq->total,pReq->buf);	    
	    }
	    else
	    {
		    JT808MessageHelper::SendTransmissionData(pReq->type, pReq->total, pReq->buf);
	    }
    }else{
#if 1
	    if( ConfigManager::m_control != 2)
	    {
		    ConfigManager::m_control = 2;
		    ConnectionManager::Instance()->Close();
	    }
	    while(1){
		    if (( ConnectionManager::Instance()-> m_connId == ENUM_CONNECTION_ID_IC )&&( ConnectionManager::Instance()->m_connected == true))
		    {
			printf("%s::%d trans info to ic server!!!!\n",__FILE__,__LINE__);
			JT808MessageHelper::SendTransmissionData(0x0B,  pReq->total, pReq->buf);
			break;
		    }else 
			    sleep(1);
	    }

#endif
#if 0
	    try{
		    int i;
		    TCPSocket sock(ConfigManager::m_icmainServerAddress.c_str(),ConfigManager::m_ictcpPort);
		    StruJT808Packet *thepacket = jt808.CreatePacket();
		    JT808Packet &packet = thepacket->packet;
		    jt808.PushByte(packet, 0x0B);
		    for(i=0;i<pReq->total;i++)
			    jt808.PushByte(packet, pReq->buf[i]);
		    jt808.PacketHead(thepacket, (unsigned short)JT808_PACKET_UP_TRANSPARENT_TRANSMISSION, JT808_CRYTO_NONE);
		    jt808.PacketCheckSumFlag(thepacket);
		    JT808DataTransmitNotifyC notify;
		    sock.send(pReq->buf, pReq->total);
		    notify.type = 0x0B;
		    notify.total= sock.recv(notify.buf,sizeof(notify.buf)-1);
		    if(notify.total > 0)
			    g_pDbusServer->sendNotify(&notify);
	    }catch(SocketException &e){
		    cerr << e.what() <<endl;
	    }
#endif
    }
    return;
}
void SendJT808LocReportNotify(int confirm)
{
    printf("xie------------------SendJT808LocReportNotify ---------------\n");
    JT8082LocReportDbusNotifyC notify;
    notify.confirm_alarm=confirm;
    g_pDbusServer->sendNotify(&notify);
    return;
}

void SendJT808TempLocReportNotify(int interval,int validity)
{
    JT8082TempLocReportDbusNotifyC notify;

    notify.interval=interval;
    notify.validity=validity;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808TextNotify(int emergency, int lcd, int tts, int bigscreen, int naviorcan, string &text)
{

     printf("fei------------8082 -------------SendJT808TextNotify\n");
    JT8082TextDbusNotifyC notify;
    
    notify.m_emergency = emergency;
    notify.m_lcd = lcd;
    notify.m_tts = tts;
    notify.m_bigscreen = bigscreen;
    notify.m_naviorcan= naviorcan; 
    notify.m_text = (char *)malloc(text.length() + 1);
    
    text.copy(notify.m_text, text.length());
    notify.m_text[text.length()] = '\0';
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}
void SendJT808InfomationNotify(int type, int len, string &contents)
{
        printf("fei ---------------- 8082 SendJT808InfomationNotify\n");
	JT8082InfomationDbusNotifyC notify;
	notify.m_type = type;
	notify.m_len = len;
	notify.m_contents = (char *)malloc(len+1);

	contents.copy(notify.m_contents, len);
	notify.m_contents[len] = '\0';

	g_pDbusServer->sendNotify(&notify);
	return;
}
void SendJT808QuestionNotify(int itime,int seq, int emergency, int lcd, int tts, int bigscreen, string &text, map<int, string> &answers)
{
    JT8082QuestionDbusNotifyC notify;
	notify.itime = itime;
    notify.m_seq = seq;
    notify.m_emergency = emergency;
    notify.m_lcd = lcd;
    notify.m_tts = tts;
    notify.m_bigscreen = bigscreen;
    notify.m_text = (char *)malloc(text.length() + 1);
    text.copy(notify.m_text, text.length());
    notify.m_text[text.length()] = '\0';
    
    int answerNumber = answers.size();
    if(answerNumber > JT808_MAX_QUESTION_NUMBER)
    {
        answerNumber = JT808_MAX_QUESTION_NUMBER;
    }
    
    notify.m_answerNumber = answerNumber;
    int count = 0;
    
    for(map<int, string>::iterator it = answers.begin(); it != answers.end(); it ++, count ++)
    {
        if(count == 0)
        {
            notify.m_answer1_id = it->first;
            string &answer = it->second;
            notify.m_answer1 = (char *)malloc(answer.length() + 1);
            answer.copy(notify.m_answer1, answer.length());
            notify.m_answer1[answer.length()] = '\0';
        }
        else if(count == 1)
        {
            notify.m_answer2_id = it->first;
            string &answer = it->second;
            notify.m_answer2 = (char *)malloc(answer.length() + 1);
            answer.copy(notify.m_answer2, answer.length());
            notify.m_answer2[answer.length()] = '\0';
        }
        else if(count == 2)
        {
            notify.m_answer3_id = it->first;
            string &answer = it->second;
            notify.m_answer3 = (char *)malloc(answer.length() + 1);
            answer.copy(notify.m_answer3, answer.length());
            notify.m_answer3[answer.length()] = '\0';
        }
        else if(count == 3)
        {
            notify.m_answer4_id = it->first;
            string &answer = it->second;
            notify.m_answer4 = (char *)malloc(answer.length() + 1);
            answer.copy(notify.m_answer4, answer.length());
            notify.m_answer4[answer.length()] = '\0';
        }
    }
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808CallBackNotify(int monitor, string &phone)
{
    JT8082CallBackDbusNotifyC notify;
    
    notify.m_monitor = monitor;
    notify.m_phone = (char *)malloc(phone.length() + 1);
    
    phone.copy(notify.m_phone, phone.length());
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808ContactsUpdateNotify()
{
    JT8082ContactsUpdateDbusNotifyC notify;
    
    g_pDbusServer->sendNotify(&notify);
    
    return;
}

void SendJT808EventSetNotify()
{
	JT8082EventSetDbusNotifyC notify;

	g_pDbusServer->sendNotify(&notify);

	return;
}
void SendJT808InfomationDemandNotify()
{
	JT8082InfomationDemandDbusNotifyC notify;
	
	g_pDbusServer->sendNotify(&notify);
	
	return;
}
void SendJT808FenceUpdateNotify(int flag,int type,int id,int state)
{

     printf("fei---------8082-----------------SendJT808FenceUpdateNotify\n ");
    //CNaviElectronicFenceNotifyC notify;
    JT8082NaviElectronicFenceNotifyC  notify;
    notify.eflag=flag;
    notify.etype=type;
    notify.eid=id;
    notify.estate=state;

    g_pDbusServer->sendNotify(&notify);
    
    return;
}


void SendJT808ParkExtendNotify(int flag, int value)
{
    JT8082ParkingDbusNotifyC notify;
    notify.flag = flag;
    notify.value = value;
    g_pDbusServer->sendNotify(&notify);
    return;
}


void JTT808ChangeRegisterPar()
{
	jt808.HandleRegister();
}

void SendJT808GnssUpdateNotify(int mode,int baud,int out,int fre)
{
    JT8082GNSSAttrChangeNotifyC notify;
    notify.mode=mode;
    notify.baudrate=baud;
    notify.outfrequency=out;
    notify.samplerate=fre;
     
    g_pDbusServer->sendNotify(&notify);
    
    return;
}
void HandleGuiEvent(DBusRequestC *request, DBusReplyC *&reply)
{
	JT8082CommonEventReqC *pReq = (JT8082CommonEventReqC *)request;
	printf("HandleGuiEvent::pReq->type=%d \n",pReq->type);
	if (pReq->type==1)
	{
		JT808MessageHelper::HandleSystemRestFactory();
	}
	if(pReq->type==2)
	{
		jt808.GetPhoneNumber();
		
	}
	if(pReq->type ==3)
	{
		JT808MessageHelper::HandleChangeSecondService();	
	}
	JT8082CommonEventRepC *pReply = new JT8082CommonEventRepC();
	pReply->m_result=0;
	reply = pReply;
	return;
}

