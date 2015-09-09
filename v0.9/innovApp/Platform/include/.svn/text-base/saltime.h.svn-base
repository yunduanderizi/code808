#ifndef SALTIME_H_
#define SALTIME_H_

#if defined(LINUX)||defined(QNX)
#include <sys/time.h>
#endif

#include "libSAL.h"

/** @file SALTime.h
 *  @brief Provides simple time routines.
 */

/** sal_sleep delays execution for a specified amount of time.
 *  @param ms number of milliseconds to delay.
 */
static void sal_sleep(int32_t ms);

/** Provides a tick count which always increases. The tick count
  * may start from zero when the application starts, or from some
  * other arbitrary point. This count should only be used to measure
  * differences, so the epoch is not important.
  * @return number of milliseconds.
  */
static uint32_t sal_tickCount(void);

#if defined(WIN32) || defined(_WIN32_WCE)
static __inline void sal_sleep(int32_t ms) {
	Sleep(ms);
}

static __inline uint32_t sal_tickCount(void) {
	return (uint32_t)GetTickCount();
}

#elif defined(LINUX) || defined(QNX)

static __inline void sal_sleep(int32_t ms) {
	struct timespec t;
	t.tv_sec = ms / 1000;
	t.tv_nsec = (ms % 1000) * 1000000L;
	nanosleep(&t, 0);
}

static __inline uint32_t sal_tickCount(void) {
	struct timeval t;
	gettimeofday(&t, 0);
	return (uint32_t)((t.tv_sec * 1000L) + (t.tv_usec / 1000L));
}

#endif /* WIN32 */

#endif /* SALTIME_H_ */

