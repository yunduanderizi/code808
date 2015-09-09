/******************************************************************************
* include 
******************************************************************************/
#include "tcc_v4l2_ioctl.h"


/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_ioctl(int fd,int request, void * arg)
{
        int ret;

        ret= ioctl (fd, request, arg);
        return ret;
}


/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_set_dual_camera
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_ioctl_set_dual_camera(int fd,int * on_off)
{
	int ret;
	ret = tcc_v4l2_ioctl(fd,VIDIOC_USER_SET_CAMINFO_TOBEOPEN,on_off);
	return ret;
}


/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_get_capability
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int tcc_v4l2_ioctl_get_capability(int fd,struct v4l2_capability *cap)
{
	int ret;
	ret = tcc_v4l2_ioctl(fd,VIDIOC_QUERYCAP,cap);
	return ret;
}




/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_get_format
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int tcc_v4l2_ioctl_get_format(int fd,struct v4l2_format *fmt)
{
	int ret;
	memset(fmt, 0,  sizeof(struct v4l2_format));
	fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = tcc_v4l2_ioctl(fd,VIDIOC_G_FMT,fmt);
	return ret;
}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_set_format
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int tcc_v4l2_ioctl_set_format(int fd,struct v4l2_format *fmt)
{
	int ret;
	fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = tcc_v4l2_ioctl(fd,VIDIOC_S_FMT,fmt);
	return ret;
}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_set_try_format
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int tcc_v4l2_ioctl_set_try_format(int fd,struct v4l2_format *fmt)
{
	int ret;
	fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = tcc_v4l2_ioctl(fd,VIDIOC_TRY_FMT,fmt);
	return ret;
}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_request_buffer
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int tcc_v4l2_ioctl_request_buffer(int fd,struct v4l2_requestbuffers *reqbuf)
{
	int ret;
	reqbuf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = tcc_v4l2_ioctl(fd,VIDIOC_REQBUFS,reqbuf);
	return ret;
}


/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_query_status_buffer
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int tcc_v4l2_ioctl_query_status_buffer(int fd,struct v4l2_buffer *buf)
{
	int ret;
	buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = tcc_v4l2_ioctl(fd,VIDIOC_QUERYBUF,buf);
	return ret;
}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_query_buffer
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int tcc_v4l2_ioctl_query_buffer(int fd,struct v4l2_buffer *buf)
{
	int ret;

	ret = tcc_v4l2_ioctl(fd,VIDIOC_QBUF,buf);
	return ret;
}
/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_do_query_buffer
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int tcc_v4l2_ioctl_do_query_buffer(int fd,struct v4l2_buffer *buf)
{
	int ret;

	memset(buf,0,sizeof(struct v4l2_buffer));
	buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf->memory= V4L2_MEMORY_MMAP;

	ret = tcc_v4l2_ioctl(fd,VIDIOC_DQBUF,buf);
	return ret;
}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_ioctl_stream_onoff
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

int tcc_v4l2_ioctl_stream_onoff(int fd,int on_off)
{
	int ret = -1;
	 enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	switch( on_off)
	{
		case CAMERA_STREAM_ON:
		ret = tcc_v4l2_ioctl(fd,VIDIOC_STREAMON,&type);
		break;

		case CAMERA_STREAM_OFF:
		ret = tcc_v4l2_ioctl(fd,VIDIOC_STREAMOFF,&type);
		break;
	}
	return ret;
}

