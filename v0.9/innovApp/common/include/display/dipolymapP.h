#ifndef DIPOLYMAPP_H
#define DIPOLYMAPP_H

#include <display/dipolymap.h>
#include <mapdata/enpolyP.h>
#include <math/gerect.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

typedef enum TPolyRenderPass{
  POLY_PASS_BASE      = 0,
  POLY_PASS_COVERAGE  = 1,
  POLY_PASS_ELEVATION = 2,
  POLY_PASS_DETAILS   = 3,
  POLY_PASS_CITYMAPS  = 4,
  POLY_PASS_LANDMARKS  = 5,
  POLY_PASS_LAST = 6
} TPolyRenderPass;

extern BOOL8 PolyMap_Render(TGenericMapInfo *const mapInfo,
                            struct TViewport *viewport,
                            INT32 globalTargetLevel,
                            BOOL8 isCityMap,
                            INT32 pass);

extern UINT32 PolyMap_GetSectorsVisibleAreaThreshold(struct TViewport *viewport);

extern void PolyMap_GetBoundingRect(const TGenericMapInfo *const mapInfo,
                                    TRect *rect);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIPOLYMAPP_H */
