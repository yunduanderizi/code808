/**********************************************************************
 Filename: ib_i2cctl.h
 Description: Declare the IBI2CControl class.
 Author: Xing Qianqian
**********************************************************************/

#ifndef _IB_I2CCTL_H_
#define _IB_I2CCTL_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define IB_MAX_I2C_NODE_LENGTH   0x20

class IBI2CControl
{
public:
    IBI2CControl(int i2cbus = 0);
    virtual ~IBI2CControl();

    int i2cset(int address, unsigned char *values, unsigned char length);
    int i2cset(int address, int daddress, unsigned char *values, unsigned char length);
    int i2cset(int address, int daddress, int value, char b_w = 'b');
    int i2cget(int address, int daddress, char b_w = 'b');
    int i2cget(int address, unsigned char *values, unsigned char length);
    int i2cget(int address, int daddress, unsigned char *values, unsigned char length);
    int i2cget(int address);
protected:
    int i2cset_inner(unsigned char *values, unsigned char length);
    int i2cset_inner(int daddress, unsigned char *values, unsigned char length);
    int i2cset_inner(int daddress, int value, char b_w);
    int i2cget_inner(int daddress,char b_w);
    int i2cget_inner(unsigned char *values, unsigned char length);
    int i2cget_inner(int daddress, unsigned char *values, unsigned char length);
    int i2cget_inner();

    int m_retry;
    int m_sleep_interval;
    int m_address;
    
private:
    int i2c_smbus_access(char read_write, unsigned char command,
                         int size, union i2c_smbus_data *data);
    int i2c_smbus_write_quick(unsigned char value);
    int i2c_smbus_read_byte();
    int i2c_smbus_write_byte(unsigned char value);
    int i2c_smbus_read_byte_data(unsigned char command);
    int i2c_smbus_write_byte_data(unsigned char command, unsigned char value);
    int i2c_smbus_read_word_data(unsigned char command);
    int i2c_smbus_write_word_data(unsigned char command, unsigned short value);
    int i2c_smbus_process_call(unsigned char command, unsigned short value);
    int i2c_smbus_read_block_data(unsigned char command, unsigned char *values);
    int i2c_smbus_write_block_data(unsigned char command, unsigned char length, unsigned char *values);
    int i2c_smbus_read_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values);
    int i2c_smbus_write_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values);
    int i2c_smbus_block_process_call(unsigned char command, unsigned char length, unsigned char *values);
    int i2copen();
    void i2cclose();

    int m_i2cbus;
    int m_file;
};

inline int IBI2CControl::i2c_smbus_access(char read_write, unsigned char command,
                                          int size, union i2c_smbus_data *data)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    return ioctl(m_file,I2C_SMBUS,&args);
}

inline int IBI2CControl::i2c_smbus_write_quick(unsigned char value)
{
    return i2c_smbus_access(value, 0, I2C_SMBUS_QUICK, NULL);
}

inline int IBI2CControl::i2c_smbus_read_byte()
{
    union i2c_smbus_data data;
    if (i2c_smbus_access(I2C_SMBUS_READ,0,I2C_SMBUS_BYTE,&data))
        return -1;
    else
        return 0x0FF & data.byte;
}

inline int IBI2CControl::i2c_smbus_write_byte(unsigned char value)
{
    return i2c_smbus_access(I2C_SMBUS_WRITE,value,
                            I2C_SMBUS_BYTE,NULL);
}

inline int IBI2CControl::i2c_smbus_read_byte_data(unsigned char command)
{
    union i2c_smbus_data data;
    if (i2c_smbus_access(I2C_SMBUS_READ,command,
                         I2C_SMBUS_BYTE_DATA,&data))
        return -1;
    else
        return 0x0FF & data.byte;
}

inline int IBI2CControl::i2c_smbus_write_byte_data(unsigned char command, unsigned char value)
{
    union i2c_smbus_data data;
    data.byte = value;
    return i2c_smbus_access(I2C_SMBUS_WRITE,command,
                            I2C_SMBUS_BYTE_DATA, &data);
}

inline int IBI2CControl::i2c_smbus_read_word_data(unsigned char command)
{
    union i2c_smbus_data data;
    if (i2c_smbus_access(I2C_SMBUS_READ,command,
                         I2C_SMBUS_WORD_DATA,&data))
        return -1;
    else
        return 0x0FFFF & data.word;
}

inline int IBI2CControl::i2c_smbus_write_word_data(unsigned char command, unsigned short value)
{
    union i2c_smbus_data data;
    data.word = value;
    return i2c_smbus_access(I2C_SMBUS_WRITE,command,
                            I2C_SMBUS_WORD_DATA, &data);
}

inline int IBI2CControl::i2c_smbus_process_call(unsigned char command, unsigned short value)
{
    union i2c_smbus_data data;
    data.word = value;
    if (i2c_smbus_access(I2C_SMBUS_WRITE,command,
                         I2C_SMBUS_PROC_CALL,&data))
        return -1;
    else
        return 0x0FFFF & data.word;
}

/* Returns the number of read bytes */
inline int IBI2CControl::i2c_smbus_read_block_data(unsigned char command, unsigned char *values)
{
    union i2c_smbus_data data;
    int i;
    if (i2c_smbus_access(I2C_SMBUS_READ,command,
                         I2C_SMBUS_BLOCK_DATA,&data))
        return -1;
    else {
        for (i = 1; i <= data.block[0]; i++)
            values[i-1] = data.block[i];
        return data.block[0];
    }
}

inline int IBI2CControl::i2c_smbus_write_block_data(unsigned char command, unsigned char length, unsigned char *values)
{
    union i2c_smbus_data data;
    int i;
    if (length > I2C_SMBUS_BLOCK_MAX)
        length = I2C_SMBUS_BLOCK_MAX;
    for (i = 1; i <= length; i++)
        data.block[i] = values[i-1];
    data.block[0] = length;
    return i2c_smbus_access(I2C_SMBUS_WRITE,command,
                            I2C_SMBUS_BLOCK_DATA, &data);
}
 
/* Returns the number of read bytes */
inline int IBI2CControl::i2c_smbus_read_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values)
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

inline int IBI2CControl::i2c_smbus_write_i2c_block_data(unsigned char command, unsigned char length, unsigned char *values)
{
    union i2c_smbus_data data;
    int i;
    if (length > I2C_SMBUS_BLOCK_MAX)
        length = I2C_SMBUS_BLOCK_MAX;
    for (i = 1; i <= length; i++)
        data.block[i] = values[i-1];
    data.block[0] = length;
    return i2c_smbus_access(I2C_SMBUS_WRITE,command,
                            I2C_SMBUS_I2C_BLOCK_DATA, &data);
}


/* Returns the number of read bytes */
inline int IBI2CControl::i2c_smbus_block_process_call(unsigned char command, unsigned char length, unsigned char *values)
{
    union i2c_smbus_data data;
    int i;
    if (length > I2C_SMBUS_BLOCK_MAX)
        length = I2C_SMBUS_BLOCK_MAX;
    for (i = 1; i <= length; i++)
        data.block[i] = values[i-1];
    data.block[0] = length;
    if (i2c_smbus_access(I2C_SMBUS_WRITE,command,
                         I2C_SMBUS_BLOCK_PROC_CALL,&data))
        return -1;
    else {
        for (i = 1; i <= data.block[0]; i++)
            values[i-1] = data.block[i];
        return data.block[0];
    }
}

inline int IBI2CControl::i2cset_inner(unsigned char *values, unsigned char length)
{
    return i2cset(m_address, values, length);
}

inline int IBI2CControl::i2cset_inner(int daddress, unsigned char *values, unsigned char length)
{
    return i2cset(m_address, daddress, values, length);
}

inline int IBI2CControl::i2cset_inner(int daddress, int value, char b_w )
{
    return IBI2CControl::i2cset(m_address, daddress, value, b_w);
}

inline int IBI2CControl::i2cget_inner(int daddress,char b_w)
{
    return i2cget(m_address, daddress, b_w);
}

inline int IBI2CControl::i2cget_inner(unsigned char *values, unsigned char length)
{
    return i2cget(m_address, values, length);
}

inline int IBI2CControl::i2cget_inner(int daddress, unsigned char *values, unsigned char length)
{
    return i2cget(m_address, daddress, values, length);
}

inline int IBI2CControl::i2cget_inner()
{
    return i2cget(m_address);
}

#endif

