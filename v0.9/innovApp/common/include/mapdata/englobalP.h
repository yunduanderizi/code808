#ifndef ENGLOBALP_H
#define ENGLOBALP_H

#include <mapdata/englobal.h>
#include <mapdata/enroadP.h>
#include <mapdata/enpocodeP.h>
#include <mapdata/enpoiP.h>
#include <mapdata/enpolyP.h>
#include <mapdata/engeomP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

TGenericMapInfo *road_findMapForDCA(const char *const dca);
TGenericMapInfo *poi_findMapForDCA(const char *const dca);
TGenericMapInfo *poitype_findMapForDCA(const char *const dca);

/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* ENGLOBALP_H */
