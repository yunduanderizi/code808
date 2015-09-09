#ifndef DILABELP_H
#define DILABELP_H

#include <display/dilabel.h>
#include <display/didisplayline.h>
#include <math/gerect.h>
#include <graphics/grbitmap.h>
#include <graphics/grcolour.h>
#include <language/utf16.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern BOOL8 label_init(void);

extern void Label_DrawAsMotorway(const UTF16* rLabel,
                                 INT32 x,
                                 INT32 y,
                                 struct TViewport *viewport,
                                 RGBCOLOUR textColour,
                                 RGBCOLOUR backColour,
                                 RGBCOLOUR borderColour,
                                 const struct TBitmap *bitmap);

extern void Label_DrawAsAdministrativeArea(const UTF16* rLabel,
                                           INT32 x,
                                           INT32 y,
                                           RGBCOLOUR textColour,
                                           RGBCOLOUR backColour,
                                           BOOL8 display,
                                           UINT32 level,
                                           TDisplayLine* returnValue,
                                           struct TViewport *viewport);

extern void Label_GetBoundingBox(const UTF16 *rLabel,
                                 struct TViewport *viewport,
                                 TRect *rectTextBounds,
                                 INT32 startX, INT32 startY,
                                 UINT8 placement,
                                 TDisplayLine *output);

extern void Label_Draw(const UTF16* rLabel,
                       struct TViewport *viewport,
                       INT32 startX, INT32 startY,
                       UINT8 placement,
                       RGBCOLOUR frameColour,
                       RGBCOLOUR frameShadowColour,
                       RGBCOLOUR anchorColour,
                       RGBCOLOUR fillColour,
                       RGBCOLOUR textColour,
                       RGBCOLOUR anchorBorderColour);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DILABELP_H */
