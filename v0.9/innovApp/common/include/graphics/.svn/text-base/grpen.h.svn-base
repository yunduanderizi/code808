#ifndef GRPEN_H
#define GRPEN_H

#include <graphics/grbitmap.h>
#include <graphics/grcolour.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Pen
/// @ingroup Graphics
/// @{

/** @typedef struct TPen */
/** Define the attributes of a drawing pen. */
typedef struct TPen
{
  /** The thickness of the pen in pixels. */
  UINT8 thickness;
  /** The colour of the pen.
      @todo Change that to graphcontext-dependent RGBCOLOUR colour space. */
  RGBCOLOUR colour;
  /** The dash style of the pen. */
  UINT32 dashStyle;
  /** The pattern of the pen, if any. Overrides the colour field. */
  struct TBitmap *pattern;
  /** The opacity of the pen.
      @todo Change to UINT8 and get rid of this bogus 256 value. */
  UINT16 alpha;
  /** Specifies whether the pen is antialiased. */
  BOOL8 antialiased;
} TPen;

extern MAPCORE_API
void Pen_New( TPen *pThis );

/// @}

#ifdef __cplusplus
}
#endif

#endif
