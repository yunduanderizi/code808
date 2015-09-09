//
// Declaration of the journey type and functions for manipulating it
//
// $Id: rtjourney.h 9578 2008-03-16 22:16:30Z rohit.grover $
//

#ifndef RTJOURNEY_H
#define RTJOURNEY_H

#include <routing/rtitinerary.h>
#include <routing/rtpath.h>
#include <routing/rtjourneycalc.h>
#include <containers/gestaticheap.h>
#include <location/locmaplocation.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/

/// @addtogroup Routing
/// @{


/// @addtogroup JourneyCalculator
/// @ingroup Routing
/// @{

/// @typedef UINT32 TJourneyLegNumber
/// Type for indexing legs in a journey
typedef UINT32 TJourneyLegNumber;

/// @}

/*----------------------------------------------------------------*/
/* MORE TYPES */
/*----------------------------------------------------------------*/



/// @addtogroup JourneyLeg
/// @ingroup Routing
/// @{

/// @typedef struct TJourneyLeg
/// Representation of a single leg in a journey.  Has an origin loc, a
/// destination loc, and if result = JOURNEYCALC_RESULT_FINISHED, a path of
/// segments between the origin and the desination.
typedef struct TJourneyLeg {
  TMapLocation               origLoc;
  TMapLocation               destLoc;
  TJourneyCalculatorResult   result;
  TPath                      path;
} TJourneyLeg;

/// @}

/// @addtogroup Journey
/// @ingroup Routing
/// @{

/// @typedef struct TJourney
/// A complete journey of legs following as calculated by a journey
/// calculator from an itinerary.
typedef struct TJourney {
  TStaticHeap              foreignStorage;
  TSegmentID*              drivingSegArray;
  TSegmentID*              sortedSegArray;
  TSegmentListStore        drivingSegStore;
  TSegmentListStore        sortedSegStore;
  TJourneyCalculatorResult result;
  TJourneyLegNumber        currentLeg;
  TJourneyLegNumber        legCount;
  TJourneyLeg              leg[ITINERARY_MAX_DESTINATIONS];
  TPath                    freeSegmentPath;
} TJourney;

/*----------------------------------------------------------------*/
/* JOURNEY FUNCTIONS */
/*----------------------------------------------------------------*/
/// Determine the number of legs in a journey
///
/// @param[in] pThis Journey to query
/// @return    The number of legs in the journey
extern MAPCORE_API
TJourneyLegNumber journey_getLegCount(
  const TJourney* pThis );


/// Initialise a journey prior to use.  This <b>must</b> be called before
/// any other API that affects a declared journey.
///
/// NOTES:
///
///   For V3 and Popeye, \p bufferDWords was 25000 for 16-destination
///   journeys.  This can safely be scaled back if the itinerary
///   length is restricted.
///
///   This API calls journey_clear() after setting the buffer up.
///
/// @param[in] pThis Journey to initialise
/// @param[in] buffer Pointer to start of buffer memory
/// @param[in] bufferDWords Available room (dwords) in buffer
extern MAPCORE_API
void journey_initialise(
  TJourney* pThis,
  UINT32* buffer,
  UINT32 bufferDWords );

/// @brief Copies the contents of one journey to another.
///
/// The target journey must have been initialised using
/// #journey_initialise prior to calling this function.  The target
/// journey must have a buffer at least as large as the source
/// journey, and these buffers must not overlap.
///
/// After this function returns \c TRUE the target journey will be an
/// identical copy of the source journey until changes are made to
/// either one; changes made to one will not affect the other.
///
/// @param[in]   pThis the source journey
/// @param[out] target the target journey
///
/// @return \c TRUE if the journey was successfully cloned, \c FALSE
///         otherwise
extern MAPCORE_API
BOOL8 journey_clone(const TJourney *pThis, TJourney *target);


/// Clear a journey, wiping all stored leg and path information.
///
/// @param[in] pThis Journey to clear
extern MAPCORE_API
void journey_clear(
  TJourney* pThis );


/// Determine the result/status of a journey
/// This is the value that was last returned by pathCalculator_iterate()
///
/// @param[in] pThis Journey to query
/// @return    The current result/status of the journey
extern MAPCORE_API
TJourneyCalculatorResult journey_getResult(
  const TJourney* pThis );


/// Get a reference to a particular leg of a journey
///
/// @param[in]  pThis Journey containing leg
/// @param[in]  index Leg index of leg to retrieve
/// @param[out] leg A reference to a leg of the journey
/// @return     TRUE if legNumber was valid, FALSE otherwise
extern MAPCORE_API
BOOL8 journey_getLeg(
  const TJourney* pThis,
  TJourneyLegNumber index,
  const TJourneyLeg** leg );


/// Set the the current leg of the journey.  This is the leg on which the
/// user should currently be driving and receiving guidance for
///
/// @param[in]  pThis The Journey object with the leg
/// @param[in]  index Index of the leg to treat as current
/// @return - TRUE if the current leg was set as requested
///         - FALSE if `index' was invalid
extern MAPCORE_API
BOOL8 journey_setCurrentLeg(
  TJourney* pThis,
  TJourneyLegNumber index );


/// Get the index of the current leg in the journey.  This is the leg on
/// which the user should currently be driving and receiving guidance
/// for
///
/// @param[in]  pThis The Journey object with the leg
/// @param[out] index The index of the current leg
/// @return - TRUE if the returned index is sensible
///         - FALSE if there are no legs in the journey
extern MAPCORE_API
BOOL8 journey_getCurrentLeg(
  const TJourney* pThis,
  TJourneyLegNumber* index );


/// Increment the index of the current leg in the journey.  This is the leg
/// on which the user should currently be driving and receiving guidance for
///
/// @param[in]  pThis The Journey object with the leg
/// @return - TRUE if the index was incremented
///         - FALSE if incrementing the current leg is not sensible
extern MAPCORE_API
BOOL8 journey_incrementCurrentLeg(
  TJourney* pThis );

/// @}

/// @addtogroup JourneyLeg
/// @ingroup Routing
/// @{

/// Determine the result/status of a leg in a journey
///
/// @param[in] pThis Journey leg to get result from
/// @return    Result of queried leg
extern MAPCORE_API
TJourneyCalculatorResult journeyLeg_getResult(
  const TJourneyLeg* pThis );


/// Retrieve the path for a particular leg of the journey
///
/// NOTE: just because a path exists, that does not mean the path has a
/// valid set of path segments.  The return value or leg result needs to be
/// check to determine that.
///
/// @param[in]  pThis Journey lef to get path from
/// @param[out] path reference to retrieved path
/// @return TRUE if the journey leg has result JOURNEYCALC_RESULT_FINISHED,
///   FALSE otherwise
extern MAPCORE_API
BOOL8 journeyLeg_getPath(
  const TJourneyLeg* pThis,
  const TPath** path );


/// Retrieve the location of the origin of a leg
///
/// @param[in]  pThis Journey leg from which to get origin
/// @param[out] orig TMapLocation representing leg origin (copied)
/// @return     TRUE
extern MAPCORE_API
BOOL8 journeyLeg_getOrigin(
  const TJourneyLeg* pThis,
  TMapLocation*      orig );


/// Retrieve the location of the destination of a leg
///
/// @param[in]  pThis Journey leg from which to get destination
/// @param[out] dest TMapLocation representing leg destination (copied)
/// @return     TRUE
extern MAPCORE_API
BOOL8 journeyLeg_getDestination(
  const TJourneyLeg* pThis,
  TMapLocation*      dest );


/// Determine if the destination of a particular leg is on the left or
/// right of the road from the approaching drivers point of view.
///
/// Compares the approach direction taken by the routing algorithm with the
/// direction specified for the destination
///
/// @param[in] pThis Journey leg to get dest side for
/// @return
/// - SIDE_LEFT if the destination is on the left
/// - SIDE_UNKNOWN if the destination side is unknown or the legs path is invalid
/// - SIDE_RIGHT if the destination is on the right
extern MAPCORE_API
TRoadSide journeyLeg_getDestinationSide(
  TJourneyLeg* pThis );

  // forward declaration. Defined in lpjourneyloc.h
struct TJourneyLocation;

extern MAPCORE_API
BOOL8 journey_locIsAhead(const TJourney *pThis,
                               TMapLocation *pSnappedLoc,
                         const struct TJourneyLocation *pJourneyLoc);

extern MAPCORE_API
BOOL8 journey_compare( const TJourney *journey1, 
                       const TJourney *journey2 );

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif // RTJOURNEY_H
