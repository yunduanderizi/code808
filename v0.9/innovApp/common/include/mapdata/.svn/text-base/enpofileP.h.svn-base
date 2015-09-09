#ifndef ENPOFILEP_H
#define ENPOFILEP_H

#include <mapdata/enpofile.h>
#include <mapdata/enmfile.h>
#include <mapdata/enpocodeP.h>
#include <mapdata/enmapinfoP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

/* the map engine understands these versions of POI map files */
#define POCODEFILE_MAJOR_VERSION 140
#define POCODEFILE_MINOR_VERSION   0

#define POCODE_LAYER_ID 4UL

/* called once at startup to initialise the roadfile module */
BOOL8 pocodefile_init(void);
TMapfileStatus pocodefile_loadChunkDir(TGenericMapInfo *const mapInfo);
TMapfileStatus pocode_load(TGenericMapInfo *const mapInfo);

UINT32 pocodefile_lockSub(TGenericMapInfo  *const mapInfo,
                          UINT32                  chkIdx);
void pocodefile_unlockSub(TGenericMapInfo  *const mapInfo,
                          INT32                   chkIdx);
UINT32 pocodefile_lockSegmentLink(TGenericMapInfo  *const mapInfo,
                                  INT32             chkIdx);
void pocodefile_unlockSegmentLink(TGenericMapInfo  *const mapInfo,
                                  INT32             chkIdx);
/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* ENPOFILEP_H */
