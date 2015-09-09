/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_containers_bitset_h
#define INCLUDE_GUARD_containers_bitset_h

/**
@file

@brief This module defines an implementation of a bitset.
*/

/* Include the following header for platform type definitions. */
#include <platform_types.h>
#include <string.h>

/* Force C linkage in C++ translation units. */
#if defined(__cplusplus)
extern "C" {
#endif

/**
@brief The bitset structure.

@note
There is no initialization function.
The fields of the bitset structure must be explicitly initialized.
*/
typedef struct TBitset {
  /** An array of 32-bit words comprising the bitset array. */
  UINT32* array;
  /** The number of significant bits stored in the bitset array. */
  UINT32  bitCount;
} TBitset;

/** This macro calculates the number of 32-bit words in a bitset array.
    This is used when allocating memory for the bitset array. */
#define BITSET_WORD_COUNT(bitCount) (((bitCount) + 31) >> 5)

/** This macro calculates the number of bytes in a bitset array.
    It is used in byte-oriented memory operations. */
#define BITSET_BYTE_COUNT(bitCount) (((bitCount) + 7) >> 3)

/** This macro calculates the number of bytes that should be allocated
    for a bitset array. */
#define BITSET_ALLOC_SIZE(bitCount) (BITSET_WORD_COUNT(bitCount) * sizeof(UINT32))

/**
@brief Set a range of bits in the bitset array.

@param[in] bitset Points to the #TBitset structure to be modified.

@param[in] firstIndex Specifies the index of the first bit to be set.

@param[in] lastIndex Specifies the index of the first bit to be set.
*/
void bitset_setRangeBits( TBitset* bitset,
                          UINT32   firstIndex,
                          UINT32   lastIndex );

/**
@brief Clear all bits in a bitset array.

@param[in] bitset Points to the #TBitset structure to be modified.
*/
static INLINE void
bitset_clearAllBits( TBitset* bitset )
{
  memset( bitset->array,
          0x00,
          BITSET_ALLOC_SIZE(bitset->bitCount) );
}

/**
@brief Set all bits in a bitset array.

@param[in] bitset Points to the #TBitset structure to be modified.
*/
static INLINE void
bitset_setAllBits( TBitset* bitset )
{
  if ( bitset->bitCount ){
    // We MUST clear the bits in the last word
    // before we set all bits in the bitset, to
    // ensure that only valid bits are ever set.
    bitset->array[BITSET_WORD_COUNT(bitset->bitCount)-1] = 0;

    bitset_setRangeBits( bitset, 0, bitset->bitCount-1 );
  }
}

/**
@brief Copy the bits from one bitset array to another bitset array.

@param[in] bitset Points to the destination #TBitset structure
to be modified.

@param[in] bitset Points to the source #TBitset structure to be copied.
*/
static INLINE void
bitset_copyBits( TBitset*       destBitset,
                 const TBitset* sourceBitset )
{
  assert( sourceBitset->bitCount >= destBitset->bitCount );

  memmove( destBitset->array,
           sourceBitset->array,
           BITSET_BYTE_COUNT(destBitset->bitCount) );
}

/**
@brief OR the bits from one bitset array with another bitset array.

@param[in] bitset Points to the destination #TBitset structure
to be modified.

@param[in] bitset Points to the source #TBitset structure to be OR-ed
with the destination.
*/
static INLINE void
bitset_orBits( TBitset*       destBitset,
               const TBitset* sourceBitset )
{
  UINT32 count, index;

  assert( sourceBitset->bitCount >= destBitset->bitCount );

  count = BITSET_WORD_COUNT(destBitset->bitCount);

  for ( index = 0; index < count; ++index ) {
    destBitset->array[index] |= sourceBitset->array[index];
  }
}

/**
@brief Clear the bits from the destBitset if they are present in the sourceBitset

@param[in] bitset Points to the destination #TBitset structure
to be modified.

@param[in] bitset Points to the source #TBitset structure to be used
*/
static INLINE void
bitset_clearBits( TBitset*       destBitset,
                  const TBitset* sourceBitset )
{
  UINT32 count, index;

  assert( sourceBitset->bitCount >= destBitset->bitCount );

  count = BITSET_WORD_COUNT(destBitset->bitCount);

  for ( index = 0; index < count; ++index ) {
    destBitset->array[index] &= ~sourceBitset->array[index];
  }

}

/**
@brief Returns TRUE if all the bits are clear for the given bitset. 
FALSE otherwise

@param[in] bitset Points to the #TBitset to check

*/
static INLINE BOOL8
bitset_isEmpty( const TBitset* bitset )
{
  UINT32 count, index;

  count = BITSET_WORD_COUNT(bitset->bitCount);

  for ( index = 0; index < count; ++index ) {
    if ( bitset->array[index] ) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
@brief AND the bits from one bitset array with another bitset array.

@param[in] bitset Points to the destination #TBitset structure
to be modified.

@param[in] bitset Points to the source #TBitset structure to be AND-ed
with the destination.
*/
static INLINE void
bitset_andBits( TBitset*       destBitset,
                const TBitset* sourceBitset )
{
  UINT32 count, index;

  assert( sourceBitset->bitCount >= destBitset->bitCount );

  count = BITSET_WORD_COUNT(destBitset->bitCount);

  for ( index = 0; index < count; ++index ) {
    destBitset->array[index] &= sourceBitset->array[index];
  }
}

/**
@brief Set one bit in the bitset array.

@param[in] bitset Points to the #TBitset structure to be modified.

@param[in] index Specifies the index of the bit to be set.
*/
static INLINE void
bitset_setBit( TBitset* bitset,
               UINT32   index )
{
  UINT32 bit;

  assert( index < bitset->bitCount );

  bit = index & 0x1F;
  index >>= 5;

  bitset->array[index] |= ((UINT32)1 << bit);
}

/**
@brief Clear one bit in the bitset array.

@param[in] bitset Points to the #TBitset structure to be modified.

@param[in] index Specifies the index of the bit to be cleared.
*/
static INLINE void
bitset_clearBit( TBitset* bitset,
                 UINT32   index )
{
  UINT32 bit;

  assert( index < bitset->bitCount );

  bit = index & 0x1F;
  index >>= 5;

  bitset->array[index] &= ~((UINT32)((UINT32)1 << bit));
}

/**
@brief Lookup the value of one bit in the bitset array.

@param[in] bitset Points to the #TBitset structure to be read.

@param[in] index Specifies the index of the bit to be read.
*/
static INLINE BOOL8
bitset_testBit( const TBitset* bitset,
                UINT32         index )
{
  UINT32 bit;

  assert( index < bitset->bitCount );

  bit = index & 0x1F;
  index >>= 5;

  return ((bitset->array[index] & ((UINT32)1 << bit)) != 0);
}

/**
@brief Returns the number of set bits for the given bitset. 

@param[in] bitset Points to the #TBitset to check

*/
static INLINE UINT32
bitset_countBits( const TBitset* bitset )
{
  UINT32 count, index;

  count = 0;

  for ( index = 0; index < bitset->bitCount; ++index ) {
    if ( bitset_testBit( bitset, index ) ) {
      count++;
    }
  }

  return count;
}


/* Force C linkage in C++ translation units. */
#if defined(__cplusplus)
}; //extern "C"
#endif

#endif /*INCLUDE_GUARD_containers_bitset_h*/
