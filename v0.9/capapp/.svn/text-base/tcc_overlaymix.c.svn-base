#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/poll.h>
#include "tcc_overlaymix.h"
//#define DEBUG_MIXER 

int tcc_overlaymix_init(void)
{
	int olymix_fd;

	olymix_fd = open(TCC_GRAPHIC_DEV_NAME, O_RDWR | O_NDELAY);
	if (olymix_fd <= 0)
	{
#ifdef DEBUG_MIXER
		printf("can't open %s", TCC_GRAPHIC_DEV_NAME);
#endif
		return -1;
	}
	else
#ifdef DEBUG_MIXER
		printf("%s open ok\n", TCC_GRAPHIC_DEV_NAME);
#endif

	return olymix_fd;
}

void tcc_overlaymix_deinit(int olymix_fd)
{
	close(olymix_fd);
}

int tcc_overlaymix_rotate(unsigned int g2d_fd, unsigned int rotate_angle, 
									char* src_addr, int src_x, int src_y, int src_w, int src_h, 
									int src_fmt, int dst_fmt, char* dst_buffer)
{
	struct pollfd poll_event[1];
	G2D_COMMON_TYPE grp_arg;
	
	grp_arg.responsetype = G2D_INTERRUPT;
	
	grp_arg.crop_offx = 0;
	grp_arg.crop_offy = 0;
	grp_arg.crop_imgx = src_w;
	grp_arg.crop_imgy = src_h;
	
	grp_arg.src_imgx = src_w;
	grp_arg.src_imgy = src_h;
	
	grp_arg.srcfm.data_swap = 0;
	grp_arg.srcfm.format = src_fmt;
	grp_arg.srcfm.uv_order = 1;
	grp_arg.DefaultBuffer = 0;
	
	grp_arg.src0 = (unsigned int)src_addr;
	grp_arg.src1 = (unsigned int)GET_ADDR_YUV42X_spU(grp_arg.src0, grp_arg.src_imgx, grp_arg.src_imgy);
	grp_arg.src2 = (unsigned int)GET_ADDR_YUV420_spV(grp_arg.src1, grp_arg.src_imgx, grp_arg.src_imgy);

	/* target setting */
	grp_arg.tgtfm.data_swap = 0;
	grp_arg.tgtfm.format = dst_fmt;
	grp_arg.tgtfm.uv_order = 1;
	switch(rotate_angle)
	{
		case FLIP_VER:
			grp_arg.ch_mode = FLIP_VER;
			grp_arg.dst_off_x = src_x;
			grp_arg.dst_off_y = src_y;
			grp_arg.dst_imgx = src_w;
			grp_arg.dst_imgy = src_h;
			break;
		
		case FLIP_HOR:
			grp_arg.ch_mode = FLIP_HOR;
			grp_arg.dst_off_x = src_x;
			grp_arg.dst_off_y = src_y;
			grp_arg.dst_imgx = src_w;
			grp_arg.dst_imgy = src_h;
			break;
		
		case FLIP_HV:
			grp_arg.ch_mode = FLIP_HV;
			grp_arg.dst_off_x = src_x;
			grp_arg.dst_off_y = src_y;
			grp_arg.dst_imgx = src_w;
			grp_arg.dst_imgy = src_h;
			break;
		
		case ROTATE_90:
			grp_arg.ch_mode = ROTATE_90;
			grp_arg.dst_off_x = src_x;
			grp_arg.dst_off_y = src_y;
			grp_arg.dst_imgx = src_h;
			grp_arg.dst_imgy = src_w;
			break;
		
		case ROTATE_180:
			grp_arg.ch_mode = ROTATE_180;
			grp_arg.dst_off_x = src_x;
			grp_arg.dst_off_y = src_y;
			grp_arg.dst_imgx = src_w;
			grp_arg.dst_imgy = src_h;
			break;	
		
		case ROTATE_270:
			grp_arg.ch_mode = ROTATE_270;
			grp_arg.dst_off_x = src_x;
			grp_arg.dst_off_y = src_y;
			grp_arg.dst_imgx = src_h;
			grp_arg.dst_imgy = src_w;
			break;

		case NOOP:
		default:
			grp_arg.ch_mode = NOOP;
			grp_arg.dst_off_x = src_x;
			grp_arg.dst_off_y = src_y;
			grp_arg.dst_imgx = src_w;
			grp_arg.dst_imgy = src_h;
			break;
	}
	
	grp_arg.tgt0 = (unsigned int)(dst_buffer);
	grp_arg.tgt1 = (unsigned int)GET_ADDR_YUV42X_spU(grp_arg.tgt0, grp_arg.dst_imgx, grp_arg.dst_imgy);
	grp_arg.tgt2 = (unsigned int)GET_ADDR_YUV420_spV(grp_arg.tgt1, grp_arg.dst_imgx, grp_arg.dst_imgy);
	
#ifdef TCC_892X_INCLUDE
	grp_arg.parallel_ch_mode = 0;
#endif
	
	if(ioctl(g2d_fd, TCC_GRP_COMMON_IOCTRL, &grp_arg) != 0)
	{
		printf("[%d][%s] overlay mixer error\n", __LINE__, __FUNCTION__);
		return -1;
	}

	if(grp_arg.responsetype == G2D_INTERRUPT)
	{
		int ret;
		memset(poll_event, 0, sizeof(poll_event));
		poll_event[0].fd = g2d_fd;
		poll_event[0].events = POLLIN;
		ret = poll((struct pollfd*)poll_event, 1, 400);

		if(ret < 0) 
		{
			printf("g2d poll error\n");
			return -1;
		}
		else if(ret == 0) 
		{
			printf("g2d poll timeout\n");		
			return -1;
		}
		else if(ret > 0) 
		{
			if (poll_event[0].revents & POLLERR) {
				printf("g2d poll POLLERR\n");
				return -1;
			}
		}
	}

	return 0;
}

