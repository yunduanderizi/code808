#ifndef DIFONTS_H
#define DIFONTS_H

#include <graphics/grfontobj.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Display
/// @{

/// @addtogroup FontPalette
/// @ingroup Display
/// @{

///
/// Identifies a purpose to bind to a font in the lookup table
///
typedef enum TDisplayFontIndex
{
  /// Used for 2D road labelling
  DISPLAY_FONT_ROAD_LABEL,
  /// Used for 2D motorway labelling
  DISPLAY_FONT_MOTORWAY_LABEL,

  /// Used for administrative area importance 0 labelling
  DISPLAY_FONT_AA_LABEL_IMP_0,
  /// Used for administrative area importance 1 labelling
  DISPLAY_FONT_AA_LABEL_IMP_1,
  /// Used for administrative area importance 2 labelling
  DISPLAY_FONT_AA_LABEL_IMP_2,
  /// Used for administrative area importance 3 labelling
  DISPLAY_FONT_AA_LABEL_IMP_3,
  /// Used for other administrative areas labelling
  DISPLAY_FONT_AA_LABEL,       /* importance other than previously listed */
  /// Used to render FPS (frame per sec) */
  DISPLAY_FONT_FPS,

  /// Family of fonts used by 3D display (3d 
  /// display will actually use several
  /// available font with the same
  /// characteristics (name, bold & italic
  /// attributes) but with different sizes
  /// depending on depth
  DISPLAY_FONT_3D_LABEL,

  /// Used for flag numbering
  DISPLAY_FONT_ROUTEFLAG_LABEL,

  // Must be last value
  DISPLAY_FONT_PALETTE_SIZE
} TDisplayFontIndex;

///
/// Represents a FontPalette object
///
typedef struct TFontPalette {
  const TFont *fonts[DISPLAY_FONT_PALETTE_SIZE];
  TPos padding[DISPLAY_FONT_PALETTE_SIZE];
} TFontPalette;

/// @}

/// @}

extern MAPCORE_API
void FontPalette_SetEntry(TFontPalette *pThis,
                          TDisplayFontIndex index,
                          const TFont *font);

extern MAPCORE_API
const TFont *FontPalette_GetEntry(const TFontPalette *pThis,
                                  TDisplayFontIndex index);

extern MAPCORE_API
void FontPalette_Create(TFontPalette *pThis);

///
/// Set the padding used for labels drawn with the specified font
/// @param pThis A font palette object.
/// @param index Display font index (padding is stored seperately for each font)
/// @param padding A TPos specifying x and y padding (in pixels)
///
extern MAPCORE_API
void FontPalette_SetLabelPadding(TFontPalette *pThis,
                                 TDisplayFontIndex index,
                                 const TPos *padding);

///
/// Get the padding used for labels drawn with the specified font
/// @param pThis A font palette object.
/// @param index Display font index (padding is stored seperately for each font)
/// @param [out] padding A TPos receiving x and y padding (in pixels)
///
extern MAPCORE_API
void FontPalette_GetLabelPadding(const TFontPalette *pThis,
                                 TDisplayFontIndex index,
                                 TPos *padding);

#ifdef __cplusplus
}
#endif

#endif /* DIFONTS_H */
