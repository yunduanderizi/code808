#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>

#define TCC_COMPOSITE_IOCTL_START		10
#define TCC_COMPOSITE_IOCTL_UPDATE  	20
#define TCC_COMPOSITE_IOCTL_END			30
#define TCC_COMPOSITE_IOCTL_PROCESS		40
#define TCC_LCDC_COMPOSITE_MODE_SET		0x0401

typedef struct
{
	unsigned char lcdc;
	unsigned char mode;
} TCC_COMPOSITE_START_TYPE;

typedef enum{
	LCDC_COMPOSITE_NONE_MODE,
	LCDC_COMPOSITE_UI_MODE,
	LCDC_COMPOSITE_VIDEO_ONLY_MODE,
	LCDC_COMPOSITE_OVERLAY_MODE,
	LCDC_COMPOSITE_MODE_MAX
}LCDC_COMPOSITE_MODE;

int main(int argc, char *argv[])
{
	int fb_fd, composite_fd;
	TCC_COMPOSITE_START_TYPE composite_start;

	printf("usage: cvbsd on ntsc/pal\n"
			"       cvbsd off\n");
	if(argc < 2)
		return -1;

	if ((fb_fd = open("/dev/fb0", O_RDWR)) < 0) {
		printf("ERROR ::  fb0 Open : fd[%d]\n", fb_fd);
		return -1;
	}

	if ((composite_fd = open("/dev/composite", O_RDWR)) < 0) {
		printf("ERROR ::  Composite Driver Open : fd[%d]\n", composite_fd);
		close(fb_fd);
		return -1;
	}


	if(!strcmp(argv[1], "on")){

		if(argv[2] != NULL && !strcmp(argv[2]), "pal")
			composite_start.mode = 1;
		else
			composite_start.mode = 0;//COMPOSITE_NTST_M;

		composite_start.lcdc = 1;//lcdc;
		if(ioctl(composite_fd, TCC_COMPOSITE_IOCTL_START, &composite_start) != 0)
		{
			printf("can't start composite mode");
			return -1;
		}
		int mode = LCDC_COMPOSITE_OVERLAY_MODE;
		if(ioctl(fb_fd, TCC_LCDC_COMPOSITE_MODE_SET, &mode) != 0)
		{
			printf("can't set composite mode");
			return -1;
		}
	}else if(!strcmp(argv[1], "off")){
	
		if(ioctl(composite_fd, TCC_COMPOSITE_IOCTL_END, NULL) != 0)
		{
			printf("can't set composite mode");
			return -1;
		}
	
	
	}
	close(composite_fd);
	close(fb_fd);

}
