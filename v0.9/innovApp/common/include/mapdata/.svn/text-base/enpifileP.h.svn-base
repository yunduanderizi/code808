#ifndef ENPIFILEP_H
#define ENPIFILEP_H

#include <mapdata/enpifile.h>
#include <mapdata/enmfile.h>
#include <mapdata/enpoiP.h>
#include <mapdata/enmapinfoP.h>
#include <innovEngine_io.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

#define POIFILE_MAJOR_VERSION 140
#define POIFILE_MINOR_VERSION   9

#define POI_LAYER_ID 3UL

/*----------------------------------------------------------------*/

TMapfileStatus poifile_loadChunkDir(TGenericMapInfo *const mapInfo);
TMapfileStatus poi_load(TGenericMapInfo *const mapInfo);
TMapfileStatus
poifile_loadBinSectorHeader(TPOISectorHeader *const poiSectorHeader,
                            IO_FILE *mapFp,
                            TGenericMapInfo *const mapInfo);
TMapfileStatus
poifile_loadBinSuperSectorHeader(TPOISuperSectorHeader *const poiSuperSectorHeader,
                                 IO_FILE *mapFp,
                                 TGenericMapInfo *const mapInfo);

TChunkType
poifile_resolveChunkType(struct TGenericMapInfo *const mapInfo,
                         const UINT32 chunkIndex,
                         UINT32 *layerP,
                         UINT32 *indexWithinLayerP);

UINT32 poifile_lockSector(TPOISectorHeader *sector );

UINT32 poifile_unlockSector_(TPOISectorHeader *sector );

#ifdef SIMULATOR
  /*
   * For simulator, we want to force the sector pointer to NULL
   * when sector has been unlocked (and its refcount has reached 0)
   * so we can catch code that is using sector after it has been
   * unlocked
   */
   extern UINT32 poiTmpLockCount;
#  define poifile_unlockSector(s)                     \
     ( poiTmpLockCount = poifile_unlockSector_(s),    \
       (s) = (poiTmpLockCount == 0) ? NULL : (s),     \
       poiTmpLockCount                                \
     )
#else
#  define poifile_unlockSector(s) poifile_unlockSector_(s)
#endif

UINT32 poifile_lockSuperSector(TPOISuperSectorHeader *superSector);

UINT32 poifile_unlockSuperSector_(TPOISuperSectorHeader *superSector);

#ifdef SIMULATOR
#  define poifile_unlockSuperSector(s)                     \
     ( poiTmpLockCount = poifile_unlockSuperSector_(s),    \
       (s) = (poiTmpLockCount == 0) ? NULL : (s),          \
       poiTmpLockCount                                     \
      )
#else
#  define poifile_unlockSuperSector(s) poifile_unlockSuperSector_(s)
#endif

UINT32 poifile_lockPOIContentTags(TGenericMapInfo *const mapInfo);
UINT32 poifile_unlockPOIContentTags(TGenericMapInfo *const mapInfo);

UINT32 poifile_lockPOIContentTagIndex(TGenericMapInfo *const mapInfo);
UINT32 poifile_unlockPOIContentTagIndex(TGenericMapInfo *const mapInfo);

UINT32 poifile_lockPOIType(TGenericMapInfo *const mapInfo);
UINT32 poifile_unlockPOIType(TGenericMapInfo *const mapInfo);

UINT32 poifile_lockPOITypeSetHeader(TGenericMapInfo *const mapInfo);
UINT32 poifile_unlockPOITypeSetHeader(TGenericMapInfo *const mapInfo);

UINT32 poifile_lockPOITypeSet(TGenericMapInfo *const mapInfo,UINT32 blocknr);
UINT32 poifile_unlockPOITypeSet(TGenericMapInfo *const mapInfo,UINT32 blocknr);

UINT32 poifile_lockPOINameSetHeader(TGenericMapInfo *const mapInfo);
UINT32 poifile_unlockPOINameSetHeader(TGenericMapInfo *const mapInfo);

UINT32 poifile_lockPOINameSet(TGenericMapInfo *const mapInfo,UINT32 blocknr);
UINT32 poifile_unlockPOINameSet(TGenericMapInfo *const mapInfo,UINT32 blocknr);

UINT32 poifile_lockPOINameSetTypeSet(TGenericMapInfo *const mapInfo,UINT32 blocknr);
UINT32 poifile_unlockPOINameSetTypeSet(TGenericMapInfo *const mapInfo,UINT32 blocknr);

UINT32 poifile_lockPOISectorPlace(TGenericMapInfo *const mapInfo);
UINT32 poifile_unlockPOISectorPlace(TGenericMapInfo *const mapInfo);

UINT32 poifile_lockPOIInstance(TGenericMapInfo *const mapInfo,UINT32 blocknr);
UINT32 poifile_unlockPOIInstance(TGenericMapInfo *const mapInfo,UINT32 blocknr);

UINT32 poifile_lockPOINameInstanceSet(TGenericMapInfo *const mapInfo,UINT32 blocknr);
UINT32 poifile_unlockPOINameInstanceSet(TGenericMapInfo *const mapInfo,UINT32 blocknr);

/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* ENPIFILEP_H */
