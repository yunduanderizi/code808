#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <sys/un.h>
#include "ConnectionManagerAT.h"
#include "LogUtility.h"
#include "ConfigManager.h"
#include "phoneMessage.h"
#include "JT808DbusMessageHandler.h"
extern unsigned char connectphone_tmp;
extern int enter_mangqu;
extern unsigned int  AccStatus;
extern unsigned short heart_retry_time;
ConnectionManager *ConnectionManager::m_instance = NULL;
int ConnectionManager::connection_num= 1;
int ConnectionManager::connection_bei= 0;
unsigned char  connect_tmp=0;
static unsigned char receiveBuffer[CONNECTION_BUFFER_LEN];
extern JT808MessageHandler jt808;
char * StrNull="未设置";
extern int ApnFlag;
extern int DeadBlock_flag;
int Lougout=0;
extern int SendAuth_flag_tmp;
int flag_1=0;
int flag_3=0;
#define CONNECT_TIME 12
int connect_num=0;
static void ConnectionTimerHandler(string &name, void *data, void *data2)
{
	ConnectionManager *handler = (ConnectionManager *)data;
    if(handler != NULL)
    {
        handler->HandleTimer(name, data2);
    }
}

ConnectionManager *ConnectionManager::Instance()
{
    if(!m_instance)
    {
       m_instance = new ConnectionManager();
    }
    
    return m_instance;
}

void ConnectionManager::Destroy()
{
    if(m_instance != NULL)
    {
        delete m_instance;
    }
}

ConnectionManager::ConnectionManager()
{
    m_exit = true;
    m_connected = false;
    m_rx_tid = -1;
    b_zone = false;
    m_connId = ENUM_CONNECTION_ID_MASTER;
    m_eventHandlers.clear();
    m_packetHandler = NULL;
    ConnectMainNum=0;
    ConnectSecNum=0;
    m_connectTimer = CreateTimer(CONNECTION_TIMER_NAME, TIMER_SEC_TO_MSEC(CONNECT_TIME), this, ConnectionTimerHandler, false);
    if(pthread_mutex_init(&m_sendingMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "ConnectionManager::ConnectionManager: mutex init failed.");
        return;
    }
}

ConnectionManager::~ConnectionManager()
{
    Stop();
    pthread_mutex_destroy(&m_sendingMutex);
    DestroyTimer(m_connectTimer);
}

void ConnectionManager::Start()
{
    if(m_exit == true)
    {
        m_exit = false;
        pthread_create(&m_rx_tid, NULL, this->RxThreadLoop, this);
    }
    
    return;
}

void ConnectionManager::Stop()
{
    if(m_exit != true)
    {
        printf("come in stop!\n");
        m_exit = true;
		Notify(ENUM_CONNECTION_STATE_DISCONNECTED);
        pthread_kill(m_rx_tid, SIGUSR1);
        pthread_join(m_rx_tid, NULL);
    }

    return;
}

void ConnectionManager::Close()
{
	;
}

int ConnectionManager::sendReq(const unsigned char *buf, int buflen)
{
    int fd, addrlen, len;
    struct sockaddr_un addr;
    int ret;
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(fd < 0) {
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    len = sizeof(PHONE_DATA_SERVER_PATH);
    memcpy(addr.sun_path, PHONE_DATA_SERVER_PATH, len);
    addrlen = len + 2;
    ret = sendto(fd, buf, buflen, MSG_DONTWAIT, (const sockaddr*)&addr, addrlen);
	if(ret != buflen)
        return -1;
    close(fd);
    return ret;
}

int ConnectionManager::forwardData(const unsigned char *buf, int len)
{

	int ret;
	TcpReqHdr *hdr = (TcpReqHdr *)m_sbuf;
	hdr->msgid = TCP_REQ_TYPE_DATA;
	hdr->msglen = len;
	hdr->conNum = LinkNum1;
	memcpy(m_sbuf + sizeof(TcpReqHdr), buf, len);
	ret = sendReq(m_sbuf, sizeof(TcpReqHdr)+len);
	return ret - sizeof(TcpReqHdr);
	//TcpReqHdr hdr;
	//hdr.msgid = TCP_REQ_TYPE_DATA;
	//hdr.msglen = len;
	//hdr.conNum=LinkNum1;
	//sendReq((unsigned char *)&hdr, sizeof(TcpReqHdr));
	//return sendReq(buf, len);
}

void ConnectionManager::HandleTimer(string &name, void *)
{
	if(name == CONNECTION_TIMER_NAME)
	{
		if(m_connId == ENUM_CONNECTION_ID_MASTER)
		{
				if(ConnectMainNum>=3)
				{
					connect_num=0;
					ConnectionManager::ConnectMainNum=0;
					m_connId = ENUM_CONNECTION_ID_SECONDARY;
					ConnectionManager::ConnectSecNum++;
				}
				else
				{
					ConnectionManager::ConnectMainNum++;
					m_connId = ENUM_CONNECTION_ID_MASTER;
				}
		}
		else if(m_connId == ENUM_CONNECTION_ID_SECONDARY)
		{
			if(ConnectSecNum>=3)
			{
					ConnectionManager::ConnectSecNum=0;
					connect_num=0;
					m_connId = ENUM_CONNECTION_ID_MASTER;
					ConnectionManager::ConnectMainNum++;
			}
			else
			{
				ConnectionManager::ConnectSecNum++;
				m_connId = ENUM_CONNECTION_ID_SECONDARY;
			}
		}
		printf("m_connId %d ConnectMainNum %d ConnectSecNum  %d \n",m_connId,ConnectMainNum,ConnectSecNum);
		Connect();
	}
}

int ConnectionManager::Connect()
{
    string serverAddress;
    int port;
    if(ConfigManager::m_control == 2)
    {
            m_connId = ENUM_CONNECTION_ID_IC;
            serverAddress = ConfigManager::m_icmainServerAddress;
            port = ConfigManager::m_ictcpPort;
			

    }
    else if(ConfigManager::m_control == 1)
    {
        if(connection_bei <= 1)
        {
            if(connection_bei == 1) connection_bei++;
            if(m_connId != ENUM_CONNECTION_ID_SECONDARY)
            {
				m_connId = ENUM_CONNECTION_ID_MASTER;
				serverAddress = ConfigManager::GetMainServerAddress();
				port = ConfigManager::GetTcpPort();
				
            }
            else
            {
                m_connId = ENUM_CONNECTION_ID_SECONDARY;
                serverAddress = ConfigManager::GetSecondaryServerAddress();
                port = ConfigManager::GetTcpPort();
				
            }
        }
        else if( connection_bei > 1)
        {
            LogUtility::Log(LOG_LEVEL_WARN, "ConnectionManager::RxThreadLoop connect to main failed.");

            m_connId = ENUM_CONNECTION_ID_SECONDARY;
            serverAddress = ConfigManager::GetSecondaryServerAddress();
            port = ConfigManager::GetTcpPort();
			
        }
    }
    else
    {
        m_connId = ENUM_CONNECTION_ID_ASSIGN;
        serverAddress = ConfigManager::m_controlServerAddress;
        port = ConfigManager::m_controlTcpPort;
		
    }
	if (strcmp(serverAddress.c_str(),StrNull)==0)
	{
		StartTimer(m_connectTimer);
		printf("Main Don't Set The serverAddress\n");
		return -1;
	}
	if (serverAddress.length()==0)
	{
		StartTimer(m_connectTimer);
		printf("Mian Don't Set The serverAddress  length  %d  \n",serverAddress.length());
		return -1;
	}

    Notify(ENUM_CONNECTION_STATE_CONNECTTING);
    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Connect entry, server %s:%d.Apn : %d ", serverAddress.c_str(), port,ApnFlag);
    
    TcpReqConnect *req = (TcpReqConnect *)malloc(sizeof(TcpReqConnect) + serverAddress.length());
    req->hdr.msgid = TCP_REQ_TYPE_CONNECT;
    req->hdr.msglen = sizeof(unsigned int) + serverAddress.length()+sizeof(unsigned int);
	req->hdr.conNum=LinkNum1;
    req->port =port ;
	req->apn=ApnFlag;
    memcpy(req->ip, serverAddress.c_str(), serverAddress.length());
    sendReq((unsigned char *)req, sizeof(TcpReqConnect) + serverAddress.length());
    //StartTimer(m_connectTimer);
    return 0;
}

int ConnectionManager::createServer()
{
	int fd, addrlen, len;
	struct sockaddr_un addr;
	int on = 1;

	fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(fd < 0) {
		return -1;
	}

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	len = sizeof(PHONE_DATA_SERVER_PATH2);
	memcpy(addr.sun_path, PHONE_DATA_SERVER_PATH2, len);
	addrlen = len + 2;

	if(bind(fd, (struct sockaddr *)&addr, addrlen) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

void ConnectionManager::processStateChange(int connected)
{
	printf("main connected, %d  m_connected, %d\n", connected, m_connected);
	if(connected && !m_connected)
	{
		printf(" main DeadBlock_flag==%d \n",Lougout,DeadBlock_flag);
		if (DeadBlock_flag==0)
		{	
			if (enter_mangqu==0)
			{
				StopTimer(m_connectTimer);
				m_connId = ENUM_CONNECTION_ID_MASTER;
				ConnectMainNum =0;
				connect_num=0;
				m_connected = true;
				connect_tmp=1;
				flag_1=0;
				flag_3=0;
				ConnectionManager::ConnectMainNum=0;
				ConnectionManager::ConnectSecNum=0;
				Notify(ENUM_CONNECTION_STATE_CONNECTED);
				if (SendAuth_flag_tmp==0)
				{
					if (jt808.m_registered)
						SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_CONNECTED, (int)CONN_STATUS_NTF_CONNECTED);
					//SendJT8082ConnectionStatus();
				}
				if (!ConfigManager::GetAuthFlag())
				{
					jt808.HandleHelloTimer();
					jt808.HandleConnect();
				}	
			}
		}
	}
	else if(!connected && m_connected)
	{
		printf("main Lougout==%d DeadBlock_flag==%d \n",Lougout,DeadBlock_flag);
		connect_tmp=0;
		if (Lougout==0)
		{
			if (DeadBlock_flag==0)
			{
				if (enter_mangqu==0)
				{
					SendAuth_flag_tmp=1;
					connectphone_tmp=0;
					m_connected = false;
					ConfigManager::SetAuthFlag(true);
					Notify(ENUM_CONNECTION_STATE_DISCONNECTED);
					SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_DISCONNECT);
					connect_num++;
					m_connectTimer->timeout=TIMER_SEC_TO_MSEC((connect_num+1)*CONNECT_TIME);
					StartTimer(m_connectTimer);
				}
			}
		}
	}
	else 
	{	
		if(!m_connected && !connected)
		{
			printf("Enter Wrong Main m_connected   %d    connected  %d  \n",m_connected,connected);
			if (DeadBlock_flag==0)
			{
				if (Lougout==0)
				{		
					if (enter_mangqu==0)
					{
						SendAuth_flag_tmp=1;
						flag_3=1;
						connect_tmp=0;
						connectphone_tmp=0;
						jt808.m_authorized = false; 
						SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_DISCONNECT);
						ConfigManager::SetAuthFlag(true);
						connect_num++;
						m_connectTimer->timeout=TIMER_SEC_TO_MSEC((connect_num+1)*CONNECT_TIME);
						StartTimer(m_connectTimer);
					}
				}
			}
		}
	}
}

void ConnectionManager::receiveData(int fd)
{
	int len;
	TcpReqHdr *hdr;
	TcpReqStatus *statusReq;
	while(1)
	{
        len = recv(fd, receiveBuffer, M_BUF_SIZE, 0);
        if(len <= 0) {
            break;
        }
        hdr = (TcpReqHdr *)receiveBuffer;
        //if(hdr->msglen)
          //  len = recv(fd, receiveBuffer + sizeof(TcpReqHdr), hdr->msglen, 0);
		printf("ConnectionManager::receiveData in... :::::::::::msgid: %d, msglen, %d  len:%d\n", 
				hdr->msgid, hdr->msglen, len);
        switch(hdr->msgid)
        {
        case TCP_REQ_TYPE_DATA:
        	if(hdr->msglen && m_connected)
        		m_packetHandler->PacketHandler(receiveBuffer + sizeof(TcpReqHdr), hdr->msglen);
        	break;
        case TCP_REQ_TYPE_STATUS:
        	statusReq = (TcpReqStatus *)receiveBuffer;
			printf("ConnectionManager::receiveData   TCP_REQ_TYPE_STATUS: msglen:%d  connected :%d\n", 
					hdr->msglen, statusReq->connected);
        	processStateChange(statusReq->connected);
            break;
        default:
        	break;
        }
	}
	//printf("ConnectionManager::receiveData out... \n");
}

static void SignalHandler(int sig)
{
    LogUtility::Log(LOG_LEVEL_INFO, "SignalHandler catch signal 0x%x.", sig);
}

void *ConnectionManager::RxThreadLoop(void *arg)
{ 
	int fd;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::RxThreadLoop entry....");
    ConnectionManager *manager = reinterpret_cast<ConnectionManager *> (arg);
    
    signal(SIGUSR1,SignalHandler);
    signal(SIGPIPE,SignalHandler);

    while(!manager -> m_exit)
    {
		printf("enter  exit !\n");
		if((fd = manager->createServer()) < 0)
       	{
			printf("createServet  failed \n");
			return NULL;
       	}
		if (AccStatus == 2&&SendAuth_flag_tmp==1)
		{	
			if(!manager->m_connected)
			{
				printf("start timer\n");
				
				manager->Connect();
				//StartTimer(manager->m_connectTimer);
			}
		}
		manager->receiveData(fd);
        close(fd);

    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::RxThreadLoop exit.");
    
    return NULL;

}

int ConnectionManager::Send(const unsigned char *buf, int len)
{
    int ret;
    int totalSent=0;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Send %d.", len);
    if(!( ConnectionManager::Instance()-> m_connId == ENUM_CONNECTION_ID_IC ))
    {
        if(m_exit)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Send already exit.");
            return -1;
        }

        if(!m_connected)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "ConnectionManager::Send not connected.");
            return -1;
        }
    }
    printf("\n");
	printf("JT808Manager  FILE %s  LINE %d \n",__FILE__,__LINE__);
    for(int i = 0; i < len; i ++)
    {
        printf("%02x ", buf[i]);
        if((i + 1) % 16 == 0)
        {
            printf("\r\n");
        }
    }
    
    pthread_mutex_lock(&m_sendingMutex);
    do{
    	ret = forwardData(buf+totalSent, len-totalSent);
		if(ret < 0)
			return totalSent;
		printf("\n%d:::Current Packet need translate %d; infact translate %d packets\n",__LINE__,len,ret);
		totalSent += ret;
    }while(totalSent < len);
    pthread_mutex_unlock(&m_sendingMutex);
    return totalSent;
}

void ConnectionManager::Reconnect()
{
	m_connId = ENUM_CONNECTION_ID_MASTER;
	ConnectMainNum =0;
	m_connected=0;
	ConfigManager::SetAuthFlag(true);
	Connect();
	//Notify(ENUM_CONNECTION_STATE_DISCONNECTED);	
}

void ConnectionManager::Notify(ENUM_CONNECTION_EVENT event)
{
    for(set<ConnectionEventHandlerInterface *>::iterator it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it ++)
    {
        ConnectionEventHandlerInterface *handler = *it;
        handler->ConnectionEventHandler(event, m_connId);
    }
    return;
}
void ConnectionManager::HandleReceiveBuf(unsigned char * buf,unsigned int len )
{
      
		printf("HandleReceiveBuf::len==%d \n",len);
		//m_packetHandler->PacketHandler(buf,len);
		jt808.PacketHandler(buf,len);
		
}
void ConnectionManager::NetBreak( int BreakNum)
{
	//BreakNum  0  全部断开1  断开第一个 2断第二个连接
	string serverAddress="1.1.1.1";
	TcpReqConfig *req = (TcpReqConfig *)malloc(sizeof(TcpReqHdr) + 2*sizeof(int));
    req->hdr.msgid = TCP_REQ_TYPE_CONFIG;
    req->hdr.msglen = 2 * sizeof(int);
	connect_num=0;
	m_connected=0;
	connect_tmp=0;
	connectphone_tmp=0;
	SendAuth_flag_tmp=1;
	if (BreakNum==0)
	{		
		req->closelink = 1;
		req->closetcp=0;
		req->hdr.conNum=LinkNum1;
		SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_DISCONNECT);
	}
	if (BreakNum==1)
	{	
		req->closelink =0;
		req->closetcp=1;
		req->hdr.conNum=LinkNum1;
		SendJT808ConncetionStatusNotify((int)CONN_STATUS_NTF_DISCONNECT, (int)CONN_STATUS_NTF_DISCONNECT);
	}
	if (BreakNum==2)
	{		
		req->closelink =0;
		req->closetcp=1;
		req->hdr.conNum=LinkNum2;
	}
	sendReq((unsigned char *)req, sizeof(TcpReqConnect) + serverAddress.length());    
}

