#pragma once
#ifndef _TIME64_H_
#define _TIME64_H_

#include "appcore_platform.h"

namespace navman {

/**
   Time64
   this class represents time in UTC format (100 nanosecond intervals since jan 1, 1601)
*/
class Time64
{
public:
  /**
     @return the number of 100 nanosecond intervals since jan 1, 1601
  */
  UINT64 getUINT64() { return _bits.QuadPart; }

  /**
     set the number of 100 nanosecond intervals since jan 1, 1601
     @param 64bit value representing the UTC
  */
  void   setUINT64(const UINT64& time) { _bits.QuadPart = time; }

  /**
     Fills in the current time represented in UTC
     @param [out] pTime receives the current time
  */
#ifdef _POSIX
  static void getCurrent(Time64 *pTime)
  {
    ASSERT(pTime != NULL);

    struct timeval now;
    gettimeofday(&now, NULL);

    // This is actually since the Jan 1, 1970
    // so some offset should probably apply
    pTime->_bits.QuadPart = (now.tv_sec * 1000000LL + now.tv_usec) * 10LL;
  }

#else

  static void getCurrent(Time64 *pTime)
  {
    ASSERT(pTime != NULL);

    SYSTEMTIME stNow;
    GetSystemTime(&stNow);

    FILETIME ftNow;
    SystemTimeToFileTime(&stNow, &ftNow);

    pTime->_bits.LowPart = ftNow.dwLowDateTime;
    pTime->_bits.HighPart = ftNow.dwHighDateTime;
  }

#endif // _POSIX

protected:
  ULARGE_INTEGER _bits;
};

} // namespace navman

#endif // _TIME64_H_
