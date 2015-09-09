#ifndef _MCU_MESSAGE_HANLDER_H_
#define _MCU_MESSAGE_HANLDER_H_
//#include "../busdaemon/SerialConnectionManager.h"
#include "GeneralTimer.h"
#include "McuProtocol.h"
#include <time.h> 
#include <stdio.h>
#include<sys/time.h> 
#include "JT808MessageHandler.h"





//extern unsigned char ACC_STATUS;

typedef struct time_interval
{
	unsigned short seq;
	unsigned int time_heart;
	unsigned int time_loc;
	unsigned short seq2;
	unsigned int flag;//判断是否修改参数 修改 1  没修改 0
	
}TIME_INTERVAL;
typedef enum
{
    AVERAGE = 1,
	NETWORK_BREAK,
	RECEIVE_COMMAND,
	DORMANT_RESTART,
	OTHER,
} ENUM_START_UP_CAUSE;





class McuMesageHandler :public ConnectionManager
{
public:
		void McuMessageHandler();
		//~McuMessageHandler(); 
		void get_utc_now_time(struct timeval *tv);
		void get_time(struct tm *date,unsigned char *buf);
		int  count_buf(unsigned char *p);
		void HandleMcuRstDownCause();
		void SendPowerOffCheckTime();
		int Handleissuedinfo();
		void Handletimeinterval(TIME_INTERVAL *time_loc_heart);
		ConnectionPacketHandlerInterface * mcu_packetHandler;
		int SystemOffHandleHeartLocPacket(StruJT808Packet *thepacket,unsigned int timenow);
		int SendSystemOffInfo();
		int WriteParaterToMcu(unsigned char * buf ,int len);
		int ReadParaterFromMcu();
		int WriteOneServerInformation();
		int PrepareOneServerInformation(unsigned char * buf ,int len);
		
};
#endif
