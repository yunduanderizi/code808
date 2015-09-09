/****************************************************************************
 *   FileName    : tcc_overlay_interface.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
#ifndef __TCC_OVERLAY_INTERFACE_H_
#define __TCC_OVERLAY_INTERFACE_H_

#include "tcc_overlay_ioctl.h"
#include <linux/fb.h>
#include <linux/videodev2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TCC_OVERLAY0_DEV_NAME 		"/dev/overlay"
#define TCC_OVERLAY1_DEV_NAME 		"/dev/overlay1"

typedef struct tagoverlayInfo
{
	char* addr_y;
	char* addr_u;
	char* addr_v;
	unsigned long x;
	unsigned long y;
	unsigned long w;
	unsigned long h;
	int format;
}overlay_info_t;


int  tcc_overlay_open(char* framebuffer);
int  tcc_overlay_close(int fb_dev_fd);
int  tca_overlay_disable(int fb_dev_fd);
void tcc_overlay_get_resolution_fd(int fbdev, unsigned int *width, unsigned int *height);
void tcc_overlay_get_resolution_dev(char* framebuffer , unsigned int *width, unsigned int *height);
int tcc_overlay_set_image_window(char* framebuffer,int width,int height,int format);
int tcc_overlay_set_image_window_fd(int fbdev,int width,int height,int format);
void tcc_overlay_window_position_update(int handle,int ImageWidth,int ImageHeight,int FrameWidth, int FrameHeight);
void tcc_overlay_set_image_update(int handle,void *physical_addr,int ImageWidth,int ImageHeight,int FrameWidth, int FrameHeight,int position);
int tcc_overlay_set_video_update(unsigned int overlay_fd, overlay_info_t* src);

#endif /* __TCC_OVERLAY_INTERFACE_H_ */
