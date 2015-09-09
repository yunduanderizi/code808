/*****************************************************************************
 *  (c) 2014 innov China limited. all rights reserved.
 *
 *  module name: geheap.c
 *
 *  author(s): matt broadbent <mbroadbent@21com.com>
 *
 *  description:
 *    this module contains functions for implementation of a simple
 *    self-compacting heap
 *****************************************************************************/

#ifndef GEHEAP_H
#define GEHEAP_H

#include <containers/gelist.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Containers
/// @ingroup Utilities
/// @{

/// @addtogroup Heap
/// The heap manager module provides a set of functions for the 
/// allocation and deallocation of memory within a self compacting 
/// relocatable heap.  As the blocks of memory within the heap can 
/// move the memory is not accessed directly, but through anchors 
/// (these do not move), which are pointers to the blocks of 
/// memory.  Multiple heaps can be declared and the heap manager
/// provides support for automatic heap compaction whenever a heap 
/// is altered; thus a heap will always have a continuous block of 
/// free memory.  Manual compaction of non-autocompacting heaps is 
/// also supported.
/// @ingroup Containers
/// @{

/// @typedef THeapAnchor
/// A THeapAnchor points to the first byte of a memory block allocated
/// by the heap manager.
typedef UINT8 *THeapAnchor;

/// @typedef THeap
/// This structure contains information that represents the current
/// state of the heap.
///
/// @par
/// DO NOT ACCESS IT DIRECTLY, INSTEAD USE THE FUNCTIONS PROVIDED.
typedef struct THeap {
    /** @internal All members are private */
    UINT32 heap_offset;
    UINT32 anchors_offset;
    UINT32 heap_size;
    UINT32 heap_used;
    INT32 total_anchors;
    INT32 free_anchors;
    BOOL8 auto_compact;
    BOOL8 fully_compact;
    BOOL8 fixed_blocks;
    BOOL8 unused;
    TList used_list;
    TList free_list;
} THeap;

/// heap_addAnchors is used to add more anchors to a heap's anchor table.
/// All new anchors are initialised to be NULL.
///
/// @param heap A pointer to the heap.
///
/// @param anchors The required number of anchors to add to the anchor 
/// table.
///
/// @return heap_addAnchors returns TRUE if the new anchors were created
/// successfully, otherwise it returns FALSE.
extern MAPCORE_API BOOL8 heap_addAnchors( THeap *heap,
                                          INT32 anchors );

/// heap_realloc is used to alter the size of a previously allocated block.
///
/// @param heap A pointer to the heap.
///
/// @param anchor A pointer to an anchor which points to a valid block.
///
/// @param size The new required size of the block in bytes.
///
/// @return heap_realloc returns TRUE if the reallocation was successful 
/// and anchor was modified, otherwise it returns FALSE and the anchor 
/// is not changed.
extern MAPCORE_API BOOL8 heap_realloc( THeap *heap,
                                       THeapAnchor *anchor,
                                       UINT32 size );

/// heap_malloc is used to allocate a block of memory within the heap.
/// The memory allocated is not initialised to any particular values.
///
/// @param heap A pointer to the heap.
/// 
/// @param size The size of the block required in bytes.
///
/// @return heap_malloc returns a pointer to the anchor for the block of 
/// memory if successful, otherwise the required memory could not be 
/// allocated and it returns NULL.
extern MAPCORE_API THeapAnchor *heap_malloc( THeap *heap,
                                             UINT32 size );

/// heap_init initialises a number of variables that describe the current
/// state of a heap. heap_init must be called before any calls to other heap
/// functions are made. If the number of anchors for the anchor table is 0,
/// a default value of 32 is used.
///
/// @param heap A pointer to a block of memory to use as the heap.
///
/// @param heap_size The length of the heap in bytes.
///
/// @param anchors The number of anchors to use for the anchor table.
///
/// @param auto_compact A boolean value which tells the heap manager 
/// whether or not it should auto compact the heap every time the heap 
/// is changes.
///
/// @return heap_init returns TRUE indicating if the heap was successfully 
/// initialised, otherwise it returns FALSE and the state of the heap is 
/// undefined.
extern MAPCORE_API BOOL8 heap_init( THeap *heap,
                                    UINT32 heap_size,
                                    INT32 anchors,
                                    BOOL8 auto_compact );

/// heap_free is used to free a block of memory within the heap.
///
/// @param heap A pointer to the heap.
///
/// @param anchor A pointer to an anchor that points to a valid block.
extern MAPCORE_API void heap_free( THeap *heap,
                                   THeapAnchor *anchor );

/// heap_freeAll is used to free all the blocks of memory currently 
/// allocated.  This call resets all anchors to NULL.
///
/// @param heap A pointer to the heap.
extern MAPCORE_API void heap_freeAll( THeap *heap );

/// heap_findAnchor is used to find the anchor for a block.
///
/// @param heap A pointer to the heap.
///
/// @param block A pointer to the first byte in the block of allocated 
/// memory.
///
/// @return heap_findAnchor returns a pointer to the anchor if it is 
/// successful, otherwise it returns NULL.
extern MAPCORE_API THeapAnchor *heap_findAnchor( THeap *heap,
                                                 UINT8 *block );
  
/// heap_fixBlocks informs the heap manager that the blocks are not allowed
/// to move. This is useful for applications where the blocks need to 
/// remain in static positions temporarily.
/// 
/// @param heap A pointer to the heap.
extern MAPCORE_API void heap_fixBlocks( THeap *heap );

/// heap_compact is used to compact the heap. heap_compact defragments
/// the heap allowing larger continuous blocks of memory to be allocated.
/// 
/// @param heap A pointer to the heap.
extern MAPCORE_API void heap_condenseFreeList( THeap *heap );

/// heap_calloc is used to allocate a block of memory for a number of
/// objects of a given size. The memory allocated is initialised to NULL.
/// 
/// @param heap A pointer to the heap.
/// 
/// @param number The number of objects to allocate space for.
/// 
/// @param size The size of each object in bytes.
/// 
/// @return heap_calloc returns a pointer to the anchor for the block of 
/// memory if successful, or NULL if the required memory could not be 
/// allocated.
extern MAPCORE_API THeapAnchor *heap_calloc( THeap *heap,
                                             UINT32 number,
                                             UINT32 size );

/// heap_blockSize is used to return the size of a block.
/// 
/// @param anchor A pointer to an anchor which points to a valid block.
///
/// @return heap_blockSize returns an unsigned long corresponding to the
/// length of the block.
extern MAPCORE_API UINT32 heap_blockSize( THeapAnchor *anchor );

/// heap_unfixBlocks informs the heap that it is allowed to move memory 
/// blocks.
///
/// @param heap A pointer to the heap.
extern MAPCORE_API void heap_unfixBlocks( THeap *heap );

/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif /* GEHEAP_H */

