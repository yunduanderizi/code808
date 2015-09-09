/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_memory_allocatorimpl_h
#define INCLUDE_GUARD_memory_allocatorimpl_h

#include <memory/allocatorP.h>

//--------------------------------------------------------------------------------
namespace innov {

//--------------------------------------------------------------------------------
BOOL8 makeAllocator( const TAllocatorParameters& parameters,
                     IAllocator**                allocator );

//--------------------------------------------------------------------------------
} //namespace innov

#endif //INCLUDE_GUARD_memory_allocatorimpl_h
