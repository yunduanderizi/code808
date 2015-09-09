/** 
    Normalise/DeNormalise Unit HardwareID to catch miskeying of HardwareID when
    manually keyed by customer or customer service agent. This for associating
    Registration (customer) details with a particular unit for "Endeavour" and
    for a future deployment of short keys.

    The device "About Screen" calls the function "normalisehwid".

    author: Colin Robinson <Colin.Robinson@navman.com> 


    Change History:
    When        Who               What
    ----------  ----------------  ----------------------------------------------
    2005-10-19  robinsonc         Initial implementation.

**/

#ifndef _normalisehwid_h
#define _normalisehwid_h

#include <platform_types.h>

#define OK_                    (1)    /* No error detected */
#define INPUT_TOO_LONG_   (-70043)    /* Input Hardware Identifier too long */
#define INPUT_TOO_SHORT_  (-70050)    /* Input Hardware Identifier too short */
#define OUTPUT_TOO_LONG_  (-70044)    /* Output Hardware Identifier too long */
#define OUTPUT_TOO_SHORT_ (-70051)    /* Output Hardware Identifier too short */
#define BAD_CRC_          (-70045)    /* Bad CRC - Hardware Identifier probably miskeyed */

#define NHWID_BUFSIZE_        256

static const UINT8 base16digits[] = { '0', '1', '2', '3', '4', '5', '6', '7'
                                    , '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' 
                                    };

static const UINT8 base34digits[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9'
                                    , 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'
                                    , 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S'
                                    , 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
                                    };


/*****************************************************************************
 * INCLUDES
 *****************************************************************************/


/*****************************************************************************
 * GLOBAL FUNCTION DECLARATIONS
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/** \brief normaliseHWID: CRC32-protect and format raw hardware identifier.

    -# Bracket input with a known, non-zero value (1) to prevent the base conversion routines from stripping leading and trailing zeros. The "brackets" include the four bytes set asside at the end of the hardware identifier for the CRC32.
    -# Zeroise the CRC32 storage area.
    -# Calculate the CRC32 for the entire formated hardware identifier including the "brackets" and the zeroised CRC32.
    -# Store the CRC32 in the formated hardware identifier.
    -# Convert the formated hardware identifier from base16 to base34.

    @param normalisedBuffer Output buffer that will contain the normalised hardware identifier (output)  - buffer allocated by calling process.
    @param iNormalisedBufferLength Maximum available size of output buffer (input).
    @param rawHWID Hardware Identifier as retrieved from device as ascii bytes (input).
    @param iRawLength Length of raw hardware identifier; required as null is a byte frequently found in the hardware identifiers of some types of devices (input).
    @return Success/Error code - 0 if successful; -ve number if failed.
*/
int normaliseHWID( UINT8* normalisedBuffer, int iNormalisedBufferLength, UINT8* rawHWID, int iRawLength );

/**  \brief deNormaliseHwID: Return the normalised hardware identifier to its original form.

    -# Convert normalised hardware identifier from base 34 to base 16.
    -# Extract and store the CRC32 then zeroise the CRC32 location in the formatted hardware identifier.
    -# Calculate the CRC32 over the formatted hardware identifier and compare to original saved value.
    -# Return the portion of the formatted hardware identifier exluding the brackets and CRC32 storage area.

    @param rawBuffer Raw hardware identifier as originally obtained from the device (output).
    @param iRawBufferLength Length of hardware identifier in its original (raw) form (output).
    @param normalisedHWID Normalised hardware identifier (input).
    @param iNormalisedLength Length of input; not really necessary as normalised hardware identifier is a null terminated string (input).
*/
int deNormaliseHwID( UINT8* rawBuffer, int iRawBufferLength, UINT8* normalisedHWID, int iNormalisedLength );

#ifdef __cplusplus
}
#endif

#endif	/* ifndef _normalisehwid_h */

