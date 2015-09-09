#ifndef DIBILLBOARDP_H
#define DIBILLBOARDP_H

#include <display/dibillboard.h>
#include <display/dicolour.h>
#include <graphics/grbitmap.h>
#include <graphics/grfontobj.h>
#include <innovEngine_types.h>
#include <language/lacodepage.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

/// @addtogroup Display
/// @{

///@addtogroup UnUsed
/// @ingroup Display
/// @{

///@addtogroup BillBoard
/// @ingroup UnUsed
/// @{


typedef struct TBillBoard
{
  TPos pos;
  const TBitmap *bitmap;
  UINT32 length;
} TBillBoard;

/// @}
/// @}
/// @}

extern void BillBoard_Define(TBillBoard *pThis,
                             const TPos *position,
                             UINT32 length, //< unused for now
                             const TBitmap *bitmap);

extern void BillBoard_Render(const TBillBoard *pThis, const struct TViewport *viewport);

///
/// TODO: this method does not work if strlen(szString) is > 2
/// (the result on the display is of very poor quality)
///
extern void BillBoard_RenderLabelled(const TBillBoard *pThis, 
                                     const struct TViewport *viewport,
                                     const TFont *font,
                                     RGBCOLOUR numberColour,
                                     RGBCOLOUR numberColourShadow,
                                     const TPos *shadowOffset,
                                     const TPos *labelLocation,
                                     const UTF16 *szString);

extern void BillBoard_RenderNumbered(const TBillBoard *pThis, 
                                     const struct TViewport *viewport,
                                     const TFont *font,
                                     RGBCOLOUR numberColour,
                                     RGBCOLOUR numberColourShadow,
                                     const TPos *shadowOffset,
                                     const TPos *numberLocation,
                                     INT32 number);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIBILLBOARDP_H */
