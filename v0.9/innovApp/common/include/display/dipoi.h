#ifndef DIPOI_H
#define DIPOI_H

#include <innovEngine_types.h>
#include <mapdata/enpoi.h>

#ifdef __cplusplus
extern "C" {
#endif

struct TViewport;

/// Set the scale factor used when blitting POIs
/// @param pThis the viewport object
/// @param scaleFactor 25.7 fixed point scale factor (e.g. 128 means original scale)
extern MAPCORE_API
void Viewport_SetPoiScale(struct TViewport *pThis, UINT32 scaleFactor);

/// Query the scale factor used when blitting POIs
/// @param pThis the viewport object
/// @return 25.7 fixed point scale factor (e.g. 128 means original scale)
extern MAPCORE_API
UINT32 Viewport_GetPoiScale(struct TViewport *pThis);

/// Set the transparency of POI icons
/// @param pThis the viewport object
/// @param alpha transparency value - 0 = fully opaque, 255 = fully transparent
extern MAPCORE_API
void Viewport_SetPoiTransparency(struct TViewport *pThis, UINT8 alpha);
///@TPE

extern MAPCORE_API
void Viewport_SetExploreFilter(struct TViewport *viewPort, BOOL8 flag, const UTF16 *keyword, UINT16 typeID);

/// Query the transparency of POI icons
/// @param pThis the viewport object
/// @return alpha transparency value - 0 = fully opaque, 255 = fully transparent
extern MAPCORE_API
UINT8 Viewport_GetPoiTransparency(struct TViewport *pThis);
///@TPE

extern MAPCORE_API
void Viewport_SetExploreFilter(struct TViewport *viewPort, BOOL8 flag, const UTF16 *keyword, UINT16 typeID);


// Deprecated old API. TODO: remove once AppCore starts using the new API

DEPRECATED(extern MAPCORE_API
void POI_SetDisplayScale(UINT32 scaleFactor));

DEPRECATED(extern MAPCORE_API
UINT32 POI_GetDisplayScale(void));

DEPRECATED(extern MAPCORE_API
void POI_SetDisplayOpacity(UINT16 alpha));

DEPRECATED(extern MAPCORE_API
UINT16 POI_GetDisplayOpacity(void));

#ifdef __cplusplus
}
#endif

#endif
