#ifndef DIFAVOUR_H
#define DIFAVOUR_H

#include <mapdata/enpoi.h>
#include <math/gerect.h>
#include <graphics/grbitmap.h>
#include <innovEngine_types.h>
#include <language/lacodepage.h>
#include <display/diiconranklist.h>

struct TViewport;
struct TDisplayFavourite;
struct TIconDisplayRank;

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Display
/// @{

///@addtogroup UnUsed
/// @ingroup Display
/// @{

///@addtogroup Favourite
/// @ingroup UnUsed
/// @{
///@TPE
#define FAVORITE_MAX_SIZE 17000000

/**
 * The type of functions that are called to obtain the bitmap for an
 * icon that is about to be rendered.
 * 
 * Bitmap pointers returned from this callback should remain valid at least 
 * until the next time the callback is called. The callback 
 */
typedef const TBitmap *TFavGetBitmapFunc(UINT32 favouriteId);

///
/// Represents a favourite.
/// All fields in this structure are internal to MapCore
/// and should not be used. Please use Favourite object methods.
///
typedef struct TDisplayFavourite
{
  /// The bitmap used to render the favourite
  const TBitmap *bitmap;
  /// The name of the favourite. This member is not currently used.
  UTF16 name[POI_MAX_NAME_SIZE];
  /// The map position of the favourite
  TPos pos;
  /// callback function used to fetch the bitmap for drawing
  TFavGetBitmapFunc *getBitmapFunc;
  /// rect defining position and centring of the bitmap
  TRect bitmapRect;
  /// index for the application to distinuish one favourite from another
  UINT32 favouriteId;
  /// importance with respect to other favourites in this frame (0 = most important)
  UINT8 importance;
} TDisplayFavourite;

/// @}
/// @}
/// @}

extern MAPCORE_API
void Favourite_Define(TDisplayFavourite *pThis, TBitmap *bitmap, TPos *pos);

extern MAPCORE_API
BOOL8 Favourite_Render(TDisplayFavourite *pThis, struct TViewport *viewport);

extern MAPCORE_API
BOOL8 Favourite_RenderRect(TDisplayFavourite *pThis, struct TViewport *viewport, TRect *screenRect);

extern MAPCORE_API
BOOL8 Favourite_RenderEx(TDisplayFavourite *pThis, struct TViewport *viewport, TIconType type);

extern MAPCORE_API
BOOL8 Favourite_DefineEx(TDisplayFavourite *pThis,
                         const TBitmap *bitmap,
                         const TPos *pos,
                         const UTF16 *name,
                         TFavGetBitmapFunc *getBitmapFunc,
                         const TRect *bitmapRect,
                         UINT32 favouriteId,
                         UINT8 importance);

extern MAPCORE_API
UINT32 IconDisplayRank_getFavouriteId(const struct TIconDisplayRank *pThis);

#ifdef __cplusplus
}
#endif

#endif
