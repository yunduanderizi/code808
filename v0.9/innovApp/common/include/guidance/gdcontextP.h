#ifndef GDCONTEXTP_H
#define GDCONTEXTP_H

#include <guidance/gdcontext.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @internal
/// The maximum number of close instructions to join and speak using one voice
/// instruction. e.g. "Turn left then turn right".
#define GUIDE_MAX_SPOKEN_INSTRUCTIONS 2


/// @internal
/// The amount to reduce distance-to-turn values when in ped-mode
/// so that the distance is to the sidewalk rather than the road centre
/// (metres)
#define HALF_ROAD_WIDTH_FOR_PEDESTRIAN 5


/// @internal
/// The minimum distance from the previous instruction until we utter the next
/// long distance turn instruction.
/// The delay is introduced due to limited GPS accuracies in pedestrian mode,
/// to make sure that we really have entered the next segment on the path.
#define PED_MODE_WAIT_DISTANCE_LONG       5

/// @internal
/// The minimum distance from the previous instruction until we utter the next
/// short distance turn instruction. (see PED_MODE_WAIT_DISTANCE_LONG)
#define PED_MODE_WAIT_DISTANCE_SHORT      3

/// @internal
/// The maximum distance to the next/coming instruction where we allow to delay
/// uttering it
#define PED_MODE_MAX_WAIT_DISTANCE_SHORT  ((PED_MODE_WAIT_DISTANCE_SHORT) * 4)

/// Real time representation of a guidance instruction. 
typedef struct {
  /// @internal
  /// The static instruction on which the dynamic instruction is based.
  TGuideInstruction *pInstr;
  /// @internal
  /// #TGuideList index of the static guidance instruction.
  INT32 guideListIndex;
  /// @internal
  /// Enumerated distance to the instruction.
  TGuideTurnDistance distanceType;
  /// @internal
  /// Distance in metres from the current position to the instruction.
  UINT32 distance;
  /// @internal
  /// Number of intermediate turns between the current position and the next
  /// turn.
  INT16 intermediateTurns;
  /// @internal
  /// TRUE if the source of the instruction is a freeway (or a major slip road
  /// leading from a freeway, etc.)
  BOOL8 flagSourceFreeway;
} TDynamicInstruction;


extern BOOL8 guideContext_clear(TGuideContext *pThis);

extern void guideContext_setToMetric(TGuideContext *pThis);

extern void guideContext_setToImperial(TGuideContext *pThis);

extern void guideContext_setToImperialYards(TGuideContext *pThis);

extern void guideContext_setToImperialDecimal(TGuideContext *pThis);

extern void guideContext_setToImperialFractional(TGuideContext *pThis);

extern void guideContext_setToHybrid(TGuideContext *pThis);

//@MIC
extern INT32 guideContext_distanceFromRouteCheckLayer(const TGuideContext *pThis);
//@MIC

#ifdef __cplusplus
}
#endif

#endif /* GDCONTEXTP_H */
