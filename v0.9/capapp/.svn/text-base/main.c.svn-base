#include <stdio.h>
#include <pthread.h>
#include <memory.h>
#include <sys/time.h>
#include <unistd.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "encoder.h"
#include "venc.h"
#include "TCC_MP4_muxer.h"
#include "tcc_linux_system.h"
#include "tcc_scaler_interface.h"
#include "pmap.h"
#include "tcc_v4l2.h"
#include "yuv2jpg.h"
#include "scalejpeg.h"
#include "main.h"

#define ALIGN_BIT (0x8-1)
#define BIT_0 3
#define GET_ADDR_YUV42X_spY(Base_addr) 		(((((unsigned int)Base_addr) + ALIGN_BIT)>> BIT_0)<<BIT_0)
#define GET_ADDR_YUV42X_spU(Yaddr, x, y) 		(((((unsigned int)Yaddr+(x*y)) + ALIGN_BIT)>> BIT_0)<<BIT_0)
#define GET_ADDR_YUV422_spV(Uaddr, x, y) 		(((((unsigned int)Uaddr+(x*y/2)) + ALIGN_BIT) >> BIT_0)<<BIT_0)
#define GET_ADDR_YUV420_spV(Uaddr, x, y) 		(((((unsigned int)Uaddr+(x*y/4)) + ALIGN_BIT) >> BIT_0)<<BIT_0)

#define	DEF_POS_X	272
#define	DEF_POS_Y	208
#define	DEF_DST_W	468
#define	DEF_DST_H	220

#define ENCODE_FRAMERATE	25 //fps
#define DEFAULT_FRAMERATE   15
#if 1
#define FILE_FRAME_NUM			ENCODE_FRAMERATE * 1800/*sec*/
#else
#define FILE_FRAME_NUM			ENCODE_FRAMERATE * 30/*sec*/
#endif
//#define STOP_VIDEO_AFTER_SET_VIDEO 1 /* stop current video immediately when set video info (framerate, duration)*/

#define CAMERA_FORMAT					TCC_FORMAT_YUV_420 //TCC_FORMAT_YUV_420 //TCC_FORMAT_YUV_420IL0

#define DBG(fmt, args...) 			do{ printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args); }while(0)

#define VIDEO_SAVE_DIR "/nand2/innovdata/video/"

struct encoder_init_info
{
	int width;
	int height;
	int codec_format;
	int encoding_quality_level;
	int user_bitrate;

	int use_scaler;
	int scaler_dst_width;
	int scaler_dst_height;

	int show_pos_x;
	int show_pos_y;

	pthread_t v4l2_thread;

	int capture_flag;
	int preview_flag;
	int recoder_flag;
	int recoder_finish;

	int encoder_init;
	int base_time;
	int frame_count;
	unsigned long total_len;
	void* enc_context;
	void* mp4_handle;

	tENC_INIT_PARAMS init;
	tENC_FRAME_INPUT Input;
	tENC_FRAME_OUTPUT Output;

	tcc_v4l2_camera  camera;

	char pic_buf[1024*1024];

};

struct encoder_init_info *init_info;

int bForceExit = 0;
static pmap_t pmap_scaler;
CameraDeviceT dev;
static int mem_fd;
static void *scaler_map_ptr;
static int scaler_fd = -1;
static int gui_framerate = 0;
static int gui_duration = 0;

static struct video_info videoinfo;
extern char video_name[128];
static int video_cmd_flag; /* 0:default video  1:jt808 cmd  2:stop default video */
long video_start_time = 0;
int video_finish = 0; /* 0:not finish   1:jt808 cmd video   2:default video */

int package_video(struct encoder_init_info *priv)
{
	if(NULL == priv)
	{
		printf("NULL param!\n");
		return -1;
	}

	priv->frame_count = 0;

	ENCODER_CLOSE(priv->enc_context);
	priv->enc_context = NULL;

	Mux_TCC_MP4_EncClose(priv->mp4_handle);
	priv->mp4_handle = NULL;

	priv->encoder_init = 0;
	priv->base_time = 0;

	return 0;
}

void* enc_thread(void* param)
{
	if(NULL == param)
	{
		printf("NULL param!\n");
		return NULL;
	}

	struct encoder_init_info *priv = (struct encoder_init_info *)param;

	unsigned long sum_of_output = 0;	
	int source_size = 0;
	char mux_output_filename[128];
	char tmp_dir_name[128];
	int frame_sum = FILE_FRAME_NUM;
	int tmp_framerate = DEFAULT_FRAMERATE;

	DBG("ENCODER START\n");
	strcpy(tmp_dir_name, VIDEO_SAVE_DIR);

	tcc_v4l2_open(CAMERA_DEVICE_NAME_0, &priv->camera);
	tcc_v4l2_get_physical_base_addr(&priv->camera);
	tcc_v4l2_get_capability(&priv->camera);
	tcc_v4l2_get_format(&priv->camera);
	tcc_v4l2_set_format(&priv->camera, CAMERA_FORMAT, priv->width, priv->height);
	tcc_v4l2_alloc_mem(&priv->camera);
	tcc_v4l2_start_stream(&priv->camera);

	dev.fd_fb = tcc_lcd_init();
	printf("dev.fd_fb:%d\n", dev.fd_fb);
	//tcc_set_video_output(priv->width, priv->height, priv->width, 100, 50, 360, 288, V4L2_PIX_FMT_YVU420, 0);

	int keyframe = 0;
	int ret = -1;

	priv->base_time = 0;
	priv->frame_count = 0;
	int tmp_count = 0;
	int tmp_video_time = 0;
	char frame_pass[][16] = {
		{2, 4, 6, 7, 9,10,12,13,15,16,18,20,21,23,24, 0},/* frameRate = 10 */
		{2, 4, 6, 8,10,12,14,16,18,20,22,24, 0, 0, 0, 0},/* frameRate = 12.5 */
		{2, 4, 7, 9,12,14,17,19,22,24, 0, 0, 0, 0, 0, 0},/* frameRate = 15 */
		{4, 9,14,19,24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}/* frameRate = 20 */
	};
	int f_row = -1, f_column = 0;

	do
	{    
		ret = tcc_v4l2_ioctl_do_query_buffer(priv->camera.fp,&priv->camera.buf);
		if(ret < 0)
		{
			DBG("Camera Query Buffer error!!\n");
			continue;
		}

		tcc_v4l2_ioctl_query_buffer(priv->camera.fp, &priv->camera.buf);


		if(priv->capture_flag)
		{
			pthread_mutex_lock(&dev.pic_mutex);
			dev.picture_num++;
			printf("num:%d\n", dev.picture_num);
			dev.picinfo.m_captime = time(NULL);    
			dev.cap_time = dev.picinfo.m_captime;

			source_size = priv->width * priv->height * 3/2;
			dev.picinfo.data_len = source_size;

			memcpy(dev.picinfo.data, priv->camera.mem[priv->camera.buf.index].virtl_addr, source_size);
			en_queue(&dev.Q, (char *)&dev.picinfo, sizeof(struct pic_info));

			priv->capture_flag = 0;

			pthread_cond_signal(&dev.pic_cond);
			pthread_mutex_unlock(&dev.pic_mutex);
			continue;
		}


		if(priv->preview_flag)
		{

			tcc_set_video_output(priv->width, priv->height, priv->width, 
								priv->show_pos_x, priv->show_pos_y, 
								priv->scaler_dst_width, priv->scaler_dst_height, 
								V4L2_PIX_FMT_YVU420, 0);

			tcc_run_video_output((void *)priv->camera.mem[priv->camera.buf.index].phy_addr, 
								NULL, NULL, NULL, NULL, NULL);
		}

		if(priv->recoder_flag)
		{
			if(!priv->encoder_init)
			{
				priv->init.codecFormat		= priv->codec_format;
				priv->init.picWidth			= priv->width;
				priv->init.picHeight			= priv->height;
				priv->init.frameRate			= ENCODE_FRAMERATE;		
				priv->init.targetKbps 		= priv->user_bitrate/ 1024; 
				priv->init.keyFrameInterval	= 30;
				priv->init.sliceMode			= 1; // 0:Off, 1: On	
				priv->init.sliceSizeMode		= 0; // 0:bits, 1:MBs 
				priv->init.sliceSize			= 4*1024; //4KB
				priv->init.use_NalStartCode	= 1;
				priv->init.bEncodingQualityLevel = priv->encoding_quality_level;
				priv->init.bChromaInterleaved = ((CAMERA_FORMAT == TCC_FORMAT_YUV_420) ? 0 : 1);
				tmp_count = 0;
				if(gui_framerate && gui_duration)
				{
					tmp_framerate = gui_framerate;
					frame_sum = gui_duration*60*25;
				}

				struct tm *tm;
				tmp_video_time = time(NULL);
				tm = localtime((time_t*)&tmp_video_time);
				printf("----------------> new video time = %d\n", tmp_video_time);
				sprintf(mux_output_filename, "%s%d%02d%02d%02d%02d%02d_%d.tmp", tmp_dir_name, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
						tm->tm_hour, tm->tm_min, tm->tm_sec, videoinfo.m_channel);
				if(video_cmd_flag == 1)
				{
					if(videoinfo.m_interval == 0)
						frame_sum = 99999999;
					else if(videoinfo.m_interval > 0)
						frame_sum = videoinfo.m_interval * 25;
					else
					{
						DBG("Error: video interval illegal!!!\n");
						video_cmd_flag = 0;
						continue;
					}

					tmp_framerate = videoinfo.m_framerate;
				}

				if(tmp_framerate == 10)
					f_row = 0;
				else if(tmp_framerate == 12 || tmp_framerate == 13)
					f_row = 1;
				else if(tmp_framerate == 15)
					f_row = 2;
				else if(tmp_framerate == 20)
					f_row = 3;
				else
					f_row = -1;

				printf("--- framerate:%d,frame_sum:%d,gui_framerate:%d,gui_duration:%d ---\n",
						tmp_framerate,frame_sum,gui_framerate,gui_duration);

				DBG("size:%d x %d, framerate:%d, kbps:%d quality level:%d, chroma interleave:%d\n", 
						priv->init.picWidth, priv->init.picHeight, priv->init.frameRate, priv->init.targetKbps, priv->init.bEncodingQualityLevel, priv->init.bChromaInterleaved);

				priv->enc_context = ENCODER_INIT(&priv->init);
				if(NULL == priv->enc_context)	
				{
					DBG(" ENCODER_INIT FAILED \n");
					goto FORCE_RETURN;
				}
				DBG("ENCODER_INIT SUCCEED \n");

				priv->Input.muxType = 1; /* mp4 */
				ret = ENCODER_ENC(priv->enc_context, &priv->Input, &priv->Output);
				if(ret < 0)
				{
					DBG(" ENCODER_ENC SEQ HEADER FAILED ret = %d\n", ret);
					goto FORCE_RETURN;
				}
				DBG(" ENCODER_ENC SEQ HEADER SUCCEED ret = %d, type:%d len:%d\n", ret, priv->Output.picType, priv->Output.headerLen);
				sum_of_output += priv->Output.headerLen;

				/* MP4 Muxer Init */
				priv->mp4_handle = Mux_TCC_MP4_EncOpen((unsigned char *)mux_output_filename, CCD_H264_MODE, CCD_NO_AUDIO_MODE, 44100, 16, 2, priv->init.picWidth, priv->init.picHeight, NULL, 0, NULL, 0, 0);
				if(priv->mp4_handle == NULL)
				{
					DBG("MP4 open Error!!\n");
				}
				DBG("output file name : %s\n", mux_output_filename);

				Mux_TCC_MP4_EncFrame(priv->mp4_handle, STREAM_VIDEO, priv->Output.outputStreamAddr, priv->Output.headerLen, keyframe, 0);

				sum_of_output = 0;
				source_size = priv->width * priv->height * 3/2;

				priv->encoder_init = 1;
			}

			priv->frame_count++;
			if(f_row != -1)
			{
				tmp_count++;
				if(tmp_count == 25)
				{
					f_column = 0;
					tmp_count = 0;
				}
				else if(frame_pass[f_row][f_column] && tmp_count == frame_pass[f_row][f_column])
				{
					f_column++;
					continue;
				}
			}

			if(priv->base_time == 0)
			{
				priv->base_time = ((unsigned long long)(priv->camera.buf.timestamp.tv_sec*1000000 + priv->camera.buf.timestamp.tv_usec) / 1000);
			}

			priv->Input.inputStreamAddr	= (unsigned char *)priv->camera.mem[priv->camera.buf.index].phy_addr;//ENCODER_BITSTREAM_BUFF_ADDR(enc_context, ADDR_PHYSICAL); //Physical Address
			priv->Input.nTimeStamp		= ((unsigned long long)(priv->camera.buf.timestamp.tv_sec*1000000 + priv->camera.buf.timestamp.tv_usec) / 1000) - priv->base_time;
			priv->Input.inputStreamSize = source_size;		
			priv->Input.isForceIFrame = 0; 
			priv->Input.isSkipFrame = 0; 
			priv->Input.frameRate = priv->init.frameRate;
			priv->Input.targetKbps = priv->init.targetKbps; 
			priv->Input.Qp = priv->init.bEncodingQualityLevel; 
			priv->Input.muxType			= 1; // mp4

			while(1)
			{
				ret = ENCODER_ENC(priv->enc_context, &priv->Input, &priv->Output);
				if(ret < 0)
				{
					DBG("ENCODER_ENC FAILED ret = %d\n", ret);
					usleep(100 * 1000);
					continue;
				}
				else
				{
					break;
				}
			}

			sum_of_output += priv->Output.frameLen;

			if(priv->Output.picType == TYPE_I_FRAME)
				keyframe = 1;
			else
				keyframe = 0;

			Mux_TCC_MP4_EncFrame(priv->mp4_handle, STREAM_VIDEO, priv->Output.outputStreamAddr, priv->Output.frameLen, keyframe, priv->Input.nTimeStamp);

			if(priv->frame_count > frame_sum || init_info->recoder_finish)
			{
				package_video(priv);
				system("sync");
				init_info->recoder_finish = 0;
				video_start_time = tmp_video_time;
				if(video_cmd_flag)
				{
					if(video_cmd_flag == 1)
						video_finish = 1;
					else
						video_finish = 2;

					init_info->recoder_flag = 0;
					video_cmd_flag = 0;
				}
				else
					video_finish = 2;

				printf("now time %d\n",time(NULL));
				char tmp_outfile[128] = "";
				int time_len = (time(NULL) - video_start_time + 30)/60; /* The Length of time is rounded to 5. */
				char tmp[8];
				char cmd[256];

				sprintf(tmp, "%02d.mp4", time_len);
				strncpy(tmp_outfile, mux_output_filename, strlen(mux_output_filename) - 4);
				strcat(tmp_outfile, tmp);

				sprintf(cmd,"mv %s %s ",mux_output_filename,tmp_outfile);
				printf("=== %s ===\n", cmd);
				system(cmd);
				strcpy(video_name, tmp_outfile);
			}
		}

		if(!priv->capture_flag && !priv->preview_flag && !priv->recoder_flag)
		{
			usleep(50*1000);
		}
	}while(!bForceExit);

FORCE_RETURN:	
	if(priv->enc_context != NULL)
	{
		ENCODER_CLOSE(priv->enc_context);
		priv->enc_context = NULL;
	}

	if(priv->mp4_handle != NULL)
	{
		Mux_TCC_MP4_EncClose(priv->mp4_handle);
		priv->mp4_handle = NULL;
	}

	tcc_lcd_deinit();		

	tcc_v4l2_end_stream(&priv->camera);
	tcc_v4l2_free_mem(&priv->camera);
	tcc_v4l2_close(&priv->camera);

	priv->encoder_init = 0;
	bForceExit = 1;
	DBG("ENCODER_CLOSE \n");

	return NULL;
}

void camera_uninit()
{
	free(init_info);
}
void preview_output_set(int full)
{
	if(1 == full){

		init_info->show_pos_x = 0;
		init_info->show_pos_y = 0;
		init_info->scaler_dst_width = LCD_WIDTH;
		init_info->scaler_dst_height = LCD_HEIGHT;

	}else if(0 == full){

		init_info->show_pos_x = DEF_POS_X;
		init_info->show_pos_y = DEF_POS_Y;
		init_info->scaler_dst_width = DEF_DST_W;
		init_info->scaler_dst_height = DEF_DST_H;

	}else{

		printf("screen full set error...\n");

	}

}

int camera_preview()
{
	//fb_clear();
	printf("## camera_preview ##\n");
	init_info->preview_flag = 1;
	usleep(50*1000);

	return 0;
}

int camera_delay()
{
	printf("## camera_delay ##\n");
	init_info->preview_flag = 0;
	usleep(50*1000);
	tcc_overlay_disable();
	usleep(50*1000);
	//init_info.capture_flag = 0;
	//init_info.recoder_flag = 0;
	return 0;
}

int camera_video(void *video_ptr)
{
	if(init_info->recoder_flag)
		return -1;

	if(video_ptr != NULL)
	{
		memcpy(&videoinfo, video_ptr, sizeof(videoinfo));
		video_cmd_flag = 1;
	}
	else
	{
		video_cmd_flag = 0;
		videoinfo.m_channel = 5;
	}

	init_info->recoder_flag = 1;
	init_info->recoder_finish = 0;
	init_info->encoder_init = 0;

	return 0;
}

int camera_cap(int resolution, int no)
{
	if(resolution < 0 || resolution > 8)
		return -1;

	dev.resolution = resolution;
	init_info->capture_flag = 1;
	dev.pic_no = no;

	return 0;    
}

unsigned long my_time()
{
	return time(NULL);
}

void camera_video_recovery()
{
	init_info->recoder_flag = 1;
	init_info->recoder_finish = 0;
}

int camera_init(void *check_video_type)
{
	/* memory device open */
	mem_fd = open("/dev/mem", O_RDWR | O_NDELAY);
	if(mem_fd < 0)
	{
		DBG("Mem dev open error!!!\n");
		return -1;
	}

	pmap_get_info("fb_scale", &pmap_scaler);
	scaler_map_ptr = (unsigned char*)mmap(0, pmap_scaler.size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, pmap_scaler.base);
	if(scaler_map_ptr == MAP_FAILED)
	{
		DBG("mem failed!!\n");
		goto err_map_scaler;
	}

	scaler_fd = -1;
	if(scaler_fd)
	{
		scaler_fd = tcc_video_scaler_open(TCC_SCALER_DEV1_NAME);
		if(scaler_fd < 0)
			goto err_open_scaler;
	}

	struct encoder_init_info *info = malloc(sizeof(*info));
	if(NULL == info)
	{
		printf("may be no mem for info!\n");
		return -1;
	}
	memset(info, 0x0, sizeof(*info));
	init_info = info;

	init_info->width = CAMERA_WIDTH;
	init_info->height = CAMERA_HEIGHT;
	init_info->codec_format = CODEC_FORMAT_H264;
	init_info->encoding_quality_level = 11;
	//init_info->user_bitrate = 10485760;
	init_info->user_bitrate = 400*1024;
	init_info->preview_flag = 0;

	init_info->show_pos_x = 0;
	init_info->show_pos_y = 0;
	init_info->scaler_dst_width = LCD_WIDTH;
	init_info->scaler_dst_height = LCD_HEIGHT;



	int ret = pthread_create(&init_info->v4l2_thread, NULL, enc_thread, init_info);
	if(ret < 0)
	{
		printf("can not create enc_thread!\n");
		goto err_thread;
	}
	pthread_detach(init_info->v4l2_thread);

	return 0;

err_thread:
	close(mem_fd);
err_open_scaler:
	munmap(scaler_map_ptr, pmap_scaler.size);
err_map_scaler:
	close(scaler_fd);

	return -1;
}

int camera_video_test(void *cmd)
{
	char *ptr = (char *)cmd;

	if(strcmp(ptr, "exit\n") == 0)
	{
		bForceExit = 1;
		close(mem_fd);
		munmap(scaler_map_ptr, pmap_scaler.size);
		close(scaler_fd);
		DBG("===> Exit!!\n");
		DBG("encoder test ended!!\n");
		return -1;
	}
	else if(strcmp(ptr, "video\n") == 0)
	{
		if(init_info->recoder_flag)
			DBG("has already into video status\n");
		else
		{
			video_cmd_flag = 1;
			videoinfo.m_interval = 40;
			videoinfo.m_framerate = 10;//10 12 13 20 25 
			printf("Please input frameRate:");
			int frame;
			scanf("%d", &frame);
			videoinfo.m_framerate = frame;
			videoinfo.m_channel = 5;
			init_info->recoder_flag = 1;
			init_info->recoder_finish = 0;
			DBG("===> Video!!\n");
		}
	}
	else if(strcmp(ptr, "stop\n") == 0)
	{
		if(init_info->recoder_flag)
		{
			init_info->recoder_flag = 0;
			init_info->recoder_finish = 1;
		}

		if(init_info->preview_flag)
			init_info->preview_flag = 0;

		if(init_info->capture_flag)
			init_info->capture_flag = 0;
		DBG("===> Stop!!\n");
	}

	return 0;
}

int camera_cap_video_stop(int mode)
{
	if(mode == 1)
	{
		init_info->recoder_finish = 1;
		video_cmd_flag = 1;
	}
	else if(mode == 2)
	{
		init_info->recoder_finish = 1;
		video_cmd_flag = 2;
	}
	else if(mode == 3)
	{
		init_info->capture_flag = 0;
	}
	else
		return -1;

	return 0;
}

int camera_set_video_info(int framerate, int duration)
{
	if(framerate != 10 && framerate != 15 && framerate != 25)
		return -1;

	gui_framerate = framerate;
	gui_duration = duration;

#ifdef STOP_VIDEO_AFTER_SET_VIDEO
	camera_cap_video_stop(video_cmd_flag);
	usleep(150000);
	camera_video(NULL);
#endif

	return 0;
}

