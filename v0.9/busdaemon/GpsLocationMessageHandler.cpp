#include <errno.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include "GpsLocationMessageHandler.h"
#include "LogUtility.h"
#include "BusStationTable.h"
#include "BusParaTable.h"
#include "busmessage.h"

GpsLocationMessageHandler *GpsLocationMessageHandler::m_instance = NULL;
static struct NaviGPSInfo m_locData;

GpsLocationMessageHandler *GpsLocationMessageHandler::Instance()
{
    if(!m_instance)
    {
        m_instance = new GpsLocationMessageHandler();
    }
    
    return m_instance;
}

void GpsLocationMessageHandler::Destroy()
{
    if(m_instance != NULL)
    {
        delete m_instance;
    }
}

GpsLocationMessageHandler::GpsLocationMessageHandler()
{
    m_exit = true;
    m_socket = -1;
    m_rx_tid = -1;
    
    m_mcuHandler = NULL;
    m_upTable = NULL;
    m_downTable = NULL;
    m_upTableLen = 0;
    m_downTableLen = 0;
    
    m_count = 0;
    m_safeCount = 0;
    m_gpsCount = 0;
    
    GetStations();
    
    m_state = BUS_STATE_UNKNOWN;
    m_station = -1;
    m_direction = -1;
    
    RefreshConfig();
}

GpsLocationMessageHandler::~GpsLocationMessageHandler()
{
    Stop();
    
    if(m_upTable)
    {
        delete m_upTable;
    }
    
    if(m_downTable)
    {
        delete m_downTable;
    }
}

void GpsLocationMessageHandler::Start()
{
    if(m_exit == true)
    {
        m_exit = false;
        pthread_create(&m_rx_tid, NULL, this->RxThreadLoop, this);
    }
    
    return;
}

void GpsLocationMessageHandler::Stop()
{
    if(m_exit != true)
    {
        m_exit = true;
        pthread_kill(m_rx_tid, SIGUSR1);
        pthread_join(m_rx_tid, NULL);
    }

    return;
}

void GpsLocationMessageHandler::Close()
{
    if(m_socket >= 0)
    {
        close(m_socket);
        m_socket = -1;
    }
}

int GpsLocationMessageHandler::Connect()
{
    struct sockaddr_in addr_gps; 

    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::Connect entry.");
    
    m_socket = socket(AF_INET,SOCK_DGRAM,0);
    if( -1 == m_socket )
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "GpsLocationMessageHandler::Connect create socket failed.");
        return -1;
    }

    addr_gps.sin_family=AF_INET;
    addr_gps.sin_addr.s_addr=inet_addr("127.0.0.1");
    addr_gps.sin_port = htons(GPS_LOCATION_PORT);
    
    bind(m_socket, (struct sockaddr*)&addr_gps, sizeof(addr_gps));

    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::Connect exit.");
    
    return 0;
}

int GpsLocationMessageHandler::ReadMessage()
{
    struct sockaddr client;
    int len = sizeof(struct sockaddr);

    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::ReadMessage.");
    int ret = recvfrom(m_socket, &m_locData, sizeof(m_locData), 0, &client, (socklen_t *)&len);
    if(ret == sizeof(m_locData))
    {
        LogUtility::Log(LOG_LEVEL_INFO, "GpsLocationMessageHandler::ReadMessage recv %d bytes.", ret);
        ProcessLoc();
    }
    
    return 0;
}

static void SignalHandler(int sig)
{
    LogUtility::Log(LOG_LEVEL_INFO, "SignalHandler catch signal 0x%x.", sig);
}

void *GpsLocationMessageHandler::RxThreadLoop(void *arg)
{
    string serverAddress;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::RxThreadLoop entry.");
    GpsLocationMessageHandler *manager = reinterpret_cast<GpsLocationMessageHandler *> (arg);
    
    signal(SIGUSR1,SignalHandler);
    signal(SIGPIPE,SignalHandler);
    
    manager->Connect();
    
    while(!manager->m_exit)
    {
        manager->ReadMessage();
    }
    
    manager->Close();
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::RxThreadLoop exit.");
    
    return NULL;
}

void GpsLocationMessageHandler::ProcessLoc()
{
    int x, y;
    int dir;
    StruBusStationInfo *curTable=NULL;
    int curTableLen;
    int ret;

    m_runTable.GetDirection(dir);
    if(dir != m_direction)
    {
        m_direction = dir;
        m_mcuHandler->SendLEDStateByDir(dir);
        m_state = BUS_STATE_UNKNOWN;
        if(curTable != NULL)
        {
            SendStationPassengerNotify(curTable[m_station].number, curTable[m_station].name,
                m_mcuHandler->m_passengerup_count,m_mcuHandler->m_passengerdown_count);
        }
        m_mcuHandler->HandleUpDownDirection(dir);
    }
    
    if(!m_locData.position)
    {
        m_count = 0;
        m_gpsCount = 0;
        return;
    }
    
    if(m_gpsCount == GPS_TIME_COUNT)
    {
        time_t ct = time(NULL);
        m_mcuHandler->SendPOSTime(&ct);
        m_mcuHandler->SendLEDTime(&ct);
        m_gpsCount ++;
    }
    else if(m_gpsCount < GPS_TIME_COUNT)
    {
        m_gpsCount ++;
    }

    if(dir == 0) {
        curTable = m_upTable;
        curTableLen = m_upTableLen;
    }
    else
    {
        curTable = m_downTable;
        curTableLen = m_downTableLen;
    }
    
    x = m_locData.fY;
    y = m_locData.fX;
    /*
    if(m_locData.ilatitude)
    {
        x = -x;
    }
    if(m_locData.ilongitude)
    {
        y = -y;
    }
    */
    switch(m_state)
    {
        case BUS_STATE_IN_STATION:
            if(CalculateDistance(curTable[m_station].x, curTable[m_station].y, x, y) <= curTable[m_station].distance)
            {
                m_count = 0;
                break;
            }
            m_count ++;
            if(m_count >= JUDGE_COUNT)
            {
                m_state = BUS_STATE_OUT_STATION;
                if(m_station < (curTableLen - 1))
                {
                    m_station ++;
                    SendStationPassengerNotify(curTable[m_station-1].number, curTable[m_station-1].name,
                        m_mcuHandler->m_passengerup_count,m_mcuHandler->m_passengerdown_count);
                }
                else
                {
                    SendStationPassengerNotify(curTable[m_station].number, curTable[m_station-1].name,
                        m_mcuHandler->m_passengerup_count,m_mcuHandler->m_passengerdown_count);
                }
                SendStationInfoNotify(curTable[m_station].number, curTable[m_station].name, LED_OUT_STATION);
                m_mcuHandler->SendLEDStation(m_ledMode, m_ledSpeed, m_ledLoopTimes, (unsigned char)LED_OUT_STATION, curTable[m_station].number);
                m_mcuHandler->SendPOSStation(curTable[m_station].number, 0x00, m_direction);
                LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::ProcessLoc out station %d.",curTable[m_station].number);
            }
            break;
        case BUS_STATE_OUT_STATION:
            if(CalculateDistance(curTable[m_station].x, curTable[m_station].y, x, y) > curTable[m_station].distance)
            {
                m_count = 0;
                m_safeCount ++;
                if(m_safeCount >= SAFE_JUDGE_COUNT)
                {
                    m_safeCount = 0;
                    if((ret = SearchStation(curTable, curTableLen, x, y)) >= 0)
                    {
                        m_station = ret;
                    }
                }
                break;
            }
            m_safeCount = 0;
            m_count ++;
            if(m_count >= JUDGE_COUNT)
            {
                m_count = 0;
                SendStationInfoNotify(curTable[m_station].number, curTable[m_station].name, LED_IN_STATION);
                m_mcuHandler->SendLEDStation(m_ledMode, m_ledSpeed, m_ledLoopTimes, (unsigned char)LED_IN_STATION, curTable[m_station].number);
                m_state = BUS_STATE_IN_STATION;
                LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::ProcessLoc in station %d.",curTable[m_station].number);
            }

            break;
        case BUS_STATE_UNKNOWN:
        default:
            if((ret = SearchStation(curTable, curTableLen, x, y)) < 0)
            {
                m_count = 0;
                break;
            }
            if(m_station == ret)
            {
                m_count ++;
            }
            else
            {
                m_station = ret;
                m_count = 1;
            }
            
            if(m_count >= JUDGE_COUNT)
            {
                m_count = 0;
                m_state = BUS_STATE_IN_STATION;
                SendStationInfoNotify(curTable[m_station].number, curTable[m_station].name, LED_IN_STATION);
                m_mcuHandler->SendLEDStation(m_ledMode, m_ledSpeed, m_ledLoopTimes, (unsigned char)LED_IN_STATION, curTable[m_station].number);
                m_mcuHandler->SendPOSStation(curTable[m_station].number, 0x00, m_direction);
            }
            break;
    }
    
    return;
}

int GpsLocationMessageHandler::SearchStation(StruBusStationInfo *table, int len, int x, int y)
{
    for(int i = 0; i < len; i ++)
    {
        if(CalculateDistance(table[i].x, table[i].y, x, y) < table[i].distance)
        {
            return i;
        }
    }
    
    return -1;
}

int GpsLocationMessageHandler::GetStations()
{
    BusStationUpTable upTable;
    BusStationDownTable downTable;
    int ret;
    int len;
    int flag;
    
    ret = upTable.RefreshAll(BUS_STATION_NUMBER_ID);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::GetStations refresh up table failed.");
        return -1;
    }
    
    len = upTable.GetResultsCount();
    
    if(m_upTable)
    {
        delete m_upTable;
    }
    
    m_upTable = new StruBusStationInfo[len];
    if(m_upTable == NULL)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::GetStations m_upTable new` failed.");
        return -1;
    }

    for(int i = 0; i < len; i ++)
    {
        upTable.GetIntValue(i, BUS_STATION_NUMBER_ID, m_upTable[i].number);
        upTable.GetStringValue(i, BUS_STATION_NAME, m_upTable[i].name);
        upTable.GetIntValue(i, BUS_STATION_LOC_FLAG, flag);
        upTable.GetIntValue(i, BUS_STATION_LATITUDE, m_upTable[i].x);
        upTable.GetIntValue(i, BUS_STATION_LONGITUDE, m_upTable[i].y);
        upTable.GetIntValue(i, BUS_STATION_DISTANCE, m_upTable[i].distance);
        if(flag & 0x04)
        {
            m_upTable[i].x = -m_upTable[i].x;
        }
        if(flag & 0x08)
        {
            m_upTable[i].y = -m_upTable[i].y;
        }
    }
    
    m_upTableLen = len;
    
    ret = downTable.RefreshAll(BUS_STATION_NUMBER_ID);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::GetStations refresh down table failed.");
        return -1;
    }
    
    len = downTable.GetResultsCount();
    
    if(m_downTable)
    {
        delete m_downTable;
    }
    
    m_downTable = new StruBusStationInfo[len];
    if(m_downTable == NULL)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "GpsLocationMessageHandler::GetStations m_downTable new` failed.");
        return -1;
    }

    for(int i = 0; i < len; i ++)
    {
        downTable.GetIntValue(i, BUS_STATION_NUMBER_ID, m_downTable[i].number);
        downTable.GetStringValue(i, BUS_STATION_NAME, m_downTable[i].name);
        downTable.GetIntValue(i, BUS_STATION_LOC_FLAG, flag);
        downTable.GetIntValue(i, BUS_STATION_LATITUDE, m_downTable[i].x);
        downTable.GetIntValue(i, BUS_STATION_LONGITUDE, m_downTable[i].y);
        downTable.GetIntValue(i, BUS_STATION_DISTANCE, m_downTable[i].distance);
        if(flag & 0x04)
        {
            m_downTable[i].x = -m_downTable[i].x;
        }
        if(flag & 0x08)
        {
            m_downTable[i].y = -m_downTable[i].y;
        }

    }
    
    m_downTableLen = len;
    
    return 0;
}

double GpsLocationMessageHandler::CalculateDistance(int lat1, int lon1, int lat2, int lon2)
{
    #define DEGREE_TO_RADIU  (3.14159265354 / (1000000.0 * 180))
    double lat_radius1 = lat1 * DEGREE_TO_RADIU;
    double lon_radius1 = lon1 * DEGREE_TO_RADIU;
    double lat_radius2 = lat2 * DEGREE_TO_RADIU;
    double lon_radius2 = lon2 * DEGREE_TO_RADIU;
    double delta_lat = lat_radius2 - lat_radius1;
    double delta_lon = lon_radius2 - lon_radius1;
    
    double delta_lat_sin = sin(delta_lat / 2);
    double delta_lon_sin = sin(delta_lon / 2);
    
    double a = delta_lat_sin * delta_lat_sin + delta_lon_sin * delta_lon_sin * cos(lat_radius1) * cos(lat_radius2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return fabs(c * 6371 * 1000);
}

void GpsLocationMessageHandler::RefreshConfig()
{
    BusRunParaTable table;
    int value;
    
    table.GetIntValue(BUS_RUN_PARA_LED_MODE, value);
    m_ledMode = (unsigned char)value;
    table.GetIntValue(BUS_RUN_PARA_LED_SPEED, value);
    m_ledSpeed = (unsigned char)value;
    table.GetIntValue(BUS_RUN_PARA_LED_LOOP_TIMES, value);
    m_ledLoopTimes = (unsigned char)value;
}
