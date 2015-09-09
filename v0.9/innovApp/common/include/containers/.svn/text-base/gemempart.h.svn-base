/*
 * Memory partion using malloc()/free()/realloc() like
 * API.
 *
 * User typically first calls mempart_create() to create
 * a memory partition (or heap) and then calls
 * mempart_malloc() and mempart_free() using that memPart.
 * Additionally, memory can be added an existing memory
 * partition by calling mempart_add().  By calling
 * mempart_add(), a memPart can be made of several memory
 * regions that don't necessarily have to be contiguous.
 *
 * Memory is allocated using a first-fit algorithm.
 *
 * Author: Dominique Pelle <dpelle@21com.com>
 */
#ifndef _GEMEMPART_H_
#define _GEMEMPART_H_

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef INT64 TAlign;

struct TMemPartBlock;

/* Header to store information global to the entire memPart */
typedef struct TMemPart {
  union {
    TAlign align;            /* unused, only there to align
                              * | structure */
    struct {
      struct TMemPartBlock *firstFree; /* point to first free block */
    } s;
  } u;
} TMemPart;

/* see gemalloc.c for documentation */
extern MAPCORE_API
TMemPart *memPart_create(void *buffer, INT32 sz);

extern MAPCORE_API
void memPart_destroy(TMemPart *memPart);

extern MAPCORE_API
void memPart_add(TMemPart *memPart, void *buffer, INT32 sz);

extern MAPCORE_API 
void *memPart_malloc(TMemPart *memPart, INT32 sz);

extern MAPCORE_API
void *memPart_realloc(TMemPart *memPart, void *ptr, INT32 sz);

extern MAPCORE_API
void memPart_free(TMemPart *memPart, void *ptr);

extern MAPCORE_API
void memPart_show(const TMemPart *memPart);

#ifdef __cplusplus
}
#endif

#endif

