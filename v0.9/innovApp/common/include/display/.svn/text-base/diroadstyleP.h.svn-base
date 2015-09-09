#ifndef DIROADSTYLEP_H
#define DIROADSTYLEP_H

#include <display/diroadstyle.h>
#include <mapdata/enmapinfoP.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL8 RoadStyles_RenderLine(IRoadTheme *iface,
                            struct TViewport *viewport,
                            TPos *v1,
                            TPos *v2,
                            TRoadSegment *segment,
                            SEGMENT_RENDERFLAGS renderFlags);

BOOL8 RoadStyles_useShapePoints(IRoadTheme *iface,
                                TRoadSegment *segment,
                                struct TViewport *viewport);

void RoadStyles_GetSegmentPen(IRoadTheme *iface,
                              TRoadSegment *segment,
                              TGeometricPen *geoPen,
                              struct TViewport *viewport);


#ifdef __cplusplus
}
#endif

#endif /* DIROADSTYLEP_H */
