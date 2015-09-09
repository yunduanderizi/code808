#ifndef GETIME_H
#define GETIME_H

// This module provides functions that can be used to manipulate Julian
// time / date values that are stored in one of three formats:
// packed - the number of seconds elapsed since 1 Jan 2000
// unpacked - a TTime structure that contains an expanded representation
// of the time and date.
// text - time and date values in a text format HHMM(SS) and DDMMYY(YY)
// Times and dates can be converted between all representations.

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup Time
    @ingroup Utilities
    This module provides functions that can be used to manipulate Julian
    time / date values that are stored in one of three formats:
    
    packed - the number of seconds elapsed since 1 Jan 2000
    unpacked - a TTime structure that contains an expanded 
    representation of the time and date.
    text - time and date values in a text format HHMM(SS) and 
    DDMMYY(YY).
    
    Times and dates can be converted between all representations. */
/**@{ start of group */

/** @def TIME_BAD_VALUE
    This value is returned if a time string cannot be decoded */
#define TIME_BAD_VALUE (UINT32)0xFFFFFFFFL

#define DATE_BAD_VALUE (UINT32)0xFFFFFFFFL

/** @def TIME_BASE_YEAR 
    This is the base year for the time functions. A time value of
    0 represents midnight on the 1 Jan 2000 */
#define TIME_BASE_YEAR ((UINT16)2000)



extern MAPCORE_API UINT32 time_stringToUlong( const char* time,
                                              const char* date );

extern MAPCORE_API void time_ulongToString( UINT32 t,
                                            char *time,
                                            char *date );

/**@} end of group */


#ifdef __cplusplus
}
#endif

#endif
