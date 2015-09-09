#ifndef DIPOIRANKLISTP_H
#define DIPOIRANKLISTP_H

#include <mapdata/enmapinfoP.h>
#include <display/dipoiranklist.h>
#include <display/diiconranklistP.h>
#include <display/di3Dengi.h>
#include <mapdata/enpoiP.h>

#ifdef __cplusplus
extern "C" {
#endif

struct TViewport;

/**
 * @brief Information used to relate a POI icon back to the POI itself
 */
typedef struct TPOIIconInfo
{
  TPOITypeSetIndex16  typeSetIndex;
  TGenericMapInfo    *mapInfo;
  UINT32              id;
  UINT8               mapLayer;
} TPOIIconInfo;

extern void IconDisplayRank_DefinePOI(TIconDisplayRank *pThis,
                                      TGenericMapInfo  *const mapInfo,
                                      TPOI             *poi,
                                      TPos3d           *pos_s,
                                      struct TViewport *viewport);

#ifdef __cplusplus
}
#endif

#endif /* DIPOIRANKLISTP_H */
