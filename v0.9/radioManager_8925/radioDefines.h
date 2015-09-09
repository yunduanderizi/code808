#ifndef RADIODEFINES_H
#define RADIODEFINES_H



#define RM_FM_FREQ_STEP	100
#define RM_AM_FREQ_STEP 9
#define RM_R_OK			0

#define RM_FM_FREQ_MIN	       87500
#define RM_FM_FREQ_MAX	108000

//需要升级厂家收音机模块的升级包
#define RM_AM_FREQ_MIN	522 //531->522 [zhenyubin 2015/03/18]
#define RM_AM_FREQ_MAX	1620 //1629->1620 [zhenyubin 2015/03/18] 

#define RM_SEARCH_R_OK		0
#define RM_SEARCH_R_CANCEL	1


#define RM_FREQ_SECTION_FM	0
#define RM_FREQ_SECTION_AM	1
#define RM_FREQ_SECTION_NULL -1

#define RM_R_ERROR_OPEN		-1
#define RM_R_ERROR_RANGE	-2
#define RM_R_ERROR_MODE		-3

#endif // RADIODEFINES_H
