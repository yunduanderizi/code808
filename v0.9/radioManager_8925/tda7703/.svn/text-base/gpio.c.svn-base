#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h> 
#include <sys/mman.h>

#define GPA_BASE                        0x74200000
#define GPA_DAT							0x00
#define GPA_EN							0x04
#define OFFSET_PE                       0x1C
#define OFFSET_PS                       0x20
#define OFFSET_FUN0                     0x30
#define OFFSET_FUN1                     0x34
#define OFFSET_FUN2                     0x38
#define OFFSET_FUN3                     0x3C

#define GPION_TDA_RSTN 18
char *gpa_vbase, *gpe_vbase;
int fd;
int gpio_init()
{

	fd = open("/dev/mem",O_RDWR|O_SYNC);
	if(fd < 0){
		fprintf(stderr,"mem:open()\n");
		return -1;
	}
	gpa_vbase = (char *) mmap( NULL, 1024,  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, fd, GPA_BASE);
	if(gpa_vbase == NULL || gpa_vbase ==  MAP_FAILED ){
		fprintf(stderr,"mmap:map file error \n");
		return -1;
	}
	gpe_vbase = (gpa_vbase + 0x100);
	*(volatile unsigned int*)(gpe_vbase + OFFSET_FUN2) &= ~(0xf << 8); 
	*(volatile unsigned int*)(gpe_vbase + GPA_EN) |= (0x1 << GPION_TDA_RSTN);

	return 0;
}
void tda7703_RSTN_set(int high)
{
	if(high)
		*(volatile unsigned int*)(gpe_vbase + GPA_DAT) |= (0x1 << GPION_TDA_RSTN); 
	else
		*(volatile unsigned int*)(gpe_vbase + GPA_DAT) &= ~(0x1 << GPION_TDA_RSTN); 
}

//int main(int argc, char **argv)
//{
//
//	gpio_init();
//	if(argc > 2)
//		tda7703_RSTN_set(1);
//	else
//		tda7703_RSTN_set(0);
//
//
//
//}
