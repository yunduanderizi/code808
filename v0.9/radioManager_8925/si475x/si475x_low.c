//-----------------------------------------------------------------------------
//
// si475x_low.c
//
// Contains the low level, hardware functions
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stddef.h>
#include "typedefs.h"
#include "commanddefs.h"
#include "io.h"
#include <pthread.h>
//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
unsigned char PoweredUp = 0;
u16 FREQ;
u8 LEN;
u8 ID;
u8 VER;
u8 STATUS;
u8 BADN;
pthread_mutex_t read_mutex;
int init_mutes = 1;
//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void wait_us(u16 us);
void wait_ms(u16 ms);

//-----------------------------------------------------------------------------
//data check //-----------------------------------------------------------------------------
int CheckSumFlag(int size,u8 *buff)
{
	int checksum=0;
	int sum=0;
	int i=0;
	for(i=0;i<size;i++)
	{
		sum+=(int)buff[i];
	}
	checksum=sum ^ 0xFF;
	return checksum;
}
#if 0
//-----------------------------------------------------------------------------
// Reset the Si475x and select the appropriate bus mode.
//-----------------------------------------------------------------------------
void si475x_reset(void)
{
    io2w_init();
    return;

}
#endif
void init_i2c()
{
	printf("+++++++++++coming init_i2c++++++++++++\n");
	io2w_init();
}

u16 get_freq()
{
	return FREQ;
}
//-----------------------------------------------------------------------------
// This command returns the status
//-----------------------------------------------------------------------------
u8 si475x_readStatus()
{
    int ret=0;
	if(init_mutes)
	{
		init_mutes=0;
		pthread_mutex_init(&read_mutex,NULL);
	}
	pthread_mutex_lock(&read_mutex);
	u8 status[16]={0};
	wait_ms(20);
    ret=io2w_read(8, status);
    //printf("%d %d %d %d %d %d %d %d %d \n",ret,status[0],status[1],status[2],status[3],status[4],status[5],status[6],status[7]);
//	printf("checksum is:%d\n",status[7]);
//	printf("checksum after is:%d\n",CheckSumFlag(7,status));
#if 0
	if(status[7]!=CheckSumFlag(7,status))
	{
		printf("receive date is faild!\n");
		return -1;
	}
#endif
    if((status[0] == 0xFF) && (status[1] == 0xFF) && (status[2] == 0xFF) && (status[3] == 0xFF) && (status[4] == 0xFF) && (status[5] == 0xFF))
    {
        pthread_mutex_unlock(&read_mutex);
        printf("Error : read radio Status !\n");
        return -1;
    }
	LEN = status[0];
	ID = status[1];
	VER = status[2];
	STATUS = status[3];
	BADN = status[4];
	FREQ = (u16)status[5]<<8 | (u16)status[6];
	if(STATUS==0x04)
	{
		printf("work error!\n");
		si475x_reset();
	}
	pthread_mutex_unlock(&read_mutex);
	return 0;
}

//-----------------------------------------------------------------------------
//Command that will wait for CTS before returning
//-----------------------------------------------------------------------------

void si475x_waitForCTS()
{
	u16 i=100; 

	// Loop until CTS is found or stop due to the counter running out.
	while (--i && !(si475x_readStatus() & CTS))
	{
		wait_us(1000);
	}

	// If the i is equal to 0 then something must have happened.
	// It is recommended that the controller do some type of error
	// handling in this case.
}

//-----------------------------------------------------------------------------
// Sends a command to the part and returns the reply bytes
//-----------------------------------------------------------------------------
int si475x_command(u8 cmd_size, u8 *cmd, u8 reply_size, u8 *reply)
{
	int res = -1;
	// It is always a good idea to check for cts prior to sending a command to
        // the part.
 // 	si475x_waitForCTS();
 	wait_ms(15);
	// Write the command to the part
        res = io2w_write(cmd_size, cmd);
	if(res  <0 )
		return res;

	// Wait for CTS after sending the command
//	si475x_waitForCTS();
	wait_ms(15);
	// If the calling function would like to have results then read them.
	if(reply_size)
	{
		res = io2w_read(reply_size, reply);
	}
	return res;
}
//-----------------------------------------------------------------------------
//Reset the Si475x and select the appropriate bus mode.
//-----------------------------------------------------------------------------
int si475x_reset()
{
	u8 cmd[3] = {0};
//	u8 rsp[3]={0};
	cmd[0] = 3;  //command len
	cmd[1] = 0x01;  //reset command ID
	cmd[2] = CheckSumFlag(2,cmd);
	
	//Invoke Command
	si475x_command(3,cmd,0,NULL);
	return 0;
}

//-----------------------------------------------------------------------------
// This command set frequency
//-----------------------------------------------------------------------------
void si475x_set_frequency(u8 am_or_fm ,u8 high ,u8 low)
{
	printf("+++++++++++++++ago+++++++++++\n");
	printf("am_or_fm is :%d\n",am_or_fm);
	u8 cmd[6] = {0};
	u8 rsp[6] = {0};
	cmd[0] = 6;  //command len
        cmd[1] = 0x02; // set frequency ID
	cmd[2] = am_or_fm;
	cmd[3] = high;
	cmd[4] = low;
	printf("++++++++++++++si475x_set_frequency+++++++++++\n");
	printf("%d %d %d \n",cmd[2],cmd[3],cmd[4]);
	cmd[5] = CheckSumFlag(5,cmd);
	si475x_command(6,cmd,0,NULL);	

}
//-----------------------------------------------------------------------------
// This command seek radio 
//----------------------------------------------------------------------------
u16 si475x_seek_radio(u8 up_or_down)
{
	u8 cmd[4] = {0};
	u8 rsp[8] = {0};
	u16 freq ;

	cmd[0] = 4;  //command len
	cmd[1] = 0x03; //seek radio ID
	cmd[2] = up_or_down;
	cmd[3] = CheckSumFlag(3,cmd);
	si475x_command(4,cmd,0,NULL);
#if 0
	status = rsp[3];
	if(status==0)
	{
		printf("resetting!\n");
	}
	if(status==1)
	{
		printf("tiao xie!\n");
	}
	if(status==0x02)
	{
		printf("playing!\n");
	}
	if(status==0x03)
	{
		printf("sousuo!\n");
	}
	if(status==0x04)
	{
		printf("error!\n");
	}
	if(status==0x05)
	{
		printf("wei sou dao !\n");
	}
	if(status==0x06)
	{
		printf("stop OK!\n");
		freq=(u16)rsp[5]<<8|(u16)rsp[6];
		FREQ=freq;
//		get_freq(freq);
	}
#endif 
	return 0;
}
//-----------------------------------------------------------------------------
//This command Configuration parameter 
//----------------------------------------------------------------------------
void si475x_configuration_parameter()
{
	u8 cmd[9] = {0};
	u8 rsp[8] = {0};

	cmd[0] = 9;  //command len
	cmd[1] = 0x04;  //Configuration parameter ID
	cmd[2] = 0xB9;  //Smeter default value
	cmd[3] = 0x0B;  //Detuning default value
	cmd[4] = 0x10;  //Mulitipath default value
	cmd[5] = 0x10;  //Adjacent channel defalut value
	cmd[6] = 0xB2;  //AM Smeter default value
	cmd[7] = 0x0B;  //AM Detuning default value
	cmd[8] = CheckSumFlag(8,cmd);
	si475x_command(9,cmd,8,rsp);
	printf("configue after status is:%d\n",rsp[3]);
}
//-----------------------------------------------------------------------------
//This command Upgrade Request 
//----------------------------------------------------------------------------
void si475x_upgrade_Req()
{
	u8 cmd[3] = {0};
	u8 rsp[3] = {0};

	cmd[0] = 3;  //command len
	cmd[1] = 0x81; //upgrade request id
	cmd[2] = CheckSumFlag(2,cmd);
	si475x_command(3,cmd,0,NULL);
}
//-----------------------------------------------------------------------------
//This command send file head
//----------------------------------------------------------------------------

void si475x_send_file_head()
{
	
}

//-----------------------------------------------------------------------------
//This command send file data
//----------------------------------------------------------------------------
void si475x_send_file_data()
{

}
//-----------------------------------------------------------------------------
//This command send file data
//----------------------------------------------------------------------------
int si475x_read_current_status(int size,unsigned char * buff)
{
	printf("+++++++++++++++++++coming in read++++++++++++++++");
	u8 len;
	u8 id;
	u8 status;
	u8 sta;
	u8 fm_or_am;
	u16 freq;

	u8 cmd[3] = {0};
	u8 rsp[8] = {0};

	cmd[0] = 3;   //command len
	cmd[1] = 0x10; //command id
	cmd[2] = CheckSumFlag(2,cmd);
	si475x_command(3,cmd,8,rsp);
	if(rsp[7]!=CheckSumFlag(7,rsp))
	{
		printf("read date error!");
		return -1;
	}
	else
		memcpy(buff,rsp,8);

	len = buff[0];
	id = buff[1];
	status = buff[2];
	if(status>0x01 && status<0xFE)
	{
		printf("Normal!\n");
	}
	else
		printf("update!\n");
	sta = buff[3];
	fm_or_am = buff[4];
	if(sta = 0x06){
		freq = ((u16)buff[5] << 8) | (u16)buff[6];
	}
	printf("++++++++++++++++over++++++++++++++++\n");
	return 0;

}
//-----------------------------------------------------------------------------
// Helper function that sends the GET_INT_STATUS command to the part
//
// Returns:
//   The status byte from the part.
//-----------------------------------------------------------------------------
u8 getIntStatus(void)
{
    u8 cmd[1] = {0};
    u8 rsp[1] = {0};

    // Put the ID for the command in the first byte.
    cmd[0] = GET_INT_STATUS;

    // Invoke the command
	si475x_command(1, cmd, 1, rsp);

	// Return the status
	return rsp[0];
}
#if 1
//-----------------------------------------------------------------------------
// Set the passed property number to the passed value.
//
// Inputs:
//      propNumber:  The number identifying the property to set
//      propValue:   The value of the property.
//-----------------------------------------------------------------------------
void si475x_set_property(u16 propNumber, u16 propValue)
{
     u8   cmd[6]={0};

    // Put the ID for the command in the first byte.
    cmd[0] = SET_PROPERTY;

	// Initialize the reserved section to 0
    cmd[1] = 0;

	// Put the property number in the third and fourth bytes.
    cmd[2] = (u8)(propNumber >> 8);
	cmd[3] = (u8)(propNumber & 0x00FF);

	// Put the property value in the fifth and sixth bytes.
    cmd[4] = (u8)(propValue >> 8);
    cmd[5] = (u8)(propValue & 0x00FF);

    // Invoke the command
	si475x_command(6, cmd, 0, NULL);
}
#endif

int si475x_getPartInformation(int size,char * buf)
{
    char revision;
    u8 partNumber;
    char partMajor;
    char partMinor;
    char partBuild;
    u8   romID;
    u8 rsp[9] = {0,0,0,0,0,0,0,0,0};
    u8   cmd[1]={0};

    // NOTE:  This routine can be called either when the part is in the boot loader
    //        mode or fully operational.

    // Put the ID for the command in the first byte.
    cmd[0] = PART_INFO;

    // Invoke the command
    si475x_command(1, cmd, 9, rsp);

    // Now take the result and put in the variables we have declared
    // Status is in the first element of the array so skip that.
    revision   = (char)rsp[1];
    partNumber = rsp[2];
    partMajor  = (char)rsp[3];
    partMinor  = (char)rsp[4];
    partBuild  = (char)rsp[5];
    romID      = rsp[8];
    printf("------>[%s]:[%d.%d.%d.%d.%d.%d] \n",__func__,revision,partNumber,partMajor,partMinor,partBuild,romID);
    if(0==revision &&
         0==partNumber  &&
            0==partMajor  &&
            0==partMinor  &&
            0==partBuild  &&
            0==romID)
        return -1;
    if(size>=9)
    {
        memcpy(buf,rsp,9);
    }
    else
    {
        memcpy(buf,rsp,size);
    }
    return 0;
}
