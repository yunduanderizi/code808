/******************************************************************************
* include 
******************************************************************************/
#include "tcc_v4l2.h"

//#include "tcc_camera_common.h"
//#include "tcc_fb_interface.h"
//#include "tcc_overlay_interface.h"
//#include "tcc_time.h"

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_open
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_open(char * devicename,tcc_v4l2_camera * camera)
{
	struct stat st; 

	memset(camera,0,sizeof(tcc_v4l2_camera));


        if (-1 == stat (devicename, &st)) 
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR, "Cannot identify %s\n",devicename);
		return camera->fp;
        }

        if (!S_ISCHR (st.st_mode)) 
	{
              TCC_DEBUG (TCC_DEB_LEV_ERR,"%s is no device\n", devicename);
		return camera->fp;
       }
#ifdef TCC_CAMERA_START_PROFILE
	TCC_Add_Profile_Time(&TCC_Profile,"camera driver open start ");
#endif	

        camera->fp = open (devicename, O_RDWR);

#ifdef TCC_CAMERA_START_PROFILE
	TCC_Add_Profile_Time(&TCC_Profile,"camera driver open end  ");
#endif

    if ( camera->fp < 0) 
	{
              TCC_DEBUG (TCC_DEB_LEV_ERR,"Cannot open : %s\n",devicename);
		return camera->fp;
        }

	// Set Camera Type 
	camera->type=TCC_BACK_CAMERA_TYPE;



#ifdef 	TCC_CAMERA_POLL
	memset(&camera->poll,0,sizeof(struct pollfd));
	camera->poll_timeout =TCC_CAMERA_POLL_TIME_OUT;
	camera->poll.fd = camera->fp;		
	camera->poll.events = POLLIN | POLLPRI;
	camera->poll.revents = 0; 
#endif


#ifdef TCC_CAMERA_SELECT
	FD_ZERO (&camera->fds);
       FD_SET (camera->fp, &camera->fds);

       camera->tv.tv_sec = TCC_CAMERA_SELECT_TIME_OUT;
       camera->tv.tv_usec = 0;
	   
#endif
	

#ifdef TCC_DUAL_CAMERA
	tcc_v4l2_ioctl_set_dual_camera(camera->fp,&camera->type); 
#endif 		


#ifdef TCC_V4L2_FILE_DUMP
	camera->fp_dump=tcc_v4l2_dump_file_open(TCC_V4L2_DUMP_FILE_NAME);
        if ( camera->fp_dump == NULL) 
	{
              TCC_DEBUG (TCC_DEB_LEV_ERR,"Cannot open dump file : %s\n",TCC_V4L2_DUMP_FILE_NAME);

        }
#endif 		



	return camera->fp;
	
}


/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_close
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_close(tcc_v4l2_camera * camera)
{
	int ret=-1;

	if( camera->fp >= 0) 
	{
		ret=	close(camera->fp);
		if( ret == -1 )
			 TCC_DEBUG (TCC_DEB_LEV_ERR,"Cannot close camera driver \n");
			
		memset(camera,0,sizeof(tcc_v4l2_camera));
	}

#ifdef TCC_V4L2_FILE_DUMP
       if( camera->fp_dump != NULL) 
	{
		ret=	tcc_v4l2_dump_file_close(camera->fp_dump);
		if( ret == -1 )
			 TCC_DEBUG (TCC_DEB_LEV_ERR,"Cannot close dump file\n");
			
	}
#endif 	
	
//	memset(camera,0,sizeof(tcc_v4l2_camera));


	return ret ; 

}


/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_open
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_get_physical_base_addr(tcc_v4l2_camera * camera)
{
//	int ret =-1;
	pmap_get_info("camera",&camera->pmap);

#ifdef TCC_V4L2_DEBUG_MESSAGE	
	TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->pmap.base: 0x%x\n",camera->pmap.base);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->pmap.size: 0x%x[%d]\n",camera->pmap.size,camera->pmap.size);
#endif

	if( camera->pmap.size> 0 )
		return  camera->pmap.size;
	else
		return -1;

	
}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_read
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_get_capability(tcc_v4l2_camera * camera)
{

	int ret=-1;
	ret =tcc_v4l2_ioctl_get_capability(camera->fp, &camera->cap);
	if( ret == -1 )
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," this driver is no V4L2 device \n");
		return ret;
	}


#ifdef TCC_V4L2_DEBUG_MESSAGE	
	TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"Driver: %s\n",camera->cap.driver);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"Card: %s\n",camera->cap.card);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"Bus Info: %s\n",camera->cap.bus_info);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"Version: %d\n",camera->cap.version);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"Capabilities: 0x%x\n",camera->cap.capabilities);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"Reserved:  0x%x\n",camera->cap.reserved[0]);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"Reserved:  0x%x\n",camera->cap.reserved[1]);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"Reserved:  0x%x\n",camera->cap.reserved[2]);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"Reserved:  0x%x\n",camera->cap.reserved[3]);
#endif



        if (!(camera->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) 
	{
                TCC_DEBUG (TCC_DEB_LEV_ERR,"this is no video capture device\n");
		  return -1;
        }
	

	if (!(camera->cap.capabilities & V4L2_CAP_READWRITE)) 
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," this does not support read i/o\n");
	}	

	if (!(camera->cap.capabilities & V4L2_CAP_STREAMING)) 
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR,"this does not support streaming i/o\n");
		return -1;
	}

	return ret;
}


/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_get_format
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_get_format(tcc_v4l2_camera * camera)
{

	int ret=-1;
	ret =tcc_v4l2_ioctl_get_format(camera->fp, &camera->fmt);
	if( ret == -1 )
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," this driver is no V4L2 device or can not support get format function  \n");
		return ret;
	}

#ifdef TCC_V4L2_DEBUG_MESSAGE	
	TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"type: 		%d\n",camera->fmt.type);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"width: 		%d\n",camera->fmt.fmt.pix.width);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"height: 		%d\n",camera->fmt.fmt.pix.height);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"pixelformat: 	0x%x\n",camera->fmt.fmt.pix.pixelformat);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"pixelformat: 	[%c][%c][%c][%c]\n",(camera->fmt.fmt.pix.pixelformat>>24 & 0x000000ff),(camera->fmt.fmt.pix.pixelformat>>16 & 0x000000ff),(camera->fmt.fmt.pix.pixelformat>>8 & 0x000000ff),(camera->fmt.fmt.pix.pixelformat & 0x000000ff));
	TCC_DEBUG (TCC_DEB_LEV_LOG,"field: 		%d\n",camera->fmt.fmt.pix.field);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"bytesperline: 	%d\n",camera->fmt.fmt.pix.bytesperline);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"sizeimage:  	%d\n",camera->fmt.fmt.pix.sizeimage);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"colorspace:  	%d\n",camera->fmt.fmt.pix.colorspace);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"priv:  		0x%x\n",camera->fmt.fmt.pix.priv);
#endif





	return ret;
}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_get_format
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_set_format(tcc_v4l2_camera * camera,TCC_FORMAT_TYPE format,int width, int height)
{

	int ret=-1;
#if 0
	struct v4l2_format			fmt;

	memset(&fmt,0,sizeof(struct v4l2_format));	 

	if ( format != NULL )
	{
		switch( format )
		{
			case TCC_FORMAT_YUV_420: 
				fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YVU420;
			break;
			case TCC_FORMAT_YUV_422 :
				fmt.fmt.pix.pixelformat =V4L2_PIX_FMT_YUYV ;
			break;
				
			default 
					TCC_DEBUG (TCC_DEB_LEV_ERR," this color format is not support  \n");
				return ret 
				break; 
		}		
		ret =tcc_v4l2_ioctl_set_try_format(camera->fp, &fmt);

		if( ret != -1) 
		{
			camera->fmt.fmt.pix.pixelformat= fmt.fmt.pix.pixelformat; 
			tcc_v4l2_ioctl_set_format(camera->fp, &camera->fmt);
		}
		else
		{
			TCC_DEBUG (TCC_DEB_LEV_ERR," this color format is not support  \n");
		}
		
	}	


#endif
		camera->format = format;
		camera->width    = width;
		camera->height   = height;

#ifdef TCC_V4L2_DEBUG_MESSAGE		
	TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->format : 		%d\n",camera->format );
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->width : 		%d\n",camera->width );
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->height: 		%d\n",camera->height);
#endif

		
		
		switch( camera->format  )
		{
			case TCC_FORMAT_YUV_420: 
				
				camera->fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YVU420;
				camera->fmt.fmt.pix.field       	= V4L2_FIELD_INTERLACED;
				camera->fmt.fmt.pix.width = width;
				camera->fmt.fmt.pix.height = height;

				camera->fmt.fmt.pix.sizeimage 	= (width * height * 3) / 2;
		
				break;

			case TCC_FORMAT_YUV_420IL0:
				camera->fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV12;
				camera->fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
				camera->fmt.fmt.pix.width = width;
				camera->fmt.fmt.pix.height = height;
				camera->fmt.fmt.pix.sizeimage 	= (width * height * 3) / 2;
			break;
			
			case TCC_FORMAT_YUV_420IL1:
				camera->fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV21;
				camera->fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
				camera->fmt.fmt.pix.width = width;
				camera->fmt.fmt.pix.height = height;
				camera->fmt.fmt.pix.sizeimage 	= (width * height * 3) / 2;
			break;
			
			case TCC_FORMAT_YUV_422 :
				camera->fmt.fmt.pix.pixelformat =V4L2_PIX_FMT_YUYV ;
				camera->fmt.fmt.pix.field       	= V4L2_FIELD_INTERLACED;
				camera->fmt.fmt.pix.width = width;
				camera->fmt.fmt.pix.height = height;
				camera->fmt.fmt.pix.sizeimage 	= width * height *2;
			break;		
			
#if 0
			case TCC_FORMAT_YUV_422IL0:
				camera->fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV16;
				camera->fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
				camera->fmt.fmt.pix.width = width;
				camera->fmt.fmt.pix.height = height;
				camera->fmt.fmt.pix.sizeimage = width * height *2;
			break;
				
			case TCC_FORMAT_YUV_422IL1:
				camera->fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV61;
				camera->fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
				camera->fmt.fmt.pix.width = width;
				camera->fmt.fmt.pix.height = height;
				camera->fmt.fmt.pix.sizeimage = width * height *2;
			break;
#endif
				
			default :
					TCC_DEBUG (TCC_DEB_LEV_ERR," this color format is not support  \n");
				return ret ;
		}		


#ifdef TCC_V4L2_DEBUG_MESSAGE		
	TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"type: 		%d\n",camera->fmt.type);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"width: 		%d\n",camera->fmt.fmt.pix.width);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"height: 		%d\n",camera->fmt.fmt.pix.height);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"pixelformat: 	0x%x\n",camera->fmt.fmt.pix.pixelformat);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"pixelformat: 	[%c][%c][%c][%c]\n",(camera->fmt.fmt.pix.pixelformat>>24 & 0x000000ff),(camera->fmt.fmt.pix.pixelformat>>16 & 0x000000ff),(camera->fmt.fmt.pix.pixelformat>>8 & 0x000000ff),(camera->fmt.fmt.pix.pixelformat & 0x000000ff));
	TCC_DEBUG (TCC_DEB_LEV_LOG,"field: 		%d\n",camera->fmt.fmt.pix.field);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"bytesperline: 	%d\n",camera->fmt.fmt.pix.bytesperline);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"sizeimage:  	%d\n",camera->fmt.fmt.pix.sizeimage);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"colorspace:  	%d\n",camera->fmt.fmt.pix.colorspace);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"priv:  		0x%x\n",camera->fmt.fmt.pix.priv);
#endif
		

		ret=tcc_v4l2_ioctl_set_format(camera->fp, &camera->fmt);
	
		if( ret == -1 )
		{
			TCC_DEBUG (TCC_DEB_LEV_ERR," this driver is no V4L2 device \n");
			return ret;
		}
		
//		tcc_v4l2_get_format(camera);

	return ret;
}


/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_alloc_mem
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_alloc_mem(tcc_v4l2_camera * camera)
{
	int ret=-1;
	unsigned int index=0;
	
	memset(&camera->reqbuf,0,sizeof(struct v4l2_requestbuffers));
	camera->reqbuf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	camera->reqbuf.memory=V4L2_MEMORY_MMAP;
	camera->reqbuf.count =TCC_CAMERA_MAX_BUFFER_COUNT;
	ret = tcc_v4l2_ioctl_request_buffer(camera->fp,&camera->reqbuf);
	if( ret == -1  )
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," this driver is not support memory map interface\n");
		return ret;
	}

	if(  camera->reqbuf.count < TCC_CAMERA_MIN_BUFFER_COUNT )
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," this driver is insufficient buffer memory[min count =%d] \n",TCC_CAMERA_MIN_BUFFER_COUNT);
		return ret;
	}

	camera->mem_count =camera->reqbuf.count;
	
#ifdef TCC_V4L2_DEBUG_MESSAGE		
	TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->reqbuf.type:%d\n",camera->reqbuf.type);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->reqbuf.memory:%d\n",camera->reqbuf.memory);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->reqbuf.count:%d\n",camera->reqbuf.count);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem_count:%d\n",camera->mem_count);
#endif


	for( index =0 ;  index < camera->mem_count ; index ++ )
	{
		memset(&camera->buf,0,sizeof( struct v4l2_buffer ));

		camera->buf.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
              camera->buf.memory      	= V4L2_MEMORY_MMAP;
              camera->buf.index       	= index;

#ifdef TCC_V4L2_DEBUG_MESSAGE		
	TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.type:%d\n",camera->buf.type);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.memory:%d\n",camera->buf.memory);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.index:%d\n",camera->buf.index);
#endif

			  
		ret = tcc_v4l2_ioctl_query_status_buffer(camera->fp,&camera->buf);
		if ( ret == -1 )
		{
			TCC_DEBUG (TCC_DEB_LEV_ERR," this driver is insufficient buffer memory[current count =%d] \n",index);
			return ret ;
		}
		camera->mem[index].index=camera->buf.index ;
		camera->mem[index].size =camera->buf.length;
		camera->mem[index].phy_offset=camera->buf.m.offset;
#if 1		
		camera->mem[index].phy_addr= camera->mem[index].phy_offset;
		camera->mem[index].virtl_addr=( char *)mmap (0 /* start anywhere */,camera->buf.length,PROT_READ | PROT_WRITE /* required */,MAP_SHARED /* recommended */,camera->fp, camera->buf.m.offset);
#else
		camera->mem[index].phy_addr=camera->pmap.base + camera->mem[index].phy_offset;
		camera->mem[index].virtl_addr=( char *)mmap (0 /* start anywhere */,camera->buf.length,PROT_READ | PROT_WRITE /* required */,MAP_SHARED /* recommended */,camera->fp,camera->mem[index].phy_addr);
#endif

		camera->mem_alloc_count ++;
#ifdef TCC_V4L2_DEBUG_MESSAGE		
	TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.type:%d\n",camera->buf.type);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.memory:%d\n",camera->buf.memory);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.index:%d\n",camera->buf.index);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.length:%d\n",camera->buf.length);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.m.offset:%d\n",camera->buf.m.offset);

	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].index:%d\n",index,camera->mem[index].index);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].size:%d\n",index,camera->mem[index].size);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].phy_offset:%d\n",index,camera->mem[index].phy_offset);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].phy_addr:0x%x\n",index,camera->mem[index].phy_addr);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].virtl_addr:0x%x\n",index,camera->mem[index].virtl_addr);

	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem_alloc_count :%d\n",camera->mem_alloc_count );

#endif
		
		
	}	


	return camera->mem_count ;
}
/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_free_mem
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_free_mem(tcc_v4l2_camera * camera)
{
	int ret =-1;
	unsigned int index=0;

	if(camera->fp < 0)
		return ret;

	for( index =0 ; index <	camera->mem_count ; index ++ )
	{
		ret = munmap (camera->mem[index].virtl_addr,camera->mem[index].size);

		if ( ret == -1)
		{
			TCC_DEBUG (TCC_DEB_LEV_ERR," %s : mem free error  \n",__FUNCTION__);
			return ret;
		}	
		memset(&camera->mem[index],0,sizeof(tcc_cam_mem_t));
		camera->mem_alloc_count --;

#ifdef TCC_V4L2_DEBUG_MESSAGE		
	TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].index:%d\n",index,camera->mem[index].index);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].size:%d\n",index,camera->mem[index].size);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].phy_offset:%d\n",index,camera->mem[index].phy_offset);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].phy_addr:%d\n",index,camera->mem[index].phy_addr);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].virtl_addr:%d\n",index,camera->mem[index].virtl_addr);
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem_alloc_count :%d\n",camera->mem_alloc_count );
#endif

		
	}	

	
	memset(&camera->reqbuf,0,sizeof(struct v4l2_requestbuffers));
	memset(&camera->buf,0,sizeof( struct v4l2_buffer ));

	return 0;
}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_start_stream
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_start_stream(tcc_v4l2_camera * camera)
{
	int ret=-1;
//	int index=0;


	camera->buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	camera->buf.memory      = V4L2_MEMORY_MMAP;

	for( camera->buf.index=0 ; camera->buf.index < camera->mem_alloc_count ;camera->buf.index ++ )
	{
		ret =tcc_v4l2_ioctl_query_buffer(camera->fp,&camera->buf);

		if ( ret == -1)
		{
			TCC_DEBUG (TCC_DEB_LEV_ERR," %s : start stream error  \n",__FUNCTION__);
			return ret;
		}
	}



	
	ret =tcc_v4l2_ioctl_stream_onoff(camera->fp,CAMERA_STREAM_ON);
	if ( ret == -1)
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," %s : start stream error  \n",__FUNCTION__);
		return ret;
	}
	return ret ;
	
	
}
/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_end_stream
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_end_stream(tcc_v4l2_camera * camera)
{
	int ret =-1;

	if(camera->fp < 0)
		return ret;
	
	ret =tcc_v4l2_ioctl_stream_onoff(camera->fp,CAMERA_STREAM_OFF);
	if ( ret == -1)
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," %s : end stream  error  \n",__FUNCTION__);
			return ret;
	}
	return ret;
}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_read_frame
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int   tcc_v4l2_read_frame(tcc_v4l2_camera * camera)
{
	int ret=-1;

#ifdef 	TCC_CAMERA_POLL
	ret = poll(&camera->poll, 1, camera->poll_timeout);
	
	if ((ret > 0) && (camera->poll.revents & (POLLIN | POLLPRI)))
#endif	

#ifdef TCC_CAMERA_SELECT

	  ret = select (camera->fp+ 1, &camera->fds, NULL, NULL, &camera->tv);

  	if (ret > 0 )

#endif
	{
			

		
		ret = tcc_v4l2_ioctl_do_query_buffer(camera->fp,&camera->buf);
		if ( ret == -1)
		{
			//TCC_DEBUG (TCC_DEB_LEV_ERR," %s : read fream  error  \n",__FUNCTION__);
			return ret;
		}


#ifdef TCC_V4L2_DEBUG_MESSAGE	
#if 1
		TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->mem[%d].virtl_addr=0x%x\n",camera->buf.index,camera->mem[camera->buf.index].virtl_addr);
	 	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.index=%d\n",camera->buf.index);
	 	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.bytesused=%d\n",camera->buf.bytesused);
	 	TCC_DEBUG (TCC_DEB_LEV_LOG,"ret=%d\n",ret);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.index:%d\n",camera->buf.index);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.type:%d\n",camera->buf.type);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.bytesused:%d\n",camera->buf.bytesused);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.flags:%d\n",camera->buf.flags);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.field:%d\n",camera->buf.field);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->previ_buf.timestamp.tv_sec:%s\n",ctime(&camera->previ_buf.timestamp.tv_sec));
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->previ_buf.timestamp.tv_sec:%d\n",camera->previ_buf.timestamp.tv_sec);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->previ_buf.timestamp.tv_usec:%d\n",camera->previ_buf.timestamp.tv_usec);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timestamp.tv_sec:%s\n",ctime(&camera->buf.timestamp.tv_sec));
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->previ_buf.timestamp.tv_sec:%d\n",camera->previ_buf.timestamp.tv_sec);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timestamp.tv_usec:%d\n",camera->buf.timestamp.tv_usec);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"frame interval %d[msec]\n",((camera->buf.timestamp.tv_sec*1000000+camera->buf.timestamp.tv_usec)- (camera->previ_buf.timestamp.tv_sec*1000000+camera->previ_buf.timestamp.tv_usec))/1000);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timestamp.tv_usec:%d\n",camera->buf.timestamp.tv_usec);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.flags:%d\n",camera->buf.timecode.flags);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.frames:%d\n",camera->buf.timecode.frames);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.hours:%d\n",camera->buf.timecode.hours);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.minutes:%d\n",camera->buf.timecode.minutes);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.seconds:%d\n",camera->buf.timecode.seconds);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.type:%d\n",camera->buf.timecode.type);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.userbits:%d\n",camera->buf.timecode.userbits);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.sequence:%d\n",camera->buf.sequence);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.memory:%d\n",camera->buf.memory);	
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.m.offset:%d\n",camera->buf.m.offset);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.length:%d\n",camera->buf.length);	
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.input:%d\n",camera->buf.input);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.reserved:%d\n",camera->buf.reserved);
#endif
	 	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.index=%d\n",camera->buf.index);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"frame interval %d[msec]\n",((camera->buf.timestamp.tv_sec*1000000+camera->buf.timestamp.tv_usec)- (camera->previ_buf.timestamp.tv_sec*1000000+camera->previ_buf.timestamp.tv_usec))/1000);
		memcpy(&camera->previ_buf,&camera->buf,sizeof( struct v4l2_buffer));
#endif

		
		return camera->buf.index;
	}
#ifdef 	TCC_CAMERA_POLL
	else
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," Camera Read Poll Time Out  \n");
		return -1;
	}
#endif	
#ifdef TCC_CAMERA_SELECT
	else
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," Camera Read Select Time Out  \n");
		return -1;
	}

#endif
}


/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_read_frame
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int   tcc_v4l2_return_frame(tcc_v4l2_camera * camera)
{
	int ret;

#ifdef TCC_V4L2_DEBUG_MESSAGE		
#if 1
		TCC_DEBUG (TCC_DEB_LEV_LOG,"---------------[%s]---------------\n",__FUNCTION__);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.index:%d\n",camera->buf.index);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.type:%d\n",camera->buf.type);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.bytesused:%d\n",camera->buf.bytesused);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.flags:%d\n",camera->buf.flags);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.field:%d\n",camera->buf.field);
		
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->previ_buf.timestamp.tv_sec:%s\n",ctime(&camera->previ_buf.timestamp.tv_sec));
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->previ_buf.timestamp.tv_sec:%d\n",camera->previ_buf.timestamp.tv_sec);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->previ_buf.timestamp.tv_usec:%d\n",camera->previ_buf.timestamp.tv_usec);

		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timestamp.tv_sec:%s\n",ctime(&camera->buf.timestamp.tv_sec));
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timestamp.tv_usec:%d\n",camera->buf.timestamp.tv_usec);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.flags:%d\n",camera->buf.timecode.flags);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.frames:%d\n",camera->buf.timecode.frames);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.hours:%d\n",camera->buf.timecode.hours);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.minutes:%d\n",camera->buf.timecode.minutes);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.seconds:%d\n",camera->buf.timecode.seconds);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.type:%d\n",camera->buf.timecode.type);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.timecode.userbits:%d\n",camera->buf.timecode.userbits);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.sequence:%d\n",camera->buf.sequence);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.memory:%d\n",camera->buf.memory);	
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.m.offset:%d\n",camera->buf.m.offset);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.length:%d\n",camera->buf.length);	
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.input:%d\n",camera->buf.input);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.reserved:%d\n",camera->buf.reserved);
#endif		
#endif

		ret =tcc_v4l2_ioctl_query_buffer(camera->fp,&camera->buf);
		return ret;

}


#ifdef TCC_V4L2_FILE_DUMP

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_dump_file_open
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
FILE * tcc_v4l2_dump_file_open(char * file_name)
{
	FILE *fp;

	fp=fopen(file_name, "wb");

        if ( fp== NULL) 
	{
              TCC_DEBUG (TCC_DEB_LEV_ERR,"Cannot open dump file : %s\n",file_name);
        }

	return fp;

}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_dump_write
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_dump_write(void *data,unsigned int size,FILE * fp )
{
	int ret=-1;
       if( fp != NULL) 
	{
		ret=	fwrite(data, 1, size, fp);
		fflush(fp);
		if( ret == -1 )
			 TCC_DEBUG (TCC_DEB_LEV_ERR,"Cannot write  dump file\n");
	}
	return ret ; 
}
/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_dump_file_close
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int tcc_v4l2_dump_file_close(FILE * fp)
{
	int ret=-1;
       if( fp != NULL) 
	{
		ret=	fclose(fp);
		if( ret == -1 )
			 TCC_DEBUG (TCC_DEB_LEV_ERR,"Cannot close dump file\n");
			
	}

	return ret ; 

}
#endif

#ifdef TCC_V4L2_DISPLAY

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_init_display
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int   tcc_v4l2_init_display(tcc_v4l2_camera * camera)
{
	int ret ;

	static struct fb_fix_screeninfo fb_fix;
	static struct fb_var_screeninfo fb_info;

	camera->fp_fb0 = tcc_fb_open(TCC_FB0_DEV_NAME);
	if(camera->fp_fb0 == -1 )
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," Overlay Driver Open : fd[%s]\n", TCC_FB0_DEV_NAME);
		return -1;
    	}

	camera->fp_display= tcc_overlay_open(TCC_V4L2_DISPLAY_NAME);
	if(camera->fp_display == -1 )
	{
		TCC_DEBUG (TCC_DEB_LEV_ERR," Overlay Driver Open : fd[%s]\n", TCC_V4L2_DISPLAY_NAME);
		return -1;
    	}


	tcc_fb_get_fscreen_info(camera->fp_fb0,&fb_fix);
    tcc_fb_get_vscreen_info(camera->fp_fb0,&fb_info);

	camera->display.sx =  0;
	camera->display.sy = 0;
	camera->display.width =   fb_info.xres;
	camera->display.height = fb_info.yres;
	camera->display.format = camera->fmt.fmt.pix.pixelformat;

	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->display.sx =%d\n", camera->display.sx  );
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->display.sy=%d\n", camera->display.sy );
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->display.width=%d\n", camera->display.width );
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->display.height=%d\n", camera->display.height );
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->display.format=%d\n", camera->display.format );
	TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->display.format: 	[%c][%c][%c][%c]\n",(camera->display.format>>24 & 0x000000ff),(camera->display.format>>16 & 0x000000ff),(camera->display.format>>8 & 0x000000ff),(camera->display.format & 0x000000ff));

	tcc_overlay_set_screen_info(camera->fp_display,&camera->display);

	if(camera->fp_fb0 >= 0)
		tcc_fb_display(camera->fp_fb0);

	return camera->fp_display;



}

/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_display_camera
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int   tcc_v4l2_display_camera(tcc_v4l2_camera * camera, unsigned int index )
{

	unsigned int base[3] = {0, 0, 0};

	if( camera->format == TCC_FORMAT_YUV_420 )
	{
		base[0] = camera->mem[index].phy_addr;
		base[1] = base[0] + (camera->width*camera->height);
		base[2] = base[1] + (camera->width*camera->height/4);
	}
	else if ( camera->format == TCC_FORMAT_YUV_422 )
	{
		base[0] = camera->mem[index].phy_addr;
		base[1] = base[0] + (camera->width*camera->height);
		base[2] = base[1] + (camera->width*camera->height/2);
	}

	tcc_overlay_display_update(camera->fp_display, OVERLAY_QUEUE_BUFFER,&base);
}
/******************************************************************************
*	FUNCTIONS			: tcc_v4l2_deinit_display
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int   tcc_v4l2_deinit_display(tcc_v4l2_camera * camera)
{

	int ret=-1;

	if( camera->fp_fb0 !=-1) 
	{
		ret= tcc_fb_close(camera->fp_fb0);
		if( ret == -1 )
			 TCC_DEBUG (TCC_DEB_LEV_ERR,"Cannot close fb \n");
			
	}

	if( camera->fp_display !=-1) 
	{
		ret= tcc_overlay_close(camera->fp_display);
		if( ret == -1 )
			 TCC_DEBUG (TCC_DEB_LEV_ERR,"Cannot close display \n");
			
	}

	return 0;


}
#endif
#ifdef TCC_CAMERA_TEST
void main()
{

	int count =1000;
	tcc_v4l2_camera  camera;
	int ret;

	char  * buffer;
	tcc_v4l2_open(CAMERA_DEVICE_NAME_0,&camera);

	tcc_v4l2_get_physical_base_addr(&camera);
	tcc_v4l2_get_capability(&camera);
	tcc_v4l2_get_format(&camera);
//	tcc_v4l2_set_format(&camera,TCC_FORMAT_YUV_420,1920,1080);
	tcc_v4l2_set_format(&camera,TCC_FORMAT_YUV_420,1280,720);
	//tcc_v4l2_set_format(&camera,TCC_FORMAT_YUV_420,720,480);
	//tcc_v4l2_set_format(&camera,TCC_FORMAT_YUV_420,640,480);
	//tcc_v4l2_set_format(&camera,TCC_FORMAT_YUV_420,240,320);
	tcc_v4l2_alloc_mem(&camera);


	tcc_v4l2_start_stream(&camera);

#ifdef TCC_V4L2_DISPLAY
	tcc_v4l2_init_display(&camera);
#endif

	while( count)
	{
		tcc_v4l2_read_frame(&camera);

#ifdef TCC_V4L2_FILE_DUMP
		if( camera.fp_dump != NULL )
		{
			buffer = camera.mem[camera.buf.index].virtl_addr;
			ret = tcc_v4l2_dump_write(buffer,  camera.buf.bytesused, camera.fp_dump);
#ifdef TCC_V4L2_DEBUG_MESSAGE		
		TCC_DEBUG (TCC_DEB_LEV_LOG,"dump write =%d\n",ret);
#endif
		}	
#endif 	

#ifdef TCC_V4L2_DISPLAY
		tcc_v4l2_display_camera(&camera,camera.buf.index);
		TCC_DEBUG (TCC_DEB_LEV_LOG,"camera->buf.index:%d\n",camera.buf.index);
#endif
		

		tcc_v4l2_return_frame(&camera);
		count --;
	}	


	tcc_v4l2_end_stream(&camera);
	tcc_v4l2_free_mem(&camera);

#ifdef TCC_V4L2_DISPLAY
	tcc_v4l2_deinit_display(&camera);
#endif
	tcc_v4l2_close(&camera);
	

}
#endif
