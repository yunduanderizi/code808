#include "update_mcu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>


int read_flag = 1;
int read_start = 1;
int send_start = 1; 
int read_sta = 1;
int read_cout =0;
int block_num =0; int cout = 0;
int error_num = 0;
u8 Send_End[1024]={0};

//int total_line_sum = 0; 
int error_flag = 0;
FILE *fp;
//compute data Check Code
u8 CheckSumFlag(int size, u8 *buff)
{	
	u8 checksum = 0;
	int i = 0;
	for(i=0;i<size;i++)
	{
		checksum -= buff[i];
	}

	return checksum;
}

//send command
int mcu_command(int cmd_size, u8 *cmd, int reply_size, u8 *reply)
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
int read_status(int len)
{
	u8 status[16]={0};
	if(len==1)
	{
		io2w_read(1, status);
		Request_ID = status[0];
		printf("Request_ID is : %02x\n",Request_ID);

	}
	else
	{
		io2w_read(3,status);
		Date_ID1 = status[0];
		Date_ID2 = status[1];
		Date_ID3 = status[2];
		printf("Date_ID1 is :%02x Date_ID2 is:%02x Date_ID3 is:%02x \n",Date_ID1,Date_ID2,Date_ID3);	
	}
	return 0;
}
//read status 0x32
int read_status32()
{
	u8 address,command,length;
	int i = 0;
	u8 buff[3] = {0};
	address = 0x54;
	command = 5;
	length = 3;
	int result = i2cget(address, command, buff, length);
	printf("result is :%d \n",result);
	printf("++++++++++++++read data++++++++++++++\n");
#if 0
	for(i=0;i<length;i++)
	{
		printf("%02x ",buff[i]);	
	}
	printf("\n");
#endif
	Date_ID1 = buff[0];
	Date_ID2 = buff[1];
	Date_ID3 = buff[2];
	printf("Date_ID1 is :%02x Date_ID2 is:%02x Date_ID3 is:%02x \n",Date_ID1,Date_ID2,Date_ID3);

}
//mcu reset
int mcu_reset()
{
	u8 cmd[2]={0};
	cmd[0] = 0x01;
	cmd[1] = 0x02;
	mcu_command(2,cmd,0,NULL);
	return 0;
}


int mcu_update_Request()
{
	u8 cmd[1] = {0};
	cmd[0] = 0x81;
	mcu_command(1,cmd,0,NULL); 
	
	return 0; 
}
int mcu_send_data(int size, u8 *buff)
{
	u8 cmd[1027] = {0};
	int i = 0;
	MSB = (u8)(block_num >> 8);
	LSB = (u8)block_num;

	cmd[0] = MSB;
	cmd[1] = LSB;
	for(i=0;i<1024;i++)
	{
		cmd[i+2] = buff[i];
	}
	cmd[1026] = CheckSumFlag(1026,cmd);
	mcu_command(1027,cmd,0,NULL);
#if 0
	printf("++++++++++++send data++++++++++++\n");
	for(i=0;i<1027;i++)
	{
		printf("%02x ",cmd[i]);
		if((i+1)%16==0)
			printf("\n");
	}
	printf("\n");
#endif	
	return 0;
	
}

int mcu_send_end_data(int size , u8 *buff)
{
	int blocknum = 252;
	int i = 0;
	u8 cmd[1027] = {0};
	MSB = blocknum >> 8;
	LSB = (u8)blocknum ;
	cmd[0] = MSB;
	cmd[1] = LSB;
	for(i=0;i<size;i++)
	{
		cmd[2+i] = buff[i];
	}
	cmd[1022] = 0x01;
	cmd[1023] = 0x01;
	cmd[1024] = 0x01;
	cmd[1025] = 0x01;	
	cmd[1026] = CheckSumFlag(1026,cmd);
	printf("++++++++++++++++send end data+++++++++++++\n");
	mcu_command(1027,cmd,0,NULL);
	for(i=0;i<1027;i++)
	{
		printf("%02x ",cmd[i]);
		if((i+1)%16==0)
			printf("\n");
	}
	printf("\n");
	return 0;
	
}
u8 *data_packag(int size,u8 *buff)
{
	u8 data_pac[16]={0};
	int i = 0;
	for(i=0;i<size;i++,i++)
	{
		if(buff[i]>=0x30 && buff[i]<=0x39)
		{
			
		}
	}

}
int update_process_read_bin()
{
	int res = 0;
	int i = 0 ;
	int offset = 0;
	int fd;
	int n;
	u8 filed_date[1024]={0};
#ifdef NORMAL
	res = io2w_init();
 	if(res==-1)
	{
		printf("init i2c error!\n");
		return -1;
	}

	mcu_update_Request();

	sleep(1);
	while(read_sta)
	{
		read_cout++;
		wait_ms(150);
		read_status(1);
		if(Request_ID==0xaa)
		{
			read_sta=0;
			read_cout=0;
		}	
		if(read_cout==30)
		{
			read_sta=0;
			printf("read data faild!\n");
			return -1;
		}
	}
	read_sta = 1;
	sleep(1);	
	if(Request_ID==0xaa)
	{
#endif		
		if((fd=open(PATH_BIN,O_RDONLY))<0)
		{
			printf("open the file faild!\n");
			return -1;
		}

		while(read_start)
		{
			block_num++;

			if((n=read(fd,filed_date,1024))==0)
	//		if((n=read(fd,filed_date,16))<16)
			{
				block_num = 0 ;
				read_start=0;
				send_start=0;
				printf("+++++++++++++read file over+++++++++++\n");
			}
			if(n!=0)
			{
				printf("+++++++++++++packet : %d+++++++++++++\n",block_num);
#if 0
				for(i=0;i<1024;i++)
				{
					printf("%02x ",filed_date[i]);
					if((i+1)%16==0)
					{
						printf("\n");
					}
				}
#endif

				if(n!=1024)
				{
					offset = 1024 - n;
					memset(filed_date+n,0xFF,offset);
				}
				while(send_start)
				{		
#ifdef NORMAL
//					error_num ++ ;
					wait_ms(20);
					mcu_send_data(1024,filed_date);
			//		mcu_send_data(16,filed_date);
					while(read_sta)
					{
						read_cout++;
						wait_ms(20);
						read_status(3);
					//	read_status32();
						if((Date_ID1 == 0x55) && (Date_ID2 == 0x55) && (Date_ID3 == 0x55))
	//					if(Request_ID==0x55)
						{
							read_sta = 0;
							read_cout = 0;
							send_start = 0;
							error_num = 0;
						}
						if(read_cout == 10)
						{
							read_cout = 0;
							read_sta = 0;
							error_num ++ ;	
						}
							
					}
					read_sta = 1;	
					if(error_num == 10)
					{
						send_start = 0;
						error_num = 0;
						read_start = 0;
						printf("+++++++++++upgrad faild ++++++++++\n");
						close(fd);
						return -1;
					}
#endif	
				}
				send_start = 1;
			}
		}
		printf("OK\n");	
	}
//	close(fd);
#ifdef NORMAL
	memset(Send_End,0xFF,1024);
	mcu_send_end_data(1024,Send_End);
	wait_ms(100);
	printf("+++++++++++++++reoly dat+++++++++++++++++\n");
	read_status(3);
	wait_ms(200);
	printf("+++++++++++++mcu update OK++++++++++++++\n");
	close(fd);
//	mcu_reset();
#endif
	return 0;
}

u8 GetByte_hex(const char *data)            //十六进制
{
	i8 cmd[3] = {0};
	cmd[0] = data[0];
	cmd[1] = data[1];
	cmd[2] = '\0';
	return (u8)strtoul(cmd,NULL,16);
}
u8 GetByte_dec(const char *data)            //十进制
{
	i8 cmd[3] ={0};
	cmd[0] = data[0];
	cmd[1] = data[1];
	cmd[2] = '\0';
	return (u8)strtoul(cmd,NULL,10);
}
int Get_Data(char *buff,int len)   
{
	int res;
	char *ret;
	int length = 0;

#if 1
	while(ret = fgets(buff,256,fp))
	{
		if(buff[7]=='0' && buff[8] =='0')
			break;
	}
#endif
	length = GetByte_dec(&(buff[1]));
	printf("len is :%d \n",length);	
	if(length<10)
		return -1;
}
int Get_Total_Line()
{
	char line[256] ={0};
	int line_sum = 0;
	int flag =1 ;
	while(fgets(line,256,fp))
	{
//		if(flag==1)
//		{
//			printf("line[7] is :%d line[8] is :%d ",line[7],line[8]);
//			flag=0;
//		}
		if((line[7]==48) && (line[8]==48))
		{	
			if(flag==1)
			{
				printf("line[7] is :%d line[8] is :%d ",line[7],line[8]);
				flag=0;
			}
		
			line_sum++;
		}
	}
	printf("line sum is:%d\n",line_sum);
	return line_sum;	
}

int update_process_read_hex()
{
	int res = 0;
	int i = 0 ;
	int fd;
	int n;
	int total_line = 0;
///	FILE *fp;
	u8 filed_date[1024]={0};
#ifdef NORMAL
#if 0
	res = io2w_init();
 	if(res==-1)
	{
		printf("init i2c error!\n");
		return -1;
	}
#endif
	if((fp=fopen(PATH_HEX,"r"))==NULL)
	{
		printf("open the hex file faild!\n");
		return -1;
	}
	total_line = Get_Total_Line();
	fseek(fp, 0, SEEK_SET);

	mcu_update_Request();

	while(read_sta)
	{
		read_cout++;
		wait_ms(150);
		read_status(1);
		if(Request_ID==0xaa)
		{
			read_sta=0;
			read_cout=0;
		}	
		if(read_cout==30)
		{
			read_sta=0;
			printf("read data faild!\n");
			return -1;
		}
	}
	read_sta = 1;	
	if(Request_ID==0xaa)
	{
#endif		

		while(read_start)
		{
			block_num++;
#if 0
			while(Get_Data(filed_date,1024)>0)
			{
			
			}
#endif
			if((n=read(fd,filed_date,1024))<1024)
			{
				block_num = 0 ;
				read_start=0;
				send_start=0;
				printf("+++++++++++++read file over+++++++++++\n");
			}
			if(n!=0)
			{ printf("+++++++++++++packet : %d+++++++++++++\n",block_num);
				for(i=0;i<1024;i++)
				{
					printf("%02x ",filed_date[i]);
					if((i+1)%16==0)
					{
						printf("\n");
					}
				}


				while(send_start)
				{		
#ifdef NORMAL
//					error_num ++ ;
					wait_ms(1);
					mcu_send_data(1024,filed_date);
					while(read_sta)
					{
						read_cout++;
						wait_ms(1);
						read_status(3);
						if((Date_ID1 == 0x55) && (Date_ID2 == 0x55) && (Date_ID2 == 0x55))
						{
							read_sta = 0;
							read_cout = 0;
							send_start = 0;
							error_num = 0;
						}
						if(read_cout == 20)
						{
							read_cout = 0;
							read_sta = 0;
							error_num ++ ;	
						}
							
					}
					read_sta = 1;	
					if(error_num == 10)
					{
						send_start = 0;
						error_num = 0;
						read_start = 0;
						printf("+++++++++++upgrad faild ++++++++++\n");
						close(fd);
						return -1;
					}
#endif	
				}
				send_start = 1;
			}
		}
		printf("OK\n");	
	}
#ifdef NORMAL
	memset(Send_End,0xFF,1024);
	mcu_send_end_data(1024,Send_End);
	wait_ms(100);
	printf("+++++++++++++++reoly dat+++++++++++++++++\n");
	read_status(3);
	wait_ms(200);
	mcu_reset();
#endif
	return 0;
}

int choice_read_mode()
{
	int res = 0;
#ifdef READ_BIN
	
	printf("++++++++++++++++coming in read bin++++++++++++\n");
	res = update_process_read_bin();

#else
	printf("+++++++++++++++coming in read hex+++++++++++\n");
	res = update_process_read_hex();	
#endif
	return res; 
       	
}
int main()
{
	printf("++++++++++++++++start update++++++++++++++++\n"); //可添加错误处理机制
#if 0
	res = io2w_init();
 	if(res==-1)
	{
		printf("init i2c error!\n");
		return -1;
	}
	read_status32();
#endif
	int res = choice_read_mode(); 
	return res;
}
