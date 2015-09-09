#ifndef DICARICON_H
#define DICARICON_H

#include <graphics/grbitmap.h>
#include <innovEngine_types.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum size in X and Y of a car icon */
#define MAX_CAR_ICON_SIZE 128

///
/// @addtogroup Display
/// @{
///

///
/// @addtogroup CarIcon
/// @ingroup Display
/// @{
///

/** @typedef struct TCarIcon TCarIcon */
/** Car icon position, heading and imagery. */
typedef struct TCarIcon
{
  /** @internal The car icon bitmap. */
  const TBitmap *carIconBitmapPtr;
  /** @internal The lat/long position of the car. */
  TPos           carIconPos_w;
  /** @internal The heading (clockwise, 0 is north) of the car. */
  UINT8          carIconHeading;
  /** @internal The height of the car icon above the map in 3D */
  UINT32         shadowHeight;
} TCarIcon;

///
/// @}
///

///
/// @}
///

extern MAPCORE_API
void CarIcon_Create(TCarIcon *pThis, TPos *position, UINT8 heading, TBitmap *bitmap);

extern MAPCORE_API
void CarIcon_Render(TCarIcon *pThis, struct TViewport *viewport);

extern MAPCORE_API
void CarIcon_SetHeading(TCarIcon *pThis, UINT8 angle);

extern MAPCORE_API
void CarIcon_SetPosition(TCarIcon *pThis, INT32 x_w, INT32 y_w);

extern MAPCORE_API
BOOL8 CarIcon_SetBitmap(TCarIcon *pThis, const TBitmap *bm);

extern MAPCORE_API
void CarIcon_SetShadowHeight(TCarIcon *pThis, UINT32 height);

#ifdef __cplusplus
}
#endif

#endif
