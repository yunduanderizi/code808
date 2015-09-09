#ifndef RTJOURNEYCALCP_H
#define RTJOURNEYCALCP_H

#include <routing/rtjourneycalc.h>
#include <routing/rtpathcalcP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// State of the a journey calculator - to allow simple call sequence
/// checking (e.g. must initialise before iterating)
typedef enum {
  JOURNEYCALC_STATE_IDLE = 0,
  JOURNEYCALC_STATE_INITIALISED
} TJourneyCalculatorState;


/*----------------------------------------------------------------*/
/* MORE INCLUDES */
/*----------------------------------------------------------------*/
/// This is needed here (rather than at the top of the file) because of the
/// cross depedency between the journey type and the journey calculator
#include <routing/rtjourneyP.h>


/*----------------------------------------------------------------*/
/* MORE TYPES */
/*----------------------------------------------------------------*/
/// Type representing a partially complete journey calculation.
/// This data should not be manipulated directly
typedef struct TJourneyCalculator {
  TJourneyCalculatorState state;
  TPathCalculator*        pathCalculator;
  TJourneyLegNumber       calculatingLeg;
  BOOL8                   isReRouting;
  BOOL8                   isFastReRoute;
  struct TJourney*        journey;
  BOOL8                   someLegsHaveErrors;
  UINT32                  startSpeed;

  // member used for logging
  UINT32                  startTime;
  UINT32                  counter;
} TJourneyCalculator;



/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// Initialise a journeyCalculator in preparation for calculating a new
// journey from the specified itinerary
//
// @param[in]  pThis Journey calculator to initialise
// @param[in]  pathCalculator Path calculator to use for calculating
//               individual paths in the journey
// @param[in]  startLocation Position to use as the start of the current leg
//               (usually the current maplock location)
// @param[in]  startSpeed Speed to use for determining high turn costs at the
//               start of the current leg (usually the current maplock speed)
// @param[in]  itinerary Itinerary of destinations to calculate a journey for
// @param[out] journey Initialised journey object to populated with the 
//               calculated paths
// @return
//   - JOURNEYCALC_RESULT_BAD_SEQUENCE if init has been called out of turn
//   - JOURNEYCALC_RESULT_BAD_PARAM if the itinerary is empty
//   - JOURNEYCALC_RESULT_READY_TO_START if init completed successfully
TJourneyCalculatorResult journeyCalculator_initialise( 
        TJourneyCalculator* pThis,
        TPathCalculator*    pathCalculator,
  const TMapLocation*       startLocation,
        UINT32              startSpeed,
  const TItinerary*         itinerary,
        struct TJourney*    journey );


//@MIC start
TJourneyCalculatorResult journeyCalculator_initialiseSTR( 
        TJourneyCalculator* pThis,
        TPathCalculator*    pathCalculator,
  const TMapLocation*       startLocation,
        UINT32              startSpeed,
  const TItinerary*         itinerary,
        struct TJourney*    journey );
//@MIC end


// Initialise a journeyCalculator in preparation for rerouting the current
// leg of the specified journey.
//
// @param[in]  pThis Journey calculator to initialise
// @param[in]  pathCalculator Path calculator to use for calculating
//               individual paths in the journey
// @param[in]  startLocation Position to use as the start of the current leg
//               (usually the current maplock location)
// @param[in]  startSpeed Speed to use for determining high turn costs at the
//               start of the current leg (usually the current maplock speed)
// @param[out] journey Calculated journey with leg to re-route
// @return
//   - JOURNEYCALC_RESULT_BAD_SEQUENCE if init has been called out of turn
//   - JOURNEYCALC_RESULT_BAD_PARAM if the itinerary is empty
//   - JOURNEYCALC_RESULT_READY_TO_START if init completed successfully
TJourneyCalculatorResult journeyCalculator_reRouteInitialise( 
        TJourneyCalculator* pThis,
        TPathCalculator*    pathCalculator,
  const TMapLocation*       startLocation,
        UINT32              startSpeed,
        BOOL8               reusePath,
        struct TJourney*    journey );


// Iterate an initialised journeyCalculator progressively calculating
// paths as required for the journey.
//
// @param[in]  pThis Journey calculator to iterate
// @param[in]  maxIterations The maximum number of A* expansions to process
// @return
//   - JOURNEYCALC_RESULT_BAD_SEQUENCE if iterate has been called out of turn
//   - JOURNEYCALC_RESULT_FINISHED if the journey calculation is complete
//   - JOURNEYCALC_RESULT_ERROR if journey calculation is complete but
//       calculation of one or more legs failed
//   - >= JOURNEYCALC_RESULT_STARTED && < JOURNEYCALC_RESULT_FINISHED
//       the proportion of the journey calculation completed so far
TJourneyCalculatorResult journeyCalculator_iterate( 
  TJourneyCalculator* pThis,
  UINT32 maxIterations );

//@MIC start
TJourneyCalculatorResult journeyCalculator_iterateSTR( 
  TJourneyCalculator* pThis,
  UINT32 maxIterations,
  UINT8 skipNode,
  TRoadType skipRoadType);
//@MIC end
  

// Terminate an initialised journeyCalculator before it has been iterated
// to completion.
//
// @param[in]  pThis Journey calculator to abort
void journeyCalculator_abort( 
  TJourneyCalculator* pThis );


// Call after allocating space for a journey calculator to set it in a
// known/good/idle state
//
// @param[in]  pThis Journey calculator to clear
void journeyCalculator_clear( 
  TJourneyCalculator* pThis );


#ifdef __cplusplus
}
#endif

#endif // RTJOURNEYCALCP_H
