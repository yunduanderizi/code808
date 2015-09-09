#include "io.h"

#define PATH "/nand4/update/ASC1411.BIN"

u16 FREQ;
u8 LEN;
u8 ID;
u8 VER;
u8 STATUS;
u8 BADN;

int CheckSumFlag(int ,u8 *);
int read_status(void);
int asc1411_reset(void);
int asc1411_command(u8 , u8 *, u8 , u8 *);
void asc1411_upgrade_Req(void);
int asc1411_send_file_head(u8 ,u8 *);
int asc1411_send_file_data(u8 ,u8 *);
int update_process(void);
