#ifndef DIRECT_H
#define DIRECT_H

#include <innovEngine_types.h>
#include <math/gerect.h>
#include <graphics/grcontext.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

extern MAPCORE_API
void Rect_Define(TRect *pThis, INT32 x, INT32 y, UINT32 size);

extern MAPCORE_API
void Rect_New(TRect *pThis, TPos *vertex);

extern MAPCORE_API
void Rect_Expand(TRect *pThis, TPos *vertex);

extern MAPCORE_API
void Rect_Render2DWithBorder(TRect *pThis, TGraphContext *graphContext, RGBCOLOUR colour, RGBCOLOUR outline);

extern MAPCORE_API
void Rect_GetCenter(TRect *pThis, TPos *center);

extern MAPCORE_API
void Rect_Render(TRect *pThis, struct TViewport *viewport, RGBCOLOUR colour);

extern MAPCORE_API
void Rect_UnionRect(TRect *pThis, TRect *rect);

extern MAPCORE_API
BOOL8 Rect_IsEmpty(TRect *pThis);

extern MAPCORE_API
void Rect_DefineEmpty(TRect *pThis);

extern MAPCORE_API
void Rect_Copy(TRect *pThis, TRect *rect);


#ifdef __cplusplus
}
#endif

#endif
