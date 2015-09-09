#ifndef GDINTERSECTIONP_H
#define GDINTERSECTIONP_H

#include <guidance/gdinstruction.h>
#include <guidance/gdpluraljP.h>
#include <mapdata/enlocaleP.h>
#include <mapdata/enroadP.h>
#include <math/mtangleP.h>

#ifdef __cplusplus
extern "C" {
#endif


/// @internal
/// @addtogroup Guidance
/// @{

/// @internal
/// @addtogroup Intersection
/// @ingroup Guidance
/// @{

#define GUIDE_STRAIGHT_ROAD_THRESHOLD 24
#define GUIDE_STRAIGHT_STRONGLY_PREFERRED_THRESHOLD 32

#define GUIDE_THROUGH_ROAD_THRESHOLD 64

#define GUIDE_RIGHT_ANGLE_ROADS_THRESHOLD 15

typedef TNodeID TGuideIntersectionID;

/// @internal
/// Expanded information about an external segment.
typedef struct TGuideSegInfo {
  /// @internal
  /// unpacked segment
  TRoadSegment segment;
  /// @internal
  TNodeID nodeID;
  /// @internal
  /// heading out of the node
  THeading heading;
  /// @internal
  /// Name 1 (road name)
  UTF16 name1[ROAD_MAX_NAME_SIZE];
  /// @internal
  /// Name 2 (route name)
  UTF16 name2[ROAD_MAX_NAME_SIZE];
  /// @internal
  /// String ID 1 (road name)
  TRoadStringID roadID1;
  /// @internal
  /// String ID 2 (road name)
  TRoadStringID roadID2;

  TTokenNameDetails name1Details;
  TTokenNameDetails name2Details;

  /// @internal
  /// Can we route out on this segment?
  BITFIELD32 flagValid:1;
  /// @internal
  /// TRUE if name1 and name2 have been populated
  BITFIELD32 flagNamesUnpacked:1;
  /// @internal
  /// TRUE if name1 and name2 have been populated
  BITFIELD32 flagStringIDsUnpacked:1;
  /// @internal
  /// TRUE if the segment is part of a bifurcation restriction
  BITFIELD32 flagBifurcation:1;
  /// @internal
  /// TRUE if the segment is part of a preferred turn restriction.
  BITFIELD32 flagPreferred:1;
  /// @internal
  /// TRUE if the segment is unpreferred (as implied by the presence of a
  /// preferred turn restriction).
  BITFIELD32 flagUnPreferred:1;
  /// @internal
  /// TRUE if the segment represents a u-turn relative to the source segment.
  BITFIELD32 flagUTurn:1;
  /// TRUE if the path includes any minor slip roads
  BITFIELD32 flagPathIncludesMinorSlipRoads:1;
  /// TRUE if the path includes any major slip roads
  BITFIELD32 flagPathIncludesMajorSlipRoads:1;
} TGuideSegInfo;


#define GDINTERS_POINT_ROUNDABOUT      0x00000001
#define GDINTERS_SOURCE_FREEWAY        0x00000002
#define GDINTERS_SOURCE_ROUNDABOUT     0x00000004
#define GDINTERS_DEST_FREEWAY          0x00000008
#define GDINTERS_DEST_SLIPROAD         0x00000010
#define GDINTERS_OFF_SLIPROAD          0x00000020
#define GDINTERS_ONTO_SLIPROAD         0x00000040
#define GDINTERS_DEST_ROUNDABOUT       0x00000080
#define GDINTERS_ROAD_BIFURCATES       0x00000100
#define GDINTERS_OTHER_ROUNDABOUT      0x00000200
#define GDINTERS_CROSS_MAJOR           0x00000400
#define GDINTERS_SOURCE_ROAD_ENDS      0x00000800
#define GDINTERS_TRAFFIC_LIGHTS        0x00001000
#define GDINTERS_CROSSING_TRAFFIC      0x00002000
#define GDINTERS_ONTO_DUAL_CARRIAGEWAY 0x00004000
#define GDINTERS_INTERNAL_BIFURCATION  0x00008000

typedef INT32 TGdIntersFlags;

/// @internal
/// Guidance intersection data abstraction layer.
typedef struct TGuideIntersection {
  // The following fields could be used
  // for lazy evaluation, but can also
  // be used to make sure our dependencies
  // are properly followed.
  struct {
    BITFIELD32 segmentsUnpacked:1;
    BITFIELD32 intersectionsUnpacked:1;
    BITFIELD32 restrictionsUnpacked:1;
    BITFIELD32 headingsCalculated:1;
    BITFIELD32 segValidityChecked:1;
    BITFIELD32 segsSorted:1;
    BITFIELD32 propertyCountsInitialised:1;
    BITFIELD32 propertyFlagsInitialised:1;
    BITFIELD32 minMaxRoadClassSet:1;
    BITFIELD32 uTurnSegsChecked:1;
    BITFIELD32 restrictionBifurcationsChecked:1;
    BITFIELD32 heuristicBifurcationsChecked:1;
    BITFIELD32 exitSegmentSet:1;
    BITFIELD32 lanesUnpacked:1;
    BITFIELD32 junctionViewsUnpacked:1;
  } flags;

  TRoadNode                 node;
  TCountryCode              countryCode;
  TLocale                   locale;

  UINT32                    numValidSegments;
  INT32                     numMotorways; // motorway segments that aren't offramps
  INT32                     numOfframps;

  // the external segments
  TGuideSegInfo             segInfo[GUIDE_JUNCTION_MAX_SEGMENTS];

  THeading                  originalDestHeading; // cached

  /// The lane/junctionView information if any is available for the
  /// upcoming intersection
  TLaneInfo                 laneInfo;
  TJunctionViewInfo         junctionViewInfo;

  // The type of the bifurcation at this intersection.
  // RESTRICTION_UNDEFINED_BIFURCATION by default.
  TRoadTurnRestrictionType  bifurcationType;

  UINT32                    minRoadClass;
  UINT32                    maxRoadClass;
  TRoadHandedness           roadHandedness; // private

  // info specific to the source/dest. Can be calculated rather than cached?
  UINT32                    sourceIndex,
                            destIndex;              // indices into the node, not the route
  const TPath               *pPath;                 
  INT32                     sourcePathIndex;        // source segment.
  INT32                     exitPathIndex;          // exit segment. For extended intersections might not
                                                    // be attached to the node.

  // The number of path segments before the intersection node (including the
  // source segment).
  INT16                     numPathSegsBeforeNode;

  UINT32                    nodeLeftTurnCount,
                            nodeRightTurnCount;

  // Flags
  TGdIntersFlags            properties;
  TSegmentID                otherBranch; // extra bifurcation info

  TGuideJunction            junction;

  TRoadIntersection         intersection[ROAD_NODE_MAX_INTERSECTION];
  UINT32                    intersectionID[ROAD_NODE_MAX_INTERSECTION];
  UINT32                    numIntersections;
} TGuideIntersection;

INT32 gdinters_init( TGuideIntersection *pThis,
                     const TPath *pPath,
                     INT32 pathSegIndex,
                     BOOL8 inDrivingOrder,
                     BOOL8 expand,
                     BOOL8 unpackLanes,
                     BOOL8 unpackJunctionViews );

BOOL8 gdinters_isThroughRoad(TGuideIntersection *pThis,
                             const UINT32 segmentIndex,
                             UINT32 *pMatchingIndex);

UINT32 gdinters_numValidSegments(const TGuideIntersection *pThis);
UINT32 gdinters_numSegments(const TGuideIntersection *pThis);
const TRoadSegment *gdinters_source(const TGuideIntersection *pThis);
const TRoadSegment *gdinters_dest(const TGuideIntersection *pThis);
const TRoadSegment *gdinters_getSeg(const TGuideIntersection *pThis,
                                    const UINT32 segmentIndex );
TSegmentID gdinters_sourceID(const TGuideIntersection *pThis);
TSegmentID gdinters_destID(const TGuideIntersection *pThis);
TSegmentID gdinters_segID(const TGuideIntersection *pThis,
                          const UINT32 segmentIndex );
UINT32 gdinters_sourceIndex(const TGuideIntersection *pThis);
UINT32 gdinters_destIndex(const TGuideIntersection *pThis);

THeading gdinters_sourceHeading(const TGuideIntersection *pThis);
THeading gdinters_destHeading(const TGuideIntersection *pThis);
THeading gdinters_segHeading(const TGuideIntersection *pThis,
                             const UINT32 segmentIndex );
THeading gdinters_originalSegHeading(const TGuideIntersection *pThis,
                                     const UINT32 segmentIndex );

BOOL8 gdinters_hasMinorSlipRoads(const TGuideIntersection *pThis,
                                 const UINT32 segmentIndex );
BOOL8 gdinters_hasMajorSlipRoads(const TGuideIntersection *pThis,
                                 const UINT32 segmentIndex );

void gdinters_roadName(TGuideIntersection *pThis,
                       const UTF16 **name1,
                       const UTF16 **name2,
                       const TTokenNameDetails **name1Details,
                       const TTokenNameDetails **name2Details,
                       const UINT32 segmentIndex);

void gdinters_sourceRoadName(TGuideIntersection *pThis,
                             const UTF16 **name1,
                             const UTF16 **name2,
                             const TTokenNameDetails **name1Details,
                             const TTokenNameDetails **name2Details);

void gdinters_destRoadName(TGuideIntersection *pThis,
                           const UTF16 **name1,
                           const UTF16 **name2,
                           const TTokenNameDetails **name1Details,
                           const TTokenNameDetails **name2Details);

void gdinters_roadStringIDs(TGuideIntersection *pThis,
                            TRoadStringID *id1,
                            TRoadStringID *id2,
                            const UINT32 segmentIndex);

void gdinters_sourceRoadStringIDs(TGuideIntersection *pThis,
                                  TRoadStringID *id1,
                                  TRoadStringID *id2);

void gdinters_destRoadStringIDs(TGuideIntersection *pThis,
                                TRoadStringID *id1,
                                TRoadStringID *id2);

typedef enum TRoadNamesMatchResult {
  RNM_NO_MATCH          = 0,
  RNM_ROAD_NAMES_MATCH  = 1,
  RNM_ROUTE_NAMES_MATCH = 2,
  RNM_STRONG_MATCH      = 3
} TRoadNamesMatchResult;

TRoadNamesMatchResult gdinters_roadNamesMatch(TGuideIntersection *pThis,
                              const UINT32 seg1Index,
                              const UINT32 seg2Index);

BOOL8 gdinters_destNameMatchesSource(TGuideIntersection *pThis);

BOOL8 gdinters_roadNamesDifferent(TGuideIntersection *pThis,
                                  const UINT32 seg1Index,
                                  const UINT32 seg2Index);

BOOL8 gdinters_destNameDifferentFromSource(TGuideIntersection *pThis);

void gdinters_getPosition(const TGuideIntersection *pThis, TPos *pos);

BOOL8 gdinters_getFlag(const TGuideIntersection *pThis, INT32 flag);
TSegmentID gdinters_getOtherBranch(const TGuideIntersection *pThis);

TRoadHandedness gdinters_getRoadHandedness(const TGuideIntersection *pThis);

INT32 gdinters_nodeLeftTurnCount(const TGuideIntersection *pThis);
INT32 gdinters_nodeRightTurnCount(const TGuideIntersection *pThis);
UINT32 gdinters_getMinRoadClass(const TGuideIntersection *pThis);
UINT32 gdinters_getMaxRoadClass(const TGuideIntersection *pThis);
BOOL8 gdinters_isValid(const TGuideIntersection *pThis,
                       const UINT32 segmentIndex );

BOOL8 gdinters_isUTurn(const TGuideIntersection *pThis,
                       const UINT32 segmentIndex);

BOOL8 gdinters_isPreferredTurn(const TGuideIntersection *pThis,
                               const UINT32 segmentIndex);

BOOL8 gdinters_isUnpreferredTurn(const TGuideIntersection *pThis,
                                 const UINT32 segmentIndex);

BOOL8 gdinters_isBifurcationTurn(const TGuideIntersection *pThis,
                                 const UINT32 segmentIndex);

BOOL8 gdinters_roadFormMatches(const TRoadForm form1, const TRoadForm form2);


BOOL8 gdinters_isGateway(const TGuideIntersection *pThis);


TGuideTurnDirection gdinters_turnDirection(const TGuideIntersection *pThis,
                                           const UINT32 segmentIndex);

TRoadLaneDirType gdinters_convertLaneDir(UINT16 eDataDir);


UINT32 gdinters_intID(
  const TGuideIntersection *pThis,
  UINT32 index);

TRoadIntersectionType gdinters_intType(
  const TGuideIntersection *pThis,
  UINT32 index);

TRoadTurnRestrictionType gdinters_bifurcationType(const TGuideIntersection *pThis);

BOOL8 gdinters_slipRoadBifurcation(const TGuideIntersection *pThis);

void gdinters_getLocale(const TGuideIntersection *pThis,
                        TLocale *pLocale);
/// @internal
/// @}

/// @internal
/// @}



#ifdef __cplusplus
}
#endif

#endif /* GDINTERSECTIONP_H */
