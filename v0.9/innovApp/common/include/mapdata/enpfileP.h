#ifndef ENPFILEP_H
#define ENPFILEP_H

#include <mapdata/enpfile.h>
#include <mapdata/enmfile.h>
#include <mapdata/enmapinfoP.h>
#include <innovEngine_types.h>
#include <io/geio.h>
#include <mapdata/enpolyP.h>
#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

#define POLY_LAYER_ID 1UL

#define GEOM_ABSTRACTION_LEVELS 4

/* Geometry file version numbers */
#define GEOMFILE_MAJOR_VERSION 140
#define GEOMFILE_MINOR_VERSION   7

/*----------------------------------------------------------------*/

typedef UINT8 TMask_v1[32];


typedef struct TBinGeomLowerSectorHeader {
  TPos    origin;         /* x,y lat / long origin. */
  UINT16  size;           /* sector size (i.e. 4000) */
  UINT16  dataChunkIndex; /* index of the data chunk that contains this sector */
  UINT8   resolution;     /* left shifts to apply to 12 bit coordinates */
  UINT8   layer;          /* map layer */

  UINT16  UpperLevelSectorIndex;
} TBinGeomLowerSectorHeader;

typedef struct TBinGeomUpperSectorHeader {
  TPos    origin;         /* x,y lat / long origin */
  UINT16  size;           /* sector size (i.e. 4000) */
  UINT16  dataChunkIndex; /* index of the data chunk that contains this sector */
  UINT8   resolution;     /* left shifts to apply to 12 bit coordinates */
  UINT8   layer;          /* map layer */

  UINT16  UpperLevelSectorIndex;
  UINT16  LowerLevelSectorStartIndex;
  UINT16  LowerLevelSectorEndIndex;
  
} TBinGeomUpperSectorHeader;
 

/*----------------------------------------------------------------*/

/* called one at startup to initialise the polyfile module */
extern BOOL8 geomfile_init(void);

extern TMapfileStatus geomfile_loadChunkDir(TGenericMapInfo *const mapInfo);
extern TMapfileStatus geom_load(TGenericMapInfo *const mapInfo);

TChunkType
geomfile_resolveChunkType(struct TGenericMapInfo *const mapInfo,
                          const UINT32 chunkIndex,
                          UINT32 *layerP,
                          UINT32 *indexWithinLayerP);

extern BOOL8 geomfile_isCityMap(const TGenericMapInfo *const mapInfo);

extern BOOL8 geomfile_isBaseMap(const TGenericMapInfo *const mapInfo);

extern UINT32 geomfile_lockLowerSector(TGeomLowerSectorHeader* sectorHeader);

extern UINT32 geomfile_unlockLowerSector_(TGeomLowerSectorHeader* sectorHeader);
#ifdef SIMULATOR
/*
 * For simulator, we want to force the sector pointer to NULL
 * when sector has been unlocked and its refcount has reached 0)
 * so we can catch code that is using sector after it has been
 * unlocked
 */
extern UINT32 geomTmpLockCount;
#define geomfile_unlockLowerSector(s)                           \
    ( geomTmpLockCount = geomfile_unlockLowerSector_(s),        \
      (s) = (geomTmpLockCount == 0) ? NULL : (s),               \
      geomTmpLockCount                                          \
    )
#else
#define geomfile_unlockLowerSector(s) geomfile_unlockLowerSector_(s)
#endif

extern UINT32 geomfile_lockUpperSector(TGeomUpperSectorHeader* sectorHeader);

UINT32 geomfile_unlockUpperSector_(TGeomUpperSectorHeader* sectorHeader);
#ifdef SIMULATOR
#define geomfile_unlockUpperSector(s)                   \
   ( geomTmpLockCount = geomfile_unlockUpperSector_(s), \
     (s) = (geomTmpLockCount == 0) ? NULL : (s),        \
     geomTmpLockCount                                   \
   )
#else
#define geomfile_unlockUpperSector(s) geomfile_unlockUpperSector_(s)
#endif

extern UINT32 geomfile_lockDictionary(TGenericMapInfo *const mapInfo);

extern UINT32 geomfile_unlockDictionary(TGenericMapInfo *const mapInfo);

extern UINT32 geomfile_lockLandmark(TGenericMapInfo *const mapInfo);

extern UINT32 geomfile_unlockLandmark(TGenericMapInfo *const mapInfo);

extern void polyfile_unpackUINT12(UINT32 *x, UINT32 *y, POS24 *address);

extern UINT32 geomfile_loadBinGeomLowerSectorHeader(TGeomLowerSectorHeader *geomSectorHeader,
                                                    IO_FILE *mapFp);
extern UINT32 geomfile_loadBinGeomUpperSectorHeader(TGeomUpperSectorHeader *polyUpperSectorHeader, IO_FILE *mapFp);

#ifdef __cplusplus
}
#endif

#endif /* __ENPFILEP_H__ */
