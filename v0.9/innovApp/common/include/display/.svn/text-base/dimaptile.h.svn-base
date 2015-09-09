#ifndef _DIMAPTILE_H_
#define _DIMAPTILE_H_

#include <innovEngine_types.h>
#include <math/gerect.h>
#include <display/diviewport.h>

#ifdef __cplusplus
extern "C" {
#endif

/// this callback allows the application to override the default tile drawing
/// routine (e.g. for drawing non-supported tile formats, like JPEG or PNG)
/// @param tileBitmap bitmap returned from the fetchTile function
/// @param viewport the viewport to be drawn to
/// @param rect map coordinates defining locations for the bitmap's corners
typedef void TDrawMapTileFunc(const TBitmap *tileBitmap,
                              TViewport *viewport,
                              const TRect *rect);

/// this callback allows the application to override the default tile fetching
/// routine (e.g. for fetching non-supported tile formats, like JPEG or PNG)
/// @param tileDirectory root string identifying location of tile bitmaps
/// @param tileString string identifying a specific tile (see note).
/// @return the tile bitmap, or NULL on failure
/// @note tileString is a sequence of q's, r's, s's and t's, indexing a quad
/// tree. Each character represents a particluar quadrant of the parent, as 
/// follows: q=top-left, r=top-right, s=bottom-right, t=bottom-left. The "root" 
/// tile covers the entire globe from "180W 90N" to "180E 90S". This encoding 
/// format was borrowed from google maps.
typedef const TBitmap *TFetchMapTileFunc(const char *tileRoot,
                                         const char *tileString);

/// Render raster map tiles
/// @param viewport the viewport to be drawn to
/// @param rect map coordinates defining locations for the root bitmap's corners
/// @param tileStringBuffer [working] buffer used to build tile names.
/// @param tileStringBufSize maximum size for tile names. For obvious reasons 
/// shouldn't be greater than ARRAYLENGTH(tileStringBuffer).
/// @param fetchTile callback allowing application to fetch tiles from a non-
/// standard data-source.
/// @param drawTile callback allowing application to draw tiles of un-supported
/// formats
extern MAPCORE_API
void maptile_render(TViewport *viewport,
                    const TRect *rect,
                    char *tileStringBuffer,
                    UINT32 tileStringBufSize,
                    TFetchMapTileFunc *fetchTile,
                    TDrawMapTileFunc *drawTile);

/// Set the root directory used for the default fetchTile implementation.
/// @param dir tile root directory
extern MAPCORE_API
void maptile_setTileDirectory(const char *dir);

/// Get the root directory set for the default fetchTile implementation.
/// @return tile root directory
extern MAPCORE_API
const char *maptile_getTileDirectory(void);

#ifdef __cplusplus
}
#endif

#endif // _DIMAPTILE_H_
