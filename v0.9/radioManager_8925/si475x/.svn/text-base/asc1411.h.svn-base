#ifndef _ASC1411_H_
#define _ASC1411_H_

#include "radioCommon.h"
#include <pthread.h>
//function prototype

void asc1411_configure(void);
void initFM(void);
void initAM(void);
int change_fm(u16 channel);
int change_am(u16 channel);
int seek(int up_or_down);
//int si475x_getPartInformation(int size,char *buf);
int si475x_read_current_status(int size, unsigned char *buf);
u16 get_freq();
int read_status();
int Reset();
void init_i2c();
int realtime_readstatus();
extern u16 FREQ;
extern u8 LEN;
extern u8 ID;
extern u8 VER;
extern u8 STATUS;
extern u8 BADN;
#endif
