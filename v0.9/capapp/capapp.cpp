#include "capapp.h"
//#define _8925_8D1

//#define STUDENT_SWIPE_CARD

DBusServerC *cap_DbusServer;
DBusClientC *ca_ClientJT808;
DBusClientC *re_Device;

DBusClientC *cap_Client;
DBusServerC	*ca_DbusServer;
DBusServerC *cap_Alarm;

#define ENCODE_FRAMERATE	25 //fps
#define TEST_FRAME			ENCODE_FRAMERATE * 30/*sec*/
#define VIDEO_SAVE_DIR          "/nand2/innovdata/video/"

//#define TEST_JT808_CAPTURE_VIDEO 1
//#define TEST_REVERSE 1
//#define TEST_VIDEO 1

/*
	Camera Format
*/
#define NUM_OF_SUPPORT_CAMERA			2
#define CAMERA_FORMAT					TCC_FORMAT_YUV_420 //TCC_FORMAT_YUV_420 //TCC_FORMAT_YUV_420IL0

#define CAM_0	"cam0"

#define UDP_PORT 8903


int resolution[][2] = {
	{320, 240},
	{640, 480},
	{800, 600},
	{1024,768},
	{176, 144},
	{352, 288},
	{704, 288},
	{704, 576}
};

unsigned char chan[] = {0x30, 0x31, 0x32, 0x33, 0x20}; 

static struct cap_obj *capobj;
static struct video_info videoinfo;
extern CameraDeviceT dev;
extern int video_finish;
extern long video_start_time;
extern int bForceExit;
static int handle_image_exit = 0;
static int handle_video_exit = 0;
static long total_disk_kb = 0;
char video_name[128] = "";
char pic_name[128] = "";

int is_gui_display(int on)
{	
	lcdc_layerctrl_params lcdparm;
    lcdparm.lcdc_num = 0;
    lcdparm.layer_num = 0;

	printf("here dev.fd_fb:%d\n", dev.fd_fb);

    if(!!on)
		ioctl(dev.fd_fb, TCC_LCDC_LAYER_ENABLE, &lcdparm);
    else
		ioctl(dev.fd_fb, TCC_LCDC_LAYER_DISABLE, &lcdparm);

	return 0;
}

int I2c_read_video_type(unsigned char &value, int len)
{
	int ret = -1;	
	IBI2CControl i2cHander;
	len++;

	if (i2cHander.checkOK() == 0)
	{
		bq_printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.readByte(SENSER_I2C_ADDRESS, SENSER_I2C_VIDETYPE, value);
    
	return ret;
}

int i2c_write_video_channle(unsigned char value)
{
	int ret = -1;	
	IBI2CControl i2cHander;

	if (i2cHander.checkOK() == 0)
	{
		bq_printf("Check mcu i2cHander err!\n");
		return -1; 
	}
	
    ret = i2cHander.writeByte(SENSER_I2C_ADDRESS, SENSER_I2C_CHANNEL, value);
    
	return ret;
}

int video_type_change()
{
	unsigned char video_type_value = -1;
	unsigned char video_type_mode = -1;
	int change_flag = 0;
	
	int i = 10;
	while(i--)
	{
		I2c_read_video_type(video_type_value, 1);
		bq_printf("vodeo_type_value:%d\n", video_type_value);
		if(PAL_IN_VALUE == video_type_value || NTSC_IN_VALUE == video_type_value)
			break;
		usleep(500);
	}

	video_type_mode = (NTSC_IN_VALUE == video_type_value)?(NTSC_IN_MODE):(PAL_IN_MODE);
	bq_printf("video_type_mode:%d\n", video_type_mode);

	if(dev.video_mode !=  video_type_mode)
	{
		change_flag = 1;
		dev.video_mode = video_type_mode;
	}

	return change_flag;
}

int video_type_init()
{
	unsigned char videtype = -1;
	int i = 7;
	
	while(i--)
	{
		I2c_read_video_type(videtype, 1);
		usleep(100);
	}
	bq_printf("videotype:%x\n", videtype);
	
	if(NTSC_IN_VALUE == videtype)
	{
		dev.preview_height = PREVIEW_HEIGHT;
		dev.video_mode = 0;
	}
	else
	{
		dev.preview_height = PREVIEW_HEIGHT;
		dev.video_mode = 1;
	}

	dev.video_type_change = video_type_change;
	return 0;
}
  
string cstr_to_string(char *p_str) 
{ 
        return p_str; 
}

int write_capture_or_video_table(void *ptr, int flag)
{
    int alarm_signal 	= 0;
    int status_flag 	= 0;
    int latitude 		= 0;
    int longitude 		= 0;
    int altitude 		= 0;
    int orientation 	= 0;
    int gps_speed 		= 0;
    int occur_time 	    = 0;

#if 0
    VDRLocStatus1Table *loacl_table = new VDRLocStatus1Table();
    loacl_table->RefreshBottom();
    loacl_table->GetIntValue(VDR_LOCS_ALARM_SIGNAL, alarm_signal);
    loacl_table->GetIntValue(VDR_LOCS_STATUS_FLAG, status_flag);
    loacl_table->GetIntValue(VDR_LOCS_LATITUDE, latitude);
    loacl_table->GetIntValue(VDR_LOCS_LONGITUDE, longitude);
    loacl_table->GetIntValue(VDR_LOCS_ALTITUDE, altitude);
    loacl_table->GetIntValue(VDR_LOCS_ORIENTATION, orientation);
    loacl_table->GetIntValue(VDR_LOCS_GPS_SPEED, gps_speed);
    loacl_table->GetIntValue(VDR_LOCS_OCCUR_TIME, occur_time);
    delete loacl_table;
#endif
    FILE *fp = NULL;
    int num = 5;
    struct LocStatusData locbuf[5];
    
    memset(locbuf,0,sizeof(struct LocStatusData)*5);
    
    fp = fopen("/nand4/loc0809.vdr", "r");
    
    if(fp == NULL) 
    {
        printf("Can't open loc0809.vdr %s\n", strerror(errno));
    }
    else
    {
        fread(locbuf,sizeof(struct LocStatusData),num,fp);
        fclose(fp);
    }
    alarm_signal = locbuf[num-1].alarm_signal;
    status_flag = locbuf[num-1].status_falg;
    longitude = locbuf[num-1].longitude;
    latitude = locbuf[num-1].latitude;
    altitude = locbuf[num-1].altitude;
    orientation = locbuf[num-1].orientation;
    gps_speed = locbuf[num-1].gps_speed;
    occur_time = locbuf[num-1].itime;

    if(flag == 0) //picture
    {
        
        VDRCaptureTable *table = new VDRCaptureTable();
        string filepath;
        if(table->GetRowCount() >= VDR_MONITOR_CAPTURE_TABLE_MAX_ROWS)
        {
            table->RefreshTop();    
            table->GetStringValue(VDR_CAP_FILE_PATH, filepath);
        }
        
        remove(filepath.c_str());
        sync();
        
        struct pic_info *picinfo_ptr = (struct pic_info *)ptr;
        table->SetStringValue(VDR_CAP_FILE_PATH, cstr_to_string(pic_name));
        table->SetIntValue(VDR_CAP_FILE_TIME, picinfo_ptr->m_captime);
        table->SetIntValue(VDR_CAP_FILE_TYPE, 0);
        table->SetIntValue(VDR_CAP_FILE_QUALITY, picinfo_ptr->m_quality);
        table->SetIntValue(VDR_CAP_RESOLUTION, picinfo_ptr->m_resolution);
        table->SetIntValue(VDR_CAP_BRIGHTNESS, picinfo_ptr->m_brightness);
        table->SetIntValue(VDR_CAP_CONTRAST, 1);
        table->SetIntValue(VDR_CAP_SATUATION, picinfo_ptr->m_saturation);
        table->SetIntValue(VDR_CAP_CHROMA, picinfo_ptr->m_chroma);
        table->SetIntValue(VDR_CAP_EVENT, picinfo_ptr->m_event);
        table->SetIntValue(VDR_CAP_CHANNEL_ID, capobj->chancur_status);
        table->SetIntValue(VDR_CAP_ALARM_SIGNAL, alarm_signal);
        table->SetIntValue(VDR_CAP_STATUS_FLAG, status_flag);
        table->SetIntValue(VDR_CAP_LATITUDE, latitude);
        table->SetIntValue(VDR_CAP_LONGITUDE, longitude);
        table->SetIntValue(VDR_CAP_ALTITUDE, altitude);
        table->SetIntValue(VDR_CAP_ORIENTATION, orientation);
        table->SetIntValue(VDR_CAP_GPS_SPEED, gps_speed);
        table->SetIntValue(VDR_CAP_OCCUR_TIME, occur_time);
        table->Commit();
        delete table;
    }
    else //video
    {
        struct video_info *videoinfo_ptr = (struct video_info *)ptr;
        VDRVideoTable *table = new VDRVideoTable();
        string filepath;
        if(table->GetRowCount() >= VDR_MONITOR_VIDEO_TABLE_MAX_ROWS)
        {
            table->RefreshTop();    
            //table->GetStringValue(VDR_CAP_FILE_PATH, filepath);
        }
        
        //remove(filepath.c_str());
        //sync();
        
        table->SetStringValue(VDR_CAP_FILE_PATH, cstr_to_string(video_name));
        table->SetIntValue(VDR_CAP_FILE_TIME, video_start_time);
        table->SetIntValue(VDR_CAP_FILE_TYPE, 0);
        table->SetIntValue(VDR_CAP_FILE_QUALITY, videoinfo_ptr->m_quality);
        table->SetIntValue(VDR_CAP_RESOLUTION, videoinfo_ptr->m_resolution);
        table->SetIntValue(VDR_CAP_BRIGHTNESS, videoinfo_ptr->m_brightness);
        table->SetIntValue(VDR_CAP_CONTRAST, 1);
        table->SetIntValue(VDR_CAP_SATUATION, videoinfo_ptr->m_saturation);
        table->SetIntValue(VDR_CAP_CHROMA, videoinfo_ptr->m_chroma);
        table->SetIntValue(VDR_CAP_EVENT, videoinfo_ptr->m_event);
        table->SetIntValue(VDR_CAP_CHANNEL_ID, capobj->chancur_status);// + 1 ??
        table->SetIntValue(VDR_CAP_ALARM_SIGNAL, alarm_signal);
        table->SetIntValue(VDR_CAP_STATUS_FLAG, status_flag);
        table->SetIntValue(VDR_CAP_LATITUDE, latitude);
        table->SetIntValue(VDR_CAP_LONGITUDE, longitude);
        table->SetIntValue(VDR_CAP_ALTITUDE, altitude);
        table->SetIntValue(VDR_CAP_ORIENTATION, orientation);
        table->SetIntValue(VDR_CAP_GPS_SPEED, gps_speed);
        table->SetIntValue(VDR_CAP_OCCUR_TIME, occur_time);
        table->Commit();
        delete table;
    }

    return 0; 
}

void *handle_image(void *param)
{
	 CameraDeviceT *dev = (CameraDeviceT *)param;
	 int width = -1;
	 int height = -1;

	 struct pic_info picinfo;

	 while(!handle_image_exit)
	 {
		 if(queue_empty(&dev->Q) == 0)
		 {
			usleep(50*100);
			continue;
		 }

		 printf("come here!\n");
		 memset(&picinfo, 0, sizeof(struct pic_info));
		 de_queue(&dev->Q, (char *)&picinfo);


		 struct tm *tm;
		 tm = localtime((time_t*)&picinfo.m_captime);
	     sprintf(picinfo.timebuf, "%d%02d%02d_%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
		 tm->tm_hour, tm->tm_min, tm->tm_sec);


    	 //char namebuf[1024] = {0};
    	 //char namebufscale[1024] = {0};
	     char buffer_src[128] = {0};
	 	 char buffer_det[128] = {0};
    	 sprintf(buffer_src, "%s%s_%d%03u_src.jpg", CAPTURE_FILE_PATH, picinfo.timebuf, picinfo.m_channel, dev->picture_num);
    	 sprintf(buffer_det, "%s%s_%d%03u.jpg", CAPTURE_FILE_PATH, picinfo.timebuf, picinfo.m_channel, dev->picture_num);

    	 //FILE* file_fd;
         FILE* file_fd = fopen(buffer_src,"w+");

    	 //int ret = compress_yuyv_to_jpeg_suoxiao(src, dest, self->preview_width , self->preview_height, 80);//数据转换

    	 //memset(dev->name, 0, 1024);
    	 //strcpy(dev->name, namebufscale);
    	 //memset(picinfo.name, 0, 1024);
    	 //strcpy(picinfo.name, namebufscale);

		 
		 //width = 640;//resolution[picinfo.m_resolution-1][0];
		 //height = 480;//resolution[picinfo.m_resolution-1][1];
		 width = resolution[picinfo.m_resolution-1][0];
		 height = resolution[picinfo.m_resolution-1][1];

		 unsigned char dest[picinfo.data_len*2];
		 int ret = put_jpeg_yuv420p_memory(dest, picinfo.data_len, (unsigned char *)picinfo.data, CAMERA_WIDTH, CAMERA_HEIGHT, 80);
    	 fwrite(dest, ret, 1, file_fd);//转换后的数据写入
    	 fflush(file_fd);
    	 fclose(file_fd);

		 scalejpeg(buffer_src, buffer_det, width, height);
		 remove(buffer_src);
		 unlink(buffer_src);
		 sync();

		 if(dev->picinfo.m_count)	
		 	dev->picinfo.m_count--;

		 //continue;
		 
		 //sprintf(buffer_src, "%s_src.jpg", picinfo.name);
		 //sprintf(buffer_det, "%s.jpg", picinfo.name);

		 bq_printf("handle resolution:%d\n", picinfo.m_resolution);
		 if(picinfo.m_resolution < 1 || picinfo.m_resolution > 8)
		 	picinfo.m_resolution = 1;

         strcpy(pic_name, buffer_det);
		 write_capture_or_video_table(&picinfo, 0);
	}

	return NULL;
}

int image_post_init()
{
	int ret = -1;

	pthread_t camapp_threads;	
	ret = pthread_create(&camapp_threads, NULL, handle_image, (void *)&dev);
	if(ret < 0)
	{
		bq_printf("create handle_image error!\n");
		return ret;
	}
	pthread_detach(camapp_threads);

	return ret;

}

void image_post_uninit()
{
	handle_image_exit = 1;
	return;
}

int switch_cam(int mode)
{
	if(mode < 0 || mode > 4)
		return -1;

	i2c_write_video_channle(chan[mode]);
	capobj->chancur_status = mode;

	return 0;
}

static int64_t mydu(const char *path)
{
	int i;
	glob_t globres;
	int64_t sum;
	static struct stat statres;
	static char nextpath[1024];

	if(lstat(path, &statres) < 0)
	{
		perror("lstat()");
		return 0;
	}
	
	if(!S_ISDIR(statres.st_mode))
		return statres.st_blocks;

	strncpy(nextpath, path,1024);
	strncat(nextpath, "/*" , 1024);
	glob(nextpath,GLOB_NOSORT, NULL, &globres);
	
	strncpy(nextpath, path,1024);
	strncat(nextpath, "/.*" , 1024);
	glob(nextpath,GLOB_NOSORT|GLOB_APPEND, NULL, &globres);
	
	sum = statres.st_blocks;

	for(i = 0 ;i < globres.gl_pathc ; i++)
	{
		char *pos;

		pos = strrchr(globres.gl_pathv[i], '/');

		if(strcmp(pos+1,".") && strcmp(pos+1,".."))
			sum += mydu(globres.gl_pathv[i]);
	}
	
	return sum/2;//1024B
}

static int delete_earliest_files()
{
    DIR *dir;
	char earliest_file_name[128] = "";
    char dir_name[128] = VIDEO_SAVE_DIR;
    char file_name[128] = "";
    
    struct stat st;
    struct dirent *p_dir_info;

    int64_t used_size = 0;
    
    while((used_size = mydu("/nand2")) >= total_disk_kb - 102400)
    {
        int earliest_time = time(NULL);
        
        printf("[INFO] /nand2 has used size is %lldKB ....\n", used_size);
        fprintf(stdout, "[WARNING] The /nand2 has almost no space ....\n");

        strcpy(dir_name, VIDEO_SAVE_DIR);
        dir = opendir(dir_name);
        for(p_dir_info = readdir(dir); p_dir_info != NULL; p_dir_info = readdir(dir))
        {
        	memset(file_name, 0, 128);
        	strcpy(file_name, dir_name);

        	if(strcmp(p_dir_info->d_name,".") && strcmp(p_dir_info->d_name,".."))
        	{
        		strcat(file_name, p_dir_info->d_name);
                //printf("---- %s ----\n", file_name);
                if(stat(file_name, &st) < 0)
                {
                    perror("stat()");
                    return -1;
                }

                if(st.st_mtime < earliest_time)
                {
                    strcpy(earliest_file_name, file_name);
                    earliest_time = st.st_mtime;               
                }
        	}
        }

        fprintf(stdout, "[INFO] delete %s ....\n", earliest_file_name);
        unlink(earliest_file_name);
        fflush(NULL);
        closedir(dir);

        VDRVideoTable *table = new VDRVideoTable();
        string filepath;
        int max_num;
        int count = 0;
        
        max_num = table->GetRowCount();
        while(count <= max_num)
        {
            table->RefreshTop();    
            table->GetStringValue(VDR_CAP_FILE_PATH, filepath);
            if(!strcmp(earliest_file_name, filepath.c_str()))
            {
                table->Delete(filepath);
                break;
            }
            count++;
        }
        
    	delete table;
    }

    return 0;
}

static int delete_illegal_power_files()
{
    char d_cmd[128];
    
    memset(d_cmd, 0, 128);
    sprintf(d_cmd,"find %s -name '*.tmp' -exec rm {} \\;",VIDEO_SAVE_DIR);
    system(d_cmd);

    return 0;
}

void *handle_video(void *)
{
    while(!handle_video_exit)
    {
        long timeout = pow(2, 31);
        while(!video_finish)
        {
            //printf("video_finish = %d, still in video !!!!!!!!!!!!\n", video_finish);
            usleep(100*1000);
            timeout -= 100*1000;
            if(timeout < 0)
                break;
        }
        printf("-------> video_start_time=%ld,video_finish=%d\n",video_start_time,video_finish);

        if(video_finish == 1)
        {
            //if(capobj->chanold_status != capobj->chancur_status)
            //{
            //	switch_cam(capobj->chanold_status);
            //	sleep(1);
            //}

            if(capobj->chancur_status != CHANNEL_IN_ALL)
            {
                switch_cam(CHANNEL_IN_ALL);//keep previewing all lines
            	sleep(1);
                capobj->chancur_status = CHANNEL_IN_ALL;
            }

            capobj->capcur_status = CAPAPP_IN_DELAY;
            if(capobj->capold_status == CAPAPP_IN_PREVIEW)
            {
            	camera_preview();
            	is_gui_display(0);
            	capobj->capcur_status = CAPAPP_IN_PREVIEW;
            }
#ifdef STUDENT_SWIPE_CARD
            camera_video_recovery();
#endif

            write_capture_or_video_table(&videoinfo, 1);
            video_finish = 0;
            if(!videoinfo.m_flag)
            {
                JT808CameraDbusReqC    request;
                DBusReplyC  *reply = NULL;
                int ret;
                
            	request.m_tableIndex = video_start_time;
                request.trans_flag = 2;
                ret = ca_ClientJT808->sendService(&request,reply,5*1000);
                printf("--- m_tableIndex = %lld, trans_flag = %d ---\n", request.m_tableIndex, request.trans_flag);//==
                if(ret < 0)
                {
                    bq_printf("Send Service Error. Error Code: %d\n",ret);
                }
                else
                {
                    if(reply->getIdentifier()== DBUS_REP_ID_COMMON)
                    {
                        bq_printf("Received Common Reply. result:%d\n",reply->result);
                    }
                    else if(reply->getIdentifier()== JT808_DBUS_REP_ID_CAMERA)
                    {
                        bq_printf("Reply ok\n");
                    }
                    else
                    {
                        bq_printf("Reply not recongnized.\n");
                    }
                    ret = 0;
                    delete reply;
                }
            }
        }
        else if(video_finish == 2)
        {
            struct video_info tmp_v;
            memset(&tmp_v, 0x0, sizeof(struct video_info));
            tmp_v.m_channel                = 5;
            tmp_v.m_command                = 0;
            tmp_v.m_interval               = 0;
            tmp_v.m_flag                   = 1;
            tmp_v.m_resolution             = 1;
            tmp_v.m_quality                = 10;
            tmp_v.m_brightness             = 128;
            tmp_v.m_saturation             = 64;
            tmp_v.m_chroma                 = 0;
            tmp_v.m_framerate              = 15;
            tmp_v.m_event                  = 1;
            write_capture_or_video_table(&tmp_v, 1);
            video_finish = 0;
        }
    }

    delete_earliest_files();
    
    return NULL;
}

int video_db_init()
{
    int ret = -1;
    
    pthread_t video_db;
    handle_video_exit = 0;
    ret = pthread_create(&video_db, NULL, handle_video, (void *)&dev);
    if(ret < 0)
    {
        bq_printf("create handle_video_db error!\n");
        return ret;
    }
    pthread_detach(video_db);

    return ret;

}

void video_db_uninit()
{
    handle_video_exit = 1;
    return;
}

void *epoll_udp_recv_handler(void *)
{ 
	struct sockaddr_in addr_recv;
	int recvfd;
	int port = UDP_PORT;
	int epfd,nfds;
	struct epoll_event ev, events;
	unsigned int channel, full, preview_on;
	unsigned int i, rdata, ret;
	recvfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(recvfd == -1)
	{
		perror("socket");
		return NULL;
	}

	epfd = epoll_create(1);
	if(epfd == -1){
		perror("epoll_create");
		goto epoll_epfd_err;
	}

	ev.events = EPOLLIN|EPOLLET;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, recvfd, &ev);
	if(ret == -1){
		perror("epoll_ctl");
		goto epoll_ctl_err;
	}

	bzero(&addr_recv, sizeof(addr_recv));
	addr_recv.sin_family = AF_INET;
	addr_recv.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr_recv.sin_port = htons(port);

	bind(recvfd, (struct sockaddr*)&addr_recv, sizeof(addr_recv));

	while(1)
	{	
		nfds = epoll_wait(epfd, &events, 1, -1);
		for(i=0; i<nfds; i++)
		{
			if(events.events & EPOLLIN){

				recvfrom(recvfd, &rdata, 4, 0, NULL, NULL);
				
				/*
				 * rdata : first  byte -> full switch (0:zoom 1:full)
				 *		   second byte -> channel No.(0:ch1 1:ch2 2:ch3 3:ch4 4:4-IN-1)
				 *		   third  byte -> preview switch(0:on 1:off)
				 *		   fourth byte -> no used...
				 */

				full = rdata & 0xFF;	
				channel = (rdata >> 8) & 0xFF;
				preview_on = (rdata >> 16) & 0xFF;

				switch_cam(channel);
				capobj->chancur_status = channel;

				if(1 == full){
					is_gui_display(0);
				}else if(0 == full){
					is_gui_display(1);
				}else if(2 == full){
					camera_cap(2,1);
				}else{
					printf("%s:normal <--> full screen exchange error...\n",__func__);
				}
				
				preview_output_set(full);

				if(0 == preview_on){

					camera_preview();
					capobj->capcur_status = CAPAPP_IN_PREVIEW;

				}else if(1 == preview_on){
				
					 camera_delay();
					 capobj->capcur_status = CAPAPP_IN_DELAY;

				}else{
				
					printf("%s:preview no change...\n", __func__);
				
				}
			}
		}
	}
epoll_ctl_err:
	close(epfd);
epoll_epfd_err:
	close(recvfd);
}

int udp_init()
{
	int ret = -1;

		pthread_t udp_recv;
		ret = pthread_create(&udp_recv, NULL, epoll_udp_recv_handler, NULL);
		if(ret < 0)
		{
			bq_printf("create udp_recv_handler error!\n");
				return ret;
		}
	pthread_detach(udp_recv);
		
		return ret;
}


void DeviceReverseOutDbusNotifyHandler(DBusNotifyC *notify)
{
	printf("--- [%s][%d] ---\n", __FUNCTION__, __LINE__);
		if(notify->getIdentifier()!= DEVICE_MANAGER_NOTF_ID_REVERSE)
			return;
				
				DeviceReverseDbusNotifyC *reverse_info = (DeviceReverseDbusNotifyC*)notify;
					
					CapappReverseDbusNotifyC notifyreverse;	
					
	if(!!reverse_info->flag)
		return;
							
	preview_output_set(0);
	is_gui_display(1);
	if(capobj->capold_status == CAPAPP_IN_DELAY)
	{
		camera_delay();
		capobj->capcur_status = CAPAPP_IN_DELAY;
	}

	switch_cam(capobj->chanold_status);
				
	//capobj->capcur_status = capobj->capold_status;
	
	notifyreverse.reverse = 0;
    notifyreverse.icount = reverse_info->icount;
	cap_DbusServer->sendNotify(&notifyreverse);
	
	return;
}

void JT808CamDbusNotifyHandler(DBusNotifyC *notify)
{
    printf("--- [%s][%d] ---\n", __FUNCTION__, __LINE__);
    if(notify->getIdentifier()!= JT808_DBUS_NTF_ID_CAMERA)
        return;

	JT808CameraDbusReqC    request;
    DBusReplyC  *reply = NULL;
    int ret = -1;
	unsigned int interval = 0;
    long first_time = -1;
	int timeout = 0;

	capobj->capold_status = capobj->capcur_status;
	capobj->chanold_status = capobj->chancur_status;

	JT808CameraDbusNotifyC *cam_info = (JT808CameraDbusNotifyC*)notify;

	bq_printf("m_command:%d\n", cam_info->m_command);
	bq_printf("m_channel:%d\n", cam_info->m_channel);

	if(cam_info->m_channel < 1 || cam_info->m_channel > 5)
	{
		bq_printf("channel num error:%d\n", cam_info->m_channel);
		return;
	}
	
#ifdef STUDENT_SWIPE_CARD
    cam_info->m_channel = 5;
#endif

	cam_info->m_channel -= 1;

	if(cam_info->m_channel != capobj->chancur_status)
	{
		switch_cam(cam_info->m_channel);
		sleep(1);
	}	
	cam_info->m_channel += 1;

	if(cam_info->m_command < 0 || cam_info->m_command > 0xFFFF)
	{
		bq_printf("pictures num error:%d\n", cam_info->m_command);
		return;
	}

    if(!cam_info->m_command) //stop
    {
        camera_cap_video_stop(1);
        usleep(120*1000);
        if(capobj->chancur_status != CHANNEL_IN_ALL)
        {
            switch_cam(CHANNEL_IN_ALL);
		    sleep(1);
            capobj->chancur_status = CHANNEL_IN_ALL;
        }
#ifdef STUDENT_SWIPE_CARD        
        camera_video_recovery();
#endif
        return;
    }
    else if(cam_info->m_command == 0xFFFF) //video
    {
        camera_cap_video_stop(2);
        usleep(120*1000);
        
        capobj->capcur_status = CAPAPP_IN_VIDEO;
        memset(&videoinfo, 0x0, sizeof(struct video_info));
        videoinfo.m_channel                = cam_info->m_channel;
        videoinfo.m_command                = cam_info->m_command;
        videoinfo.m_interval               = cam_info->m_interval;
        videoinfo.m_flag                   = cam_info->m_flag;
        videoinfo.m_resolution             = cam_info->m_resolution;
        videoinfo.m_quality                = cam_info->m_quality;
        videoinfo.m_brightness             = cam_info->m_brightness;
        videoinfo.m_saturation             = cam_info->m_saturation;
        videoinfo.m_chroma                 = cam_info->m_chroma;
        videoinfo.m_framerate              = 15;
        videoinfo.m_event                  = 0;
        camera_video(&videoinfo);

        return;
    }

    //capture
	if(CAPAPP_IN_PHOTO == capobj->capcur_status)
	{
		bq_printf("already in take picture status!\n");
		return;
	}
	capobj->capcur_status = CAPAPP_IN_PHOTO;
	if(!capobj->pthoto_status)
	{
		capobj->pthoto_status = PHOTO_IN_BACKEND;
	}

	interval = (cam_info->m_interval > 1)?cam_info->m_interval:1;

	int i = 0;

	dev.picinfo.m_channel 		= cam_info->m_channel;
	dev.picinfo.m_command 		= cam_info->m_command;
	dev.picinfo.m_interval 		= cam_info->m_interval;
	dev.picinfo.m_flag 			= cam_info->m_flag;
	dev.picinfo.m_resolution 	= cam_info->m_resolution;
	dev.picinfo.m_quality 		= cam_info->m_quality;
	dev.picinfo.m_brightness 	= cam_info->m_brightness;
	dev.picinfo.m_saturation 	= cam_info->m_saturation;
	dev.picinfo.m_chroma 		= cam_info->m_chroma;
	dev.picinfo.m_event			= capobj->pthoto_status;   	
	dev.picinfo.m_count			= dev.picinfo.m_command;
	//irq_ts_disable();
	for(i = 0; i < cam_info->m_command; i++)
	{	
		printf("i:%d\n", i);
		
		pthread_mutex_lock(&dev.pic_mutex);
		camera_cap(cam_info->m_resolution, i);
        pthread_cond_wait(&dev.pic_cond, &dev.pic_mutex);
		pthread_mutex_unlock(&dev.pic_mutex);
        if(!i)
			first_time = dev.cap_time;

        printf("first_time = %ld, dev.cap_time = %ld\n", first_time, dev.cap_time);
		while((my_time() - dev.cap_time) < interval)
		{
			usleep(50*1000);
		}
	}

	//irq_ts_enable();
	timeout = pow(2, 31);
	while(dev.picinfo.m_count)
	{
		bq_printf("still in capture!!!!!!!!!!!!!!!\n");
  		usleep(50*1000);

		timeout -= 50*1000;
		if(timeout < 0)
			return;
	}
    system("sync");
  
    if(!cam_info->m_flag)
    {
    	request.m_tableIndex = first_time;
        request.trans_flag = 1;
        ret = ca_ClientJT808->sendService(&request,reply,5*1000);
        printf("--- m_tableIndex = %lld, trans_flag = %d ---\n", request.m_tableIndex, request.trans_flag);//==
        if(ret < 0)
        {
            bq_printf("Send Service Error. Error Code: %d\n",ret);
        }
        else
        {
            if(reply->getIdentifier()== DBUS_REP_ID_COMMON)
            {
                bq_printf("Received Common Reply. result:%d\n",reply->result);
            }
            else if(reply->getIdentifier()== JT808_DBUS_REP_ID_CAMERA)
            {
                bq_printf("Reply ok\n");
            }
            else
            {
                bq_printf("Reply not recongnized.\n");
            }
            ret = 0;
            delete reply;
        }
	}else{
	
    	request.m_tableIndex = first_time;
        request.trans_flag = 0;
        ret = ca_ClientJT808->sendService(&request, reply, 0);
        printf("--- m_tableIndex = %lld, trans_flag = %d ---\n", request.m_tableIndex, request.trans_flag);//==
        if(ret < 0)
        {
            bq_printf("Send Service Error. Error Code: %d\n",ret);
        }
	
            delete reply;
	}

    //if(capobj->chanold_status != capobj->chancur_status)
    //{
    //	switch_cam(capobj->chanold_status);
    //	sleep(1);
    //}

    if(capobj->chancur_status != CHANNEL_IN_ALL)
    {
        switch_cam(CHANNEL_IN_ALL);//keep previewing all lines
    	sleep(1);
        capobj->chancur_status = CHANNEL_IN_ALL;
    }

    capobj->capcur_status = CAPAPP_IN_DELAY;
    
    if(capobj->capold_status == CAPAPP_IN_PREVIEW)
    {
    	camera_preview();
    	is_gui_display(0);
    	capobj->capcur_status = CAPAPP_IN_PREVIEW;
    }
}

void DeviceAlarmDbusNotifyHandler(DBusNotifyC *notify)
{
    printf("--- [%s][%d] ---\n", __FUNCTION__, __LINE__);
	if(notify->getIdentifier()!= DEVICE_MANAGER_NOTF_ID_CAMERA)
        return;
	
	int i = 0;
	DeviceCameraDbusNotifyC *cam_info = (DeviceCameraDbusNotifyC*)notify;

	bq_printf("event error:%d\n", cam_info->m_event);

	if(cam_info->m_event < 0 || cam_info->m_event > PHOTO_IN_OHTERALARM)
	{
		bq_printf("event error:%d\n", cam_info->m_event);
		return;
	}
	else
	{
		capobj->pthoto_status = cam_info->m_event;

		for(i = 0; i < 5; i++)
		{
			JT808CameraDbusNotifyC info;
			bq_printf("channelall:%d, channel:%d\n", cam_info->m_channel, (cam_info->m_channel & (int)pow(2, i)) >> i);
			if((cam_info->m_channel & (int)pow(2, i)) >> i)
			{	
				info.m_channel = 5;
				info.m_command = cam_info->m_command;
				info.m_interval = cam_info->m_interval;
				info.m_flag = (info.m_flag & (int)pow(2, i)) >> i;
				info.m_resolution = cam_info->m_resolution;
				info.m_quality = 0;
				info.m_brightness = 0;
				info.m_saturation = 0;
				info.m_chroma = 0;

				JT808CamDbusNotifyHandler((DBusNotifyC *)&info);
			}
		}

		capobj->pthoto_status = PHOTO_IN_BACKEND;
	}

}

void UiSwitchChannelHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    printf("--- [%s][%d] ---\n", __FUNCTION__, __LINE__);
	CapappDbusReqC * req = (CapappDbusReqC *)request;
	
	int cap_num = 0;
	if(capobj->capcur_status == CAPAPP_IN_REVERSE || capobj->capcur_status == CAPAPP_IN_PHOTO)
		return;

	if(req->display_flag)
	{
		if(capobj->capcur_status != CAPAPP_IN_PREVIEW)
		{
			camera_preview();
			is_gui_display(0);
			capobj->capcur_status = CAPAPP_IN_PREVIEW;
		}
		
		if(req->switch_flag)
		{	
			int state = capobj->chancur_status + 1;
            if(state == CHANNEL_IN_ALL)
                switch_cam(CHANNEL_IN_ALL);
            else
            {
			    cap_num = offset(state);
			    switch_cam(cap_num);
            }
		}
		
	}
	else
	{
		is_gui_display(1);
		camera_delay();
		capobj->capcur_status = CAPAPP_IN_DELAY;
	}

	return;
}

struct cap_obj *capobj_create(int size)
{
	if(size < 0)
		size = 0;
	
	struct cap_obj *capobj = (struct cap_obj *)malloc(sizeof(struct cap_obj) + size);
	if(NULL == capobj)
	{
		bq_printf("malloc capobj error!\n");
		return NULL;
	}
	memset(capobj, 0x0, sizeof(struct cap_obj) + size);

	return capobj;
}

void capobj_destory()
{
	free(capobj);
	capobj = NULL;
}

int capobj_init(struct cap_obj *capobj)
{
	if(NULL == capobj)
	{
		bq_printf("error param!\n");
		return -1;
	}
	
	capobj->capcur_status = CAPAPP_IN_DELAY;
	capobj->capold_status = CAPAPP_IN_DELAY;
	capobj->pthoto_status = PHOTO_IN_BACKEND;
	capobj->chancur_status = CHANNEL_IN_REVERSE;
	capobj->chanold_status = CHANNEL_IN_REVERSE;

	capobj->school_custom = 0;

	capobj->pri = (void *)(capobj + 1);

	return 0;
}

void UiReverseInDbusNotifyHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    printf("--- [%s][%d] ---\n", __FUNCTION__, __LINE__);
	CapappReverseDbusReqC * req = (CapappReverseDbusReqC *)request;

	if(!req->reverse)
		return;

	capobj->chanold_status = capobj->chancur_status;
	capobj->capold_status = capobj->capcur_status;
	
	printf("current is %d\n", capobj->capcur_status);
	//printf("reverse: %d\n", req->reverse);

	switch(req->reverse){
		case TURN_LEFT:
			switch_cam(CHANNEL_IN_LEFT);
			break;
		case TURN_RIGHT:
			switch_cam(CHANNEL_IN_RIGHT);
			break;
		case DO_REVERSE:
			switch_cam(CHANNEL_IN_REVERSE);
			break;
		default:
			printf("###########default out...\n");
			break;
	}
	//if(capobj->chancur_status != CHANNEL_IN_ALL)
	//{	
	//	i2c_write_video_channle(chan[CHANNEL_IN_ALL]);
	//}
	
	//if(capobj->capcur_status != CAPAPP_IN_PREVIEW)
	//{
		preview_output_set(1);
		camera_preview();
		is_gui_display(0);
		capobj->capcur_status = CAPAPP_IN_PREVIEW;
	//}

}

void UiSetVideoInfoNotifyHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    printf("--- [%s][%d] ---\n", __FUNCTION__, __LINE__);
    CapappVideoInfoDbusReqC *req = (CapappVideoInfoDbusReqC *)request;

    camera_set_video_info(req->framerate, req->duration);
    
    int tmp = 0;
    tmp |= req->framerate;
    tmp |= (req->duration << 8); 
    printf("[%s] ColD1: 0x%x:%d\n", __FUNCTION__, tmp, tmp);
    VDRBaseConfigTable *table = new VDRBaseConfigTable();
    table->SetIntValue(VDR_BASE_CONFIG_D1, tmp);
    table->Commit();
    delete table;
}

void DeviceAccOffDbusNotifyHandler(DBusNotifyC *notify)
{
	if(notify->getIdentifier() != DEVICE_MANAGER_NOTF_ID_ACCOFF)
        return;
    
    camera_cap_video_stop(2);
    bForceExit = 1;
}

int cap_dbus_init()
{
	cap_DbusServer = new DBusServerC(CAPAPP_DBUS_SERVER_ID, CAPAPP_DBUS_SERVER_OBJ);
	cap_DbusServer->registerService(CAPAPP_REQ_ID_REVERSE, UiSwitchChannelHandler, CapappDbusReqC::factory);	
	cap_DbusServer->registerService(CAPAPP_REQ_ID_REVERSEIN, UiReverseInDbusNotifyHandler, CapappReverseDbusReqC::factory);
    cap_DbusServer->registerService(CAPAPP_REQ_ID_SET_VIDEOINFO, UiSetVideoInfoNotifyHandler, CapappVideoInfoDbusReqC::factory);
    if(cap_DbusServer->connect() < 0)
    {
        bq_printf("Cann't connect to capapp DBUS server.\n");
        return -1;
    }

	ca_ClientJT808 = new DBusClientC(JT808_DBUS_SERVER_ID, JT808_DBUS_SERVER_OBJ);
	ca_ClientJT808->registerReply(JT808_DBUS_REP_ID_CAMERA, JT808CameraDbusRepC::factory);
	ca_ClientJT808->registerNotify(JT808_DBUS_NTF_ID_CAMERA,JT808CamDbusNotifyHandler,JT808CameraDbusNotifyC::factory);

	if(ca_ClientJT808->connect()<0)
    {
        bq_printf("Cann't connect to jt808 DBUS server.\n");
        return -1;
    }

	re_Device = new DBusClientC(DEVICE_MANAGER_DBUS_SERVER_ID, DEVICE_MANAGER_DBUS_SERVER_OBJ);
	re_Device->registerNotify(DEVICE_MANAGER_NOTF_ID_REVERSE, DeviceReverseOutDbusNotifyHandler, DeviceReverseDbusNotifyC::factory);
	re_Device->registerNotify(DEVICE_MANAGER_NOTF_ID_CAMERA, DeviceAlarmDbusNotifyHandler,DeviceCameraDbusNotifyC::factory);
    re_Device->registerNotify(DEVICE_MANAGER_NOTF_ID_ACCOFF, DeviceAccOffDbusNotifyHandler,DeviceAccOffDbusNotifyC::factory);
    
	if(re_Device->connect()<0)
    {
        printf("Cann't connect to re_Device DBUS server.\n");
        return -1;
    }

	return 0;
}

void cap_dbus_uninit()
{
	delete re_Device;
	delete ca_ClientJT808;
	delete cap_DbusServer;
}

 
void camera_close()
{
    image_post_uninit();

	camera_uninit();
	
	capobj_destory();

	cap_dbus_uninit();
}

void read_db_set_video_info()
{
	int tmp, tmp_framerate, tmp_duration;
	VDRBaseConfigTable *table = new VDRBaseConfigTable();
	table->GetIntValue(VDR_BASE_CONFIG_D1, tmp);
	delete table;
	
	tmp_framerate = tmp & 0xFF;
	tmp_duration = (tmp & 0xFF00) >> 8;
	printf("[%s] ColD1: 0x%x:%d, framerate = 0x%x:%d, duration = 0x%x:%d\n", 
	__FUNCTION__, tmp, tmp, tmp_framerate, tmp_framerate, tmp_duration, tmp_duration);
	
	camera_set_video_info(tmp_framerate, tmp_duration);
}

#ifndef TEST
void TestServiceHandler(DBusRequestC *request, DBusReplyC *&reply)
{
	printf("server receive!\n");

	return;
}
#endif

int get_total_size()
{
    system("df | grep nand2 > /tmp/tmp_cap.txt");

    int fd;
    int i = 0, j = 0;
    char buf[64];
    int flag = 0;
    char tmp_b[20] = "";

    fd = open("/tmp/tmp_cap.txt", O_RDONLY);
    if(fd < 0)
    {
        perror("open()");
        return -1;
    }

    for(i = 0; i < 64; i++)
    {
        read(fd, (void *)(buf + i), 1);
        if(!flag)
        {
            if(buf[i] != ' ')
                continue;
            else
                flag = 1;
        }
        else
        {
            if(buf[i] == ' ')
            {
                if(j != 0)
                    break;
                
                continue;
            }
            else
                tmp_b[j++] = buf[i];
        }
    }
    tmp_b[j] = '\0';
    close(fd);
    unlink("/tmp/tmp_cap.txt");

    return atol(tmp_b);
}

typedef struct
{
	int thread_index;
	pthread_t hThread;
	int width;
	int height;
	char name[1024];
}thread_private_t;
 

int main(int argc, char **argv)
{
	DBusApplicationC app(argc, argv);
		
		JT808CameraDbusNotifyC nor_notify;
		CapappDbusReqC request;
		DBusReplyC  *reply = NULL;
		char key_input[64];
		
		atexit(camera_close);
		memset(&videoinfo, 0x0, sizeof(struct video_info));
		
	if(cap_dbus_init())
	{
		bq_printf("cap_dbus_init error!\n");
		return -1;
	}
	
	bq_printf("connet successfully!\n");

	capobj = capobj_create(0);
	if(NULL == capobj)
	{
		bq_printf("create capobj error!\n");
		goto err_capobj_create;
	}

	if(capobj_init(capobj) < 0)
	{
		bq_printf("init capobj error!\n");
		goto err_capobj_init;	
	}

    total_disk_kb = get_total_size();
    delete_illegal_power_files();
    read_db_set_video_info();

	if(camera_init((void *)video_type_init) < 0)
	{
		bq_printf("camera_init error!\n");
		goto err_camera_init;
	}
	
	if(image_post_init() < 0)
	{
		bq_printf("image_post_init error!\n");
		goto err_image_post_init;
	}

    if(video_db_init() < 0)
    {
        bq_printf("video_db_init error!\n");
		goto err_video_db_init;
    }

#ifdef _8925_8D1
	if(udp_init() < 0){
		bq_printf("udp_init error!\n");
		goto err_video_db_init;
	}
#endif

#ifdef STUDENT_SWIPE_CARD
	switch_cam(CHANNEL_IN_ALL);
    sleep(1);
	camera_preview();
	is_gui_display(0);
	capobj->capcur_status = CAPAPP_IN_PREVIEW;
	capobj->school_custom = 1;

    /* start continuous video */
    switch_cam(CHANNEL_IN_ALL);
    sleep(1);
    camera_video(NULL);
    capobj->capcur_status = CAPAPP_IN_VIDEO;
#endif

    if(capobj->chancur_status != CHANNEL_IN_ALL)
    {
        switch_cam(CHANNEL_IN_ALL);
        sleep(1); 
    }

	system("rm -rf /nand2/innovdata/photo/*src.jpg");
	sync();

#ifdef TEST_JT808_CAPTURE_VIDEO
    printf("========= 1: JT808 cmd start capture =========\n");
	nor_notify.m_channel = 5;
    nor_notify.m_command = 4;
	nor_notify.m_interval = 5;
	nor_notify.m_flag = 0;
	nor_notify.m_resolution = 1;
	nor_notify.m_quality = 1;
	nor_notify.m_brightness = 1;
	nor_notify.m_saturation = 1;
	nor_notify.m_chroma = 1;
	JT808CamDbusNotifyHandler(&nor_notify);
#if 1
    /* start video */
    sleep(15);//default video mode
    printf("========= 2: JT808 cmd start video =========\n");
    nor_notify.m_channel = 5;
	nor_notify.m_command = 0xFFFF;
	nor_notify.m_interval = 55;
	JT808CamDbusNotifyHandler(&nor_notify);
#endif
    /* stop video */
    sleep(20);
    printf("========= 3: JT808 cmd stop video =========\n");
    nor_notify.m_command = 0;
    JT808CamDbusNotifyHandler(&nor_notify);
#endif

#ifdef TEST_REVERSE
	request.display_flag = 1;
    sleep(3);
	cap_Client->sendService(&request,reply,5*1000);
#endif



#ifdef TEST_VIDEO
	switch_cam(CHANNEL_IN_ALL);
    sleep(1);
    camera_preview();
    is_gui_display(0);
    capobj->capcur_status = CAPAPP_IN_PREVIEW;

	printf("[Capapp]# ");
	while(1)
	{	
		fgets(key_input, 1024, stdin);

        if(strcmp(key_input, "preview\n") == 0)
        {
            switch_cam(CHANNEL_IN_ALL);
            sleep(1);
            camera_preview();
            is_gui_display(0);
            capobj->capcur_status = CAPAPP_IN_PREVIEW;
        }
        
        if(camera_video_test(key_input))
            break;
        
		printf("[Capapp]# ");
        usleep(200000);
	}
#endif

	return app.loop();

err_video_db_init:
    video_db_uninit();
err_image_post_init:
	image_post_uninit();
	camera_close();
err_camera_init:
err_capobj_init:
	capobj_destory();
err_capobj_create:
	cap_dbus_uninit();
	
	return -1;
}

