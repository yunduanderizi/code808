#include "JT808GnssManager.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <dbusClient.h>
#include <signal.h>
#include <queue>
#include "LogUtility.h"
#include "ConfigManager.h"
#include "ConnectionManager.h"
#include "JT808MessageHandler.h"
#include "JT808MessageHelper.h"
#include "JT808DbusMessage.h"
#include "JT808DbusMessageHandler.h"
#include "DeviceManagerDbusMessage.h"
#include <boost/locale.hpp>
using namespace std;

extern int mangqu;

JT808GnssManager g_JT808GnssManager;
pthread_mutex_t GnssDateMutex;
unsigned long GetTickCount()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
void *GnssPorc(void *param)
{
    g_JT808GnssManager.GnssProc();
    return NULL;
}
#if 1
void *GnssSendDate(void *param)
{
    g_JT808GnssManager.GnssSend();
    return NULL;
}
#endif
JT808GnssManager::JT808GnssManager()
{
    m_gnssupmode = 0;
    m_gnssupset = 0;
	Gnssqueue.set_capacity(GnssFiFo::capacity_type(1000));
}
JT808GnssManager::~JT808GnssManager()
{

}
bool JT808GnssManager::Init()
{
    JT808GnssManager::SetGnssUp(ConfigManager::m_iGNSSUpLoadMode,ConfigManager::m_iGNSSUpLoadSet);
    int port =  1012;
    bzero(&m_addr_gnss, sizeof(m_addr_gnss));
    m_addr_gnss.sin_family = AF_INET;
    m_addr_gnss.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_addr_gnss.sin_port = htons(port);

    m_socket_dp = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_socket_dp == -1)
    {
        perror("socket call failed");
        return -1;
    }
#if 0
    int flag=1,len=sizeof(int);
    if( setsockopt(m_socket_dp, SOL_SOCKET, SO_REUSEADDR, &flag, len) == -1 )
    {
        perror("socket call failed");
        return -1;
    }
#endif
    if( bind(m_socket_dp, (struct sockaddr*)&m_addr_gnss, sizeof(m_addr_gnss)) == -1 )
    {
        perror("m_socket_dp bind error!\n");
        return -1;
    }

    pthread_t sock_recv;
    int ret = pthread_create(&sock_recv, NULL, GnssPorc, NULL);
    if(ret < 0)
    {
        printf("create handle_image error!\n");
        return ret;
    }
    pthread_detach(sock_recv);
#if 1
    pthread_t sock_send;
    int ret1 = pthread_create(&sock_send , NULL , GnssSendDate , NULL);
    if(ret1 < 0)
    {
        printf("create handle_image2 error!\n");
        return ret;
    }
    pthread_detach(sock_send);
#endif
    return true;
}

void JT808GnssManager::StopGnssUp()
{
    printf("JT808GnssManager::StopGnssUp\n");
    m_gnssupmode = 0;
    m_distace = 0;
    VDRRunParaTable *vdrtable = new  VDRRunParaTable();
    vdrtable->SetIntValue(VDR_PARA_GNSS_DATA_UPLOAD_MODE,0);
    vdrtable->Commit();
    delete vdrtable;
    ConfigManager::m_iGNSSUpLoadMode=0;
}
void JT808GnssManager::SetGnssUp()
{
    m_gnssupmode = 0;
    m_gnssupset = 0;
   // m_times = 0;
    m_distace = 0;
    m_gnsslasttime = GetTickCount();
    m_gnssbegintime = m_gnsslasttime;
}

void JT808GnssManager::SetGnssUp( int gnssupmode, int gnssupset )
{
    printf("JT808GnssManager::SetGnssUp gnssupmode=%d,gnssupset=%d\r\n",
           gnssupmode,gnssupset);
    m_gnssupmode = gnssupmode;
    m_gnssupset = gnssupset;
    //m_times = 0;
   // m_distace = 0;
    m_gnsslasttime = GetTickCount();
    m_gnssbegintime = m_gnsslasttime;
}

void JT808GnssManager::GnssProc()
{
    int n;
    struct sockaddr client;
    unsigned char buff[1024];
    int len = sizeof(struct sockaddr);
    m_times = 0;
	SendDate *senddate;
    //memset(buff , 0 , sizeof(buff));
    while(1)
    {
        
    	memset(buff , 0 , sizeof(buff));
        n = recvfrom(m_socket_dp, buff, 1024, 0, &client, (socklen_t *)&len);
        m_gnsslasttime = GetTickCount();
        printf("gnss n= %d,m_times=%d,m_gnssupmode=%d,m_gnssupset=%d\r\n",n,m_times,m_gnssupmode,m_gnssupset);
        if( m_times < 0 )
           m_times = 0;

        m_times++;
       
        switch( m_gnssupmode )
        {
        case 0x01:
            if( m_gnssupset > 0 )
            {
                if( m_times%m_gnssupset == 0 )
                {
#if 1  
                    senddate = new SendDate();
					senddate->type = 0x00;
                    senddate->total = n;
                    memcpy(senddate->buff, buff,sizeof(buff));
                    pthread_mutex_lock(&GnssDateMutex);
                    Gnssqueue.push_back(senddate);
                    pthread_mutex_unlock(&GnssDateMutex);
#endif
                   // JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
                  

                }
            }
            else
            {
                JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
            }
            break;
        case 0x02:
            //upload gnss data by distance
            printf("distance up load . no up ,default 10 mi!");
            m_distace += 3;
            if( m_distace > m_gnssupset )
            {
                JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
                m_distace = 0;
            }
            break;
        case 0x0B:
            if( m_gnssupset > 0 )
            {
                m_gnssupset--;
                JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
            }
            else
            {
                StopGnssUp();
            }
            break;
        case 0x0C:
            //upload gnss data by distance
            m_gnssupset -= 3;
            printf("distance up load . no up ,default 3 mi!");
            if( m_gnssupset > 0 )
            {
                JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
            }
            else
            {
                StopGnssUp();
            }
            break;
        case 0x0D:
            if( m_gnssupset > 0 )
            {
                m_gnssupset--;
                JT808MessageHelper::SendTransmissionData(0x00 , n, buff);
            }
            else
            {
                StopGnssUp();
            }
            break;
        default:
            //printf("gnss gnssmode=%d,gnssmodeset=%d buf length is %d\n", m_gnssupmode,m_gnssupset,n );
            usleep(800000);
        }
        usleep(50000);
    }
   // ::close( m_socket_dp );
    return;
}

#if 1
void JT808GnssManager::GnssSend()
{
    SendDate *tmp;
    while(1)
    {
        if(!Gnssqueue.empty())
        {
            if(mangqu == 0)
            {
            	printf("GnssSendQueue size = %d\n",Gnssqueue.size());
                pthread_mutex_lock(&GnssDateMutex);
                tmp = *(Gnssqueue.begin());
                Gnssqueue.pop_front();
                JT808MessageHelper::SendTransmissionData(0x00 , tmp->total, tmp->buff);
				delete tmp;
				pthread_mutex_unlock(&GnssDateMutex);
            }
        }
        usleep(50000);
    }
    return;
}
#endif

