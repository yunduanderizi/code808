#ifndef DIPOISECTORP_H
#define DIPOISECTORP_H

#include <display/dipoisector.h>
#include <display/diiconranklist.h>
#include <mapdata/enpoiP.h>
#include <math/gerect.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void POISector_Render(TPOISectorHeader *pThis,
                             struct TViewport *viewport,
                             TGenericMapInfo *const mapInfo,
                             TPOIMapLayer layer,
                             const TRect *screenRect,
                             TIconRankSet *rankSet);

extern void POISector_RenderSettlements(TPOISectorHeader *sector,
                                        TGenericMapInfo *const mapInfo,
                                        const TRect *screenRect,
                                        struct TViewport *viewport);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIPOISECTORP_H */
