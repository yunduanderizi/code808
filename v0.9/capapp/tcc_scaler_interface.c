#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/poll.h>
#include "tcc_scaler_interface.h"

/*
#############################################################################################
							SCALER INTERFACE FOR VIDEO
#############################################################################################
*/

/*****************************************************************************
* Function Name : 
******************************************************************************
* Desription  : tcc_video_scaler_open
* Parameter   : dev_name:device driver name 
* Return	  : error:-1, others file descriptor
******************************************************************************/
int tcc_video_scaler_open(const char* dev_name)
{
	int fd = -1;

	if(dev_name != NULL)
	{
		fd = open(dev_name, O_RDWR | O_NDELAY);
		if(fd >= 0)
		{
			printf("%s open succeed\n", dev_name);
		}
		else
			printf("can't open %s\n", dev_name);
	}

	return fd;
}

/*****************************************************************************
* Function Name : 
******************************************************************************
* Desription  : tcc_video_scaler_execute
* Parameter   : fd:file descriptor

				src: addr_y, addr_u, addr_v is src address 
					 x,y,w,h is input image size
					 w_x, w_y, w_w, w_h is window size (it can use for zoom-in)

				dst: 
					addr_y, addr_u, addr_v is dest address 
					x,y,w,h is destination size
				
* Return	  : error:-1, others succeed
******************************************************************************/
int tcc_video_scaler_execute(const int fd, tcc_scaler_info_t* src, tcc_scaler_info_t* dst)
{
	int ret;
	struct pollfd poll_event[1];
	SCALER_TYPE scaler_info;

	if(fd < 0)
		return -1;
		
	scaler_info.responsetype 		= SCALER_INTERRUPT; 
	
	scaler_info.src_Yaddr			= src->addr_y;
	scaler_info.src_Uaddr			= src->addr_u;
	scaler_info.src_Vaddr			= src->addr_v;
	scaler_info.src_fmt				= src->format;
	scaler_info.src_ImgWidth 		= src->w;		
	scaler_info.src_ImgHeight		= src->h;	
	
	scaler_info.src_winLeft			= src->w_x;
	scaler_info.src_winTop			= src->w_y;
	scaler_info.src_winRight 		= src->w_w;		
	scaler_info.src_winBottom		= src->w_h;		

	scaler_info.dest_Yaddr			= dst->addr_y;
	scaler_info.dest_Uaddr			= dst->addr_u;
	scaler_info.dest_Vaddr			= dst->addr_v;
	
	scaler_info.dest_fmt 			= dst->format; 	
	scaler_info.dest_ImgWidth		= dst->w;
	scaler_info.dest_ImgHeight		= dst->h;
	
	scaler_info.dest_winLeft 		= dst->x;
	scaler_info.dest_winTop			= dst->y;
 	scaler_info.dest_winRight		= dst->w;
	scaler_info.dest_winBottom		= dst->h;
	
  	scaler_info.viqe_onthefly = 0;
  	scaler_info.interlaced = 0;
  
	if(ioctl(fd, TCC_SCALER_IOCTRL, &scaler_info) != 0)
	{
		printf("scaler error\n");
		close(fd);
		return -1;
	}

	if(scaler_info.responsetype == SCALER_INTERRUPT)
	{
		memset(poll_event, 0, sizeof(poll_event));
		poll_event[0].fd = fd;
		poll_event[0].events = POLLIN;
		ret = poll((struct pollfd*)poll_event, 1, 400);

		if(ret < 0) 
		{
			printf("m2m poll error %d\n", __LINE__);
			return -1;
		}
		else if(ret == 0) 
		{
			printf("m2m poll error %d\n", __LINE__);
			return -1;
		}
		else if(ret > 0) 
		{
			if (poll_event[0].revents & POLLERR) 
			{
				printf("m2m poll error %d\n", __LINE__);
				return -1;
			}
		}
	}

	return 0;
}

/*****************************************************************************
* Function Name : 
******************************************************************************
* Desription  : tcc_video_scaler_close
* Parameter   : fd:file descriptor
* Return	  : error:-1, others succeed
******************************************************************************/
int tcc_video_scaler_close(const int fd)
{
	if(fd >= 0)
	{
		close(fd);
		printf("video scaler close succeed\n");
		return 0;	
	}
	else
		printf("video scaler close error\n");
		
	return -1;
}


