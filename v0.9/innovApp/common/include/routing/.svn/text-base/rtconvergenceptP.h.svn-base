#ifndef RTCONVERGENCEPTP_H
#define RTCONVERGENCEPTP_H

#include <mapdata/enroadP.h>
#include <routing/rtsearchnodeP.h>
#include <routing/rtturncostsP.h>
#include <routing/rtoptions.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
typedef struct TConvergencePoint {
  const TSearchNode*              origNode;
  const TSearchNode*              destNode;
  TRoadSectorHeader*              origSegSector;
  TRoadSectorHeader*              destSegSector;
  TRoadSegment                    origSeg;
  TRoadSegment                    destSeg;
  TRoadNode                       convRoadNode;
  const TInternalRoutingOptions*  options;
  UINT32                          pathCost;
  BOOL8                           validated;
} TConvergencePoint;


/*----------------------------------------------------------------*/
/* EXPOSED FUNCTION INTERFACES */
/*----------------------------------------------------------------*/
// set up the convergence point struct to contain a proposed convergence
// point.
//
// @param pThis
//   The convergence point struct to populate
// @param options
//   The internal routing options used for calculating this path.
// @param firstNode
//   The node at this point from one of the searches
// @param secondNode
//   The node at this point from the other search
// @param firstIsOrigin
//   Is the first node passed in from the ORIGIN search
BOOL8 convergencePoint_initialise(
  TConvergencePoint             *pThis,
  const TInternalRoutingOptions *options,
  const TExpansionSet           *expansionSet,
  const TSearchNode             *otherNode,
  BOOL8                         originSearch );


// Is convergence at the specified point allowed? If so, work out how much
// the path through this convergence point would cost?
//
// @param [in,out]  pThis
//   The convergence point to examine
// @param [in]      expansion
//   The expansion that led to the convergence. It should overlap the other
//   search by one segment.
// @return 
//   TRUE if convergence is allowed here, FALSE otherwise
BOOL8 convergencePoint_validateAndCost(
  TConvergencePoint*    pThis,
  const TExpansionCost* expansionCost );


// Get the total actual cost of the path going through this convergence
// point.  This is the sum of costs from each seach and the turn cost as
// the convergence point.
//
// This function will return an invalid value (UINT32_MAX) if
// convergencePoint_validateAndCost() returned FALSE
//
// @param  pThis The struct representing the convergence point
// @return The total actual cost of the route through this point
UINT32 convergencePoint_getPathCost(
  TConvergencePoint* pThis );

// Check that a convergence point obeys turn restrictions
//
//  A list of nodes and segments representing the path through the
// convergence point is built from the bestNode back segments and passed
// to sequenceObeysTurnRestrictions() for validation
//
// @param pThis The convergence point to check
// @return FALSE if an obligated or forbidden restriction is breached
BOOL8 convergencePoint_checkManoeuvres(
  TConvergencePoint* pThis );


#ifdef __cplusplus
}
#endif

#endif // RTCONVERGENCEPTP_H
