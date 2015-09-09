#ifndef GELZWC_H
#define GELZWC_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Compression
/// Provides functions to compress and expand binary data using the
/// Lempel-Ziv-Welch (LZW) algorithm.
/// @ingroup Utilities
/// @{

/// LZW compressed data is stored at some possibly unaligned memory buffer
/// that is interpreted to be a pointer to the TLZWData structure.
typedef struct TLZWData {
  /// Uncompressed size of the data.
  UINT32 dataSize;
  /// The start location of the data (there can actually be more than 1 byte).
  UINT8 data[1];
} TLZWData;

/// 13 bit LZW compression routine.
///
/// @param dest Points to a destination buffer for the data (which must be 
/// aligned to a 32 bit boundary).  The destination buffer must be at 
/// least as large as sourceSize.
///
/// @param source Points to the (potentially not 32 bit aligned) location 
/// of source data for compression.
///
/// @param sourceSize Contains the size of the source data.
///
/// @return The number of bytes written to the destination buffer.
extern MAPCORE_API
UINT32 lzw_compress( TLZWData *dest,
                     UINT8 *source,
                     UINT32 sourceSize );

/// @}
/// @}

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------*/

#endif
