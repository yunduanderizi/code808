#ifndef LPJOURNEYLOC_H
#define LPJOURNEYLOC_H

#include <routing/rtjourney.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Journey location represents a linear location on a journey. It may be
/// converted to/from latitude/longitude coordinates.
/// If we consider a journey to be be a straight line, a #TJourneyLocation
/// represents any given point on the line.
/// The journey location object can be a useful way of representing a location
/// on a journey for sub-systems like guidance, maplock, and the journey
/// simulator.
typedef struct TJourneyLocation {
  /// @internal
  /// Journey leg number.
  TJourneyLegNumber legNumber;
  /// @internal
  /// Journey leg path segment index. This is an index into the array of
  /// driving segments for the given journey leg.
  INT32 pathSegIndex;
  /// @internal
  /// The global ID of the path segment. This is the path driving segment at
  /// index #pathSegIndex.
  TSegmentID segmentID;
  /// @internal
  /// Distance in metres << 8 to the path destination/exit node. This is
  /// relative to the segment roadLength, not to the calculated length between 
  /// each pair of vertices.
  /// A calculated distance may be converted to a correct distToDestNode value
  /// by scaling it by the difference between the calculated segment length and
  /// the true TRoadSegment roadLength.
  UINT32 distToDestNode;
  /// @internal
  /// TRUE if the direction of travel is toward the first node. FALSE
  /// otherwise. We must store this so that we are able to determine the
  /// coordinates of the location without using the driving segments from the
  /// source journey.
  BOOL8 towardFirstNode;

  // The remaining fields are cached values to help make certain calculations
  // more efficient.

  /// @internal
  /// The length of the road segment in metres. This is the same as
  /// TRoadSegment roadLength.
  UINT32 realRoadLength;
  /// @internal
  /// The length of the road in metres calculated by adding up the distance
  /// between each of the vertices.
  UINT32 calcRoadLength;
  /// @internal
  /// Length in metres << 8 from the end of the driving path to the
  /// destination node. This value is only relevant on the last segment of the
  /// path, where the end position is not at a node. It is in the range
  /// 0 <= endLength <= realRoadLength.
  UINT32 endLength;
  /// @internal
  /// Total driving length of the path along the segment in metres << 8. This
  /// is only relevant on the first and last segments of the path, where the
  /// start and/or end positions are not at nodes. It is in the range
  /// 0 <= drivingLength <= realRoadLength.
  UINT32 drivingLength;
} TJourneyLocation;

typedef struct TTunnelSimLocation {
  /// @internal
  /// The global ID of the path segment. This is the path driving segment at
  /// index #pathSegIndex.
  TSegmentID segmentID;
  /// @internal
  /// The length of the road segment in metres. This is the same as
  /// TRoadSegment roadLength.
  UINT32 realRoadLength;
  /// @internal
  /// Distance in metres << 8 to the path destination/exit node. This is
  /// relative to the segment roadLength, not to the calculated length between 
  /// each pair of vertices.
  /// A calculated distance may be converted to a correct distToDestNode value
  /// by scaling it by the difference between the calculated segment length and
  /// the true TRoadSegment roadLength.
  UINT32 distToDestNode;
  /// @internal
  /// TRUE if the direction of travel is toward the first node. FALSE
  /// otherwise. We must store this so that we are able to determine the
  /// coordinates of the location without using the driving segments from the
  /// source journey.
  BOOL8 towardFirstNode;
  /// @internal
  /// The global ID of the destination node.
  TNodeID   destNodeID;
} TTunnelSimLocation;

#ifdef __cplusplus
}
#endif

#endif /* LPJOURNEYLOC_H */
