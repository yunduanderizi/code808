//
// Declaration of types and functions for the single stop A* route calculator
//
// $Id: rtpathcalcP.h 10913 2008-08-13 12:24:10Z irene.peng $
//

#ifndef RTPATHCALCP_H
#define RTPATHCALCP_H

#include <containers/gestaticheapP.h>
#include <mapdata/enroadP.h>
#include <routing/rtmapdumpP.h>
#include <routing/rtoptionsP.h>
#include <routing/rtpathcalc.h>
#include <routing/rtpathlistP.h>
#include <routing/rtsearchdataP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* MORE INCLUDES */
/*----------------------------------------------------------------*/
// TPath depends on the above types so including rtpath.h before here will
// not work
#include <routing/rtpath.h>
#include <routing/rtmanoeuvrestoreP.h>
#include <routing/rtexpansionhintsP.h>


/*----------------------------------------------------------------*/
/* MORE TYPES */
/*----------------------------------------------------------------*/
/// Special costs returned to the path calculator call back
typedef struct TPathCalculatorExploreCost {
  UINT32 routeCost;
  UINT32 travelDistance;
  UINT32 detourDistance;
} TPathCalculatorExploreCost;

/// Status returned by the callback
typedef enum {
  PATHCALC_CALLBACK_SUSPEND,    /* Suspend the path calculation for now       */
  PATHCALC_CALLBACK_CONTINUE,   /* Keep processing the path                   */
  PATHCALC_CALLBACK_COMPLETE    /* Complete the path calculation and clean up */
} TPathCalculatorCallbackStatus;

/// User supplied call back function that will get called for each explored segment
/// during an 'exploration mode' path calculation.
typedef TPathCalculatorCallbackStatus TPathCalculatorExploreCallback(
  void                              *context,
  TSearchNode                       *searchNode,
  TRoadSegment                      *segment,
  TRoadNode                         *node,
  UINT32                             travelProp,
  const TPathCalculatorExploreCost  *pathCost,
  const TPathCalculatorExploreCost  *segCost );

/// The state or context of a path calculator
typedef struct TPathCalculatorState {
  TPathList           currentList;
  TSearchNode         *oneSegmentSolution;
  const TSearchNode   *minSolution[2];
  UINT32              expansions[2];      ///< num nodes expanded in each search
  BOOL8               residentialRestrictionLifted;
  UINT32              upperBound;
  BOOL8               converging;
  UINT32              convergeNodeLimit;
  BOOL8               convergeNodeLimitChanged;
  UINT32              reRouteSearchNodes;
  TRoadType           roadTypeFilter;
  UINT32              nodeLimit;
  UINT32              nodesExplored;
  TRoadMapLayer       layer;
  TAStarSearchData    *searchData;        /// ORIGIN/DESTINATION
  BOOL8               pathFound;          ///< set when ready to return a result
  TPartExpManoeuvres  pems;
  TExpansionHints     eHints;
  TMapdumpFile        glueNodeDump;
  TMapdumpFile        convergencePointDump;
  TMapdumpFile        avoidedUTurnDump;
} TPathCalculatorState;


/// All data required for a path calculation to proceed
typedef struct TPathCalculator {
  TInternalRoutingOptions         options;
  TPathCalculatorState            state;
  TAStarSearchData                searchData[2][2];
  TMapLocation                    startLoc;
  TMapLocation                    endLoc;
  TMapLocationNetwork             startLocNet;
  TMapLocationNetwork             endLocNet;
  const TPath                     *oldPath;
  TPath                           *path;
  BOOL8                           forceCheapest;
  BOOL8                           disableImplicit;
  BOOL8                           trafficReRoute;
  BOOL8                           exploreMode;
  BOOL8                           reRoute;           // TODO: remove
  UINT32                          initialSpeed;      // TODO: remove
  UINT32                          maxDetourDistance; //Only used during explore mode
  TSearchNodeAllocator            allocator;
  TStaticHeap                     *foreignStorage;
  TPathCalculatorExploreCallback  *exploreCallback;
  void                            *exploreContext;
  TPathCalculatorCallbackStatus   callbackStatus;
} TPathCalculator;


/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// Set the route calculation engine in preparation for an iterative route
// calculation.
//
// This function together with pathCalculator_iterate() control the
// calculation of a single route through the roading network between two
// points.
//
// pathCalculator_initialise() performs the following tasks
// + Initialises the routing state
// + Determines initial nodes on the roading network from which to start
//     both the origin and destination searches
// + Handles the special case of a single segment route where both the
//     origin and destination are on the same road segment
TPathCalculatorResult pathCalculator_initialise(
  TPathCalculator*       pThis,
  const TRoutingOptions* options,
  TStaticHeap*           foreignStorage,
  TMapLocation*          startLoc,
  TMapLocation*          endLoc,
  UINT32                 startSpeed,
  TPath*                 path,
  const TPath*           oldPath );

/// Set the route calculation engine in preparation for an exploration of
/// drivable paths originating from a given start location.
///
/// This function together with pathCalculator_iterate() allows an arbitrary
/// user supplied function to have access to all the valid paths that originate
/// from the given location.
///
/// pathCalculator_initialise() performs the following tasks
/// + Initialises the routing state
/// + Determines initial nodes on the roading network from which to start
TPathCalculatorResult pathCalculator_initialiseExplore(
  TPathCalculator*                pThis,
  const TRoutingOptions*          options,
  TMapLocation*                   startLoc,
  UINT32                          startSpeed,
  TPath*                          path,
  const TPath*                    oldPath,
  UINT32                          maxDetour,
  UINT32                          maxSearchNodes,
  TPathCalculatorExploreCallback* callback,
  void*                           callbackContext );

// Performs up to maxIterations of the core route calculation algorithm.
TPathCalculatorResult pathCalculator_iterate (
  TPathCalculator* pThis,
  INT32 maxIterations );

//@MIC start
// Performs up to maxIterations of the core route calculation algorithm.
// But, this function will skip first 10 node turn resutriction
TPathCalculatorResult pathCalculator_iterateSTR (
  TPathCalculator* pThis,
  INT32 maxIterations,
  UINT8 skipNode,
  TRoadType skipRoadType);
//@MIC end

// Cleans up memory associated with a path calculation
void pathCalculator_cleanUp( TPathCalculator* pThis );


void pathCalculator_switchMapDumpFlag(BOOL8 bFlag);

/*----------------------------------------------------------------*/
/* JUNCTION CALCULATOR FUNCTIONS USED BY GUIDANCE */
/*----------------------------------------------------------------*/

// A segment on a driving path through a junction/intersection.
typedef struct {
  // The path road segment.
  TSegmentID segmentID;
  // The node by which the driving path exits the segment.
  TNodeID exitNodeID;
} TJunctionPathSegment;


// Use the route calculation engine to traverse a small set of nodes.
//
// Note that this function uses the main path calculator memory store -- so
// this object can not be in existance while a normal path caluclation is
// taking place.
TPathCalculatorResult junctCalculator_getTraversalPath(
  const TRoutingOptions* options,
  TNodeID                startNode,
  TSegmentID             startBackSegment,
  TNodeID                endNode,
  TSegmentID             endBackSegment,
  void*                  segmentList,
  BOOL8                  (*checkSegmentAgainstList)(void*, TSegmentID),
  INT32                  maxPathSegments,
  TJunctionPathSegment   *pathSegList,
  INT32                  *pathSegCount );


#ifdef __cplusplus
}
#endif

#endif // RTPATHCALC_H
