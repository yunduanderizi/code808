#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/poll.h>
#include "tcc_overlay_interface.h"


/*****************************************************************************
* Function Name : tcc_overlay_open
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
int  tcc_overlay_open(char* framebuffer)
{
	int fb_dev_fd ;

	fb_dev_fd = open(framebuffer, O_RDWR);

	if (fb_dev_fd < 0)
	{
		printf("Cound not open file\n");
		return -1;
	}
	return fb_dev_fd;
}

/*****************************************************************************
* Function Name : tcc_overlay_close
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
int  tcc_overlay_close(int fb_dev_fd)
{
	if(fb_dev_fd==-1)
		return -1;

	close(fb_dev_fd);
	return 0;
}

int  tca_overlay_disable(int fb_dev_fd)
{
	//if(fb_dev_fd==-1)
	//	return -1;

	ioctl(fb_dev_fd, OVERLAY_SET_DISABLE, NULL);
	return 0;
}
/*****************************************************************************
* Function Name : tcc_overlay_get_resolution_fd
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void tcc_overlay_get_resolution_fd(int fbdev, unsigned int *width, unsigned int *height)
{
	overlay_config_t overlay_info;

	if(ioctl(fbdev, OVERLAY_GET_SCREEN_INFO, &overlay_info) == -1)
	{
		printf("overlay_get_info error\n");
	}

	*width =  overlay_info.width;
	*height = overlay_info.height;
}

/*****************************************************************************
* Function Name : tcc_overlay_get_resolution_dev
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
void tcc_overlay_get_resolution_dev(char* framebuffer, unsigned int *width, unsigned int *height)
{
	int fb_dev_fd ;
	overlay_config_t overlay_info;

	fb_dev_fd = open(framebuffer, O_RDWR);

	if (fb_dev_fd < 0)
	{
		printf("Cound not open file\n");
		return;
	}

	if(ioctl(fb_dev_fd, OVERLAY_GET_SCREEN_INFO, &overlay_info) == -1)
	{
		printf("overlay_get_info error\n");
	}

	*width =  overlay_info.width;
	*height = overlay_info.height;

	close(fb_dev_fd);
}


/*****************************************************************************
* Function Name : tcc_overlay_set_image_window
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
int tcc_overlay_set_image_window(char* framebuffer,int width,int height,int format)
{	
	int ret;
	int fb_dev_fd;
	overlay_config_t overlay_info;

	fb_dev_fd = open(framebuffer, O_RDWR);

	if (fb_dev_fd < 0)
	{
		printf("Cound not open file\n");
		return -1;
	}

	//overlay setting
	overlay_info.format = format;
	overlay_info.sx = 0;
	overlay_info.sy = 0;
	overlay_info.width = width;
	overlay_info.height = height;

	ret = ioctl(fb_dev_fd, OVERLAY_SET_CONFIGURE, &overlay_info);
	if(ret < 0)
		printf("overlay 0 configure error\n");
	else
		printf("overlay 0 configure succeed\n");

	close(fb_dev_fd);
	
	return 0;
}

/*****************************************************************************
* Function Name : tcc_overlay_set_image_window
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
int tcc_overlay_set_image_window_fd(int fbdev,int width,int height,int format)
{	
	int ret;
	overlay_config_t overlay_info;

	//overlay setting
	overlay_info.format = format;
	overlay_info.sx = 0;
	overlay_info.sy = 0;
	overlay_info.width = width;
	overlay_info.height = height;

	ret = ioctl(fbdev, OVERLAY_SET_CONFIGURE, &overlay_info);
	if(ret < 0)
		printf("overlay 0 configure error\n");
	else
		printf("overlay 0 configure succeed\n");

	
	return 0;
}


/*****************************************************************************
* Function Name : tcc_overlay_set_image_update
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
int tcc_overlay_set_video_update(unsigned int overlay_fd, overlay_info_t* src)
{
	unsigned int framebuffer_addr;
	overlay_config_t overlay_info;
	overlay_info.format = V4L2_PIX_FMT_YUV422P;
	overlay_info.sx = src->x;
	overlay_info.sy = src->y;
	overlay_info.width = src->w;
	overlay_info.height = src->h;
		
	//if(ioctl(overlay_fd, OVERLAY_SET_CONFIGURE, &overlay_info) < 0)
	if(ioctl(overlay_fd, OVERLAY_SET_POSITION, &overlay_info) < 0)
	{
		printf("ioctrl overlay configure error\n");
		return -1;
	}

	//display image
	framebuffer_addr = (unsigned int)src->addr_y;
	if(ioctl(overlay_fd, OVERLAY_QUEUE_BUFFER, &framebuffer_addr) < 0)
	{
		printf("ioctrl overlay error\n");
		return -1;
	}

	return 0;
}

/*****************************************************************************
* Function Name : tcc_overlay_set_camera_update
******************************************************************************
* Desription  : 
* Parameter   :
* Return      :
******************************************************************************/
int tcc_overlay_set_camera_update(unsigned int overlay_fd, overlay_info_t* src)
{
	overlay_config_t overlay_info;
	overlay_info.format = V4L2_PIX_FMT_YUV422P;
	overlay_info.sx = src->x;
	overlay_info.sy = src->y;
	overlay_info.width = src->w;
	overlay_info.height = src->h;
		
//	if(ioctl(overlay_fd, OVERLAY_SET_CONFIGURE, &overlay_info) < 0)
	if(ioctl(overlay_fd, OVERLAY_SET_POSITION, &overlay_info) < 0)
	{
		printf("ioctrl overlay configure error\n");
		return -1;
	}

	//display image
	if(ioctl(overlay_fd, OVERLAY_QUEUE_BUFFER, src->addr_y) < 0)
	{
		printf("ioctrl overlay error\n");
		return -1;
	}

	return 0;
}

