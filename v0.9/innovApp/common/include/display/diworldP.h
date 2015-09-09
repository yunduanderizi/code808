#ifndef DIWORLDP_H
#define DIWORLDP_H

#include <display/diworld.h>
#include <display/dicolour.h>
#include <display/diiconranklistP.h>
#include <mapdata/enpoiP.h>
#include <routing/rtjourney.h>
#include <language/utf16.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern TPOIMapLayer World_CalcPOIMapLayer(UINT32 mapSize, INT32 detailLevel);

extern void World_RenderArcs(struct TViewport *viewport);

extern void World_RenderRoads(struct TViewport *viewport,BOOL8 renderlabels);

extern void World_RenderPolys( WORLD_RENDERFLAGS renderFlags, 
                               struct TViewport *viewport);

extern void World_RenderSettlements(struct TViewport *viewport);

extern TGenericMapInfo *World_RoadMapFromPoint(const TPos *position);

extern void World_RenderPrepareIcons(struct TViewport *viewport, const TJourney *journey);

extern void World_RenderIcons(struct TViewport *viewport, TIconType iconType);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIWORLDP_H */
