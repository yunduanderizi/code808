#include<stddef.h>
#include"typedefs.h"
#include"commanddefs.h"
#include"asc1411.h"

int si475x_command(u8 cmd_size, u8 *cmd, u8 reply_size, u8 *reply);
int si475x_reset();
void si475x_set_frequency(u8 am_or_fm ,u8 high ,u8 low);
u16 si475x_seek_radio(u8 up_or_down);
void si475x_configuration_parameter();
int si475x_read_current_status(size,buf);
u8 si475x_readStatus();
////////////////u16 get_freq();

pthread_t read_tid = 1;

void *read_status_pthread(void *data)
{
	while(1)
	{
		read_status();
        //printf("real time read!\n");
		read_tid = pthread_self();
		wait_ms(500);
	}
}
void asc1411_configure(void)
{
	si475x_configuration_parameter();
}

void initFM(void)
{
	u8 high = 0x22;          //defalut frequency value 87.5MHz
	u8 low = 0x2E;
	u8 am_or_fm = 0;
	si475x_set_frequency(am_or_fm,high,low);
}
void initAM(void)
{
	u8 high = 0x02;
	u8 low = 0x13;
	u8 am_or_fm = 0x01;
	si475x_set_frequency(am_or_fm,high,low);
}
int change_fm(u16 channel)
{
	u8 high = channel >> 8;
	u8 low  = channel &0xFF;
	u8 am_or_fm = 0;
	si475x_set_frequency(am_or_fm,high,low);
	return 0;
}

int change_am(u16 channel)
{
	u8 high = channel >> 8;
	u8 low = channel & 0xFF;
	u8 am_or_fm = 0x01;
	printf("++++++++++++++++change_am+++++++++++++++\n");
	si475x_set_frequency(am_or_fm,high,low);
	return 0;
}
int seek(int up_or_down)
{
	return (int)(si475x_seek_radio(up_or_down));
}
int read_status()
{
	return si475x_readStatus();
}
int Reset()
{
	return si475x_reset();	
}
int realtime_readstatus()
{
	int ret = -1;
	if(read_tid==1)
	{
		ret = pthread_create(&read_tid,NULL,read_status_pthread,NULL);
		if(ret<0)
		{
			printf("Create read status pthread faild!\n");
			return ret;
		}	
	}
	return 0;	
}
#if 0
int si475x_getPartInformation(int size,char *buf)
{
	return si475x_read_current_status(size,buf); 
}
#endif
