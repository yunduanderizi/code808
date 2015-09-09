#ifndef DIPOLYGONP_H
#define DIPOLYGONP_H

#include <display/dipolygon.h>
#include <display/di3Dengi.h>
#include <graphics/grcolour.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void  Polygon_Init(TPol2d *pThis, TPos *pos);
extern void  Polygon_InitExisting(TPol2d *pThis, TPos *pos, UINT32 count);
extern void  Polygon_Copy(TPol2d *pThis, TPol2d *p);
extern void  Polygon_AddVertex(TPol2d *pThis, TPos *vertex);
extern void  Polygon_UnionAddVertex(TPol2d *pThis, TPos *vertex);
extern void  Polygon_InsertVertex(TPol2d *pThis, INT32 index, TPos *vertex);
extern void  Polygon_UnionInsertVertex(TPol2d *pThis, INT32 index, TPos *vertex);
extern BOOL8 Polygon_DuplicateLastVertex(TPol2d *pThis);
extern void  Polygon_GetBoundingBox(TPol2d *pThis, TRect *rect);
extern void  Polygon_TranslateVertices(TPol2d *pThis, INT32 x, INT32 y);
extern void  Polygon_ScaleVertices(TPol2d *pThis, INT32 numerator, INT32 denominator);
extern void  Polygon_ScaleVerticesXY(TPol2d *pThis,
                                     INT32 numeratorX, INT32 denominatorX,
                                     INT32 numeratorY, INT32 denominatorY);
extern void  Polygon_RenderColouredPolyline(TPol2d *pThis, struct TViewport *viewport);
extern void  Polygon_RenderVolumic(TPol2d *pThis,
                                   struct TViewport *viewport,
                                   RGBCOLOUR fillColour,
                                   RGBCOLOUR borderColour,
                                   UINT32 height,
								   UINT32 floatheight);
extern UINT32 Polygon_RemoveDoublonVertices(TPol2d *pThis);
extern void Polygon_ArrowFromPolyline(TPol2d *pThis, INT32 thickness);

extern BOOL8 Polygon3D_IsFacingViewport(TPol3d *pThis, struct TViewport *viewport);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIPOLYGONP_H */
