#include "GeneralTimer.h"
#include "McuMessageHandler.h"
#include "JT808MessageHandler.h"
#include <time.h> 
#include <stdio.h>
#include <string.h>
#include <sys/time.h> 
#include "ConnectionManager.h"
#include <queue>
#include "Utility.h"
#include <vector>
#include "VDRRunParaTable.h"
#include "VDRBaseConfigTable.h"
#include "ConnectionManagerAT.h"


ConnectionManager connection;



unsigned int START_UP_CAUSE;

unsigned int hearttimelast=0;
unsigned int loctimelast=0;
unsigned char  loc_buf_last[90];
unsigned char  heart_buf_last[15];
unsigned short seq_last=0;
unsigned short heart_retry_time=0;
unsigned short loc_retry_time=0;
unsigned char buf_heart_last[15]={0x7e,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x7a,0x7e};
unsigned int SizeOfLoc=0;
extern int DbusSystemOff;
extern unsigned char connect_tmp;
char *AuthString;
char *Main_Server_To_Mcu;
char *Second_Server_To_Mcu;
int Port_To_Mcu1;
int Port_To_Mcu2;

void McuMesageHandler::McuMessageHandler()
{
	mcu_packetHandler = NULL;
}
void McuMesageHandler::get_utc_now_time(struct timeval *tv)
{
	struct timezone tz; 
	gettimeofday (tv , &tz);
}
void McuMesageHandler::get_time(struct tm *date,unsigned char *buf )
{  

	time_t timep; 
	time(&timep); 
	date=gmtime(&timep); 
	printf("%d  %d  %d  \n",(1900+date->tm_year), (1+date->tm_mon),date->tm_mday); 
	printf(" %d;%d;%d\n", date->tm_hour, date->tm_min, date->tm_sec);
	buf[0]=(1900+(date->tm_year))/256;
	buf[1]=(1900+date->tm_year)%256;
	buf[2]=(unsigned char )(1+date->tm_mon);
	buf[3]=(unsigned char )(date->tm_mday);
	buf[4]=(unsigned char )(date->tm_hour);
	buf[5]=(unsigned char )(date->tm_min);
	buf[6]=(unsigned char )(date->tm_sec);
}

int McuMesageHandler::count_buf(unsigned char *p)
{
		int count=1;
		p++;
		do{
			p++;
			count++;
		}while((*p) !=0x7e);
		return count+1;
}
/***************处理接收唤醒方式******************************/
void McuMesageHandler::HandleMcuRstDownCause()
{
	unsigned char value;
	int ret=I2c_read_mcu_rst_down_cause(value,1);
	printf("开机原因 value===%02x \n",value);
	switch (value)
	{
		case 0xc0:
			START_UP_CAUSE=DORMANT_RESTART;
			printf("START_UP_CAUSE==%d \n",START_UP_CAUSE);
			break;
		case 0x01:
		case 0x10:
			START_UP_CAUSE=AVERAGE;
			printf("START_UP_CAUSE==%d \n",START_UP_CAUSE);
			break;
		case 0x80:
			START_UP_CAUSE=NETWORK_BREAK;
			printf("START_UP_CAUSE==%d \n",START_UP_CAUSE);
			break;
		case 0x40:
			START_UP_CAUSE=RECEIVE_COMMAND;
			printf("START_UP_CAUSE==%d \n",START_UP_CAUSE);
		default:
			START_UP_CAUSE=OTHER;
			printf("START_UP_CAUSE==%d \n",START_UP_CAUSE);
			break;
	}
}




/*********************发送关机校时************************************/
void McuMesageHandler::SendPowerOffCheckTime()
{
	unsigned char buf[8];
	struct tm *date;
	get_time(date,buf);
	printf("connect_tmp===%d \n",connect_tmp);
	if (connect_tmp==0)
		buf[7]=0x01;
	else
		buf[7]=0x00;
	for(int i=0;i<8;i++)
	{
		printf("buf[%d]=%02x\n",i,buf[i]);
	}
	int ret=i2c_write_calibration_time( buf, 8);
}

/*******************处理MCU发送的指令的内容**********************/
int  McuMesageHandler::Handleissuedinfo()
{
	//McuMessageHandler();
	unsigned char buf_tmp1[31];
	unsigned char buf_tmp2[31];
	unsigned char buf_tmp3[31];
	unsigned char buf_tmp4[31];
	unsigned char buf_tmp5[31];
	unsigned int i=0;
	unsigned char flag=0;
	unsigned char command[155];
	memset(command,0,sizeof(command));
	printf("enter Handleissuedinfo   \n  ");
	i2c_read_issued_information(buf_tmp1,31);
	i2c_read_issued_information(buf_tmp2,31);
	i2c_read_issued_information(buf_tmp3,31);
	i2c_read_issued_information(buf_tmp4,31);
	i2c_read_issued_information(buf_tmp5,31);
	for(i=0;i<31;i++)
	{
		printf("buf_tmp1[1]=%02x \n",i,buf_tmp1[1]);
		if (buf_tmp1[1]==0x00)
			return 0;
		command[i]=buf_tmp1[i+1];
		printf("buf_tmp1[%d]=%02x \n",i,buf_tmp1[i]);
		if(command[i]==0x7e)
		{
			flag=1;
			printf("Handleissuedinfo1::flag  %d   i %d \n",flag,i);
		}
		if((i!=0)&&(command[i]==0x7e)&&(flag==1))
		{
			flag=0;
			printf("Handleissuedinfo2::flag  %d   i %d \n",flag,i);
			break;
		}
	}
	for(i=0;i<31;i++)
	{
		command[i+30]=buf_tmp2[i];
		if((buf_tmp2[i]==0x7e)&&(flag==1))
			break;
	}
	for(i=0;i<31;i++)
	{
		command[i+61]=buf_tmp3[i];
		if((buf_tmp3[i]==0x7e)&&(flag==1))
			break;
	}
	for(i=0;i<31;i++)
	{
		command[i+92]=buf_tmp4[i];
		if((buf_tmp4[i]==0x7e)&&(flag==1))
			break;
	}
	for(i=0;i<31;i++)
	{
		command[i+123]=buf_tmp5[i];
		if((buf_tmp5[i]==0x7e)&&(flag==1))
			break;
	}

	int ret=count_buf(command);
	if (ret>150)
		return 0;
	printf("ret ====================%d \n",ret);
	for (int i=0;i<ret;i++)
	{
		printf("%02x ",command[i]);
	}
	connection.HandleReceiveBuf(command,ret);
	//mcu_packetHandler->PacketHandler(command, ret);
}
/*********接收MCU上报最后一条流水号和还有多长时间启动心跳和定时上报的时间************/
void McuMesageHandler::Handletimeinterval(TIME_INTERVAL *time_loc_heart)
{	
	unsigned char buf[10];
	printf("enter Handletimeinterval \n");
	int ret=i2c_read_time_interval(buf, 10);
	
	for (int i=0;i<10;i++)
		{
		printf("%02x ",buf[i]);
	}
	//printf("ret===%d \n",ret);
	for (int i=0;i<10;i++)
		{
		printf("%02x ",buf[i]);
	}
	time_loc_heart->seq=(buf[0]<<8)|buf[1];
	time_loc_heart->time_heart=(buf[2]<<24)|(buf[3]<<16)|(buf[4]<<8)|buf[5];
	time_loc_heart->time_loc=(buf[6]<<24)|(buf[7]<<16)|(buf[8]<<8)|buf[9];
	
}

/*************************获取每一次的0200 包***************/
int McuMesageHandler::SystemOffHandleHeartLocPacket(StruJT808Packet *thepacket,unsigned int timenow)
{
	JT808Packet &packet = thepacket->packet;
	unsigned char checksum=0;
	
	//printf("packet.size()===%d \n",packet.size());
	  if(packet.size()>=60&&packet.size()<100) 
	  {   
			printf("size  ===%d \n",packet.size());
			if ((thepacket->id)==0x0200)
			{
				SizeOfLoc=packet.size();
				printf("SizeOfLoc==%d   SystemOffHandleHeartLocPacket  \n",SizeOfLoc);
				memset(loc_buf_last,0x7e,sizeof(loc_buf_last));
				loc_buf_last[0]=SizeOfLoc+3;
				checksum=0;
		        for(int j=0;j<packet.size();j++)
		        {
		            loc_buf_last[j+2]= JT808_GET_BYTE(packet, j);
		        }       
		        for(int k=2;k<SizeOfLoc+1;k++)
		        {
		            checksum ^= loc_buf_last[k];
		        }
				printf("checksum==%02x \n",checksum);
		        loc_buf_last[SizeOfLoc+2]=checksum;
#if 0
				for(int i=0 ;i<SizeOfLoc+4;i++)
		        {
		           printf("%02x ",loc_buf_last[i]);
		           if (i%15 == 0)
		           {   
		              printf("\n");
		           }
		       }
#endif

			}
			
	}
	  else if (packet.size()==12)
	  {
		hearttimelast=timenow;
		printf("hearttimelast==%d\n",hearttimelast);
	  }
				
}
#if 0
int McuMesageHandler::SendSystemOffInfo()
{
	unsigned char buf_loc1[30];
	unsigned char buf_loc2[30];
	unsigned char buf_loc3[30];
	unsigned char buf[8];
	memset(buf_loc1,0x00,sizeof(buf_loc1));
	memset(buf_loc2,0x00,sizeof(buf_loc2));
	memset(buf_loc3,0x00,sizeof(buf_loc3));
	seq_last=seq_last-1;
	buf_loc1[0]=0x01;
	buf_loc2[0]=0x02;
	buf_loc3[0]=0x03;
	struct tm *date;
	unsigned char buf_last_seq_time[14];
	vector<unsigned char> bcd;
	unsigned int body=36;
    bcd.clear();
	
    for(int i = 0; i < 6; i ++)
    {
		//printf(" loc_buf_last[%d]= %02x ",i+36,loc_buf_last[i+36]);
		bcd.push_back(JT808_GET_BYTE(loc_buf_last, body));
        body ++;
    }
	//loctimelast = GetTime(bcd);
	//GetLocTimeInter();
	printf ("loc_retry_time====================%d \n",loc_retry_time);
	printf ("heart_retry_time====================%d \n",heart_retry_time);
	printf ("hearttimelast====================%d \n",hearttimelast);
	printf ("loctimelast====================%d \n",loctimelast);
	//printf("seq_last==%d  hearttimelast=====%d loctimelast==%d=====heart_retry_time==%d =loc_retry_time==%d \n",seq_last,hearttimelast,loctimelast,heart_retry_time,loc_retry_time);
//最后的流水号心跳间隔和位置上报间隔和最后一次心跳的上报时间和位置上报时间
	buf_last_seq_time[0]=(unsigned char)(seq_last>>8);
	buf_last_seq_time[1]=(unsigned char)seq_last;
	buf_last_seq_time[2]=(unsigned char)(heart_retry_time>>8);
	buf_last_seq_time[3]=(unsigned char)heart_retry_time;
	buf_last_seq_time[4]=(unsigned char)(loc_retry_time>>8);
	buf_last_seq_time[5]=(unsigned char)loc_retry_time;
	buf_last_seq_time[6]=(unsigned char)(hearttimelast>>24);
	buf_last_seq_time[7]=(unsigned char)(hearttimelast>>16);
	buf_last_seq_time[8]=(unsigned char)(hearttimelast>>8);
	buf_last_seq_time[9]=(unsigned char)hearttimelast;
	buf_last_seq_time[10]=(unsigned char)(loctimelast>>24);
	buf_last_seq_time[11]=(unsigned char)(loctimelast>>16);
	buf_last_seq_time[12]=(unsigned char)(loctimelast>>8);
	buf_last_seq_time[13]=(unsigned char)loctimelast;
	for(int j=0;j<29;j++)
	{
		buf_loc1[j+1]=loc_buf_last[j];
		buf_loc2[j+1]=loc_buf_last[j+29];
	}
	for (int k=1;k<=SizeOfLoc+4-58;k++)
	{
		buf_loc3[k]=loc_buf_last[k+57];
		//printf("buf_loc3[%d]=%02x \n",k,buf_loc3[k]);
	}
	for(int m=0;m<6;m++)
	{
		buf_heart_last[m+5]=loc_buf_last[m+6];
	}
	int ret_last_seq_time=i2c_write_time_interval(buf_last_seq_time,14);//发送最后流水号
	int ret_heart=I2c_write_heartbeat_information(buf_heart_last,15);//发送最后一个心跳包
	//int ret_loc1=i2c_write_location_information(buf_loc1,30);//发送位置信息
	//int ret_loc2=i2c_write_location_information(buf_loc2,30);
	//int ret_loc3=i2c_write_location_information(buf_loc3,SizeOfLoc+4-58+1);
	//printf("ret_heart===%d =ret_loc1===%d =======ret_loc2====%d====ret_loc3========%d ret_last_seq_time=%d\n",ret_heart,ret_loc1,ret_loc2,ret_loc3,ret_last_seq_time);
	
	
	
}
#endif

int McuMesageHandler::PrepareOneServerInformation(unsigned char * buf ,int len)
{
	printf("len 222222222222222222222222222222222222222222 ============%d \n",len);	
	int i;
	seq_last=seq_last-1;
	buf[0]=(unsigned char)seq_last>>8;
	buf[1]=(unsigned char)seq_last;
	for ( i=0;i<64;i++)
	{
		buf[2+i]=(unsigned char)AuthString[i];
	}
	for(i=0;i<30;i++)
	{
		buf[66+i]=(unsigned char)Main_Server_To_Mcu[i];
	}
	buf[96]=(unsigned char)(Port_To_Mcu1>>8);
	buf[97]=(unsigned char )Port_To_Mcu1;
	for(i=0;i<30;i++)
	{
		buf[98+i]=(unsigned char)Second_Server_To_Mcu[i];
	}
	buf[128]=(unsigned char)(Port_To_Mcu2>>8);
	buf[129]=(unsigned char )Port_To_Mcu2;
	for (int i=0;i<130;i++)
	{
		printf("%02x ",buf[i]);
		if((i+1)%15==0)
			printf("\n");
	}
	return 0;
	
}

int McuMesageHandler::WriteOneServerInformation()
{
	unsigned char buf[130];
	unsigned char buf_tmp[30];
	int packetnum=sizeof(buf)/29;
	int lastdata=sizeof(buf)%29;
	printf("packetnum                %d         lastdata     %d \n",packetnum,lastdata);
	PrepareOneServerInformation(buf,130);
	
	for (int i=1;i<=packetnum;i++)
	{
		buf_tmp[0]=i;
		for (int j =0;j<29;j++)
		{	
			buf_tmp[j+1]=buf[29*(i-1)+j];
		}
		int ret=I2c_write_secondserver_data(buf_tmp,30);
		usleep(10*1000);
	}
	if (lastdata!=0)
	{
		//memcpy(buf_tmp,0,sizeof(buf_tmp));
		buf_tmp[0]=packetnum+1;
		for (int j=0;j<lastdata;j++)
		{
			buf_tmp[j+1]=buf[packetnum*29+1+j];
		}
		int ret1=I2c_write_secondserver_data(buf_tmp,lastdata+1);
	}
	return 0;
}

int McuMesageHandler::SendSystemOffInfo()
{

	printf("fei --------------------------- SendSystemOffInfo\n");
 // 获取第二服务器的主备ip地址port  以及鉴权码
      VDRBaseConfigTable *table = new VDRBaseConfigTable();
	int port_i, port_b;
	string ip_i , ip_b ,auth;
	
	
	
	 unsigned char buf_last_seq_time[130];
	unsigned char seq_iic[2] ;
	char auth_iic[64]={'\0'};
	 char server_main[30]={'\0'};
	 char server_bei[30]={'\0'};
	 char port_main[2];
        char port_bei[2];
       if(table->GetIntValue((int)VDR_BASE_CONFIG_D6, port_i) < 0)
      {
  		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult get main_ip token failed");
	 	delete table;
		return -1;
	}
	if(table->GetIntValue((int)VDR_BASE_CONFIG_D7, port_b) < 0)
      {
	       LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult get bei_ip token failed");
		delete table;
		return -1;
	}
	if(table->GetStringValue((int)VDR_BASE_STATUS_D5,ip_i ) < 0)
	{
		LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult get main_port token failed");
		delete table;
	       return -1;
	}
	if(table->GetStringValue((int)VDR_BASE_STATUS_D6,ip_b) < 0)
       {
	       LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult get auth token failed");
	       delete table;
	       return -1;
	}
	if(table->GetStringValue((int)VDR_BASE_STATUS_D7, auth) < 0)
       {
	   LogUtility::Log(LOG_LEVEL_ERROR, "JT808MessageHelper::HandleRegisterResult get auth token failed");
	   delete table;
	   return -1;
       }
	

	printf("fei ------------main port  = %d\n", port_i);
	printf("fei ------------bei    prot  = %d\n",port_b);
	printf("fei ------------main ip      = %s\n",ip_i.c_str());
	printf("fei ------------bei    ip      =%s\n",ip_b.c_str());
	printf("fei ------------auth           =%s\n",auth.c_str());

 //获取心跳报的流水号	
	seq_last=seq_last-1;
	
	
    /* 
      组串协议
      BYTE[0]:流水号高8位
      BYTE[1]:流水号低8位
      BYTE[2~65]:鉴权码64字节
      BYTE[66~95]:第二中心ip （30字节）
      BYTE[96、97]第二中心端口
      BYTE[98~127]:第一中心备份ip （30字节）
      BYTE[128、129]:第一中心备份端口
    
     
	// 组串
	//流水号
	seq_iic[0]=(unsigned char)(seq_last>>8);
	seq_iic[1]=(unsigned char)seq_last;
	// 鉴权码
	strcpy(server_main ,auth.c_str());
	//第二中心主 备服务器ip
	strcpy(server_main , ip_i.c_str());
	strcpy(server_bei ,ip_b.c_str());

	
	//第二中心主备port
	port_main[0] = (unsigned char)(port_i>>8);
	port_main[1] = (unsigned char)port_i;

	port_bei[0] = (unsigned char)(port_b>>8);
	port_bei[1] =(unsigned char)port_b;
	
	//int ret_last_seq_time=i2c_write_time_interval(buf_last_seq_time,14);//发送最后流水号
	//int ret_heart=I2c_write_heartbeat_information(buf_heart_last,15);//发送最后一个心跳包
 */
	
}



