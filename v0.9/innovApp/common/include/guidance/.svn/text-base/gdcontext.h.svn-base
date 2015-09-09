#ifndef GDCONTEXT_H
#define GDCONTEXT_H

#include <math/gemath.h>
#include <math/geconvert.h>
#include <routing/rtjourney.h>
#include <guidance/gdinstruction.h>
#include <guidance/gdlist.h>
#include <guidance/gdspeedwarning.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Guidance
/// @{

/// @addtogroup GuideContext
/// @ingroup Guidance
/// @{

/// The minimum repeat distance in meters that can be set using
/// #guideContext_setRepeatDistance
#define GUIDE_MINIMUM_REPEAT_DISTANCE 1000

/** These are the types of guidance distances that can be configured.
    They mostly refer to a set distance to use before the turn.
    They should be set and retrieved using #guideContext_setDistanceThreshold
    and #guideContext_getDistanceThreshold */
typedef enum {
  /** The distance between spoken turn instructions on an otherwise straight
      piece of road. This will allow several "Continue for..." type
      instructions separated by this distance */
  GUIDE_REPEAT_DISTANCE = 0,
  /** The distance that the additional "Prepare to..." freeway instruction
      is given */
  GUIDE_FREEWAY_MEDIUM_DISTANCE,
  /** The distance at which no "Continue for.." turn instruction is given.
      Normally, if the distance is greater than GUIDE_CLOSE_DISTANCE_SPEEDx,
      a "Continue for.." type instruction is give. However, if a turn is
      made, and the distance is less than this, no turn instruction is
      given.  This prevents the "Continue for..." instruction being too
      close to the "In xm..." instruction. */
  GUIDE_NO_LONG_DISTANCE,
  /** As with GUIDE_NO_LONG_DISTANCE except it suppresses SHORT
      instructions while on the freeway */
  GUIDE_NO_SHORT_FREEWAY_DISTANCE,
  /** As with GUIDE_NO_LONG_DISTANCE except it suppresses MEDIUM
      instructions while on the freeway */
  GUIDE_NO_MEDIUM_FREEWAY_DISTANCE,
  /** As with GUIDE_NO_LONG_DISTANCE except it suppresses "Continue for..."
      instructions while on the freeway */
  GUIDE_NO_LONG_FREEWAY_DISTANCE,
  /** The distance (when on a freeway) at which the "Exit ... in xm"
      instruction is given */
  GUIDE_FREEWAY_SHORT_DISTANCE,
  /** The distance (when on a freeway) at which the "Exit..."
      instruction is given */
  GUIDE_FREEWAY_IMMEDIATE_DISTANCE,

  /** The instruction warning distances when on foot */
  GUIDE_IMMEDIATE_DISTANCE_WALKING,
  GUIDE_SHORT_DISTANCE_WALKING,
  GUIDE_LONG_DISTANCE_WALKING,

  /** The distance which the "Turn left in.."/"Turn Right in.."
      instruction is given at high speeds */
  GUIDE_SHORT_DISTANCE_HIGH_SPEED,
  /** The distance which the "Turn left in..."/"Turn Right in..."
      instruction is given at medium speeds */
  GUIDE_SHORT_DISTANCE_MED_SPEED,
  /** The distance which the "Turn left in..."/"Turn Right in..."
      instruction is given at low speeds */
  GUIDE_SHORT_DISTANCE_LOW_SPEED,
  /** The distance which the "Turn left"/"Turn Right" instruction is given */
  GUIDE_IMMEDIATE_DISTANCE,
  /** The distance which the "Destination in xm" instruction is given */
  GUIDE_FINAL_TURN_WARNING_DISTANCE,
  /** The distance at which the final destination (end of route) instruction is given */
  GUIDE_FINAL_DESTINATION_DISTANCE,
  /** The threshold below which concatenated instructions are given.
      E.g., "turn left then turn right" */
  GUIDE_CLOSE_SUBSEQUENT_TURN_THRESHOLD,
  /** The threshold below which concatenated instructions are given
      after a continue instruction
      E.g., "continue left then enter the roundabout" */
  GUIDE_CLOSE_CONTINUE_SUBSEQUENT_TURN_THRESHOLD,
  /** The threshold below which concatenated instructions are given.
      E.g., "bear left then bear right". when heading up an offramp */
  GUIDE_OFFRAMP_SUBSEQUENT_TURN_THRESHOLD,
  /** The threshold below which concatenated freeway instructions are
      given. E.g., "bear left then bear right". */
  GUIDE_FREEWAY_SUBSEQUENT_TURN_THRESHOLD,
  /** The threshold below which ordinal concatenated spoken instructions are
      given. E.g. "Turn left then take the second right". */
  GUIDE_ORDINAL_SUBSEQUENT_TURN_THRESHOLD,
  /** If the short instruction is given at less than this distance, we do not
      speak any road names with the immediate instruction. */
  GUIDE_IMMEDIATE_ROAD_NAMES_DISTANCE,
  /** @internal
      Counter for the number of valid turn distance types. This should not
      be used directly. */
  GUIDE_NUM_TURN_DISTANCES
} TGuideDistanceType;

/** Enumerates the types of speed that are allowed for the "Turn left"/
    "Turn right" type turn instructions. The speeds used are in meters per
    second.  Any speed below the #GUIDE_MED_SPEED is considered low speed */
typedef enum {
  /// High speeds use the distance enumerated in
  /// #GUIDE_SHORT_DISTANCE_HIGH_SPEED for the turn instruction distance
  GUIDE_HIGH_SPEED = 0,
  /// Medium speeds use the distance enumerated in
  /// #GUIDE_SHORT_DISTANCE_MED_SPEED for the turn instruction distance
  GUIDE_MED_SPEED,
  /// @internal
  /// Counter for the number of valid turn speed types. This should not be
  /// used directly.
  GUIDE_NUM_SPEEDS
} TGuideSpeedType;

/// TInstructionReturnCode is used to report back if an instruction was given
/// or a reason why the instruction was not given.
typedef enum {
  /// @deprecated
  /// This instruction return code is no longer used.
  NOGUIDE_BELOW_DETAIL_THRESHOLD = -100,
  /// @deprecated
  /// This instruction return code is no longer used.
  NOGUIDE_NO_TIME_CHANGE,
  /// @deprecated
  /// This instruction return code is no longer used.
  NOGUIDE_INVALID_DISTANCE,
  /// No guidance can be given. The journey is not valid.
  NOGUIDE_NO_ROUTE,
  /// No guidance can be given. The #TGuideList is empty.
  NOGUIDE_NO_TURNS,
  /// No guidance can be given. Maplock is not locked to a road segment.
  NOGUIDE_NO_LOCK,
  /// No guidance can be given. Maplock is locked to a road segment that is not
  /// on the path for the current journey.
  NOGUIDE_NOT_ON_ROUTE,
  NOGUIDE_ON_ROUTE_APPROACH_POINT,
  /// A guidance voice instruction was given.
  GUIDE_OK = 0,
  /// No guidance given. The voice instruction for the current turn at this
  /// distance threshold has already been given.
  NOGUIDE_ALREADY_GIVEN,
  /// No guidance given. The distance to the next turn is too great for a voice
  /// instruction to be given.
  NOGUIDE_WAITING,
  /// The car is heading the wrong way. A "perform a u-turn when possible"
  /// voice instruction was given.
  GUIDE_UTURN,
  /// No guidance given. The car is heading the wrong way but it is too soon
  /// to give a u-turn voice instruction.
  NOGUIDE_UTURN_TOO_SOON,
  /// No guidance given. The car is heading the wrong way but it is travelling
  /// too slow to give a u-turn voice instruction.
  NOGUIDE_TOO_SLOW,
  /// No guidance given. The car is heading the wrong way, and a "perform a
  /// u-turn when possible" instruction has already been given.
  NOGUIDE_UTURN_ALREADY_GIVEN
} TInstructionReturnCode;

/** This is the form of the callback function that displays guidance
    information on the screen.

    @param[in] turn The index of the next turn */
typedef void TGuideTurnFunc(INT32 turn);

/** This is the form of the callback function that indicates that
    a destination has been reached

    @param[in] legNumber the internal index of the destination reached */
typedef void TDestinationReachedFunc(TJourneyLegNumber legNumber);

typedef void *TJunctionViewData;
typedef void  TJunctionViewCallback(UINT32             event,
                                    TJunctionViewData  junctionViewData,
                                    void              *appContext);

typedef enum TJunctionImageDayNightType {
  JUNCTION_IMAGE_DAY,
  JUNCTION_IMAGE_NIGHT,
} TJunctionImageDayNightType;

/// @internal
/// The following distances determine the variation of spoken guidance that
/// is given. For example, "Turn Left" or "In 400 metres turn left".
/// @note
/// The ordering of these values is important as it is used by the
/// #closerDistance function.
typedef enum {
  GUIDEDISTANCE_IMMEDIATE,
  GUIDEDISTANCE_SHORT,
  GUIDEDISTANCE_MEDIUM,
  GUIDEDISTANCE_WAIT,
  GUIDEDISTANCE_LONG,
  GUIDEDISTANCE_UTURN,
  GUIDEDISTANCE_REINSTRUCT,
  GUIDEDISTANCE_INVALID
} TGuideTurnDistance;


/// This structure encapsulates all the state information of a journey leg.
/// At the moment it contains only one flag, but that might change in the future.
typedef struct {
  /// @internal
  /// Has the initial instruction already been uttered?
  BOOL8 initialInstructionUttered;
} TGuideLegContext;


/// Guidance context. This object provides support for the real time delivery
/// of guidance instructions from a guidance list. The #TGuideList is delivered
/// using turn-by-turn voice instructions. The primary interface is through
/// #guideContext_update, which reports guidance via the speech module and the
/// user defined callback functions.
typedef struct {
  /// @internal
  /// Guidance list from which this context is derived
  TGuideList *guideList;
  /// @internal
  /// The state/context of each journey leg
  TGuideLegContext legContexts[ITINERARY_MAX_DESTINATIONS];
  /// @internal
  /// Speed / safety warning
  TSpeedWarning speedWarning;
  /// @internal
  /// Distance measurement units (metric, imperial, etc.)
  TDistanceCombination distanceUnits;
  /// @internal
  /// Handedness of the current maplocked road.
  TRoadHandedness handedness;
  /// @internal
  /// UTC last time guidance was updated and the car was on the journey
  UINT32 lastValidTime;
  /// @internal
  /// UTC time at which a UTurn was last detected
  UINT32 uTurnTime;
  /// @internal
  /// The distances at which instructions are delivered before turns
  UINT32 distanceThresholds[GUIDE_NUM_TURN_DISTANCES];
  /// @internal
  /// Speeds that determine which distance thresholds are used. If the
  /// current speed is high, the distances are generally longer
  UINT8 speedThresholds[GUIDE_NUM_SPEEDS];
  /// @internal
  /// Callback function for when we pass a turn
  TGuideTurnFunc *reportTurnFunc;
  /// @internal
  /// Callback function for when we pass a destination
  TDestinationReachedFunc *destinationReachedFunc;
  /// @internal
  /// Callback function for a junctionView event
  TJunctionViewCallback *junctionViewCallback;
  /// @internal
  /// The currently active junctionView.
  TJunctionViewInfo *currentJunctionView;
  /// @internal
  /// The turn's guidance index for junction view
  INT32 nCurrentJunctionViewIndex;
  /// @internal
  /// The turn's guidance index shown on tick count
  UINT32 nCurrentJunctionViewTickCount;
  /// @internal
  /// Opaque data registered to be passed back to the App along with
  /// junctionView callbacks.
  void *junctionViewOpaqueAppData;
  /// @internal
  /// Distance in metres from the current turn at which to repeat the
  /// instruction. Typically only applies to long turn distances
  UINT32 repeatDistance;
  /// @internal
  /// The minimum distance in metres between repeated instructions
  UINT32 minimumRepeatThreshold;
  /// @internal
  /// The last spoken instruction index. This is recorded so that instructions
  /// are not repeated unnecessarily.
  INT32 lastGivenGuideIndex;
  /// @internal
  /// The last spoken instruction distance. This is recorded so that
  /// instructions are not repeated unnecessarily.
  TGuideTurnDistance lastGivenDistanceType;
  /// @internal
  /// The effective distance to turn at which the last voice instruction was given.
  UINT32 lastGivenDistance;
  /// @internal
  /// These values refer to the delay in the system in m/s that
  /// we spend before actually managing to finish delivering a speech prompt
  /// They are currently just estimates.
  /// Speech length is how long it takes to spit it out
  UINT32 speechLength;
  /// @internal
  /// The time taken to deliver new pos info, e.g., slower GPS units deliver position info quite late.
  UINT32 positionLatency;
  /// @internal
  /// If this is set, guidance believes a reroute and a reset of the guidelist
  /// would be a good idea
  BOOL8 rerouteHint;
} TGuideContext;

/*----------------------------------------------------------------*/

extern MAPCORE_API
BOOL8 guideContext_init(TGuideContext *pThis);

extern MAPCORE_API
BOOL8 guideContext_reset(TGuideContext *pThis,
                                TGuideList *pGuideList);

extern MAPCORE_API
TInstructionReturnCode guideContext_update(TGuideContext *pThis);

//guideContext_update for landmark guidance
//current landmark guidance just support Traditional Chinese and Simplified Chinese
//landmarkName1 and landmarkName2 can be NULL if has no landmark guidnace
//in current landmark guidance, we insert landmark name to guide instruction sentence
//below is the current rule:
//1.
//if the guide instruction include traffic light and we will replace the traffic light with the landmark name.
//
//2.
//If ( guide instruction include "turn lelf" or "turn right" or "sharp left turn" or "sharp right turn" or ""U-turn")
//{  
//  If (guide instruction doesn't include "traffic light" &&
//      "turn lelf", "turn right", "sharp left turn", "sharp right turn", "U-turn" isn't the first token of the instruction)
//  {
//    Insert the landmark name before "turn lelf" or "turn right" or "hard left turn" or "hard right turn" or "U-turn"
//  }
//}
//
//3.
//if sentence has two instruction and combine by "then", and we can use the 2nd landmark name in later instruction
//landmarkName1 use on first instruction and landmarkName2 use on second instruction.
//maybe name1 and name2 have landmark or one of name has landmark or both them are NULL.
extern MAPCORE_API
TInstructionReturnCode guideContext_updateForLMGD(TGuideContext *pThis,
                                                  UTF16         *landmarkName1,
                                                  UTF16         *landmarkName2);

extern MAPCORE_API
void guideContext_installReportTurnFunction(TGuideContext  *pThis,
                                            TGuideTurnFunc *func);

extern MAPCORE_API
void guideContext_installDestinationReachedFunction(TGuideContext *pThis,
                                                    TDestinationReachedFunc *func);

extern MAPCORE_API
void guideContext_registerJunctionViewCallback(TGuideContext         *pThis,
                                               TJunctionViewCallback *func,
                                               void                  *opaqueAppData);

extern MAPCORE_API
BOOL8 guideContext_getJunctionViewContext(TGuideContext       *pThis,
										                      UINT32               guideIndex,
                                          TJunctionViewData    *junctionViewData,
										                      void                **appContext);

extern MAPCORE_API
BOOL8 guideContext_setDistanceUnits(TGuideContext *pThis, TDistanceCombination unit);

//@TPE Estimate the travel time and distance to destination.
extern MAPCORE_API
BOOL8 guideContext_timeAndDistanceToDestination(const TGuideContext *pThis,
                                         INT32 *distanceToDestination,
                                         INT32 *timeToDestination);

extern MAPCORE_API
BOOL8 guideContext_timeAndDistanceToTurn(const TGuideContext *pThis,
                                         INT32 guideIndex,
                                         INT32 *distanceToTurn,
                                         INT32 *timeToTurn);

extern MAPCORE_API
INT32 guideContext_getNextTurnIndex(const TGuideContext *pThis);

extern MAPCORE_API
void guideContext_reinstruct(TGuideContext *pThis);

extern MAPCORE_API
INT32 guideContext_timeFromRoute(const TGuideContext *pThis);

extern MAPCORE_API
void guideContext_setRepeatDistance(TGuideContext *pThis, UINT32 distance);

extern MAPCORE_API
void guideContext_setDistanceThreshold(TGuideContext *pThis,
                                       TGuideDistanceType type,
                                       UINT32 distance);

extern MAPCORE_API
UINT32 guideContext_getDistanceThreshold(const TGuideContext *pThis,
                                         TGuideDistanceType type);

extern MAPCORE_API
BOOL8 guideContext_setShortSpeed(TGuideContext *pThis,
                                 TGuideSpeedType type,
                                 UINT8 speed);

extern MAPCORE_API
UINT8 guideContext_getShortSpeed(const TGuideContext *pThis,
                                 TGuideSpeedType type);

extern MAPCORE_API
void guideContext_setPositionLatency(TGuideContext *pThis,
                                     const INT32 milliSeconds);

extern MAPCORE_API
TGuideList *guideContext_getGuideList(TGuideContext *pThis);

extern MAPCORE_API
TJourney *guideContext_getJourney(const TGuideContext *pThis);

extern MAPCORE_API
TSpeedWarning *guideContext_getSpeedWarning(TGuideContext *pThis);

extern MAPCORE_API
BOOL8 guideContext_getRerouteHint(const TGuideContext *pThis);

extern MAPCORE_API
INT32 guideContext_distanceFromRoute(const TGuideContext *pThis);

extern MAPCORE_API
BOOL8 guideContext_subsequentInstructionExpected(const TGuideContext* pThis, INT32 index);

extern MAPCORE_API
UINT32 getJunctionViewPathCount(TGuideContext* pGuideContext, INT32 nGuideIndex);

BOOL8 guideContext_resetJunctionViewList(TGuideContext* pThis);

BOOL8 guideContext_getNextJunctionView(TGuideContext *pGuideContext, INT32 nCurrentIdx, UINT32 nRange, INT32* pnGuideIndex, INT32* pnDistanceToNext, INT32* pnTimeToNext);

BOOL8 guideContext_checkJunctionView(INT32 nCurrentIdx, TGuideContext *pGuideContext);

//this function support the junction view disappear after go through all junction view path
BOOL8 guideContext_getNextJunctionView2(TGuideContext *pGuideContext, 
                                        INT32          nCurrentIdx, 
                                        UINT32         nRange, 
                                        INT32         *pnGuideIndex, 
                                        INT32         *pnDistanceToNext, 
                                        INT32         *pnTimeToNext,
                                        INT32         *jvPathSegIdx);

//this function support the junction view disappear after go through all junction view path
BOOL8 guideContext_checkJunctionView2(INT32          nCurrentIdx, 
                                      TGuideContext *pGuideContext, 
                                      INT32          pathSegIndex);

//@MIC
BOOL8 guideContext_expandSegments(TSegmentID sourceSegID, TNodeID sourceNode, UINT16 expandRange, UINT8 expandSegments, TSegmentID* segmentsList, UINT32* expandedLength, UINT8* expandedSegments);

TJunctionViewStatus getJunctionViewStatus(TGuideContext* pGuideContext, INT32 nGuideIndex);

TJunctionViewInfo* getJunctionViewInfo(TGuideContext* pGuideContext, INT32 nGuideIndex);

//INT32 guideContext_getNextTurnIndexInternal(const TGuideContext *pThis);

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif /* GDCONTEXT_H */
