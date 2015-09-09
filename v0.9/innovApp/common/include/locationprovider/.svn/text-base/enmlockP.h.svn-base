#ifndef ENMLOCKP_H
#define ENMLOCKP_H

#include <locationprovider/enmlock.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AVERAGE_SPEED_SEEDS 4

// BEGIN PRIVATE
typedef struct TMaplockVariables {
  INT32 x, y;           /* The location of the estimate in metres from map center */
  INT16 heading;        /* The estimated heading - between 0 and 255              */
  INT32 speed;          /* The current estimate of the speed in mm/s              */
  INT32 averageSpeed;
  INT32 speedHistory[AVERAGE_SPEED_SEEDS];
  UINT8 speedHistoryIdx;
  TSegmentID segment;
  /*Estimate is between shapePointIndex and  shapePointIndex+1 */
  INT32 shapePointIndex;
  TPos pos;             /* The estimated latitude and longitude */
  BOOL8 distIsToEnd;
  UINT16 roadLength;
  UINT16 distToDestNode;/* Distance in metres to the destination node */
  TNodeID destNodeID;   /* ID number of destination node */
  BOOL8 headingLocked;  /* True if the heading is locked to the road direction */
  TRoadType roadClass;  /* Class of the road */
  BOOL8 lockedToRoute;  /* true if this segment along the route */
  UINT32 score;
  /* route specific info */
  TPathId pathID;       /* route info is only valid if the pathId hasn't changed */
  TJourneyLegNumber destinationNumber; /* dest. number of route locked to */
  INT32 routeIndex;     /* route index in CURRENT route if available */
  BOOL8 connected;      /* whether this is connected to a previous legitimate pos.*/
  BOOL8 hasAddressData;
  BOOL8 allowedForPedMode;
  UINT32 roadSetIndex;
  UINT32 placePairIndex;
} TMaplockVariables;

BOOL8 maplock_init(void);

/** @internal
    Function to help debug the maplocking.  Sets the parameters of the
    maplock algorithm.

    @param[in] base
               base score
    @param[in] dist
               A multiplier that scales the value of the distance penalty.
               The further we are from the road we are trying to lock to
               the greater the penalty.
               The penalty is calculated as the square of the distance
               times this scale factor.
    @param[in] angle
               A multiplier that scales the value of the angle penalty.
               The greater the angle we are from the road we are trying
               to lock to the greater the penalty, especially
               at high speed.
               The penalty is calculated as the angle to the road
               times this scale factor. It is also scaled by the
               speed.
    @param[in] prev
               The bonus for locking to the same segment
               as last time.
    @param[in] route
               The bonus for attempting to lock to a segment
               on the route.
    @param[in] oneWay
               The penalty applied if we try to lock the wrong way
               on a one way road.
    @param[in] pedestrian
               The penalty applied if we try to lock to a pedestrian
               road.
    @param[in] currentDestBonus
               A bonus applied if we are locked to the current leg
               of a journey.
    @param[in] drRadiusSquared
               The maximum radius at which a dr bonus is added to the
               maplock score. This value is the squared value, thus
               if the value is 256, then the dr bonus will be applied
               up to sqrt(256) = 16m. The bonus decreases linearly
               with (squared) distance, so it is maximised if the
               dr estimate is exactly the same as the position.
               E.g., If the drRadiusSquared was 256 then a bonus
               of 256 would be given if the DR estimate and (gps)
               position matched exactly. If the (gps) position
               was 8m away from the DR estimate then the squared
               discrepance would be 8*8 = 64. The bonus would then
               be 256 - 64 = 192. And at 16m discrepancy there
               would be 256 - (16 * 16) = 0 bonus.
               (Note that these calculations assume that
               drBonusScalar is 1. If drBonusScalar was 2, for example,
               these example boni would be doubled.)
    @param[in] drBonusScalar
               Multiply the bonus computed for having a dr match
               by this value.
    @param[in] disconnectedPenalty
               The penalty applied if the road is not connected by roads
               and at a reasonable distance to the current position.
    @return none. */
extern MAPCORE_API void maplock_setLockParameters( INT32 base,
                                                   INT32 dist,
                                                   INT32 angle,
                                                   INT32 prev,
                                                   INT32 route,
                                                   INT32 oneWay,
                                                   INT32 pedestrian,
                                                   INT32 relock,
                                                   INT32 curDestBonus,
                                                   INT32 drRadiusSquared,
                                                   INT32 drBonusScalar,
                                                   INT32 disconnectedPenalty,
                                                   INT32 maxLookAhead,
                                                   TVehicleType vehicle);

TNodeID maplock_getDestNode(void);

UINT32 maplock_segDist(TPos p1, TPos p2, TPos px, INT32* xi, INT32* yi);

TSegmentID maplock_getSegment(void);

INT32 maplock_getShapePointIndex(void);

UINT16 maplock_getDistToDestNode(void);

BOOL8 maplock_isHeadingLocked(void);

/** @internal Return the route index of the segment we are locked to on the
    any route that maplock looks at.

    NB: The current algorithm only attempts to lock to the current leg
    and the previous leg.

    @return The route index into the route
    -1 if we are not locked to the route or there is no route. */
INT32 maplock_getRouteIndex(TJourneyLegNumber *destinationNumber);

BOOL8 maplock_isValid(void);

BOOL8 maplock_isOn(void);

BOOL8 maplock_isOnRoute(void);

void maplock_setOn(BOOL8 on);

void maplock_makeList( const TPos* currentPos );

TMaplockVariables* maplock_getLockVariables(void);

extern void maplock_setJourney( TJourney *journey );

/// @internal
/// Retrieve the local mapLockJourney
///
/// @param[out] journey Returned pointer to journey
/// @return     Always TRUE for now
BOOL8 maplock_getJourney( TJourney** journey );

extern void maplock_updateJourney( TJourney *journey );

/// return speed in metres/sec
extern INT32 maplock_getSpeed(void);

extern UINT8 maplock_getHeading( void );

extern void maplock_getLastValidPos( TPos* result );

extern TRoadType maplock_roadType(void);

// Forward declaration
struct TViewport;
//@todo remove this when application supports the feature
extern void speedLimit_render(const struct TViewport *viewport);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif
