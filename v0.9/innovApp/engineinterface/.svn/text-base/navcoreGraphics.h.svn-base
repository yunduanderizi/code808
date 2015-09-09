#ifndef _PLATFORM_GRAPHICS_H_
#define _PLATFORM_GRAPHICS_H_

#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#include <graphics/grcontext.h>
#include <graphics/grplatform.h>
#include <QRect>
//#include <QFont>
#include <QImage>
#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL8 TPlatformGraphicsFunction(struct TGraphContext *pThis,
                                        TPlatformGraphicsCommand cmd,
                                        const void *param);

typedef struct Rect
{
    int top;
    int left;
    int right;
    int bottom;
} RECT;

typedef struct TGdiPlatformGraphics {
  TPlatformGraphicsFunction *func;
//   HDC hDC;
//   HFONT hFont;
//   HFONT hPrevFont;
//   HBITMAP hBitmap;
//   HBITMAP hOldBitmap;
  RGBCOLOUR foreColour;
  RGBCOLOUR backColour;
  UINT8  *pscreen;
  TPos cursor;
  UINT32 penWidth;
  TRect *recordTextRect;
  const TFont *font;
  const TFont *prevFont;
  QRect clipRect;
  void *image;
  QFont *qFont;
} TGdiPlatformGraphics;

extern TGdiPlatformGraphics g_defaultGraphics;

#ifdef __cplusplus
}
#endif

#endif // _PLATFORM_GRAPHICS_H_
