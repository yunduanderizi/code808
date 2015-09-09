#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "i2ccontrol.h"
#include "McuProtocol.h"
/**********************读取开关机原因*************/
int I2c_read_mcu_rst_down_cause(unsigned char &value, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	ret = i2cHander.readByte(MCUADDR, READ_MCU_RST_DOWN_CAUSE, value);
    
	return ret;
}

/**********************CPU下发一条心跳包给MCU*************/
/*
注意：MCU应改变字节
      BYTE[11]:流水号高8位
      BYTE[12]:流水号低8位
      BYTE[13]:校验码
其他数据不变！！！
*/

int I2c_write_heartbeat_information( unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeBlock(MCUADDR, WRITE_HEARTBEAT_INFORMATION, buf , len);
    if(ret < 0)
    {
    	printf("mcu I2c write byte err: %d\n", ret);
    	return -1;
    }
	return ret;
}
/**********************CPU下发一条定位数据信息*************/
/*
注意：MCU应改变字节
      BYTE[11]:流水号高8位
      BYTE[12]:流水号低8位
      BYTE[35]:年 BCD码转换
      BYTE[36]:月 BCD码转换
      BYTE[37]:日 BCD码转换
      BYTE[38]:时 BCD码转换
      BYTE[39]:分 BCD码转换
      BYTE[40]:秒 BCD码转换
      BYTE[73]:校验码
其他数据不变！！！
*/
int i2c_write_location_information( unsigned char *buf, int len)
{
		int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeBlock(MCUADDR,WRITE_LOCATION_INFORMATION, buf , len);
    if(ret < 0)
    {
    	printf("mcu I2c write byte err: %d\n", ret);
    	return -1;
    }
	return ret;
}

/**********************CPU下发：最后一条流水号；心跳休眠间隔；定时上报间隔；心跳最后一条上报时间；定时上报最后一条时间。*************/
/*
 BYTE[0]:流水号高8位；      BYTE[1]:流水号低8位；
 BYTE[2]:心跳间隔高8位；    BYTE[3]:心跳间隔低8位；
 BYTE[4]:定位信息间隔高8位；BYTE[5]:定位信息间隔低8位；

 */
int i2c_write_time_interval( unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeBlock(MCUADDR, WRITE_TIME_INTERVAL, buf , len);
    if(ret < 0)
    {
    	printf("mcu I2c write byte err: %d\n", ret);
    	return -1;
    }
	return ret;
}

/**********************关机校时*****************************/
/*
BYTE[0]： 年 高8位
BYTE[1]： 年 低8位
BYTE[2]： 月
BYTE[3]： 日
BYTE[4]： 时
BYTE[5]： 分
BYTE[6]： 秒
*/
int i2c_write_calibration_time( unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeBlock(MCUADDR, WRITE_CALIBRATION_TIME, buf , len);
    if(ret < 0)
    {
    	printf("mcu I2c write byte err: %d\n", ret);
    	return -1;
    }
	return ret;
}
/**********************MCU上报下发指令的所有内容*****************************/
int i2c_read_issued_information(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_ISSUED_INFORMATION, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}
/**********************MCU上报最后一条流水号和还有多长时间启动心跳和定时上报的时间*****************************/
/*
BYTE[0]： 流水号高8位
BYTE[1]:  流水号低8位
BYTE[2]： 启动心跳时间 秒高8位
BYTE[3]： 启动心跳时间 秒低8位

*/
int i2c_read_time_interval(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_TIME_INTERVAL, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}
int I2c_write_shutdown_data(unsigned char value)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeByte(MCUADDR, WRITE_MCU_POWER_MANAGER, value);
    
	return ret;
}

int I2c_write_Parameter_data(unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;
/*
	for (int i =0;i<len;i++)
	{
		printf("%02x ",buf[i]);
		if ((i+1)%15==0)
			printf("\n");
	}
*/
	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeBlock(MCUADDR, WRITE_READ_PARAMETER_DATA, buf , len);
    if(ret < 0)
    {
    	printf("mcu I2c write byte err: %d\n", ret);
    	return -1;
    }
	return ret;
}
int I2c_read_setting_data(unsigned char *buf, int len)
{
	int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_READ_PARAMETER_DATA, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}
int I2c_write_firstserver_data(unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;
	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
    ret = i2cHander.writeBlock(MCUADDR, WRITE_FIRSTSERVER_INFORMATION, buf , len);
    if(ret < 0)
    {
    	printf("mcu I2c write byte err: %d\n", ret);
    	return -1;
    }
	return ret;
}