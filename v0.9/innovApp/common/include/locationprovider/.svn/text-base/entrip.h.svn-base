#ifndef ENTRIP_H
#define ENTRIP_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Tripmeter
/// This module provides a set of functions that keep stats on distance, time
/// and speed for the trip, and on the total over the life of the system.
/// The core has no memory of the system totals, so these need to be reset
/// by the application.
/// @ingroup LocationProviders
/// @{

/** Find the distance traveled.

    @return Distance in meters since the last trip reset. */
extern MAPCORE_API UINT32 trip_getDistance( void );

/** Find the length of the journey.

    @return Seconds since the last trip reset. */
extern MAPCORE_API UINT32 trip_getTime( void );

/** Estimate the average speed of the journey.

    @param discountZero If set to TRUE, will not count the time that the
    vehicle was stationary (eg: stopped at traffic lights) for the average
    speed calculation.
    @return The average speed in meters per second. */
extern MAPCORE_API UINT32 trip_getSpeed( BOOL8 discountZero );

/** Estimate the average speed of the journey.

    @param discountZero If set to TRUE, will not count the time that the
    vehicle was stationary (eg: stopped at traffic lights) for the average
    speed calculation.
    @return The average speed in meters per hour. */
extern MAPCORE_API UINT32 trip_getSpeedExt( BOOL8 discountZero );

/** Find the highest estimated speed for the journey.

    @return The maximum speed in meters per second. */
extern MAPCORE_API UINT32 trip_getMaxSpeed( void );

/** Find the highest estimated speed for the journey.

    @return The maximum speed in meters per hour. */
extern MAPCORE_API UINT32 trip_getMaxSpeedExt( void );

/** Find the amount of time that the vehicle was stationary.

    @return The number of seconds the vehicle was stationary. */
extern MAPCORE_API UINT32 trip_getStationaryTime( void );

/** Find the total distance traveled over the life of the unit.

    @return Distance traveled in meters. */
extern MAPCORE_API UINT32 trip_getTotalDistance( void );

/** Find the total time the systems has been active.
    Note that this value is only valid up to 134 years.

    @return Seconds elapsed over the life of the unit. */
extern MAPCORE_API UINT32 trip_getTotalTime( void );

/** Estimate the average speed of the unit.

    @param discountZero If set to TRUE, will not count the time that the
    vehicle was stationary (eg: stopped at traffic lights) for the average
    speed calculation.

    @return The average speed in meters per second. */
extern MAPCORE_API UINT32 trip_getTotalSpeed( BOOL8 discountZero );

/** Estimate the average speed of the unit.

    @param discountZero If set to TRUE, will not count the time that the
    vehicle was stationary (eg: stopped at traffic lights) for the average
    speed calculation.

    @return The average speed in meters per hour. */
extern MAPCORE_API UINT32 trip_getTotalSpeedExt( BOOL8 discountZero );

/** Find the highest estimated speed for unit.

    @return The maximum speed in meters per second. */
extern MAPCORE_API UINT32 trip_getTotalMaxSpeed( void );

/** Find the highest estimated speed for unit.

    @return The maximum speed in meters per hour. */
extern MAPCORE_API UINT32 trip_getTotalMaxSpeedExt( void );

/** Find the total amount of time that the vehicle has been stationary.
    Note that this value is only valid up to 134 years.

    @return The number of milliseconds the vehicle has been stationary. */
extern MAPCORE_API UINT32 trip_getTotalStationaryTime( void );

/** Reset the trip counters. */
extern MAPCORE_API void trip_resetCounter( void );

/** Reset the total trip counters. */
extern MAPCORE_API void trip_resetTotalCounter( BOOL8 areYouSure );

/** Sets the total counters for the unit. Should be called once at startup
    after loading these figures from a file.

    @param distance Total distance traveled in meters.
    @param time Total time of travel in milliseconds.
    @param zeroTime Total number of seconds that the vehicle has been 
    stationary.
    @param maxSpeed Maximum speed reached in meters per second. */
extern MAPCORE_API void trip_setTotalStats( UINT32 distance,
                                            UINT32 time,
                                            UINT32 zeroTime,
                                            UINT32 maxSpeed );

/** Sets the total counters for the unit. Should be called once at startup
    after loading these figures from a file.

    @param distance Total distance traveled in meters.
    @param time Total time of travel in seconds.
    @param zeroTime Total number of seconds that the vehicle has been 
    stationary.
    @param maxSpeed Maximum speed reached in meters per hour. */
extern MAPCORE_API void trip_setTotalStatsExt( UINT32 distance,
                                            UINT32 time,
                                            UINT32 zeroTime,
                                            UINT32 maxSpeed );

/** Sets the allowable downtime in the odometer to allow the logging 
    to be valid. That is, if the time between logging samples exceeds
    this ammount, that period will not be logged. 
    Set to zero to allow unlimited time between samples.
    Default value is 300 (5 minutes).

    @param s Maximum number of seconds to allow for downtime. */
extern void trip_setMaxDownTime( UINT32 s );

/** Returns the current allowable odometer downtime.
    That is, if the time between logging samples exceeds
    this ammount, that period will not be logged. 
    A return value of zero is unlimited time between samples.

    @return Maximum number of seconds of downtime. */
extern UINT32 trip_getMaxDownTime(void);

/// @}

#ifdef __cplusplus
}
#endif

#endif
