#ifndef RTROADSEGMENTP_H
#define RTROADSEGMENTP_H

#include <routing/rtroadsegment.h>
#include <mapdata/enroadP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// Unpack all the vertices involved in a segment.  This includes both the
// shapepoints and the node positions.
//
// NOTE: THE SECTOR CONTAINING pThis MUST BE LOCKED IN ITS ORIGINAL
// POSITION PRIOR TO CALLING THIS FUNCTION
//
// @param[in]  pThis The segment from which to get the vertices
// @param[in]  backwards If TRUE, vertices will be written in lastNode to
//               firstNode order rather than firstNode to LastNode order
// @param[out] out A struct containing the vertices and their count
// @return     TRUE usually - FALSE on unexpected data or program error
BOOL8 roadSegment_getVertices( 
  TRoadSegment* pThis,
  BOOL8               backwards,
  TSegmentVertices*   out );


// Unpack all the vertices involved in a segment.  This includes both the
// shapepoints and the node positions.
//
// @param[in]  segId The ID of the segment from which to get the vertices
// @param[out] out A struct containing the vertices and their count
// @return     TRUE usually - FALSE on unexpected data or program error
BOOL8 roadSegmentId_getVertices( 
  TSegmentID          segId,
  TSegmentVertices*   out );


// Given two positions on a segment, generate an array of shape points
// between them.
//
// NOTE: THE SECTOR CONTAINING pThis MUST BE LOCKED IN ITS ORIGINAL
// POSITION PRIOR TO CALLING THIS FUNCTION
//
// @param[in]  pThis The segment from which to get the vertices
// @param[in]  startPost Start postion for partial shapes
// @param[in]  endPos End position for partial shapes
// @param[out] out A struct containing the vertices and their count
// @return     TRUE usually - FALSE on unexpected data or program error
BOOL8 roadSegment_getPartialVertices(
  TRoadSegment*       pThis,
  const TPos*         startPos,
  const TPos*         endPos,
  TSegmentVertices*   out );


// Given two positions and a segment id, generate an array of shape points
// between the positions on the unpaced segment.
//
// @param[in]  segId The ID of the segment from which to get the vertices
// @param[in]  startPost Start postion for partial shapes
// @param[in]  endPos End position for partial shapes
// @param[out] out A struct containing the vertices and their count
// @return     TRUE usually - FALSE on unexpected data or program error
BOOL8 roadSegmentId_getPartialVertices(
  TSegmentID          segId,
  const TPos*         startPos,
  const TPos*         endPos,
  TSegmentVertices*   out );


// Determine the heading of a road segment at one of its nodes.
//
// The returned heading is from the specified node, along the target
// segment.  e.g. 0----->--<-----0
//
// NOTE: THE SECTOR CONTAINING pThis MUST BE LOCKED IN ITS ORIGINAL
// POSITION PRIOR TO CALLING THIS FUNCTION.
//
// NOTE: The returned heading is toward the next vertex (shapepoint or
// node) along the segment.  If all shapepoints and both vertices have the
// same position then the returned heading will be 0,0.
//
// @param[in]  pThis   The road segment whose heading is to be found
// @param[in]  node    The id of the node (on the segment) to use as the
//                     source of the heading 
// @param[out] heading The returned heading
// @return FALSE if `pThis' does not have `node' as a node, TRUE otherwise
BOOL8 roadSegment_getHeading( 
  TRoadSegment* pThis,
  TNodeID node,
  TPos* heading );


/// Returns whether a segment is one-way.
///
/// A segment is one-way if it has either a forwards restriction or a
/// backwards restriction (but not both.)
BOOL8 roadSegment_isOneWay(const TRoadSegment *segment);

/// Gets the node at one end of a one-way segment based on allowed
/// driving order.
///
/// @param segment
/// @param start   \c TRUE to return the node that must be first in a
///                legal driving order, \c FALSE to return the other
///                node
///
/// @return one of the segment's nodes (possible 0 if the segment
///         lacks an end node)
TNodeID roadSegment_getNodeAtEndOfOneWaySegment(const TRoadSegment *segment,
                                                BOOL8 start);

/// The prototype for predicates that can be passed to \c
/// roadSegment_followOneWayRoad.
///
/// @param context the pointer that was passed to \c
///                roadSegment_followOneWayRoad
/// @param segment the segment under consideration
/// @param nodeId  the ID of the node of \p segment that is not
///                connected to the previous segment considered
///
/// @return \c TRUE to stop following the road
typedef BOOL8 TFollowRoadPredicate(void *context, const TRoadSegment *segment,
                                   TNodeID nodeId);

/// The reasons that \c roadSegment_followOneWayRoad can stop.
typedef enum TFollowOneWayRoadResult {
  predicateSucceeded,           ///< the predicate returned \c TRUE
  ranOutOfSegments, ///< the maximum permitted number of segments were followed
  reachedJunction, ///< a node with more than two segments was reached
  reachedTwoWaySegment,         ///< a non-one-way segment was reached
  reachedNoNode            ///< a segment with no end node was reached
} TFollowOneWayRoadResult;

/// Follows a sequence of one-way road segments, calling a supplied
/// predicate for each one, stopping when the predicate returns \c
/// TRUE or the sequence cannot be trivially followed.
///
/// The sequence of road segments can be followed as long as
/// - the segments are one-way; and
/// - the segments' nodes are only attached to two segments; and
/// - not more than \p maxSegments have already been followed.
///
/// As each segment in the sequence is reached, the predicate is
/// called with that segment and the ID of the node that connects to
/// the next segment in the sequence (although that next segment might
/// not be reachable due to the above rules.)
///
/// @param[in]  startSegment     the segment to start at
/// @param[in]  reverseDirection \c TRUE to follow the road in the
///                              opposite direction to that allowed by
///                              the one-way restrictions
/// @param[in]  maxSegments      the maximum number of segments,
///                              including the start segment, to
///                              follow
/// @param[in]  stopPredicate    a function that is called for each
///                              followed segment; following stops if
///                              this function returns \c TRUE
/// @param[in]  context          a pointer that is passed as the first
///                              argument to \p stopPredicate
/// @param[out] numSegments      if not \c NULL, this is filled in
///                              with the number of segments followed
///
/// @return the reason the function stopped following segments
TFollowOneWayRoadResult roadSegment_followOneWayRoad(
  const TRoadSegment   *startSegment,
  BOOL8                 reverseDirection,
  UINT32                maxSegments,
  TFollowRoadPredicate  stopPredicate,
  void                 *context,
  UINT32               *numSegments);


/// Given a road segment id and the id of one of the nodes at the end of
/// that segment, return the id of the node at the other end of the
/// segment.
///
/// This is a dumb function - it needs to be used safely.
/// 1. It assumes that the data passed in is valid
/// 2. It always returns the id of the node in the segments layer and map.
///    (which may not be what is wanted if the node is glue or a wormhole)
///
/// NOTE: This is safe even if the segment only has one node, in that case,
/// 0 may be passed or returned as the other node id;
///
/// @param[in]  segId The ID of the segment whose other node should be retrieved
/// @param[in]  nodeId The ID of the known node
/// @return     The other node ID
TNodeID roadSegmentId_getOtherNodeId(
  TSegmentID          segId,
  TNodeID             nodeId );


#ifdef __cplusplus
}
#endif

#endif // RTROADSEGMENT_H
