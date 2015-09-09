#include <errno.h>
#include <signal.h>
#include "GpsMessageHandler.h"
#include "LogUtility.h"

GpsMessageHandler *GpsMessageHandler::m_instance = NULL;
static unsigned char receiveBuffer[CONNECTION_BUFFER_LEN];

GpsMessageHandler *GpsMessageHandler::Instance()
{
    if(!m_instance)
    {
        m_instance = new GpsMessageHandler();
    }
    
    return m_instance;
}

void GpsMessageHandler::Destroy()
{
    if(m_instance != NULL)
    {
        delete m_instance;
    }
}

GpsMessageHandler::GpsMessageHandler()
{
    m_exit = true;
    m_socket = -1;
    m_rx_tid = -1;
    m_handler = NULL;
}

GpsMessageHandler::~GpsMessageHandler()
{
    Stop();
}

void GpsMessageHandler::Start()
{
    if(m_exit == true)
    {
        m_exit = false;
        pthread_create(&m_rx_tid, NULL, this->RxThreadLoop, this);
    }
    
    return;
}

void GpsMessageHandler::Stop()
{
    if(m_exit != true)
    {
        m_exit = true;
        pthread_kill(m_rx_tid, SIGUSR1);
        pthread_join(m_rx_tid, NULL);
    }

    return;
}

void GpsMessageHandler::Close()
{
    if(m_socket >= 0)
    {
        close(m_socket);
        m_socket = -1;
    }
}

int GpsMessageHandler::Connect()
{
    struct sockaddr_in addr_gps; 

    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsMessageHandler::Connect entry.");
    
    m_socket = socket(AF_INET,SOCK_DGRAM,0);
    if( -1 == m_socket )
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GpsMessageHandler::Connect create socket failed.");
        return -1;
    }

    addr_gps.sin_family=AF_INET;
    addr_gps.sin_addr.s_addr=inet_addr("127.0.0.1");
    addr_gps.sin_port = htons(GPS_PORT);
    
    bind(m_socket, (struct sockaddr*)&addr_gps, sizeof(addr_gps));

    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsMessageHandler::Connect exit.");
    
    return 0;
}
int iicount = 0;
int GpsMessageHandler::ReadMessage()
{
    struct sockaddr client;
    int len = sizeof(struct sockaddr);

    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsMessageHandler::ReadMessage.");
    int ret = recvfrom(m_socket, receiveBuffer, CONNECTION_BUFFER_LEN, 0, &client, (socklen_t *)&len);
    if(ret > 0)
    {
        LogUtility::Log(LOG_LEVEL_WARN, "GpsMessageHandler::ReadMessage recv %d bytes.", ret);
        if(m_handler != NULL)
        {
            if(iicount == 2)
            {
                m_handler->SendGPSData(receiveBuffer, ret);
                iicount = 0;
            }
        }
    }
    
    return 0;
}

static void SignalHandler(int sig)
{
    LogUtility::Log(LOG_LEVEL_INFO, "SignalHandler catch signal 0x%x.", sig);
}

void *GpsMessageHandler::RxThreadLoop(void *arg)
{
    string serverAddress;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsMessageHandler::RxThreadLoop entry.");
    GpsMessageHandler *manager = reinterpret_cast<GpsMessageHandler *> (arg);
    
    signal(SIGUSR1,SignalHandler);
    signal(SIGPIPE,SignalHandler);
    
    manager->Connect();
    iicount = 0;
    while(!manager->m_exit)
    {
        iicount++;
        manager->ReadMessage();
    }
    
    manager->Close();
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsMessageHandler::RxThreadLoop exit.");
    
    return NULL;
}
