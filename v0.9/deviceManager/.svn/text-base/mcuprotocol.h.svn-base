#ifndef _MCU_PROTOCOL_H_
#define _MCU_PROTOCOL_H_

#define MCUADDR 0x54
#define CAMERAADDR  0x45

#define READ_MCU_CODE_VERSION   0x05
#define READ_MCU_ACC_ADDR       0x1A
#define READ_MCU_SPEED_ADDR     0x1B
#define READ_MCU_ALARM_ADDR     0x1C
#define READ_MCU_ANALOGUE_QUANTITY_ADDR     0x20
#define READ_MCU_CAN_DATA_ADDR              0x21
#define READ_MCU_START_UP_MODE              0x06
#define READ_MCU_OPENDATA_NUM_ADDR          0x22
#define READ_MCU_OPENDATA_ADDR              0x23
#define READ_MCU_CAN_UPINFO_ADDR            0x24
#define READ_MCU_CAN_UPDATA_ADDR            0x25
#define READ_MCU_TOTAL_MILEAGE_ADDR         0x29
#define READ_MCU_D310_CAN_READY_ADDR        0x2A
#define READ_MCU_D310_CAN_PULSE_FACTOR_ADDR 0x2B
#define READ_MCU_D310_CAN_CAR_VIN_ADDR      0x2C
#define READ_MCU_D310_CAN_CAR_IP_ADDR       0x2D
#define READ_MCU_D310_PULSE_PER_SECOND      0x36
#define READ_MCU_CM_REPLY_INFO_ADDR         0x43
#define READ_MCU_CM_SENSOR_INFO_ADDR        0x44
#define READ_MCU_CM_NO_VECU_INFO_ADDR       0x46


#define WRITE_MCU_SHUTDOWN_ADDR     0x01
#define WRITE_MCU_SPEED_FACTOR_ADDR 0x18      //速度脉冲系数
#define WRITE_MCU_LAP_FACTOR_ADDR   0x19      // 单圈脉冲系数
#define WRITE_MCU_DB9_7_CHECk_ADDR  0x1D      // 检定DB9-7
#define WRITE_MCU_CAN_DOWNINFO_ADDR   0x26
#define WRITE_MCU_CAN_DOWNDATA_ADDR   0x27
#define WRITE_MCU_CLEAR_MILEAGE_ADDR   0x28
#define WRITE_MCU_CM_CONTROL_MODULE_ADDR  0x42
#define WRITE_MCU_CM_SPEED_CONTROL_ADDR   0x45


#define SHUTDOWN_FLAG       0x01
#define REBOOT_FLAG         0x02
#define SOFT_SHUTDOWN_FLAG  0x04

#define READ_CAMERA_STATUS  0x01    //获取摄像头连接状态

typedef enum
{
    DB9_MILEAGE_CHECK = 0x1,             
    DB9_PULSE_FACTOR_CHECK = 0x02, 
    DB9_TIME_CHECK = 0x04,
    DB9_OUT_CHECK = 0x08          
}DB9_7_CHECK;

int I2c_read_mcu_code_version(unsigned char *buf, int len);
int I2c_read_speed_data(unsigned char *speed_buf, int speed_len);
int I2c_read_acc_data(unsigned char *buf, int len);
int I2c_read_alarm_data(unsigned char &value, int len);
int I2c_read_analog_quantity_data(unsigned char *buf, int len);
int I2c_read_camera_status(unsigned char &value);
int I2c_read_can_data(unsigned char *buf, int len);
int I2c_read_startup_mode_data(unsigned char &value, int len);
int I2c_read_startingup_data_num(unsigned char *buf, int len);
int I2c_read_startingup_data_info(unsigned char *buf, int len);
int I2c_read_can_up_param_info(unsigned char *buf, int len);
int I2c_read_can_up_data_info(unsigned char *buf, int len);
int I2c_read_total_mileage_data(unsigned char *buf, int len);
int I2c_read_D310_can_ready_data(unsigned char &value);
int I2c_read_D310_can_pulsefactor_data(unsigned char *buf, int len);
int I2c_read_D310_can_carvin_data(unsigned char *buf, int len);
int I2c_read_D310_can_carip_data(unsigned char *buf, int len);
int I2c_read_D310_pulse_per_second(unsigned char *buf, int len);
int I2c_read_CM_reply_info_data(unsigned char *buf, int len);
int I2c_read_CM_sensor_info_data(unsigned char *buf, int len);
int I2c_read_CM_NO_VECU_data(unsigned char &value);


int I2c_write_shutdown_data(unsigned char value);
int I2c_write_speed_pluse_factor_data(int value);
int I2c_write_lap_pluse_factor_data(int value);
int I2c_write_DB9_7_check_data(unsigned char value);
int I2c_write_can_down_param_info(unsigned char *buf, int len);
int I2c_write_can_down_data_info(unsigned char *buf, int len);
int I2c_write_clear_mileage_data(int value);
int I2c_write_CM_Control_Module_data(unsigned char value);
int I2c_write_CM_Speed_Control_data(unsigned char value);

int set_lcd_enable_disable(int flag);

#endif
