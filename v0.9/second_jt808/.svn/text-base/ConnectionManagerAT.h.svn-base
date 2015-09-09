#ifndef _CONNECTION_MANAGER_AT_H_
#define _CONNECTION_MANAGER_AT_H_

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include "ConnectionManagerInterface.h"
#include "JT808MessageHandler.h"
#include "GeneralTimer.h"

using namespace std;

#define CONNECTION_BUFFER_LEN       64*1024

#define CONNECTION_TIMER_NAME       "CONN_TIMER"

#define M_BUF_SIZE 64*1024
//#define SLEEP
class ConnectionManager
{
public:
    ~ConnectionManager();
    void Start();
	void SleepConnect();
    void Close();
    void Stop();
	void NetBreak(int BreakNum);
	 void Notify(ENUM_CONNECTION_EVENT event);
    static ConnectionManager *Instance();
    static void Destroy();
    int Send(const unsigned char *buf, int len);
    void Reconnect();
    void AddEventHandler(ConnectionEventHandlerInterface * handler) { if(handler) { m_eventHandlers.insert(handler); } }
    void SetPacketHandler(ConnectionPacketHandlerInterface *handler) { m_packetHandler = handler; }
    void HandleTimer(string &name, void *data);
	void HandleReceiveBuf(unsigned char * buf,unsigned int len );
    static int connection_bei;
    bool m_connected;
    ENUM_CONNECTION_ID m_connId;
    bool b_zone;
	int ConnectMainNum;
	int ConnectSecNum;
	ConnectionManager();
    int errornum_register;
	
    
private:
    static void *RxThreadLoop(void *arg);
    int Connect();
    ConnectionManager(const ConnectionManager &);
    ConnectionManager & operator=(ConnectionManager &);
    int sendReq(const unsigned char *buf, int buflen);
    int createServer();
    void receiveData(int fd);
    void processStateChange(int connected);
    int forwardData(const unsigned char *buf, int len);
	

	unsigned char m_sbuf[M_BUF_SIZE];
    bool m_exit;
    pthread_t m_rx_tid;
    pthread_mutex_t m_sendingMutex;
    static ConnectionManager *m_instance;
    static int connection_num;
    set<ConnectionEventHandlerInterface *>m_eventHandlers;
    ConnectionPacketHandlerInterface *m_packetHandler;
    GeneralTimerItem *m_connectTimer;
};
#endif
