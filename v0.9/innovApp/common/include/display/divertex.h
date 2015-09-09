#ifndef DIVERTEX_H
#define DIVERTEX_H

#include <innovEngine_types.h>
#include <display/di3Dengi.h>
#include <graphics/grcolour.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup UnUsed
/// @ingroup Display



/// @addtogroup GeometricPrimitives
/// @ingroup UnUsed
/// @{

/// @addtogroup Vertex
/// @ingroup GeometricPrimitives
/// @{

///
/// Flags determining how to look for a median vertex.
///
typedef enum VERTEX_GETMEDIAN_SIDE
{
  /// Looking for the left side from the first vertex to the last one
  GETMEDIAN_LEFTSIDE,
  /// Looking for the right side from the first vertex to the last one
  GETMEDIAN_RIGHTSIDE
} VERTEX_GETMEDIAN_SIDE;

/// @}

/// @}




extern MAPCORE_API
BOOL8 Vertex_GoingLeft(const TPos *vecBack, const TPos *v2);

extern MAPCORE_API
BOOL8 Vertex_Equal(const TPos *pThis, const TPos *p);

///
/// Create a vertex object with the specified coordinate pair
///
#define      Vertex_Define(pThis, _x, _y) { (pThis)->x = _x; (pThis)->y = _y; }

///
/// Create a vertex object as a duplicate of the argument vertex
///
#define      Vertex_DefineCopy(pThis, v) { (pThis)->x = (v)->x; (pThis)->y = (v)->y; }

extern MAPCORE_API
void  Vertex_Render(const TPos *pThis, struct TViewport *viewport, UINT32 thickness, RGBCOLOUR colour);

extern MAPCORE_API
void  Vertex_NormalizeScale(TPos *pThis, INT32 scale);

extern MAPCORE_API
void  Vertex_Scale(TPos *pThis, INT32 scale);

extern MAPCORE_API
void  Vertex_Add(TPos *pThis, const TPos *v);

extern MAPCORE_API
void  Vertex_Translate(TPos *pThis, INT32 x, INT32 y);

extern MAPCORE_API
void  Vertex_Subtract(TPos *pThis, const TPos *v);

extern MAPCORE_API
void  Vertex_GetMedian(const TPos *pThis, const TPos *v0, const TPos *v2, INT32 thickness, VERTEX_GETMEDIAN_SIDE side, TPos *v);

extern MAPCORE_API
INT32 Vertex_DotProduct(const TPos *pThis, const TPos *v);

extern MAPCORE_API
BOOL8 Vertex_FormsAcuteAngle(const TPos *pThis, const TPos *vec);

extern MAPCORE_API
UINT32 Vertex_Length(const TPos *pThis);

extern MAPCORE_API
INT32 Vertex3D_DotProduct(TPos3d *pThis, TPos3d *v);

extern MAPCORE_API
void  Vertex3D_VectorProduct(TPos3d *pThis, TPos3d *v, TPos3d *vecProd);

extern MAPCORE_API
void  Vertex3D_Define(TPos3d *pThis, INT32 x, INT32 y, INT32 z);

extern MAPCORE_API
void  Vertex3D_Subtract(TPos3d *pThis, TPos3d *v);

extern MAPCORE_API
void  Vertex3D_NormalizeScale(TPos3d *pThis, INT32 scale);

#ifdef __cplusplus
}
#endif

#endif
