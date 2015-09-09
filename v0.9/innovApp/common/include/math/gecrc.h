#ifndef GECRC_H
#define GECRC_H

// This module provides a function that allows for error checking data
// by Cyclic Redundancy Checking (CRC).  This is useful for ensuring the
// integrity of data that is used for critical processing.

/*----------------------------------------------------------------*/

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/// @addtogroup CRC
/// This module provides function(s) that allows for error checking 
/// data by Cyclic Redundancy Checking (CRC).  This is useful for 
/// ensuring the integrity of data that is used for critical processing.
/// @ingroup Utilities
/// @{

/** Compute a CRC32 (Cyclic Redundancy Check) on byteCount
    bytes starting at the address buffer

    @param buffer address of buffer where to compute CRC32
    @param byteCount number of bytes to compute CRC32
    @param seed Seed can be used when computing crc32 
    across multiple buffers.  Seed can then be
    set as last crc32 computed value for example.

    @return CRC32 Computed CRC.

    @note Was previously UINT32 crc32(const void *buffer, UINT32 byteCount).
    @note Was previously UINT32 crc32(const void *buffer, UINT32 byteCount, UINT32 seed) */
extern MAPCORE_API UINT32 crc32_calculate(const void *buffer,
                                          UINT32 byteCount,
                                          UINT32 seed);

/** @deprecated
    crc32() should now be used instead of crc16_iterate().

    Updates the crc value based on the input data. The CRC value should be
    initialised to 0x0000 before commencing the CRC check.

    @param crc the current CRC value to iterate.

    @param d the new value in the data stream.

    @return returns the updated CRC value. */
DEPRECATED(extern MAPCORE_API UINT16 crc16_iterate( UINT16 crc, UINT8 d ));

/// @}

#ifdef __cplusplus
}
#endif

#endif
