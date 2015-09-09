#ifndef ENRFILEP_H
#define ENRFILEP_H

#include <mapdata/enrfile.h>
#include <mapdata/enmfile.h>
#include <mapdata/enroadP.h>
#include <mapdata/enmapinfoP.h>
#include <innovEngine_io.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

/* version of road data that the engine understands */
#define ROADFILE_MAJOR_VERSION 140
//@MIC Start
#define ROADFILE_MINOR_VERSION  33
#define ROADFILE_OLD_MINOR_VERSION  32
//@MIC End

#define ROAD_LAYER_ID 0UL

/*----------------------------------------------------------------*/

/* called once at startup to initialise the roadfile module */
BOOL8 roadfile_init(void);

TMapfileStatus
roadfile_loadBinSectorHeader(TRoadSectorHeader *const roadSectorHeader,
                             IO_FILE *mapFp,
                             TGenericMapInfo *const mapInfo);
TMapfileStatus
roadfile_loadBinSuperSectorHeader(TRoadSuperSectorHeader *const roadSuperSectorHeader,
                                  IO_FILE *mapFp,
                                  TGenericMapInfo *const mapInfo);

TChunkType
roadfile_resolveChunkType(struct TGenericMapInfo *const mapInfo,
                          const UINT32 chunkIndex,
                          UINT32 *layerP,
                          UINT32 *indexWithinLayerP);

TMapfileStatus road_load(TGenericMapInfo *const mapInfo);

BOOL8 roadfile_isBaseMap(const TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockSector(TRoadSectorHeader *sector);
UINT32 roadfile_unlockSector_(TRoadSectorHeader *sector);

#ifdef SIMULATOR
  /*
   * For simulator, we want to force the sector pointer to NULL
   * when sector has been unlocked (and its refcount has reached 0)
   * so we can catch code that is using sector after it has been
   * unlocked
   */
  extern UINT32 roadTmpLockCount;
# define roadfile_unlockSector(s)                    \
     ( roadTmpLockCount = roadfile_unlockSector_(s), \
       (s) = ((roadTmpLockCount == 0) ? NULL : (s)), \
       roadTmpLockCount                              \
     )
#else
# define roadfile_unlockSector(s) roadfile_unlockSector_(s)
#endif


UINT32 roadfile_lockSuperSector(TRoadSuperSectorHeader *superSector);
UINT32 roadfile_unlockSuperSector_(TRoadSuperSectorHeader *superSector);

#ifdef SIMULATOR
#define roadfile_unlockSuperSector(s)                   \
   ( roadTmpLockCount = roadfile_unlockSuperSector_(s), \
     (s) = ((roadTmpLockCount == 0) ? NULL : (s)),      \
     roadTmpLockCount                                   \
   )
#else
#define roadfile_unlockSuperSector(s) roadfile_unlockSuperSector_(s)
#endif

UINT32 roadfile_lockGlueNodes(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockGlueNodes(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockPostalCodes(TGenericMapInfo *const mapInfo, const UINT32 blocknr);
UINT32 roadfile_unlockPostalCodes(TGenericMapInfo *const mapInfo, const UINT32 blocknr);

UINT32 roadfile_lockPostalCodeIndex(TGenericMapInfo *const mapInfo, const UINT32 blocknr);
UINT32 roadfile_unlockPostalCodeIndex(TGenericMapInfo *const mapInfo, const UINT32 blocknr);

UINT32 roadfile_lockPlaces(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockPlaces(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockPlacePair(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockPlacePair(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockSectorPlacePair(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockSectorPlacePair(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockSectorPlacePairSet(TGenericMapInfo *const mapInfo, const UINT32 blocknr);
UINT32 roadfile_unlockSectorPlacePairSet(TGenericMapInfo *const mapInfo, const UINT32 blocknr);

UINT32 roadfile_lockRoadEntity(TGenericMapInfo *const mapInfo, const UINT32 blocknr);
UINT32 roadfile_unlockRoadEntity(TGenericMapInfo *const mapInfo, const UINT32 blocknr);

UINT32 roadfile_lockGlueRoadBounds(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockGlueRoadBounds(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockRoadSetHeader(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockRoadSetHeader(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockRoadSet(TGenericMapInfo *const mapInfo, const UINT32 blocknr);
UINT32 roadfile_unlockRoadSet(TGenericMapInfo *const mapInfo, const UINT32 blocknr);

UINT32 roadfile_lockRoadSetSPPSet(TGenericMapInfo *const mapInfo, const UINT32 blocknr);
UINT32 roadfile_unlockRoadSetSPPSet(TGenericMapInfo *const mapInfo, const UINT32 blocknr);

UINT32 roadfile_lockRoadSetIntersection(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockRoadSetIntersection(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockHouseNumberRange(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockHouseNumberRange(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockHouseNumber(TGenericMapInfo *const mapInfo, const UINT32 blocknr);
UINT32 roadfile_unlockHouseNumber(TGenericMapInfo *const mapInfo, const UINT32 blocknr);

UINT32 roadfile_lockPlaceNameSetHeader(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockPlaceNameSetHeader(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockPlaceNameSet(TGenericMapInfo *const mapInfo, const UINT32 blocknr);
UINT32 roadfile_unlockPlaceNameSet(TGenericMapInfo *const mapInfo, const UINT32 blocknr);

/*----------------------------------------------------------------*/

/* These functions deal with extended chunks that are only present in map
   versions 1.11 onwards. */
UINT32 roadfile_lockAddressDisplayCountries(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockAddressDisplayCountries(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockAddressDisplayFormats(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockAddressDisplayFormats(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockSignPostStringIndex(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockSignPostStringIndex(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockSignPostString(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockSignPostString(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockTimeZones(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockTimeZones(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockIntersections(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockIntersections(TGenericMapInfo *const mapInfo);

UINT32 roadfile_lockCountryState(TGenericMapInfo *const mapInfo);
UINT32 roadfile_unlockCountryState(TGenericMapInfo *const mapInfo);

/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* ENRFILEP_H */
