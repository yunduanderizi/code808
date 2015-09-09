#ifndef DIPOIMAPP_H
#define DIPOIMAPP_H

#include <display/dipoimap.h>
#include <display/diiconranklist.h>
#include <mapdata/enpoiP.h>
#include <math/gerect.h>
#include <innovEngine_types.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern TBitmap *GetGlobalBitmapWithBuffer(const char *iconName, int iStyle, UINT8* pBuff);

extern TBitmap *POIMap_FindBitmap(const TGenericMapInfo *const mapInfo,
                                  const char *name);

//extern TBitmap *POIMap_FindBrandBitmap(const char *name);

extern void POIMap_Render(TGenericMapInfo *const mapInfo,
                          struct TViewport *viewport,
                          TPOIMapLayer layer,
                          TIconRankSet *rankSet);

extern void POIMap_RenderSettlements(TGenericMapInfo *const mapInfo,
                                     TPOIMapLayer layer,
                                     struct TViewport *viewport);

extern void POIMap_GetBoundingRect(const TGenericMapInfo *const mapInfo,
                                   TRect *rect);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIPOIMAPP_H */
