#ifndef DIJOURNEY_H
#define DIJOURNEY_H

#include <graphics/grbitmap.h>
#include <graphics/grcolour.h>
#include <routing/rtjourney.h>
#include <innovEngine_types.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

extern MAPCORE_API
void Journey_RenderFlags(TJourney *pThis, struct TViewport *viewport);

extern MAPCORE_API
void Journey_Render(TJourney *pThis, struct TViewport *viewport);

extern MAPCORE_API
void STATICMETHOD Journey_SetStartFlag(struct TViewport *viewport, const TBitmap *bitmap);

extern MAPCORE_API
void STATICMETHOD Journey_SetWaypointFlag(struct TViewport *viewport, 
                                          const TBitmap *bitmap,
                                          RGBCOLOUR colour,
                                          RGBCOLOUR colourShadow,
                                          TPos *shadowOffset,
                                          TPos *numberLocation);

extern MAPCORE_API
void STATICMETHOD Journey_SetEndFlag(struct TViewport *viewport, const TBitmap *bitmap);

#ifdef __cplusplus
}
#endif

#endif
