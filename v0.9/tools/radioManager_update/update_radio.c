#include "update_radio.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int read_flag = 1;
int read_start = 1;
int read_sta = 1;
int read_cout =0;
int cout = 0;
void *real_read_pthread(void *data)
{
	while(1)
	{
		if(read_flag)
		{
			read_status();
		}
		else
			wait_ms(20);

		wait_ms(20);
	}
	
}
//compute data Check Code
int CheckSumFlag(int size, u8 *buff)
{
	int sum = 0;
	int checksum = 0;
	int i = 0;
	for(i=0;i<size;i++)
	{
		sum+=(int)buff[i];
	}
	checksum = sum ^ 0xFF;
	return checksum;
}

//send command
int asc1411_command(u8 cmd_size, u8 *cmd, u8 reply_size, u8 *reply)
{
	int res = -1;
	wait_ms(15);
	res = io2w_write(cmd_size, cmd);
	if(res  <0 )
		return res;
	wait_ms(15);
	if(reply_size)
	{
		res = io2w_read(reply_size, reply);
	}
	return res;
}

//read status
int read_status()
{
	u8 status[16]={0};
	io2w_read(8, status);
//	printf("%d %d %d %d %d %d %d %d \n",status[0],status[1],status[2],status[3],status[4],status[5],status[6],status[7]);
	printf("%02x %02x %02x %02x %02x %02x %02x %02x \n",status[0],status[1],status[2],status[3],status[4],status[5],status[6],status[7]);
	printf("checksum is:%d\n",status[7]);

	printf("checksum after is:%d\n",CheckSumFlag(7,status));
#if 0
	if(status[7]!=CheckSumFlag(7,status))
	{
		printf("receive date is faild!\n");
		return -1;
	}	
#endif
	LEN = status[0];
	ID = status[1];
	VER = status[2];
	STATUS = status[3];
	BADN = status[4];
	FREQ = (u16)status[5]<<8 | (u16)status[6];
	if(STATUS==0x04)
	{
		printf("work error!\n");
		asc1411_reset();
	}
	return 0;
}

//radio reset
int asc1411_reset()
{
	u8 cmd[3]={0};
	cmd[0] = 3;
	cmd[1] = 1;
	cmd[2] = CheckSumFlag(2,cmd);
	asc1411_command(3,cmd,0,NULL);
	return 0;
}

//Host upgrade request
void asc1411_upgrade_Req()
{
	u8 cmd[3] = {0};
	cmd[0] = 3;
	cmd[1] = 0x81;
	cmd[2] = CheckSumFlag(2,cmd);
	asc1411_command(3,cmd,0,NULL);
}
//Host send file head
int asc1411_send_file_head(u8 size,u8 *buff)
{
	u8 cmd[67]={0};
	int i = 0;
	cmd[0] = 67;
	cmd[1] = 0x82;
	for(i=0;i<size;i++)
	{
		cmd[2+i]=buff[i];
	}
	cmd[66] = CheckSumFlag(66,cmd);
	asc1411_command(67,cmd,0,NULL);

	return 0;

}

//Host send file data
int asc1411_send_file_data(u8 size, u8 *buff)
{
	u8 cmd[133] = {0};
	int i = 0;
	cmd[0] = 133;
	cmd[1] = 0x83;
	cmd[2] = 0; //   H8 目前MCU不做处理，可随意填写进行占位 
	cmd[3] = 0; //   L8 目前MCU不做处理，可随意填写进行占位
	for(i=0;i<size;i++)      
	{
		cmd[4+i]=buff[i];	
	}
	cmd[132] = CheckSumFlag(132,cmd);
	asc1411_command(133,cmd,0,NULL);
	return 0;
}
int update_process()
{
	int i=0;
	int res = -1;
	int n =0;

	u8 file_head[64]={0};
	u8 file_data[128]={0};
	int fp;
	
	res = io2w_init();
	if(res==-1)
	{
		printf("init i2c error!\n");
		return -1;
	}


	asc1411_upgrade_Req();
	while(read_sta)
	{
		read_cout++;
		wait_ms(100);
		read_status();	
		if((VER==0xFF)&&(STATUS==1))
		{
			read_cout=0;
			read_sta=0;
		}
		if(read_cout==20)
		{
			read_sta=0;
			printf("read data fail!\n");
			return -1;
		}

	}
	if((VER==0xFF)&&(STATUS==1))
	{

		if((fp = open(PATH,O_RDONLY))<0)
		{
			printf("can't open file!\n");
			return -1;
		}


		if((n=read(fp,file_head,64))<0)
		{
			printf("read data fail!\n");
			close(fp);
			return -1;
		}

		printf("++++++++++++++head+++++++++++++++\n");
		for(i=0;i<64;i++)
		{
			printf("%02x ",file_head[i]);
			if((i+1)%16==0)
			printf("\n");
		}
		printf("\n");
		printf("++++++++++++++data++++++++++++++\n");
		asc1411_send_file_head(64,file_head);
		wait_ms(500);
		read_sta=1;
		while(read_sta)
		{
			read_cout++;
			wait_ms(150);
			read_status();
			if((VER==0xFF)&&(STATUS==2))
			{
				read_sta=0;
				read_cout=0;
			}
			if(read_cout==20)
			{
				printf("read status faild!");
				close(fp);
				return -1;
			}
		}
	
		if((VER==0xFF)&&(STATUS==2))
		{			

			while(read_start)
			{
				cout++;
				if((n=read(fp,file_data,128))==0)                  //get upgrade data
				{
					read_start=0;
					cout=0;
					printf("++++++++++read over+++++++\n");
				}

				if(n!=0)
				{
					printf("++++++++++++++++cout %d+++++++++++++++++++\n",cout);
					for(i=0;i<128;i++)
					{
						printf("%02x ",file_data[i]);
						if((i+1)%16==0)
						printf("\n");
					}
					wait_ms(200);
					asc1411_send_file_data(128,file_data);
			//		wait_ms(200);
			//		read_status();
				
				}
		
			}
			printf("++++++++++++++++++over+++++++++++++++++\n");
		}

		read_sta=1;
		while(read_sta)
		{
			read_cout++;
			wait_ms(100);
			read_status();
			if((VER==0xFF)&&(STATUS==4))
			{
				read_sta=0;
				read_cout=0;
				printf("+++++++++++++upgrad OK!+++++++++++++++\n");
			}
			if(read_cout==30)
			{
				printf("+++++++++++++upgrade fail!++++++++++\n");
				return -1;
			}
		}
	}
	return 0;
}
int main()
{
	printf("++++++++++++++++start update++++++++++++++++"); //可添加错误处理机制
	int res = update_process();  
	return res;
}
