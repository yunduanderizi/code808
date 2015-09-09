#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct
{
         unsigned lcdc_num;
         unsigned layer_num;
}lcdc_layerctrl_params;

#define TCC_LCDC_LAYER_ENABLE                       0x0052
#define TCC_LCDC_LAYER_DISABLE                      0x0053

int onoff_fb(int onoff)
{

	int fd = open("/dev/fb0", O_RDWR);
	if(fd < 0)
	{
		printf("can not open fd");
		return -1;
	}

	lcdc_layerctrl_params lcdparm;
	lcdparm.lcdc_num = 0;
	lcdparm.layer_num = 0;


	if(!!onoff)
		ioctl(fd, TCC_LCDC_LAYER_ENABLE, &lcdparm);	
	else
		ioctl(fd, TCC_LCDC_LAYER_DISABLE, &lcdparm);

    close(fd);
		
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("error use!\n");
		return -1;
	}

	int onoff = atoi(argv[1]);

	onoff_fb(onoff);
}
