/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_memory_enallocP_h
#define INCLUDE_GUARD_memory_enallocP_h

#include <platform_types.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define MEMORY_ALLOC_HINT_PROCESS_MEMORY ((void*)0x00000000)
#define MEMORY_ALLOC_HINT_SHARED_MEMORY  ((void*)0xFFFFFFFF)

/* This function should only be called from engine_init. */
BOOL8 memory_init(void);

/* This function should only be called from engine_shutdown. */
void memory_shutdown(void);

void *engine_malloc(UINT32 size, void *hint);
void *engine_realloc(void *ptr, UINT32 size, void *hint);
void  engine_free(void *ptr);

#if defined(__cplusplus)
} //extern "C"
#endif

#endif /*INCLUDE_GUARD_memory_enallocP_h*/
