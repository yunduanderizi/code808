#ifndef _MCU_MESSAGE_HANLDER_H_
#define _MCU_MESSAGE_HANLDER_H_

#include "SerialConnectionManager.h"
#include "GeneralTimer.h"
#include <pthread.h>
#include <boost/circular_buffer.hpp>
#include <vector>
#include <time.h>

using namespace std;

#define MCU_MAX_PACKET_SIZE               263
#define MCU_MIN_PACKET_SIZE               8
#define MCU_MAX_CONTENT_SIZE              255

#define MCU_MAX_WAITING_QUEUE             32

#define MCU_RETRY_TIMER_NAME              "MCURetry"
#define MCU_PASSENGER_RECORD_TIMER        "MCUPassenger"
#define MCU_DEAL_UPLOAD_TIMER             "MCUDealUpload"
#define MCU_CAN_DATA_TIMER                "MCUCanData"
#define MCU_CAN_UPLOAD_TIMER              "MCUCanUpload"

#define MCU_QUEUE_TIME_OUT                1
#define MCU_RETRY_TIME_OUT                2
#define MCU_PASSENGER_INTERVAL            3
#define MCU_CAN_DATA_INTERVAL             1
#define MCU_CAN_UPLOAD_INTERVAL           20

#define MCU_MAX_RETRY_TIMES               1

#define MCU_MAX_POS                       3

typedef boost::circular_buffer_space_optimized<unsigned char> McuPacket;
typedef boost::circular_buffer_space_optimized<unsigned char>::value_type McuPacketValue;

typedef struct
{
    unsigned short seq;
    unsigned short id;
    unsigned long data;
    unsigned long data2;
    int retry;
    GeneralTimerItem *timer;
    McuPacket packet;
    bool reply;
}StruMcuPacket;

typedef boost::circular_buffer<StruMcuPacket *> McuPacketFifo;

#define MCU_PACKET_FLAG              ((unsigned char)0x7E)
#define MCU_PACKET_ESCAPE            ((unsigned char)0x7D)

#define MCU_GET_BYTE(buf, idx)       (buf[idx])
#define MCU_GET_SHORT(buf, idx)      (((buf[idx]) << 8) | (buf[(idx) + 1]))
#define MCU_GET_LONG(buf, idx)       (((buf[idx]) << 24) |  ((buf[(idx) + 1]) << 16) | ((buf[(idx) + 2]) << 8) | (buf[(idx) + 3]))

#define MCU_NEW_PACKET               (new McuPacket(McuPacket::capacity_type(MCU_MAX_PACKET_SIZE, MCU_MIN_PACKET_SIZE))) 

#define MCU_PACKET_HANDSHAKE         0x0001
#define MCU_PACKET_STATION_REPORT    0x1201
#define MCU_PACKET_LED1              0x1101
#define MCU_PACKET_LED2              0x1102
#define MCU_PACKET_LED3              0x1103
#define MCU_PACKET_LED4              0x1104
#define MCU_POS_STATION              0x1401
#define MCU_POS_DATE                 0x1402
#define MCU_POS_TIME                 0x1403
#define MCU_POS_GET_PASSENGER        0x1480
#define MCU_POS_GET_DEAL             0x1481
#define MCU_CAN_GET_DATA             0x1801

#define MCU_PACKET_LED_SUBTYPE_STATION   0x02
#define MCU_PACKET_LED_SUBTYPE_STATE     0x04
#define MCU_PACKET_LED_SUBTYPE_TIME      0x07

#define MCU_POS_SRC_ADDR             0xF0
#define MCU_POS_FLAG                 0x40

typedef enum {
    CLEAR_RECORD = 0,
    KEEP_RECORD
}EnumRecordFlag;

typedef enum {
    STATE_TIMER = 0,
    STATE_PASSENGER_SENT,
    STATE_DEAL_SENT
}EnumRecordState;

typedef struct
{
    GeneralTimerItem *posPassengerRecordTimer;
    int passengerUpCount;
    int accuPassengerUpCount;
    int passengerDownCount;
    int accuPassengerDownCount;
    int dealCount;
    int lastDealCount;
    EnumRecordState state;
}StruPassengerRecord;

class McuMessageHandler : public SerialConnectionEventHandlerInterface, public SerialPacketHandlerInterface
{
public:
    McuMessageHandler();
    ~McuMessageHandler();
    virtual int ConnectionEventHandler(ENUM_SERIAL_CONNECTION_EVENT state);
    virtual int PacketHandler(unsigned char *buf, int len);
    void HandleTimer(string &name, void *data);
    
    void SendGPSData(unsigned char *buf, int len);
    void SendLEDStation(unsigned char mode, unsigned char speed, unsigned char times, unsigned char inout, unsigned char station);
    void SendLEDState(int flag);
    void SendLEDStateByDir(int dir);
    void SendPOSStation(unsigned char station, unsigned char call_station, unsigned char dir);
    void SendPOSTime(time_t *timep);
    void SendLEDTime(time_t *timep);
    void HandleUpDownDirection(int dir);

    int m_passengerup_count;
    int m_passengerdown_count;
private:
    inline StruMcuPacket *CreatePacket()
    {
        StruMcuPacket *packet = new StruMcuPacket();
        packet->packet.set_capacity(McuPacket::capacity_type(MCU_MAX_PACKET_SIZE, MCU_MIN_PACKET_SIZE));
        packet->retry = 0;
        packet->timer = NULL;
        packet->reply = false;
        
        return packet;
    }
    
    inline void DestroyPacket(StruMcuPacket *packet)
    {
	    if(packet->timer != NULL)
	    {
		    DestroyTimer(packet->timer);
	    }

        delete packet;
    }
    
    inline void PushByteFront(McuPacket &packet, unsigned char byte)
    {
        packet.push_front(byte);
    }

    inline void PushWordFront(McuPacket &packet, unsigned short word)
    {
        PushByteFront(packet, (unsigned char)(word & 0xFF));
        PushByteFront(packet, (unsigned char)((word >> 8) & 0xFF));
    }

    inline void PushLongFront(McuPacket &packet, unsigned long word)
    {
        PushByteFront(packet, (unsigned char)(word & 0xFF));
        PushByteFront(packet, (unsigned char)((word >> 8) & 0xFF));
        PushByteFront(packet, (unsigned char)((word >> 16) & 0xFF));
        PushByteFront(packet, (unsigned char)((word >> 24) & 0xFF));
    }
    
    inline void PushByte(McuPacket &packet, unsigned char byte)
    {
        packet.push_back(byte);
    }

    inline void PushWord(McuPacket &packet, unsigned short word)
    {
        PushByte(packet, (unsigned char)((word >> 8) & 0xFF));
        PushByte(packet, (unsigned char)(word & 0xFF));
    }

    inline void PushLong(McuPacket &packet, unsigned long word)
    {
        PushByte(packet, (unsigned char)((word >> 24) & 0xFF));
        PushByte(packet, (unsigned char)((word >> 16) & 0xFF));
        PushByte(packet, (unsigned char)((word >> 8) & 0xFF));
        PushByte(packet, (unsigned char)(word & 0xFF));
    }

    inline void PushString(McuPacket &packet, string str)
    {
        for(int i = 0; i < (int)str.length(); i ++)
        {
            PushByte(packet, (unsigned char)(str[i]));
        }
    }
    
    inline void PushSequence(McuPacket &packet, vector<unsigned char> &sequence, int len)
    {
        int seqlen = sequence.size();
        int offset = len - seqlen;
        if(offset < 0)
        {
            offset = 0;
        }
        
        for(int i = 0; i < len; i ++)
        {
            if(offset > i)
            {
                packet.push_back(0x00);
            }
            else
            {
                PushByte(packet, sequence[i - offset]);
            }
        }
    }

    inline void PushSequence(McuPacket &packet, string &sequence, int len)
    {
        int seqlen = sequence.length();
        
        for(int i = 0; i < len; i ++)
        {
            if(seqlen >= i)
            {
                packet.push_back(sequence[i]);
            }
            else
            {
                PushByte(packet, 0x00);
            }
        }
    }
    
    void HandlePacket();
    void ProcessCommonReply(int len, int id, int seq);
    void ProcessHandshake(int len, int seq);
    void ProcessPosStation(int len, int seq);
    void ProcessPosDate(int len, int seq);
    void ProcessPosTime(int len, int seq);
    void ProcessGetPassenger(int len, int seq);
    void ProcessGetDeal(int len, int seq);
    void ProcessGetCanData(int len, int seq);
    
    void ClearQueue();
    void SendQueue();
    int PushQueue(StruMcuPacket *packet);
    int PopQueue(int id, int seq, unsigned long *data, unsigned long *data2);
    void ClearRxPacket()
    {
        m_packet_start = false;
        m_packet_escape = false;
        m_rxPacket.clear();
    }
    void HandleRetryTimer(void *data);
    void HandlePassengerTimer(void *data);
    void HandleDealUploadTimer(void *data);
    void HandleCanDataTimer(void *);
    void HandleCanInfoUploadTimer(void *);
    
    void SendBody(StruMcuPacket *packet, unsigned short id);
    void PacketHead(StruMcuPacket *packet, unsigned short id);
    void PacketCheckSumFlag(StruMcuPacket *packet);
    void SendPacket(StruMcuPacket *packet);
    int SendContent(McuPacket &content);
    
    void SendHandshake();
    void SendGetPosPassengerRecord(int i, EnumRecordFlag clear);
    void SendGetDealRecord(int i, EnumRecordFlag clear, int index);
    void SendGetCanData();
    
    void WriteToFile(vector<unsigned char> &vrecord);
    void CanDataWriteToFile(vector<unsigned char> &vrecord);
    
    GeneralTimerItem *m_retryTimer;
    GeneralTimerItem *m_dealReportTimer;
    GeneralTimerItem *m_CanDataTimer;
    GeneralTimerItem *m_CanUploadTimer;
    
    unsigned short m_seq;
    pthread_mutex_t m_queueMutex;
    pthread_mutex_t m_packetMutex;
    
    McuPacket m_rxPacket;
    bool m_packet_start;
    bool m_packet_escape;
    
    McuPacketFifo m_waitingQueue;
    
    unsigned char m_send_buffer[MCU_MAX_PACKET_SIZE];
    unsigned int m_send_len;
    
    unsigned char m_pos_addr[MCU_MAX_POS];
    unsigned char m_pos_num;
    vector<unsigned char> m_pos_pass;
    
    int m_driving_state;
    
    bool m_handshake;
    
    StruPassengerRecord *m_passengerRecord;
    
    int m_record_fd;
    string m_record_filename;
    int m_dealUploadInterval;
    pthread_mutex_t m_dealRecordMutex;
    
    int m_can_fd;
    int m_can_num;
    string m_can_filename;
    pthread_mutex_t m_candataMutex;
};

#endif
