#ifndef GDROUNDABOUTP_H
#define GDROUNDABOUTP_H

#include <mapdata/enroadP.h>
#include <routing/rtjourney.h>
#include <guidance/gdinstruction.h>

#ifdef __cplusplus
extern "C" {
#endif


/// @internal
/// The maximum number of road segments that may be involved in a roundabout.
#define GUIDE_ROUNDABOUT_MAX_SEGMENTS 64

/// @internal
/// The maximum number of exit segments attached to one roundabout.
#define GUIDE_ROUNDABOUT_MAX_EXIT_SEGMENTS 32

/// @internal
/// constants to define the various types of turns possible when leaving a roundabout
/// NOTE: these use bit values so they can be tested en-masse eg:
/// if ( (exit & (ROUNDABOUT_STRAIGHT_BIT | ROUNDABOUT_FIRSTQUADRANT_BIT) ) ...
#define ROUNDABOUT_STRAIGHT_BIT         0x0001
#define ROUNDABOUT_FIRSTQUADRANT_BIT    0x0002
#define ROUNDABOUT_SECONDQUADRANT_BIT   0x0004
#define ROUNDABOUT_UTURN_BIT            0x0008
#define ROUNDABOUT_ENTRANCE_BIT         0x0010
#define ROUNDABOUT_LEFT_BIT             0x0020    // NOTE: only use this after resolving the road 'handedness' issues!
#define ROUNDABOUT_RIGHT_BIT            0x0040    // NOTE: only use this after resolving the road 'handedness' issues!
#define ROUNDABOUT_FIRSTSTRAIGHT_BIT    0x0080    // on the border between straight and first quadrant
#define ROUNDABOUT_SECONDSTRAIGHT_BIT   0x0100    // on the border between straight and the second quadrant

/// debugging macro
#define SIDE_STRING(side) ((side&ROUNDABOUT_STRAIGHT_BIT)?"straight ": \
                            (side&ROUNDABOUT_FIRSTQUADRANT_BIT)?"first-quad ": \
                            (side&ROUNDABOUT_SECONDQUADRANT_BIT)?"second_quad ": \
                            (side&ROUNDABOUT_UTURN_BIT)?"u-turn ": \
                            (side&ROUNDABOUT_FIRSTSTRAIGHT_BIT)?"first/straight(ambiguous) ": \
                            (side&ROUNDABOUT_SECONDSTRAIGHT_BIT)?"second/straight(ambiguous) ": "entry ")


// NOTE: "extended guidance" means saying 'at the roundabout turn right' instead of
// 'at the roundabout take the 3rd exit'

/// what is the largest is roundabout that extended guidance will be given for
/// the measurement is in metres for the circumference of the road
#define MAXIMUM_EXT_GUIDE_ROUNDABOUT_SIZE 250   // 125 = 40 metre diameter (pi * diameter = circumference)
/// largest number of exits from a roundabout allowed if we're going to use extended guidance
#define MAXIMUM_EXT_GUIDE_ROUNDABOUT_EXITS 5
/// if a roundabout has a circumference of less than this, use the percentage method of telling
/// when two exits are too close together (instead of the absolute distance between them)
#define EXT_GUIDE_SMALL_ROUNDABOUT 120
/// if there's another exit too close to the desired exit, don't use extended guidance (metres)
#define EXT_GUIDE_EXITS_TOO_CLOSE 20
/// if there's another exit too close to the desired exit, don't use extended guidance (percentage of total circumference)
#define EXT_GUIDE_EXITS_TOO_CLOSE_PERCENT 15
/// larger values from gdinters_roundaboutRoundness() mean the roundabout is less 'round' (0 = perfectly round)
/// this value dictates the maximum acceptable value
#define MAXIMUM_EXT_GUIDE_ROUNDNESS_VALUE 110

// NOTE: the next two values implicitly specify when it will say 'go straight through',
// it Might be worth having 'dead' areas between 'left' & 'straight' & 'right' in which it will revert
// to the ordinal method of describing exits

/// how wide is the 'ambiguous' margin between 'straight' and 'turn left' or 'turn right'
#define ROUNDABOUT_AMBIGUOUS_PERCENTAGE (10)
/// how far around a roundabout must you travel before it stops saying 'turn left' (percentage)
#define ROUNDABOUT_GRADE_PERCENTAGE_LEFT (40-ROUNDABOUT_AMBIGUOUS_PERCENTAGE)
/// how far around a roundabout must you travel before it starts saying 'turn right' (percentage)
#define ROUNDABOUT_GRADE_PERCENTAGE_RIGHT (60+ROUNDABOUT_AMBIGUOUS_PERCENTAGE)
/// how far around a roundabout must you travel before it's considered a 'u-turn' for the extended guidance system
/// expressed as a percentage of the roundabouts circumference
#define ROUNDABOUT_GRADE_PERCENTAGE_UTURN 85
/// what is the maximum difference in the entry and exit road angles which will generate 'straight ahead' guidance
#define STRAIGHT_ROADS_ANGLE_THRESHOLD 25
/// what is the range of differences in the entry and exit road angles which will generate 'left/right' guidance
#define CORNER_ANGLE_MIN_THRESHOLD 34
#define CORNER_ANGLE_MAX_THRESHOLD 94
/// what is the maximum acceptable tangent angle for an entry into a roundabout
#define ROUNDABOUT_TANGENT_ENTRY_THRESHOLD 30
/// what is the maximum acceptable tangent angle for an exit from a roundabout
#define ROUNDABOUT_TANGENT_EXIT_THRESHOLD 25
/// what is the smallest radius roundabout that we still care about how 'round' it is
#define ROUNDABOUT_SMALLEST_RADIUS 7

typedef struct {
  TNodeID ID;
  UINT32 intersectionID;
} TRoundaboutNode;


typedef struct {
  TSegmentID ID;
  TNodeID firstNodeID;
  TNodeID lastNodeID;
  TRoadFow roadFow;
  UINT16 turnDirection;
  UINT32 distance_from_start;
  BOOL8 pluralJ;
} TRoundaboutSegment;


typedef struct {
  /// @internal
  /// The list of segments comprising the roundabout.
  TRoundaboutSegment segments[GUIDE_ROUNDABOUT_MAX_SEGMENTS];
  UINT32 segmentCount;

  /// @internal
  /// The list of exit segments connected to the roundabout.
  TRoundaboutSegment exitSegments[GUIDE_ROUNDABOUT_MAX_EXIT_SEGMENTS];
  UINT32 exitCount;

  /// @internal
  /// The total length of the road segments forming the roundabout. We may
  /// decide to implement different behaviour for very large or very small
  /// roundabouts.
  UINT32 totalRoadLength;

  /// @internal
  /// the total length of the roundabout segments which are traversed by the journey
  /// created by gdinters_roundaboutStats
  UINT32 pathRoadLength;

  /// @internal
  /// approximate centroid of the roundabout based on bounding box
  /// this is by-product of gdinters_roundaboutRoundness() which is used when
  /// doing the tangential exit tests for 'extended guidance'
  TPos centroid;

  /// @internal
  /// which side of the road is being driven on?
  TRoadHandedness roadHandedness;

  /// @internal
  /// segment ID where the current journey exits the roundabout
  /// created by gdinters_roundaboutStats
  UINT32 journeyExitSegmentID;

  /// @internal
  /// index into exitSegments[] list where the journeyExitSegmentID was found
  /// created by gdinters_roundaboutStats
  INT32 journeyExitSegmentIndex;

  /// @internal
  /// bit flags that represent all of the exits from a roundabout using ROUNDABOUT_direction_BIT values
  /// (see above eg. ROUNDABOUT_STRAIGHT_BIT)
  UINT16 exitSides;

  /// @internal
  /// Either ROAD_FOW_ROUNDABOUT or ROAD_FOW_TRAFFIC_FIGURE.
  TRoadFow roadFow;

  /// @internal
  /// TRUE if the roundabout is enclosed (it has no gaps or discontinuities).
  /// Technically, this means that each roundabout node must be attached to at
  /// least two roundabout segments.
  BOOL8 isEnclosed;

  /// @internal
  /// TRUE if the roundabout is a 'circular' arrangement of segments. Being
  /// circular means that each roundabout node must be attached to exactly or
  /// two roundabout segments.
  /// - Two roundabout segments if the roundabout is enclosed.
  /// - One roundabout segment if the roundabout is not enclosed.
  ///
  /// If any segment comprising the roundabout is attached to more than two
  /// other roundabout segments, the roundabout is abnormal/deformed.
  BOOL8 isNormal;

  /// @internal
  /// TRUE if all of the segments forming the roundabout or special traffic
  /// figure are one-way.
  BOOL8 isOneWay;

  /// @internal
  /// TRUE if the path taken along the roundabout is in driving order;
  /// this can be false for pedestrian mode.
  BOOL8 parsedInDrivingOrder;

} TGuideRoundabout;


extern BOOL8 gdinters_roundaboutStats(TGuideRoundabout *pThis,
                               const TJourney *pJourney,
                               TGuideInstruction *pInstr);

extern UINT16 gdinters_roundaboutSide( UINT32 percentageAround );

extern INT32 gdinters_roundaboutRoundness( TGuideRoundabout *pThis );

extern BOOL8 gdinters_roundaboutTangentEntry( TGuideRoundabout *pThis, UINT8 *roadAngle );

extern BOOL8 gdinters_roundaboutTangentExit( TGuideRoundabout *pThis, UINT8 *roadAngle );


/// @internal
/// return the ID of any node which is shared between these two segments
/// return 0 if there isn't any such node
extern TNodeID segmentFindSharedNode( TSegmentID segID1, TSegmentID segID2 );

/// @internal
/// return the ID of the node at the other end of this segment than this node
/// return 0 if this node isn't in the segment
extern TNodeID segmentFindOtherNode( TSegmentID segID, TNodeID nodeID );


#ifdef __cplusplus
}
#endif

#endif /* GDROUNDABOUTP_H */
