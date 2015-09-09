//
// Declaration of global object to support api simplification
//
// $Id: rtapibase.h 9567 2008-03-13 03:54:28Z sajeewa.dayaratne $
//

// To simplify the API exposed to the application each of the major object
// types requires a global instance.
//
// This module declares those instances

#ifndef RTAPIBASE_H
#define RTAPIBASE_H

#include <routing/rtjourneycalc.h>
#include <routing/rtoptions.h>
#include <routing/rtitinerary.h>
#include <routing/rtjourney.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* JOURNEY CALCULATOR TYPES */
/*----------------------------------------------------------------*/

//## I have decided to hide the journey calculator types.  It makes little
//## sense to declare multiple journey calculators, and to expose the type
//## here would involve exposing 14 private types.  A number of these types
//## are not under my control and it would be a pity if we had to change
//## the API headers every time the internals changed.  The core will
//## declare a single global journey calculator that can be manipulated by
//## the following APIs.

// A journey calculator calculates journeys from itineraries.  <b>At this
// time, the core only support a single journey calculator instance</b>.
// This is declared internally within the core

// The result type TJourneyCalculatorResult is declared above in the
// journey section as TJourney is dependent on it

/// @addtogroup Routing
/// @{

/// @addtogroup JourneyCalculator
/// @ingroup Routing
/// @{

/*----------------------------------------------------------------*/
/* JOURNEY CALCULATOR APIs */
/*----------------------------------------------------------------*/
/// Initialise a journeyCalculator in preparation for calculating a new
/// journey from the specified itinerary
///
/// @param[in]  startLocation Position to use as the start of the current leg
///               (usually the current maplock location)
/// @param[in]  startSpeed Speed to use for determining high turn costs at the
///               start of the current leg (usually the current maplock speed)
/// @param[in]  itinerary Itinerary of destinations to calculate a journey for
/// @param[out] journey Initialised journey object to populated with the 
///               calculated paths
/// @return
///   - JOURNEYCALC_RESULT_BAD_SEQUENCE if init has been called out of turn
///   - JOURNEYCALC_RESULT_BAD_PARAM if the itinerary is empty
///   - JOURNEYCALC_RESULT_READY_T0_START if init completed successfully
extern MAPCORE_API 
TJourneyCalculatorResult apiJourneyCalculator_initialise( 
  const TMapLocation*       startLocation,
        UINT32              startSpeed,
  const TItinerary*         itinerary,
        TJourney*           journey );

//@MIC start
extern MAPCORE_API 
TJourneyCalculatorResult apiJourneyCalculator_initialiseSTR( 
  const TMapLocation*       startLocation,
        UINT32              startSpeed,
  const TItinerary*         itinerary,
        TJourney*           journey );
//@MIC end

/// Initialise a journeyCalculator in preparation for rerouting the current
/// leg of the specified journey.
///
/// @deprecated The new api apiJourneyCalculator_reRouteTypeInitialise should
///  be used instead. The old functionality can be obtained by setting the 
///  quickReroute parameter to TRUE 
///
/// @param[in]  startLocation Position to use as the start of the current leg
///               (usually the current maplock location)
/// @param[in]  startSpeed Speed to use for determining high turn costs at the
///               start of the current leg (usually the current maplock speed)
/// @param[out] journey Calculated journey with leg to re-route
/// @return
///   - JOURNEYCALC_RESULT_BAD_SEQUENCE if init has been called out of turn
///   - JOURNEYCALC_RESULT_BAD_PARAM if the itinerary is empty
///   - JOURNEYCALC_RESULT_READY_T0_START if init completed successfully
DEPRECATED(
extern MAPCORE_API 
TJourneyCalculatorResult apiJourneyCalculator_reRouteInitialise( 
  const TMapLocation*       startLocation,
        UINT32              startSpeed,
        TJourney*           journey )
);

/// Initialise a journeyCalculator in preparation for rerouting the current
/// leg of the specified journey.
///
/// @param[in]  startLocation Position to use as the start of the current leg
///               (usually the current maplock location)
/// @param[in]  startSpeed Speed to use for determining high turn costs at the
///               start of the current leg (usually the current maplock speed)
/// @param[in]  quickReroute If TRUE the reroute calculation will use the old
///               path when performing a reroute, a full reroute will be 
///               performed otherwise.
/// @param[out] journey Calculated journey with leg to re-route
/// @return
///   - JOURNEYCALC_RESULT_BAD_SEQUENCE if init has been called out of turn
///   - JOURNEYCALC_RESULT_BAD_PARAM if the itinerary is empty
///   - JOURNEYCALC_RESULT_READY_T0_START if init completed successfully
extern MAPCORE_API 
TJourneyCalculatorResult apiJourneyCalculator_reRouteTypeInitialise( 
  const TMapLocation*       startLocation,
        UINT32              startSpeed,
        BOOL8               quickReroute,
        TJourney*           journey );

/// Iterate an initialised journeyCalculator progressively calculating
/// paths as required for the journey.
///
/// @param[in]  maxIterations The maximum number of A* expansions to process
/// @return
///   - JOURNEYCALC_RESULT_BAD_SEQUENCE if iterate has been called out of turn
///   - JOURNEYCALC_RESULT_FINISHED if the journey calculation is complete
///   - JOURNEYCALC_RESULT_ERROR if journey calculation is complete but
///       calculation of one or more legs failed
///   - >= JOURNEYCALC_RESULT_STARTED && < JOURNEYCALC_RESULT_FINISHED
///       the proportion of the journey calculation completed so far
extern MAPCORE_API 
TJourneyCalculatorResult apiJourneyCalculator_iterate( 
        UINT32 maxIterations );

//@MIC start
/// Iterate an initialised journeyCalculator progressively calculating
/// paths as required for the journey.
///
/// @param[in]  maxIterations The maximum number of A* expansions to process
/// @param[in]  skipNode Fistr N node will skip turn restriction
/// @param[in]  skipRoadType Which road type will skip turn restriction
/// @return
///   - JOURNEYCALC_RESULT_BAD_SEQUENCE if iterate has been called out of turn
///   - JOURNEYCALC_RESULT_FINISHED if the journey calculation is complete
///   - JOURNEYCALC_RESULT_ERROR if journey calculation is complete but
///       calculation of one or more legs failed
///   - >= JOURNEYCALC_RESULT_STARTED && < JOURNEYCALC_RESULT_FINISHED
///       the proportion of the journey calculation completed so far
/// But, this function will skip first N node turn resutriction in some level road
extern MAPCORE_API 
TJourneyCalculatorResult apiJourneyCalculator_iterateSTR( 
        UINT32 maxIterations, UINT8 skipNode, TRoadType skipRoadType);
//@MIC end
  

/// Terminate an initialised journeyCalculator before it has been iterated
/// to completion.
extern MAPCORE_API 
void apiJourneyCalculator_abort( void );

void apiJourneyCalculator_EnableRouteDumpMap(BOOL8 bFlag);
/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif // RTAPIBASE_H
