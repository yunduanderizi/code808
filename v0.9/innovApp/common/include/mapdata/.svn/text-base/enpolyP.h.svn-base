#ifndef ENPOLYP_H
#define ENPOLYP_H

#include <mapdata/enpoly.h>
#include <innovEngine_types.h>
#include <innovEngine_math.h>
#include <mapdata/enmfileP.h>
#include <mapdata/enmapinfoP.h>
#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

/* Maximum number of donut holes per polygon */
#define GEOM_MAX_DONUT_COUNT 128

enum {
  GEOM_MAP_LAYER_0,
  GEOM_MAP_LAYER_1,
  GEOM_MAP_LAYER_2,
  GEOM_MAP_LAYER_3,
  // *** THIS MUST COME LAST
  GEOM_MAP_LAYERS
};

typedef enum TExtendedDataType {
  GEOM_EXTENDED_DATA_TYPE_HEIGHT = 13139
} TExtendedDataType;


typedef struct TGeomLowerSectorHeader {
  TPos origin;           /* x,y lat / long origin */
  UINT16 size;           /* sector size (i.e. 4000) */
  UINT16 dataChunkIndex; /* index of the data chunk that contains this sector */
  UINT32 resolution:8;   /* left shifts to apply to 12 bit coordinates */
  UINT32 locks:4;        /* lock up to 16 times */
  UINT32 layer:4;        /* map layer */

  UINT32 UpperLevelSectorIndex:16;
  /* Calculated fields - not in the file */
  UINT32 offsetHint;     /* offset of polygon to render on the next pass */
  TGenericMapInfo *mapInfo;
} TGeomLowerSectorHeader;

typedef struct TGeomUpperSectorHeader {
  TPos origin;           /* x,y lat / long origin */
  UINT16 size;           /* sector size (i.e. 4000) */
  UINT16 dataChunkIndex; /* index of the data chunk that contains this sector */
  UINT32 resolution:8;   /* left shifts to apply to 12 bit coordinates */
  UINT32 locks:4;        /* lock up to 16 times */
  UINT32 layer:4;        /* map layer */

  UINT32 UpperLevelSectorIndex:16;
  UINT16 LowerLevelSectorStartIndex;
  UINT16 LowerLevelSectorEndIndex;
  /* Calculated fields - not in the file */
  UINT32 offsetHint;     /* offset of polygon to render on the next pass */
  TGenericMapInfo *mapInfo;
} TGeomUpperSectorHeader;

/* Flag is set if the map has coverage polygon data */
#define GEOM_GLOBAL_FLAG_HAS_COVERAGE 1
/* Flag is set if the map is a city map */
#define GEOM_GLOBAL_FLAG_IS_CITY      2
/* Flag is set if the map has landmark data */
#define GEOM_GLOBAL_FLAG_HAS_LANDMARK 4

/*----------------------------------------------------------------*/

typedef struct TBinGeomGlobals {
  TRect  bounds;
  UINT16 flags;

  UINT32 lowerSectorHeadersCount;
  UINT32 lowerSectorHeadersFptr;
  UINT32 upperSectorHeadersCount[GEOM_MAP_LAYERS - 1];
  UINT32 upperSectorHeadersFptr [GEOM_MAP_LAYERS - 1];
} TBinGeomGlobals;

/* An instance of the following structure hangs off every MapInfo
 * structure corresponding to a geometry map. At present it contains
 * everything from the ONDISK structure.
 */
typedef TBinGeomGlobals TGeomMapGlobals;

/*----------------------------------------------------------------*/
#define NUM_UPPER_GEOM_SECTOR_HEADERS (GEOM_MAP_LAYER_3 - GEOM_MAP_LAYER_0)
#define NUM_TEXTURE_CHUNKS 5

/* A MapHeader is an interpretation of the chunk directory table
 * which makes available symbolic names to refer to the first few
 * chunks. Macros such as <maptype>_CHUNK_ADDRESS allow us to
 * translate these names in the mapHeader into chunk-directory
 * indices. */
typedef struct TGeomMapHeader {
  /* These chunks at the start of the chunk directory */
  TChunkFptr dictionary;                  /* common vertex dictionary */
  TChunkFptr landmark;                    /* 3D landmark data */
  TChunkFptr palette;                     /* Texture image palette */
  TChunkFptr texture[NUM_TEXTURE_CHUNKS]; /* Texture chunks */
  TChunkFptr sectorData[1];               /* Actually can be more than 1 sector chunk */
} TGeomMapHeader;

#define GEOM_GLOBALS(mapInfo)                   \
  (assert((mapInfo)->mapType == MAPTYPE_GEOM),  \
   ((TGeomMapGlobals *)((mapInfo)->_globals)))

#define GEOM_CHUNK_INDEX(chunkName)                             \
  ((offsetof(TGeomMapHeader, chunkName)) / sizeof(TChunkFptr))

#define GEOM_CHUNK_SIZE_MEM(mapInfo, chunkName)         \
  CHUNK_SIZE_MEM(mapInfo, GEOM_CHUNK_INDEX(chunkName))
#define GEOM_CHUNK_IS_EMPTY(mapInfo, chunkName)         \
  CHUNK_IS_EMPTY(mapInfo, GEOM_CHUNK_INDEX(chunkName))

#define COUNTOF_GEOM_LOWER_SECTOR_HEADERS(mapInfo)      \
  (GEOM_GLOBALS(mapInfo)->lowerSectorHeadersCount)
#define COUNTOF_GEOM_UPPER_SECTOR_HEADERS(mapInfo, layer)       \
  (assert(layer >= GEOM_MAP_LAYER_1),                           \
   assert(layer < GEOM_MAP_LAYERS),                             \
   GEOM_GLOBALS(mapInfo)->upperSectorHeadersCount[(layer) - 1])
#define FPTROF_GEOM_LOWER_SECTOR_HEADERS(mapInfo)       \
  (GEOM_GLOBALS(mapInfo)->lowerSectorHeadersFptr)
#define FPTROF_GEOM_UPPER_SECTOR_HEADERS(mapInfo, layer)        \
  (assert(layer >= GEOM_MAP_LAYER_1),                           \
   assert(layer < GEOM_MAP_LAYERS),                             \
   GEOM_GLOBALS(mapInfo)->upperSectorHeadersFptr[(layer) - 1])

#define GEOM_LOWER_SECTOR_HEADER_VA(mapInfo, index) (UINT32)            \
  (assert((index) < COUNTOF_GEOM_LOWER_SECTOR_HEADERS(mapInfo)),        \
   mapInfo->mapStartVa +                                                \
   FPTROF_GEOM_LOWER_SECTOR_HEADERS(mapInfo) +                          \
   ((index) * sizeof(TBinGeomLowerSectorHeader)))
#define GEOM_LOWER_SECTOR_HEADER(mapInfo, index)                            \
  SVM_RESOLVE_GEOM_LOWER_SECTOR_HEADER(GEOM_LOWER_SECTOR_HEADER_VA(mapInfo, \
                                                                   index))
#define GEOM_UPPER_SECTOR_HEADER_VA(mapInfo, layer, index) (UINT32)       \
  (assert((index) < COUNTOF_GEOM_UPPER_SECTOR_HEADERS(mapInfo, (layer))), \
   mapInfo->mapStartVa +                                                  \
   FPTROF_GEOM_UPPER_SECTOR_HEADERS(mapInfo, (layer)) +                   \
   ((index) * sizeof(TBinGeomUpperSectorHeader)))
#define GEOM_UPPER_SECTOR_HEADER(mapInfo, layer, index)                     \
  SVM_RESOLVE_GEOM_UPPER_SECTOR_HEADER(GEOM_UPPER_SECTOR_HEADER_VA(mapInfo, \
                                                                   layer,   \
                                                                   index))

/*----------------------------------------------------------------*/

UINT32 geom_numberOfSectors(TGenericMapInfo *const mapInfo, UINT32 level);


/* Called once at startup to initialise the polygon map module */
extern BOOL8 geom_init(void);

/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __ENPOLYP_H__ */
