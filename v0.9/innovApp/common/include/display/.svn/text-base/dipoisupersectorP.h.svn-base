#ifndef DIPOISUPERSECTORP_H
#define DIPOISUPERSECTORP_H

#include <display/dipoisupersector.h>
#include <display/dipoiranklist.h>
#include <mapdata/enpoiP.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void POISuperSector_Render(TPOISuperSectorHeader *pThis,
                                  struct TViewport *viewport,
                                  TGenericMapInfo *const mapInfo,
                                  TPOIMapLayer layer,
                                  TIconRankSet *rankSet);

extern void POISuperSector_RenderSettlements(TPOISuperSectorHeader *pThis,
                                             TGenericMapInfo *const mapInfo,
                                             TPOIMapLayer layer,
                                             struct TViewport *viewport);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIPOISUPERSECTORP_H */
