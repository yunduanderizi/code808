/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_memory_memorypool_h
#define INCLUDE_GUARD_memory_memorypool_h

#include <platform_types.h>

namespace innov {

//--------------------------------------------------------------------------------
class CMemoryPool {

public:
  typedef void* TAllocBlock( void*  allocContext,
                             UINT32 blockSize );
  typedef void TFreeBlock( void* allocContext,
                           void* block );

private:
  struct TBlock;
  struct TSlot;

private:
  void*        m_allocContext;
  TAllocBlock* m_allocBlock;
  TFreeBlock*  m_freeBlock;
  TBlock*      m_firstBlock;
  UINT32       m_blockSize;
  UINT32       m_slotSize;
  TSlot*       m_firstFreeSlot;
  UINT32       m_numAllocSlots;
#if !defined(NDEBUG)
  UINT32       m_numSlots;
  UINT32       m_maxNumAllocSlots;
  UINT32       m_numAllocs;
#endif

public:
  CMemoryPool( void*        allocContext,
               TAllocBlock* allocBlock,
               TFreeBlock*  freeBlock,
               UINT32       blockSize,
               UINT32       slotSize );
  ~CMemoryPool();

  void* allocSlot();
  void freeSlot( void* slot );

private:
  void grow();
};

//--------------------------------------------------------------------------------
} //namespace innov

#endif //INCLUDE_GUARD_memory_memorypool_h
