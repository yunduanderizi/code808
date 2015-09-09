#include "io.h"
#include "ib_i2cctl.h"
#define PATH_BIN "/nand4/update/ER_IROM1.IROM1"
#define PATH_HEX "Innov_Lpc1765.hex"
#define READ_BIN
#define NORMAL 
u8 Request_ID;
u8 Date_ID1;
u8 Date_ID2;
u8 Date_ID3;

u8 MSB;
u8 LSB;

u8 CheckSumFlag(int ,u8 *);
int read_status(int);
int read_status32();
int mcu_reset(void);
int mcu_command(int , u8 *, int , u8 *);


int update_process_read_bin(void);
int update_process_read_hex(void);
int mcu_update_Request(void);
int mcu_send_data(int,u8 *);
int mcu_send_end_data(int,u8 *);
u8 *data_packag(int ,u8 *);

u8 GetByte_hex(const char *);
u8 GetByte_dec(const char *);
int Get_Total_Line(void);
int Get_Data(i8 *, int);
int choice_read_mode(void);
