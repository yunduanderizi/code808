#ifndef GDPLURALJP_H
#define GDPLURALJP_H

#include <guidance/gdjunctreeP.h>
#include <mapdata/enroadP.h>
#include <math/mtangleP.h>
#include <routing/rtjourney.h>
#include <routing/rtpathcalcP.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @internal
/// The maximum number of road segments that may be involved in a plural
/// junction.
#define GUIDE_JUNCTION_MAX_SEGMENTS 32

/// @internal
/// The maximum number of path segments that may be involved in a plural
/// junction.
#define GUIDE_JUNCTION_MAX_PATH_SEGMENTS 32

typedef struct {
  /// @internal
  /// The IDs of the segments heading out of the intersection.
  TSegmentID ID;
  /// @internal
  /// The headings of the segments heading out of the intersection.
  THeading heading;
  /// @internal
  /// The form of the segment. Sealed, unsealed, freeway or ferry.
  TRoadForm roadForm;
  /// @internal
  /// The form of way of the segment. Especially useful if the segment is a
  /// dual-carriageway.
  TRoadFow roadFow;
  /// @internal
  /// The node by which we enter this segment (when we leave the junction).
  TSegmentID entryNodeID;
  /// @internal
  /// The node by which we exit this segment (when we leave the junction).
  TSegmentID exitNodeID;
  /// @internal
  /// Length of the (external) road segment in metres.
  UINT16 roadLength;

  BITFIELD32 flagFwdPathCalculated:1;

  /// TRoadMapLayer (ROAD_MAP_LAYER_0 or ROAD_MAP_LAYER_1)
  BITFIELD32 roadMapLayer:2;

  /// TRUE if the segment is a plural junction.
  BITFIELD32 flagPluralJ:1;

  /// TRUE if the segment is a roundabout segment
  BITFIELD32 flagRoundabout:1;

  /// TRUE if the forward path includes any minor slip roads
  BITFIELD32 flagFwdPathHasMinorSlipRoads:1;

  /// TRUE if the forward path includes any major slip roads
  BITFIELD32 flagFwdPathHasMajorSlipRoads:1;

  // TRUE if the segment is so short that heading computation from the
  // lat/long of the end-nodes isn't accurate.
  BITFIELD32 flagHeadingNeedsInheriting:1;

  /// Path from the source segment to the dest segment.
  TJunctionPathSegment fwdPathSegList[GUIDE_JUNCTION_MAX_PATH_SEGMENTS];
  INT32 numFwdPathSegs;

} TJunctionSegment;


/// Item in a TGuideJunction segmentTree
typedef struct {
  /* This field is required by TGuideJunctionTree. */
  TGuideJunctionTreeNode treeNode;

  /* Include other useful information below: */

  // has to be named equal to TRoadSegment::roadForm, otherwise code doesn't work
  TRoadForm roadForm;
  // has to be named equal to TRoadSegment::roadFow, otherwise code doesn't work
  TRoadFow roadFow;

  // The entry and exit node of the segment.
  // ??????????????????????????????????????????????????????????????????????????
  // Well, what that means, depends on the kind of segment:
  // If it's an external segment (connected to a junction, but not embedded in
  // it), the entry node is the node that lies on the junction and the exit 
  // node is the node, that isn't part of the junction anymore.
  // If it's an internal segment, it doesn't mean anything useful.
  // ??????????????????????????????????????????????????????????????????????????
  TNodeID entryNodeID;
  TNodeID exitNodeID;

  BITFIELD32 roadLength:16;

  /// TRoadMapLayer (ROAD_MAP_LAYER_0 or ROAD_MAP_LAYER_1)
  BITFIELD32 roadMapLayer:2;

  /// TRUE if segment is a plural junction segment
  BITFIELD32 flagPluralJ:1;
  /// TRUE if this is a boundary segment.
  BITFIELD32 flagExternal:1;
  /// TRUE if segment is on the path
  BITFIELD32 flagOnPath:1;
  /// TRUE if this segment is drivable. A non-drivable segment has
  /// flagForwardsRestriction and flagBackwardsRestriction set to TRUE
  BITFIELD32 flagIsDrivable:1;
  /// TRUE if this segment is part of a roundabout or a special traffic figure
  BITFIELD32 flagRoundabout:1;
  /// TRUE if this segment is a slip road
  BITFIELD32 flagSlipRoad:1;

} TSegmentTreeObject;


/// Item in a TGuideJunction nodeTree
typedef struct {
  /* This field is required by TGuideJunctionTree. */
  TGuideJunctionTreeNode treeNode;

  /* Include other useful information below: */

  /// The number of segments attached to this node.
  BITFIELD32 segCount:4;
  /// The number of PJ segments attached to this node.
  BITFIELD32 pjSegCount:4;
  /// TRUE if this is a boundary node.
  BITFIELD32 flagInternal:1;
  /// TRUE if this node is part of the roundabout "ring", i.e. if it's
  /// connected to a roundabout segment
  BITFIELD32 flagRoundabout:1;
} TNodeTreeObject;



typedef struct {
  /// @internal
  /// TSegmentTreeObject tree containing all of the segments in the junction.
  TGuideJunctionTree segmentTree;

  /// @internal
  /// TNodeTreeObject tree containing all of the nodes in the junction.
  TGuideJunctionTree nodeTree;

  /// @internal
  /// Source segment
  TJunctionSegment *pSourceSeg;

  /// @internal
  /// Destination segment
  TJunctionSegment *pDestSeg;

  /// @internal
  /// The segments heading out of the plural junction.
  TJunctionSegment externalSegs[GUIDE_JUNCTION_MAX_SEGMENTS];

  /// @internal
  // The total number of segments heading out of the plural junction.
  INT32 numExternalSegs;

  /// @internal
  /// The number of journey path segments in the plural junction.
  INT32 numPathSegs;

  /// @internal
  /// The total number of segments involved in the plural junction.
  INT32 totalNumSegs;

  /// @internal
  /// The journey path source driving segment index.
  INT32 sourcePathIndex;

  /// @internal
  /// The journey path destination driving segment index.
  INT32 destPathIndex;

  /// @internal
  /// The intersection ID. Zero if there is no intersection.
  UINT32 intersectionID;

  /// @internal
  /// fwdPathSegList index of the first roundabout (or special traffic figure)
  /// segment on the destination path. -1 if there are no roundabout segments.
  INT32 firstRBDestPathSegIndex;

  /// @internal
  /// TRUE if there is a road map layer change at this intersection.
  BOOL8 roadMapLayerChanges;

  /// @internal
  /// TRUE if there is a very short segment whose heading needs to be inherited.
  BOOL8 containsShortSegments;

} TGuideJunction;


extern void gdJunction_create(TGuideJunction *pThis);

extern void gdJunction_destroy(TGuideJunction *pThis);

extern INT32 gdJunction_unpack(TGuideJunction *pThis,
                               const TPath *pPath,
                               INT32 pathSegIndex,
                               BOOL8 inDrivingOrder,
                               BOOL8 expand);

extern TJunctionSegment *gdJunction_sourceSeg(TGuideJunction *pThis);

extern TJunctionSegment *gdJunction_destSeg(TGuideJunction *pThis);

extern BOOL8 gdJunction_isExtended(const TGuideJunction *pThis);

extern void gdJunction_dump(const TGuideJunction *pThis);

#ifdef __cplusplus
}
#endif

#endif /* GDPLURALJP_H */
