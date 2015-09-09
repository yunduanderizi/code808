#ifndef DIROADNAMEPOS_H
#define DIROADNAMEPOS_H

#include <innovEngine_types.h>
#include <math/gerect.h>
#include <mapdata/enroad.h>
#include <display/didisplayline.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BEGIN HACK */
struct TViewport;
/* END HACK */

struct TRoadMapHeader;

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

/// @addtogroup RoadLabelling
/// @ingroup UnUsed
/// @{

/// @addtogroup RoadNamePos
/// @ingroup RoadLabelling
/// @{

//
// The TRoadNamePos is for retaining position information between iterations
//
typedef struct TRoadNamePos
{
  const UTF16 *name;
  TPos pos;           // map coordinates (lat/long)
  UINT8 placement;    // topleft, bottomleft, topright, bottomright
  TRoadType roadType;
  UINT16 routeType;
  TRect *pTextBounds;
  const struct TGenericMapInfo *mapInfo;
} TRoadNamePos;

/// @}

/// @}

/// @}



//=struct TViewport;

extern MAPCORE_API
void RoadNamePos_GetBoundingBox(TRoadNamePos *pThis, struct TViewport *viewport, TDisplayLine *bbox);

extern MAPCORE_API
void RoadNamePos_Show(const TRoadNamePos *pThis, struct TViewport *viewport, BOOL8 showInverted);

extern MAPCORE_API
void RoadNamePos_Init(TRoadNamePos *pThis);

#define RoadNamePos_SetName(pThis, name, textBounds) (pThis)->name = name; (pThis)->pTextBounds = textBounds
#define RoadNamePos_SetPlacement(pThis, place) (pThis)->placement = place
#define RoadNamePos_SetMapInfo(pThis, mapInfo) (pThis)->mapInfo = mapInfo

#ifdef __cplusplus
}
#endif

#endif
