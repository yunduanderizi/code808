#pragma once
/** @file 
  (c) 2008 navman new zealand limited. all rights reserved.
*/
#ifndef _PLAT_TIME_H_
#define _PLAT_TIME_H_
#include "object.h"

namespace navman {

/*
GetLocalTime();
GetSystemTime();
SetSystemTime();
SetTimeZoneInformation();
SystemTimeToFileTime();
FileTimeToSystemTime();
UnixTimeToFileTime();
flash_setLastTime();
flash_getLastTime();
flash_isLastTimeMoreRecent();
sys_isClockValid();


os_time()/GetTickCount();
*/

class PlatformTime
{
public:

  PlatformTime(void)
  : m_isValid(false)
  {}

  bool getCurrentTime(bool lastKnownTime = true);
  bool setCurrentTime(bool lastKnownTime = true);

  UINT32 year(bool getUTC = false);
  UINT32 month(bool getUTC = false);
  UINT32 day(bool getUTC = false);
  UINT32 hour(bool getUTC = false);
  UINT32 minute(bool getUTC = false);
  UINT32 second(bool getUTC = false);

  bool getUTCTimeInSeconds(void *secondCount64bit);
  bool setUTCTimeInSeconds(void *secondCount64bit);

  bool compareTimes(const PlatformTime &otherTime, UINT32 errorMargin);

  UINT32 yearsInSec(bool getUTC = false);
  UINT32 monthsInSec(bool getUTC = false);
  UINT32 daysInSec(bool getUTC = false);

private:
  PlatformTime(const PlatformTime &x);

#ifdef _POSIX
  bool Assign(const time_t &fromTime);

  time_t m_UTC_time_t;
  tm m_UTC_tm;
  tm m_Local_tm;
#else
  bool Assign(const FILETIME &fromTime);

  SYSTEMTIME m_UTC_SYSTEMTIME;
  SYSTEMTIME m_Local_SYSTEMTIME;
  FILETIME m_UTC_FILETIME;
#endif

  bool m_isValid;
};
}
#endif // _PLAT_TIME_H_
