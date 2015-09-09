#ifndef DIPOLYSECTORP_H
#define DIPOLYSECTORP_H

#include <display/dipolysector.h>
#include <mapdata/enpolyP.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void PolySector_Render(TGeomLowerSectorHeader *lowerSector,
                              struct TViewport *viewport,
                              INT32 pass);

extern void PolySector_GetBoundingRect(TGeomLowerSectorHeader *sector,
                                       TRect *rect);

extern UINT32 PolySector_GetAreaThreshold(TGeomLowerSectorHeader *sector,
                                          struct TViewport *viewport);

extern void PolyUpperSector_GetBoundingRect(TGeomUpperSectorHeader *sector,
                                            TRect *rect);

extern BOOL8 PolyUpperSector_IsVisible(TGeomUpperSectorHeader *sector,
                                       struct TViewport *viewport);

extern void PolyUpperSector_Render(TGeomUpperSectorHeader *upperSector,
                                   struct TViewport *viewport,
                                   INT32 pass);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIPOLYSECTORP_H */
