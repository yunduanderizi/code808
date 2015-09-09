#ifndef GDLIST_H
#define GDLIST_H

#include <guidance/gdinstruction.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Guidance
/// @{

/// @addtogroup GuideList
/// @ingroup Guidance
/// @{


/// Guidance detail flags. These provide the ability to control the amount
/// of optional guidance detail that is generated. By default,
/// #GUIDEFLAG_ROADNAME_CHANGES, #GUIDEFLAG_ENTERING_FREEWAY and
/// #GUIDEFLAG_ENTERING_FERRY are enabled. All other detail flags are
/// disabled. Detail flags can be set using #guideList_setDetailFlag.
///
/// @todo GUIDEFLAG_TRAFFIC_LIGHTS is not implemented.
typedef enum {
  /// @deprecated
  /// This flag is obsolete.
  GUIDEFLAG_EXITING_ROUNDABOUT,
  /// Give guidance when the turn angle is steep and the road name
  /// changes even if there is only one possibility
  GUIDEFLAG_ROADNAME_CHANGES,
  /// Give instruction when entering a motorway / freeway.
  GUIDEFLAG_ENTERING_FREEWAY,
  /// Give instruction when entering ferry.
  GUIDEFLAG_ENTERING_FERRY,
  /// Give instruction when entering private road.
  GUIDEFLAG_ENTERING_PRIVATE,
  /// Give instruction when entering unsealed road.
  GUIDEFLAG_ENTERING_UNSEALED,
  /// Give instruction when entering toll zone.
  GUIDEFLAG_ENTERING_TOLL,
  /// Give instruction when entering a bridge.
  GUIDEFLAG_ENTERING_BRIDGE,
  /// Give instruction when entering a tunnel.
  GUIDEFLAG_ENTERING_TUNNEL,
  /// Give guidance detail about traffic lights.
  GUIDEFLAG_TRAFFIC_LIGHTS,
  /// Give guidance when we cross a major intersection. On by default
  GUIDEFLAG_CROSS_MAJOR,
  /// Always give some guidance when we cross a complex intersection. On by default
  GUIDEFLAG_CROSS_COMPLEX,
  /// Allow advisory speech guidance close to decision points
  GUIDEFLAG_CLOSE_ADVISORIES,
  /// Give guidance at every intersection. You ask, you get. Off by default.
  GUIDEFLAG_EVERY_INTERSECTION,
  /// @internal
  /// GUIDEFLAG_MAX is always be the last TGuidanceDetailFlag
  GUIDEFLAG_MAX
} TGuidanceDetailFlag;


/// @internal
/// This structure stores additional information for a leg, which is used, when
/// we want to recycle part of the guidance list.
typedef struct {
  TPathId pathID;
  UINT32  distanceToEnd;
  UINT32  timeToEnd;
  UINT32  wormholeDistanceToEnd;
  UINT32  numTurnsToEnd;
  UINT32  textSizeToEnd;
} TGuideLegInfo;


/// A list of guidance instructions for a particular #TJourney. The primary
/// interface to the guide list is through #guideList_reset, which creates
/// the list of instructions. Once a list has been created, the instructions
/// can be delivered using a #TGuideContext.
typedef struct {
  /// @internal
  /// A memory buffer for storing guidance instructions. The memory is
  /// partitioned into two regions:
  ///   - The start of the buffer is populated by TGuideInstruction objects.
  ///   - The end of the buffer is allocated to instruction route and exit
  ///     number strings associated with some instructions.
  TGuideInstruction *list;
  /// @internal
  /// The journey used to create the instruction list
  TJourney          *pJourney;
  /// @internal
  /// Information stored for each leg, to allow recycling part of a journey
  /// during a reroute calculation.
  TGuideLegInfo     legInfos[ITINERARY_MAX_DESTINATIONS];
  /// @internal
  /// Total length of the journey in metres
  UINT32            totalDistance;
  /// @internal
  /// Total journey travel time estimate in seconds
  UINT32            totalTime;
  /// @internal
  /// Distance of first (upward) wormhole from end of path
  UINT32            wormholeDistance;
  /// @internal
  /// The number of instructions in the list
  INT32             numTurns;
  /// @internal
  /// The total size of the instruction buffer in bytes (including the text
  /// buffer at the end of the buffer).
  UINT32            bufferSize;
  /// @internal
  /// The number of bytes allocated for storage of strings.
  UINT32            textSize;
  /// @internal
  /// Detail flags used to control the amount of optional guidance generated
  BOOL8             guidanceDetail[GUIDEFLAG_MAX];
} TGuideList;

/*----------------------------------------------------------------*/

extern MAPCORE_API
INT32 guideList_init(TGuideList *pThis,
                     void *instructionBuffer,
                     UINT32 bufferSize);

extern MAPCORE_API
BOOL8 guideList_reset(TGuideList *pThis,
                      TJourney *pJourney);

extern MAPCORE_API
TJourney *guideList_getJourney(const TGuideList *pThis);

extern MAPCORE_API
INT32 guideList_firstIndexForLeg(const TGuideList *pThis,
                                 TJourneyLegNumber legNumber);

extern MAPCORE_API
INT32 guideList_lastIndexForLeg(const TGuideList *pThis,
                                TJourneyLegNumber legNumber);


extern MAPCORE_API
UINT32 guideList_distanceToEnd(const TGuideList *pThis,
                               INT32 index);

extern MAPCORE_API
UINT32 guideList_timeToEnd(const TGuideList *pThis,
                           INT32 index);

extern MAPCORE_API
UINT32 guideList_distanceFromStart(const TGuideList *pThis,
                                   INT32 index);

extern MAPCORE_API
UINT32 guideList_timeFromStart(const TGuideList *pThis,
                               INT32 index);

extern MAPCORE_API
UINT32 guideList_distanceBetweenInstr(const TGuideList *pThis,
                                      INT32 index1,
                                      INT32 index2);

extern MAPCORE_API
UINT32 guideList_timeBetweenInstr(const TGuideList *pThis,
                                  INT32 index1,
                                  INT32 index2);

extern MAPCORE_API
TGuideInstruction *guideList_get(const TGuideList *pThis,
                                 INT32 index);

extern MAPCORE_API
BOOL8 guideList_getSignpost(const TGuideList *pThis,
                    INT32 index,
					UTF16* szUTF,
					UINT32 nSize);

extern MAPCORE_API
BOOL8 guideList_isEmpty(const TGuideList *pThis);

extern MAPCORE_API
BOOL8 guideList_isFull(const TGuideList *pThis);

extern MAPCORE_API
INT32 guideList_size(const TGuideList *pThis);

extern MAPCORE_API
INT32 guideList_capacity(const TGuideList *pThis);

extern MAPCORE_API
BOOL8 guideList_setDetailFlag(TGuideList *pThis,
                              TGuidanceDetailFlag guidanceDetailFlag,
                              BOOL8 value);

extern MAPCORE_API
BOOL8 guideList_getDetailFlag(const TGuideList *pThis,
                              TGuidanceDetailFlag guidanceDetailFlag);

extern MAPCORE_API
void guideList_clearDetailFlags(TGuideList *pThis);


/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif
