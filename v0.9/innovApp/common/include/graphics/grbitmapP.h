#ifndef GRBITMAPP_H
#define GRBITMAPP_H

#include <graphics/grbitmap.h>
#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif

UINT32 Bitmap_GetDataSize(const TBitmap *bitmap);

UINT8 *Bitmap_GetData(const TBitmap *bitmap);

int Bitmap_GetPixelColour(const TBitmap *pThis, UINT8 *data, INT32 u, INT32 v);

#ifdef __cplusplus
}
#endif

#endif /* GRBITMAPP_H */
