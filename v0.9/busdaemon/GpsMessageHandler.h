#ifndef _GPS_MESSAGE_HANDLER_H_
#define _GPS_MESSAGE_HANDLER_H_

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <set>
#include <map>
#include <sstream>
#include "McuMessageHandler.h"

using namespace std;

#define CONNECTION_BUFFER_LEN       4096
#define GPS_PORT                    1014

class GpsMessageHandler
{
public:
    ~GpsMessageHandler();
    void Start();
    void Close();
    void Stop();
    static GpsMessageHandler *Instance();
    static void Destroy();
    inline void RegisterHandler(McuMessageHandler *handler)
    {
        m_handler = handler;
    }

protected:
    GpsMessageHandler();
    
private:
    static void *RxThreadLoop(void *arg);
    int Connect();
    int ReadMessage();

    static GpsMessageHandler *m_instance;
    bool m_exit;
    pthread_t m_rx_tid;
    int m_socket;
    McuMessageHandler *m_handler;
};
#endif
