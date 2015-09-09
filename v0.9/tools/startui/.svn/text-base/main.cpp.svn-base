#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <errno.h>
#include <linux/i2c.h>  
#include <linux/i2c-dev.h>  

#define DEV_NAME	"/dev/i2c-0"
#define MCU_ADDR 	0x45
#define SLAVE_ADDR	0x01

#ifdef LARGE_PICTURE
#define	FB_SIZE	(800*480*4)
#define	FB_PIX_SIZE	(800*480)
unsigned int topPicture[800][480];
#else
#define	FB_SIZE	(320*240*4)
#define	FB_PIX_SIZE	(320*240)
unsigned int topPicture[320][240];
#endif

static int pixelBits = 16;

unsigned int *setFrameBufferArgAndMap(int fd)
{
	struct fb_var_screeninfo vinfo;
	if (ioctl(fd,FBIOGET_VSCREENINFO,&vinfo)){
		printf("Error reading variable information\n");
		return NULL;
	}

    vinfo.bits_per_pixel = pixelBits;
    if(ioctl(fd,FBIOPUT_VSCREENINFO,&vinfo))
    {
        printf("Error put  variable information\n");
        return NULL;
    }

	struct fb_fix_screeninfo finfo;
	void *frame_buffer;
	if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Can't get FSCREENINFO: %s\n", strerror(errno));
		return NULL;
	}

	printf("size: %d %d\n", finfo.smem_len,800*480*4);
	if ((frame_buffer = (char *) mmap(0,finfo.smem_len, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0)) == (char *) -1) 
	{
		printf("Can't mmap /dev/fb0 %s\n", strerror(errno));
		return NULL;
	}
	return (unsigned int*)frame_buffer;
}

void unmapFrameBuffer(int fd,void *frameBuffer)
{
	struct fb_fix_screeninfo finfo;
	if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Can't get FSCREENINFO: %s\n", strerror(errno));
		return;
	}
	
	munmap((void*)frameBuffer,finfo.smem_len);
}

int i2c_read_reg(char *dev, unsigned char *buf, unsigned slave_address, unsigned reg_address, int len)  
{  
    struct i2c_rdwr_ioctl_data work_queue;  
    unsigned char w_val = reg_address;  
    int ret;  
  
    int fd = open(dev, O_RDWR);  
    if (!fd) {  
        printf("Error on opening the device file\n");  
        return 0;  
    }  
  
    work_queue.nmsgs = 2;  
    work_queue.msgs = (struct i2c_msg*)malloc(work_queue.nmsgs *sizeof(struct  i2c_msg));  
    if (!work_queue.msgs) {  
        printf("Memory alloc error\n");  
        close(fd);  
        return 0;  
    }  
  
    ioctl(fd, I2C_TIMEOUT, 2);  
    ioctl(fd, I2C_RETRIES, 1);  
  
    (work_queue.msgs[0]).len = 1;  
    (work_queue.msgs[0]).addr = slave_address;  
    (work_queue.msgs[0]).buf = &w_val;  
  
    (work_queue.msgs[1]).len = len;  
    (work_queue.msgs[1]).flags = I2C_M_RD;  
    (work_queue.msgs[1]).addr = slave_address;  
    (work_queue.msgs[1]).buf = buf;  
  
    ret = ioctl(fd, I2C_RDWR, (unsigned long) &work_queue);  
    if (ret < 0) {  
        printf("Error during I2C_RDWR ioctl with error code: %d\n", ret);  
        close(fd);  
        free(work_queue.msgs);  
        return 0;  
    } else {  
        //printf("read salve:%02x reg:%02x\n", slave_address, reg_address);  
        close(fd);  
        free(work_queue.msgs);  
        return len;  
    }  
}

int main(int argc,char *argv[])
{
    printf("Usage: %s path startNo endNo interval [fb0/fb1/fb2]  \n\n",argv[0]);
    printf("example: %s /application/pic 1 49 100 fb1  \n\n",argv[0]);
	if(argc<6)
	{
		printf("argument is too less.\n");
		return -1;
	}
    
    char fb_dev_name[20]="/dev/fb0";
	char *path=argv[1];
	int   startNo=atoi(argv[2]);
	int   endNo  =atoi(argv[3]);
	int   interval=atoi(argv[4]);
    
	if(argc>5)
	{
		if(strcmp(argv[5],"fb0")==0)
		{
			strcpy(fb_dev_name,"/dev/fb0");
		}
		else if(strcmp(argv[5],"fb1")==0)
		{
			strcpy(fb_dev_name,"/dev/fb1");
		}
		else if(strcmp(argv[5],"fb2")==0)
		{
			strcpy(fb_dev_name,"/dev/fb2");
		}
	}

	printf("path=%s startNo=%d endNo=%d interval=%d dev=%s\n\n",
        path,startNo,endNo,interval,fb_dev_name);

	int fd =open(fb_dev_name,O_RDWR);
	unsigned int *fdm =setFrameBufferArgAndMap(fd);
	memset((void*)topPicture,0,FB_SIZE);
	memcpy((void*)fdm,(void*)topPicture,FB_PIX_SIZE);
	char fileName[1000];
	while(startNo <= endNo)
	{
		sprintf(fileName,"%s/%d.bmp",path,startNo);
		printf("open file: %s\n",fileName);

		FILE *fp=fopen(fileName,"r");
		if(fp==NULL)
		{
			usleep(interval*1000);
			startNo++;
			printf("Warnning: open file error. fileName=%s\n",fileName);
			continue;
		}

        int size = 0;
		fseek(fp,54,SEEK_SET);
		if((size = fread((void*)topPicture,pixelBits/8,FB_PIX_SIZE,fp)) != FB_PIX_SIZE)
		{
			printf("startui: picture file: %s is too short. size:%d\n",fileName, size);
		}
        
		fclose(fp);
		memcpy((void*)fdm,(void*)topPicture,FB_PIX_SIZE*pixelBits/8);

		startNo++;
		usleep(interval*1000);
	}

    unsigned char rw_val;  
	int result = -1;

    i2c_read_reg(DEV_NAME, &rw_val, MCU_ADDR, SLAVE_ADDR, 1); 
    result = (rw_val >> 7)?0:1;
    printf("%x\n", result);
    if(result == 1)
        sprintf(fileName,"%s/50.bmp",path);
    else
        sprintf(fileName,"%s/49.bmp",path);
    
    printf("open file: %s\n",fileName);
    FILE *fp=fopen(fileName,"r");
    if(fp==NULL)
	{
		usleep(interval*1000);
		startNo++;
		printf("Warnning: open file error. fileName=%s\n",fileName);
		return -1;
	}
    
    fseek(fp,54,SEEK_SET);
    if(fread((void*)topPicture,pixelBits/8,FB_PIX_SIZE,fp) != FB_PIX_SIZE)
	{
		printf("startui: picture file: %s is too short.\n",fileName);
	}
    
    fclose(fp);
    memcpy((void*)fdm,(void*)topPicture,FB_PIX_SIZE*pixelBits/8);
	unmapFrameBuffer(fd,fdm);
	close(fd);

	return 0;
}

