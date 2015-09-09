#include "io.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define SI4740_I2C_ADDR  0x54                      //8925 radio

static int g_io_control_fd=-1;
int io2w_init(void)
{

    if(g_io_control_fd>=0)
        return -1;
    char filename[]="/dev/i2c-0";

    g_io_control_fd=open(filename,O_RDWR);
    if(g_io_control_fd<0){
        printf("----open %s node failed!----\n",filename);
        g_io_control_fd=-1;
        return -1;
    }
    return 0;
} int io2w_write(int number_bytes, u8 *data_out) {
	int ret=-1;
	int i=0;
	int j=0;
	struct i2c_rdwr_ioctl_data queue;
	struct i2c_msg msg;
	int rTimes=2;
	printf("g_io_control_fd is :%d\n",g_io_control_fd);
	if(g_io_control_fd<0)
		return -1;

	msg.addr=SI4740_I2C_ADDR;
	msg.flags=0;
	msg.buf=data_out;
	msg.len=number_bytes;
	queue.msgs=&msg;
	queue.nmsgs=1;
	printf("write date:\n");
	for(j=0;j<number_bytes;j++)
	{
		printf("%02x ",data_out[j]);
		if((j+1)%16==0)
			printf("\n");
	}
	printf("\n");
	while( (ret<0)&&(rTimes--) )
	{
		if( (ret=ioctl(g_io_control_fd,I2C_RDWR,&queue))<0 )
		{
			ret=-1;
			usleep(50);
		}else{
			ret=msg.len;
		}
	}

	return ret;
}
int io2w_read(int number_bytes, u8 *data_in)
{
	int ret= -1;
	struct i2c_rdwr_ioctl_data queue;
	struct i2c_msg msg;
	int rTimes=2;

	if(g_io_control_fd<0)
		return -1;

	msg.addr=SI4740_I2C_ADDR;
	msg.flags= 1;
	msg.buf=data_in;
	msg.len=number_bytes;
	
	queue.msgs=&msg;
	queue.nmsgs=1;

	
	while( (ret<0)&&(rTimes--) )
	{
		if( (ret=ioctl(g_io_control_fd,I2C_RDWR,&queue))<0 )
		{
			ret=-1;
			usleep(50);
		}else{
			ret=msg.len;
		}
	}

	return ret;

}
int i2cget(int address, int daddress, unsigned char *values, unsigned char length)
{
	int size,res;
	long funcs;
	int nTimes;
	if(g_io_control_fd <= 0)
	{
		return -1;
	}
	if( (address<0) || (address>0x7f) )
	{
		return -1;
	}
	if( (daddress<0) || (daddress>0xff) )
	{
		return -1;
	}
	/* check adapter functionality */
	if (ioctl(g_io_control_fd,I2C_FUNCS,&funcs) < 0)
	{   
		return -1; 
	}   
	if(!(funcs & I2C_FUNC_SMBUS_I2C_BLOCK))
	{   
		return -1; 
	}
	res = -1;
	nTimes = 1;
	while( (res<0) && (nTimes--) )
    	{
		/* use FORCE so that we can write registers even when a driver is also running */
		if ((res = ioctl(g_io_control_fd,I2C_SLAVE_FORCE,address)) < 0)
		{
			usleep(10000);
		}
		else
		{
			res = i2c_smbus_read_i2c_block_data(daddress, length, values);
			if(res<0)
			{
				usleep(10000);
			}
		}
	}
    return res;
}

/* Returns the number of read bytes */
int i2c_smbus_read_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values)
{
	union i2c_smbus_data data;
	int i;
	data.block[0] = length;
	if (i2c_smbus_access(I2C_SMBUS_READ,command,
				I2C_SMBUS_I2C_BLOCK_DATA,&data))
		return -1; 
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}   
}

int i2c_smbus_access(char read_write, unsigned char command,int size, union i2c_smbus_data *data)
{
       	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(g_io_control_fd,I2C_SMBUS,&args);
}

