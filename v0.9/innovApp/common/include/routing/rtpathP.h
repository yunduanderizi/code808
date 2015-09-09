//
// Declaration of the TPath type and associated functions.
//
// $Id: rtpathP.h 9792 2008-04-21 14:47:09Z marcus.spranger $
//

#ifndef RTPATHP_H
#define RTPATHP_H

#include <routing/rtpath.h>

#ifdef __cplusplus
extern "C" {
#endif

// @brief Copies the contents of one path to another.
//
// Note that the segment lists pointed to by the target journey are
// \em not copied by this function; that must be done by the owner of
// those lists, e.g. the containing journey.
//
// @param[in]   pThis the source path
// @param[out] target the target path
void path_clone(const TPath *pThis, TPath *target);

// Search for a particular segment ID in a path
//
// It is assumed that indices in the segment id ordered list for equal
// segment ids are in route order.
//
// @param  pThis Path to check for segment ID
// @param  segment Segment ID to search for
// @param  startIndex Start searching this far into the segment list.
//           Allows multiple occurrences of a segment to be found.  Use 0 to
//           search from the start of the route.
// @param  occursAgain A hint that the segmentId occurs again in the route
//           list after the returned index.  Optional, NULL okay
// @return The index of the first occurrence of the segment id at or after
//           startIndex.  -1 if the segment was not found.
INT32 path_findSegmentId( 
  const TPath* pThis,
  TSegmentID segment,
  UINT32 startIndex,
  BOOL8* occursAgain );


// Return the node that the specified segment is entered from when the
// path is traversed in driving order.
//
// This function assumes its parameters are valid
//
// @param[in]  pThis Path being examined
// @param[in]  pathSegIndex Index of seg in path for which to find the entry
//              node.  Must be > 0 as as prior nonsensical otherwise
// @param[out] priorNode The node segment `pathSegIndex' has in common 
//               with its prior segment
// @return
//   - TRUE  The entry node was identified
//   - FALSE On error or if the segment does not have a contiguous prior 
//       segment in the segment list

BOOL8 path_getSegmentEntryNode(
  const TPath* pThis,
  INT32 pathSegIndex,
  TNodeID* priorNode );


// Return the path driving direction for the specified segment. Driving
// direction is either toward the segment first node or last node.
//
// @param[in]  pThis Path being examined
// @param[in]  pathSegIndex Index of a segment in the path for which to find
//             the driving direction.
// @param[out] towardFirstNode TRUE if the driving direction along the path
//             segment is towards the firstNode. FALSE if it is towards the
//             lastNode.
// @return
//   - TRUE If towardFirstNode was populated correctly
//   - FALSE If the path is not valid or if the segment index is incorrect.
BOOL8 path_getSegmentDrivingDirection(const TPath *pPath,
                                      INT32 pathSegIndex,
                                      BOOL8 *towardFirstNode);


// Retreive the vertex points (in driving order) of a segment in a path.
// For the first and last segments, these will be vertices representing
// part of a segment.  For other segments, they will be the full set of
// vertex shapes, reversed if necessary so they are supplied in driving
// order.
//
// @param[in]     pThis Path that `legIndex' is in
// @param[in]     pathSegIndex Path index to get vertices for
// @param[out]    vertices Array of vertices to populate.
// @return
//   - TRUE vertices populated okay
//   - FALSE pThis state not PATHCALC_RESULT_FINISHED or pathSegmentIndex bad
BOOL8 path_getDrivingVertices( 
  const TPath* pThis,
  INT32 pathSegIndex,
  TSegmentVertices* vertices );


// Retrieve the road segments (in driving order) of a path.
//
// @param[in]     pThis Path containing the segments
// @param[out]    count Number of segments in the path
// @return        A pointer to the list of segment IDs
TSegmentID* path_getDrivingSegments( 
  const TPath* pThis,
  INT32* count);


// Retrieve the heading of a path at a specific segment and sub-segment.
// If the sub-segment has insufficient length we try to find the next segment with a
// sufficient length along the path in driving direction.
//
// @param[in]   pThis
//   The path of which we want to determine the heading.
// @param[in]   pathSegIndex
//   The index of the segment of which we want to determine the heading.
// @param[in]   subSegment
//   The sub-segment (in segment order) of which we want to determine the heading.
// @return
//   -1 if we can't determine a heading or the byte heading in the interval
//   0 .. 255
INT32 path_getHeading(
  const TPath* pThis,
  INT32 pathSegIndex,
  UINT32 subSegment );


// Return whether a path is the result of a re-route.
//
// @param[in]     pThis Path to query for re-route
// @return
//   - TRUE true if the path is a re-route

BOOL8 path_isAReroute(
  const TPath* pThis);

/*----------------------------------------------------------------*/
/// Return whether a path starts with a uturn
///
/// @param[in]     pThis Path to query
/// @return
///   - TRUE true if the path starts with a U-turn
BOOL8 path_startsWithAUTurn(const TPath* pThis);

// Set the id of a path to the id of the last path plus one.
//
// This should keep path ids unique unless the range of a UINT32 is
// exhausted.
//
// @param[in]     pThis Path whose id to set
void path_setIdUniquely(
  TPath* pThis );

// get the id of a path
//
// @param[in]     pThis Path whose id to get
TPathId path_getId(
  const TPath* pThis );

#ifdef __cplusplus
}
#endif

#endif // RTPATHP_H
