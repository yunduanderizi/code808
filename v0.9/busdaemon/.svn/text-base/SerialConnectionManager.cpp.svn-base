#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include "SerialConnectionManager.h"
#include "LogUtility.h"

SerialConnectionManager *SerialConnectionManager::m_instance = NULL;

static unsigned char receiveBuffer[CONNECTION_BUFFER_LEN];

SerialConnectionManager *SerialConnectionManager::Instance()
{
    if(!m_instance)
    {
        m_instance = new SerialConnectionManager();
    }
    
    return m_instance;
}

void SerialConnectionManager::Destroy()
{
    if(m_instance != NULL)
    {
        delete m_instance;
    }
}

SerialConnectionManager::SerialConnectionManager()
{
    m_exit = true;
    m_fd = -1;
    m_eventHandlers.clear();
    m_packetHandler = NULL;

    if(pthread_mutex_init(&m_sendingMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "ConnectionManager::ConnectionManager: mutex init failed.");
        return;
    }
}

SerialConnectionManager::~SerialConnectionManager()
{
    Stop();
    pthread_mutex_destroy(&m_sendingMutex);
}


void SerialConnectionManager::Start()
{
    if(m_exit == true)
    {
        m_exit = false;
        pthread_create(&m_rx_tid, NULL, this->RxThreadLoop, this);
    }
    
    return;
}

void SerialConnectionManager::Stop()
{
    if(m_exit != true)
    {
        m_exit = true;
        pthread_kill(m_rx_tid, SIGUSR1);
        pthread_join(m_rx_tid, NULL);
    }

    return;
}

void SerialConnectionManager::Close()
{
    if(m_fd >= 0)
    {
        tcsetattr (m_fd, TCSANOW, &m_termios_old);
        close(m_fd);
        m_fd = -1;
    }
}

int SerialConnectionManager::Connect()
{
    struct termios termios_new;
    int ret;

    LogUtility::Log(LOG_LEVEL_DEBUG, "SerialConnectionManager::Connect entry");
    
    m_fd = open(MCU_SERIAL_PORT, O_RDWR|O_NONBLOCK);
    if (m_fd == -1) 
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SerialConnectionManager::Connect failed to open %s", MCU_SERIAL_PORT);
        return -1;
    }
    
    ret = tcgetattr (m_fd, &m_termios_old);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SerialConnectionManager::Connect failed to getattr");
        return -1;
    }

    bzero (&termios_new, sizeof (termios_new));
    cfmakeraw (&termios_new);
    termios_new.c_cflag = MCU_BAUD_RATE;
    termios_new.c_cflag |= CS8;   /* 8bit */
    termios_new.c_cflag &= ~PARENB; /* No parity check */
    termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */
    tcflush (m_fd, TCIFLUSH);
    ret = tcsetattr (m_fd, TCSANOW, &termios_new);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "SerialConnectionManager::Connect failed to setattr");
        return -1;
    }

    LogUtility::Log(LOG_LEVEL_DEBUG, "SerialConnectionManager::Connect exit.");
    
    return 0;
}

int SerialConnectionManager::ReadMessage()
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "SerialConnectionManager::ReadMessage.");
    int ret = read(m_fd, receiveBuffer, CONNECTION_BUFFER_LEN);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_WARN, "SerialConnectionManager::ReadMessage read failed.");
        return -1;
    }
    else if(ret == 0)
    {
        LogUtility::Log(LOG_LEVEL_WARN, "SerialConnectionManager::ReadMessage get no data.");
        return -1;
    }
    
    if(m_packetHandler != NULL)
    {
        m_packetHandler->PacketHandler(receiveBuffer, ret);
    }
    return 0;
}

int SerialConnectionManager::Select()
{
    fd_set fds;
    fd_set wfds;
    int max_fd;
    int ret;

    while(!m_exit)
    {
        if(m_fd < 0)
        {
            return -1;
        }
        
        FD_ZERO(&fds);
        FD_SET(m_fd, &fds);
        max_fd = m_fd;
        if(m_blocked)
        {
            FD_ZERO(&wfds);
            FD_SET(m_fd, &wfds);
            ret = select(max_fd +1, &fds, &wfds, NULL, NULL);
        }
        else
        {
            ret = select(max_fd +1, &fds, NULL, NULL, NULL);
        }
        if(ret > 0)
        {
            if(m_blocked && FD_ISSET(m_fd, &wfds))
            {
                Notify(ENUM_CONNECTION_STATE_UNBLOCKED);
                m_blocked = false;
            }
            if(FD_ISSET(m_fd, &fds))
            {
                ret = ReadMessage();
                if(ret < 0)
                {
                    return -1;
                }
            }
        }
        else if(ret < 0)
        {
            if(errno == EINTR)
            {
                LogUtility::Log(LOG_LEVEL_INFO, "SerialConnectionManager::Select got a signal.");
                continue;
            }
            else
            {
                LogUtility::Log(LOG_LEVEL_ERROR, "SerialConnectionManager::Select select failed.");
                return -1;
            }
        }
    }
    
    return 0;
}

static void SignalHandler(int sig)
{
    LogUtility::Log(LOG_LEVEL_INFO, "SignalHandler catch signal 0x%x.", sig);
}

void SerialConnectionManager::WaitInterval()
{
    struct timeval interval;
    interval.tv_sec = MCU_CONNECTION_WAIT_INTERVAL;
    interval.tv_usec = 0;
    
    select(0, NULL, NULL, NULL, &interval);
    return;
}

void *SerialConnectionManager::RxThreadLoop(void *arg)
{
    int ret;

    LogUtility::Log(LOG_LEVEL_DEBUG, "SerialConnectionManager::RxThreadLoop entry.");
    SerialConnectionManager *manager = reinterpret_cast<SerialConnectionManager *> (arg);
    
    signal(SIGUSR1,SignalHandler);
    signal(SIGPIPE,SignalHandler);
    
    while(!manager -> m_exit)
    {
        ret = manager->Connect();
        if(ret < 0)
        {
            LogUtility::Log(LOG_LEVEL_WARN, "SerialConnectionManager::RxThreadLoop connect to mcu failed.");
            manager->WaitInterval();
            LogUtility::Log(LOG_LEVEL_WARN, "SerialConnectionManager::RxThreadLoop try again.");
            continue;
        }

        manager->Notify(ENUM_CONNECTION_STATE_OPENED);
        
        manager->Select();

        manager->Notify(ENUM_CONNECTION_STATE_BROKEN);

        manager->Close();
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "SerialConnectionManager::RxThreadLoop exit.");
    
    return NULL;
}

int SerialConnectionManager::Send(const unsigned char *buf, int len)
{
    int ret;
    int totalSent=0;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "SerialConnectionManager::Send %d.", len);
    printf("sd: time=%d ",(int)time(NULL));
    for(int i=0; i< len; i++)
    {
        printf("%02x ",buf[i]);
    }
    printf("\n");
    if(m_exit)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "SerialConnectionManager::Send already exit.");
        return -1;
    }

    if(m_blocked)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "SerialConnectionManager::Send blocked.");
        return -1;
    }

    if(m_fd < 0)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "SerialConnectionManager::Send not connected.");
        return -1;
    }
    
    pthread_mutex_lock(&m_sendingMutex);
    do{
	    ret = write(m_fd, buf+totalSent, len-totalSent);
	    if(ret < 0)
	    {
		    if((errno == EAGAIN) || (errno == EWOULDBLOCK))
		    {
			    m_blocked = true;
			    pthread_kill(m_rx_tid, SIGUSR1);
			    pthread_mutex_unlock(&m_sendingMutex);
			    return 0;
		    }
		    else
		    {
			    pthread_mutex_unlock(&m_sendingMutex);
			    return -1;
		    }
	    }
	    else if(ret == 0)
	    {
		    m_blocked = true;
		    pthread_kill(m_rx_tid, SIGUSR1);
		    pthread_mutex_unlock(&m_sendingMutex);
		    return 0;
	    }
	    else
	    {
		    totalSent += ret;
	    }
    }while(totalSent < len);
    
    pthread_mutex_unlock(&m_sendingMutex);
    return totalSent;
}

void SerialConnectionManager::Notify(ENUM_SERIAL_CONNECTION_EVENT event)
{
    for(set<SerialConnectionEventHandlerInterface *>::iterator it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it ++)
    {
        SerialConnectionEventHandlerInterface *handler = *it;
        handler->ConnectionEventHandler(event);
    }
    return;
}
