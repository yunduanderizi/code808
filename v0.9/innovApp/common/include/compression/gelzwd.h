#ifndef GELZWD_H
#define GELZWD_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Compression
/// @ingroup Utilities
/// @{

/// 13 bit LZW decompression routine.
///   
/// @param dest Points to the location of a destination buffer (which does 
/// not have to be word aligned).  The decompression routine continues 
/// until the End of Data (EOD) marker is located in the compressed data 
/// or until destSize bytes of data have been written to the output buffer.
///
/// @param destSize The size of the destination buffer (which should be as 
/// large as source->dataSize.
///
/// @param source The first few bytes of the uncompressed data is treated 
/// as if it is a (non aligned) TLZWData structure.
///
/// @return The number of bytes written to the destination buffer.
extern MAPCORE_API
UINT32 lzw_decompress( UINT8 *dest,
                       UINT32 destSize,/* TLZWData */
                       const UINT8 *source );

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif

