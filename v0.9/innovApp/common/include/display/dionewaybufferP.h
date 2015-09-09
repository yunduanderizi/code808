#ifndef DIONEWAYBUFFERP_H
#define DIONEWAYBUFFERP_H

#include <display/dionewaybuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

//=struct TViewport;

extern void OneWayArrow_Render(TDisplayOneWayArrow *pThis, struct TViewport *viewport);

extern BOOL8 OneWayArrows_IsFull(TOneWayArrowBuffer *pThis);

extern void  OneWayArrows_Reset(TOneWayArrowBuffer *pThis);

extern void  OneWayArrows_Add(TOneWayArrowBuffer *pThis,
                              UINT8 heading,
                              BOOL8 forwardsRestriction,
                              BOOL8 backwardsRestriction,
                              TPos *pos);

extern void  OneWayArrows_Render(TOneWayArrowBuffer *pThis, struct TViewport *viewport);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIONEWAYBUFFERP_H */
