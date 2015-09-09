#ifndef TMTIME_H
#define TMTIME_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Utilities
/// @{ 

/// @addtogroup Time
/// @ingroup Utilities
/// @{

/// Basic representation of time. #TTime is used to represent:
/// - An instant in time. Time is relative to 00:00:00, January 1, 1970 C.E.
/// - A time interval. e.g. You can subtract one #TTime from another to obtain
///   the time interval between them.
///
/// @todo
/// Beware the Y2K38 bug! We will eventually need to represent time as an INT64
/// value. 64-bit time - even measured in microseconds - will not roll
/// over for about 292,000 years.
typedef struct {
  /// Number of seconds.
  INT32 sec;
  /// Number of additional microseconds (0 to 999999).
  INT32 usec;
} TTime;

extern MAPCORE_API
INT32 time_compare(TTime t1, TTime t2);

extern MAPCORE_API
TTime time_add(TTime t1, TTime t2);

extern MAPCORE_API
TTime time_subtract(TTime t1, TTime t2);


/// @}

/// @}


#ifdef __cplusplus
}
#endif

#endif /* TMTIME_H */
