#ifndef GEDYNARRAY_H
#define GEDYNARRAY_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup DynamicArray
/// @ingroup Containers
///
/// @brief A dynamically growing and shrinking array.
///
/// The intended use of this data structure is for slowly growing and
/// shrinking collections for which space usage is important.  Adding
/// and accessing elements is \em O(1) while deleting elements is \em
/// O(n), as for a normal array.
///
/// Memory is allocated in blocks that hold a user-specified number of
/// elements.  Each element is the same, user specified size.
/// As elements are removed, unused blocks are freed so that the array
/// will not use much more memory than necessary to hold the in-use elements.
///
/// @section MemoryAllocation Memory allocation
///
/// Memory allocation is performed using user-specified alloc and free
/// functions.
///
/// A new block is allocated when adding an element when all existing
/// blocks are full.  This is suboptimal if adding a large number of
/// elements, but this is not considered an important use case for
/// this data structure.
///
/// A user specified hysteresis controls when empty blocks are freed.
/// The hysteresis is the number of empty blocks that are maintained;
/// this prevents excessive allocation and freeing of blocks when the
/// number of elements oscillates about a multiple of the block size.
///
/// @section Example
///
/// First, we define a structure for our elements.
///
/// @code
/// typedef struct TElement {
///   UINT32 field1;
///   UINT16 field2;
/// } TElement;
/// @endcode
///
/// Then we decide how many elements we want in each block and then
/// calculate the number of blocks we want based on how many elements
/// we want to store and statically allocate the dynamic array
/// structure itself and some storage for the blocks.
///
/// @code
/// #define MAX_ELEMENTS 100
/// #define ELEMENTS_PER_BLOCK
/// #define NUM_BLOCKS (MAX_ELEMENTS / ELEMENTS_PER_BLOCK)
///
/// static TDynamicArray array;
/// static TDynamicArrayBlockInfo blocks[NUM_BLOCKS];
/// @endcode
///
/// Inside some initialisation function we then initialise the dynamic
/// array.  This will create a dynamic array with a hysteresis of 1,
/// i.e. one spare block will be kept empty, but we also specify that
/// this spare block is \em not to be kept around if the array is
/// empty.  We use the standard \c malloc and \c free functions.
///
/// @code
///   if (!dynamicArray_init(&array, blocks, NUM_BLOCKS, ELEMENTS_PER_BLOCK,
///                          sizeof(TElement), 1, FALSE,
///                          &malloc, &free)) {
///     return FALSE;
///   }
/// @endcode
///
/// Later, we can add an element to the array.  We know the index of
/// the newly added element because it is always at the end of the
/// array.
///
/// @code
///   TElement *e;
///   UINT32 index;
///
///   index = dynamicArray_size(&array);
///   e = dynamicArray_add(&array);
///   if (!e) {
///     // allocation failure
///     return FALSE;
///   }
///   e.field1 = 10;
///   e.field2 = 20;
/// @endcode
///
/// If we know the index we can retrieve this element.
///
/// @code
///   e = dynamicArray_get(&array, index);
/// @endcode
///
/// Similarly, we can remove the element given its index.
///
/// @code
///   dynamicArray_delete(&array, index);
/// @endcode
///
/// To iterate over the elements of the array we can use the
/// handy-dandy FOREACH macros.
///
/// @code
///   DYNAMIC_ARRAY_FOREACH(i, TElement *, e) {
///     printf("index = %d, element = { field1 = %d, field2 = %d }\n",
///            i, e->field1, e->field2);
///   } DYNAMIC_ARRAY_FOREACH_END;
/// @endcode
///
/// When we're finished with the array we destroy it.
///
/// @code
///   dynamicArray_destroy(&array);
/// @endcode
///
/// @{

/// Storage for blocks of memory.
typedef void *TDynamicArrayBlockInfo;

/// The type of functions that can be used to allocate memory.
typedef void *TDynamicArrayAlloc(UINT32 size);

/// The type of functions that can be used to free memory.
typedef void TDynamicArrayFree(void *block);

/// @brief A dynamic array.
///
/// @internal
/// This struct contains meta data about the array.
typedef struct TDynamicArray {
  /// @name Configuration
  /// @internal
  ///
  /// These are copied directly from the arguments to
  /// #dynamicArray_init.
  ///
  /// @{
  UINT32                  elementSize;
  UINT32                  elementsPerBlock;
  UINT32                  maxBlocks;
  TDynamicArrayBlockInfo *blocks;
  BOOL8                   useHysteresisWhenEmpty;
  UINT32                  shrinkHysteresis;
  TDynamicArrayAlloc     *alloc;
  TDynamicArrayFree      *free;
  /// @}

  /// @name State
  /// @internal
  /// @{

  UINT32 size;                  ///< @internal the number of elements
  UINT32 numBlocks;             ///< @internal the number of blocks

  /// @}
} TDynamicArray;

extern MAPCORE_API
BOOL8 dynamicArray_init(TDynamicArray          *dynArray,
                        TDynamicArrayBlockInfo *blocks,
                        UINT32                  maxBlocks,
                        UINT32                  elementsPerBlock,
                        UINT32                  elementSize,
                        UINT32                  shrinkHysteresis,
                        BOOL8                   useHysteresisWhenEmpty,
                        TDynamicArrayAlloc     *alloc,
                        TDynamicArrayFree      *free);

extern MAPCORE_API
void dynamicArray_destroy(TDynamicArray *dynArray);

extern MAPCORE_API
void dynamicArray_clear(TDynamicArray *a);

extern MAPCORE_API
UINT32 dynamicArray_size(const TDynamicArray *dynArray);

extern MAPCORE_API
void *dynamicArray_get(const TDynamicArray *dynArray, UINT32 index);

extern MAPCORE_API
void *dynamicArray_add(TDynamicArray *dynArray);

extern MAPCORE_API
void dynamicArray_delete(TDynamicArray *dynArray, UINT32 index);

extern MAPCORE_API
void dynamicArray_resize(TDynamicArray *dynArray,
                         UINT32 newMaxBlocks);

/// @brief Begins an iteration over the elements of a dynamic array in
///        increasing index order.
///
/// @param a the address of the dynamic array
/// @param i the name to use for the variable holding the index
/// @param t the type to use for the variable holding each element
/// @param v the name to use for the variable holding each element
///
#define DYNAMIC_ARRAY_FOREACH(a, i, t, v)        \
  {                                              \
    UINT32 i;                                    \
    t      v;                                    \
    for (i = 0; i < dynamicArray_size(a); ++i) { \
      v = dynamicArray_get(a, i);

/// @brief Begins an iteration over the elements of a dynamic array in
///        decreasing index order.
///
/// @param a the address of the dynamic array
/// @param i the name to use for the variable holding the index
/// @param t the type to use for the variable holding each element
/// @param v the name to use for the variable holding each element
///
#define DYNAMIC_ARRAY_FOREACH_REVERSE(a, i, t, v)       \
  {                                                     \
    UINT32 i;                                           \
    t      v;                                           \
    i = dynamicArray_size(a);                           \
    while (i--) {                                       \
      v = dynamicArray_get(a, i);

/// @brief Ends an iteration over the elements of a dynamic array.
///
/// This macro may optionally be followed by a semicolon, according to
/// taste.
///
#define DYNAMIC_ARRAY_FOREACH_END \
    }                             \
  }

/// @}

#ifdef __cplusplus
}
#endif

#endif
