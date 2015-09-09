#ifndef DIDISPLAYNAME_H
#define DIDISPLAYNAME_H

#include <innovEngine_types.h>
#include <display/diline.h>
#include <display/diroadnameposset.h>
#include <display/diviewporttransition.h>
#include <mapdata/enroad.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BEGIN HACK */
struct TViewport;
/* END HACK */

struct TRoadMapHeader;

/// @addtogroup Display
/// @{

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

/// @addtogroup RoadLabelling
/// @ingroup UnUsed
/// @{

///
/// @enum TLabelPlacementStatus
/// Can a label be positioned on the display?
///
typedef enum TLabelPlacementStatus
{
  /// The label is already placed.
  LABEL_ALREADY_PLACED,
  /// The label was successfully placed by this operation.
  LABEL_SUCCESSFULLY_PLACED,
  /// The label could not be placed by this operation.
  LABEL_CANNOTBE_PLACED
} TLabelPlacementStatus;

///
/// @typedef enum TLabelPlacementStatus TLabelPlacementStatus
/// Can a label be positioned on the display?
///

/// @addtogroup DisplayName
/// @ingroup RoadLabelling
/// @{

///
/// @typedef struct TRoadDisplayName
/// The reverse of this entire structure is used as the sort key for the
/// name placement (i.e. sorted as a little endian number).
///
typedef struct TRoadDisplayName
{
  // DO NOT CHANGE ORDER, ASSUMES LITTLE ENDIAN UINT8 ORDER
  // ELEMENTS ARE ORDERED FROM LOWEST TO HIGHEST PRIORITY
  const UTF16 *name;
  const struct TGenericMapInfo *mapInfo;
  TRect textBounds;
  UINT16 routeType;

  BITFIELD32 roadLength:16;          // 0xFFFF = shortest road, 0 = longest road (reverse logic for sorting purposes)
  BITFIELD32 motionFollowFactor:16;  // (2D only) priority of labels covering 'empty' screen space will be increased through this member.
                                     // See DisplayList_ReviewNotationOfDisplayName function.
  BITFIELD32 roadFormOfWay:4;        // 0 = most important roadFow, 7 = least important
  BITFIELD32 roadType:4;             // 0 = most important roadType, 7 = least important
  BITFIELD32 flagNotOnRoute:1;       // 1 = segment not on route, 0 = segment on route (reverse logic for sorting purposes)
  BITFIELD32 flagNotHighlighted:1;   // 1 = segment not highlighted, 0 = segment highlighted (reverse logic for sorting purposes)
  BITFIELD32 flagSlipRoad:1;         // 1 = segment is a slip road (don't want names on slip roads)
} TRoadDisplayName;

/// @}

/// @}

/// @}

/// @}

//=struct TViewport;

extern MAPCORE_API 
TLabelPlacementStatus DisplayName_Show(TRoadDisplayName *pThis,
                                       struct TViewport *viewport,
                                       TRoadNamePosSet *currentSet,
                                       TRoadNamePosSet *prevSet,
                                       ViewTransitionType transitionType);

extern MAPCORE_API
void DisplayName_ReviewNotation(TRoadDisplayName *roadDName,
                                struct TViewport *pThis,
                                TPos *translationVector);

#ifdef __cplusplus
}
#endif

#endif
