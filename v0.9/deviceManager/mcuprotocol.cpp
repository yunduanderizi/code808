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
#include "mcuprotocol.h"

#define TCC_LCDC_SET_ENABLE             0x0050
#define TCC_LCDC_SET_DISABLE            0x0051

int set_lcd_enable_disable(int flag)
{
    int fd, lcdc, layer, on;

    fd = open("/dev/fb0", O_RDWR);
    if(fd < 0)
    {
        printf("fb0  open  error!\n");
        return -1;
    }

    if(flag == 1)
        ioctl(fd, TCC_LCDC_SET_ENABLE, NULL);
    else
        ioctl(fd, TCC_LCDC_SET_DISABLE, NULL);

    close(fd);

    printf("----lcd status %d---\n",flag);
    return 0;
}
int I2c_read_mcu_code_version(unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_CODE_VERSION, buf, len);
	if(ret < 0)
	{
		printf("mcu I2c read byte err: %d\n", ret);
		return -1;
	}

	return ret;
}
int I2c_read_speed_data(unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_SPEED_ADDR, buf, len*2);
	if(ret < 0)
	{
		printf("mcu I2c read byte err: %d\n", ret);
		return -1;
	}

	return ret;
}

int I2c_read_acc_data(unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_ACC_ADDR, buf, len);
    
	return ret;
}

int I2c_read_alarm_data(unsigned char &value, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(MCUADDR, READ_MCU_ALARM_ADDR, value);
    
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
	
    ret = i2cHander.writeByte(MCUADDR, WRITE_MCU_SHUTDOWN_ADDR, value);
    
	return ret;
}

int I2c_write_speed_pluse_factor_data(int value)
{
    int ret = -1;	
    IBI2CControl i2cHander;
    unsigned char buf[2];

    if (i2cHander.checkOK() == 0)
    {
        printf("Check mcu i2cHander err!\n");
        return -1; 
    }
    buf[0] = (value >> 8) & 0xFF;
    buf[1] = value & 0xFF;
    ret = i2cHander.writeBlock(MCUADDR, WRITE_MCU_SPEED_FACTOR_ADDR, buf, 2);
    return ret;
}

int I2c_write_lap_pluse_factor_data(int value)
{
    int ret = -1;	
    IBI2CControl i2cHander;
    unsigned char buf[2];

    if (i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    
    buf[1] = (value >> 8) & 0xFF;
    buf[0] = value & 0xFF;
    ret = i2cHander.writeBlock(MCUADDR, WRITE_MCU_LAP_FACTOR_ADDR, buf, 2);

    return ret;
}


int I2c_write_DB9_7_check_data(unsigned char value)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if (i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }

    ret = i2cHander.writeByte(MCUADDR, WRITE_MCU_DB9_7_CHECk_ADDR, value);

    return ret;
}

int I2c_read_analog_quantity_data(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_ANALOGUE_QUANTITY_ADDR, buf, len*2);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_camera_status(unsigned char &value)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(CAMERAADDR, READ_CAMERA_STATUS, value);
    
	return ret;
}

int I2c_read_can_data(unsigned char *buf, int len)
{
    int ret = -1;
    IBI2CControl i2cHander;

    if (i2cHander.checkOK() == 0)
    {
        printf("Check mcu i2cHander err!\n");
        return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_CAN_DATA_ADDR, buf, len);
    if(ret < 0)
    {
        printf("mcu I2c read byte err: %d\n", ret);
        return -1;
    }

    return ret;
}
int I2c_read_startup_mode_data(unsigned char &value, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(MCUADDR, READ_MCU_START_UP_MODE, value);
    
	return ret;
}

int I2c_read_startingup_data_num(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_OPENDATA_NUM_ADDR, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_startingup_data_info(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_OPENDATA_ADDR, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_can_up_param_info(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_CAN_UPINFO_ADDR, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_can_up_data_info(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_CAN_UPDATA_ADDR, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_write_can_down_param_info(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.writeBlock(MCUADDR, WRITE_MCU_CAN_DOWNINFO_ADDR, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c write byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_write_can_down_data_info(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.writeBlock(MCUADDR, WRITE_MCU_CAN_DOWNDATA_ADDR, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c write byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_write_clear_mileage_data(int value)
{
    int ret = -1;	
    IBI2CControl i2cHander;
    unsigned char buf[3];

    if (i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    
    buf[0] = (value >> 16) & 0xFF;
    buf[1] = (value >> 8) & 0xFF;
    buf[2] = value & 0xFF;
    ret = i2cHander.writeBlock(MCUADDR, WRITE_MCU_CLEAR_MILEAGE_ADDR, buf, 3);

    return ret;
}

int I2c_read_total_mileage_data(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_TOTAL_MILEAGE_ADDR, buf, len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_D310_can_ready_data(unsigned char &value)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readByte(MCUADDR, READ_MCU_D310_CAN_READY_ADDR, value);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_D310_can_pulsefactor_data(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_D310_CAN_PULSE_FACTOR_ADDR, buf,len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_D310_can_carvin_data(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_D310_CAN_CAR_VIN_ADDR, buf,len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_D310_can_carip_data(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_D310_CAN_CAR_IP_ADDR, buf,len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;
}

int I2c_read_D310_pulse_per_second(unsigned char *buf, int len)
{
    int ret = -1;	
    IBI2CControl i2cHander;

    if(i2cHander.checkOK() == 0)
    {
    	printf("Check mcu i2cHander err!\n");
    	return -1; 
    }
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_D310_PULSE_PER_SECOND, buf,len);
    if(ret < 0)
    {
    	printf("mcu I2c read byte err: %d\n", ret);
    	return -1;
    }

    return ret;

}

int I2c_write_CM_Control_Module_data(unsigned char value)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeByte(MCUADDR, WRITE_MCU_CM_CONTROL_MODULE_ADDR, value);
    
	return ret;
}

int I2c_read_CM_reply_info_data(unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_CM_REPLY_INFO_ADDR, buf, len);
    
	return ret;
}

int I2c_read_CM_sensor_info_data(unsigned char *buf, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readBlock(MCUADDR, READ_MCU_CM_SENSOR_INFO_ADDR, buf, len);
    
	return ret;
}

int I2c_write_CM_Speed_Control_data(unsigned char value)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeByte(MCUADDR, WRITE_MCU_CM_SPEED_CONTROL_ADDR, value);
    
	return ret;
}

int I2c_read_CM_NO_VECU_data(unsigned char &value)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(MCUADDR, READ_MCU_CM_NO_VECU_INFO_ADDR, value);
    
	return ret;
}

#if 0
int main(int argc, char* argv[])
{
    int ret = -1;
	unsigned char speed_buf[128];
    unsigned char value;
	int speed_len = 20, distance=0;
    int i=0;

    
    if(!strcmp(argv[1], "1"))
    {
        ret = I2c_read_acc_data(value,1);
        printf("acc=0x%02x\n",value);
    }
    else
    {
        ret = I2c_read_speed_data(speed_buf,10);
        for(i= 0; i<ret; i++)
            printf(" 0x%02x ", speed_buf[i]);
    }
    #if 0
    distance = 0;
    for(i=0;i<speed_len/2; i++)
    {
        distance += speed_buf[i*2]/18;
    }
    #endif    
    
	
	return 0;

}
#endif


