/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_memory_memoryheap_h
#define INCLUDE_GUARD_memory_memoryheap_h

#include <platform_types.h>

namespace innov {

//--------------------------------------------------------------------------------
class CMemoryHeap {

public:
  typedef void* TAllocPartition( void*  allocContext,
                                 UINT32 partitionSize,
                                 void*  partitionHint );
  typedef void TFreePartition( void* allocContext,
                               void* partition );

private:
  struct TPartition;
  struct TBlock;

private:
  void*            m_allocContext;
  TAllocPartition* m_allocPartition;
  TFreePartition*  m_freePartition;
  void*            m_partitionHint;
  UINT32           m_partitionUnitSize;
  UINT32           m_partitionOverhead;
  TPartition*      m_firstPartition;
  UINT32           m_numPartitions;

public:
  CMemoryHeap( void*            allocContext,
               TAllocPartition* allocPartition,
               TFreePartition*  freePartition,
               void*            partitionHint,
               UINT32           partitionUnitSize,
               UINT32           partitionOverhead );

  ~CMemoryHeap();

public:
  void* alloc( UINT32 blockSize,
               void*  hint = 0 );

  void* realloc( void*  block,
                 UINT32 newBlockSize,
                 void*  hint = 0 );

  void free( void* block );

  UINT32 blockSize( void* block );

private:
  static UINT32 getBlockSize( TBlock* block );

  TPartition* grow( UINT32   blockSize,
                    void*    hint,
                    TBlock** freeBlock );

// For debugging.
private:
  static CMemoryHeap*  g_debugFirstHeap;
         CMemoryHeap*  m_debugNextHeap;
         CMemoryHeap** m_debugHeapOwner;
public:
  static void resetHeapList();
  static void dumpAllHeaps();
};

//--------------------------------------------------------------------------------
} //namespace innov

#endif //INCLUDE_GUARD_memory_memoryheap_h
