#ifndef RTFOREIGNSTORE_H
#define RTFOREIGNSTORE_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup StaticHeap
/// @ingroup Containers
/// @brief A partitioned memory area whose layout is determined by a
///        template.
///
/// This is similar to a \ref Heap used for dynamic memory allocation,
/// but instead of directly allocating memory, clients of a static
/// heap reserve space in a template.  Once the template has been
/// populated it can be instantiated on a particular piece of memory
/// and then the clients of the static heap can obtain pointers to
/// parts of the static heap that correspond to the space they
/// reserved in the template.
///
/// @section Structure
///
/// A static heap template is made up of static heap blocks.  Each
/// block is a number of bytes reserved for a particular owner along
/// with some meta-information about what will be stored in the block.
/// The amount of memory required to instantiate a static heap
/// template is the sum of the sizes of the template's blocks,
/// possibly including padding for alignment purposes.
///
/// A static heap contains a block of memory that is used to store the
/// contents of the heap and a pointer to the static heap template
/// that controls how that memory is laid out.
///
/// @section Alignment
///
/// Because each block can store arbitrary data, a static heap
/// template ensures that each block is aligned on an 4-byte boundary
/// relative to the start of the static heap.  This should ensure
/// compatibility with all foreseeable target platforms.  Addresses
/// returned by #staticHeap_resolveBlock are always 4-byte aligned.
/// If the memory passed to #staticHeapTemplate_instantiate is not
/// 4-byte aligned then some of it is wasted to ensure this alignment.
///
/// @section Usage
///
/// The use of a static heap proceeds in two distinct phases: template
/// definition and heap usage.  The boundary between these phases is
/// marked by the first use of the #staticHeapTemplate_instantiate
/// function.  Once this function has been called for a particular
/// static heap template, that template can not be modified (but it
/// can be instantiated again).
///
/// @subsection TemplateDefintion Template definiton
///
/// Template definition consists of initialising a static heap
/// template (#staticHeapTemplate_init) and then adding blocks to it
/// (#staticHeapTemplate_add).
///
/// @subsection HeapUsage Heap usage
///
/// Heap usage consists of instantiating a static heap template on
/// some memory to yield a static heap
/// (#staticHeapTemplate_instantiate) and then using the memory
/// allocated to the various blocks in the template
/// (#staticHeap_resolveBlock).
///
/// @section Example
///
/// First, a static heap template must be created.
///
/// @code
/// TStaticHeapTemplate shTemplate;
/// TStaticHeapBlock shBlocks[10];
///
/// staticHeapTemplate_init(&shTemplate, 10, blocks);
/// @endcode
///
/// Then, blocks can be added to the template.
///
/// @code
/// typedef struct TBlock1 {
///   UINT32 numFoos;
///   UINT8 foo[32];
/// } TBlock1;
///
/// typedef struct TBlock2 {
///   UINT8 field1;
///   UINT16 field2;
///   UINT32 field3;
///   UINT64 field4;
/// } TBlock2;
///
/// // ...
///
/// TStaticHeapBlock *shBlock1, *shBlock2;
///
/// shBlock1 = staticHeapTemplate_add(&shTemplate, "block1",
///                                   sizeof(TBlock1), NULL, NULL);
/// shBlock2 = staticHeapTemplate_add(&shTemplate, "block2",
///                                   sizeof(TBlock2),
///                                   block2ClearFunc, block2ClearFunc);
/// @endcode
///
/// Once the template has been defined, static heaps can be
/// instantiated from it.
///
/// @code
/// UINT32 shStorage[1000]; // UINT32 ensures 4-byte alignment
/// UINT32 shSize;
/// TStaticHeap sHeap;
///
/// shSize = staticHeapTemplate_instantiate(&shTemplate, shStorage,
///                                         1000 * sizeof(UINT32),
///                                         &sHeap);
/// if (shSize < 0) {
///   // need more than 1000 UINT32s for the static heap
/// }
/// @endcode
///
/// Then the  pre-allocated blocks can be resolved against the
/// instantiated static heap.
///
/// @code
/// TBlock1 *block1;
/// TBlock2 *block2;
///
/// block1 = (TBlock1 *)staticHeap_resolveBlock(&sHeap, shBlock1);
/// block2 = (TBlock2 *)staticHeap_resolveBlock(&sHeap, shBlock2);
/// @endcode
///
/// @{

/// @name Template definition
/// @{

/// @brief The type of functions that can clear a static heap block buffer
///
/// @param buffer the static heap block buffer
/// @param   size the size of the buffer
///
typedef void TStaticHeapBlockClearFunc(void *buffer, UINT32 size);

/// @brief The type of functions that can copy a static heap block buffer
///
/// @param[in]  sourceBuffer the source buffer
/// @param              size the size of the buffer
/// @param[out] targetBuffer the target buffer
///
typedef void TStaticHeapBlockCloneFunc(const void *sourceBuffer,
                                       UINT32 size,
                                       void *targetBuffer);

/// @internal
/// @brief A description of a static heap block.
///
typedef struct TStaticHeapBlock {
  /// some identifying string, for debugging
  const char *owner;
  /// size of buffer in bytes
  UINT32 size;
  /// offset from start of storage in bytes
  UINT32 offset;
  /// function that is used to clear this block's buffer
  TStaticHeapBlockClearFunc *clear;
  /// function that is used to copy this block's buffer
  TStaticHeapBlockCloneFunc *clone;
} TStaticHeapBlock;

/// @internal
/// @brief A template for a static heap.
///
typedef struct TStaticHeapTemplate {
  /// the number of blocks that have been added to the template
  UINT32 numBlocks;
  /// the number of bytes needed by the template
  UINT32 size;
  /// @brief set to \c TRUE once storage has been created for this set
  ///
  /// After that, no more blocks can be added.
  BOOL8 registrationClosed;
  /// the maximum number of blocks that the template can have
  UINT32 maxBlocks;
  TStaticHeapBlock *blocks;     ///< the blocks in the template
} TStaticHeapTemplate;

extern MAPCORE_API
BOOL8 staticHeapTemplate_init(TStaticHeapTemplate *heapTemplate,
                              UINT32 maxBlocks,
                              TStaticHeapBlock *blockStorage);

extern MAPCORE_API
TStaticHeapBlock *staticHeapTemplate_add(TStaticHeapTemplate *heapTemplate,
                                         const char *owner,
                                         UINT32 size,
                                         TStaticHeapBlockClearFunc *clear,
                                         TStaticHeapBlockCloneFunc *clone);

extern MAPCORE_API
UINT32 staticHeapTemplate_getSize(const TStaticHeapTemplate *heapTemplate);

/// @}

/// @name Static heap usage
/// @{

/// @internal
/// @brief An instantiation of a static block template.
///
typedef struct TStaticHeap {
  /// the template defining the layout of the heap
  TStaticHeapTemplate *heapTemplate;
  void *storage;                ///< the storage containing the heap
} TStaticHeap;

INT32 staticHeapTemplate_instantiate(TStaticHeapTemplate *heapTemplate,
                                     void *storage,
                                     UINT32 size,
                                     TStaticHeap *heap);

void staticHeap_clear(TStaticHeap *heap);

BOOL8 staticHeap_clone(const TStaticHeap *source,
                       TStaticHeap *target);

void *staticHeap_resolveBlock(const TStaticHeap *heap,
                              const TStaticHeapBlock *block);

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif
