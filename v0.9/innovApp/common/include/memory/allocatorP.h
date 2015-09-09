/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_memory_allocatorP_h
#define INCLUDE_GUARD_memory_allocatorP_h

#include <memory/allocator.h>

/*********************************************************************************
 *                                 C declarations                                *
 *********************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

//--------------------------------------------------------------------------------
struct TAllocatorParameters {
  BOOL8  useMemoryHeap;
  BOOL8  useMemoryPools;
  UINT32 heapPartitionSize;
  void*  heapPartitionHint;
}; //typedef already in <allocator.h>

//--------------------------------------------------------------------------------
#if defined(__cplusplus)
} //extern "C"

/*********************************************************************************
 *                                C++ declarations                               *
 *********************************************************************************/

//--------------------------------------------------------------------------------
namespace innov {

//--------------------------------------------------------------------------------
} //namespace innov

//--------------------------------------------------------------------------------
#endif /*defined(__cplusplus)*/

//--------------------------------------------------------------------------------
#endif /*INCLUDE_GUARD_memory_allocatorP_h*/
