/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_v4l2_ioctl.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef	_TCC_V4L2_IOCTL_H__
#define	_TCC_V4L2_IOCTL_H__
/******************************************************************************
* include 
******************************************************************************/
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "videodev2.h"
#include "tcc_cam_ioctrl.h"



/******************************************************************************
* defines 
******************************************************************************/
#define	 CAMERA_STREAM_ON 	1
#define	 CAMERA_STREAM_OFF	0

/******************************************************************************
* typedefs & structure
******************************************************************************/


/******************************************************************************
* globals
******************************************************************************/


/******************************************************************************
* locals
******************************************************************************/

/******************************************************************************
* declarations
******************************************************************************/
int tcc_v4l2_ioctl(int fd,int request, void * arg);
int tcc_v4l2_ioctl_set_dual_camera(int fd,int * on_off);
int tcc_v4l2_ioctl_get_capability(int fd,struct v4l2_capability *crop);
int tcc_v4l2_ioctl_get_format(int fd,struct v4l2_format *fmt);
int tcc_v4l2_ioctl_set_format(int fd,struct v4l2_format *fmt);
int tcc_v4l2_ioctl_set_try_format(int fd,struct v4l2_format *fmt);
int tcc_v4l2_ioctl_request_buffer(int fd,struct v4l2_requestbuffers *reqbuf);
int tcc_v4l2_ioctl_query_status_buffer(int fd,struct v4l2_buffer *buf);
int tcc_v4l2_ioctl_query_buffer(int fd,struct v4l2_buffer *buf);
int tcc_v4l2_ioctl_do_query_buffer(int fd,struct v4l2_buffer *buf);
int tcc_v4l2_ioctl_stream_onoff(int fd,int on_off);



#endif //_TCC_V4L2_IOCTL_H__



