#ifndef DIDISPLAYLINEP_H
#define DIDISPLAYLINEP_H

#include <display/didisplayline.h>
#include <innovEngine_types.h>
#include <math/gerect.h>

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void DisplayLine_Define(TDisplayLine *displayLine, INT16 x1, INT16 y1, INT16 x2, INT16 y2);

extern void DisplayLine_Expand(TDisplayLine *pThis, UINT32 margin);

extern void DisplayLine_GetMidPoint(TDisplayLine *pThis, TPos *midPoint);

extern BOOL8 DisplayLine_IsInsideScreenClip(const TDisplayLine *bbox, const TRect *clipRect);

extern BOOL8 DisplayLine_IsOutsideScreenClip(const TDisplayLine *bbox, const TRect *clipRect);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIDISPLAYLINEP_H */
