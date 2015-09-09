#ifndef _GPS_LOCATION_MESSAGE_HANDLER_H_
#define _GPS_LOCATION_MESSAGE_HANDLER_H_

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
#include "devicemanager.h"
#include "BusRecordTable.h"
#include "McuMessageHandler.h"

using namespace std;

#define GPS_LOCATION_PORT           1015
#define JUDGE_COUNT                 3
#define SAFE_JUDGE_COUNT            20
#define GPS_TIME_COUNT              10

typedef struct {
    int number;
    string name;
    int x;
    int y;
    int distance;
}StruBusStationInfo;

typedef enum {
    BUS_STATE_UNKNOWN = 0,
    BUS_STATE_IN_STATION,
    BUS_STATE_OUT_STATION
}EnumBusState;

typedef enum {
    LED_IN_STATION = 0,
    LED_OUT_STATION = 1
}EnumLedInOutStation;

class GpsLocationMessageHandler
{
public:
    ~GpsLocationMessageHandler();
    void Start();
    void Close();
    void Stop();
    static GpsLocationMessageHandler *Instance();
    static void Destroy();
    
    void RegisterHandler(McuMessageHandler *handler)
    {
        m_mcuHandler = handler;
    }
    
    inline int GetDirection()
    {
        return m_direction;
    }

protected:
    GpsLocationMessageHandler();
    
private:
    static void *RxThreadLoop(void *arg);
    int Connect();
    int ReadMessage();
    void ProcessLoc();
    int GetStations();
    double CalculateDistance(int lat1, int lon1, int lat2, int lon2);
    int SearchStation(StruBusStationInfo *table, int len, int x, int y);
    void RefreshConfig();

    static GpsLocationMessageHandler *m_instance;
    bool m_exit;
    pthread_t m_rx_tid;
    int m_socket;
    
    McuMessageHandler *m_mcuHandler;
    StruBusStationInfo *m_upTable;
    int m_upTableLen;
    StruBusStationInfo *m_downTable;
    int m_downTableLen;
    EnumBusState m_state;
    int m_station;
    int m_direction;
    int m_count;
    int m_safeCount;
    BusRunStateTable m_runTable;
    int m_gpsCount;
    
    unsigned char m_ledMode;
    unsigned char m_ledSpeed;
    unsigned char m_ledLoopTimes;
};
#endif
