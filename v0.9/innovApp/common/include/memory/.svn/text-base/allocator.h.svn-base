/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_memory_allocator_h
#define INCLUDE_GUARD_memory_allocator_h

#include <platform_types.h>

/*********************************************************************************
 *                                 C declarations                                *
 *********************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

//--------------------------------------------------------------------------------
/// @brief An allocator handle.
///
/// @note  An allocator handle (TAllocatorHandle) can be cast to
///        and from an allocator interface pointer (IAllocator*).
///
typedef struct TAllocatorImpl* TAllocatorHandle;

//--------------------------------------------------------------------------------
/// @brief The null allocator handle value.
///
#define ALLOCATOR_HANDLE_NULL ((TAllocatorHandle)0)

//--------------------------------------------------------------------------------
/// @brief Add a reference to an allocator.
///
/// @param[in]  allocator  An allocator handle.
///
/// @return  The new reference count.
///
UINT32 allocator_addRef( TAllocatorHandle allocator );

//--------------------------------------------------------------------------------
/// @brief Release a reference to an allocator.
///        Destroy the allocator if the last reference is released.
///
/// @param[in]  allocator  An allocator handle.
///
/// @return  The new reference count.
///
/// @note  Whether or not an allocator will free memory that is still
///        allocated from it when it is destroyed is implementation
///        defined for a specific allocator.
///
UINT32 allocator_release( TAllocatorHandle allocator );

//--------------------------------------------------------------------------------
/// @brief Allocate a block of memory.
///
/// @param[in]  allocator  An allocator handle.
///
/// @param[in]  blockSize  The size of the block to allocate.
///
/// @param[in]  hint       (Optional) A pointer to another block that
///                        can be used as a hint of where to allocate
///                        the new block.
///
/// @return  If success, a pointer to the allocated block.
///          If failure, NULL.
///
void* allocator_allocBlock( TAllocatorHandle allocator,
                            UINT32           blockSize,
                            void*            hint );

//--------------------------------------------------------------------------------
/// @brief Reallocate a block of memory to a new size.
///        The content of the old block will be copied into the new
///        block up to the smaller size of the old block and the new
///        block.  If the new block is larger, the new extension
///        portion of the block will be set to zero.
///
/// @param[in]  allocator     An allocator handle.
///
/// @param[in]  block         The pointer to a currently allocated
///                           block, or NULL to do a first allocation.
///                    
/// @param[in]  newBlockSize  The new size of the block to allocate.
///
/// @param[in]  hint          (Optional) A pointer to another block
///                           that can be used as a hint of where to
///                           allocate the new block.
///
/// @return  If success, a pointer to the allocated block.
///          If failure, NULL.
///
/// @note  If the reallocation fails, the old block is still allocated.
///        You should not overwrite a strong pointer to the block until
///        you know that a reallocation is successful.
///
/// @note  The specified block (if any) must have been previously
///        allocated by a call to either 'allocBlock' or 'reallocBlock'
///        on the current allocator.
///
void* allocator_reallocBlock( TAllocatorHandle allocator,
                              void*            block,
                              UINT32           newBlockSize,
                              void*            hint );

//--------------------------------------------------------------------------------
/// @brief Free a block of memory.
///
/// @param[in]  allocator  An allocator handle.
///
/// @param[in]  block      The pointer to a currently allocated block.
///                        (A Null pointer will be ignored by this function.)
///                    
/// @note  The specified block (if any) must have been previously
///        allocated by a call to either 'allocBlock' or 'reallocBlock'
///        on the current allocator.
///
void allocator_freeBlock( TAllocatorHandle allocator,
                          void*            block );

//--------------------------------------------------------------------------------
/// @brief Allocate memory for an object.
///
/// @note  Object allocation is beneficial for small memory allocations
///        where the size cannot change over time and the size is known
///        at deallocation time.  Due to these requirements, an allocator
///        can optimise allocations and deallocations of objects.
///
/// @param[in]  allocator   An allocator handle.
///
/// @param[in]  objectSize  The size of the object to allocate.
///
/// @param[in]  hint        (Optional) A pointer to another object that
///                         can be used as a hint of where to allocate
///                         the new object.
///
/// @return  If success, a pointer to the allocated object.
///          If failure, NULL.
///
void* allocator_allocObject( TAllocatorHandle allocator,
                             UINT32           objectSize,
                             void*            hint );

//--------------------------------------------------------------------------------
/// @brief Free memory allocated for an object.
///
/// @param[in]  allocator   An allocator handle.
///
/// @param[in]  object      The pointer to a currently allocated block.
///                         (A Null pointer will be ignored by this function.)
///
/// @param[in]  objectSize  The size of the object to deallocate.
///
/// @note  The specified object (if any) must have been previously
///        allocated by a call to 'allocObject' on the current allocator.
///
void allocator_freeObject( TAllocatorHandle allocator,
                           void*            object,
                           UINT32           objectSize );

//--------------------------------------------------------------------------------
/// @internal
typedef struct TAllocatorParameters TAllocatorParameters;

//--------------------------------------------------------------------------------
/// @brief Make a new allocator.
///
/// @param[in]  parameters  A pointer to a parameters structure,
///                          or NULL to use the default parameters.
///
/// @param[out] allocator  An allocator handle, or ALLOCATOR_HANDLE_NULL
///                        if an allocator could not be created.
///
/// @return  A boolean that indicates whether or not an allocator was
///          successfully created.
///
/// @note  The allocator must eventually be released by a call to the release
///        method of the allocator.
///
/// @note  The parameters structure is currently an internal structure,
///        for innovEngine use only.
///
/// @note  Using the default parameters will cause the new allocator to be
///        created in a single engine session, and such an allocator will
///        become invalid after an engine reinitialisation.
///
BOOL8 allocator_make( const TAllocatorParameters* parameters,
                      TAllocatorHandle*           allocator );

//--------------------------------------------------------------------------------
/// @brief Get the innovEngine global allocator.
///
/// @return  An allocator handle, or ALLOCATOR_HANDLE_NULL if the
///          innovEngine global allocator could not be created.
///
/// @note  The innovEngine global allocator is created in the mapcache,
///        and will not be valid after an engine reinitialisation.
///
TAllocatorHandle engine_getAllocator( void );

//--------------------------------------------------------------------------------
#if defined(__cplusplus)
} //extern "C"

/*********************************************************************************
 *                                C++ declarations                               *
 *********************************************************************************/

//--------------------------------------------------------------------------------
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED

//typedef INT32 HRESULT;
typedef long HRESULT;

#endif

//--------------------------------------------------------------------------------
#ifndef GUID_DEFINED
#define GUID_DEFINED

typedef struct _GUID {
  UINT32 Data1;
  UINT16 Data2;
  UINT16 Data3;
  UINT8  Data4[8];
} GUID;

#endif

//--------------------------------------------------------------------------------
typedef GUID IID;

//--------------------------------------------------------------------------------
#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED

#define REFIID const IID &

#endif

//--------------------------------------------------------------------------------
namespace innov {

//--------------------------------------------------------------------------------
/// @brief An allocator interface.
///
/// @note  An allocator interface pointer (IAllocator*) can be cast
///        to and from an allocator handle (TAllocatorHandle).
///
class IAllocator {

public:
  virtual HRESULT
  QueryInterface( REFIID iid,
                  void** object ) = 0;

  virtual UINT32
  AddRef() = 0;

  virtual UINT32
  Release() = 0;

public:
  /// @brief Allocate a block of memory.
  ///
  /// @param[in]  blockSize  The size of the block to allocate.
  ///
  /// @param[in]  hint       (Optional) A pointer to another block that
  ///                        can be used as a hint of where to allocate
  ///                        the new block.
  ///
  /// @return  If success, a pointer to the allocated block.
  ///          If failure, NULL.
  ///
  void* allocBlock( UINT32 blockSize,
                    void*  hint = 0 );

  /// @brief Reallocate a block of memory to a new size.
  ///        The content of the old block will be copied into the new
  ///        block up to the smaller size of the old block and the new
  ///        block.  If the new block is larger, the new extension
  ///        portion of the block will be set to zero.
  ///
  /// @param[in]  block         The pointer to a currently allocated
  ///                           block, or NULL to do a first allocation.
  ///                    
  /// @param[in]  newBlockSize  The new size of the block to allocate.
  ///
  /// @param[in]  hint          (Optional) A pointer to another block
  ///                           that can be used as a hint of where to
  ///                           allocate the new block.
  ///
  /// @return  If success, a pointer to the allocated block.
  ///          If failure, NULL.
  ///
  /// @note  If the reallocation fails, the old block is still allocated.
  ///        You should not overwrite a strong pointer to the block until
  ///        you know that a reallocation is successful.
  ///
  /// @note  The specified block (if any) must have been previously
  ///        allocated by a call to either 'allocBlock' or 'reallocBlock'
  ///        on the current allocator.
  ///
  void* reallocBlock( void*  block,
                      UINT32 newBlockSize,
                      void*  hint = 0 );

  /// @brief Free a block of memory.
  ///
  /// @param[in]  block  The pointer to a currently allocated block.
  ///                    (A Null pointer will be ignored by this function.)
  ///                    
  /// @note  The specified block (if any) must have been previously
  ///        allocated by a call to either 'allocBlock' or 'reallocBlock'
  ///        on the current allocator.
  ///
  void freeBlock( void* block );

  /// @brief Allocate memory for an object.
  ///
  /// @note  Object allocation is beneficial for small memory allocations
  ///        where the size cannot change over time and the size is known
  ///        at deallocation time.  Due to these requirements, an allocator
  ///        can optimise allocations and deallocations of objects.
  ///
  /// @param[in]  objectSize  The size of the object to allocate.
  ///
  /// @param[in]  hint        (Optional) A pointer to another object that
  ///                         can be used as a hint of where to allocate
  ///                         the new object.
  ///
  /// @return  If success, a pointer to the allocated object.
  ///          If failure, NULL.
  ///
  void* allocObject( UINT32 objectSize,
                     void*  hint = 0 );

  /// @brief Free memory allocated for an object.
  ///
  /// @param[in]  object      The pointer to a currently allocated block.
  ///                         (A Null pointer will be ignored by this function.)
  ///
  /// @param[in]  objectSize  The size of the object to deallocate.
  ///                    
  /// @note  The specified object (if any) must have been previously
  ///        allocated by a call to 'allocObject' on the current allocator.
  ///
  void freeObject( void*  object,
                   UINT32 objectSize );

private:
  virtual void*
  doAllocBlock( UINT32 blockSize,
                void*  hint ) = 0;

  virtual void*
  doReallocBlock( void*  block,
                  UINT32 newBlockSize,
                  void*  hint ) = 0;

  virtual void
  doFreeBlock( void* block ) = 0;

  virtual void*
  doAllocObject( UINT32 objectSize,
                 void*  hint ) = 0;

  virtual void
  doFreeObject( void*  object,
                UINT32 objectSize ) = 0;
};

//--------------------------------------------------------------------------------
} //namespace innov

//--------------------------------------------------------------------------------
#endif /*defined(__cplusplus)*/

//--------------------------------------------------------------------------------
#endif /*INCLUDE_GUARD_memory_allocator_h*/
