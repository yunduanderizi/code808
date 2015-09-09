#ifndef TMTIMEP_H
#define TMTIMEP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time/tmtime.h>

/// @addtogroup Utilities
/// @{ 

/// @addtogroup Time
/// @ingroup Utilities
/// @{

/// @internal
/// Days of the week
enum {
  TM_SUNDAY = 0,
  TM_MONDAY,
  TM_TUESDAY,
  TM_WEDNESDAY,
  TM_THURSDAY,
  TM_FRIDAY,
  TM_SATURDAY
};

/// @internal
/// Months of the year
enum {
  TM_JANUARY = 0,
  TM_FEBRUARY,
  TM_MARCH,
  TM_APRIL,
  TM_MAY,
  TM_JUNE,
  TM_JULY,
  TM_AUGUST,
  TM_SEPTEMBER,
  TM_OCTOBER,
  TM_NOVEMBER,
  TM_DECEMBER
};

/// @internal
/// The number of seconds per minute
#define TM_SECSPERMIN   60

/// @internal
/// The number of minutes per hour
#define TM_MINSPERHOUR  60

/// @internal
/// The number of hours in one day
#define TM_HOURSPERDAY  24

/// @internal
/// The number of days per week
#define TM_DAYSPERWEEK  7

/// @internal
/// The maximum number of times a given weekday may occur in one calendar month
#define TM_MAXWDAYSPERMONTH 5

/// @internal
/// The number of days in a non-leap year
#define TM_DAYSPERNYEAR 365

/// @internal
/// The number of days in a leap year
#define TM_DAYSPERLYEAR 366

/// @internal
/// The number of seconds per hour
#define TM_SECSPERHOUR (TM_SECSPERMIN * TM_MINSPERHOUR)

/// @internal
/// The number of seconds per day
#define TM_SECSPERDAY (TM_SECSPERHOUR * TM_HOURSPERDAY)

/// @internal
/// The number of months per year
#define TM_MONSPERYEAR  12

/// @internal
/// The start of the time epoch.
#define TM_EPOCH_YEAR  1970
//#define TM_EPOCH_YEAR 2000

#define TM_YEAR_BASE  1900

/* January 1, 1970 C.E. was a Thursday */
#define TM_EPOCH_WDAY  TM_THURSDAY

/* January 1, 2000 C.E. was a Saturday. */
//#define TM_EPOCH_WDAY TM_SATURDAY

extern INT32 time_daysPerYear(BOOL8 yearIsLeap);

extern INT32 time_daysPerMonth(INT32 month, BOOL8 yearIsLeap);

extern BOOL8 time_yearIsLeap(INT32 year);

extern INT32 time_leapDays(INT32 year);

extern INT32 time_daysSinceEpoch(INT32 year, INT32 month, INT32 dayOfMonth);

extern INT32 time_dayOfWeek(INT32 dayOffset);

extern INT32 time_getNthOfMonth(INT32 year,
                                INT32 month,
                                INT32 dayOfWeek,
                                INT32 nth);

extern INT32 time_stringToTime(TTime *pThis, const char *string);

extern INT32 time_year(INT32 time);

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif /* TMTIMEP_H */
