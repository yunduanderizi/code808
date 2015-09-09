#include "GeneralTimer.h"
#include "McuMessageHandler.h"
#include "JT808MessageHandler.h"
#include <time.h> 
#include <stdio.h>
#include <sys/time.h> 
#include "ConnectionManager.h"
#include <queue>
#include "Utility.h"
#include <vector>
#include "VDRRunParaTable.h"
#include "VDRBaseConfigTable.h"
#include "ConnectionManagerAT.h"


ConnectionManager connection;
extern JT808MessageHandler jt808;

char *  AuthString;
char * Main_Server_To_Mcu;
char * Second_Server_To_Mcu;
unsigned short Port_To_Mcu;
unsigned int START_UP_CAUSE;

unsigned int hearttimelast=0;
unsigned int loctimelast=0;
unsigned char  loc_buf_last[90];
unsigned char  heart_buf_last[15];
unsigned short seq_last=0;
unsigned short heart_retry_time=0;
unsigned short loc_retry_time=0;
unsigned int  loc_night_sleep=0;
unsigned int  loc_day_sleep=0;
unsigned char day_time[4];
unsigned char buf_heart_last[15]={0x7e,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x7a,0x7e};
unsigned int SizeOfLoc=0;
extern int DbusSystemOff;
extern unsigned char connect_tmp;


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
	unsigned char buf[13];
	printf("enter Handletimeinterval \n");
	int ret=i2c_read_time_interval(buf, 13);
	
	for (int i=0;i<13;i++)
		{
		printf("%02x ",buf[i]);
	}
	//printf("ret===%d \n",ret);
	time_loc_heart->seq=(buf[0]<<8)|buf[1];
	time_loc_heart->time_heart=(buf[2]<<24)|(buf[3]<<16)|(buf[4]<<8)|buf[5];
	time_loc_heart->time_loc=(buf[6]<<24)|(buf[7]<<16)|(buf[8]<<8)|buf[9];
	time_loc_heart->seq2=(buf[10]<<8)|buf[11];	
	time_loc_heart->flag=(unsigned int )buf[12];
	printf("seq2  ==================1111111111=======================================%d \n",time_loc_heart->seq2);
	
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
int McuMesageHandler::SendSystemOffInfo()
{
	unsigned char buf_loc1[30];
	unsigned char buf_loc2[30];
	unsigned char buf_loc3[30];
	unsigned char buf[8];
	memset(buf_loc1,0x00,sizeof(buf_loc1));
	memset(buf_loc2,0x00,sizeof(buf_loc2));
	memset(buf_loc3,0x00,sizeof(buf_loc3));
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
	printf ("seq_last====================%d \n",seq_last);
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
/*
	for (int num=0;num<4;num++)
	{
		loc_buf_last[14+num]=0x00;
	}
*/
	loc_buf_last[21]=loc_buf_last[21]&0xfd;
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
	usleep(10*1000);
	int ret_heart=I2c_write_heartbeat_information(buf_heart_last,15);//发送最后一个心跳包
	usleep(10*1000);
	int ret_loc1=i2c_write_location_information(buf_loc1,30);//发送位置信息
	usleep(10*1000);
	int ret_loc2=i2c_write_location_information(buf_loc2,30);
	usleep(10*1000);
	int ret_loc3=i2c_write_location_information(buf_loc3,SizeOfLoc+4-58+1);
	//printf("ret_heart===%d =ret_loc1===%d =======ret_loc2====%d====ret_loc3========%d ret_last_seq_time=%d\n",ret_heart,ret_loc1,ret_loc2,ret_loc3,ret_last_seq_time);
	
	
	
}
int McuMesageHandler::WriteParaterToMcu(unsigned char *buf ,int len)
{
	int packnum,num;
	packnum= len/29;
	num=len%29;
	unsigned char buf_tmp[30];
	memset(buf_tmp,0,30);
	int j=0;
	printf("i    %d     num  %d   len  %d   \n",packnum,num,len);
	for ( j=1;j<=packnum;j++)
	{
		buf_tmp[0]=j;
		for (int m=0;m<29;m++)
		{

			buf_tmp[m+1]=buf[((j-1)*29)+m];
			//printf(" j   %d   m   %d the data is  %d \n",j,m,j*30+m);
		}
		int ret=I2c_write_Parameter_data(buf_tmp,30);
		usleep(10*1000);
		//memcpy(buf_tmp,0x00,sizeof(buf_tmp));
		//printf("ret    %d   packnum    %d \n",ret,packnum);
	}
	if (num!=0)
	{
		memset(buf_tmp,0,30);
		buf_tmp[0]=packnum+1;
		for (j=0;j<num;j++)
		{
			buf_tmp[j+1]=buf[29*packnum+1+j];
		}
		int ret1=I2c_write_Parameter_data(buf_tmp,num+1);
		printf("ret1    %d   packnum%d \n",ret1,packnum);
	}
	return 0;
}

int McuMesageHandler::ReadParaterFromMcu()
{
	printf("enter  the readParater   \n");
	unsigned char buf_data[663];
	unsigned char buf_tmp[30];
	memset(buf_tmp,0,30);
	memset(buf_data,0,663);
	int num=0;
	int m,n;
	int packetnum=663/29;
	int lastdata=663%29;
	printf("lastdata   %d \n",lastdata);
	for (m=0;m<packetnum;m++)
	{
		printf("packetnum=======%d====m==%d \n",packetnum,m);
		int ret=I2c_read_setting_data(buf_tmp,30);
		for ( n =0;n<29;n++)
		{
			buf_data[n+m*29]=buf_tmp[n+1];
			//printf("m%02x ",buf_tmp[n+1]);
		}
		usleep(100*1000);
		
	}
	num=m*29;
	printf("num===========%d \n",num);
	int ret_1=I2c_read_setting_data(buf_tmp,lastdata+1);
	for (m=0;m<lastdata;m++)
	{
		buf_data[num+m]=buf_tmp[m+1];
	}
	printf("num+m=======%d \n",num+m);
	jt808.HandleParaMcutoCpu(buf_data,663);

	


/*
	for (int m=0;m<packetnum;m++);
	{	

		int ret=I2c_read_setting_data(buf_tmp,30);
		for ( j =0;j<29;j++)
		{
			buf_data[j+m*29]=buf_tmp[j+1];
			printf("%02x ",buf_tmp[j+1]);
		}
		usleep(10*1000);
	}
	num=j+m*29+1;
	printf("num===========%d \n",num);
	int ret_1=I2c_read_setting_data(buf_tmp,4);
	for (i=0;i<lastdata;i++)
	{
		buf_data[num+i]=buf_tmp[i];
	}
	printf("num+i=======%d \n",num+i);
	jt808.HandleParaMcutoCpu(buf_data,663);
*/
/*
	for (i = 0 ;i<663;i++)
	{
		printf("%02x ",buf_data[i]);
		if ((i+1)%15==0)
			printf("\n");
	}
*/
}
int McuMesageHandler::PrepareOneServerInformation(unsigned char * buf ,int len)
{
	printf("len main  ============%d seq_last   %d \n",len,seq_last);	
	printf ("seq_last====================%d \n",seq_last);
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
	buf[96]=(unsigned char)(Port_To_Mcu>>8);
	buf[97]=(unsigned char )Port_To_Mcu;
	for(i=0;i<30;i++)
	{
		buf[98+i]=(unsigned char)Second_Server_To_Mcu[i];
	}
	buf[128]=(unsigned char)(Port_To_Mcu>>8);
	buf[129]=(unsigned char )Port_To_Mcu;
	
	buf[130]=(unsigned char)(loc_day_sleep>>24);
	buf[131]=(unsigned char)(loc_day_sleep>>16);
	buf[132]=(unsigned char)(loc_day_sleep>>8);
	buf[133]=(unsigned char)(loc_day_sleep);
	buf[134]=(unsigned char)(loc_night_sleep>>24);
	buf[135]=(unsigned char)(loc_night_sleep>>16);
	buf[136]=(unsigned char)(loc_night_sleep>>8);
	buf[137]=(unsigned char)(loc_night_sleep);
	for (i=0;i<4;i++)
	{
		buf[138+i]=day_time[i];
	}
	for (int i=0;i<142;i++)
	{
		printf("%02x ",buf[i]);
		if((i+1)%15==0)
			printf("\n");
	}
	return 0;
	
}

int McuMesageHandler::WriteOneServerInformation()
{
	unsigned char buf[142];
	unsigned char buf_tmp[30];
	int packetnum=sizeof(buf)/29;
	int lastdata=sizeof(buf)%29;
	memset(buf,0,142);
	printf("packetnum                %d         lastdata     %d \n",packetnum,lastdata);
	PrepareOneServerInformation(buf,142);
	
	for (int i=1;i<=packetnum;i++)
	{
		buf_tmp[0]=i;
		for (int j =0;j<29;j++)
		{	
			buf_tmp[j+1]=buf[29*(i-1)+j];
		}
		int ret=I2c_write_firstserver_data(buf_tmp,30);
		printf("ret======%d  i    %d  \n",ret,i);
		usleep(10*1000);
	}
	memset(buf_tmp,0,30);
	if (lastdata!=0)
	{
		//memcpy(buf_tmp,0,sizeof(buf_tmp));
		buf_tmp[0]=packetnum+1;
		for (int j=0;j<lastdata;j++)
		{
			buf_tmp[j+1]=buf[packetnum*29+j];
		}
		int ret1=I2c_write_firstserver_data(buf_tmp,lastdata+1);
		printf("ret1  ================%d  5 \n",ret1);
	}
	return 0;
}

