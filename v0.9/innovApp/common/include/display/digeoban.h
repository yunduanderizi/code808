#ifndef DIGEOBAN_H
#define DIGEOBAN_H

#include <graphics/grbitmap.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TGeoban
{
  TBitmap *tileBitmap;
} TGeoban;

extern MAPCORE_API
void Geoban_New(TGeoban *pThis);

extern MAPCORE_API
void Geoban_Render(TGeoban *pThis, struct TViewport *viewport);

extern MAPCORE_API
void Geoban_SetBitmap(TGeoban *pThis, TBitmap *bitmap);

#ifdef __cplusplus
}
#endif

#endif
