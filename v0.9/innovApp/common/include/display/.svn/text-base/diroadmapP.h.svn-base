#ifndef DIROADMAPP_H
#define DIROADMAPP_H

#include <display/diroadmap.h>
#include <display/disegment.h>
#include <mapdata/enroadP.h>
#include <language/utf16.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void RoadMap_Render(TGenericMapInfo *const mapInfo,
                           TRoadMapLayer          roadMapLayer,
                           TRoadNetworkType       network,
                           TRoadType              maxRoadType,
                           struct TViewport       *viewport,
                           SEGMENT_RENDERFLAGS    renderFlags,
                           UINT8                  pass);

extern void RoadMap_GetBoundingRect(const TGenericMapInfo *const mapInfo,
                                    TRect *rect);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIROADMAPP_H */
