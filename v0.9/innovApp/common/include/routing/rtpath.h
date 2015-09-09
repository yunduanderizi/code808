//
// Declaration of the TPath type and associated functions.
//
// $Id: rtpath.h 10429 2008-07-02 02:49:42Z marcus.spranger $
//

#ifndef RTPATH_H
#define RTPATH_H

#include <routing/rtsegmentlist.h>
#include <routing/rtroadsegment.h>
#include <routing/rtoptions.h>
#include <routing/rtpathcalc.h>
#include <routing/rtsegmentlist.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @{
/// @addtogroup Path
/// @ingroup Routing
/// @{

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// @typedef INT32 TPathId
/// Unique id of a calculated path
typedef INT32 TPathId;

/// @typedef struct TPathStatisticsPointFlags 
/// A statistics sub-structure.  Used only in TPathStatistics below
typedef struct TPathStatisticsPointFlags {
  /// Point is in a forbidden geoban area.
  BOOL8 inForbiddenGeoban;
  /// Point is on a toll road segment.
  BOOL8 onTollRoad;
  /// Point is on an unsealed road segment (road form).
  BOOL8 onUnsealedRoad;
  /// Point is on a ferry segment (road form).
  BOOL8 onFerry;
  /// Point is on a freeway segment (road form).
  BOOL8 onFreeway;
  /// Point is on a motorway segment (form of way).
  BOOL8 onMotorway;
  /// Point is on a pedestrian segment.
  BOOL8 onPedestrianRoad;
  /// Point is on a non-public segment.
  BOOL8 onNonPublicRoad;
  /// Point is on a non-drivable segment (but you can walk on it).
  BOOL8 onNonDrivableRoad;
  /// Point is on a residential segment.
  BOOL8 onResidentialRoad;
  /// Path requires a uturn to reach point.
  BOOL8 uturn;
} TPathStatisticsPointFlags;

/// @typedef struct TPathStatistics
/// A structure for recording various useful debug/test statistics
/// associated with a route calculation.  Many of these statistics are only
/// valid if the path is in state PATH_RESULT_FINISHED
typedef struct TPathStatistics {
  /// The cost of the chosen route. (For full routes, this is the sum of
  /// <i>actual</i> costs of the origin and destination portions of the
  /// route.  For re-routes, this is <i>actual</i> cost of <b>only</b> the
  /// new portion of the path.)
  UINT32 pathCost;

  /// The number of nodes expanded during the search.  This should be the
  /// total number of nodes in the origin/destination open/closed
  /// level0/level1 trees
  UINT32 nodesExplored;

  /// The number of nodes actually allocated during the search.  This is a
  /// count of memory use and may differ from nodesExplored if not all
  /// explored nodes are stored.
  UINT32 nodesAllocated;

  /// The number of calls by pathCalculator_chooseNodeToExpand() to
  /// searchNodeTree_findCheapestNode().  This is an expensive call, so
  /// algorithmic reductions here are nice.
  UINT32 findCheapestCount;

  /// Was this leg last calculated as part of a full route calculation or
  /// as part of a re-route calculation?
  BOOL8 isAReroute;

  /// Is the chosen route a complete calculation or does it (actually)
  /// reuse a portion of the previously calculated route (not all re-routes
  /// do).  This can only be TRUE if isAReRoute is also TRUE.
  BOOL8 reusesSomeOldRouteSegments;

  /// Contains the number of segments this path had before the re-route
  /// calculation. Will be 0, if there is no re-route calculation.
  UINT32 oldSegmentCount;

  /// Contains the number of unchanged segments at the end of the route,
  /// compared to the route before the re-route calculation. Will be 0, if it's
  /// no re-route calculation.
  /// It should not be mixed up with up reusesSomeOldRouteSegments, which is an
  /// indicator that routing explicitely reused part of the old route.
  UINT32 unchangedSegmentCount;

  /// Route origin/destination location flags.  These flags record various
  /// supplemental information about the origin/destination that is
  /// determined at route time.
  TPathStatisticsPointFlags flags[2];

} TPathStatistics;


#define RT_INVALID_PATH_ID -1

/// @typedef struct TPath
/// A struct representing a calculated path in all its glory.  This type
/// encompases the output of a single stop route calculation and includes
/// the details of the segments of that route.
///
/// Memory management of a path's segment lists is provided by the
/// encompassing journey.  For this reason, a path without a journey makes
/// no sense.
typedef struct TPath {
  TPathId                id;
  TPathCalculatorResult  result;
  TPathCalculatorResult  interResult;
  struct TSegmentList*   drivingSegs;
  struct TSegmentList*   sortedSegs;
  TSegmentVertices       startVertices;
  TSegmentVertices       endVertices;
  TRoutingOptions        options;
  TPathStatistics        statistics;
} TPath;


/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
/// Scan a path for segments with flagTollRoad = TRUE
///
/// @param pThis Path to check for toll segments
/// @return 
///   - TRUE true if the path has any toll segments
///   - FALSE if the path is empty or contains no toll segments
extern MAPCORE_API
BOOL8 path_hasTollSegments( 
  const TPath* pThis );


/// Scan a path for segments with roadForm = ROAD_FORM_FREEWAY
///
/// @param pThis Path to check for freeway segments
/// @return 
///   - TRUE true if the path has any freeway segments
///   - FALSE if the path is empty or contains no freeway segments
extern MAPCORE_API
BOOL8 path_hasFreewaySegments( 
  const TPath* pThis );

/// Scan a path for segments with roadFow = ROAD_FOW_OVERPASS
///
/// @param pThis Path to check for overpass segments
/// @return 
///   - TRUE true if the path has any overpass segments
///   - FALSE if the path is empty or contains no overpass segments
extern MAPCORE_API
BOOL8 path_hasOverpassSegments( 
  const TPath* pThis );


/// Scan a path for segments with roadFrom = ROAD_FORM_UNSEALED
///
/// @param pThis Path to check for unsealed segments
/// @return 
///   - TRUE true if the path has any unsealed segments
///   - FALSE if the path is empty or contains no unsealed segments
extern MAPCORE_API
BOOL8 path_hasUnsealedSegments( 
  const TPath* pThis );


/// Scan a path for segments with roadForm = ROAD_FORM_FERRY
///
/// @param pThis Path to check for ferry segments
/// @return 
///   - TRUE true if the path has any ferry segments
///   - FALSE if the path is empty or contains no ferry segments
extern MAPCORE_API
BOOL8 path_hasFerrySegments( 
  const TPath* pThis );


/// Scan a path for avoided segments or portions inside an avoid area
///
/// @param pThis Path to check for avoidances
/// @return 
///   - TRUE true if the path has any avoided segments or traverses any 
///       avoided areas
///   - FALSE if the path is empty or does not contain any avoidances
extern MAPCORE_API
BOOL8 path_hasAvoidAreasOrSegments( 
  const TPath* pThis );


/// Returns the route bounding box which completely encloses all segments
/// in a leg of the internal journey.
///
/// @param[in]  pThis Path for which to calculate bounding box
/// @param[out] bbox Bounding box of the path
/// @return FALSE if the pThis does not have state PATHCALC_RESULT_FINISHED
///         TRUE if the bounding box is determined successfully
extern MAPCORE_API
BOOL8 path_getApproxBoundingBox(
  const TPath* pThis,
  TRect* bbox );
//## This API scales the bounding box to include a margin around the route
//## so that it looks good on the screen.  I don't think core should be
//## doing this, as we don't know for what purpose the bounding box is
//## required.  This is why I have put "approx" in the API name.


/// Returns the current result of the given path
///
/// @param[in] pThis The Path to get the result of 
/// @return TPathCalculatorResult indicating the result of the given path
extern MAPCORE_API
TPathCalculatorResult path_getResult(
  const TPath* pThis);

/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif // RTPATH_H
