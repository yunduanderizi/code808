#ifndef GEZLIBD_H
#define GEZLIBD_H

#include <platform_types.h>
#include <io/geio.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Compression
/// @ingroup Utilities
/// @{

/// ZLib Decompression routine
///   
/// @param outputBuffer   Pointer to the location of a destination buffer 
/// @param outputSize     The size of the destination buffer (which should be 
///                       large enough to hold the decompressed data
/// @param inputBuffer    Pointer to the location of compressed data
/// @param inputSize      The size of the input data block 
///
/// @return The number of bytes written to the destination buffer.
extern MAPCORE_API
UINT32  zlib_decompress( UINT8       *outputBuffer, 
                         UINT32       outputSize, 
                         const UINT8 *inputBuffer,  
                         UINT32       inputSize );

/// ZLib Stream Decompression routine
///   
/// @param outputBuffer   Pointer to the location of a destination buffer 
/// @param outputSize     The size of the destination buffer (which should be 
///                       large enough to hold the decompressed data
/// @param inputBuffer    Pointer to an *open* file handle 
/// @param inputSize      The size of the input data block 
///
/// @return The number of bytes written to the destination buffer.
extern MAPCORE_API
UINT32 zlib_stream_decompress( UINT8   *outputBuffer,
                               UINT32   outputSize,
                               IO_FILE *inputFile,
                               UINT32   inputSize ); 

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif // GEZLIBD_H 
