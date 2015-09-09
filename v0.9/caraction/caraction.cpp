#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <i2ctl.h>
#include <stdarg.h> 
#include <tinyxml.h>
#include <string>
#include <termios.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "DevCtrl.h"

#define AUXIN_USED		1

#ifdef _8925_8D1
#define GPA_BASE                        0x74200000
#else
#define GPA_BASE                        0xF0102000
#endif

#define GPA_DAT							0x00
#define GPA_EN							0x04
#define OFFSET_PE                       0x1C
#define OFFSET_PS                       0x20

#ifdef _8925_8D1
#define OFFSET_FUN0                     0x30
#define OFFSET_FUN1                     0x34
#define OFFSET_FUN2                     0x38
#define OFFSET_FUN3                     0x3C
#else
#define OFFSET_FUN0                     0x24
#define OFFSET_FUN1                     0x28
#define OFFSET_FUN2                     0x2c
#define OFFSET_FUN3                     0x30
#endif

static volatile int ThreadCommand = 0;
char *gpa_vbase, *gpe_vbase, *gpadc_vbase;
int fd = -1;  
int udpfd;
int open_serialport(char *dev);
int at_command(int fd, char *cmd, int to);
int InitGF();
int MuteGF();
int UnmuteGF();
int DetectGF();
int PowerOffGF();

int headinit();
int headclose();

int headclose()
{
	close(fd);
	munmap(gpa_vbase, OFFSET_FUN3);
	return 0;
}
#define GPION_MUTE 17
#define GPION_DET 3
int headinit()
{
	udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(udpfd < 0){
		fprintf(stderr,"socket error!!! \n");
		return -1;
	}

	fd = open("/dev/mem",O_RDWR|O_SYNC);
	if(fd < 0){
		fprintf(stderr,"mem:open()\n");
		return -1;
	}
	gpa_vbase = (char *) mmap( NULL, 250,  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, fd, GPA_BASE);
	if(gpa_vbase == NULL || gpa_vbase ==  MAP_FAILED ){
		fprintf(stderr,"mmap:map file error \n");
		return -1;
	}
#ifdef _8925_8D1
	//gpa_vbase += 0x100; //GPE_BASE           
	gpe_vbase = (gpa_vbase + 0x100);        //GPE17:GF MUTE
	gpadc_vbase = (gpa_vbase + 0x200);      //GPADC03:Detect aux in 
	*(volatile unsigned int*)(gpe_vbase + OFFSET_FUN2) &= ~(0xf << 4); 
	*(volatile unsigned int*)(gpe_vbase + GPA_EN) |= (0x1 << GPION_MUTE);
	*(volatile unsigned int*)(gpadc_vbase + OFFSET_FUN0) &= ~(0xf << 12); 
	*(volatile unsigned int*)(gpadc_vbase + GPA_EN) &= ~(0x1 << GPION_DET);
#else
	*(volatile unsigned int*)(gpa_vbase + OFFSET_FUN0) &= ~(0xf << 12); 
	*(volatile unsigned int*)(gpa_vbase + GPA_EN) &= ~(0x1 << 11);
#endif


	return 0;
}

#define UDP_PORT 8905
void AUXIN_send(unsigned int inserted)
{
	//printf("AUXIN_send: %d\n", inserted);
	int port;
	struct sockaddr_in address;

	port = UDP_PORT;
	memset(&address, 0,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(port);
	sendto(udpfd, &inserted, sizeof(inserted), 0, (struct sockaddr *)&address, sizeof(address));
}

void * WatchGFThread(void*arg)
{	
	unsigned int ins = 0;
	while( ThreadCommand == 0 )
	{
#ifdef _8925_8D1
		ins = *(volatile unsigned int*)(gpadc_vbase + GPA_DAT) & (0x1 << GPION_DET); 
#else
		ins = *(volatile unsigned int*)(gpa_vbase + GPA_DAT) & (0x1 << 11); 
#endif
#ifdef AUXIN_USED
		AUXIN_send(ins);
#else
		if(!ins){
			fprintf(stderr,"MuteGF:headset has inserted !\n");
			MuteGF();	
		}else{
			UnmuteGF();
		}
#endif

		usleep(1000*500);
		DetectGF();
	}

	pthread_detach(pthread_self());
	return 0;
}

int main(int argc, char* argv[])
{

	//#ifndef _8925_8D1
	headinit();
	//#endif
	if( argc == 1 )
	{
		daemon(0, 1);
		//改在启动脚本初始化功放，此处不考虑初始化功放
		//		InitGF();
		//		UnmuteGF();

		//启动功放过载保护		
		pthread_t idGF = 0;
		pthread_create( &idGF, 0, WatchGFThread, 0 );
		while( 1 )
		{
			sleep(100);
		}
	}
	else if( argc == 2 )
	{
		if( strcmp( argv[1], "opengf" ) == 0  )
		{
			InitGF();
		}
		else if( strcmp( argv[1], "mutegf" ) == 0  )
		{
			MuteGF();
		}
		else if( strcmp( argv[1], "unmutegf" ) == 0  )
		{
			UnmuteGF();
		}
		else if( strcmp( argv[1], "poweroffgf" ) == 0  )
		{
			PowerOffGF();
		}
		else if( strcmp( argv[1], "revivegf" ) == 0  )
		{
			DetectGF();
		}
		else
			printf("using as:\n\tcaraction opengf\n\tcaraction mutegf\n\tcaraction unmutegf\n\tcaraction poweroffgf\n\tcaraction revivegf\n");
	}
END:	
	headclose();
	return 0;

}


unsigned char tas5414_master[] =			// iic address: 0x6c
{
	0X08,0X55,            //gain choose.0x00-12db,0x55-20db,0xaa-26db,0xff-32db
	0X09,0X00,            //dc offset reduction and current limit
	0X0A,0X0D,            //switch frequency choose
	0X0B,0X40,            //master and slaver choose
	0X0D,0X00,            //output control
	0X0E,0X8E,            //positive dc detect threshold selection
	0X0F,0X3D,            //negative dc detect threshold selection
};

int InitGF()
{
#ifndef _8925_8D1
	int file,i;
	file = i2copen(0);
	if( file == 0 )
		return -1;

	Incar_initvolume();

	//把声音设置为工作状态
	i2cset( file, 0x50, 0x00, 0xFF, 'b' );
	//必须进行延时，等待功放稳定，否则配置无效
	usleep( 200000 );

	//必须读0x00 和0x01地址，清除失败状态
	i2cget(file,0x6c,0x00,'b');
	i2cget(file,0x6c,0x01,'b');

	//以下功能检查是否输出短路
	i2cset( file, 0x50, 0x0b, 0x0F, 'b' );
	usleep( 200000 );//等 200 ms

	int nCount = 10;
	int nState = i2cget(file,0x6c,0x07,'b');
	while( (nState != 0x00) && (nCount-- > 0))
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x07,'b');
		fprintf(stderr, "InitGF nState is %x, nCount is %d\n", nState, nCount);
	}

	int nState1 = i2cget(file,0x6c,0x02,'b');
	int nState2 = i2cget(file,0x6c,0x03,'b');
	if( nState1 != 0x00 || nState2 != 0x00 )
	{
		printf("tas5414_master 0x02 0x03 address is %x %x\n", nState1, nState2);
		return -2;
	}

	//写用户配置参数
	unsigned char* p = tas5414_master;
	for(i = 0;i < (int)(sizeof(tas5414_master)/sizeof(tas5414_master[0]));i+=2)
	{
		i2cset(file,0x6c,*p,*(p+1),'b');
		p += 2;
	}

	i2cclose(file);

	usleep( 3000000 );//必须进行延时，等待功放稳定，否则立即切换界面不能屏蔽电流冲击声
#endif
	return 0;
}

int MuteGF()
{
#ifdef _8925_8D1
	//*(volatile unsigned int*)(gpa_vbase + OFFSET_PE) &= ~(0x1 << GPIO_NUM); 
	//*(volatile unsigned int*)(gpa_vbase + OFFSET_PS) &= ~(0x1 << GPIO_NUM); 
	*(volatile unsigned int*)(gpe_vbase + OFFSET_FUN2) &= ~(0xf << 4); 
	*(volatile unsigned int*)(gpe_vbase + GPA_EN) |= (0x1 << GPION_MUTE);
	*(volatile unsigned int*)(gpe_vbase + GPA_DAT) &= ~(0x1 << GPION_MUTE); 
#else
	int file;
	file = i2copen(0);	
	if( file == 0 )
		return -1;

	i2cset(file,0x6c,0x0c,0x1f,'b');

	i2cclose(file);
#endif
	return 0;
}	

int UnmuteGF()
{
#ifdef _8925_8D1
	//*(volatile unsigned int*)(gpa_vbase + OFFSET_PE) |= (0x1 << GPIO_NUM); 
	//*(volatile unsigned int*)(gpa_vbase + OFFSET_PS) |= (0x1 << GPIO_NUM); 
	*(volatile unsigned int*)(gpe_vbase + OFFSET_FUN2) &= ~(0xf << 4); 
	*(volatile unsigned int*)(gpe_vbase + GPA_EN) |= (0x1 << GPION_MUTE);
	*(volatile unsigned int*)(gpe_vbase + GPA_DAT) |= (0x1 << GPION_MUTE); 
#else
	int file;
	file = i2copen(0);	
	if( file == 0 )
		return -1;

	i2cset(file,0x6c,0x0c,0x00,'b');
	usleep( 35000 );//等 35 ms
	int nState = i2cget(file,0x6c,0x06,'b');
	int nCount = 3;
	while( (nState != 0x0f)  && (nCount-- > 0) )
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x06,'b');
		fprintf(stderr, "UnmuteGF nState is %x, nCount is %d\n", nState, nCount);
	}

	i2cclose(file);
#endif
	return 0;
}	

int DetectGF()
{
#ifndef _8925_8D1
	int file;
	file = i2copen(0);
	if(file == 0)
		return -1;

	int nState1 = i2cget(file,0x6c,0x00,'b');
	int nState2 = i2cget(file,0x6c,0x01,'b');
	if( nState1 != 0x00 || nState2 != 0x00 )
	{
		UnmuteGF();
	}
	i2cclose(file);
#endif
	return 0;
}

int PowerOffGF()
{
#ifndef _8925_8D1
	int file;
	file = i2copen(0);	
	if( file == 0 )
		return -1;

	//由PLAY-MODE进入到MUTE-MODE
	i2cset(file,0x6c,0x0c,0x10,'b');
	int nCount = 10;
	int nState = i2cget(file,0x6c,0x06,'b');
	while( (nState != 0xf0) && (nCount-- > 0) )
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x06,'b');
		fprintf(stderr, "PowerOffGF MUTE-MODE i2cget 0x6c,0x06 nState is %x, nCount is %d\n", nState, nCount);
	}

	//由MUTE-MODE 进入到LOW-LOW-MODE
	i2cset(file,0x6c,0x0d,0x0f,'b');
	nCount = 10;
	nState = i2cget(file,0x6c,0x05,'b');
	while( (nState != 0xf0) && (nCount-- > 0) )
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x05,'b');
		fprintf(stderr, "PowerOffGF LOW-LOW-MODE i2cget 0x6c,0x05 nState is %x, nCount is %d\n", nState, nCount);
	}

	//由LOW-LOW-MODE 进入到HIZ-MODE
	i2cset(file,0x6c,0x0c,0x1f,'b');
	nCount = 10;
	nState = i2cget(file,0x6c,0x05,'b');
	while( (nState != 0x0f) && (nCount-- > 0) )
	{
		usleep( 35000 );//等 35 ms
		nState = i2cget(file,0x6c,0x05,'b');
		fprintf(stderr, "PowerOffGF HIZ-MODE i2cget 0x6c,0x05 nState is %x, nCount is %d\n", nState, nCount);
	}

	i2cclose(file);
#endif
	return 0;
}
