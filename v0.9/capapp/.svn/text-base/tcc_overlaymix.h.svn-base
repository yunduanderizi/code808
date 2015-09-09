#ifndef __TCC_OVERLAYMIX__
#define __TCC_OVERLAYMIX__

#include "tcc_grp_ioctrl.h"
#include "tcc_gre2d_type.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TCC_GRAPHIC_DEV_NAME	"/dev/graphic"

typedef struct
{
	int		overlaymix_fd;
//	pmap_t	g2d_mem;

}TCC_OVERLAYMIXER_Struct_t;

int tcc_overlaymix_init(void);
void tcc_overlaymix_deinit(int olymix_fd);
int tcc_overlaymix_rotate(unsigned int g2d_fd, unsigned int rotate_angle, 
									char* src_addr, int src_x, int src_y, int src_w, int src_h, 
									int src_fmt, int dst_fmt, char* dst_buffer);


#endif /* __TCC_OVERLAYMIX__ */
