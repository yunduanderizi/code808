#ifndef DIVIEWPORTP_H
#define DIVIEWPORTP_H

#include <display/diviewport.h>

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void Viewport_GetViewpoint(const TViewport *pThis, TPos3d *p);
extern void Viewport_GetHeadingVector(const TViewport *pThis, TPos3d *p);
#define     Viewport_GetType(pThis) (pThis)->type
extern void   Viewport_RenderLine(TViewport *pThis, TPos *v1, TPos *v2);
extern const TPlaneIdx *Viewport_GetClipPlanesForRect(TViewport *pThis,
                                                      TRect *rect,
                                                      TPlaneIdx *clipPlanesBuffer);

extern TGeometricPen *Viewport_GetPen(const TViewport *pThis);
extern void Viewport_SetPen(TViewport *pThis, TGeometricPen *pen);
extern void Viewport_MoveTo(TViewport *pThis, TPos *pos);

extern BOOL8 Viewport_LineTo(TViewport *pThis,
                             TPos *pos,
                             VIEWPORT_RENDERLINEFLAGS flags);

extern TDisplayList *Viewport_GetDisplayList(TViewport *pThis);
extern struct TSettlementNameSet *Viewport_GetSettlementNameSet(TViewport *pThis);

//extern struct TPolyDisplayTheme *Viewport_GetPolyTheme(TViewport *pThis);
extern struct TOneWayArrowBuffer *Viewport_GetOneWayArrowBuffer(TViewport *pThis);
extern const TPlaneIdx *Viewport_GetClipPlanes(TViewport *pThis);
extern void Viewport_SetClipPlanes(TViewport *pThis, const TPlaneIdx *clipPlanes);

extern BOOL8 Viewport_GetClippedLine(TViewport *pThis,
                                     TPos *p1, TPos *p2,
                                     const TPos *v1, const TPos *v2);

extern BOOL8 Viewport_DrawPolygon(TViewport *pThis,
                                  TPol2d *polygon, BOOL8 assumeClipped);

extern void Viewport_GetPointDepthFactor(const TViewport *pThis,
                                         const TPos *pos,
                                         INT32 *depth, INT32 *depthFactor);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIVIEWPORTP_H */
