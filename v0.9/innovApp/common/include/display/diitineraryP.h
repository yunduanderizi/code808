#ifndef DIITINERARYP_H
#define DIITINERARYP_H

#include <display/diitinerary.h>
#include <graphics/grbitmap.h>
#include <graphics/grcolour.h>
#include <graphics/grfontobj.h>
#include <innovEngine_types.h>
#include <language/lacodepage.h>

#ifdef __cplusplus
extern "C" {
#endif

extern MAPCORE_API
const TBitmap *itinerary_getLabelledFlag(const TBitmap *flagBitmap,
                                         const UTF16 *szString,
                                         const TFont *font,
                                         RGBCOLOUR colour,
                                         RGBCOLOUR colourShadow,
                                         const TPos *labelLocation,
                                         const TPos *shadowOffset);

#ifdef __cplusplus
}
#endif

#endif
