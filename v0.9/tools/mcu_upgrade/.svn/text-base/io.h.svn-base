#ifndef __IO_H__
#define __IO_H__

#include "typedefs.h"

int io2w_init(void);
int  io2w_write(int number_bytes, u8 *data_out);
int   io2w_read(int  number_bytes, u8 *data_in);
int i2c_smbus_read_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values);
int i2cget(int address, int daddress, unsigned char *values, unsigned char length);
int i2c_smbus_access(char read_write, unsigned char command,int size, union i2c_smbus_data *data);

#endif  
