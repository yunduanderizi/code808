#ifndef GDINSTRUCTION_H
#define GDINSTRUCTION_H

#include <routing/rtjourney.h>
#include <mapdata/enroad.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Guidance
/// @{

/// @addtogroup GuideInstruction
/// @ingroup Guidance
/// @{

///@internal Update guideInstruction_dump() in gdinstruction.c
/// if you alter these masks.
#define GUIDE_MASK_FREEWAY      0x00000001
#define GUIDE_MASK_FERRY        0x00000002
#define GUIDE_MASK_PRIVATE      0x00000004
#define GUIDE_MASK_UNSEALED     0x00000008
#define GUIDE_MASK_TOLL         0x00000010
#define GUIDE_MASK_BRIDGE       0x00000020
#define GUIDE_MASK_TUNNEL       0x00000040

// These three masks are not used by the language engine
#define GUIDE_MASK_FROM_FREEWAY       0x00000080
#define GUIDE_MASK_TO_FREEWAY         0x00000100
#define GUIDE_MASK_FROM_SLIPROAD      0x00000200
#define GUIDE_MASK_TO_SLIPROAD        0x00000400

// These will be used for "at the..." instructions
#define GUIDE_MASK_TRAFFIC_LIGHTS     0x00000800
#define GUIDE_MASK_ROUNDABOUT         0x00001000

// Allow the intersection pictorial code (intersectionPic_setGuideIndex()) to handle
// cases where extended guidance is used at roundabouts (eg. 'at roundabout turn left'
// rather than 'at roundabout take the 1st exit')
#define GUIDE_MASK_ROUNDABOUT_EXIT    0x00002000

#define GUIDE_MASK_END_OF_ROAD        0x00080000
#define GUIDE_MASK_BEYOND_WORMHOLE    0x00100000

// The instruction is a multi-lane or simple bifurcation.
#define GUIDE_MASK_IMPORTANT_BIFURCATION 0x00200000

// @todo Dutch, Danish and German use two different words for 'exit'. One is
// used for motorway exits, the other is for roundabout exits (and all other
// exits?). Instead of a single TURNSTYLE_EXIT, we really need to have two
// different turn styles, one for each type. For now, to avoid introducing a
// new turn style or changing the public API, we accomplish the same thing
// using a flag mask.
#define GUIDE_MASK_FREEWAY_STYLE_EXIT 0x00400000

// Some countries should override the default language
// setting when announcing exits. The following
// two values should be mutually exclusive. Either one
// or neither should be set.
#define GUIDE_MASK_USE_JUNCTIONS      0x00800000
#define GUIDE_MASK_USE_EXITS          0x01000000

// Source and dest names match
#define GUIDE_MASK_NAMES_MATCH        0x02000000

// for attribute change from (exit bridge, tunnel, overpass, highway)
#define GUIDE_MASK_OVERPASS           0x04000000
#define GUIDE_MASK_HIGHWAY            0x08000000
#define GUIDE_MASK_FASTWAY            0x10000000
#define GUIDE_MASK_SLOWWAY            0x20000000
#define GUIDE_MASK_UNDERPASS          0x40000000

#define GUIDE_MASK_EXIT_BRIDGE        (GUIDE_MASK_BRIDGE    | GUIDE_MASK_USE_EXITS)
#define GUIDE_MASK_EXIT_TUNNEL        (GUIDE_MASK_TUNNEL    | GUIDE_MASK_USE_EXITS)
#define GUIDE_MASK_EXIT_OVERPASS      (GUIDE_MASK_OVERPASS  | GUIDE_MASK_USE_EXITS)
#define GUIDE_MASK_EXIT_HIGHWAY       (GUIDE_MASK_HIGHWAY   | GUIDE_MASK_USE_EXITS)
#define GUIDE_MASK_EXIT_FASTWAY       (GUIDE_MASK_FASTWAY   | GUIDE_MASK_USE_EXITS)
#define GUIDE_MASK_EXIT_SLOWWAY       (GUIDE_MASK_SLOWWAY   | GUIDE_MASK_USE_EXITS)
#define GUIDE_MASK_EXIT_UNDERPASS     (GUIDE_MASK_UNDERPASS | GUIDE_MASK_USE_EXITS)

#define GUIDE_MASK_ATTRIBUTE_CHANGE (GUIDE_MASK_FREEWAY \
                                     | GUIDE_MASK_FERRY \
                                     | GUIDE_MASK_PRIVATE \
                                     | GUIDE_MASK_UNSEALED \
                                     | GUIDE_MASK_TOLL \
                                     | GUIDE_MASK_BRIDGE \
                                     | GUIDE_MASK_TUNNEL \
									 | GUIDE_MASK_FASTWAY \
									 | GUIDE_MASK_SLOWWAY \
									 | GUIDE_MASK_UNDERPASS \
									 | GUIDE_MASK_OVERPASS)

/// A set of GUIDE_MASK attributes associated with an instruction
typedef UINT32 TGuideFlag;


/// Describes the style of a turn. The turn style greatly influences the
/// kind of guidance instructions given to the user. Most turns will also be
/// "graded". That is, they are associated with a #TGuideTurnGrade.
/// @internal Update GuideTurnStyleString[] in gdinstruction.c if
/// you update TGuideTurnStyle
typedef enum {
  /** This instruction should be removed from the list */
  TURNSTYLE_REMOVE_THIS = -1,
  /** There is no turn (give a "continue" instruction) */
  TURNSTYLE_NO_TURN,
  /** The turn is graded. That is, it may be sharp, slight, medium etc. */
  TURNSTYLE_GRADED_TURN,
  /** The turn is leaving a motorway or roundabout */
  TURNSTYLE_EXIT,
  /** @deprecated The turn is at a point roundabout */
  TURNSTYLE_POINT_ROUNDABOUT,
  /** @deprecated The turn is at the entrance of a (non-point) roundabout */
  TURNSTYLE_ENTERING_ROUNDABOUT,
  /** @deprecated The turn is leaving a (non-point) roundabout */
  TURNSTYLE_EXITING_ROUNDABOUT,
  /** @deprecated The turn is at a motorway bifurcation */
  TURNSTYLE_BIFURCATION,
  /** @deprecated The turn is at an offramp bifurcation (rare!) */
  TURNSTYLE_OFFRAMP_BIFURCATION,
  /** @deprecated The turn is onto an offramp from a motorway */
  TURNSTYLE_OFFRAMP,
  /** The turn is a "good" u-turn such as at a dual carriageway u-turn bay. To
      give a "bad" u-turn, use the function language_speakUTurnInstruction. */
  TURNSTYLE_UTURN,
  /** The "turn" is the end of the planned route (single stop) */
  TURNSTYLE_TERMINATION,
  /** The "turn" is the end of a leg of a multistop route */
  TURNSTYLE_INTERMEDIATE_DESTINATION,
  /** The "turn" is the end of the last leg of a multistop route */
  TURNSTYLE_FINAL_DESTINATION,
  /** The "Turn" is entering a ferry, unsealed road etc. */
  TURNSTYLE_ATTRIBUTE_CHANGE,
  /** The turn is a merge onto (for example) a controlled access road */
  TURNSTYLE_MERGE,
  /** The turn is a curve (advisory) */
  TURNSTYLE_CURVE,
  /** The turn is a lane change */
  TURNSTYLE_LANE,
  /** The signs should be followed */
  TURNSTYLE_FOLLOW_SIGNS,
  /** Approaching an important road or POI */
  TURNSTYLE_APPROACHING,
  /** The road joins another */
  TURNSTYLE_JOIN,
  /** This is the first turn on a journey leg - treated differently eg for ped-mode */
  TURNSTYLE_INITIAL,
  /** Last style */
  TURNSTYLE_MAX
} TGuideTurnStyle;


/// Describes the possible turn grades. A single turn to the left or right
/// will probably be "normal". If there is more than one possible turn, the
/// grade is relative. This helps to ensure that no ambiguous instructions
/// are given.
/// @internal Update GuideTurnGradeString[] in gdinstruction.c if
/// you update TGuideTurnGrade
typedef enum {
  /** There is no turn */
  TURNGRADE_NO_TURN = -1,
  /** @deprecated No modifier is necessary */
  TURNGRADE_NORMAL = 0,
  /** The turn is a slight turn */
  TURNGRADE_SLIGHT,
  /** @deprecated The turn is a slight-to-medium turn */
  TURNGRADE_SLIGHT_MEDIUM,
  /** The turn is a medium turn */
  TURNGRADE_MEDIUM,
  /** @deprecated The turn is a medium-to-sharp turn */
  TURNGRADE_MEDIUM_SHARP,
  /** The turn is a sharp turn */
  TURNGRADE_SHARP,
  /** End of comparable turn grades */
  TURNGRADE_MAX_COMPARABLE,
  /** @deprecated Slight turn hint */
  TURNGRADE_BEAR,
  /** Reassurance "Continue Left" etc. */
  TURNGRADE_CONTINUE,
  /** "Keep Left" etc. Used at bifurcations. */
  TURNGRADE_KEEP,
  /** Last grade. */
  TURNGRADE_MAX
} TGuideTurnGrade;

/// Direction of a turn. That is, left, right or straight ahead.
/// @internal Update GuideTurnDirectionString[] in gdinstruction.c if
/// you update TGuideTurnDirection
typedef enum {
  /** The turn is to the left */
  TURNDIRECTION_LEFT = -1,
  /** There is no turn */
  TURNDIRECTION_STRAIGHT = 0,
  /** The turn is to the right */
  TURNDIRECTION_RIGHT = 1,
  /** The turn follows the current road */
  TURNDIRECTION_CURRENT_ROAD = 7
} TGuideTurnDirection;


typedef enum {
  CARDINAL_UNKNOWN = -1,
  CARDINAL_FIRST = 0,      // marker; the first valid cardinal direction in the enum

  CARDINAL_NORTH = CARDINAL_FIRST,
  CARDINAL_SOUTH,
  CARDINAL_EAST,
  CARDINAL_WEST,
  CARDINAL_NORTHEAST,
  CARDINAL_NORTHWEST,
  CARDINAL_SOUTHEAST,
  CARDINAL_SOUTHWEST
} TCardinalDirection;


typedef enum {
  /** The instruction is essential */
  INSTR_ESSENTIAL,
  /** The instruction provides useful but non-essential information */
  INSTR_ADVISORY,
  /** The instruction contains no useful information */
  INSTR_REDUNDANT
} TGuideInstrPriority;


/// Which lanes should the driver try to be driving in?
typedef struct {
  // how many lanes are there available for the driver?
  INT32 numLanes;
  // if lanesToUse is zero then there is no useful lane information
  UINT32 lanesToUse;
  // should we mention which lanes to drive in with the spoken guidance
  BOOL8 sayLanes;
  // Lane Pictorial Data
  // Lane Direction data for each of the lanes
  UINT16 laneDir[ROAD_MAX_LANES];
  // Lane direction data corresponding to the travel direction
  UINT16 useLaneDir[ROAD_MAX_LANES];
} TLaneInfo;

typedef enum TJunctionViewStatus {
  JUNCTION_VIEW_UNINITIALIZED = 0,
  JUNCTION_VIEW_AVAILABLE,          /* This JV is available for guidance. */
} TJunctionViewStatus;

typedef struct {
  /* Last callback event triggered on behalf of this junctionView. */
  TJunctionViewStatus status;
 
  UINT32 lastSegmentIdx;

  UINT32 jvPathCount;

  /* The identity of the containing road-map. */
  UINT16              roadMapIndex;
  /* Number of component images making up the pictorial. */
  UINT16              numComponents;
  /* Indices into the sections of the corresponding road-map's
   * junctionImageSet chunk; these need to be resolved into the
   * corresponding archiveMap indices. */
  UINT16              imageSetIndices[MAX_JUNCTION_IMAGE_TYPES];
} TJunctionViewInfo;

#define JUNCTION_VIEW_DISPLAY_EVENT 1 /* A JV is current and displayable */
#define JUNCTION_VIEW_RESET_EVENT   0 /* A previously current JV is no
                                         longer applicable. */

/// Detailed description of the nature of a guidance instruction.
typedef struct {
  /// Turn style. E.g. bifurcation, offramp, roundabout
  TGuideTurnStyle style;
  /// Slight (1) through sharp (5), bear (7), or no modifier (0)
  TGuideTurnGrade grade;
  /// Turn direction. Left, right or straight
  TGuideTurnDirection direction;
  /// Heading direction as a cardinal (N,S,E,W,NE,NW, etc)
  TCardinalDirection cardinalDirection;
  /// What type of vehicle is the current journey leg being travelled by
  TVehicleType vehicle : 8;
  /// Journey leg this instruction belongs to
  TJourneyLegNumber legNumber;
  /// @internal
  /// heading from the node to the source segment
  UINT8 sourceHeading;
  /// uses the GUIDE_MASK_ defines
  TGuideFlag flagMask;
  /// @internal
  /// Index of the entry segment in the list of path segments
  INT32 sourcePathIndex;
  /// @internal
  /// The number of path segments in the instruction including the source
  /// segment, exit segment, and all of the intermediate segments.
  INT16 numPathSegs;
  /// @internal
  /// The number of path segments before the instruction node (including the
  /// source segment).
  INT16 numPathSegsBeforeNode;
  /// @internal
  /// Road node of the instruction. 0 if the instruction is not located at a node.
  TNodeID nodeID;
  /// @internal
  /// Incoming road segment
  TSegmentID entrySegment;
  /// @internal
  /// Outgoing road segment
  TSegmentID exitSegment;
  /// @internal
  /// The segment containing the road name for this instruction
  TSegmentID roadNameSegment;
  /// @internal
  /// The lane information if any is available for the upcoming intersection
  TLaneInfo laneInfo;
  /// @internal
  /// The junctionView info., if any is available, for the upcoming intersection
  TJunctionViewInfo junctionViewInfo;
  /// @internal
  /// Driving distance to the end of the journey in metres.
  UINT32 distanceToEnd;
  /// @internal
  /// Estimated travel time to the end of the journey in metres.
  UINT32 timeToEnd;
  /// @internal
  /// longitude/latitude position of the (0:start 1:end of the) turn (marked by chijung.lin for smartst compatable issue)
  //TPos pos[2];//marked by chijung.lin(for SmartST compatable issue)
  TPos pos;
  /// @internal
  /// count of intermediate turns since the previous instruction
  INT16 intermediateTurns;
  /// @internal
  /// Packed signpost strings. Note that this is actually a pointer to a
  /// #TGuideInstrSignPostInfo structure.
  void *signPostInfo;
  /// @internal
  /// Priority of the instruction. Used as a clue
  /// as to when the instruction should be delivered or
  /// not.
  TGuideInstrPriority priority;
  /// @internal
  /// Intersection ID. 0 if there is no intersection ID
  UINT32 intersectionID;
  /// @internal
  /// The type of intersection (TRoadIntersectionType enumerated value)
  INT8 intersectionType;
  /// @internal
  /// This is a hint to a #TGuideContext that the instruction can be spoken in
  /// the ordinal style. E.g. "Take the second left".
  BOOL8 isOrdinal;
  /// @internal
  /// Location uttered name for destination instructions
  void  *utteredName;
} TGuideInstruction;

/*----------------------------------------------------------------*/

extern MAPCORE_API
UINT8 guideInstr_getSourceHeading(const TGuideInstruction *pThis);

extern MAPCORE_API
UINT8 guideInstr_getTurnDegree(const TGuideInstruction *pThis);

extern MAPCORE_API
BOOL8 guideInstr_getPosition(const TGuideInstruction *pThis,
                                   TPos *position);

extern MAPCORE_API
BOOL8 guideInstr_isEndOfJourney(const TGuideInstruction *pThis);

extern MAPCORE_API
BOOL8 guideInstr_isEndOfLeg(const TGuideInstruction *pThis);

extern MAPCORE_API
BOOL8 guideInstr_isRoundaboutEntry(const TGuideInstruction *pThis);

extern MAPCORE_API
BOOL8 guideInstr_isRoundaboutExit(const TGuideInstruction *pThis);

extern MAPCORE_API
BOOL8 guideInstr_isMotorwayExit(const TGuideInstruction *pThis);

extern MAPCORE_API
BOOL8 guideInstr_isRightTurn(const TGuideInstruction *pThis);

extern MAPCORE_API
BOOL8 guideInstr_isLeftTurn(const TGuideInstruction *pThis);


/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif /* GDINSTRUCTION_H */
