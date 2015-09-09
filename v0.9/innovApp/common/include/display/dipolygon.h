#ifndef DIPOLYGON_H
#define DIPOLYGON_H

#include <display/di3Dengi.h>
#include <graphics/grcolour.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

#define POLYGON_MAX_POINTS 500

extern MAPCORE_API
void  Polygon_Render(TPol2d *pThis, struct TViewport *viewport, RGBCOLOUR colour);

extern MAPCORE_API
void  Polygon3D_Render(TPol3d *pThis, struct TViewport *viewport, const TPlaneIdx *clipPlanes);

#ifdef __cplusplus
}
#endif

#endif
