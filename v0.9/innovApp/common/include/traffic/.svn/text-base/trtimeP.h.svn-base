#ifndef TRTIMEP_H
#define TRTIMEP_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Represents a date and time for the purposes of TMC.
///
/// The date and time are stored as the number of non-leap seconds
/// since the datum, which is 2000-01-01 00:00:00 UTC.  Because we use
/// an unsigned 32 bit integer this allows us to represent times from
/// 2000-01-01 00:00:01 to about 136 years later.  The first second of
/// 2000 is lost for use as an invalid value.
///
typedef struct TRDSTime {
  UINT32 seconds;               ///< since 2000-01-01 00:00:00
} TRDSTime;

/// For use in initialisers.
#define ZERO_RDS_TIME {0}

/// For use in assignments.
static const TRDSTime rdsTime_zero = ZERO_RDS_TIME;

// RDS has different base for MJD than GPS
#define MJD_FOR_JAN_2000 51544

/// The number of seconds in a day.
#define SECONDS_PER_DAY (24 * 60 * 60)

TRDSTime rdsTime_fromRDS(UINT32 mjd, UINT32 hours, UINT32 minutes);
TRDSTime rdsTime_fromGPS(UINT32 gpsDate, UINT32 gpsUTC);
void rdsTime_toGPS(TRDSTime t, UINT32 *gpsDate, UINT32 *gpsUTC);
BOOL8 rdsTime_isValid(TRDSTime t);
int rdsTime_compare(TRDSTime t1, TRDSTime t2);
INT32 rdsTime_differenceSeconds(TRDSTime t1, TRDSTime t2);
TRDSTime rdsTime_addSeconds(TRDSTime t, INT32 seconds);
TRDSTime rdsTime_addMinutes(TRDSTime t, INT32 minutes);
TRDSTime rdsTime_addDays(TRDSTime t, INT32 days);
UINT32 rdsTime_getSeconds(TRDSTime t);
UINT32 rdsTime_getTimeOfDay(TRDSTime t);
TRDSTime rdsTime_setTimeOfDay(TRDSTime t,
                              UINT8 hours, UINT8 minutes, UINT8 seconds);
TRDSTime rdsTime_setMinutesFromMidnight(TRDSTime t, UINT16 minutes);
UINT8 rdsTime_getDayOfWeek(TRDSTime t);
const char *rdsTime_toString(TRDSTime t);

#ifdef __cplusplus
}
#endif

#endif
