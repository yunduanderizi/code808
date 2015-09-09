#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

#include <graphics/grcontext.h>
#include <graphics/grplatform.h>
#include <logging/enlog.h>
#include <logging/sdebug.h>

#include <malloc.h>
#include "navcoreGraphics.h"
#include <QObject>
#include <QPainter>
#include <QImage>
#include "stdio.h"
#include "fontset.h"
#define ARRAYLENGTH(x) (sizeof(x) / sizeof(x[0]))
#if defined(ARM_LINUX)
    #define FONT_SIZE_IN_MAP 50
#else
    #define FONT_SIZE_IN_MAP 16
#endif

//#if (_WIN32_WINNT >= 0x0501)
//#define TEXT_QUALITY CLEARTYPE_NATURAL_QUALITY
//#elif (_WIN32_WINNT >= 0x0500)
//#define TEXT_QUALITY CLEARTYPE_QUALITY
//#elif (WINVER >= 0x0400)
//#define TEXT_QUALITY ANTIALIASED_QUALITY
//#else
#define TEXT_QUALITY DEFAULT_QUALITY
//#endif

RDBG_declareFlag(gdiFont);

TGdiPlatformGraphics g_defaultGraphics = { os_graphics, NULL };


/// Helper function to swap the current and previous fonts. We have a 2 element
/// cache of font handles, which is enough for a reasonably high cache hit rate.
 static
 BOOL8 swapFonts(TGdiPlatformGraphics *pThis) {
   const TFont *tempFont;

   tempFont = pThis->font;
   pThis->font = pThis->prevFont;
   pThis->prevFont = tempFont;

   return TRUE;
 }

// /// Change the font selected into the graphics context.
 static
 BOOL8 setFont(TGdiPlatformGraphics *pThis,
               const TFont *font)
 {
     if(pThis->qFont == NULL) {
         pThis->qFont  = new QFont(FONT_FAMILY, FONT_SIZE_IN_MAP);
     }
     return TRUE;
 }

// /// Draw text onto the graphics context.
 static
 BOOL8 drawText(TGdiPlatformGraphics *pThis,
                const wchar_t *text,UINT8* pscreen)
 {
     if (pThis->recordTextRect != NULL) {
       if (text[0] == '\0') {
         pThis->recordTextRect->xmin = 0;
         pThis->recordTextRect->ymax = 0;
         pThis->recordTextRect->xmax = 0;
         pThis->recordTextRect->ymin = 0;
       } else {

           QFontMetrics fm(*pThis->qFont);
           QString str(QString::fromUtf16(reinterpret_cast<const unsigned short *>(text)));
           QSize size = fm.size(0,str);

         pThis->recordTextRect->xmin = 0;
         pThis->recordTextRect->ymax = -(fm.leading() >> 1);
         pThis->recordTextRect->xmax = size.width() /*- 1*/;
         pThis->recordTextRect->ymin = -(size.height() - (fm.leading() >> 1));
       }

       pThis->recordTextRect = NULL;
     } else {
         QRect rect;
         rect = QRect(pThis->cursor.x,pThis->cursor.y,pThis->clipRect.right() + 1,pThis->clipRect.bottom() + 1);
         if(pscreen != NULL)
         {
             if(pThis->image == NULL)
                pThis->image = new QImage(pscreen,504, 387, QImage:: Format_RGB16);
             QPainter imagePainter((QImage *)pThis->image);
             imagePainter.setFont(*pThis->qFont);
             QString str(QString::fromUtf16(reinterpret_cast<const unsigned short *>(text)));
             imagePainter.drawText(rect,str);
             g_defaultGraphics.image = pThis->image;
         }
     }

   return TRUE;
 }

 /// Draw a filled rectangle (using the current backColour).
 static
 BOOL8 drawRectangle(TGdiPlatformGraphics *pThis,
                     const TRect *r,UINT8* pscreen)
 {
//   HBRUSH hbr = CreateSolidBrush(pThis->backColour);
//   RECT rect;

//   SetRect(&rect, r->xmin, -r->ymax, r->xmax, -r->ymin);
//   FillRect(pThis->hDC, &rect, hbr);
//   DeleteObject(hbr);
     QImage image(pscreen,504, 387, QImage:: Format_RGB16);
     QPainter imagePainter(&image);
     imagePainter.drawRect(QRect(r->xmin, -r->ymax, r->xmax, -r->ymin));
   return TRUE;
 }

 /// Draw a line (using the current foreColour and penWidth)
 static
 BOOL8 drawLine(TGdiPlatformGraphics *pThis,
                const TRect *rect,UINT8* pscreen)
 {
   return TRUE;
 }

 void setimageptr(void *ptr)
 {

 }

/// Once registered against a TGraphContext, this function will be called by
/// MapCore to perform graphics functions on the context. Different functions 
/// are specified by a command enumeration. Each command takes a single 
/// parameter, passed through a void pointer.
///
/// @brief Platform graphics layer handler entry point.
/// @param pThis A TGraphContext object
/// @param cmd A graphics command
/// @param param An optional parameter to the specified command
/// @retrun TRUE if the command was executed successfully, otherwise FALSE
///
BOOL8 os_graphics(TGraphContext *pThis,
                  TPlatformGraphicsCommand cmd,
                  const void *param) {
  TGdiPlatformGraphics *graphics = (TGdiPlatformGraphics*)pThis->graphics;
    //printf("os_graphics CMD:%d\n",cmd);
  //assert(graphics != NULL);

   switch (cmd) {
//   // cmdCreate - the parameter is an HDC, if it's NULL then we will have to
//   //             create a new context here, otherwise use the one provided.
   case cmdMoveTo:        // param is TPos
     graphics->cursor.x = ((const TPos*)param)->x;
     graphics->cursor.y = -((const TPos*)param)->y;
     break;
   case cmdCreate:
     // replace the TPlatformGraphics struct with a TGdiPlatformGraphics struct
     graphics = (TGdiPlatformGraphics *)malloc(sizeof(TGdiPlatformGraphics));
     if (graphics == NULL) {
       return FALSE;
     }
     memset(graphics, 0, sizeof(TGdiPlatformGraphics));
     graphics->func = pThis->graphics->func;
     pThis->graphics = (TPlatformGraphics*)graphics;

     break;
   case cmdDestroy:
     free(pThis->graphics);
     pThis->graphics = NULL;
     break;
   case cmdSetForeColour: // param is RGBCOLOUR
   {
     RGBCOLOUR rgbColour = *(const RGBCOLOUR*)param;
     graphics->foreColour = rgbColour;//RGBCOLOUR(rgbColour.red, rgbColour.green, rgbColour.blue);
     break;
   }
   case cmdSetBackColour: // param is RGBCOLOUR
   {
     RGBCOLOUR rgbColour = *(const RGBCOLOUR*)param;
     graphics->backColour = rgbColour;//RGBCOLOUR(rgbColour.red, rgbColour.green, rgbColour.blue);
     break;
   }
//   case cmdSetPenStyle:   // param is TPenStyle
//     break;
//   case cmdSetFillStyle:  // param is TFillStyle
//     break;
   case cmdSetFont:       // param is TFont
     setFont(graphics, (const TFont *)param);
     break;
   case cmdSetClipRect:
   {
     const TRect *rect = (const TRect*)param;
     graphics->clipRect.setTop(-rect->ymax);
     graphics->clipRect.setLeft(rect->xmin);
     graphics->clipRect.setBottom(-rect->ymin);
     graphics->clipRect.setRight(rect->xmax);
     break;
   }
//   case cmdSetTransform:  // param is TMatrix
//     break;

   case cmdRecordTextRect:   // param is TString; use cursor, pen, brush, ???transform???
     graphics->recordTextRect = (TRect*)param;
     break;
//   case cmdGetTextHeight: // param is UINT32; use cursor, pen, brush, ???transform???
//     getTextHeight(graphics, (UINT32 *)param);
//     break;
   case cmdDrawText:      // param is wchar_t; use cursor, pen, brush, ???transform???
     //GraphContext_DrawText(pThis, (TFont *)pThis->font, (UTF16*)param, &pos, RGB_FromInternal(0, pThis->depth));
     drawText(graphics, (const wchar_t *)param,pThis->screen);
     break;
   case cmdDrawLine:      // param is TRect
     drawLine(graphics, (const TRect *)param,pThis->screen);
     break;
//   case cmdDrawLineTo:    // param is TPos; use cursor, colours,
//     break;
//   case cmdDrawEllipse:   // param is TRect; use fill, colours,
//     break;
//   case cmdDrawPolygon:   // param is TPolygon; use pen, brush
//     break;
   case cmdDrawRectangle: // param is TRect; ???gradients???
     drawRectangle(graphics, (const TRect *)param,pThis->screen);
     break;
//   case cmdDrawBitmap:    // param is TBitmap; use transform, alpha
//     break;
   default:
     break;
   }
  return FALSE;
}
