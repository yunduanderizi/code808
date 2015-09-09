#ifndef ENBITSTREAMP_H
#define ENBITSTREAMP_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

// Implements a bitstream type which encapsulates unpack
// functionality which does not use any byte-alignment.
typedef struct TBitStream {
  UINT8  *addr;
  INT8    bitPos;  // 7 = msb .. 0 = lsb
  UINT32  size;
} TBitStream;

// open a bitstream.
// specify a non-null address for the input buffer.
void bitstream_open( TBitStream *bitStream,
                     UINT8      *addr );

// close a bitstream.
// this function returns the size of the input buffer.
UINT32 bitstream_close( TBitStream *bitStream );

// unpack either the value 0 or 1.
void bitstream_unpackBit( TBitStream *bitStream,
                          BOOL8      *value );

// unpack a value between 0 and the largest value inclusive
// that can be packed into the fieldWidth number of bits.
void bitstream_unpackFixedWidthValue( TBitStream *bitStream,
                                      UINT8       fieldWidth,
                                      UINT32     *value );

// unpack a value between 1 and maxValue inclusive.
void bitstream_unpackPositiveValue( TBitStream *bitStream,
                                    UINT32      maxValue,
                                    UINT32     *value );

// unpack a value between 0 and maxValue inclusive.
void bitstream_unpackNonNegativeValue( TBitStream *bitStream,
                                       UINT32      maxValue,
                                       UINT32     *value );

#ifdef __cplusplus
}
#endif

#endif /* ENBITSTREAMP_H */
