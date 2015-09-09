//
// Declaration of functionality associated with the expansion set
// and expansion.
//
// $Id: rtexpansionP.h 10913 2008-08-13 12:24:10Z irene.peng $
//

#ifndef RTEXPANSIONP_H
#define RTEXPANSIONP_H


#include <innovEngine_types.h>
#include <mapdata/enroadP.h>
#include <routing/rtsearchnodeP.h>
#include <routing/rtpathcalcP.h>
#include <routing/rtbanareasP.h>


#ifdef __cplusplus
extern "C" {
#endif



/*----------------------------------------------------------------*/
/* FORWARD DECLARATION OF TYPES */
/*----------------------------------------------------------------*/
struct TExpansionSet;
struct TExpansion;


/*----------------------------------------------------------------*/
/* MORE INCLUDES */
/*----------------------------------------------------------------*/
#include <routing/rtturncostsP.h>


/*----------------------------------------------------------------*/
/* DEFINES */
/*----------------------------------------------------------------*/
#define BLOCKAGE_ALLOWED(segment)                             \
  ( ( (segment).roadType >= ROAD_TYPE_LANE )                  \
    ||                                                        \
    ( (segment).flagForwardsRestriction                       \
      && (segment).flagBackwardsRestriction ) )

#define COST_PASS_BLOCKAGE  10000


/*----------------------------------------------------------------*/
/* CONSTANTS */
/*----------------------------------------------------------------*/




/*----------------------------------------------------------------*/
/* TYPES FOR REAL */
/*----------------------------------------------------------------*/

/// This structure contains all factors that contribute to the costs
/// of an expansion - still separated.
typedef struct TExpansionCost {
  UINT32 turnCost;
  UINT32 travelDistance;
  UINT32 detourDistance; //Detour distance used during exploration mode
  UINT32 turnScaleFactor;
  /// positive for left shift, negative for right shift
  INT32  turnShiftFactor;
  UINT32 travelCost;
  UINT32 travelScaleFactor;
  /// positive for left shift, negative for right shift
  INT32  travelShiftFactor;
  UINT32 totalScaleFactor;
  /// positive for left shift, negative for right shift
  INT32  totalShiftFactor;
} TExpansionCost;


// there can't be more sectors than attached segments and even that
// is already a very conservative assumption
#define SECTOR_BUFFER_MAX_SECTORS     ROAD_NODE_MAX_SEGMENTS


typedef struct TSectorBuffer {
  TRoadSectorHeader*  sectors[SECTOR_BUFFER_MAX_SECTORS];
  UINT8               usedSectors;
} TSectorBuffer;


/// Type to hold data associated with a particular expansion from an
/// expansion set
typedef struct TExpansion {

  // pointer to expansion (the "root" of that expansion)
  struct TExpansionSet* eSet;

  // segment and node we expand into
  TRoadSectorHeader*    forwardSegmentSector;
  TRoadSegment          forwardSegment;
  TRoadSectorHeader*    forwardNodeSector;
  TRoadNode             forwardNode;

  // some additional data for that expansion
  TExpansionCost        cost;
  UINT32                heuristicEstimate;
  TPos                  forwardHeading;
  BOOL8                 onRoute;
  BOOL8                 deadEnd;
  BOOL8                 enteredResidential;

  // flags indicating what data is available
  BOOL8                 validForwardHeading;

} TExpansion;


/// Type to hold all data that the set of expansions from a particular node
/// have in common
typedef struct TExpansionSet {

  // members pointing to pathCalculator and its options
  TPathCalculator*                context;
  const TInternalRoutingOptions*  options;

  // the node we expand and the segment from where we are coming
  TSearchNode*            searchNode;
  TRoadSectorHeader*      roadNodeSector;
  TRoadNode               roadNode;
  TRoadSectorHeader*      backSegmentSector;
  TRoadSegment            backSegment;

  // some additional information about the back segment and the search node
  TPos                    backHeading;
  TRoadTurnRestriction    manoeuvres[ROAD_NODE_MAX_RESTRICTIONS];
  BOOL8                   isAPEMExpansion;     ///< partially expanded manoeuvre?
  BOOL8                   isAHintedExpansion;  ///< intermediate expansion?
  INT8                    driveSide;

  // flags indicating what data is available
  BOOL8                   validBackHeading;
  BOOL8                   validManoeuvres;
  BOOL8                   validTurnCostsSetup;

  // all possible expansions where we might going
  TExpansion              expansions[EXPANSION_SET_MAX_EXPANSIONS];
  UINT8                   expansionCount;

  // the buffer where we store locked sector for recycling
  TSectorBuffer           sectorBuffer;

  // additional data stored for turn cost calculations, which are reused for all
  // expansions of that set
  TTurnCosts              turnCostData;

} TExpansionSet;



/*----------------------------------------------------------------*/
/* EXPANSION COST FUNCTION DECLARATIONS                           */
/*----------------------------------------------------------------*/

// sum up an expansion cost into a single integer
UINT32 expansionCost_accumulate(
  const TExpansionCost* pThis );

// sum up only the travel cost part of expansion cost
UINT32 expansionCost_accumulateTravelCost(
  const TExpansionCost* pthis );

// sum up only the turn cost part of expansion cost
UINT32 expansionCost_accumulateTurnCost(
  const TExpansionCost* pthis );

// zero all elemenst of an expansion cost
void expansionCost_setToZero(
  TExpansionCost* pThis );

// add two expansion cost objects together
void expansionCost_add(
  TExpansionCost* pThis,
  const TExpansionCost* addee );



/*----------------------------------------------------------------*/
/* DECLARATION OF GENERAL FUNCTIONS FOR EXPANSION SETS            */
/*----------------------------------------------------------------*/

// Set up a new expansion set
void expansionSet_initialise(
  TExpansionSet* pThis,
  TPathCalculator* pc,
  TSearchNode* searchNode );

// clean up expansion set data (unlock sectors etc)
void expansionSet_destroy(
  TExpansionSet* pThis );


/*----------------------------------------------------------------*/
/* DECLARATION OF FUNCTIONS FOR LOADING EXPANSION SET DATA        */
/* Do NOT use them directly but rely on the macros below, that    */
/* call these functions only if needed.                           */
/*----------------------------------------------------------------*/

// Populate the back heading of an expansion.  This is the driving
// direction heading either from or to the road node along the back
// segment.
//
// Zero segment lengths are handled by jumping back to the prior segment
// in the path and using it to determine the heading.
//
// NOTE: This returned heading will be 0,0 in rare cases where a back
// heading cannot be determined
//
// @param pThis An init'd expansion set whose back heading is required
// @return FALSE if invalid data (mismatched nodes/segments) are
//         encountered, TRUE otherwise
void expansionSet_makeBackHeadingAvailable(
  TExpansionSet* pThis );

// Set up the turn costs to be used for turns at this point
void expansionSet_setupTurnCosts(
  TExpansionSet* pThis );

// Populate the manoeuvres table of the expansion set
void expansionSet_makeManoeuvresAvailable(
  TExpansionSet* pThis );


/*----------------------------------------------------------------*/
/* DECLARATION OF FUNCTIONS FOR LOADING EXPANSION DATA            */
/* Do NOT use them directly but rely on the macros below, that    */
/* call these functions only if needed.                           */
/*----------------------------------------------------------------*/

// Populate the forward node fields of the expansion
void expansion_makeForwardNodeAvailable(
  TExpansion* pThis );

// Populate the forward heading field of the expansion
void expansion_makeForwardHeadingAvailable(
  TExpansion* pThis );



/*----------------------------------------------------------------*/
/* MACROS THAT ENSURE THAT EXPANSION DATA IS AVAILABLE            */
/* These macros call the according function to load the expansion */
/* data only if needed.                                           */
/*----------------------------------------------------------------*/

/// Ensure the road node is unpacked, locked and ready for use
#define expansion_requireRoadNode( pThis )

/// Ensure the road node is unpacked, locked and ready for use
#define expansionSet_requireRoadNode( pThis )

/// Ensure the back segment is unpacked, locked and ready for use
#define expansion_requireBackSegment( pThis )

/// Ensure the back segment is unpacked, locked and ready for use
#define expansionSet_requireBackSegment( pThis )

/// Ensure the forward segment is unpacked, locked and ready for use
#define expansion_requireForwardSegment( pThis )

/// Ensure the back heading is calculated and ready for use
#define expansion_requireBackHeading( pThis )             \
  if( !pThis->eSet->validBackHeading ) {                  \
    expansionSet_makeBackHeadingAvailable( pThis->eSet ); \
  }

/// Ensure the turn costs are set up and ready for use
#define expansion_requireTurnCosts( pThis )               \
  if( !pThis->eSet->validTurnCostsSetup ) {                    \
    expansionSet_setupTurnCosts( pThis->eSet );   \
  }

/// Ensure the manoeuvres are unpacked and ready for use
#define expansion_requireManoeuvres( pThis )              \
  if( !pThis->eSet->validManoeuvres ) {                   \
    expansionSet_makeManoeuvresAvailable( pThis->eSet );  \
  }

/// Ensure the forward node is unpacked, (sector is not locked)
#define expansion_requireForwardNode( pThis )             \
  if( !pThis->forwardNodeSector ) {                       \
    expansion_makeForwardNodeAvailable( pThis );          \
  }

/// Ensure the forward heading is calculated and ready for use
#define expansion_requireForwardHeading( pThis )      \
  if( !pThis->validForwardHeading ) {                 \
    expansion_makeForwardHeadingAvailable( pThis );   \
  }



/*----------------------------------------------------------------*/
/* CONVENIENCE MACROS TO OBTAIN EXPANSION DATA                    */
/*----------------------------------------------------------------*/

/// Retrieve the search direction of the search this expansion is being
/// considered for
#define expansion_getSearchDirection( pThis ) \
  (pThis->eSet->context->state.currentList)

/// Retrieve the routing options to be used for this expansion
#define expansion_getOptions( pThis ) \
  (pThis->eSet->options)



/*----------------------------------------------------------------*/
/* EXPANSION CHECK DECLARATIONS */
/*----------------------------------------------------------------*/

// Check if proposed segment expansion involves a road that is more minor
// than the road filter currently allows.
//
// @param[in]
//   Pointer to the expansion to check.
// @return
//   FALSE if the expansion is of a road that is too minor
BOOL8 expansion_checkRoadFilter(
  TExpansion* pThis );


// Check if the forward segment is closed at either end. We allow passing
// a blockage under certain circumstances but give a huge penalty then.
//
// We also check here, if a segment is just a plain dead-end.
//
// @param[in]
//   Pointer to the expansion to check.
// @return
//   TRUE if we want to expand into that segment
BOOL8 expansion_checkAndCostClosedEnds(
  TExpansion* pThis );


// Check if the proposed segment expansion will violate a freeway or motorway
// restriction specified in the routing options.
//
// @return FALSE if the road type or form of way prevents expansion
BOOL8 expansion_checkFreewaysAndMotorways(
  TExpansion* pThis );

// Check if the proposed segment expansion will violate an overpass
// restriction specified in the routing options.
//
// @return FALSE if the road type or form of way prevents expansion
BOOL8 expansion_checkOverpass( 
  TExpansion *pThis );

// Check if the proposed segment expansion will violate an expressway
// restriction specified in the routing options.
//
// @return FALSE if the road type or form of way prevents expansion
BOOL8 expansion_checkExpressway( 
  TExpansion *pThis );

// Check if the route travels through a residential area and dissalow
// the expansion. The only exceptions to this is if we are  either
// a) Attempting to leave a residential area
// b) Attempting to navigate to a residential area
//
// @param[in]
//   Pointer to the expansion to check.
// @return
//   TRUE if we don't break the residential restriction
BOOL8 expansion_checkResidential(
  TExpansion* pThis );


// Check if proposed segment expansion will violate private road
// restrictions.
//
// @return FALSE if a closed restriction prevents expansion
BOOL8 expansion_checkPrivateRoads(
  TExpansion* pThis );


// Check if proposed segment expansion will violate oneway restrictions.
//
// NOTE: Segments restricted in both directions are not regareded as one
// way, but are rather regarded as pedestrian only streets.  Routing to and
// from such segments is allowed, but not routing through them.
//
// @return FALSE if a closed restriction prevents expansion
BOOL8 expansion_checkOneWay(
  TExpansion* pThis );


// Check if proposed segment expansion will violate any obligated-turn
// turn-restrictions.
//
// @return FALSE if obligated restrictions are breeched by this expansion
BOOL8 expansion_checkObligatedRestrictions(
  TExpansion* pThis );


/// Check if the proposed segment expansion matches any bifurcation
/// restrictions and apply a turn cost as appropriate.
///
/// @return FALSE if there are no matching bifurcations for the expansion.
BOOL8 expansion_checkAndCostBifurcations( TExpansion *pThis );


// Check if the proposed segment expansion will enter a ferry segment from
// a non ferry segment and disallow or apply a cost as appropriate.
//
// We specifically allow routing to continue on a dissallowed type if we
// are already on that type.  This is necessary for situations where the
// user chooses to route from or to a disallowed segment type
//
// @return FALSE if the ferry option and flag prevent expansion
BOOL8 expansion_checkAndCostFerryTransition(
  TExpansion* pThis );


BOOL8 expansion_checkUnsealedRestriction(
  TExpansion* pThis );


BOOL8 expansion_checkTollRestriction(
  TExpansion* pThis );


// Check if proposed segment expansion will violate any forbidden-turn
// turn-restrictions.
//
// @return FALSE if a forbidden restriction is breeched by this expansion
BOOL8 expansion_checkForbiddenRestrictions(
  TExpansion* pThis );

//@MIC
// Check if proposed segment expansion will violate any forbidden-turn
// turn-restrictions.
// But, this function will skip first n node turn resutriction
//
// @return FALSE if a forbidden restriction is breeched by this expansion
BOOL8 expansion_checkForbiddenRestrictionsSTR( TExpansion* pThis, 
                                               UINT8 skipNode,
                                               TRoadType skipRoadType );

// Check the proposed segment expansion for segment bans; cost
// appropriately or disallow the ban if necessary.
//
// We specifically allow travel on completely banned segments if the
// search is expanding from a completely banned segment.
//
// @return FALSE if a forbidden restriction is breeched by this expansion
BOOL8 expansion_checkAndCostBanSegments(
  TExpansion* pThis );


/// Apply the cost of some overlaps between a segment and the set of
/// avoidance areas.
///
/// @param[in]       overlaps the avoidance area overlaps
/// @param        numOverlaps the number of overlaps
/// @param           forwards \c TRUE if we are expanding from the
///                           segment's first node to its last, or \c
///                           FALSE if it's the other way around
/// @param segmentVertexCount the number of vertices the segment has
/// @param[in,out]       cost the cost to modify
///
void applyBanAreaCost(
  const TGeobanOverlapArray *overlaps,
  BOOL8                      forwards,
  TExpansionCost            *cost );


// Check the proposed segment expansion for area (geo) bans; cost
// appropriately or disallow the ban if necessary.
//
//
// @return FALSE if a forbidden restriction is breeched by this expansion
BOOL8 expansion_checkAndCostBanAreas(
  TExpansion* pThis );


// Check the proposed segment expansion for dual carriageway u-turns
//
// NOTE: THIS TEST REQUIRES THAT expansion_checkOneWay() AND
// expansion_checkAndCalculateTurnCost() HAVE ALREADY PASSED
//
// This is a big hack until we get new data that allows this to be done
// efficiently
//
// @return FALSE if a forbidden restriction is breeched by this expansion
BOOL8 expansion_checkAndCostDualCarriagewayUTurns(
  TExpansion* pThis );


/// Checks to see if the proposed segment is a minor road that enters
/// a motorway that we recently exited.  This is to prevent the route
/// from taking short detours through rest areas to avoid high-cost
/// segments on motorways, such as those resulting from traffic
/// events.
///
/// @param[in,out] pThis the expansion to check
///
/// @return whether this expansion is allowed
///
BOOL8 expansion_checkAndCostRestAreaDetours( 
  TExpansion* pThis );


/// Check the proposed segment expansion for Traffic events
///
/// @return FALSE if a Traffic event absolutely forbids the expansion
BOOL8 expansion_checkAndCostTrafficEvents( 
  TExpansion* pThis );


/// Check if the proposed segment should be expanded during an 
/// exploration mode path calculation
///
/// @return FALSE if the segment is too far away from the route
BOOL8 expansion_checkAndCostExploration( 
  TExpansion* tpThis );



#ifdef __cplusplus
}
#endif

#endif // RTEXPANSIONP_H
