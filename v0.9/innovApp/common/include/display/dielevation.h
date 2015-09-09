#ifndef DIELEVATION_H
#define DIELEVATION_H

#include <graphics/grcolour.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Display
/// @{

/// @addtogroup ElevationPalette
/// @ingroup Display
/// @{

/*
 * Following tables are used to compute the colour of an elevation 
 * polygon.  Instead of storing a table of colours for all possible
 * elevations, table below stores colours only for a few elevations.
 * Actual colour of a given elevation is linearly interpolated between
 * 2 consecutive entries in the table.
 */
#define MAX_ELEVATIONS_COLOUR 8


// The elevation palette MUST be private
// as its functionality is wrapped around the PolyDisplayTheme class.
// We can't have opaque structures yet, so leave the definition included for now.

typedef struct TElevationPaletteEntry {
  INT32 elevation;
  UINT8 r, g, b;  /* red green and blue colour component at this elevation */
} TElevationPaletteEntry;


///
/// @internal
/// Represents an elevation palette object
///
typedef struct TElevationPalette
{
  UINT32 elevationThresholdsCount;
  TElevationPaletteEntry elevationPalette[MAX_ELEVATIONS_COLOUR];
  /// Cache the last computed colour corresponding to an elevation
  RGBCOLOUR cachedColour;
  INT32 cachedElevation;
} TElevationPalette;



extern MAPCORE_API
void ElevationPalette_Init(TElevationPalette *pThis);

extern MAPCORE_API
void ElevationPalette_Define(TElevationPalette *pThis,
                             const TElevationPaletteEntry ep[],
                             UINT32 n);

extern MAPCORE_API
UINT32 ElevationPalette_GetNumElevations(const TElevationPalette *pThis);

extern MAPCORE_API
void ElevationPalette_GetElevationTable(const TElevationPalette *pThis,
                                        UINT32 size,
                                        TElevationPaletteEntry *table);

extern MAPCORE_API
RGBCOLOUR ElevationPalette_GetElevationColour(TElevationPalette *pThis,
                                              INT32 elevation);
// @}

// @}

#ifdef __cplusplus
}
#endif

#endif /* dielevation.h */
