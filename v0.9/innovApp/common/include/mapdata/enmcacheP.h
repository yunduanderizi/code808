#ifndef ENMCACHEP_H
#define ENMCACHEP_H

// This module provides memory allocation functions that maintain a
// set of memory blocks that can be in one of three states.
// STATE_FREE - these block contain no data and can be joined, split,
// and moved at will.
// STATE_FLOATING - these blocks contain data but can be moved around
// by the cache provided that it uses the mapcache_notifyBlock function
// to inform other parties of the changing location of the blocks.
// Floating blocks can also be flushed from memory if more cache memory is
// allocated.
// STATE_LOCKED - blocks of this type are created by the mapcache_malloc
// function and must stay in the same location until mapcache_free is
// called.

#include <mapdata/enmcache.h>
#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Use statistics counters only on simulator (useless overhead on target) */
#ifdef SIMULATOR
#  define HAS_STATS
#endif

#define NUM_MAPCACHE_HASH_BUCKETS ((UINT16)4096) /* should be a power of 2 */
#define MAPCACHE_MIN_SIZE_TO_SUPPORT_LANDMARKS ((UINT32)(6*1024*1024))

struct TGenericMapInfo; // forward declaration

/*----------------------------------------------------------------*/

#ifdef  HAS_STATS
/* Statistics counter for performance analysis */
typedef struct TMapCacheStats {
  UINT32 biggestBlock;
  UINT32 smallestBlock;
  UINT32 staticAllocCount;
  UINT32 full;
  UINT32 maxBlocks;
  UINT32 compactCount;
  UINT32 lockCount;
  UINT32 freeFloatingBlock;
  UINT32 lockedUnitCount;
  UINT32 floatingUnitCount;
  UINT32 chunkMallocCount;
  UINT32 mallocCount;
  UINT32 freeCount;
} TMapCacheStats;
#endif

// all memory that is maintained by the map cache is in one of 3 states
// and is contained in one of 3 lists
typedef enum {
  STATUS_FREE = 0,
  STATUS_FLOATING,
  STATUS_LOCKED,
  // leave this at the end
  STATUS_MAX
} TMapCacheStatus;

// #define DEBUG_USE_MAPCACHE_GUARD

// this block header comes before all portions of memory that are under the
// control of the cache manager
typedef struct TMapCacheBlock {
  // these elements are required for the tree functions
  struct TMapCacheBlock *previousItem;
  struct TMapCacheBlock *nextItem;

#ifdef DEBUG_USE_MAPCACHE_GUARD
  UINT32  guard;
#endif

  // these elements are used for the memory management
  struct TMapCacheBlock *previous; // the block before this one (in memory)
  UINT32 size;                     // size of block in units (including the block header)
  UINT32 dataSize;                 // actual size of stored data in bytes
  struct TGenericMapInfo *mapInfo; // map that this block belonged to
  UINT32 chunkIndex:16;            // the chunk index of the chunk concerned
  UINT32 lockCount:4;              // the number of locks on this block
  UINT32 status:4;                 // status of this block (number of the memory list it is contained in)
  UINT32 spare:8;

  struct TMapCacheBlock *hashLink; // to chain blocks belonging to the same hash lookup bucket.
} TMapCacheBlock;

#define MAPCACHE_MAX_LOCKCOUNT ((UINT32)0xF)

// If the address is not zero, grab the lock count from the
// cache block header.
// If the address is zero, then the lock count is zero.
#define MAPCACHE_LOCKCOUNT(pAddress)                                    \
  ((pAddress)?((((TMapCacheBlock *)(pAddress)) - 1)->lockCount) : 0)

#define MAPCACHE_SET_LOCKCOUNT(pAddress, value) do {    \
  TMapCacheBlock *_block;                               \
                                                        \
  assert((pAddress) != 0);                              \
  assert((UINT32)value <= MAPCACHE_MAX_LOCKCOUNT);      \
  _block = ((TMapCacheBlock *)(pAddress)) - 1;          \
  _block->lockCount = value;                            \
} while (0)

#define MAPCACHE_INC_LOCKCOUNT(pAddress) do {           \
  TMapCacheBlock *_block;                               \
                                                        \
  assert((pAddress) != 0);                              \
  _block = ((TMapCacheBlock *)(pAddress)) - 1;          \
  assert(_block->lockCount < MAPCACHE_MAX_LOCKCOUNT);   \
                                                        \
  _block->lockCount++;                                  \
} while (0)

#define MAPCACHE_DEC_LOCKCOUNT(pAddress) do {   \
  TMapCacheBlock *_block;                       \
                                                \
  assert((pAddress) != 0);                      \
  _block = ((TMapCacheBlock *)(pAddress)) - 1;  \
  assert(_block->lockCount > 0);                \
                                                \
  _block->lockCount--;                          \
} while (0)


/*----------------------------------------------------------------*/
typedef enum {
  BLOCK_LINK,
  BLOCK_UNLINK,
  BLOCK_UNLOAD
} TBlockNotifyType;

/* This function is called by the map cache manager when it is freeing
 * memory blocks or moving them to other locations.  If data is NULL
 * then the memory block corresponding to mapHeader and chunk index is
 * no longer available.  If data is != NULL then the memory block
 * corresponding to map header and chunk index is available at the
 * location indicated by data.
 */
extern void mapcache_notifyBlock(TMapCacheBlock   *block,
                                 TBlockNotifyType  notifyType,
                                 const void       *data);

/*----------------------------------------------------------------*/

/* This function looks up a block in the mapcache based on the owning
 * mapInfo and chunkIndex. Should the block be absent, it returns NULL. */
UINT8 *mapcache_lookup(const struct TGenericMapInfo *const mapInfo,
                       const UINT16 chunkIndex);

// this function moves the cache block concerned from the locked list
// to the floating list.  This means that the location of the data can be
// altered provided that the mapcache_notifyBlock callback function is
// used to notify other parties prior to and after changing a blocks location.
void mapcache_unlock(void *ptr);

// this function moves an existing floating cache block from the floating list
// to the locked list.  This means that the location of the data will not be
// altered.  The block can be moved back to the floating list by calling
// the mapcache_free function.
void mapcache_lock(void  *cp);

void *mapcache_chunkMalloc(UINT32 nbytes,
                           struct TGenericMapInfo *const mapInfo,
                           UINT32 chunkIndex);
void mapcache_chunkFree(void *ptr);

UINT32 mapcache_getDataSize(void *dataBlock);
UINT32 mapcache_getTotalSize(void);

// Calls an underlying os function to get the size and location
// of map cache memory.  Returns TRUE if the memory was successfully
// obtained.  The memory location and size are stored internally.
BOOL8 mapcache_init(void);

// this function forceably performs garbage collection of all
// free and floating map blocks.  Locked blocks are not affected
// by this call.  It returns the size of the biggest free units
// after compacting memory.
UINT32 mapcache_compact(UINT32 nunits);

// check the cache integrity to ensure that everything is as it should be
void mapcache_check(void);

/*----------------------------------------------------------------*/

#ifdef DEBUG_MAPCACHE_CHECK_LOCK_PTR
BOOL8 mapcache_isLockedPtr(const void *ptr);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __ENMCACHEP_H__ */
