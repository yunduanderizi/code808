#ifndef ENMLOCK_H
#define ENMLOCK_H

#include <innovEngine_types.h>
#include <mapdata/enroad.h>
#include <location/locmaplocation.h>
#include <locationprovider/lpjourneyloc.h>
#include <geosearch/geosearch.h>

#define PREFERRED_PATH_MAX_DEFLECTION 57 //80 * (256/360) = 57

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Maplock
/// These functions are the public interface to the internal representation
/// of the maplock data.
/// @ingroup LocationProviders
/// @{

/*----------------------------------------------------------------*/
/** Resets the maplock variables, and sets the locked position to badvalue.

    @return TRUE if the maplock was succesfully reset. Otherwise FALSE */
extern MAPCORE_API BOOL8 maplock_reset( void );

/** Pass the current journey to maplock. Set this before attempting to use
    maplock_updateJourney(TJourney *defaultJourney). By default uses the journey
    from the old API (path_getDefaultJourney()) */
extern MAPCORE_API void maplock_setJourney( TJourney *defaultJourney);

/** Update maplock. Takes in a Journey or NULL */
extern MAPCORE_API void maplock_updateJourney( TJourney *defaultJourney );

/** Finds the last valid position that was locked to a road. This function
    can be used if the GPS is not responding and a valid road position is
    needed.

    @param result A pointer to the position (lat/lon) that was last valid. */
extern MAPCORE_API void maplock_getLastValidPos( TPos *result );

/** Gets the speed as calculated from the maplock.

    @return Current maplock speed in meters per second. */
extern MAPCORE_API INT32 maplock_getSpeed( void );

/** Gets the heading as calculated from the maplock.

    @return Current maplock heading, where 0 is due north, 64 is due east,
    128 is due south and 192 is due west. */
extern MAPCORE_API UINT8 maplock_getHeading( void );

/** Determines if the maplocked position is on a base map,
    if there is a lock to the road. If there is no lock
    to a road then FALSE is returned.

    @return TRUE if there is a lock to a road and the road is on a base map,
    FALSE otherwise. */
extern MAPCORE_API BOOL8 maplock_isOnBaseMap( void );

/** Find if the current maplock position has successfully locked to a
    road.

    @return TRUE if the maplock position is a valid road segment. Otherwise
    FALSE. */
extern MAPCORE_API BOOL8 maplock_isValid( void );

/** Returns a pointer to the name of the road that is currently locked
    to.

    @return A pointer to the name of the locked road, or NULL of no road
    is currently locked. */
extern MAPCORE_API UTF16 *maplock_getLockName( void );

/** Find whether the current road lock position is on the current route.
    The intention of this routine is to determine whether the driver is
    following the route.  If there is more than one leg of a journey, then
    this routine will always check whether we are locked to the current
    leg.  Potentially it may also check to see whether the road lock is to
    other legs of the journey.  To find out exactly which leg the roadlock
    maps to, use the API maplock_destinationLockedTo().

    @return TRUE if the locked road position is on the current route,
    FALSE if the current locked position is not on the route, or if there is
    currently no route */
extern MAPCORE_API BOOL8 maplock_isOnRoute( void );

/** Determine whether the current roadlock position is on the current
    route and return which leg of the route we are locked to.

    @param[out] destinationNumber
    The locked road position is on the a leg of a journey
    that terminates at destination destinationNumber.

    @return TRUE if maplock_isOnRoute() returns TRUE
    FALSE if the current locked position is not on the route, or if there is
    currently no route */
extern MAPCORE_API BOOL8 maplock_destinationLockedTo( TJourneyLegNumber *destinationNumber );

/** Get the current maplocked journey location. The journey location represents
    a linear point location on a journey.

    @param[out] pLoc Current location on the current journey.
    @return
       - TRUE if maplock is locked to the journey and the output location is
         valid.
       - FALSE if maplock is not locked to the journey. */
extern MAPCORE_API BOOL8 maplock_getJourneyLocation(TJourneyLocation *pLoc);

/** Find the class of the road as determined by the current roadlock.

    @return The road type of the current lock.  ROAD_TYPE_NO_ROAD if not
    currently locked to a valid road */
extern MAPCORE_API TRoadType maplock_roadType( void );

/** Finds the map location of the currently locked road segment

    @param mapLocation The map location structure. */
extern MAPCORE_API BOOL8 maplock_getMapLocation( TMapLocation* mapLocation );


/// Returns the speed limit for the current maplocked road in the current
/// direction. Zero is returned if
///   - the speed limit data is not populated.
///   - there is no speed limit for the given road. This might occur,
///      for example, if the road is a walkway or if the road is an autobahn
///   - there is no maplock or the heading cannot be determined.
///
/// Notes:
///   - There is no way of distinguishing the case of unpopulated mapdata
///      and no speed limit.
///   - The road speed returned is in locale specific units, e.g., MPH
///      for the UK/US, KPH elsewhere.
/// @param[out] pRoadSpeed The speed restriction in the direction of travel.
/// @param[out] pRoadSpeedUnit The unit of measurement
/// @return FALSE if there was no maplock, or the heading could not be
///         determined. TRUE otherwise.
extern MAPCORE_API
BOOL8 maplock_getSpeedRestriction( TRoadSpeed     *pRoadSpeed,
                                   TRoadSpeedUnit *pRoadSpeedUnit );

/* Function to help debug the maplocking.  Gets the number of possible
   locked positions. */
extern MAPCORE_API UINT8 maplock_getNumPositions( void );

/* Function to help debug the maplocking. Gets the certainty of each
   possible maplock position. */
extern MAPCORE_API UINT16 maplock_getLockCertainty( UINT8 pos );

/* Function to set the vehicle type that maplock should assume */
extern MAPCORE_API
void maplock_setVehicleType( TVehicleType vehicleType );  

/* Function to get the vehicle type that maplock currently uses */
extern MAPCORE_API
TVehicleType maplock_getVehicleType(void);

//@MIC check the speed camera is in forward segments from current segment.
extern MAPCORE_API
BOOL8 maplock_getPreferredPath(UTF16 range,                   //[in] maxmum of expand distance (meters).
                               UTF16 maxExpandSegments,       //[in] maxmum number of expand segments.
                               TSegmentID *preferredPath,     //[out] segments id of preferred path.
                               UINT16 *expandSegments,        //[out] number of segments in preferred path.
                               INT32 *distanceToCamera,       //[out] distance from current pos to speed-camera.
                               UINT8* heading,                //[out] heading of speed-camera.
                               UINT16* speedLimited,          //[out] speedLimited of this speed-camera (0 is unknow speed limit).
                               UTF16* name);                  //[out] name of speed-camera.


/**
 * Enable/disable speed limit display
 *
 * @param[in] onOff TRUE to enable, FALSE to disable
 * @return previous setting so caller can save it and
 *         restore it.
 */
//@todo remove this when application supports the feature
BOOL8 speedLimitDisp_enable(BOOL8 onOff);

/**
 * Set the location in screen coordinates where to
 * render the speed limit
 *
 * @param[in] pos_s position where to render speed limit in
 *                  screen coordinates
 * @return previous location so caller can save it and
 *         restore it.
 */
//@todo remove this when application supports the feature
TPos speedLimitDisp_setPos(const TPos *pos_s);


/// @}

#ifdef __cplusplus
}
#endif

#endif
