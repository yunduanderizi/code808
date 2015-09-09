#ifndef TMTZDATABASE_H
#define TMTZDATABASE_H

#include <innovEngine_types.h>
#include <time/tmtimezone.h>
#include <io/iocallbacks.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Utilities
/// @{ start of group


/// @addtogroup Time
/// @ingroup Utilities
/// @{

/// @addtogroup TimeZoneDatabase
/// @ingroup Time
/// @{

/// Time zone database based on the public domain tz database. Information on
/// the tz database can be found at http://www.twinsun.com/tz/tz-link.htm. This
/// implementation has been simplified to exclude all historical time zone
/// changes, and extended to support lookup of time zones by location.
typedef struct {
  /// Handle to the time zone database source. When data is read from a file,
  /// the datasource is a #OS_FILE.
  void *datasource;
  /// Callback functions to support reading time zone data from a source other
  /// than a file.
  TFileCallbacks cb;
  /// @internal
  /// The last time zone read from the tz database is cached in order to avoid
  /// unnecessary text file I/O.
  TTimeZone cachedResult;
} TTimeZoneDatabase;


extern MAPCORE_API
INT32 tzDatabase_open(TTimeZoneDatabase *pThis, OS_FILE f);

extern MAPCORE_API
INT32 tzDatabase_openCallbacks(TTimeZoneDatabase *pThis,
                               void *datasource,
                               const TFileCallbacks *cb);

extern MAPCORE_API
INT32 tzDatabase_close(TTimeZoneDatabase *pThis);

extern MAPCORE_API
BOOL8 tzDatabase_getFromPos(TTimeZoneDatabase *pThis,
                            const TPos *pos,
                            TTimeZone *tz);

extern MAPCORE_API
BOOL8 tzDatabase_getFromMaplockPos(TTimeZoneDatabase *pThis,
                                   TTimeZone *tz);

/// @}

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif /* TMTZDATABASE_H */
