/****************************************************************************
 *   FileName    : tcc_scaler_interface.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
#ifndef __TCC_SCALER_INTERFACE_H_
#define __TCC_SCALER_INTERFACE_H_

#include "tcc_scaler_ioctrl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TCC_SCALER_DEV0_NAME 		"/dev/scaler"
#define TCC_SCALER_DEV1_NAME 		"/dev/scaler1"

typedef struct tagScalerInfo
{
	//image address
	char* addr_y;
	char* addr_u;
	char* addr_v;

	//image size
	long x;
	long y;
	long w;
	long h;

	//window size
	long w_x;
	long w_y;
	long w_w;
	long w_h;

	//image format
	int format;
}tcc_scaler_info_t;


enum{
	SCALER_MEMORY_NO_ERROR = 0,
	SCALER_MEMORY_OPEN_ERR,	
	SCALER_MEMORY_MALLOC_ERR,
	SCALER_MEMORY_REG_ERROR, 
	SCALER_INTERRUPT_OPEN_ERROR, 
	SCALER_INTERRUPT_POLL_ERROR, 		
	SCALER_INTERRUPT_POLLTIME_ERROR, 	
	SCALER_INTERRUPT_POLLERR_ERROR 	
};

int tcc_video_scaler_open(const char* dev_name);

int tcc_video_scaler_execute(const int fd, tcc_scaler_info_t* src, tcc_scaler_info_t* dst);

int tcc_video_scaler_close(const int fd);

#endif /* __TCC_SCALER_INTERFACE_H_ */
