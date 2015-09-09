#ifndef __TCC_LINUX_SYSTEM__
#define __TCC_LINUX_SYSTEM__

/*----------------------------------------------
	LCD Output
----------------------------------------------*/

int tcc_lcd_init();

void tcc_lcd_deinit();
void tcc_overlay_disable();
void tcc_set_video_output(
	unsigned int uiOutWidth, 
	unsigned int uiOutHeight, 
	unsigned int uiSrcStride,
	unsigned int uiDstStartH,
	unsigned int uiDstStartV,
	unsigned int uiDstWidth,
	unsigned int uiDstHeight,
	unsigned int uiSrcFormat,
	int interlaced);

void tcc_run_video_output(
	void * pSrcY, 
	void * pSrcU, 
	void * pSrcV,
	void * pPrevY,
	void * pPrevU,
	void * pPrevV);

#endif //__TCC_LINUX_SYSTEM__
