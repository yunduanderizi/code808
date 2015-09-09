/**********************************************************************
 Filename: ib_i2cctl.cpp
 Description: Defines the IBI2CControl class.
 Author: Xing Qianqian
**********************************************************************/

#include "ib_i2cctl.h"

IBI2CControl::IBI2CControl(int i2cbus) :
    m_i2cbus(i2cbus),
    m_file(-1),
    m_retry(1),
    m_sleep_interval(10000),
    m_address(0)
{
    if(i2copen() < 0)
    {
        fprintf(stderr, "IBI2CControl: Fail to open i2c device(%d).\r\n", m_i2cbus);
    }
}

IBI2CControl::~IBI2CControl()
{
    i2cclose();
}

int IBI2CControl::i2copen()
{
    char filename1[IB_MAX_I2C_NODE_LENGTH];
    char filename2[IB_MAX_I2C_NODE_LENGTH];
    char filename3[IB_MAX_I2C_NODE_LENGTH];
    sprintf(filename1,"/dev/i2c-%d",m_i2cbus);
    sprintf(filename2,"/dev/i2c%d",m_i2cbus);
    sprintf(filename3,"/dev/i2c/%d",m_i2cbus);
    if ((m_file = open(filename1,O_RDWR)) < 0)
    {
        if ((m_file = open(filename2,O_RDWR)) < 0)
        {
            if ((m_file = open(filename3,O_RDWR)) < 0)
            {
                return -1;
            }
        }
    }

    return 0;
}

void IBI2CControl::i2cclose()
{
    if(m_file > 0)
    {
        if(close(m_file) < 0)
        {
            fprintf(stderr, "IBI2CControl: Failed to close file handle.\r\n");
        }
    }
}

int IBI2CControl::i2cset(int address, unsigned char *values, unsigned char length)
{
    int ret;
    struct i2c_rdwr_ioctl_data queue;
    struct i2c_msg msg;
    int nTimes;
    if(m_file <= 0)
    {
        return -1;
    }

    msg.addr = address;
    msg.flags = 0;
    msg.buf = values;
    msg.len = length;
    queue.msgs = &msg;
    queue.nmsgs = 1;
    nTimes = m_retry;
    ret = -1;
    while( (ret<0) && (nTimes--) )
    {
        if((ret = ioctl(m_file,I2C_RDWR,&queue)) < 0)
        {
            usleep( m_sleep_interval );
        }
    }
    return ret;

}

int IBI2CControl::i2cset(int address, int daddress, int value, char b_w)
{
    int size,res;
    long funcs;
    int nTimes;
    
    if(m_file <= 0)
    {
        return -1;
    }
    
    if(b_w == 'b')
    {
        size = I2C_SMBUS_BYTE_DATA;
    }
    else if(b_w == 'w')
    {
        size = I2C_SMBUS_WORD_DATA;
    }
    else
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
    
    if( (value<0) || ( (size==I2C_SMBUS_BYTE_DATA) && (value>0xff) ) || ( (size==I2C_SMBUS_WORD_DATA) && (value>0xffff) ) )
    {
        return -1;
    }

    /* check adapter functionality */
    if (ioctl(m_file,I2C_FUNCS,&funcs) < 0)
    {
        return -1;
    }
    
    switch(size)
    {
        case I2C_SMBUS_BYTE_DATA:
            if( ! (funcs & (I2C_FUNC_SMBUS_WRITE_BYTE_DATA | I2C_FUNC_SMBUS_READ_BYTE_DATA) ) )
            {
                return -1;
            }
            break;
        case I2C_SMBUS_WORD_DATA:
            if( !(funcs & (I2C_FUNC_SMBUS_WRITE_WORD_DATA | I2C_FUNC_SMBUS_READ_WORD_DATA) ) )
            {
                return -1;
            }
            break;
    }

    res = -1;
    nTimes = m_retry;
    while( (res<0) && (nTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ( (res = ioctl(m_file,I2C_SLAVE_FORCE,address)) < 0) 
        {
            usleep( m_sleep_interval );
        }
        else
        {
            if(size == I2C_SMBUS_WORD_DATA)
            {
                res = i2c_smbus_write_word_data(daddress, value);
            }
            else
            {
                res = i2c_smbus_write_byte_data(daddress, value);
            }
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }

    return res;
}

int IBI2CControl::i2cset(int address, int daddress, unsigned char *values, unsigned char length)
{
    int res;
    long funcs;
    int nTimes;
    
    if(m_file <= 0)
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
    
    if(length > I2C_SMBUS_BLOCK_MAX)
    {
        return -1;
    }
    
    /* check adapter functionality */
    if (ioctl(m_file,I2C_FUNCS,&funcs) < 0)
    {
        return -1;
    }

    if(!(funcs & I2C_FUNC_SMBUS_I2C_BLOCK))
    {
        return -1;
    }

    res = -1;
    nTimes = m_retry;
    while( (res<0) && (nTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ( (res = ioctl(m_file,I2C_SLAVE_FORCE,address)) < 0) 
        {
            usleep( m_sleep_interval );
        }
        else
        {
            res = i2c_smbus_write_i2c_block_data(daddress, length, values);
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }

    return res;
}

int IBI2CControl::i2cget(int address,int daddress,char b_w)
{
    int size,res;
    long funcs;
    int nTimes;

    if(m_file <= 0)
    {
        return -1;
    }
    
    if( b_w == 'b' )
    {
        size = I2C_SMBUS_BYTE_DATA;
    }
    else if( b_w == 'w')
    {
        size = I2C_SMBUS_WORD_DATA;
    }
    else
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
    if (ioctl(m_file,I2C_FUNCS,&funcs) < 0)
    {
        return -1;
    }

    switch(size)
    {
        case I2C_SMBUS_BYTE_DATA:
            if(!(funcs & (I2C_FUNC_SMBUS_WRITE_BYTE_DATA | I2C_FUNC_SMBUS_READ_BYTE_DATA)))
            {
                return -1;
            }
            break;
        case I2C_SMBUS_WORD_DATA:
            if(!(funcs & (I2C_FUNC_SMBUS_WRITE_WORD_DATA | I2C_FUNC_SMBUS_READ_WORD_DATA)))
            {
                return -1;
            }
            break;
    }
    
    res = -1;
    nTimes = m_retry;
    while( (res<0) && (nTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ((res = ioctl(m_file,I2C_SLAVE_FORCE,address)) < 0)
        {
            usleep( m_sleep_interval );
        }
        else
        {
            if(size == I2C_SMBUS_WORD_DATA)
            {
                res = i2c_smbus_read_word_data(daddress);
            }
            else
            {
                res = i2c_smbus_read_byte_data(daddress);
            }
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return res;
}

int IBI2CControl::i2cget(int address, unsigned char *values, unsigned char length)
{
    int ret;
    struct i2c_rdwr_ioctl_data queue;
    struct i2c_msg msg;
    int nTimes;

    if(m_file <= 0)
    {
        return -1;
    }
    
    msg.addr = address;
    msg.flags = 1;
    msg.buf = values;
    msg.len = length;
    queue.msgs = &msg;
    queue.nmsgs = 1;
    
    nTimes = m_retry;
    ret = -1;
    while( (ret<0) && (nTimes--) )
    {
        if((ret = ioctl(m_file,I2C_RDWR,&queue)) < 0)
        {
            usleep( m_sleep_interval );
        }
    }

    return ret;
}

int IBI2CControl::i2cget(int address, int daddress, unsigned char *values, unsigned char length)
{
    int size,res;
    long funcs;
    int nTimes;

    if(m_file <= 0)
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
    if (ioctl(m_file,I2C_FUNCS,&funcs) < 0)
    {
        return -1;
    }

    if(!(funcs & I2C_FUNC_SMBUS_I2C_BLOCK))
    {
        return -1;
    }
    
    res = -1;
    nTimes = m_retry;
    while( (res<0) && (nTimes--) )
    {
        /* use FORCE so that we can write registers even when a driver is also running */
        if ((res = ioctl(m_file,I2C_SLAVE_FORCE,address)) < 0)
        {
            usleep( m_sleep_interval );
        }
        else
        {
            res = i2c_smbus_read_i2c_block_data(daddress, length, values);
            if(res<0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return res;
}

int IBI2CControl::i2cget(int address)
{
    int nTimes;
    int ret;

    if(m_file <= 0)
    {
        return -1;
    }

    nTimes = m_retry;
    ret = -1;
    while( (ret<0) && (nTimes--) )
    {
        if((ret = ioctl(m_file,I2C_SLAVE_FORCE,address)) < 0)
        {
            usleep( m_sleep_interval );
        }
        else
        {
            if((ret = i2c_smbus_read_byte()) < 0)
            {
                usleep( m_sleep_interval );
            }
        }
    }
    return ret;
}
