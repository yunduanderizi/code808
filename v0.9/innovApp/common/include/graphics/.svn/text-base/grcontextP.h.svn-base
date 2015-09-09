#ifndef GRCONTEXTP_H
#define GRCONTEXTP_H

#include <graphics/grcontext.h>
#include <graphics/grcolour.h>
#include <graphics/grfontobj.h>
#include <graphics/grfontobj.h>
#include <innovEngine_types.h>

#ifdef __cplusplus
extern "C" {
#endif

  /// @internal
  /// This macro is used internally to calculate the screen address that
  /// corresponds to a particular pixel
  /// @note Was screenAddress(x, y) in MapCore 1.1.7.
  ///
#define GraphContext_GetScreenAddress(pThis, x, y)          \
  (&pThis->screen[((x) - ((y) * (UINT32)pThis->width))*   \
  (pThis->depth >> 3)                    \
  ])

  /// @internal
  ///
  /// Same as above but slightly more efficient when we know that
  /// context depth is 8
  /// @note Was screenAddress8(x, y) in MapCore 1.1.7.
  ///
#define GraphContext_GetScreenAddress8(pThis, x, y) \
  (&pThis->screen[((x) - ((y) * (UINT32)pThis->width))])

  /// @internal
  ///
  /// Same as above but slightly more efficient when we know that
  /// context depth is 16
  /// @note Was screenAddress16(x, y) in MapCore 1.1.7.
  ///
#define GraphContext_GetScreenAddress16(pThis, x, y) \
  (&(((UINT16 *)pThis->screen)[((x) - ((y) * (UINT32)pThis->width))]))

BOOL8 GraphContext_UnsafeClipLine(TGraphContext *pThis, TPos *v1, TPos *v2, UINT32 radius);

extern MAPCORE_API
UINT32 GraphContext_GetTextWidthL(TGraphContext *pThis,
                                  const TLatin1Char *text);

extern MAPCORE_API
void GraphContext_GetTextBoundsL(TGraphContext *pThis,
                                 const TLatin1Char *text,
                                 TRect *rect);

extern MAPCORE_API
void GraphContext_DrawTextL(TGraphContext *pThis,
                            const TFont *font,
                            const TLatin1Char *text,
                            const TPos *pos,
                            RGBCOLOUR rgbColour);

extern MAPCORE_API
void GraphContext_DrawAngledTextL(TGraphContext *pThis,
                                  const TFont *font,
                                  const TLatin1Char *s,
                                  const TPos *pos,
                                  RGBCOLOUR rgbColour,
                                  UINT8 angle);

#ifdef __cplusplus
}
#endif

#endif /* GRCONTEXTP_H */
