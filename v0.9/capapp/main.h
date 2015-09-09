#ifndef __CAM_MAIN_H__
#define __CAM_MAIN_H__

#define LCD_WIDTH	800
#define LCD_HEIGHT	480
struct video_info
{
    int m_channel;
    int m_command; 
    int m_interval; // video interval    0:Repeated video
    int m_flag; // 1:save 0:upload
    int m_resolution;
        /* 0x01:320*240£» 0x02:640*480£» 0x03:800*600£» 0x04:1024*768; 
           0x05:176*144;[Qcif];  0x06:352*288;[Cif];  0x07:704*288;[HALF D1];  0x08:704*576;[D1]; */
           
    int m_quality; // 1-10
    int m_brightness; // 0-255
    int m_saturation; // 0-127
    int m_chroma; // 0-255
    int m_framerate;
    int m_event; //0:command  video    1: video all the time
};

int camera_init(void *check_video_type);
void camera_uninit();
int camera_preview();
int camera_delay();
int camera_video(void *video_ptr);
int camera_cap(int resolution, int no);
void camera_video_recovery();
int camera_video_test(void *cmd);
int camera_set_video_info(int framerate, int duration);
int camera_cap_video_stop(int mode);
unsigned long my_time();
void preview_output_set(int full);
#endif
