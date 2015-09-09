#ifndef DIONEWAYBUFFER_H
#define DIONEWAYBUFFER_H

#include <innovEngine_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BEGIN HACK */
struct TViewport;
/* END HACK */

// maximum number of one way arrows that we can display on one page
#define MAX_ONE_WAYS 200

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

///
/// @addtogroup Map
/// @ingroup UnUsed
/// @{
///


/// @addtogroup Road
/// @ingroup Map
/// @{

/// @addtogroup OneWayArrow
/// @ingroup Road
/// @{

typedef struct TDisplayOneWayArrow
{
  TPos  pos;
  // < heading of the driving restriction in trigonometric unit (counterclockwise, 0=right)
  UINT8 heading;
  BOOL8 forwardsRestriction;
  BOOL8 backwardsRestriction;
} TDisplayOneWayArrow;

/// @}

/// @addtogroup BlockedRoad
/// @ingroup Road
/// @{

extern void Viewport_EnableBlockedRoadIcons(struct TViewport *viewport, BOOL8 enable);

extern BOOL8 Viewport_IsEnabledBlockedRoadIcons(struct TViewport *viewport);

/// @}

/// @addtogroup OneWayArrowBuffer
/// @ingroup Road
/// @{

typedef struct TOneWayArrowBuffer
{
  TDisplayOneWayArrow oneWays[MAX_ONE_WAYS];
  UINT32 oneWayCount;
} TOneWayArrowBuffer;

/// @}

/// @}

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif
