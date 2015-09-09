#ifndef GEARRAYUTILS_H
#define GEARRAYUTILS_H

#include <memory/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Duplicate an array, possibly re-allocating an existing block of memory
    allocated with the same allocator.
    If size is 0 and dst is non-NULL, dst will be free'd.

    @param src The source data to copy from.

    @param size The number of bytes to copy.

    @param allocator An allocator handle to use for memory de/allocation.

    @param dst An existing block of memory allocated with #allocator that
               should be reallocated or freed.

    @param dstName A string to use in any log statements if an error occurs.
*/
void* arrayDup(const void *src,
               UINT32 size,
               TAllocatorHandle allocator,
               void *dst, 
               const char *dstName);

#ifdef __cplusplus
}
#endif

#endif // GEARRAYUTILS_H
