#include <boost/program_options.hpp>
#include <iostream>
#include <dbusClient.h>
#include "LogUtility.h"
#include "ConfigManager.h"
#include "ConnectionManager.h"
#include "JT808MessageHandler.h"
#include "JT808MessageHelper.h"
#include "JT808DbusMessage.h"
#include "../second_jt808/JT8082DbusMessage.h"
#include "JT808DbusMessageHandler.h"
#include "DeviceManagerDbusMessage.h"
#include "../busdaemon/busmessage.h"
#include "../deviceManager/devicemanager.h"
#include "../wm/dbusClientProxy.h"
#include "NaviDbusMessage.h"
#include "VDRQuestionTable.h"
#include "JT808GnssManager.h"
#include "../tables/BusStationTable.h"
#include "BusParaTable.h"
#include <boost/locale.hpp> 
#include "McuProtocol.h"
#include "McuMessageHandler.h"
#include "McuProtocol.h"
#include <time.h>
#include "GeneralTimer.h"
#include <string.h>
using namespace std;

extern DBusServerC    *g_pDbusServer;

#define SYSTEMOFF 1
#define NEW_310
extern int TimeToDev;
extern unsigned short loc_retry_time;
extern unsigned int hearttimelast;
extern unsigned int loctimelast;
extern unsigned char  loc_buf_last[90];
extern unsigned char  heart_buf_last[15];
extern unsigned short seq_last;
extern unsigned int START_UP_CAUSE;
extern unsigned char connect_tmp;

McuMesageHandler mcumessagehandle;
int DbusSystemOff=1;
int DeadBlock=0; //0 非盲区1 8105下发盲区
int DeadBlock_flag=0;// 1 盲区进入链接线程
namespace po = boost::program_options;
extern unsigned short tmp_seq;
unsigned int  AccStatus=0;//0 接收命令1 断网重启 2  正常3 休眠启动
GeneralTimerItem *systemoff;

static int socket_recvfd;
static int socket_dp;
static int socket_gps;
int  mangqu=0;
int gnss_set_flag=1;
int SendAuth_flag_tmp=1;
int enter_mangqu=0;
static int gnss_flag = 0;
volatile int  mangqu_flag = 0; // mangqu jin chu  biaoji  
volatile int fence_inout_flag =0;   // In or  out  fence  flag  
struct sockaddr_in addr_recv;
struct sockaddr_in addr_gnss;
struct sockaddr_in addr_gps;
JT808MessageHandler jt808;
pthread_mutex_t  FenceMutex;
static int ProcessOptions(int argc, char *argv[])
{
    try {

        po::options_description options("Allowed options");
        options.add_options()
            ("help", "produce help message")
            ("nohello", "no hello send")
            ("noauth", "no auth send")
            ("debug", po::value<int>(), "set debug level")
            ("log", po::value<int>(), "set log level")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(argc, argv, options), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << options << "\n";
            return 1;
        }

        if (vm.count("nohello")) {
            ConfigManager::SetHelloFlag(false);
        }

        if (vm.count("noauth")) {
            ConfigManager::SetAuthFlag(false);
        }
        
        if (vm.count("debug")) {
            cout << "debug level was set to " 
                 << vm["debug"].as<int>() << ".\n";
            LogUtility::SetStdoutLevel(vm["debug"].as<int>());
        } else {
            cout << "debug level was not set.\n";
        }
        if (vm.count("log")) {
            cout << "log level was set to " 
                 << vm["log"].as<int>() << ".\n";
            LogUtility::SetLogLevel(vm["log"].as<int>());
        } else {
            cout << "log level was not set.\n";
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}

#define MAX_PACKET 26 
struct LocStatusData locData[MAX_PACKET];
//GPS recv thread
void *gps_recv_thread(void *param)
{
	int n, i=0, m=0, j=0;
	// i count packet numbers
	struct sockaddr client;
	int len = sizeof(struct sockaddr);
	while(1)
	{
		n = recvfrom(socket_gps, &locData[i], sizeof(LocStatusData), 0, &client, (socklen_t *)&len);
		printf("gps_recv_thread 111111111111111111111111111111===========data==========%d \n",n);
		printf("ConnectionManager::Instance()->b_zone %d mangqu %d fence_inout_flag %d mangqu_flag %d  ",ConnectionManager::Instance()->b_zone, mangqu,fence_inout_flag,mangqu_flag);
		if(n > 0)
		{
			if (DeadBlock==1)
			{
				printf("j===============%d \n",j);
				if (ConnectionManager::Instance()->b_zone == true)
				{
					j++;
				}
				else if (ConnectionManager::Instance()->b_zone == false)
				{
						JT808MessageHelper::SendLocationMulti(locData, j);
						DeadBlock=0;
				}		
			}
			else 
			{
					if((ConnectionManager::Instance()->b_zone == false)&& i>0)     // Communication start
					{
						JT808MessageHelper::SendLocationbypass(locData[i]);  //0200
						JT808MessageHelper::SendLocationMulti(locData, i);     //0704
						i=0;
						mangqu = 0;
					}
					else if((ConnectionManager::Instance()->b_zone == true) &&mangqu_flag == 1)  //Communication close
					{
						if( fence_inout_flag == 1 )
						{
							JT808MessageHelper::SendLocationbypass(locData[0]);
							jt808.HandleEnterDeadZone();
							sleep(20);
							ConnectionManager::Instance()->Notify(ENUM_CONNECTION_STATE_DISCONNECTED);
							ConnectionManager::Instance()->NetBreak(1);
							enter_mangqu=1;
							pthread_mutex_lock(&FenceMutex);
							fence_inout_flag = 0;
							printf("**************************mangqu == 1&& fence_inout_flag ==1 fenc_inout_flag= %d\n",fence_inout_flag);
							pthread_mutex_unlock(&FenceMutex);
							continue;
						}
						i++;
						printf("i=================================================================% d\n",i);

						if(i >= MAX_PACKET-1)
						{
							JT808MessageHelper::SendLocationMulti(locData, i);
							i = 0;
						}
					}
					else
					{
						if(mangqu == 0)
						{
							JT808MessageHelper::SendLocationbypass(locData[0]);
							if(gnss_flag == 1)
								g_JT808GnssManager.SetGnssUp( ConfigManager::m_iGNSSUpLoadMode, ConfigManager::m_iGNSSUpLoadSet );
						}
					}
				}
			}
    }
    return NULL;
}

int gps_socket_recv()
{
    int port =  8901;
    int ret;

    bzero(&addr_gps, sizeof(addr_gps));
    addr_gps.sin_family = AF_INET;
    addr_gps.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_gps.sin_port = htons(port);

    socket_gps = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_gps == -1)
    {
        perror("socket call failed");
        return -1;
    }

    bind(socket_gps, (struct sockaddr*)&addr_gps, sizeof(addr_gps));

    pthread_t sock_recv;
    ret = pthread_create(&sock_recv, NULL, gps_recv_thread, NULL);
    if(ret < 0)
    {
	    printf("create handle_image error!\n");
	    return ret;
    }
    pthread_detach(sock_recv);

    return 0;
}

void *can_recv_thread(void *param)
{ 
    fd_set rfds;
    struct timeval tv;
    int retval;

    struct sockaddr client;
    int n;
    unsigned char buff[1024];

    int len = sizeof(struct sockaddr);
    while(1)
    {
		    FD_ZERO(&rfds);
		    FD_SET(socket_recvfd, &rfds);

		    tv.tv_sec = 0;
		    tv.tv_usec = 50*1000;

		    retval = select(socket_recvfd + 1, &rfds, NULL, NULL, &tv);

		    if (retval == -1)
		    {
			    perror("select()");
			    continue;
		    }  
		    else if (retval)
		    {
			    n = recvfrom(socket_recvfd, buff, 1024, 0, &client, (socklen_t *)&len);
			    printf("buf: %s \n", buff);
			    JT808MessageHelper::SendCanData(0x00 , n, buff);
		    }
		    else
			    sleep(1);
     }
    return NULL;
}

int can_socket_recv()
{
    socket_recvfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_recvfd == -1)
    {
        perror("socket call failed");
        return -1;
    }

    int port_recv = 2010;
    bzero(&addr_recv, sizeof(addr_recv));
    addr_recv.sin_family = AF_INET;
    addr_recv.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_recv.sin_port = htons(port_recv);

    bind(socket_recvfd, (struct sockaddr*)&addr_recv, sizeof(addr_recv));

    int ret = -1;
    pthread_t sock_recv;
    ret = pthread_create(&sock_recv, NULL, can_recv_thread, (void *)&addr_recv);
    if(ret < 0)
    {
    printf("create handle_image error!\n");
    return ret;
    }
    pthread_detach(sock_recv);

    return 0;
} 



void BusDCanNotifyHandler(DBusNotifyC *notify)
{
    if(notify->getIdentifier()!=BUSD_DBUS_NTF_ID_CARINFO_UPLOAD)
        return;
    
    BUSDCarINfoUpdateDbusNotifyC *cannotify=(BUSDCarINfoUpdateDbusNotifyC*)notify;
    
    JT808MessageHelper::SendBusDDriverData(cannotify->m_filename,cannotify->m_itime,cannotify->m_num);
}
void BusDPosNotifyHandler(DBusNotifyC *notify)
{
    if(notify->getIdentifier()!=BUSD_DBUS_NTF_ID_POSINFO_UPLOAD)
        return;

    BUSDPosInfoUpdateDbusNotifyC *posnotify=(BUSDPosInfoUpdateDbusNotifyC*)notify;
    
    JT808MessageHelper::SendBusDPosData(posnotify->m_filename);
}
void BusDStationPassengerHandler(DBusNotifyC *notify)
{
    if(notify->getIdentifier()!=BUSD_DBUS_NTF_ID_STATION_PASSENGER_UPDATE)
        return;

    BUSDStationPassengerDbusNotifyC *spnotify=(BUSDStationPassengerDbusNotifyC*)notify;
    
    JT808MessageHelper::SendBusDPassengerData(spnotify->m_time,spnotify->m_number,spnotify->m_name,
        spnotify->m_upNumber,spnotify->m_downNumber);
}

void CNaviElectronicFenceInorOutNotifyHandler(DBusNotifyC *pNotify)
{
	int gnss_tmp ;
	if(pNotify->getIdentifier() != DEVICE_MANAGER_NOTF_ID_FENCE_INOUT)
		return;
	DeviceFenceInOutDbusNotifyC *Notify= (DeviceFenceInOutDbusNotifyC *)pNotify;
	printf("CNaviElectronicFenceInorOutNotifyHandler *******************8 Notify->gnss_flag=%d flag %d Notify->cm_flag  %d ",Notify->gnss_flag,Notify->flag,Notify->cm_flag );
	printf("ConnectionManager::Instance()->b_zone == %d \n",ConnectionManager::Instance()->b_zone);
	
	if(Notify->flag == 1) // 进区域
	{
		gnss_set_flag= Notify->gnss_flag;
		if(Notify->gnss_flag == 1)    // gnss  采集
		{
			//gnss_tmp = 2;
			g_JT808GnssManager.SetGnssUp( ConfigManager::m_iGNSSUpLoadMode, ConfigManager::m_iGNSSUpLoadSet );
		}
		else    //gnss  不采集
		{
			gnss_tmp = 0;
			gnss_flag = 0;
			g_JT808GnssManager.SetGnssUp( gnss_tmp, ConfigManager::m_iGNSSUpLoadSet );

		}
		if((Notify->cm_flag == 0)&&(ConnectionManager::Instance()->b_zone == false))
		{
			printf("********************************************    break net\n");
			ConnectionManager::Instance()->b_zone = true;
			
			//ConnectionManager::Instance()->Stop();
			mangqu = 1;
			mangqu_flag = 1;
			pthread_mutex_lock(&FenceMutex);
			fence_inout_flag = 1;
			pthread_mutex_unlock(&FenceMutex);
		}
	}
	else
	{
		gnss_flag = 1;
		mangqu_flag = 0;
		if((Notify->cm_flag == 1)&&(ConnectionManager::Instance()->b_zone == true))
		{
			printf("********************************************    start net\n");
			ConnectionManager::Instance()->b_zone = false;
			enter_mangqu=0;
			ConnectionManager::Instance()->Connect();
		}
	}

	return;
}
#if 1
void JT8082NaviInfoHandler(DBusNotifyC *pNotify)
{
	printf("JT8082NaviInfoHandler  \n");
	if(pNotify->getIdentifier() != JT8082_DBUS_REQ_ID_NaviElectronicFence)
		return;
	JT8082NaviElectronicFenceNotifyC *Notify= (JT8082NaviElectronicFenceNotifyC *)pNotify;

	CNaviElectronicFenceNotifyC notify2;
	
	 notify2.eflag=Notify->eflag;
	 notify2.etype=Notify->etype;
	 notify2.eid=Notify->eid;
	 notify2.estate=Notify->estate;
	
	 g_pDbusServer->sendNotify(&notify2);


	return;
}

void JT8082GNSSInfoHandler(DBusNotifyC *pNotify)
{
	printf("JT8082GNSSInfoHandler  **************************************888\n");

	
	JT8082GNSSAttrChangeNotifyC *notify1= (JT8082GNSSAttrChangeNotifyC *)pNotify;
	if (notify1->gnss_flag==1)
	{	
		CGNSSAttrChangeNotifyC notify2;
	    notify2.mode=notify1->mode;
	    notify2.baudrate=notify1->baudrate;
	    notify2.outfrequency=notify1->outfrequency;
	    notify2.samplerate=notify1->samplerate;
	    
	    g_pDbusServer->sendNotify(&notify2);
	}
	if(notify1->refresh==1)
	{
		jt808.RefreshConfig();
	}
	if(notify1->reregister==1)
	{
			ConnectionManager::Instance()->NetBreak(1);
			ConnectionManager::Instance()->Notify(ENUM_CONNECTION_STATE_DISCONNECTED);
			jt808.RefreshConfig();
			jt808.m_registered = false;
            jt808.m_authorized = false;
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
			 ConnectionManager::Instance()->Reconnect();
 	 }
	if (notify1->reconnect==1)
	{
		if (connect_tmp==0)
		{
			ConnectionManager::Instance()->NetBreak(1);
			ConnectionManager::Instance()->Notify(ENUM_CONNECTION_STATE_DISCONNECTED);
			jt808.RefreshConfig();
			jt808.m_registered = false;
            jt808.m_authorized = false;
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
			ConnectionManager::Instance()->Reconnect();
		}
	}
       
	

	return;    
}
void HandleJT8082NetBreak(DBusNotifyC *pNotify)
{
	DeadBlock=1;
	DeadBlock_flag=1;
	ConnectionManager::Instance()->b_zone = true;
	ConnectionManager::Instance()->NetBreak(0);
}

#endif
DBusClientC    *g_pDbusClient;
DBusClientC    *g_p2DbusClient;

DBusServerC    *g_pDbusServer;
DBusClientProxyC *sp_ClientGps;
DBusClientC    *g_pDbusBusD;
void SystimeOffTimerHandler(string &name, void *data, void *data2)
{
		JT808AccOffDbusNotifyC  Notify;
		Notify.flag=0;
		g_pDbusServer->sendNotify(&Notify);
		printf("power    off    Timer   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		mcumessagehandle.SendPowerOffCheckTime();
		mcumessagehandle.SendSystemOffInfo();

		printf("即		将		关		机\n");
		system("accoff.sh 1 &");
   
}

void JT808SystemOffHandler(DBusNotifyC *pNotify)
{
	DeviceACCInfoDbusNotifyC *notify= (DeviceACCInfoDbusNotifyC *)pNotify;

	printf("notify->flag111111111111111111111111==%d \n",notify->flag);
	if(notify->flag==0)
	{
		if (START_UP_CAUSE==AVERAGE)
		{	
			jt808.HandleShutDownReboot(1);
			return;
		}
	}

}

void * SendNotifyToJT8082(void * argc)
{
	 int seq = *(int *)argc;
	int i=60;
	printf(" seq   ======================================== %d       \n",seq);
	JT808SleepWakeServer2NotifyC  notify;	
	while (i)
	{
		i--;
		notify.seq=seq+1;
		g_pDbusServer->sendNotify(&notify);
		sleep(1);
	}
}

void HandleMcuStatus()
{
    TIME_INTERVAL McuToCpu;
    struct timeval tv; 
	unsigned int utc_time;
	JT808LocTimeDbusNotifyC notify;
    mcumessagehandle.HandleMcuRstDownCause();
    printf("START_UP_CAUSE============%d\n",START_UP_CAUSE);
    switch(START_UP_CAUSE)
    {	

		case DORMANT_RESTART:
			printf ("休		眠		启		动3\n");
			
		 	AccStatus=3;			
			jt808.m_registered = true;
			jt808.m_authorized = true;
			//jt808.RefreshGetLocTime(1);
			ConnectionManager::Instance()->Start();
			sleep(3);
			ConfigManager::SetAuthFlag(false);
			connect_tmp=1;
			mcumessagehandle.Handletimeinterval(&McuToCpu);
			tmp_seq=McuToCpu.seq+1;
			jt808.HandleHelloTimer();
			StartTimer(systemoff);
            break;
		case NETWORK_BREAK:
            tmp_seq=0;
			connect_tmp=0;
            printf("断      网      重      启1\n");
			AccStatus=1;
			//jt808.RefreshGetLocTime(1);
			tmp_seq=McuToCpu.seq+1;
			mcumessagehandle.Handletimeinterval(&McuToCpu);
            ConnectionManager::Instance()->Start();
			sleep(2);
			StartTimer(systemoff);
            break;
        case AVERAGE:
			printf("正      常      启      动    2\n");
			AccStatus=2;
			mcumessagehandle.Handletimeinterval(&McuToCpu);
			jt808.RefreshGetLocTime(1);
			printf("McuToCpu.seq=%d\n McuToCpu.time_heart=%d\n  McuToCpu.time_loc=%d\n",McuToCpu.seq,McuToCpu.time_heart,McuToCpu.time_loc);
			if((McuToCpu.seq!=0)||(McuToCpu.time_heart!=0)||(McuToCpu.time_loc!=0)||(McuToCpu.flag==1)||(McuToCpu.seq2!=0))
			{
					pthread_t tid;
					pthread_create(&tid,NULL,SendNotifyToJT8082,&(McuToCpu.seq2));
					pthread_detach(tid);
					
					SendAuth_flag_tmp=0;
					tmp_seq=McuToCpu.seq;
					VDRDeployParaTable *table = new VDRDeployParaTable();
				    string authToken = "";
				    if(table->GetStringValue((int)VDR_DEPLOY_PARA_AUTH_TOKEN, authToken) < 0)
				    {
				        LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHandler::JT808MessageHandler get authToken failed");
				    }
				    if(authToken.length() > 0)
				    {
				       jt808.m_registered = true;
				    }
					if (!jt808.m_registered)
					{
						tmp_seq=0;
						jt808.m_authorized = false;
						ConnectionManager::Instance()->Start();
					}
					else 
					{
						tmp_seq=McuToCpu.seq+1;
						jt808.m_authorized = true;
						ConfigManager::SetAuthFlag(false);
						connect_tmp=1;
						mcumessagehandle.get_utc_now_time(&tv);
	             		utc_time=(unsigned int )tv.tv_sec;
						notify.LocUtcTime=McuToCpu.time_loc;
						notify.LocInterval=loc_retry_time;
						//g_pDbusServer->sendNotify(&notify);
						ConnectionManager::Instance()->Start();
						sleep(2);
						jt808.HandleHelloTimer();
						printf("setting flag ==========================================%d \n",McuToCpu.flag);
						if (McuToCpu.flag==1)
						{
							mcumessagehandle.ReadParaterFromMcu();
						}
					}
					
					
			 }
			 else 
			 {
				ConnectionManager::Instance()->Start();
				tmp_seq=0;
			 }
			
			
			 break;

   }
}
int main(int argc, char *argv[])
{
	if(ProcessOptions(argc, argv) != 0)
    {
	    return 1;
	}
    printf("DBusApplicationC!\n" );
	DBusApplicationC app(argc, argv);

	sp_ClientGps = new DBusClientProxyC((char*)"com.YunTu.Navi", (char*)"/Navi");
	sp_ClientGps->registerReply(REP_ID_NAVI_COMMON,CNaviCommonRepC::factory);//通用应答
	if(sp_ClientGps->connect()<0)
	{
	    printf("Failed to connect to DBUS server.\r\n");
	    //return -1;
	}
    printf("DBusServerC---------------------------------------------------------------\r\n");

	DBusServerC server(JT808_DBUS_SERVER_ID,JT808_DBUS_SERVER_OBJ);

	g_pDbusServer =&server;

	server.registerService(JT808_DBUS_REQ_ID_REGISTER,JT808RegisterServiceHandler,JT808RegisterDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_AUTHENTICATION,JT808AuthenticationServiceHandler,JT808AuthenticationDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_ANSWER,JT808AnswerServiceHandler,JT808AnswerDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_SETTING_MODIFY,JT808ModifySettingServiceHandler,JT808ModifySettingReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_DRIVER_INFO,JT808DriverInfoServiceHandler,JT808DriverInfoReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_CAMERA,JT808CameraServiceHandler,JT808CameraDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_EVENTR,JT808EventServiceHandler,JT808EventReportDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_VIDEO,JT808VideoServiceHandler,JT808VideoDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_RECORD,JT808RecordServiceHandler,JT808RecordDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_DATA_TRANSMIT,JT808TransmissionDataServiceHandler,JT808DataTransmitReqC::factory);
    
	server.registerService(JT808_DBUS_REQ_ID_INFOMATION_DEMAND,JT808InfomationDemandHandler,JT808InfomationDbusReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_SHUTDOWN,ShutdownMachineHandler,JT808ShutdownMachineReqC::factory);
   	server.registerService(JT808_DBUS_REQ_ID_REBOOT,RebootMachineHandler,JT808RebootMachineReqC::factory);
	server.registerService(JT808_DBUS_REQ_ID_COMMON_EVENT,HandleGuiEvent,JT808CommonEventReqC::factory);
	/************************* 渣土车 devicemanager    请求   ******************/
	server.registerService(JT808_DBUS_REQ_ID_DREG_COMMON,HandleDevPartOfCarRequest,JT808DregCommonReqC::factory);
	
	if(server.connect()<0)
	{
	    cerr << "Failed to connect to DBUS server.\r\n";
	    return -1;
	}
	DBusClientC client(DEVICE_MANAGER_DBUS_SERVER_ID, DEVICE_MANAGER_DBUS_SERVER_OBJ);
	DBusClientC client_2(JT8082_DBUS_SERVER_ID, JT8082_DBUS_SERVER_OBJ);
	g_pDbusClient= &client;
	g_p2DbusClient= &client_2;
	g_pDbusClient->registerNotify(DEVICE_MANAGER_NOTF_ID_FENCE_INOUT,CNaviElectronicFenceInorOutNotifyHandler,DeviceFenceInOutDbusNotifyC ::factory);
	g_pDbusClient->registerNotify(DEVICE_MANAGER_NOTF_ID_ACC_INFO,JT808SystemOffHandler,DeviceACCInfoDbusNotifyC::factory);
	
	g_p2DbusClient->registerNotify(JT8082_DBUS_NTF_ID_NaviElectronicFence,JT8082NaviInfoHandler,JT8082NaviElectronicFenceNotifyC::factory);
	g_p2DbusClient->registerNotify(JT8082_DBUS_NTF_ID_GNSSAttrChange,JT8082GNSSInfoHandler,JT8082GNSSAttrChangeNotifyC::factory);

	g_p2DbusClient->registerNotify(JT8082_DBUS_NTF_ID_NETBREAK,HandleJT8082NetBreak,JT8082NetBreakNotifyC::factory);

	systemoff=CreateTimer("systemoff", TIMER_SEC_TO_MSEC(SYSTEMOFFTIMEOUT), (void *)SYSTEMOFF, SystimeOffTimerHandler, false);
	if(g_pDbusClient->connect()<0)
	{
	    cerr << "Failed to connect to DBUS client.\r\n";
	    return -1;
	}
	if(g_p2DbusClient->connect()<0)
	{
	    cerr << "Failed to connect to DBUS2 client.\r\n";
	    return -1;
	}
	can_socket_recv();
	if( !g_JT808GnssManager.Init() )
	{
		cerr << "UpLoadManager::Init Failed\r\n";
		return -1;
	}
	gps_socket_recv();
 	HandleMcuStatus();
		
    #ifdef BUS_DAEMON_PROJECT
	g_pDbusBusD = new DBusClientC(BUSD_DBUS_SERVER_ID, BUSD_DBUS_SERVER_OBJ); ;
	g_pDbusBusD->registerNotify(BUSD_DBUS_NTF_ID_POSINFO_UPLOAD,BusDPosNotifyHandler,BUSDPosInfoUpdateDbusNotifyC::factory);
	g_pDbusBusD->registerNotify(BUSD_DBUS_NTF_ID_STATION_PASSENGER_UPDATE,BusDStationPassengerHandler,BUSDStationPassengerDbusNotifyC::factory);
	g_pDbusBusD->registerNotify(BUSD_DBUS_NTF_ID_CARINFO_UPLOAD,BusDCanNotifyHandler,BUSDCarINfoUpdateDbusNotifyC::factory);

	if(g_pDbusBusD->connect()<0)
	{
	    cerr << "Failed to connect to DBUS client.\r\n";
	    return -1;
	}

    #endif 
	return app.loop();
}
