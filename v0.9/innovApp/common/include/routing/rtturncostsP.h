#ifndef RTTURNCOSTSP_H
#define RTTURNCOSTSP_H

#include <innovEngine_types.h>
#include <mapdata/enroadP.h>
#include <routing/rtsearchnodeP.h>
#include <location/locmaplocationP.h>


#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------*/
/* TYPE DEFINITIONS */
/*----------------------------------------------------------------*/

/// The maximum number of expansions per expansion set
#define EXPANSION_SET_MAX_EXPANSIONS  (ROAD_NODE_MAX_SEGMENTS-1)

/// This structure stores all additional turn cost information, needed for
/// all expansions of an expansion set.
typedef struct TTurnCosts 
{
  /// turn cost factor depending on the distance from the start, the initial speed
  /// and the general turn cost setting
  UINT32              costFactor;

  // road classification data - used by turn cost calculation

  /// the most major road class of the expansion set
  TRoadType           majorRoadClass;
  /// the number of incoming expansions that are more minor than majorRoadClass
  UINT32              incomingMinorSegmentCount;
  /// the number of incoming expansions that are of type majorRoadClass
  UINT32              incomingMajorExpansionCount;
  /// the incoming expansions that are of type majorRoadClass
  struct TExpansion*  incomingMajorExpansions[EXPANSION_SET_MAX_EXPANSIONS];
} TTurnCosts;


/*----------------------------------------------------------------*/
/* MORE INCLUDES */
/*----------------------------------------------------------------*/
#include <routing/rtexpansionP.h>


/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/

/// Initialises the turn data that is needed for the whole expansion set.
///
/// @param[in,out] expansionSet
///   A pointer to the expansion set of which the turn data shall be
///   initialised.
void turnCosts_initialize( 
  struct TExpansionSet *expansionSet );

/// Calculates the turn cost, that shall be applied if the route is heading
/// in the opposite direction to the moving direction (of the origin).
///
/// @param[in]  loc
///   The location of the origin.
/// @param[in]  turnCostPreference
///   The turn cost preference of the routing options.
/// @param[in]  originSpeed
///   The speed of the origin in m/s.
/// @return
///   the turn cost to apply to the initial search node.
UINT32 turnCosts_getOriginReverseHeadingCost(
  TMapLocation* loc,
  UINT32 turnCostPreference,
  UINT32 originSpeed );

/// Calculates the turn cost, that shall be applied if the route is arriving
/// in opposite direction as the destination is orientated. What that in 
/// reality means: The cost of arriving at the wrong side of the road.
///
/// @param[in]  loc
///   The location of the destination.
/// @param[in]  turnCostPreference
///   The turn cost preference of the routing options.
/// @return
///   the turn cost to apply to the initial search node.
UINT32 turnCosts_getDestinationReverseHeadingCost(
  TMapLocation* loc,
  UINT32 turnCostPreference );

/// Calculates the cost of a bifurcation manoeuvre.
///
/// @param[in]  pThis
///   The turn cost data structure of the current expansion.
/// @return
///   the cost of the bifurcation manoeuvre.
UINT32 turnCosts_getBifurcationCost(
  const TTurnCosts* pThis ) ;

/// Calculate the total cost of turning from the backSegment to the
/// expandedSegment (in driving order).  The calculation takes into account
/// the degree of the turn as well as the types of roads involved.
///
/// @param[in]  pThis
///   The expansion for the turn costs shall be calculated.
/// @return
///   TRUE if it's a valid turn or FALSE if not
BOOL8 expansion_checkAndCostTurn(
  struct TExpansion* pThis );


#ifdef __cplusplus
}
#endif

#endif // RTTURNCOSTSP_H
