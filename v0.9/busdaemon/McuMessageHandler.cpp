#include "McuMessageHandler.h"
#include "LogUtility.h"
#include "SerialConnectionManager.h"
#include "Utility.h"
#include <dbusServer.h>
#include <dbusApplication.h>
#include <sstream>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "BusParaTable.h"
#include "busmessage.h"
#include "DeviceManagerDbusMessage.h"
#include "GpsLocationMessageHandler.h"

#define LED_FLAG_STATION (0x1 << 0)
#define LED_FLAG_STATE   (0x1 << 1)
#define LED_FLAG_TIME    (0x1 << 2)
#define LED_FLAG_DIRECT  (0x1 << 3)

static unsigned short LEDID[] = {MCU_PACKET_LED1, MCU_PACKET_LED2, MCU_PACKET_LED3, MCU_PACKET_LED4};
static unsigned char LEDFLAG[] = {LED_FLAG_STATION|LED_FLAG_TIME|LED_FLAG_DIRECT, LED_FLAG_DIRECT, LED_FLAG_STATE|LED_FLAG_DIRECT, 0x0};

static void McuTimerHandler(string &name, void *data, void *data2)
{
    McuMessageHandler *handler = (McuMessageHandler *)data;
    if(handler != NULL)
    {
        handler->HandleTimer(name, data2);
    }
}

McuMessageHandler::McuMessageHandler()
{
    m_retryTimer = CreateTimer(MCU_RETRY_TIMER_NAME, TIMER_SEC_TO_MSEC(MCU_RETRY_TIME_OUT), this, McuTimerHandler, false);
    m_CanDataTimer = CreateTimer(MCU_CAN_DATA_TIMER, TIMER_SEC_TO_MSEC(MCU_CAN_DATA_INTERVAL), this, McuTimerHandler, true);
    m_CanUploadTimer = CreateTimer(MCU_CAN_UPLOAD_TIMER, TIMER_SEC_TO_MSEC(MCU_CAN_UPLOAD_INTERVAL), this, McuTimerHandler, true);
    m_waitingQueue.set_capacity(McuPacketFifo::capacity_type(MCU_MAX_WAITING_QUEUE));
    m_rxPacket.set_capacity(McuPacket::capacity_type(MCU_MAX_PACKET_SIZE, MCU_MIN_PACKET_SIZE));
    ClearRxPacket();
    m_seq = 0;
    m_handshake = false;
    m_pos_num = 0;
    SerialConnectionManager::Instance()->AddEventHandler(this);
    SerialConnectionManager::Instance()->SetPacketHandler(this);
    
    BusDeployParaTable table;
    table.GetSequenceValue(BUS_DEPLOY_PARA_POS_PASSWORD, m_pos_pass);
    
    BusRunParaTable rtable;
    rtable.GetIntValue(BUS_RUN_PARA_PASSENGER_DATA_UPLOAD_INTERVAL, m_dealUploadInterval);
    m_dealReportTimer = CreateTimer(MCU_DEAL_UPLOAD_TIMER, TIMER_SEC_TO_MSEC(m_dealUploadInterval), this, McuTimerHandler, true);
    
    m_passengerRecord = NULL;
    
    m_record_fd = -1;
    m_can_fd = -1;
    m_can_num = 0;
    
    m_driving_state = 0x03;
    
    if(pthread_mutex_init(&m_queueMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::McuMessageHandler: mutex init failed.");
        return;
    }
    if(pthread_mutex_init(&m_packetMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::McuMessageHandler: mutex init failed.");
        return;
    }
    if(pthread_mutex_init(&m_dealRecordMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::McuMessageHandler: mutex init failed.");
        return;
    }
    if(pthread_mutex_init(&m_candataMutex, NULL))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::McuMessageHandler: mutex init failed.");
        return;
    }
}

McuMessageHandler::~McuMessageHandler()
{
    DestroyTimer(m_retryTimer);
    if(m_passengerRecord)
    {
        for(int i = 0; i < m_pos_num; i ++)
        {
            DestroyTimer(m_passengerRecord[i].posPassengerRecordTimer);
        }
    }
    delete []m_passengerRecord;

    ClearQueue();
    ClearRxPacket();
    pthread_mutex_lock(&m_dealRecordMutex);
    if(m_record_fd >= 0)
    {
        close(m_record_fd);
        m_record_fd = -1;
    }
    pthread_mutex_unlock(&m_dealRecordMutex);
    pthread_mutex_lock(&m_candataMutex);
    if(m_can_fd >= 0)
    {
        close(m_can_fd);
        m_can_fd = -1;
    }
    pthread_mutex_unlock(&m_candataMutex);
    pthread_mutex_destroy(&m_queueMutex);
    pthread_mutex_destroy(&m_packetMutex);
    pthread_mutex_destroy(&m_dealRecordMutex);
    pthread_mutex_destroy(&m_candataMutex);
}

void McuMessageHandler::ClearQueue()
{
    pthread_mutex_lock(&m_queueMutex);

    for(McuPacketFifo::iterator it = m_waitingQueue.begin(); it != m_waitingQueue.end(); it ++)
    {
        StruMcuPacket *packet = *it;
        DestroyPacket(packet);
    }
    m_waitingQueue.clear();

    pthread_mutex_unlock(&m_queueMutex);
}

int McuMessageHandler::ConnectionEventHandler(ENUM_SERIAL_CONNECTION_EVENT state)
{
    LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::ConnectionEventHandler state %x.", (int)state);
    
    switch(state)
    {
        case ENUM_CONNECTION_STATE_OPENED:
            SendHandshake();
            break;
        case ENUM_CONNECTION_STATE_BROKEN:
            StopTimer(m_retryTimer);
            ClearQueue();
            ClearRxPacket();
            m_handshake = false;
            break;
        default:
            break;
    }
    
    return 0;
}

int McuMessageHandler::PacketHandler(unsigned char *buf, int len)
{
    for(int i = 0; i < len; i ++)
    {
        unsigned char ch = buf[i];
        
        if(m_rxPacket.size() >= MCU_MAX_PACKET_SIZE)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::PacketHandler buffer overflow.");
            ClearRxPacket();
            continue;
        }

        if(ch == MCU_PACKET_FLAG)
        {
            if(m_packet_start)
            {
                m_rxPacket.push_back(ch);
                if(m_rxPacket.size() < MCU_MIN_PACKET_SIZE)
                {
                    LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::PacketHandler too short.");
                    ClearRxPacket();
                    continue;
                }
                HandlePacket();
                ClearRxPacket();
            }
            else
            {
                m_rxPacket.push_back(ch);
                m_packet_start = true;
                m_packet_escape = false;
            }
            continue;
        }
        
        if(!m_packet_start)
        {
            continue;
        }
        
        if(ch == MCU_PACKET_ESCAPE)
        {
            if(m_packet_escape)
            {
                m_rxPacket.push_back(MCU_PACKET_ESCAPE);
            }
            m_packet_escape = true;
            continue;
        }
        
        if(m_packet_escape)
        {
            if(ch == 0x01)
            {
                m_rxPacket.push_back(MCU_PACKET_ESCAPE);
            }
            else if(ch == 0x02)
            {
                m_rxPacket.push_back(MCU_PACKET_FLAG);
            }
            else
            {
                m_rxPacket.push_back(MCU_PACKET_ESCAPE);
                m_rxPacket.push_back(ch);
            }
            m_packet_escape = false;
            continue;
        }
        
        m_rxPacket.push_back(ch);
    }
    
    return 0;
}

void McuMessageHandler::HandlePacket()
{
    unsigned short seq = MCU_GET_SHORT(m_rxPacket, 2);
    unsigned short id = MCU_GET_SHORT(m_rxPacket, 4);
    unsigned char len = MCU_GET_BYTE(m_rxPacket, 6);
    unsigned char checksum = 0;
    int packetsize = m_rxPacket.size();
    printf("rx:");
    for(int j = 0; j<packetsize; j++)
    {
        printf("%02x ",m_rxPacket[j]);
    }
    printf("\n");

    if((packetsize - 8) != len)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::HandlePacket length not matching(%d, %d).", packetsize, len);
        return;
    }

    for(int i = 2; i < packetsize - 1; i ++)
    {
        checksum ^= m_rxPacket[i];
    }
    
    if(checksum != m_rxPacket[1])
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::HandlePacket checksum not matching(%02x, %02x).", checksum, m_rxPacket[1]);
        return;
    }
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::HandlePacket check pass(%04x, %d, %d).", id, len, seq);
    
    switch(id)
    {
        case MCU_PACKET_STATION_REPORT:
        case MCU_PACKET_LED1:
        case MCU_PACKET_LED2:
        case MCU_PACKET_LED3:
        case MCU_PACKET_LED4:
            ProcessCommonReply(len, id, seq);
            break;
        case MCU_PACKET_HANDSHAKE:
            if(m_handshake == false)
            {
                ProcessHandshake(len, seq);
            }
            if(m_handshake == false)
            {
                SendHandshake();
            }
            break;
        case MCU_POS_STATION:
            ProcessPosStation(len, seq);
            break;
        case MCU_POS_DATE:
            ProcessPosDate(len, seq);
            break;
        case MCU_POS_TIME:
            ProcessPosTime(len, seq);
            break;
        case MCU_POS_GET_PASSENGER:
            ProcessGetPassenger(len, seq);
            break;
        case MCU_POS_GET_DEAL:
            ProcessGetDeal(len, seq);
            break;
        case MCU_CAN_GET_DATA:
            ProcessGetCanData(len, seq);
            break;
        default:
            LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::HandlePacket unknown message.");
            break;
    }
}

void McuMessageHandler::ProcessCommonReply(int len, int id, int seq)
{
    if(len != 1)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessCommonReply len invalid.");
        return;
    }

    int result = MCU_GET_BYTE(m_rxPacket, 7);
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::ProcessCommonReply %d, %04x, %d.", seq, id, result);

    PopQueue(id, seq, NULL, NULL);
}

void McuMessageHandler::ProcessHandshake(int len, int seq)
{
    if(len < 2)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessHandshake len invalid.");
        return;
    }
    
    if(PopQueue(MCU_PACKET_HANDSHAKE, seq, NULL, NULL) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessHandshake out of order.");
        return;
    }
    
    unsigned char flag = MCU_GET_BYTE(m_rxPacket, 7);
    if(flag != 0x01)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessHandshake invalid flag %d.", flag);
        return;
    }
    
    unsigned char number = MCU_GET_BYTE(m_rxPacket, 8);
    
    if((len < number + 1) || (number > MCU_MAX_POS))
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessHandshake number invalid %d.", number);
        return;
    }
    
    m_pos_num = number;
    LogUtility::Log(LOG_LEVEL_INFO, "McuMessageHandler::ProcessHandshake pos number %d.", m_pos_num);
    
    for(int i = 0; i < number; i ++)
    {
        m_pos_addr[i] = MCU_GET_BYTE(m_rxPacket, 9 + i);
        LogUtility::Log(LOG_LEVEL_INFO, "McuMessageHandler::ProcessHandshake pos %d addr: %d.", i, m_pos_addr[i]);
    }
    
    m_handshake = true;

    m_passengerRecord = new StruPassengerRecord[m_pos_num];
    for(int i = 0; i < m_pos_num; i ++)
    {
        m_passengerRecord[i].passengerUpCount = 0;
        m_passengerRecord[i].accuPassengerUpCount = 0;
        m_passengerRecord[i].passengerDownCount = 0;
        m_passengerRecord[i].accuPassengerDownCount = 0;
        m_passengerRecord[i].dealCount = 0;
        m_passengerRecord[i].lastDealCount = 0;
        m_passengerRecord[i].posPassengerRecordTimer = CreateTimer(MCU_PASSENGER_RECORD_TIMER, 
                                                                   TIMER_SEC_TO_MSEC(MCU_PASSENGER_INTERVAL + i), 
                                                                   this, 
                                                                   McuTimerHandler, 
                                                                   false);
        m_passengerRecord[i].state = STATE_TIMER;
        m_passengerRecord[i].posPassengerRecordTimer->data2 = (void *)i;
        StartTimer(m_passengerRecord[i].posPassengerRecordTimer);
    }
    m_passengerup_count = 0;
    m_passengerdown_count = 0;
    StartTimer(m_dealReportTimer);
    StartTimer(m_CanDataTimer);
    StartTimer(m_CanUploadTimer);
    return;
}

void McuMessageHandler::ProcessPosStation(int len, int seq)
{
    if(len < 10)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessPosStation len invalid.");
        return;
    }
    
    if(PopQueue(MCU_POS_STATION, seq, NULL, NULL) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessPosStation out of order.");
        return;
    }
    
    return;
}

void McuMessageHandler::ProcessPosDate(int len, int seq)
{
    if(len != 0x0A)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessPosDate len invalid.");
        return;
    }
    
    if(PopQueue(MCU_POS_DATE, seq, NULL, NULL) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessPosDate out of order.");
        return;
    }
    
    return;
}

void McuMessageHandler::ProcessPosTime(int len, int seq)
{
    if(len != 0x0A)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessPosTime len invalid.");
        return;
    }
    
    if(PopQueue(MCU_POS_TIME, seq, NULL, NULL) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessPosTime out of order.");
        return;
    }
    
    return;
}

void McuMessageHandler::ProcessGetPassenger(int len, int seq)
{
    unsigned long data;
    unsigned long data2;
    if(len < 0x0A)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessGetPassenger len invalid.");
        return;
    }
    
    if(PopQueue(MCU_POS_GET_PASSENGER, seq, &data, &data2) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessGetPassenger out of order.");
        return;
    }
    
    if(data == (unsigned long)CLEAR_RECORD)
    {
        LogUtility::Log(LOG_LEVEL_INFO, "McuMessageHandler::ProcessGetPassenger get clear response %d.", (int)data2);
        m_passengerRecord[(int)data2].passengerUpCount = 0;
        m_passengerRecord[(int)data2].passengerDownCount = 0;
        m_passengerRecord[(int)data2].posPassengerRecordTimer->timeout = TIMER_SEC_TO_MSEC(MCU_PASSENGER_INTERVAL),
        StartTimer(m_passengerRecord[(int)data2].posPassengerRecordTimer);
        return;
    }
    
    unsigned short dealnumber = MCU_GET_SHORT(m_rxPacket, 11);
    unsigned short upnumber = MCU_GET_SHORT(m_rxPacket, 13);
    unsigned short downnumber = MCU_GET_SHORT(m_rxPacket, 15);
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::ProcessGetPassenger  delnum:%d up:%d down: %d",dealnumber,upnumber,downnumber);

    if((upnumber <= m_passengerRecord[(int)data2].passengerUpCount) &&
       (downnumber <= m_passengerRecord[(int)data2].passengerDownCount) &&
       ((upnumber != 0) || (downnumber != 0)))
    {
        SendGetPosPassengerRecord((int)data2, CLEAR_RECORD);
    }
    else
    {
        m_passengerRecord[(int)data2].accuPassengerUpCount += upnumber;
        m_passengerRecord[(int)data2].accuPassengerDownCount += downnumber;
        m_passengerup_count += upnumber;
        m_passengerdown_count += downnumber;

        if((upnumber!=0)||(downnumber!=0))
        {
            SendPosPassengerNotify((int)data2, m_pos_addr[(int)data2], 
                                m_passengerup_count, m_passengerdown_count);
        }
    }

    m_passengerRecord[(int)data2].dealCount = dealnumber;
    m_passengerRecord[(int)data2].passengerUpCount = upnumber;
    m_passengerRecord[(int)data2].passengerDownCount = downnumber;
    if(m_passengerRecord[(int)data2].lastDealCount == 0)
        m_passengerRecord[(int)data2].lastDealCount = dealnumber;
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::ProcessGetPassenger  (%d,%d)(%d,%d)(%d,%d)",
       m_passengerup_count,m_passengerdown_count,m_passengerRecord[(int)data2].accuPassengerUpCount,m_passengerRecord[(int)data2].accuPassengerDownCount,
       dealnumber,m_passengerRecord[(int)data2].lastDealCount);

    if(dealnumber && m_passengerRecord[(int)data2].lastDealCount < dealnumber)
    {
        m_passengerRecord[(int)data2].state = STATE_DEAL_SENT;
        //SendGetDealRecord((int)data2, CLEAR_RECORD, dealnumber);
        m_passengerRecord[(int)data2].lastDealCount ++;
        SendGetDealRecord((int)data2, KEEP_RECORD, m_passengerRecord[(int)data2].lastDealCount);
    }
    else
    {
        m_passengerRecord[(int)data2].state = STATE_TIMER;
        m_passengerRecord[(int)data2].posPassengerRecordTimer->timeout = TIMER_SEC_TO_MSEC(MCU_PASSENGER_INTERVAL);
        StartTimer(m_passengerRecord[(int)data2].posPassengerRecordTimer);
    }
    
    return;
}

void McuMessageHandler::ProcessGetDeal(int len, int seq)
{
    unsigned long data;
    unsigned long data2;
    vector<unsigned char> vrecord;
    unsigned char dlen = MCU_GET_BYTE(m_rxPacket, 8);
    
    if(len < 0x46)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessGetDeal len invalid.");
        return;
    }
    
    if(PopQueue(MCU_POS_GET_DEAL, seq, &data, &data2) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessGetDeal out of order.");
        return;
    }
    
    for(int i = 0; i < 96; i ++)
    {
        vrecord.push_back(MCU_GET_BYTE(m_rxPacket, 13 + i));
    }

    WriteToFile(vrecord);
    
    m_passengerRecord[(int)data2].state = STATE_TIMER;
    m_passengerRecord[(int)data2].posPassengerRecordTimer->timeout = TIMER_SEC_TO_MSEC(MCU_PASSENGER_INTERVAL);
    StartTimer(m_passengerRecord[(int)data2].posPassengerRecordTimer);
    
    return;
}
void McuMessageHandler::ProcessGetCanData(int len, int seq)
{
    unsigned long data;
    unsigned long data2;
    vector<unsigned char> vrecord;
    if(len < 16)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessGetCanData len invalid.");
        return;
    }
    
    if(PopQueue(MCU_CAN_GET_DATA, seq, &data, &data2) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::ProcessGetCanData out of order.");
        return;
    }
    
    for(int i = 0; i < 16; i ++)
    {
        vrecord.push_back(MCU_GET_BYTE(m_rxPacket, 7 + i));
    }

    int dir = GpsLocationMessageHandler::Instance()->GetDirection();
    if(dir == 0)
    {
        vrecord.push_back(0x00);
    }
    else
    {
        vrecord.push_back(0x01);
    }
    CanDataWriteToFile(vrecord);

    return;
}
int McuMessageHandler::PopQueue(int id, int seq, unsigned long *data, unsigned long *data2)
{
    int ret = 0;
    pthread_mutex_lock(&m_queueMutex);

    if(m_waitingQueue.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::PopQueue waiting queue empty.");
        pthread_mutex_unlock(&m_queueMutex);
        return -1;
    }

    StruMcuPacket *packet = *(m_waitingQueue.begin());
    
    if((packet->id == id) && (packet->seq == seq))
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::PopQueue correct resp.");
        if(data)
        {
            *data = packet->data;
        }
        if(data2)
        {
            *data2 = packet->data2;
        }
        m_waitingQueue.pop_front();
        DestroyPacket(packet);
        StopTimer(m_retryTimer);
        if(m_waitingQueue.size() > 0)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::PopQueue send queue");
            SendQueue();
        }
    }
    else
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::PopQueue incorrect resp(%d, %04x).", packet->seq, packet->id);
        ret = -1;
    }
    
    pthread_mutex_unlock(&m_queueMutex);
    
    return ret;
}

void McuMessageHandler::PacketCheckSumFlag(StruMcuPacket *thepacket)
{
    unsigned char checksum = 0;
    
    McuPacket &packet = thepacket->packet;
    
    for(McuPacket::iterator it = packet.begin(); it != packet.end(); it ++)
    {
        checksum ^= (*it);
    }
    
    PushByteFront(packet, checksum);

    for(McuPacket::iterator it = packet.begin(); it != packet.end(); it ++)
    {
        if(*it == MCU_PACKET_FLAG)
        {
            *it = MCU_PACKET_ESCAPE;
            if(++it == packet.end())
            {
                PushByte(packet, 0x02);
                break;
            }
            else
            {
                it = packet.insert(it, 0x02);
            }
            continue;
        }
        else if(*it == MCU_PACKET_ESCAPE)
        {
            if(++it == packet.end())
            {
                PushByte(packet, 0x01);
                break;
            }
            else
            {
                it = packet.insert(it, 0x01);
            }
        }
    }

    PushByteFront(packet, MCU_PACKET_FLAG);
    PushByte(packet, MCU_PACKET_FLAG);
}

void McuMessageHandler::PacketHead(StruMcuPacket *thepacket, unsigned short id)
{
    unsigned char len;

    McuPacket &packet = thepacket->packet;
    
    len = (unsigned char)packet.size();
    
    LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::PacketHead Count len %d", len);
    
    PushByteFront(packet, len);
    PushWordFront(packet, id);
    PushWordFront(packet, m_seq);

    pthread_mutex_lock(&m_packetMutex);

    thepacket->seq = m_seq;
    thepacket->id = id;
    m_seq ++;

    pthread_mutex_unlock(&m_packetMutex);

}

void McuMessageHandler::SendBody(StruMcuPacket *packet, unsigned short id)
{
    if((m_handshake == false) && (id != MCU_PACKET_HANDSHAKE))
    {
        LogUtility::Log(LOG_LEVEL_WARN, "McuMessageHandler::SendBody not handshake yet");
        DestroyPacket(packet);
        return;
    }
    
    McuPacket &content = packet->packet;
    if(content.size() > MCU_MAX_CONTENT_SIZE)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::SendBody too large");
        DestroyPacket(packet);
        return;
    }
    PacketHead(packet, (unsigned short)id);
    PacketCheckSumFlag(packet);
    PushQueue(packet);
}

int McuMessageHandler::PushQueue(StruMcuPacket *packet)
{
    pthread_mutex_lock(&m_queueMutex);
    if(m_waitingQueue.size() >= MCU_MAX_WAITING_QUEUE)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::PushQueue full");
        pthread_mutex_unlock(&m_queueMutex);
        DestroyPacket(packet);
        return -1;
    }
    m_waitingQueue.push_back(packet);
    if(m_waitingQueue.size() == 1)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::PushQueue empty");
        SendQueue();
    }
    pthread_mutex_unlock(&m_queueMutex);
    return 0;
}

void McuMessageHandler::HandleTimer(string &name, void *data)
{
    if(name == MCU_RETRY_TIMER_NAME)
    {
        HandleRetryTimer(data);
    }
    else if(name == MCU_PASSENGER_RECORD_TIMER)
    {
        HandlePassengerTimer(data);
    }
    else if(name == MCU_DEAL_UPLOAD_TIMER)
    {
        HandleDealUploadTimer(data);
    }
    else if(name == MCU_CAN_DATA_TIMER)
    {
        HandleCanDataTimer(data);
    }
    else if(name == MCU_CAN_UPLOAD_TIMER)
    {
        HandleCanInfoUploadTimer(data);
    }
}

void McuMessageHandler::HandleRetryTimer(void *)
{
    pthread_mutex_lock(&m_queueMutex);

    if(m_waitingQueue.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::HandleRetryTimer empty");
        pthread_mutex_unlock(&m_queueMutex);
        return ;
    }

    StruMcuPacket *packet = *(m_waitingQueue.begin());
    if(packet->retry > MCU_MAX_RETRY_TIMES)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::HandleRetryTimer %d reach limit", packet->id);
        m_waitingQueue.pop_front();
        if((packet->id == MCU_POS_GET_PASSENGER) || (packet->id == MCU_POS_GET_DEAL))
        {
            int index = packet->data2;
            m_passengerRecord[index].state = STATE_TIMER;
            StartTimer(m_passengerRecord[index].posPassengerRecordTimer);
        }
        DestroyPacket(packet);
        if(m_waitingQueue.size() > 0)
        {
            LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::HandleRetryTimer proceed to next message");
            SendQueue();
        }
    }
    else
    {
        SendPacket(packet);
        packet->retry ++;
        StartTimer(m_retryTimer);
    }
    
    pthread_mutex_unlock(&m_queueMutex);
}

void McuMessageHandler::HandlePassengerTimer(void *data)
{
    int index = (int)data;
    m_passengerRecord[index].state = STATE_PASSENGER_SENT;
    SendGetPosPassengerRecord(index, KEEP_RECORD);
}

void McuMessageHandler::HandleDealUploadTimer(void *)
{
    pthread_mutex_lock(&m_dealRecordMutex);
    if(m_record_fd >= 0)
    {
        close(m_record_fd);
        m_record_fd = -1;
        SendPosInfoNotify(m_record_filename);
    }
    pthread_mutex_unlock(&m_dealRecordMutex);
}
void McuMessageHandler::HandleCanDataTimer(void *)
{
    SendGetCanData();
}
void McuMessageHandler::HandleCanInfoUploadTimer(void *)
{
    pthread_mutex_lock(&m_candataMutex);
    if(m_can_fd >= 0)
    {
        close(m_can_fd);
        m_can_fd = -1;
        SendCanDataNotify(m_can_filename,m_can_num);
    }
    pthread_mutex_unlock(&m_candataMutex);
}
void McuMessageHandler::HandleUpDownDirection(int dir)
{
    for(int i = 0; i < m_pos_num; i ++)
    {
        SendGetPosPassengerRecord(i, CLEAR_RECORD);
        
        m_passengerRecord[i].passengerUpCount = 0;
        m_passengerRecord[i].accuPassengerUpCount = 0;
        m_passengerRecord[i].passengerDownCount = 0;
        m_passengerRecord[i].accuPassengerDownCount = 0;
        m_passengerRecord[i].dealCount = 0;
        m_passengerRecord[i].lastDealCount = 0;
    }
    SendPOSStation(1, 0x00, dir);
    m_passengerup_count = 0;
    m_passengerdown_count = 0;
    SendPosPassengerNotify(0, 0, m_passengerup_count, m_passengerdown_count);
}

void McuMessageHandler::SendQueue()
{
    if(m_waitingQueue.size() <= 0)
    {
        LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::SendQueue empty");
    }
    else
    {
        StruMcuPacket *packet = *(m_waitingQueue.begin());
        LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::SendQueue  Retry:%d Seq:*%d Id: %d",packet->retry, packet->seq, packet->id);
        SendPacket(packet);
        packet->retry ++;
        StartTimer(m_retryTimer);
    }
    return;
}

void McuMessageHandler::SendPacket(StruMcuPacket *packet)
{
    if(SendContent(packet->packet) < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::SendPacket failed to send packet");
    }
    
    return;
}

int McuMessageHandler::SendContent(McuPacket &content)
{
    int ret = 0;
    
    m_send_len = 0;

    LogUtility::Log(LOG_LEVEL_DEBUG, "McuMessageHandler::SendContent entry(%d)", content.size());
    McuPacket::const_array_range ar = content.array_one();
    if(ar.second > 0)
    {
        memcpy(m_send_buffer, ar.first, ar.second);
        m_send_len += ar.second;
    }
    
    ar = content.array_two();
    if(ar.second > 0)
    {
        memcpy(m_send_buffer + m_send_len, ar.first, ar.second);
        m_send_len += ar.second;
    }
    
    ret = SerialConnectionManager::Instance()->Send(m_send_buffer, m_send_len);
    if(ret < 0)
    {
        LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::SendContent failed");
        return -1;
    }
    
    return m_send_len;
}

void McuMessageHandler::SendGPSData(unsigned char *buf, int len)
{
    StruMcuPacket *thepacket = CreatePacket();
    McuPacket &packet = thepacket->packet;
    buf[1] = 'G';
    buf[2] = 'P';
    printf("GSPData: %s\n",buf);
    for(int i = 0; i < len; i++){
	    PushByte(packet, buf[i]);
    }

    SendBody(thepacket, MCU_PACKET_STATION_REPORT);
}

void McuMessageHandler::SendHandshake()
{
    StruMcuPacket *thepacket = CreatePacket();

    SendBody(thepacket, MCU_PACKET_HANDSHAKE);
}

void McuMessageHandler::SendLEDStation(unsigned char mode, unsigned char speed, unsigned char times, unsigned char inout, unsigned char station)
{
    for(int i = 0; i < (int)(sizeof(LEDID) / sizeof(short)); i ++)
    {
        if((LEDFLAG[i] & LED_FLAG_STATION) == 0)
        {
            continue;
        }
        
        StruMcuPacket *thepacket = CreatePacket();
        
        thepacket->data = MCU_PACKET_LED_SUBTYPE_STATION;
        
        McuPacket &packet = thepacket->packet;
        PushByte(packet, MCU_PACKET_LED_SUBTYPE_STATION);
        PushByte(packet, mode);
        PushByte(packet, speed);
        PushByte(packet, times);
        PushByte(packet, inout);
        PushByte(packet, station);
        SendBody(thepacket, LEDID[i]);
    }
}

void McuMessageHandler::SendLEDState(int flag)
{
    int dir = GpsLocationMessageHandler::Instance()->GetDirection();
    if(dir == 0)
    {
        dir = 0x01;
    }
    else
    {
        dir = 0x02;
    }

    m_driving_state = flag;
    
    for(int i = 0; i < (int)(sizeof(LEDID) / sizeof(short)); i ++)
    {
        if((LEDFLAG[i] & LED_FLAG_STATE) == 0)
        {
            continue;
        }
        
        StruMcuPacket *thepacket = CreatePacket();
        
        thepacket->data = MCU_PACKET_LED_SUBTYPE_STATE;
        
        McuPacket &packet = thepacket->packet;
        PushByte(packet, MCU_PACKET_LED_SUBTYPE_STATE);
        PushByte(packet, dir);
        PushByte(packet, flag);
        SendBody(thepacket, LEDID[i]);
    }
}

void McuMessageHandler::SendLEDStateByDir(int dir)
{
    int send_dir;
    if(dir == 0)
    {
        send_dir = 0x01;
    }
    else
    {
        send_dir = 0x02;
    }

    for(int i = 0; i < (int)(sizeof(LEDID) / sizeof(short)); i ++)
    {
        if((LEDFLAG[i] & LED_FLAG_DIRECT) == 0)
        {
            continue;
        }
        
        StruMcuPacket *thepacket = CreatePacket();
        
        thepacket->data = MCU_PACKET_LED_SUBTYPE_STATE;
        
        McuPacket &packet = thepacket->packet;
        PushByte(packet, MCU_PACKET_LED_SUBTYPE_STATE);
        PushByte(packet, send_dir);
        PushByte(packet, 0x03);
        SendBody(thepacket, LEDID[i]);
    }
}

void McuMessageHandler::SendPOSStation(unsigned char station, unsigned char call_station, unsigned char dir)
{
    vector<unsigned char> vstation;
    vector<unsigned char> vcall_station;
    vector<unsigned char> vdir;
    
    GetCharSequenceFromInt(vstation, station, 2);
    GetCharSequenceFromInt(vcall_station, call_station, 1);
    GetCharSequenceFromInt(vdir, dir, 1);
    
    for(int i = 0; i < m_pos_num; i ++)
    {
        StruMcuPacket *thepacket = CreatePacket();
        McuPacket &packet = thepacket->packet;
        
        PushByte(packet, ((m_pos_addr[i] << 4) & MCU_POS_SRC_ADDR));
        PushByte(packet, 0x07);
        PushByte(packet, MCU_POS_FLAG);
        PushByte(packet, 0x30);
        PushByte(packet, 0x31);
        PushSequence(packet, vstation, 2);
        PushSequence(packet, vcall_station, 1);
        PushSequence(packet, vdir, 1);
        SendBody(thepacket, MCU_POS_STATION);
    }
}

void McuMessageHandler::SendPOSTime(time_t *timep)
{
    struct tm *t = localtime(timep);
    int year, month, day, hour, minute, second;
    vector<unsigned char> vyear, vmonth, vday, vhour, vminute, vsecond;
    
    year = 1900 + t->tm_year;
    month = t->tm_mon + 1;
    day = t->tm_mday;
    hour = t->tm_hour;
    minute = t->tm_min;
    second = t->tm_sec;
    
    GetCharSequenceFromInt(vyear, year, 4);
    GetCharSequenceFromInt(vmonth, month, 2);
    GetCharSequenceFromInt(vday, day, 2);
    GetCharSequenceFromInt(vhour, hour, 2);
    GetCharSequenceFromInt(vminute, minute, 2);
    GetCharSequenceFromInt(vsecond, second, 2);
    
    for(int i = 0; i < m_pos_num; i ++)
    {
        StruMcuPacket *thepacket = CreatePacket();
        McuPacket &packet = thepacket->packet;
        
        PushByte(packet, ((m_pos_addr[i] << 4) & MCU_POS_SRC_ADDR));
        PushByte(packet, 0x11);
        PushByte(packet, MCU_POS_FLAG);
        PushByte(packet, 0x30);
        PushByte(packet, 0x32);
        PushSequence(packet, vyear, 4);
        PushSequence(packet, vmonth, 2);
        PushSequence(packet, vday, 2);
        PushSequence(packet, m_pos_pass, 6);
        SendBody(thepacket, MCU_POS_DATE);
    }

    for(int i = 0; i < m_pos_num; i ++)
    {
        StruMcuPacket *thepacket = CreatePacket();
        McuPacket &packet = thepacket->packet;
        
        PushByte(packet, ((m_pos_addr[i] << 4) & MCU_POS_SRC_ADDR));
        PushByte(packet, 0x0F);
        PushByte(packet, MCU_POS_FLAG);
        PushByte(packet, 0x30);
        PushByte(packet, 0x33);
        PushSequence(packet, vhour, 2);
        PushSequence(packet, vmonth, 2);
        PushSequence(packet, vday, 2);
        PushSequence(packet, m_pos_pass, 6);
        SendBody(thepacket, MCU_POS_TIME);
    }
}

void McuMessageHandler::SendLEDTime(time_t *timep)
{
    struct tm *t = localtime(timep);
    int year, month, day, hour, minute, second, week;
    vector<unsigned char> vyear, vmonth, vday, vweek, vhour, vminute, vsecond;
    
    year = t->tm_year;
    month = t->tm_mon + 1;
    day = t->tm_mday;
    hour = t->tm_hour;
    minute = t->tm_min;
    second = t->tm_sec;
    week = t->tm_wday;
    
    IntToBcd(year % 100, vyear, 1);
    IntToBcd(month, vmonth, 1);
    IntToBcd(day, vday, 1);
    IntToBcd(hour, vhour, 1);
    IntToBcd(minute, vminute, 1);
    IntToBcd(second, vsecond, 1);
    IntToBcd(week, vweek, 1);

    for(int i = 0; i < (int)(sizeof(LEDID) / sizeof(short)); i ++)
    {
        if((LEDFLAG[i] & LED_FLAG_TIME) == 0)
        {
            continue;
        }
        
        StruMcuPacket *thepacket = CreatePacket();
        
        thepacket->data = MCU_PACKET_LED_SUBTYPE_TIME;
        
        McuPacket &packet = thepacket->packet;
        PushByte(packet, MCU_PACKET_LED_SUBTYPE_TIME);
        PushSequence(packet, vyear, 1);
        PushSequence(packet, vmonth, 1);
        PushSequence(packet, vday, 1);
        PushSequence(packet, vweek, 1);
        PushSequence(packet, vhour, 1);
        PushSequence(packet, vminute, 1);
        PushSequence(packet, vsecond, 1);
        SendBody(thepacket, LEDID[i]);
    }
}

void McuMessageHandler::SendGetPosPassengerRecord(int i, EnumRecordFlag clear)
{
    StruMcuPacket *thepacket = CreatePacket();
    McuPacket &packet = thepacket->packet;
    
    PushByte(packet, ((m_pos_addr[i] << 4) & MCU_POS_SRC_ADDR));
    if(clear == CLEAR_RECORD)
    {
        PushByte(packet, 0x03);
    }
    else
    {
        PushByte(packet, 0x05);
    }
    PushByte(packet, MCU_POS_FLAG);
    PushByte(packet, 0x38);
    PushByte(packet, 0x30);
    if(clear != CLEAR_RECORD)
    {
        PushByte(packet, 0x30);
        PushByte(packet, 0x31);
    }
    thepacket->data = (int)clear;
    thepacket->data2 = i;
    SendBody(thepacket, MCU_POS_GET_PASSENGER);
}

void McuMessageHandler::SendGetDealRecord(int i, EnumRecordFlag clear, int index)
{
    StruMcuPacket *thepacket = CreatePacket();
    McuPacket &packet = thepacket->packet;
    
    PushByte(packet, ((m_pos_addr[i] << 4) & MCU_POS_SRC_ADDR));
    PushByte(packet, 0x06);
    PushByte(packet, MCU_POS_FLAG);
    PushByte(packet, 0x38);
    PushByte(packet, 0x31);
    if(clear == CLEAR_RECORD)
    {
        PushByte(packet, 0x02);
    }
    else
    {
        PushByte(packet, 0x01);
    }
    PushWord(packet, index);
    thepacket->data = (int)clear;
    thepacket->data2 = i;
    SendBody(thepacket, MCU_POS_GET_DEAL);
}
void McuMessageHandler::SendGetCanData()
{
    StruMcuPacket *thepacket = CreatePacket();
    McuPacket &packet = thepacket->packet;

    PushByte(packet, 0x01);
    SendBody(thepacket, MCU_CAN_GET_DATA);
}
void McuMessageHandler::WriteToFile(vector<unsigned char> &vrecord)
{
    pthread_mutex_lock(&m_dealRecordMutex);
    
    if(m_record_fd < 0)
    {
        ostringstream ss;
        int cur_time = (int)time(NULL);
        ss << cur_time;
        //m_record_filename = getenv("POS_REOCRD_DIR");
        m_record_filename = "/nand3/posrecord/";
        m_record_filename += ss.str();
        m_record_filename += ".bus";
        
        m_record_fd = open(m_record_filename.c_str(), O_RDWR|O_CREAT|O_TRUNC);
        if(m_record_fd < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::WriteToFile open file failed %s", m_record_filename.c_str());
        }
    }
    
    for(int i = 0; i < (int)vrecord.size(); i ++)
    {
        unsigned ch = vrecord[i];
        if(write(m_record_fd, &ch, 1) != 1)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::WriteToFile write file failed");
            break;
        }
    }
    
    pthread_mutex_unlock(&m_dealRecordMutex);
    
    return;
}

void McuMessageHandler::CanDataWriteToFile(vector<unsigned char> &vrecord)
{    
    pthread_mutex_lock(&m_candataMutex);

    if(m_can_fd < 0)
    {
        ostringstream ss;
        int cur_time = (int)time(NULL);
        ss << cur_time;
        //m_can_filename = getenv("POS_REOCRD_DIR");
        m_can_filename = "/nand3/posrecord/";
        m_can_filename += ss.str();
        m_can_filename += ".can";
        m_can_num = 0;
        
        m_can_fd = open(m_can_filename.c_str(), O_RDWR|O_CREAT|O_TRUNC);
        if(m_can_fd < 0)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::CanDataWriteToFile open file failed %s", m_can_filename.c_str());
        }
    }
    
    for(int i = 0; i < (int)vrecord.size(); i ++)
    {
        unsigned ch = vrecord[i];
        if(write(m_can_fd, &ch, 1) != 1)
        {
            LogUtility::Log(LOG_LEVEL_ERROR, "McuMessageHandler::CanDataWriteToFile write file failed");
            break;
        }
    }
    m_can_num++;
    printf("CanDataWriteToFile---num=%d,filename=%s\n",m_can_num,m_can_filename.c_str());
    pthread_mutex_unlock(&m_candataMutex);
    return;
}

