#ifndef DIITINERARY_H
#define DIITINERARY_H

#include <graphics/grbitmap.h>
#include <graphics/grcolour.h>
#include <graphics/grfontobj.h>
#include <innovEngine_types.h>
#include <routing/rtitinerary.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TItineraryFlag {
  const struct TBitmap *bitmap;
  TRect bitmapRect;
  RGBCOLOUR textColour;
  RGBCOLOUR textShadowColour;
  TPos textOffset;
  TPos textShadowOffset;
  const struct TFont *font;
} TItineraryFlag;

typedef struct TItineraryFlags {
  const TItineraryFlag *start;
  const TItineraryFlag *waypoint;
  const TItineraryFlag *finish;
} TItineraryFlags;

extern MAPCORE_API
void itinerary_render(const TItinerary *pThis,
                      struct TViewport *viewport,
                      TItineraryFlags *flags,
                      BOOL8 showVisited);

#ifdef __cplusplus
}
#endif

#endif
