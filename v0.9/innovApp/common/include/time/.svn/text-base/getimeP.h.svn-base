#ifndef GETIMEP_H
#define GETIMEP_H

#include <time/getime.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Time */
/**@{ start of group */

/// @internal
/// Difference between innovEngine GPS time and ANSI C (Unix) time. This is the
/// number of elapsed seconds from midnight (00:00:00), January 1, 1970 to
/// midnight, January 1, 2000. Calculated with the command:
/// date --date='2000-01-01 00:00:00 +0000' +%s
#define TIME_BASE_SECONDS 946684800

/**@} end of group */

/// @internal
/// Formerly TTime. Although the time_unpack() function is internal to innovEngine,
/// iCN510 code declares it locally and calls the function regardless! Until
/// they remove this we have to keep this structure around.
typedef struct TTimeUnpacked {
  UINT16 year;
  UINT8 month;
  UINT8 day;
  UINT8 hour;
  UINT8 min;
  UINT8 sec;
} TTimeUnpacked;

/*----------------------------------------------------------------*/

// this function takes an unpacked time value and packs it into a single
// 32 bit number representing the number of seconds from the start of the
// base year.  A packed time value is valid for 138 years.
// if the packed time value is outside the range of values that can be
// encoded or is invalid then TIME_BAD_VALUE is returned.
UINT32 time_pack( TTimeUnpacked *u );

// Unpacks a 32 bit time value into a TTime structure where the date and time
// are represented more explicitly.  This makes it much easier to read and
// manipulate the time value.
void time_unpack( UINT32 time,
                  TTimeUnpacked *u );

#ifdef __cplusplus
}
#endif

#endif /* GETIMEP_H */
