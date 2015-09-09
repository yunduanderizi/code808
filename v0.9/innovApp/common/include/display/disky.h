#ifndef DISKY_H
#define DISKY_H

#include <graphics/grbitmap.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TSky
{
  const TBitmap *skyBitmap;
} TSky;

extern MAPCORE_API
void Sky_Render(TSky *pThis, struct TViewport *viewport);

extern MAPCORE_API
void Sky_SetBitmap(TSky *pThis, const TBitmap *bm);

#ifdef __cplusplus
}
#endif

#endif
