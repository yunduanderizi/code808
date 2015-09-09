#ifndef _MCU_PROTOCOL_H_
#define _MCU_PROTOCOL_H_

#define MCUADDR 						       0x54
#define READ_MCU_RST_DOWN_CAUSE 		0x06
#define WRITE_HEARTBEAT_INFORMATION		0x30
#define WRITE_TIME_INTERVAL				0x32
#define WRITE_CALIBRATION_TIME			0x33
#define READ_ISSUED_INFORMATION			0x34
#define READ_TIME_INTERVAL				0x35
#define WRITE_MCU_POWER_MANAGER		0x01
#define WRITE_SECONDSERVER_INFO			0X38

int I2c_read_mcu_rst_down_cause(unsigned char &value, int len);
int I2c_write_heartbeat_information( unsigned char *buf, int len);
int i2c_write_location_information( unsigned char *buf, int len);
int i2c_write_time_interval( unsigned char *buf, int len);
int i2c_write_calibration_time( unsigned char *buf, int len);
int i2c_read_issued_information(unsigned char *buf, int len);
int i2c_read_time_interval(unsigned char *buf, int len);
int I2c_write_shutdown_data(unsigned char value);
int I2c_write_secondserver_data(unsigned char *buf, int len);


#endif
