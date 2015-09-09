#include <boost/program_options.hpp>
#include <iostream>
#include <dbusClient.h>
#include <signal.h>
#include "LogUtility.h"
#include "ConfigManager.h"
#include "ConnectionManager.h"
#include "JT808MessageHandler.h"
#include "JT808MessageHelper.h"
#include "JT808DbusMessage.h"
#include "JT808DbusMessageHandler.h"
#include "DeviceManagerDbusMessage.h"
#include "../busdaemon/busmessage.h"
#include "../deviceManager/devicemanager.h"
#include "../wm/dbusClientProxy.h"
#include "NaviDbusMessage.h"
#include "VDRQuestionTable.h"
#include "VDRBaseConfigTable.h"
#include "../tables/BusStationTable.h"
#include "BusParaTable.h"
#include <boost/locale.hpp> 
#include "JT808GnssManager.h"
#include "../common/message/phoneMessage.h"

#include "JT808MessageHandler.h"


#define FREIGHT
namespace po = boost::program_options;
using namespace std;

static int socket_recvfd;
static int socket_gps;
static int gnss_flag = 0;
int  mangqu=0;
int wire_state =0;
int gnss_set_flag=1;
volatile int  mangqu_flag = 0; // mangqu jin chu  biaoji  
volatile int fence_inout_flag =0;   // In or  out  fence  flag  
struct sockaddr_in addr_recv;
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
#if 1

#if 1
#define MAX_PACKET 10240
struct LocStatusData locData[MAX_PACKET];
//GPS recv thread
void *gps_recv_thread(void *param)
{
	int n, i=0, m=0;
	// i count packet numbers
	struct sockaddr client;
	int len = sizeof(struct sockaddr);
	while(1)
	{
		n = recvfrom(socket_gps, &locData[i], sizeof(LocStatusData), 0, &client, (socklen_t *)&len);
		printf("n============================================%d \n",n);
		if(n > 0)
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
					sleep(1);
					ConnectionManager::Instance()->Stop();
					pthread_mutex_lock(&FenceMutex);
					fence_inout_flag = 0;
					printf("**************************mangqu == 1&& fence_inout_flag ==1 fenc_inout_flag= %d\n",fence_inout_flag);
					pthread_mutex_unlock(&FenceMutex);
					continue;
				}
				i++;
				printf("i============================% d\n",i);

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
	int flag=1,len=sizeof(int);
	if( setsockopt(socket_gps, SOL_SOCKET, SO_REUSEADDR, &flag, len) == -1 )
	{
		perror("socket call failed");
		return -1;
	}
	if( bind(socket_gps, (struct sockaddr*)&addr_gps, sizeof(addr_gps)) == -1 )
	{
		perror("socket call failed");
		return -1;
	}

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
#endif

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
	::close( socket_recvfd );
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

	int flag=1,len=sizeof(int);
	if( setsockopt(socket_recvfd, SOL_SOCKET, SO_REUSEADDR, &flag, len) == -1 )
	{
		perror("socket call failed");
		return -1;
	}
	if( bind(socket_recvfd, (struct sockaddr*)&addr_recv, sizeof(addr_recv)) == -1 )
	{
		perror("socket call failed");
		return -1;
	}
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
void  PhoneSMSToSetupHandler(DBusNotifyC *pNotify)
{
	PhoneSMSToSetupNotifyC *notify=(PhoneSMSToSetupNotifyC*)pNotify;
	if(pNotify->getIdentifier()!=NOTF_PHONE_SMS_TO_SETUP)
	{
		printf("pNotify->getIdentifier()!=NOTF_PHONE_SMS_TO_SETUP \n");
		return;
	}
	int rec=0;
	printf("notify->success==%d notify->fun ==%d \n",notify->success,notify->fun);
	jt808.PhoneSMSToSetup(notify->success,notify->fun, rec );

}

void CNaviElectronicFenceInorOutNotifyHandler(DBusNotifyC *pNotify)
{
	int gnss_tmp ;
	if(pNotify->getIdentifier() != DEVICE_MANAGER_NOTF_ID_FENCE_INOUT)
		return;
	DeviceFenceInOutDbusNotifyC *Notify= (DeviceFenceInOutDbusNotifyC *)pNotify;
	printf("CNaviElectronicFenceInorOutNotifyHandler Notify->flag = %d ,Notify->gnss_flag=%d\n",Notify->flag,Notify->gnss_flag );
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
			ConnectionManager::Instance()->b_zone = true;
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
			ConnectionManager::Instance()->b_zone = false;
			ConnectionManager::Instance()->Start();
		}
	}

	return;
}

#endif
DBusClientC    *g_pDbusClient;
DBusServerC    *g_pDbusServer;
DBusClientProxyC *sp_ClientGps;
DBusClientC    *g_pDbusBusD;
DBusClientC    *g_Clientphonesms;

int main(int argc, char *argv[])
{
	if(ProcessOptions(argc, argv) != 0)
	{
		return 1;
	}
	/*
	   VDRQuestionsTable *test = new VDRQuestionsTable();
	//VDRQuestionsTable *tabl = new VDRQuestionsTable();
	int time_now = time(NULL);
	if(test->SetIntValue(VDR_QUESTIONS_TIME,time_now) < 0)
	{
	LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion write table time faild");
	delete test;
	}
	if(test->SetIntValue(VDR_QUESTIONS_FLAG,2) < 0)
	{
	LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion set flag faild");
	delete test;
	}
	if(test->SetIntValue(VDR_QUESTIONS_LEN,10) < 0)
	{
	LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion set question_len faild");
	delete test;
	}
	if(test->SetIntValue(VDR_QUESTIONS_ANSWER_SUM,500)<0)
	{
	LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion set answers_sum faild");
	delete test;
	}
	if(test->SetIntValue(VDR_QUESTIONS_SEQ,12) <0)
	{
	printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
	delete test;
	}
	vector<unsigned char> ll;
	int a=0;
	for(a;a<100;a++)
	{
	ll.push_back('a');
	}
	if(test->SetSequenceValue(VDR_QUESTIONS_ANSWER_LIST,ll)<0)
	{
	LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion set answers_table faild");
	delete test;

	}
	string abc = "please";
	if(test->SetStringValue(VDR_QUESTIONS_QUESTION,abc) < 0)
	{
	LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandlerQuestion set utf_question faild");
	printf("*************************************\n");
	printf("string leixing fail\n");
	printf("*************************************\n");
	delete test;
	}

	if(test->Commit()<0)
	{
	printf("============================================\n");
	}
	printf("It's done!\n");





*/

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

	if(server.connect()<0)
	{
		cerr << "Failed to connect to DBUS server.\r\n";
		return -1;
	}
#if 1

	g_Clientphonesms = new DBusClientProxyC(ServerPhone, ObjectPhone); 
	g_Clientphonesms->registerNotify(NOTF_PHONE_SMS_TO_SETUP,PhoneSMSToSetupHandler,PhoneSMSToSetupNotifyC::factory);
	if(g_Clientphonesms->connect()<0)
	{
		printf("DBusClientProxyC Failed to connect to DBUS server.\r\n");
		return -1;
	}


#endif 
	printf("DBusClientC---- client-----------------------------------------------------------\r\n");
	DBusClientC client(DEVICE_MANAGER_DBUS_SERVER_ID, DEVICE_MANAGER_DBUS_SERVER_OBJ);
	g_pDbusClient= &client;
	g_pDbusClient->registerNotify(DEVICE_MANAGER_NOTF_ID_FENCE_INOUT,CNaviElectronicFenceInorOutNotifyHandler,DeviceFenceInOutDbusNotifyC ::factory);

	if(g_pDbusClient->connect()<0)
	{
		cerr << "Failed to connect to DBUS client.\r\n";
		return -1;
	}

	printf("can_socket_recv-----------------------------------------------------------\r\n");
	can_socket_recv();
	printf("gnss_socket_recv-----------------------------------------------------------\r\n");
	//gnss_socket_recv();
	if( !g_JT808GnssManager.Init() )
	{
		cerr << "UpLoadManager::Init Failed\r\n";
		return -1;
	}
	printf("gps_socket_recv-----------------------------------------------------------\r\n");
	gps_socket_recv();
	printf("ConnectionManager::Instance()->Start();-----------------------------------------------------------\r\n");
	ConnectionManager::Instance()->Start();
	printf("ConnectionManager::Instance()->Start() OK;-----------------------------------------------------------\r\n");

	//启动的时候初始化gnss参数
	/*printf("JT808Manager------ ConfigManager::m_iGNSSUpLoadMode=%d!\n",ConfigManager::m_iGNSSUpLoadMode);
	  SendJT808SettingUpdateNotify();
	  if(ConfigManager::m_iGNSSUpLoadMode != 0 )
	  {
	  printf("JT808Manager----GNSS mode is %d- %d -%d- %d ======================\n",ConfigManager::m_iGNSSMode,ConfigManager::m_iGNSSBaud,ConfigManager::m_iGNSSOut,ConfigManager::m_iGNSSFre);
	  SendJT808GnssUpdateNotify(ConfigManager::m_iGNSSMode,ConfigManager::m_iGNSSBaud,ConfigManager::m_iGNSSOut,ConfigManager::m_iGNSSFre);
	  }*/
#ifdef BUS_DAEMON_PROJECT
	g_pDbusBusD = new DBusClientC(BUSD_DBUS_SERVER_ID, BUSD_DBUS_SERVER_OBJ); ;
	g_pDbusBusD->registerNotify(BUSD_DBUS_NTF_ID_POSINFO_UPLOAD,BusDPosNotifyHandler,BUSDPosInfoUpdateDbusNotifyC::factory);
	g_pDbusBusD->registerNotify(BUSD_DBUS_NTF_ID_STATION_PASSENGER_UPDATE,BusDStationPassengerHandler,BUSDStationPassengerDbusNotifyC::factory);

	if(g_pDbusBusD->connect()<0)
	{
		cerr << "Failed to connect to DBUS client.\r\n";
		return -1;
	}

	pthread_t dri;
	pthread_create(&dri, NULL, send_BusD_driver_data, NULL);
	pthread_detach(dri);
#endif
	return app.loop();
}
