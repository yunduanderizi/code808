#ifndef TMTIMEZONE_H
#define TMTIMEZONE_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Utilities
/// @{


/// @addtogroup Time
/// @ingroup Utilities
/// @{

/// @addtogroup TimeZone
/// @ingroup Time
/// @{

/// Maximum size of the time zone database name e.g. "Pacific/Auckland". Time
/// zone database names are unique.
#define TZ_MAX_DATABASE_NAME_SIZE 40

/// Maxmium size of the time zone name e.g. "Pacific Standard Time". Time zone
/// names are not unique.
#define TZ_MAX_NAME_SIZE 40

/// Maximum size of the abbreviated time zone name e.g. "NZDT" for New Zealand
/// Daylight Time. Abbreviated time zone names are not unique.
#define TZ_MAX_ABBR_NAME_SIZE 8

/// @internal
/// Time zone rule. Rules are used to determine daylight saving start / end
/// times and the amount of time that is added to local time. Given a rule
/// and a year, it is possible to work out the exact time at which the rule
/// comes into effect.
typedef struct {
  /// @internal
  /// The "nth" weekday of the month. Valid values are from 1 (first) to 5
  /// (fifth) or -1 (last).
  INT32 nth;
  /// @internal
  /// Day of the week. The range of possible values is from 0 (Sun) to 6 (Sat).
  INT32 day;
  /// @internal
  /// Month from 0 (Jan) to 11 (Dec).
  INT32 month;
  /// @internal
  /// The time of day that the transition is made (seconds past midnight, wall
  /// clock time).
  INT32 tod;
} TTimeZoneRule;

/// Time zone information. Includes details of the UTC offset and rules for
/// daylight saving start / end dates.
typedef struct {
  /// Time zone database name e.g. "Pacific/Auckland"
  char dbName[TZ_MAX_DATABASE_NAME_SIZE];
  /// Standard name for the time zone e.g. "Central Standard Time".
  char stdName[TZ_MAX_NAME_SIZE];
  /// Daylight saving name for the time zone e.g. "Central Daylight Time".
  char dstName[TZ_MAX_NAME_SIZE];
  /// Standard time abbreviated name for the time zone e.g. "PST" for Pacific
  /// Standard Time.
  char stdAbbrev[TZ_MAX_ABBR_NAME_SIZE];
  /// Daylight saving abbreviated name for the time zone e.g. "PDT" for Pacific
  /// Daylight time.
  char dstAbbrev[TZ_MAX_ABBR_NAME_SIZE];
  /// The number of seconds added to UTC to get the local time before any
  /// daylight saving adjustments are made.
  INT32 utcOffset;
  /// The number of seconds added to the UTC offset when daylight saving is in
  /// effect.
  INT32 dstAdjust;
  /// @internal
  /// Date and local time when the transition from standard time to daylight
  /// saving time occurs at this location.
  TTimeZoneRule startRule;
  /// @internal
  /// Date and local time when the transition from daylight saving time to
  /// standard time occurs at this location.
  TTimeZoneRule endRule;
  /// TRUE if the time zone makes a daylight saving shift.
  BOOL8 hasDST;
} TTimeZone;


extern MAPCORE_API
const char * tz_databaseName(const TTimeZone *pThis);

extern MAPCORE_API
const char * tz_STDZoneName(const TTimeZone *pThis);

extern MAPCORE_API
const char * tz_DSTZoneName(const TTimeZone *pThis);

extern MAPCORE_API
const char * tz_STDZoneAbbrev(const TTimeZone *pThis);

extern MAPCORE_API
const char * tz_DSTZoneAbbrev(const TTimeZone *pThis);

extern MAPCORE_API
BOOL8 tz_hasDST(const TTimeZone *pThis);

extern MAPCORE_API
INT32 tz_DSTLocalStartTime(const TTimeZone *pThis, INT32 year);

extern MAPCORE_API
INT32 tz_DSTLocalEndTime(const TTimeZone *pThis, INT32 year);

extern MAPCORE_API
INT32 tz_baseUTCOffset(const TTimeZone *pThis);

extern MAPCORE_API
INT32 tz_DSTOffset(const TTimeZone *pThis);

extern MAPCORE_API
INT32 tz_UTCToLocalTime(const TTimeZone *pThis, INT32 utcTime);


/// @}

/// @}

/// @}


#ifdef __cplusplus
}
#endif

#endif /* TMTIMEZONE_H */
