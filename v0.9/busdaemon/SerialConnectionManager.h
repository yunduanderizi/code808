#ifndef _SERIAL_CONNECTION_MANAGER_H_
#define _SERIAL_CONNECTION_MANAGER_H_

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

using namespace std;

#define CONNECTION_BUFFER_LEN       4096
#define MCU_BAUD_RATE               B9600
#define MCU_SERIAL_PORT             "/dev/tcc-uart3"
#define MCU_CONNECTION_WAIT_INTERVAL 5

typedef enum
{
    ENUM_CONNECTION_STATE_BROKEN = 0x00,
    ENUM_CONNECTION_STATE_OPENED = 0x01,
    ENUM_CONNECTION_STATE_UNBLOCKED = 0x02
} ENUM_SERIAL_CONNECTION_EVENT;

class SerialConnectionEventHandlerInterface
{
public:
    virtual int ConnectionEventHandler(ENUM_SERIAL_CONNECTION_EVENT state) = 0;
};

class SerialPacketHandlerInterface
{
public:
    virtual int PacketHandler(unsigned char *buf, int len) = 0;
};

class SerialConnectionManager
{
public:
    ~SerialConnectionManager();
    void Start();
    void Close();
    void Stop();
    static SerialConnectionManager *Instance();
    static void Destroy();
    int Send(const unsigned char *buf, int len);
    void AddEventHandler(SerialConnectionEventHandlerInterface * handler) { if(handler) { m_eventHandlers.insert(handler); } }
    void SetPacketHandler(SerialPacketHandlerInterface *handler) { m_packetHandler = handler; }

protected:
    SerialConnectionManager();
    
private:
    static void *RxThreadLoop(void *arg);
    int Connect();
    int ReadMessage();
    int Select();
    void WaitInterval();
    void Notify(ENUM_SERIAL_CONNECTION_EVENT event);

    bool m_exit;
    pthread_t m_rx_tid;
    pthread_mutex_t m_sendingMutex;
    int m_fd;
    struct termios m_termios_old;
    bool m_blocked;
    static SerialConnectionManager *m_instance;
    set<SerialConnectionEventHandlerInterface *>m_eventHandlers;
    SerialPacketHandlerInterface *m_packetHandler;
};
#endif
