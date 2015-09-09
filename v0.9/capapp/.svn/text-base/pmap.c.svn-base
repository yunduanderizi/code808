/******************************************************************************
* include 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "pmap.h"



/******************************************************************************
*	FUNCTIONS			: pmap_get_info
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
int pmap_get_info(const char *name, pmap_t *mem)
{
    int fd;
    int matches;
    char buf[2048];
    const char *p;
    ssize_t nbytes;
    unsigned int base_addr;
    unsigned int size;
    char s[128];

    fd = open(PATH_PROC_PMAP, O_RDONLY);
    if (fd < 0)
	return 0;

    nbytes = read(fd, buf, sizeof(buf));
    close(fd);

    p = buf;
	while (1) 
	{
	matches = sscanf(p, "0x%x 0x%x %s", &base_addr, &size, s);
		if (matches == 3 && !strcmp(name, s)) 
		{
	    //printf("requested physical memory '%s' (base=0x%x size=0x%x)\n", name, base_addr, size);
	    mem->base = base_addr;
	    mem->size = size;
	    return 1;
	}
	p = strchr(p, '\n');
	if (p == NULL)
	    break;
	p++;
    }
    return 0;
}
