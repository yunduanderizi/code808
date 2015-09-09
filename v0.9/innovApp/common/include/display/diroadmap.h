#ifndef DIROADMAP_H
#define DIROADMAP_H

#include <platform_types.h>
#include <math/gerect.h>
#include <graphics/grbitmap.h>

#ifdef __cplusplus
extern "C" {
#endif

extern MAPCORE_API
const char *RoadMap_getGlobalShieldBitmapFile(void);

extern MAPCORE_API
void RoadMap_setGlobalShieldBitmapFile(const char *filename);

extern MAPCORE_API
const TBitmap *RoadMap_FindShieldBitmap(const char *name);

extern MAPCORE_API
BOOL8 RoadMap_GetRoadBoundingRect(TRect *rect);

#ifdef __cplusplus
}
#endif

#endif
