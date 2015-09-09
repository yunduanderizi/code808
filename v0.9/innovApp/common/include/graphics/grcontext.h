#ifndef GRCONTEXT_H
#define GRCONTEXT_H

#include <innovEngine_types.h>
#include <math/gerect.h>
#include <graphics/grbitmap.h>
#include <graphics/grpen.h>
#include <graphics/grfontobj.h>
#include <graphics/grcolour.h>
#include <graphics/grmatrix.h>
#include <graphics/grplatform.h>
#include <display/dipolstyle.h>
#include <language/lacodepage.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Basic
/// @ingroup Graphics
/// @{

/// Represents a scanline for the rasteriser
typedef struct TGraphExtents
{
  INT16 min, max;
} TGraphExtents;

/// Represents a graphics context.
/// A graphics context provides methods to draw on a rasterisation surface,
/// typically a TBitmap object.
/// All TGraphContext structure members are private and should not be accessed
/// directly.
typedef struct TGraphContext {
  /** pointer to frame buffer memory */
  UINT8 *screen;
  /** buffer scanline width */
  UINT32 width;
  /** buffer height */
  UINT32 height;
  /** depth (number of bits per pixels) */
  UINT8 depth;
  /** region to draw into from xmin through xmax and from ymin through ymax
      pixels. */
  TRect clipRect;
  /** pointer to currently selected font in this context */
  const struct TFont *font;
  
  TPen currentPen;
  
  TGraphExtents *extents;
  
  TPlatformGraphics *graphics;
  
} TGraphContext;

/** The polygon drawing routine uses coordinates in the X direction
    that have a certain fixed point accuracy.  This constant defines 
    the number of shifts required to scale integer values to their 
    equivalent fixed point representation. */
#define POLYFILL_X_RESOLUTION 9

/** Number of bits shifts to be applied to texel coordinates u, v */
#define TEXEL_RES 12
  
/** Transparent texture attribute (to be used in 'kind' field of TTexture) */
#define TEXTURE_TRANSPARENT 1

/** Tiled texture attribute (to be used in 'kind' field of TTexture) */
#define TEXTURE_TILED       2

/** @typedef struct TTexture
    Texture bitmap (U, V sizes can only be power of 2) */
typedef struct TTexture {
  /** kind of texture: or'ed value of
      TEXTURE_TRANSPARENT, TEXTURE_TILED */
  UINT32 kind;
  /** size of texture in U, V coordinates.
      size has to be a power of 2 when
      using tiled textures */
  UINT16  sizeU, sizeV; 
  /** texture bitmap raw data */
  UINT8 *bitmapData;   
  /** depth of the texture bitmap (bits per pixel) */
  UINT8  depth;
} TTexture;

// Should this be public???
void GraphContext_Define(TGraphContext *pThis,
                         UINT8 *frameBuffer,
                         UINT8 depth,
                         UINT32 width, UINT32 height,
                         TGraphExtents *extents);

extern MAPCORE_API
void GraphContext_Create(TGraphContext *pThis,
                         const TBitmap *bitmap,
                         TGraphExtents *extents);

extern MAPCORE_API
void GraphContext_Destroy(TGraphContext *pThis);

extern MAPCORE_API
const TFont *GraphContext_SetFont(TGraphContext *pThis,
                                  const TFont *font);

extern MAPCORE_API
const TFont* GraphContext_GetFont(const TGraphContext *pThis);

extern MAPCORE_API
void GraphContext_DrawText(TGraphContext *pThis,
                           const TFont *font,
                           const UTF16 *s,
                           const TPos *pos,
                           RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_Fill(TGraphContext *pThis, RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_GetClipRect(TGraphContext *pThis, TRect *r);

extern MAPCORE_API
BOOL8 GraphContext_ClipRect(TGraphContext *pThis, TRect *ov, const TRect *r1);

extern MAPCORE_API
void GraphContext_SetClipRect(TGraphContext *pThis, const TRect *r);

extern MAPCORE_API
void GraphContext_FillRect(TGraphContext *pThis, const TRect *r, RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_BlitRect(TGraphContext *pThis, const TRect *r, const TBitmap *bitmap);

extern MAPCORE_API
void GraphContext_BlitAlpha(TGraphContext *pThis,
                            const TBitmap *bitmap,
                            INT32 x, INT32 y,
                            UINT16 alpha);

extern MAPCORE_API
void GraphContext_AffineBlit(TGraphContext *pThis,
                             const TBitmap *bm,
                             const TMatrix2D *matrix,
                             const TRect *window);

extern MAPCORE_API
void GraphContext_RotoBlitBlend(TGraphContext *pThis,
                                const TBitmap *bm,
                                TPos srcXY, TPos destXY,
                                UINT8 angle, BOOL8 blendWithBackground);

extern MAPCORE_API
void GraphContext_ScaleRotoBlit(TGraphContext *pThis,
                                const TBitmap *bm,
                                const TPos *srcXY,
                                const TPos *destXY,
                                const TPos *scaleXY,
                                UINT8 angle);

extern MAPCORE_API
void GraphContext_DrawScaledBitmap(TGraphContext *pThis,
                                   const TBitmap *bm,
                                   INT32 x, INT32 y,
                                   UINT32 scaleX, UINT32 scaleY,
                                   UINT16 alpha);

extern MAPCORE_API
void GraphContext_MonoBlit(TGraphContext *pThis,
                           const TRect *rect, const UINT8 *data, UINT16 dataWidth,
                           BOOL8 transparent, RGBCOLOUR *rgbColours);

extern MAPCORE_API
void GraphContext_MonoBlitAlpha(TGraphContext *pThis,
                                const TRect *rect, const UINT8 *data,
                                UINT16 dataWidth, BOOL8 transparent, RGBCOLOUR *rgbColours);

extern MAPCORE_API
void GraphContext_MonoBlitSub(TGraphContext *pThis,
                              const TRect *rect, const UINT8 *data,
                              UINT16 dataWidth, BOOL8 transparent, RGBCOLOUR *rgbColours);

extern MAPCORE_API
void GraphContext_MonoBlitSubAlpha(TGraphContext *pThis,
                                   const TRect *rect,
                                   const UINT8 *data, UINT16 dataWidth,
                                   BOOL8 transparent, RGBCOLOUR *rgbColours);

extern MAPCORE_API
void GraphContext_MoveRect(TGraphContext *pThis,
                           const TRect *rect,
                           INT32 x, INT32 y);

extern MAPCORE_API
void GraphContext_DrawCircle(TGraphContext *pThis,
                             INT32 cx, INT32 cy, UINT16 r,
                             RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawFilledCircle(TGraphContext *pThis,
                                   INT32 cx, INT32 cy, UINT16 r,
                                   RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawHLine(TGraphContext *pThis,
                            INT32 xmin, INT32 xmax,
                            INT32 y, RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawVLine(TGraphContext *pThis,
                            INT32 x,
                            INT32 ymin, INT32 ymax,
                            RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawEllipse(TGraphContext *pThis,
                              INT32 x1, INT32 y1,
                              UINT16 tx, UINT16 ty,
                              RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawLineThinDirectAlphaDashed(TGraphContext *pThis,
                                                INT32 x1, INT32 y1,
                                                INT32 x2, INT32 y2,
                                                RGBCOLOUR colour,
                                                UINT16 alpha,
                                                UINT32 *pattern);

extern MAPCORE_API
void GraphContext_DrawLineThinDirectAlpha(TGraphContext *pThis,
                                          INT32 x1, INT32 y1,
                                          INT32 x2, INT32 y2,
                                          RGBCOLOUR rgbColour, UINT16 alpha);

extern MAPCORE_API
void GraphContext_DrawLineThinDirect(TGraphContext *pThis,
                                     INT32 x1, INT32 y1,
                                     INT32 x2, INT32 y2,
                                     RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawLineDirect(TGraphContext *pThis,
                                 INT32 x1, INT32 y1,
                                 INT32 x2, INT32 y2,
                                 UINT16 t, RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawLineDirectAA(TGraphContext *pThis,
                                   INT32 x1, INT32 y1,
                                   INT32 x2, INT32 y2,
                                   UINT16 t, RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawLineDirectPattern(TGraphContext *pThis,
                                        INT32 x1, INT32 y1,
                                        INT32 x2, INT32 y2,
                                        UINT16 t, const TBitmap *bitmap);

extern MAPCORE_API
void GraphContext_DrawLineDirectPatternAA(TGraphContext *pThis,
                                          INT32 x1, INT32 y1,
                                          INT32 x2, INT32 y2,
                                          UINT16 t, const TBitmap *bitmap);

extern MAPCORE_API
BOOL8 GraphContext_DrawLine(TGraphContext *pThis,
                            INT32 x1, INT32 y1, INT32 x2, INT32 y2,
                            UINT16 t, RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawLineThinDirectAA(TGraphContext *pThis,
                                       INT32 x1, INT32 y1,
                                       INT32 x2, INT32 y2,
                                       RGBCOLOUR rgbColour);

extern MAPCORE_API
BOOL8 GraphContext_DrawLineAA(TGraphContext *pThis,
                              INT32 x1, INT32 y1,
                              INT32 x2, INT32 y2,
                              UINT16 t,
                              RGBCOLOUR rgbColour);

extern MAPCORE_API
BOOL8 GraphContext_DrawLineThinAlpha(TGraphContext *pThis,
                                     INT32 x1, INT32 y1,
                                     INT32 x2, INT32 y2,
                                     RGBCOLOUR rgbColour, UINT16 alpha);

extern MAPCORE_API
BOOL8 GraphContext_DrawLineThinAlphaDashed(TGraphContext *pThis,
                                           INT32 x1, INT32 y1,
                                           INT32 x2, INT32 y2,
                                           RGBCOLOUR rgbColour,
                                           UINT16 alpha, UINT32 *pattern);

extern MAPCORE_API
BOOL8 GraphContext_ClipLine(TGraphContext *pThis,
                            INT32 x1, INT32 y1,
                            INT32 x2, INT32 y2,
                            UINT32 radius,
                            INT32 *clipped);

extern MAPCORE_API
BOOL8 GraphContext_PenDrawLine(TGraphContext *pThis,
                               TPos *v1,
                               TPos *v2,
                               BOOL8 assumeClipped,
                               TPen *pen);

extern MAPCORE_API
void GraphContext_CurrentPenDrawLine(TGraphContext *pThis,
                              TPos *v1,
                              TPos *v2,
                              BOOL8 assumeClipped);

extern MAPCORE_API
void GraphContext_FillRectBlend(TGraphContext *pThis,
                                const TRect *r, RGBCOLOUR vertex[4], BOOL8 accurate);

extern MAPCORE_API
void GraphContext_FillRectAlpha(TGraphContext *pThis,
                                const TRect *rect,
                                RGBCOLOUR rgbColour, UINT8 alpha);

extern MAPCORE_API
void GraphContext_DrawPolyConvexFillAA(TGraphContext *pThis,
                                       INT16 n, const TPos v[],
                                       RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawPolyConvexFill(TGraphContext *pThis,
                                     INT16 n, const TPos v[],
                                     RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawPolyConvexPattern(TGraphContext *pThis,
                                        INT16 n, const TPos v[],
                                        const TBitmap *bm);

extern MAPCORE_API
void GraphContext_DrawPolyPattern(TGraphContext *pThis,
                                  INT16 nvert, const TPos *point,
                                  INT16 ndonuts, const INT16 *donutList,
                                  const TBitmap *bm);

extern MAPCORE_API
void GraphContext_DrawPolyConvexStyle(TGraphContext *pThis,
                                        INT16 n, const TPos v[],
                                        const TPolStyle *style);

extern MAPCORE_API
void GraphContext_DrawPolyStyle(TGraphContext *pThis,
                                  INT16 nvert, const TPos *point,
                                  INT16 ndonuts, const INT16 *donutList,
                                  const TPolStyle *style);

extern MAPCORE_API
void GraphContext_DrawPolyConvexTextured(TGraphContext *pThis,
                                         INT16 n,
                                         const TPos v[],
                                         const TPos t[],
                                         const TTexture *tex);

extern MAPCORE_API
void GraphContext_DrawPolyfill(TGraphContext *pThis,
                               INT16 nvert, const TPos *point,
                               INT16 ndonuts, const INT16 *donutList,
                               RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawPolyfillAA(TGraphContext *pThis,
                                 INT16 nvert, const TPos *point,
                                 INT16 ndonuts, const INT16 *donutList,
                                 RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawPolyLine(TGraphContext *pThis,
                               INT16 nvert, const TPos *point,
                               RGBCOLOUR rgbColour, INT16 thickness);

extern MAPCORE_API
void GraphContext_DrawPolyLineAA(TGraphContext *pThis,
                                 INT16 nvert, const TPos *point,
                                 RGBCOLOUR rgbColour, INT16 thickness);

/** Find the centroid of a polygon. That is, the center of mass, assuming
    the density is uniform.

    @param x A pointer to the x values of the polygon
    @param y A pointer to the y values of the polygon
    @param n The number of points in the polygon
    @param xCentroid A pointer to the calculated x value of the centroid
    @param yCentroid A pointer to the calculated y value of the centroid
    @param area A pointer to the calculated area of the polygon

    @return Return values are 0 for normal execution , 1 for degenerate 
    polygons and 2 if area == 0 (centroid undefined) */
extern MAPCORE_API
UINT8 graph_polyCentroid(INT32 *x, INT32 *y, UINT16 n, INT32 *xCentroid, INT32 *yCentroid, INT32 *area);

extern MAPCORE_API
void GraphContext_SetPixel(TGraphContext *pThis,
                           INT32 x, INT32 y,
                           RGBCOLOUR colour);

extern MAPCORE_API
UINT32 GraphContext_GetTextHeight(TGraphContext *pThis);

extern MAPCORE_API
UINT32 GraphContext_GetTextWidth(TGraphContext *pThis,
                                 const UTF16 *text);

extern MAPCORE_API
void GraphContext_GetTextBounds(TGraphContext *pThis,
                                const UTF16 *text,
                                TRect *rect);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif
