/*----------------------------------------------
 *
	LCD Output
----------------------------------------------*/
#if 0
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
//#include "lcdc_ctrl.h"
#include <linux/fb.h>
//#include "M2Mscaler_Video.h"
#include "tcc_linux_system.h"

#include "tcc_overlay_interface.h"
#include "tcc_scaler_interface.h"
#include "tcc_overlaymix.h"
#include "pmap.h"

//#define USE_VIQE

#if 0
static int giLcdFb = 0;
static int giLcdWidth = 0;
static int giLcdHeight = 0;
static int giLcdStride = 0;
static int giLcdMapSize = 0;
static void *gpLcdFb = 0;
#endif

//static int viqe_fd = 0;
static int scaler_fd = 0;
static int overlay_fd = 0;
static int fb_fd = 0;
static int mem_fd = 0;
static int g2d_fd = 0;

void *overlay_map_ptr; 
void *viqe_map_ptr;
void *g2d_map_ptr; 

static overlay_config_t overlay_info;

static int sequence_type; //progress:0/interlaced:1
static int first_frame_ignore = 0;

static pmap_t pmap_overlay;
//static pmap_t pmap_viqe;
static pmap_t pmap_g2d;


struct fb_var_screeninfo info;

static tcc_scaler_info_t scaler_src;
static tcc_scaler_info_t scaler_dst;
static overlay_info_t overlay;

static unsigned long source_format = 0;

#define NB_FB		2
static int 	glcd_double_buf_address[NB_FB];
static int curr_fb = 0;

//static int viqe_initialized = 0;

static int lcd_width;
static int lcd_height;
static int video_width;
static int video_height;

#define PATH_PROC_PMAP	"/proc/pmap"

extern void* get_virtual_addr(unsigned int physical_addr, unsigned int mem_size);

static void clear_fb()
{
	unsigned short *ptr;
	int i,j;

	ptr = overlay_map_ptr;

	for(i=0;i<=overlay_info.height;i++)
		for(j=0;j<=overlay_info.width;j++)
			*ptr++=0x8000;
	ptr = overlay_map_ptr + (1920 * 1080 * 2);
	for(i=0;i<=overlay_info.height;i++)
		for(j=0;j<=overlay_info.width;j++)
			*ptr++=0x8000;
}
int tcc_lcd_init()
{
	int ii;
	
	scaler_fd = tcc_video_scaler_open(TCC_SCALER_DEV0_NAME);
	if(scaler_fd < 0)
	{
		printf("can't open %s\n", TCC_SCALER_DEV0_NAME);
		return -1;
	}
	printf("scaler open succeed!!\n");
	
	overlay_fd = open(TCC_OVERLAY0_DEV_NAME, O_RDWR | O_NDELAY);
	if(overlay_fd < 0)
	{
		printf("can't open %s\n", TCC_OVERLAY0_DEV_NAME);
		return -1;
	}
	printf("overlay open succeed!!:%d\n", overlay_fd);
	
	fb_fd = open("/dev/fb0", O_RDWR);
	if(fb_fd < 0)
	{
		printf("can't open %s\n", "/dev/fb0");
		return -1;
	}
	printf("fb open succeed!!\n");

	mem_fd = open("/dev/mem", O_RDWR | O_NDELAY);
	if (mem_fd < 0)
	{
		printf("can't open %s\n", "/dev/mem");
		return -1;
	}
	printf("mem open succeed!!\n");

#ifdef USE_VIQE
	viqe_fd = tcc_viqe_open();
	if(viqe_fd < 0)
	{
		printf("can't open %s\n", "VIQE");
		return -1;
	}

	printf("viqe open succeed!!\n");

	pmap_get_info("viqe", &pmap_viqe);
	viqe_map_ptr = (unsigned char*)mmap(0, pmap_viqe.size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, pmap_viqe.base);
	if(viqe_map_ptr == MAP_FAILED)
	{
		printf("pmap error viqe\n");
		return -1;
	}

	printf("viqe base:0x%x, size:%d\n", pmap_viqe.base, pmap_viqe.size);
#endif

	//overlay buffer 
	pmap_get_info("overlay", &pmap_overlay);
	overlay_map_ptr = (unsigned char*)mmap(0, pmap_overlay.size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, pmap_overlay.base);
	if(overlay_map_ptr == MAP_FAILED)
	{
		//DEBUG (DEB_LEV_ERR, "mmap failed. fd(%d), addr(0x%x), size(%d)\n", omx_fbdev_sink_component_Private->memory_fd, omx_fbdev_sink_component_Private->pmap_overlay.base, omx_fbdev_sink_component_Private->pmap_overlay.size);
		printf("pmap error overlay\n");
		return -1;
	}


	if(ioctl(overlay_fd, OVERLAY_GET_SCREEN_INFO, &overlay_info) == -1)
	{
		printf("overlay_get_info error\n");
		return -1;
	}

	//printf("overlay info=>sx:%ld, sy:%ld w:%ld, h:%ld, fmt:%ld, trans:%ld\n", overlay_info.sx, overlay_info.sy, overlay_info.width, overlay_info.height, overlay_info.format, overlay_info.transform);
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &info);


	overlay_info.format = V4L2_PIX_FMT_YUV422P;
	if(ioctl(overlay_fd, OVERLAY_SET_CONFIGURE, &overlay_info) == -1)
	{
		printf("overlay configure error\n");
		return -1;
	}

	for(ii=0; ii<NB_FB; ii++)
	{
		glcd_double_buf_address[ii] = pmap_overlay.base + (1920 * 1080 * 2) * ii;
		printf("fb[%d]:0x%x\n", ii, glcd_double_buf_address[ii]);
	}

	lcd_width = (overlay_info.width & 0xfffffff0);
	lcd_height = (overlay_info.height & 0xfffffff0);


	g2d_fd = tcc_overlaymix_init();
	pmap_get_info("fb_g2d0", &pmap_g2d);
	g2d_map_ptr = (unsigned char*)mmap(0, pmap_g2d.size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, pmap_g2d.base);
	if(g2d_map_ptr == MAP_FAILED)
	{
		//DEBUG (DEB_LEV_ERR, "mmap failed. fd(%d), addr(0x%x), size(%d)\n", omx_fbdev_sink_component_Private->memory_fd, omx_fbdev_sink_component_Private->pmap_overlay.base, omx_fbdev_sink_component_Private->pmap_overlay.size);
		printf("pmap error gd\n");
		return -1;
	}
	printf("g2d open succeed!!\n");
	
	printf("LCCD open succeed\n");
	return fb_fd;
}



void tcc_lcd_deinit()
{
	//clear_fb();
	munmap(overlay_map_ptr, pmap_overlay.size);
	close(overlay_fd);
	close(fb_fd);
	close(mem_fd);
	tcc_video_scaler_close(scaler_fd);
#ifdef USE_VIQE	
	tcc_viqe_close(viqe_fd);
#endif	
	tcc_overlaymix_deinit(g2d_fd);
	
	printf("lcd close\n");
}
void  tcc_overlay_disable()
{
	tca_overlay_disable(overlay_fd);
	clear_fb();
}

void tcc_set_video_output(
	unsigned int uiOutWidth, 
	unsigned int uiOutHeight, 
	unsigned int uiSrcStride,
	unsigned int uiDstStartH,
	unsigned int uiDstStartV,
	unsigned int uiDstWidth,
	unsigned int uiDstHeight,
	unsigned int uiSrcFormat,
	int interlaced)
{
   // printf("%d,%d,%d,%d,%d,%d,%d\n", uiOutWidth, uiOutHeight, uiSrcStride,
     //   uiDstStartH, uiDstStartV, uiDstWidth, uiDstHeight);
	
	

	source_format = uiSrcFormat;
	video_width = uiOutWidth;
	video_height = uiOutHeight;
	
	scaler_src.w_x = 0;
	scaler_src.w_y = 0;
	scaler_src.w_w = uiOutWidth;
	scaler_src.w_h = uiOutHeight;

	scaler_src.x = 0;
	scaler_src.y = 0;
	scaler_src.w = uiOutWidth;
	scaler_src.h = uiOutHeight;

	//scaler_dst.w_x = 0;
	//scaler_dst.w_y = 0;
	//scaler_dst.w_w = uiDstWidth;
	//scaler_dst.w_h = uiDstHeight;

	scaler_dst.x = 0;
	scaler_dst.y = 0;
    scaler_dst.w = uiDstWidth;
	scaler_dst.h = uiDstHeight;


	overlay.x = uiDstStartH;
	overlay.y = uiDstStartV;
	overlay.w = uiDstWidth;// > lcd_width ? lcd_width : uiDstWidth;
	overlay.h = uiDstHeight;// > lcd_height ? lcd_height : uiDstHeight;

	//if(video_codec == STD_WMV78)
	//	scaler_src.format = SCALER_YUV420_sp; //WMV78 output only yuv420sp
	//else

#ifdef USE_VIQE
	if(interlaced > 0 && viqe_initialized == 0)
	{
		viqe_initialized = 1;
		sequence_type = 1;
		tcc_viqe_initialize(viqe_fd, 0, 1, uiOutWidth, uiOutHeight);
		printf("\nVIQE Initialized Succeed!!\n");
	}
	else if(interlaced == 0)
#endif	
	{
		sequence_type = 0;
	}

	//printf("output setting : src w:%d, h:%d, sequence_type:%d\n", uiOutWidth, uiOutHeight, sequence_type);
}

void tcc_run_video_output(
	void * pSrcY, 
	void * pSrcU, 
	void * pSrcV,
	void * pPrevY,
	void * pPrevU,
	void * pPrevV)
{
   if(!pPrevY || !pPrevU || !pPrevV)
   {
        //printf("maybe NULL!\n");
   }
        
	unsigned int frame_buffer;

	frame_buffer = glcd_double_buf_address[curr_fb];
	curr_fb = (curr_fb + 1) % NB_FB;

#ifdef USE_VIQE
	if(sequence_type && first_frame_ignore)
	{
		int width, height;
		int address[6] = {pSrcY, pSrcU, pSrcV, pPrevY, pPrevU, pPrevV};

		width = (video_width+15)&0xfffffff0;
		height = (video_height+1)&0xfffffffe;

		tcc_viqe_excuting(viqe_fd, 
							0, // 1: on-the-fly, 0: normal
							1, // LCD output 
							0, // odd first ?
							width,
							height, 
							address, 
							pmap_viqe.base);
							
		scaler_src.addr_y = (char*)pmap_viqe.base;
		scaler_src.addr_u = NULL;
		scaler_src.addr_v = NULL;

		scaler_src.format = SCALER_YUV422_sq0; 
	}
#endif
	{
		scaler_src.addr_y = pSrcY;
		scaler_src.addr_u = pSrcU;
		scaler_src.addr_v = pSrcV;

		if(source_format == V4L2_PIX_FMT_YVU420)
			scaler_src.format = SCALER_YUV420_sp; 
		else if(source_format == V4L2_PIX_FMT_NV12)
			scaler_src.format = SCALER_YUV420_inter; 
	}

	if(first_frame_ignore == 0)
		first_frame_ignore = 1;

	scaler_dst.addr_y = frame_buffer;//(char*)pmap_g2d.base; //frame_buffer;
	scaler_dst.addr_u = NULL;
	scaler_dst.addr_v = NULL;

#if 0	
	scaler_dst.x = 0;
	scaler_dst.y = 0;
	scaler_dst.w = lcd_height;
	scaler_dst.h = lcd_width;
#endif
	scaler_dst.format = SCALER_YUV422_sq0; //422se0 해야지만 422p로 나온다.

	if(tcc_video_scaler_execute(scaler_fd, &scaler_src, &scaler_dst) < 0)
		printf("scaler error\n");

	//tcc_overlaymix_rotate(g2d_fd, NOOP, scaler_dst.addr_y, 0, 0, scaler_dst.w, scaler_dst.h, GE_YUV422_sq, GE_YUV422_sq, (char *)frame_buffer);
	
#if 0
	overlay.x = 0;
	overlay.y = 0;
	overlay.w = 360;//lcd_width;
	overlay.h = 288;//lcd_height;
#endif

	overlay.addr_y = (char *)frame_buffer;
	overlay.addr_u = NULL;
	overlay.addr_v = NULL;

	//printf("\noverlay_fd:%d, addr_y:0x%x, w:%d, h:%d\n", overlay_fd, overlay.addr_y, lcd_width, lcd_height);

	tcc_overlay_set_video_update(overlay_fd, &overlay);

#if 0
	//for dump
	if(0)
	{
		unsigned char* va_y;
		FILE* fp = fopen("/nand2/scaler_out_dump.yuv", "a");

		va_y = (unsigned char*)get_virtual_addr(scaler_dst.addr_y, lcd_width * lcd_height);
		fwrite(va_y, 1, lcd_width * lcd_height, fp);
		
		fclose(fp);

		printf("scaler out write to file\n");
	}

	if(1)
	{
		unsigned char* va_y;
		FILE* fp = fopen("/nand2/vpu_out_dump.yuv", "a");

		va_y = (unsigned char*)get_virtual_addr(scaler_src.addr_y, scaler_src.w * scaler_src.h);
		if(va_y)
		{
			fwrite(va_y, 1, scaler_src.w * scaler_src.h, fp);
			printf("vpu out write to file\n");
		}
		
		fclose(fp);
	}
#endif
}


