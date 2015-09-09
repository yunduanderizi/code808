 #include "Utility.h"
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ConfigManager.h"
#include "VDRRunParaTable.h"
#include "JT808GnssManager.h"
#include "JT808GnssManager.h"
#include "phoneMessage.h"
#include "VDRBaseConfigTable.h"
#include "JT808MessageHandler.h"
extern JT808MessageHandler jt808;
extern unsigned short loc_retry_time;//位置信息间隔
//extern 	SQLiteSequence value_sleep;
extern unsigned short heart_retry_time;
string ConfigManager::m_mainServerAddress = DEFAULT_MAIN_SERVER_ADDRESS;
string ConfigManager::m_icmainServerAddress = DEFAULT_MAIN_SERVER_ADDRESS;
string ConfigManager::m_secondaryServerAddress  = DEFAULT_SECONDARY_SERVER_ADDRESS;
string ConfigManager::m_mainserverApn=DEFAULT_MAIN_SERVER_APN;
string ConfigManager::m_auth= "";
int ConfigManager::m_tcpPort = DEFAULT_TCP_PORT;
int ConfigManager::m_secondtcpPort = DEFAULT_TCP_PORT;
int ConfigManager::m_ictcpPort = DEFAULT_TCP_PORT;
int ConfigManager::m_tcpConnectTimeout = DEFAULT_TCP_CONNECT_TIMEOUT;
int ConfigManager::m_reconnectInterval = DEFAULT_RECONNECT_INTERVAL;
int ConfigManager::m_heartBeatInterval = DEFAULT_HEART_BEAT_INTERVAL;
int ConfigManager::m_tcpRetryTimeout = DEFAULT_TCP_RETRY_TIMEOUT;
int ConfigManager::m_tcpRetryTimes = DEFAULT_TCP_RETRY_TIMES;
//GNSS设置
int ConfigManager::m_iGNSSMode=1;
int ConfigManager::m_iGNSSBaud=1;
int ConfigManager::m_iGNSSOut=0;
int ConfigManager::m_iGNSSFre=0;
int ConfigManager::m_iGNSSUpLoadMode = 0;
int ConfigManager::m_iGNSSUpLoadSet = 0;
//--
int ConfigManager::m_control = 1;
string ConfigManager::m_controlServerAddress = DEFAULT_MAIN_SERVER_ADDRESS;
int ConfigManager::m_controlTcpPort = DEFAULT_TCP_PORT;
int ConfigManager::m_controlTime = 0;
bool ConfigManager::m_helloFlag = true;
bool ConfigManager::m_authFlag = true;
extern int gnss_set_flag;
char * StrCmnet="CMNET";
char * StrUninet="UNINET";
char * StrCtnet="CTNET";
unsigned int ApnFlag=0;
extern char *AuthString;
extern char *Main_Server_To_Mcu;
extern char *Second_Server_To_Mcu;
extern int  Port_To_Mcu1;
extern int Port_To_Mcu2;
const StruStringConfigItem ConfigManager::m_stringItems[] = {
    //{"MainServerAddress",       VDR_PARA_MAIN_SERVER_ADDRESS,       DEFAULT_MAIN_SERVER_ADDRESS,        &ConfigManager::m_mainServerAddress},
    //{"SecondaryServerAddress",  VDR_PARA_SECOND_SERVER_ADDRESS,     DEFAULT_SECONDARY_SERVER_ADDRESS,   &ConfigManager::m_secondaryServerAddress},
    {"ICMainServerAddress",      VDR_PARA_IC_MAIN_SERVER_IP_ADDR,       DEFAULT_MAIN_SERVER_ADDRESS,        &ConfigManager::m_icmainServerAddress},
    {"MainSeverApn",			VDR_PARA_MAIN_SERVER_APN,				DEFAULT_MAIN_SERVER_APN,								&ConfigManager::m_mainserverApn},
};

const StruIntConfigItem ConfigManager::m_intItems[] = {
 //  {"TcpPort",                 VDR_PARA_TCP_PORT,                  DEFAULT_TCP_PORT,                   &ConfigManager::m_tcpPort},
    {"HeartBeatInterval",       VDR_PARA_HEART_BEAT_INTERVAL,       DEFAULT_HEART_BEAT_INTERVAL,        &ConfigManager::m_heartBeatInterval},
    {"TcpRetryTimeout",         VDR_PARA_TCP_REPLY_TIMEOUT,         DEFAULT_TCP_RETRY_TIMEOUT,          &ConfigManager::m_tcpRetryTimeout},
    {"TcpRetryTimes",           VDR_PARA_TCP_RETRY_TIMES,           DEFAULT_TCP_RETRY_TIMES,            &ConfigManager::m_tcpRetryTimes},
    {"ICTcpPort",               VDR_PARA_IC_MAIN_SERVER_TCP_PORT,                  DEFAULT_TCP_PORT,    &ConfigManager::m_ictcpPort},
  //  {"GNSS_LOC_MODE",           VDR_PARA_GNSS_LOC_MODE,            1,                                   &ConfigManager::m_iGNSSMode},
   // {"GNSS_BAUD_RATE",          VDR_PARA_GNSS_BAUD_RATE,           1,                                   &ConfigManager::m_iGNSSBaud},
   // {"GNSS_DATA_OUT_RATE",      VDR_PARA_GNSS_DATA_OUT_RATE,       0,                                   &ConfigManager::m_iGNSSOut},
   // {"GNSS_DATA_COLLECT_RATE",  VDR_PARA_GNSS_DATA_COLLECT_RATE,   0,                                   &ConfigManager::m_iGNSSFre},
    //{"GNSS_DATA_UPLOAD_MODE",    VDR_PARA_GNSS_DATA_UPLOAD_MODE,     0,                                 &ConfigManager::m_iGNSSUpLoadMode},
	//{"GNSS_DATA_UPLOAD_SET",    VDR_PARA_GNSS_DATA_UPLOAD_SET,     0,                                   &ConfigManager::m_iGNSSUpLoadSet},
};

const StruStringConfigItem ConfigManager::m_stringItems_base[] = {
    {"MainServerAddress",             VDR_BASE_STATUS_D5,    DEFAULT_MAIN_SERVER_ADDRESS  ,        &ConfigManager::m_mainServerAddress},
    {"SecondaryServerAddress",    VDR_BASE_STATUS_D6,    DEFAULT_SECONDARY_SERVER_ADDRESS,   &ConfigManager::m_secondaryServerAddress},
};
const StruIntConfigItem ConfigManager::m_intItems_base[]={
    {"TcpPort",                                               VDR_BASE_CONFIG_D6,                  DEFAULT_TCP_PORT,                   &ConfigManager::m_tcpPort},
    {"SencodyTcpPort",                                   VDR_BASE_CONFIG_D7,                 DEFAULT_TCP_PORT,                      &ConfigManager::m_secondtcpPort},   
};
void ConfigManager::Refresh()
{
	
    VDRRunParaTable *table = new VDRRunParaTable();
    VDRBaseConfigTable *baseconfig = new VDRBaseConfigTable();
	string auth;
    int itemNumber = sizeof(m_stringItems)/sizeof(m_stringItems[0]);
	
    for(int i = 0; i < itemNumber; i ++)
    {
        const StruStringConfigItem &item = m_stringItems[i];

        if(table->GetStringValue(item.col, *(item.value)) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "ConfigManager::Refresh fail to get %s.", item.name.c_str());
            *(item.value) = item.defaultValue;
        }
        LogUtility::Log(LOG_LEVEL_DEBUG, "ConfigManager::%d-----------------fail to get %s.",__LINE__, (*(item.value)).c_str());
    }

    itemNumber = sizeof(m_stringItems_base)/sizeof(m_stringItems_base[0]);	
    for(int i = 0; i < itemNumber; i ++)
    {
        const StruStringConfigItem &item = m_stringItems_base[i];

        if(baseconfig->GetStringValue(item.col, *(item.value)) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "ConfigManager::Refresh fail to get %s.", item.name.c_str());
            *(item.value) = item.defaultValue;
        }
        LogUtility::Log(LOG_LEVEL_DEBUG, "ConfigManager-secondservice ip ::%d-----------------  get %s.",__LINE__, (*(item.value)).c_str());
    }

	
    itemNumber = sizeof(m_intItems)/sizeof(m_intItems[0]);
    for(int i = 0; i < itemNumber; i ++)
    {
        const StruIntConfigItem &item = m_intItems[i];

        if(table->GetIntValue(item.col, *(item.value)) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "ConfigManager::Refresh fail to get %s.", item.name.c_str());
            *(item.value) = item.defaultValue;
        }
        printf(" table item.col=%d,item.value=%d\n",item.col,item.value);
    }

    itemNumber = sizeof(m_intItems_base)/sizeof(m_intItems_base[0]);
    for(int i = 0; i < itemNumber; i ++)
    {
        const StruIntConfigItem &item = m_intItems_base[i];

        if(baseconfig->GetIntValue(item.col, *(item.value)) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "ConfigManager::Refresh fail to get %s.", item.name.c_str());
            *(item.value) = item.defaultValue;
        }
        printf(" table item.col=%d,item.value=%d\n",item.col,item.value);
    }
	if(baseconfig->GetStringValue(VDR_BASE_STATUS_D7, auth) < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "ConfigManager::Refresh fail to get %s.", auth.c_str());
           
        }
	
    delete table;
    //ConfigManager::GetServerIPByName();
    printf("m_mainServerAddress.c_str()==%s  , m_secondaryServerAddress.c_str()=%s \n ,",m_mainServerAddress.c_str(),m_secondaryServerAddress.c_str());
	m_tcpConnectTimeout = DEFAULT_TCP_CONNECT_TIMEOUT;
    m_reconnectInterval = DEFAULT_RECONNECT_INTERVAL;
	Main_Server_To_Mcu=(char *)malloc(sizeof(char)*30);
	Second_Server_To_Mcu=(char *)malloc(sizeof(char)*30);
	memcpy(Main_Server_To_Mcu,m_mainServerAddress.c_str(),m_mainServerAddress.length());
	memcpy(Second_Server_To_Mcu,m_secondaryServerAddress.c_str(),m_secondaryServerAddress.length());	
	Port_To_Mcu1=m_tcpPort;
	Port_To_Mcu2=m_secondtcpPort;
	AuthString= (char * )malloc (sizeof(char)*64);
	memcpy(AuthString,auth.c_str(),auth.length());


	if (strcmp(StrCmnet,ConfigManager::m_mainserverApn.c_str())==0)
	{	
		ApnFlag=APN_CMNET;
		printf("APN_CMNET ConfigManager::m_mainserverApn.c_str()=%s ",ConfigManager::m_mainserverApn.c_str());
	}else if (strcmp(StrUninet,ConfigManager::m_mainserverApn.c_str())==0)
	{
		ApnFlag=APN_UNINET;
		printf("APN_UNINET ConfigManager::m_mainserverApn.c_str()=%s ",ConfigManager::m_mainserverApn.c_str());
	}else if (strcmp(StrCtnet,ConfigManager::m_mainserverApn.c_str())==0)
	{
		ApnFlag=APN_CTNET;
		printf("APN_CTNET ConfigManager::m_mainserverApn.c_str()=%s ",ConfigManager::m_mainserverApn.c_str());
	}
	else 
		ApnFlag=APN_CMNET;
	printf("ApnFlag ==%d \n",ApnFlag);
   if (gnss_set_flag==1)
	   g_JT808GnssManager.SetGnssUp( ConfigManager::m_iGNSSUpLoadMode, ConfigManager::m_iGNSSUpLoadSet );

    return;
}

ostream& operator <<(ostream& os, const ConfigManager& table)
{
    int itemNumber = sizeof(table.m_stringItems)/sizeof(table.m_stringItems[0]);

    os << "ConfigManager:" << endl;

    for(int i = 0; i < itemNumber; i ++)
    {
        const StruStringConfigItem &item = table.m_stringItems[i];
        os << item.name << "=" << (*item.value) << endl;
    }
    
    itemNumber = sizeof(table.m_intItems)/sizeof(table.m_intItems[0]);
    for(int i = 0; i < itemNumber; i ++)
    {
        const StruIntConfigItem &item = table.m_intItems[i];
        os << item.name << "=" << (*item.value) << endl;
    }
    return os;
}
bool ConfigManager::GetServerIPByName()
{
    //货运平台固定地址,货运平台需要打开
    bool bRet = false;
    struct hostent *h;
    printf( "GetServerIPByName   m_mainServerAddress=%s m_secondaryServerAddress=%s\n",m_mainServerAddress.c_str(),m_secondaryServerAddress.c_str() );
    if( !isdigit(m_mainServerAddress.at(0)) )
    {
        //char* ip=inet_ntoa(addr);
        if((h = gethostbyname( m_mainServerAddress.c_str())) == NULL)
        {
            printf("error: %s\n", m_mainServerAddress.c_str());
            //m_mainServerAddress = "210.82.111.147";
        }
        else
        {
            //(*item.value).assign(inet_ntoa(*((struct in_addr *)h->h_addr)));
            //m_mainServerAddress = "210.82.111.147";
            m_mainServerAddress = inet_ntoa(*((struct in_addr *)h->h_addr));
            printf("m_mainServerAddress: %s\n", m_mainServerAddress.c_str());
            bRet = true;
        }
    }
    if( !isdigit(m_secondaryServerAddress.at(0)) )
    {
        if((h = gethostbyname( m_secondaryServerAddress.c_str())) == NULL)
        {
            printf("error: %s\n", m_secondaryServerAddress.c_str());
            //m_secondaryServerAddress = "114.242.194.226";
        }else
        {
            //(*item.value).assign(inet_ntoa(*((struct in_addr *)h->h_addr)));
           //m_secondaryServerAddress = "114.242.194.226";
           m_secondaryServerAddress = inet_ntoa(*((struct in_addr *)h->h_addr));
           printf("m_secondaryServerAddress: %s\n", m_secondaryServerAddress.c_str());
           bRet = true;
        }
    }
    return bRet;
}
