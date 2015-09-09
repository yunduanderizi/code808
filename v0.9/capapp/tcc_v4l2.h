/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_v4l2.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef	_TCC_V4L2_H__
#define	_TCC_V4L2_H__
/******************************************************************************
* include 
******************************************************************************/
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/poll.h>
#include <sys/time.h>

#include <sys/types.h>
#include "videodev2.h"
#include "tcc_v4l2_ioctl.h"
#include "pmap.h"
#include "queue.h"
//#include "tcc_debug_levels.h"

#define TCC_DEB_LEV_ERR      1
#define TCC_DEB_LEV_LOG      2
#define TCC_DEB_ALL_MESS   255
#define TCC_DEBUG_LEVEL (TCC_DEB_ALL_MESS ) 

#if TCC_DEBUG_LEVEL > 0
#define TCC_DEBUG(n, args...) do { if (TCC_DEBUG_LEVEL & (n)){fprintf(stderr, args);} } while (0)
#else
#define DEBUG(n, args...) 
#endif




/******************************************************************************
* defines 
******************************************************************************/
#define CAMERA_WIDTH 688
#define CAMERA_HEIGHT 574

//	#define	TCC_V4L2_DEBUG_MESSAGE 

//	#define	TCC_V4L2_FILE_DUMP
#ifdef TCC_V4L2_FILE_DUMP
#define	TCC_V4L2_DUMP_FILE_NAME	"./v4l2_camera.yuv"		
#endif



//	#define	TCC_V4L2_DISPLAY
#ifdef TCC_V4L2_DISPLAY
#define	TCC_V4L2_DISPLAY_NAME	"/dev/overlay"	


#define OVERLAY_SET_CONFIGURE		50
#define TCC_LCDC_SET_ENABLE				0x0050
#define TCC_LCDC_SET_DISABLE			0x0051
#define OVERLAY_QUEUE_BUFFER		40
#endif


#define	CAMERA_DEVICE_NAME_0		"/dev/video0"				//Camera 0 Device Driver Name 
#define	CAMERA_DEVICE_NAME_1		"/dev/video1"			//Camera 1 Device Driver Name , current it is not support 

//#define	TCC_DUAL_CAMERA

#define	TCC_CAMERA_MAX_BUFFER_COUNT	8
#define	TCC_CAMERA_MIN_BUFFER_COUNT	2



//#define	TCC_CAMERA_POLL
#ifdef 	TCC_CAMERA_POLL
#define	TCC_CAMERA_POLL_TIME_OUT	10000			// msec
#endif


//#define	TCC_CAMERA_SELECT
#ifdef 	TCC_CAMERA_SELECT
#define	TCC_CAMERA_SELECT_TIME_OUT	1			// sec
#endif




/******************************************************************************
* typedefs & structure
******************************************************************************/
typedef enum
{	
	TCC_BACK_CAMERA_TYPE=0,
	TCC_FRONT_CAMERA_TYPE,
	TCC_CAMERA_TYPE_MAX
}TCC_CAMERA_TYPE;


typedef enum
{	
	TCC_FORMAT_NULL=0,
	TCC_FORMAT_YUV_420, //yuv420 saparate
	TCC_FORMAT_YUV_420IL0, //yuv420 chroma interleave, type 0
	TCC_FORMAT_YUV_420IL1, //yuv420 chroma interleave, type 1
	TCC_FORMAT_YUV_422, //yuv422 saparate
	TCC_FORMAT_YUV_422IL0, //yuv422 chroma interleave, type 0
	TCC_FORMAT_YUV_422IL1, //yuv422 chroma interleave, type 1
	TCC_FORMAT_MAX
}TCC_FORMAT_TYPE;



typedef struct tcc_camera_memory_struct
{
	unsigned int index;
	unsigned int size;
	unsigned int phy_addr;
	 char *virtl_addr;
	unsigned int phy_offset;
}tcc_cam_mem_t,*ptcc_cam_mem;



#ifdef TCC_V4L2_DISPLAY
typedef struct  tcc_v4l2_display_sturct
{
	unsigned int  sx;
	unsigned int  sy;
	unsigned int  width;
	unsigned int  height;
	unsigned int  format;
	unsigned int  transform;
} tcc_v4l2_display_t,*ptcc_v4l2_display;
#endif


struct pic_info{
	int m_channel;
    int m_command; 
    int m_interval;
    int m_flag;
    int m_resolution;
    int m_quality;
    int m_brightness;
    int m_saturation;
    int m_chroma;
	int m_captime;
	int m_event;
    int m_count;
    char timebuf[100];
    int data_len;
    char data[610*1024];
};

#define CAPTURE_FILE_PATH	"/nand2/innovdata/photo/"

typedef struct tcc_v4l2_camera_struct {
	int fp;			// Device Driver handler 
	TCC_CAMERA_TYPE type;
	TCC_FORMAT_TYPE format;
	unsigned int  width;
	unsigned int height;
	unsigned int frame_size;
	tcc_cam_mem_t mem[TCC_CAMERA_MAX_BUFFER_COUNT];
	unsigned int	mem_count;
	unsigned int	mem_alloc_count;
	pmap_t pmap;		// Camera Physical Memoy Information ( Base Address and Size)
	struct v4l2_capability		cap;
	struct v4l2_format			fmt;
	struct v4l2_requestbuffers	reqbuf;
	struct v4l2_buffer 			buf;
#ifdef TCC_V4L2_DEBUG_MESSAGE		
	struct v4l2_buffer 			previ_buf;
#endif
#ifdef 	TCC_CAMERA_POLL
	struct pollfd poll;
	int	poll_timeout;
#endif
#ifdef 	TCC_CAMERA_SELECT
	fd_set fds;
       struct timeval tv;
#endif

#ifdef TCC_V4L2_FILE_DUMP
//	int fp_dump;			
	FILE *fp_dump;			
#endif

#ifdef TCC_V4L2_DISPLAY
	int fp_fb0;	
	int fp_display;	
	tcc_v4l2_display_t display;
#endif

    queue 						Q;
    struct pic_info             picinfo;
    int                         video_mode;
    unsigned int 				resolution;
	unsigned long				cap_time;
	char 						name[1024];
    unsigned int			    picture_num;
    int                         fd_fb;
    int                         pic_no;

    /*初始化互斥锁*/
    pthread_mutex_t             pic_mutex;
    /*初始化条件变量*/
    pthread_cond_t              pic_cond;

    int                                                     preview_width;
    int                                                     preview_height;

    int     (* video_init)();
    int     (* video_type_change)();
    int     (* video_restart)();
}tcc_v4l2_camera,*ptcc_v4l2_camera;

typedef struct tcc_v4l2_camera_struct CameraDeviceT;



/******************************************************************************
* globals
******************************************************************************/


/******************************************************************************
* locals
******************************************************************************/

/******************************************************************************
* declarations
******************************************************************************/
int tcc_v4l2_open(char * devicename,tcc_v4l2_camera * camera);
int tcc_v4l2_close(tcc_v4l2_camera * camera);
int tcc_v4l2_get_physical_base_addr(tcc_v4l2_camera * camera);
int tcc_v4l2_get_capability(tcc_v4l2_camera * camera);
int tcc_v4l2_get_format(tcc_v4l2_camera * camera);
int tcc_v4l2_set_format(tcc_v4l2_camera * camera,TCC_FORMAT_TYPE format,int width, int height);
int tcc_v4l2_alloc_mem(tcc_v4l2_camera * camera);
int tcc_v4l2_free_mem(tcc_v4l2_camera * camera);
int tcc_v4l2_start_stream(tcc_v4l2_camera * camera);
int tcc_v4l2_end_stream(tcc_v4l2_camera * camera);
int tcc_v4l2_read_frame(tcc_v4l2_camera * camera);
#ifdef TCC_V4L2_FILE_DUMP
FILE * tcc_v4l2_dump_file_open(char * file_name);
int tcc_v4l2_dump_write(void *data,unsigned int size,FILE * fp );
int tcc_v4l2_dump_file_close(FILE * fp);
#endif

#ifdef TCC_V4L2_DISPLAY
int   tcc_v4l2_init_display(tcc_v4l2_camera * camera);
int   tcc_v4l2_display_camera(tcc_v4l2_camera * camera, unsigned int index );
int   tcc_v4l2_deinit_display(tcc_v4l2_camera * camera);
#endif


#endif //_TCC_V4L2_H__



