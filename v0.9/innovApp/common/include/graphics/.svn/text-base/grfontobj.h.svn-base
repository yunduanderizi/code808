#ifndef GRFONTOBJECT_H
#define GRFONTOBJECT_H

#include <math/gerect.h>
#include <graphics/grbitmap.h>
#include <language/lacodepage.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Font
/// @ingroup Graphics
/// @{

/// A bitmap font glyph descriptor.
typedef struct TFontGlyph {
  /** @internal Bounding box */
  INT16 boxX, boxY;
  /** @internal Origin of glyph (maps to point where cursor is) */
  INT16 orgX, orgY;
  /** @internal Cursor increment vector */
  INT16 incX, incY;
  /** @internal Pointer to MONOCHROME glyph bitmap */
  TBitmap *bitmap;
} TFontGlyph;

/// A bitmap font structure (i.e. for a whole font).
typedef struct TFont {
  /** @internal Array of glyphs, indexed by ASCII code. */
  const TFontGlyph *glyph[256];
  /** @internal Font family name */
  char name[256];
  /** @internal Font point size */
  UINT32 size;
  /** @internal TRUE if font is bold */
  BOOL8 isBold;
  /** @internal TRUE if font is italic */
  BOOL8 isItalic;
  /** @internal Pixels above baseline (added to descent 
      to give text_Height) */
  INT16 ascent;
  /** @internal Pixels below baseline */
  INT16 descent;
} TFont;

extern MAPCORE_API
UINT16 Font_GetHeight(const TFont *pThis);

extern MAPCORE_API
UINT16 Font_GetTextWidth(const TFont *pThis, const TLatin1Char *text);

extern MAPCORE_API
void Font_GetTextBounds(const TFont *pThis, const TLatin1Char *s, TRect *r);

/// @}

#ifdef __cplusplus
}
#endif

#endif
