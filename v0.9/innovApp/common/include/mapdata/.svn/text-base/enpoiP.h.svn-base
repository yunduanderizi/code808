#ifndef ENPOIP_H
#define ENPOIP_H

#include <location/locmaplocationP.h>
#include <mapdata/enpoi.h>
#include <mapdata/enpoitype.h>
#include <mapdata/enmfileP.h>
#include <mapdata/enroadP.h>
#include <mapdata/enmapinfoP.h>
#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup MapData
///    @{

#define POI_CHUNK_COUNT_POI_TYPE_SET             8            
#define POI_CHUNK_COUNT_POI_NAME_SET             8 //20 
#define POI_CHUNK_COUNT_POI_NAME_SET_TYPE_SET    8
#define POI_CHUNK_COUNT_POI_INSTANCE             8
#define POI_CHUNK_COUNT_POI_NAME_INSTANCE_SET    8 //18 
#define POI_CHUNK_COUNT_TOKENISED_POI_NAME       25 //86 
#define POI_CHUNK_COUNT_POI_TOKEN                20

/* Bit flags in global chunk of poi map */
#define POI_GLOBAL_FLAG_IS_BASEMAP      2

/*----------------------------------------------------------------*/



/* These identify extended data chunks that have been added in poi formats
   1.02 and upwards. */
typedef enum {

  /* New tokenised chunks */
  POI_EXTENDED_CHUNK_IDX_TOKENISED_POI_NAME,

  POI_EXTENDED_CHUNK_IDX_POI_TOKEN =
    POI_EXTENDED_CHUNK_IDX_TOKENISED_POI_NAME +
    POI_CHUNK_COUNT_TOKENISED_POI_NAME,

  POI_EXTENDED_CHUNK_IDX_TOKEN_HEADER =
    POI_EXTENDED_CHUNK_IDX_POI_TOKEN +
    POI_CHUNK_COUNT_POI_TOKEN,

  POI_EXTENDED_CHUNK_IDX_POI_TOKEN_HAS_ALT,

  POI_EXTENDED_CHUNK_IDX_ALT_TOKEN,

  POI_EXTENDED_CHUNK_ROAD_POI_SECTOR_COMBO,

  /* THIS MUST COME LAST */
  POI_EXTENDED_CHUNKS
} TPOIExtendedChunks;


/* The numeric suffix in the typedef names must be the
 * same as in the decl type. */
typedef UINT32 TPOINameIndex32;
typedef UINT32 TPOINameSetIndex32;
typedef UINT16 TPOITypeIndex16;
typedef UINT16 TPOITypeSetIndex16;
typedef UINT32 TPOINameSetTypeSetIndex32;
typedef UINT16 TPOISectorIndex16;
typedef UINT16 TPOISectorPlaceIndex16;
typedef UINT32 TPOIInstanceComboIndex32;


#define POI_NAME_SET_BUFFER_SIZE (sizeof(TAllocatorHandle)/sizeof(TPOINameIndex32))
#define POI_NAME_SET_DEFAULT_COUNT 6

/** @note this structure cannot be moved or copied */
typedef struct TPOINameSet {
  TPOINameIndex32    *names;
  UINT16              officialNameCount;
  UINT16              officialExonymCount;
  UINT16              alternateNameCount;
  union {
    TAllocatorHandle  allocator;
    TPOINameIndex32   names[POI_NAME_SET_BUFFER_SIZE];
  }                   _buffer;
} TPOINameSet;


#define POI_TYPE_SET_BUFFER_SIZE (sizeof(TAllocatorHandle)/sizeof(TPOITypeIndex16))

/** @note this structure cannot be moved or copied */
typedef struct TPOITypeSet {
  TPOITypeIndex16    *types;
  UINT16              types1Count;
  UINT16              types2Count;
  union {
    TAllocatorHandle  allocator;
    TPOITypeIndex16   types[POI_TYPE_SET_BUFFER_SIZE];
  }                   _buffer;
} TPOITypeSet;


typedef struct TPOINameSetTypeSet {
  TPOINameSetIndex32 nameSetIndex;
  TPOITypeSetIndex16 typeSetIndex;
} TPOINameSetTypeSet;


typedef struct TPOISectorPlace {
  TPOISectorIndex16 POISectorIndex;
  TRoadPlaceIndex16 placeIndex;
} TPOISectorPlace;


typedef struct TPOIInstanceCombo {
  TPOITypeSetIndex16     typeSetIndex;
  TPOISectorPlaceIndex16 sectorPlaceIndex;
  UINT16                 instanceCount;
} TPOIInstanceCombo;

#define POI_INSTANCE_COMBO_INSTANCE_COUNT_MIN (1)
#define POI_INSTANCE_COMBO_INSTANCE_COUNT_MAX (256)


#define POI_INSTANCE_COMBO_SET_BUFFER_SIZE (sizeof(TAllocatorHandle)/sizeof(TPOIInstanceComboIndex32))

/** @note this structure cannot be moved or copied */
typedef struct TPOIInstanceComboSet {
  TPOIInstanceComboIndex32   *instanceCombos;
  UINT16                      instanceComboCount;
  union {
    TAllocatorHandle          allocator;
    TPOIInstanceComboIndex32  instanceCombos[POI_INSTANCE_COMBO_SET_BUFFER_SIZE];
  }                           _buffer;
} TPOIInstanceComboSet;


typedef struct TBinPOISuperSectorHeader {
  /* The fields in this structure should have counterparts in the
   * in-memory TPOISuperSectorHeader; the in-memory superSectorHeader
   * is expected to be a superset of this structure. */
  UINT32 globalStartVa;
  UINT32 globalEndVa;

  /*
   * Caveat: sGlobalStartVa and sGlobalEndVa in super sector header
   * are the start and end of all sector data (not super sector!)
   * within this super sector.
   */
  UINT32 sGlobalStartVa;
  UINT32 sGlobalEndVa;
  TPos   origin;
  UINT32 size;
  UINT16 dataChunkIndex;
  UINT8  resolution;
  UINT8  layer;
  UINT16 sectorStartIndex;
  UINT16 sectorEndIndex;
} TBinPOISuperSectorHeader;

typedef struct TPOISuperSectorHeader {
  UINT32 globalStartVa;
  UINT32 globalEndVa;

  /*
   * Caveat: globalStart and globalEnd in super sector header
   * are the start and end of all sector data (not super sector!)
   * within this super sector.
   */
  UINT32 sGlobalStartVa;
  UINT32 sGlobalEndVa;
  TPos   origin;
  UINT32 size;
  UINT16 dataChunkIndex;
  UINT8  resolution;
  UINT8  layer;
  UINT16 sectorStartIndex;
  UINT16 sectorEndIndex;
  /* The fields above this comment should have a one-to-one
   * correspondence with those in the ondisk POI Super Sector Header. */

  struct TGenericMapInfo *mapInfo;
} TPOISuperSectorHeader;

typedef struct TBinPOISectorHeader {
  /* The fields in this structure should have counterparts in the
   * in-memory TPOISectorHeader; the in-memory sectorHeader is
   * expected to be a superset of this structure. */
  UINT32 globalStartVa;
  UINT32 globalEndVa;
  TPos   origin;
  UINT16 halfSizeX, halfSizeY; /* Half size in X and Y of bounding box
                                  centered around sector origin (need to
                                  be left-shifted by resolution).  This
                                  bounding box is guaranteed to contain
                                  all POIs and settlements in this sector */
  UINT16 dataChunkIndex;
  UINT8  resolution;
  UINT8  layer;
} TBinPOISectorHeader;

typedef struct TPOISectorHeader {
  UINT32 globalStartVa;
  UINT32 globalEndVa;
  TPos   origin;
  UINT16 halfSizeX, halfSizeY; /* Half size in X and Y of bounding box
                                  centered around sector origin (need to
                                  be left-shifted by resolution).  This
                                  bounding box is guaranteed to contain
                                  all POIs and settlements in this sector */
  UINT16 dataChunkIndex;
  UINT8  resolution;
  UINT8  layer;
  /* The fields above this comment should have a one-to-one
   * correspondence with those in the ondisk POI Sector Header. */

  UINT8 *globalAddrTrans;
  UINT8 *poiContentPa;
  struct TGenericMapInfo *mapInfo;
} TPOISectorHeader;

typedef struct TPOISectorBlock {
  /* file pointers to these data... */
  UINT32 poiContentOffset;
  UINT32 poiContentSize;
  UINT32 poiDataVa;
  UINT32 poiSize;
  UINT32 settlementDataVa;
  UINT32 settlementSize;
  UINT32 segLinksDataVa;
  UINT32 segLinksSize;
} TPOISectorBlock;

typedef struct TPOI {
  /* THIS MUST STAY HERE!!!
     encoded directly in the first byte of the POI. */
#ifndef PLATFORM_BIG_ENDIAN
  BITFIELD32                 flagImportanceChange     : 1;
  BITFIELD32                 flagPlaceChange          : 1;
  BITFIELD32                 flagNameSetTypeSetChange : 1;
  BITFIELD32                 flagSpare1               : 1;
  BITFIELD32                 flagSegPosChange         : 1;
  BITFIELD32                 flagSpare2               : 1;
  BITFIELD32                 flagContent              : 1;
  BITFIELD32                 flagEntrySegChange       : 1;
#else
#error BIG ENDIAN NOT CURRENTLY SUPPORTED
#endif

  UINT8                      importance;
  UINT8                      contentLen;
  UINT8                      size;
  UINT8                      entryPosData;

  TRoadSectorIndex16         roadSectorIndex;
  UINT16                     roadSegmentOffset;

  TRoadPlaceIndex16          placeIndex;
  TPOINameSetTypeSetIndex32  nameSetTypeSetIndex;

  UINT32                     parent;
  TPos                       pos;

  UINT8                     *contentData;
  UINT32                     ID;
  TPOISectorHeader          *sectorHeader;
} TPOI;

typedef struct TPOISettlement {
  /* THIS MUST STAY HERE!!!
     encoded directly in the first 16 bits of the settlement */
#ifndef PLATFORM_BIG_ENDIAN
  UINT8              flagDisplay          : 1;
  UINT8              flagImportanceChange : 1;
  UINT8              spare                : 6;
#else
#error BIG ENDIAN NOT SUPPORTED
#endif
  TPos               pos;
  TRoadPlaceIndex16  placeIndex;
  UINT8              importance;
  UINT8              size;
  UINT32             ID;
  TPOISectorHeader  *sectorHeader;
} TPOISettlement;

/* ONDISK global data chunk definition - this data applies to a whole map */
typedef struct TBinPOIGlobals {
  /* global map data - bounds, etc */
  TRect   bounds;     /* the map area is within these bounds */
  UINT8   flags;      /* bit flags: basemap? */
  char    vendor[2];  /* vendor id (TA, NT, GS, SS, ...) */
  UINT8   spare;

  UINT32  sectorHeadersCount     [POI_MAP_LAYERS];
  UINT32  sectorHeadersFptr      [POI_MAP_LAYERS];
  UINT32  superSectorHeadersCount[POI_MAP_LAYERS];
  UINT32  superSectorHeadersFptr [POI_MAP_LAYERS];

  UINT32  crcRoadMap; /* 32bit CRC of the matching road map */
} TBinPOIGlobals;

/* An instance of the following structure hangs off every MapInfo
 * structure corresponding to a poi map. At present it contains
 * everything from the ONDISK structure except the mapName field.
 */
typedef struct TPOIMapGlobals {
  /* global map data - bounds, etc */
  TRect   bounds;     /* the map area is within these bounds */
  UINT8   flags;      /* bit flags: basemap? */
  char    vendor[2];  /* vendor id (TA, NT, GS, SS, ...) */
  UINT8   spare;

  UINT32  sectorHeadersCount     [POI_MAP_LAYERS];
  UINT32  sectorHeadersFptr      [POI_MAP_LAYERS];
  UINT32  superSectorHeadersCount[POI_MAP_LAYERS];
  UINT32  superSectorHeadersFptr [POI_MAP_LAYERS];

  UINT32  crcRoadMap; /* 32bit CRC of the matching road map */
  /* The order of the above fields matches that of the ondisk
   * structure. It should not be tampered with. */

  TGenericMapInfo *roadMapInfo;/* Matching road map for this POI map. */
} TPOIMapGlobals;

/* Name type for tokens and names */
typedef enum {
  POI_NAME_TYPE_POI
}TPOINameType;

typedef struct TBinPoiTokenHeader {
  UINT8   sizeHeader;
  UINT8   languageCodes[3];
  UINT32  tokPartBlobSz;

  /* The implicit token id used for separator (often space) */
  UINT16  implicitSepPoiTokId;

  /* The number of separator tokens in this file */
  UINT16  poiSepTokenCount;

  /* Offsets of the various tables */
  UINT16  offsetPoiMostFrequent;
  UINT16  offsetTokenParts;
  UINT16  offsetExactSpIdx;
  UINT16  offsetPhonemeIdx;
  UINT16  offsetEquivSpell;
  UINT16  offsetDataBlob;
  UINT16  offsetTemplateTable;
  UINT16  offsetTemplateDict;

  /* Number of rows in the above tables */
  UINT16  mostFreqPoiTokenCount;
  UINT16  tokenPartCount;
  UINT16  equivSpellCount;
  UINT16  altPoiTokenCount;
  UINT16  templateRowCount;
  UINT16  templateDictCount;
} TBinPoiTokenHeader;

/* A MapHeader is an interpretation of the chunk directory table which
 * makes available symbolic names to refer to the first few
 * chunks. Macros such as <maptype>_CHUNK_ADDRESS allow us to
 * translate these names in the mapHeader into chunk-directory
 * indices. */
struct TPOIMapHeader {
  /* These chunks at the start of the chunk directory */
  TChunkFptr poiContentTags;
  TChunkFptr poiContentTagIndex;
  TChunkFptr poiType;
  TChunkFptr poiTypeSetHeader;
  TChunkFptr poiTypeSet[POI_CHUNK_COUNT_POI_TYPE_SET];
  TChunkFptr poiNameSetHeader;
  TChunkFptr poiNameSet[POI_CHUNK_COUNT_POI_NAME_SET];
  TChunkFptr poiNameSetTypeSet[POI_CHUNK_COUNT_POI_NAME_SET_TYPE_SET];
  TChunkFptr poiSectorPlace;
  TChunkFptr poiInstance[POI_CHUNK_COUNT_POI_INSTANCE];
  TChunkFptr poiNameInstanceSet[POI_CHUNK_COUNT_POI_NAME_INSTANCE_SET];

  /* other chunks containing sector data follow */
  TChunkFptr sectorData[1]; /* up to 30k chunks allowed */
};

#define POI_GLOBALS(mapInfo)                    \
  (assert(((mapInfo))->mapType == MAPTYPE_POI), \
   ((TPOIMapGlobals *)((mapInfo)->_globals)))

#define POI_CHUNK_INDEX(chunkName)                              \
  ((offsetof(TPOIMapHeader, chunkName)) / sizeof(TChunkFptr))

#define POI_CHUNK_SIZE_MEM(mapInfo, chunkName)                          \
  CHUNK_SIZE_MEM((mapInfo), POI_CHUNK_INDEX(chunkName))
#define POI_CHUNK_IS_EMPTY(mapInfo, chunkName)                          \
  CHUNK_IS_EMPTY((mapInfo), POI_CHUNK_INDEX(chunkName))

#define COUNTOF_POI_SECTOR_HEADERS(mapInfo, layer)      \
  (assert((layer) < POI_MAP_LAYERS),                    \
   POI_GLOBALS(mapInfo)->sectorHeadersCount[layer])
#define COUNTOF_POI_SUPER_SECTOR_HEADERS(mapInfo, layer)        \
  (assert((layer) < POI_MAP_LAYERS),                            \
   POI_GLOBALS(mapInfo)->superSectorHeadersCount[layer])
#define FPTROF_POI_SECTOR_HEADERS(mapInfo, layer)       \
  (assert((layer) < POI_MAP_LAYERS),                    \
   POI_GLOBALS(mapInfo)->sectorHeadersFptr[layer])
#define FPTROF_POI_SUPER_SECTOR_HEADERS(mapInfo, layer) \
  (assert((layer) < POI_MAP_LAYERS),                    \
   POI_GLOBALS(mapInfo)->superSectorHeadersFptr[layer])

#define POI_SECTOR_HEADER_VA(mapInfo, layer, index) (UINT32)            \
  (assert((index) < COUNTOF_POI_SECTOR_HEADERS(mapInfo, (layer))),      \
   mapInfo->mapStartVa +                                                \
   FPTROF_POI_SECTOR_HEADERS(mapInfo, (layer)) +                        \
   ((index) * sizeof(TBinPOISectorHeader)))
#define POI_SECTOR_HEADER(mapInfo, layer, index) ((TPOISectorHeader *) \
  SVM_RESOLVE_POI_SECTOR_HEADER(POI_SECTOR_HEADER_VA(mapInfo, layer, index)))
#define POI_SUPER_SECTOR_HEADER_VA(mapInfo, layer, index) (UINT32)      \
  (assert((index)<COUNTOF_POI_SUPER_SECTOR_HEADERS(mapInfo, (layer))),  \
   mapInfo->mapStartVa +                                                \
   FPTROF_POI_SUPER_SECTOR_HEADERS(mapInfo, (layer)) +                  \
   ((index) * sizeof(TBinPOISuperSectorHeader)))
#define POI_SUPER_SECTOR_HEADER(mapInfo, layer, index)         \
  ((TPOISuperSectorHeader *)                                   \
    SVM_RESOLVE_POI_SUPER_SECTOR_HEADER(                       \
      POI_SUPER_SECTOR_HEADER_VA(mapInfo, layer, index)))

/*----------------------------------------------------------------*/

/* In each map file pointers to objects are relative to the start of
 * the file.  The poifile and mapfile module allows them to be linked
 * into a global range of map pointer addresses.  It does this by
 * making POI POINTERS GLOBAL by allocating each map a range of
 * pointer addresses.  poi_findMap finds the map containing the
 * specified object in the global map space.  The guessMap parameter
 * can be NULL or may point to a map that probably contains the
 * required data.
 */
TGenericMapInfo *poi_findMap(UINT32 globalMapPtr, TGenericMapInfo *guessMap);

/* Find the first and last super sector indexes in order to cover the
 * entire rectangular region <bounds_w> (in world coordinates).
 *
 * Returns TRUE if any super sector overlaps visible area or FALSE if
 * no super sector overlaps visible area.  firstIndex and lastIndex
 * are set only when function returns TRUE.
 */
BOOL8 poi_findSuperSectorRange(INT32           *firstIndex,
                               INT32           *lastIndex,
                               TGenericMapInfo *const mapInfo,
                               TPOIMapLayer     mapLevel,
                               const TRect     *bounds_w);

/* In each map file pointers to objects are relative to the start of
 * the file.  The poifile and mapfile module allows them to be linked
 * into a global range of map pointer addresses.  It does this by
 * making POI POINTERS GLOBAL by allocating each map a range of
 * pointer addresses.  poi_findSuperSector finds the sector containing
 * the specified object in the global map space.  The guessSuperSector
 * parameter can be NULL or may point to a sector that probably
 * contains the required data.
 */
TPOISuperSectorHeader *
poi_findSuperSector(UINT32 globalMapPtr,
                    TPOISuperSectorHeader *guessSuperSector);

/* In each map file pointers to objects are relative to the start of
 * the file.  The poifile and mapfile module allows them to be linked
 * into a global range of map pointer addresses.  It does this by
 * making POI POINTERS GLOBAL by allocating each map a range of
 * pointer addresses.  poi_findSector finds the sector containing the
 * specified object in the global map space.  The guessSector
 * parameter can be NULL or may point to a sector that probably
 * contains the required data.
 */
TPOISectorHeader *poi_findSector(UINT32 globalMapPtr,
                                 TPOISectorHeader *guessSector);

/* Return the address of the data block at the start of a sector.
 * The sector should first be locked by calling poi_lockSector.
 */
TPOISectorBlock *poi_getSectorBlock(const TPOISectorHeader *sector);

/* Unpack POI data at source file offset into TPOI structure.  POIs
 * must be unpacked in sequence to extract the delta compressed
 * data. Please see poi_unpackCompletePOI for an alternate function
 * if all POI details need to be unpacked */
void poi_unpackPOI(TPOI *poi, TPOISectorHeader *sector, UINT32 source);

/* Unpack POI data at source file offset into TPOI structure. All
 * data fields in the POI are correctly populated, but this requires
 * a complete unpack of all POI records in the sector (leading up
 * to the required POI)
 */
void poi_unpackCompletePOI( TPOI *poi, TPOISectorHeader *sector, UINT32 source );

/* Unpack POI data at source file offset into TPOI structure.  POIs
 * must be unpacked in sequence to extract the delta compressed
 * data. */
void poi_unpackSettlement(TPOISettlement *settlement,
                          TPOISectorHeader *sector,
                          UINT32 source);

/* get the number of poi name records in a particular poi map. */
UINT32 poi_getGlobalPOINameCount(TGenericMapInfo *const mapInfo);

/* Called once at startup to initialise the poi module. */
BOOL8 poi_init(void);

/* This function extracts the next field of POI content from the POI.
 * offset should be zero if the first POI content field is required
 * and is incremented as POI content is unpacked.  The function
 * returns an updated offset.  No more POI content data is available
 * if the returned offset is >= poi->contentLen, or if output->tag ==
 * POI_CONTENT_END.
 */
UINT8 poi_unpackContentField(TPOIContentField *output,
                             const TPOI *poi,
                             TGenericMapInfo *const pMapInfo,
                             UINT8 offset /* set to zero to extract first content field */ );

/* Get the name for a certain poi type. */
UINT8 poi_unpackGlobalPOITypeName(UTF16 *output,
                                  TGenericMapInfo *const mapInfo,
                                  const UINT32 index);

/* Get the unique identifier for a certain poi type. */
UINT8 poi_unpackGlobalPOITypeUniqueID(UTF16 *output,
                                      TGenericMapInfo *const mapInfo,
                                      const UINT32 index);

/* Get the icon name for a certain poi type. */
UINT8 poi_unpackGlobalPOITypeIconName(char *output,
                                      TGenericMapInfo *const mapHeader,
                                      const UINT32 index);

/* To extract valid poi names for poi segments they must be scanned by
 * poi_unpackSegment in the right order (starting from the beginning
 * of a particular poi layer).  poi_getSettlementPlaceNameSetIndex
 * determines the place name set index for the settlement by specifying
 * the place index of the settlement.
 *
 *   @param[in] mapInfo
 *   @param[in] settlementPlaceIndex (i.e. TPOISegment.place)
 */
TRoadPlaceNameSetIndex16
poi_getSettlementPlaceNameSetIndex( TGenericMapInfo   *mapInfo,
                                    TRoadPlaceIndex16  settlementPlaceIndex );

/* Gets the name of the POI map.  This is a succinct descriptor that
 * describes the map coverage.  E.g. "Greater Sydney".
 */
void poi_unpackMapName(char *const output,
                       const TGenericMapInfo *const mapInfo);

/* Given a POI content tag this function returns the text that
 * corresponds to the tag. */
UINT8 poi_findGlobalPOIContentTagName(UTF16 *output,
                                      TGenericMapInfo *const mapInfo,
                                      UINT8 tag);

/* Returns the index into the global data file of this data chunk. */
UINT32 poi_getExtendedChunkIndex(TGenericMapInfo *const mapInfo,
                                 UINT32 index);


/// @internal
///
/// \brief An iterator that loops on a POI sector returning POIs.
///
/// The iterator should be created and initialised to point to
/// the first element by poiIterator_first().
///
/// The iterator should be destroyed by
/// poiIterator_free().
///
/// Use poiIterator_next() and optionally
/// poiIterator_hasNext() to loop through the iterator.
///
///
/// @code
///    TPOIIterator sectorIterator;
///    TPOI poi;
///
///    poiIterator_first(&poiIterator, sector, header);
///    while (poiIterator_next(&poiIterator, &poi)) {
///      poiFunc(&poi); // do some stuff with the poi now we have it
///    } // end of loop over sector
///    assert(!poiIterator_hasNext(&poiIterator));
///    poiIterator_free(&poiIterator);
///
/// @endcode

typedef struct TPOIIterator {
  UINT32 poiOffset;
  UINT32 poiEnd;

  TGenericMapInfo *pMapInfo;
  TPOISectorHeader *pSector;
  BOOL8 sectorLocked;

  UINT8* pNextContent; // in mem address of next content data
} TPOIIterator;


void poiIterator_first(TPOIIterator *pThis,
                       TPOISectorHeader *pSector,
                       TGenericMapInfo *const pMapInfo);
BOOL8 poiIterator_next(TPOIIterator *pThis,
                       TPOI *pPOI);
BOOL8 poiIterator_hasNext(TPOIIterator *pThis);
void poiIterator_free(TPOIIterator *pThis);

///
/// @internal
///
/// \brief An iterator that loops on a POI super sector returning
/// sectors.
///
/// The iterator should be created and initialised to point to
/// the first element by poiSectorIterator_first().
///
/// The iterator should be destroyed by
/// poiSectorIterator_free().
///
/// Use poiSectorIterator_next() and optionally
/// poiSectorIterator_hasNext() to loop through the iterator.
///
///
/// @code
///    TPOISectorIterator sectorIterator;
///    TPOISectorHeader   *sector;
///
///    poiSectorIterator_first(&sectorIterator, superSector, header);
///    while (poiSectorIterator_next(&sectorIterator, &sector)) {
///        sector_func(sector); // do something with the sector
///    }
///    assert(!poiSectorIterator_hasNext(&sectorIterator));
///    poiSectorIterator_free(&sectorIterator);
///
/// @endcode

typedef struct TPOISectorIterator {
  UINT32 index;
  UINT32 sectorCount;
  UINT32 pSectorVa;
  TPOISectorHeader *pSectorPa;
} TPOISectorIterator;

void poiSectorIterator_first(TPOISectorIterator *pThis,
                             TPOISuperSectorHeader *pSuperSector,
                             TGenericMapInfo *const pMapInfo);

BOOL8 poiSectorIterator_next(TPOISectorIterator *pThis,
                             TPOISectorHeader** ppSector);
BOOL8 poiSectorIterator_hasNext(TPOISectorIterator *pThis);
void poiSectorIterator_free(TPOISectorIterator *pThis);


/// @internal
///
/// \brief An iterator that loops on a map and returns the super sectors
///        for a given map layer.
///
/// The iterator should be created and initialised to point to
/// the first element by poiSuperSectorIterator_first().
///
/// The iterator should be destroyed by
/// poiSuperSectorIterator_free().
///
/// Use poiSuperSectorIterator_next() and optionally
/// poiSuperSectorIterator_hasNext() to loop through the iterator.
///
/// @code
///    TPOISuperSectorIterator superSectorIterator;
///    TPOISuperSectorHeader *superSector;
///
///    poiSuperSectorIterator_first(&superSectorIterator, header, 0);
///    while (poiSuperSectorIterator_next(&superSectorIterator, &superSector)) {
///        superSector_func(superSector); // do something with super sector
///    }
///    assert(!poiSuperSectorIterator_hasNext(&superSectorIterator));
///    poiSuperSectorIterator_free(&superSectorIterator);
///
/// @endcode
typedef struct TPOISuperSectorIterator {
  UINT32 index;
  UINT32 superSectorCount;
  UINT32 pSuperSectorVa;
  TPOISuperSectorHeader *pSuperSectorPa;
} TPOISuperSectorIterator;

void poiSuperSectorIterator_first(TPOISuperSectorIterator *pThis,
                                  TGenericMapInfo *const pMapInfo,
                                  const UINT32 mapLayer);

BOOL8 poiSuperSectorIterator_next(TPOISuperSectorIterator *pThis,
                          TPOISuperSectorHeader** ppSuperSector);
BOOL8 poiSuperSectorIterator_hasNext(TPOISuperSectorIterator *pThis);
void poiSuperSectorIterator_free(TPOISuperSectorIterator *pThis);

BOOL8 poi_initTokenDataSource(TTokenDataSource   *const tokenSource,
                              TGenericMapInfo    *const mapInfo,
                              const TPOINameType  nameType);


typedef struct TPOIRoadSectorSearchKey{
  UINT8 *tableAddress;
  UINT16 roadSector;
} TPOIRoadSectorSearchKey;


#define POI_NAME_SSECTOR_ITERATOR_INSTANCE_COMBO_BUFFER_SIZE 32

typedef struct TPOINameInstanceIterator {
  TGenericMapInfo              *mapInfo;
  const TBitset                *sectorBitset;
  const TBitset                *placeBitset;
  UINT32                        poiNameId;
  UINT32                        chunkIdx;
  TPOIInstanceComboIndex32      instanceCombos[POI_NAME_SSECTOR_ITERATOR_INSTANCE_COMBO_BUFFER_SIZE];
  UINT16                        instanceComboCount;
  UINT16                        instanceComboItem;
  BOOL8                         complete;
} TPOINameInstanceIterator;

typedef struct TPOINameInstanceResult {
  TRoadPlaceIndex16  placeIndex;
  TPOISectorIndex16  sectorIndex;
  TPOITypeSetIndex16 typeSetIndex;
  UINT16             instanceCount;
} TPOINameInstanceResult;

BOOL8 poi_initNameInstanceIterator(
  TPOINameInstanceIterator *iterator,
  TGenericMapInfo           *mapInfo,
  const TBitset             *sectorBitset,
  const TBitset             *placeBitset,
  const UINT32               globalPOINameId);

BOOL8 poi_unsafeUnpackNextNameInstanceCombo(
  TPOINameInstanceIterator *iterator,
  TPOINameInstanceResult   *output );

UINT32 poi_setupPOINameFilter(
  TGenericMapInfo  *mapInfo,
  const TBitset    *sectorFilter,
  const TBitset    *placeFilter,
  const TBitset    *typeFilter,
  TBitset          *nameFilterBitset);

BOOL8 poi_unpackEntryPoint(
  const TPOI  *poi,
  TRoadSide   *sideOfLine,
  UINT32      *proportion,
  UINT32      *segmentId );

BOOL8 poi_unsafeUnpackEntryPoint(const TPOI       *poi,
                                 TPOISectorHeader *sector,
                                 TGenericMapInfo  *const mapInfo,
                                 TRoadSide        *sideOfLine,
                                 UINT32           *proportion,
                                 UINT32           *segmentId);

void poi_calculateSectorBounds( TRect                   *bbox,
                                const TPOISectorHeader  *sector );
void poi_calculateSuperSectorBounds( TRect                       *bbox,
                                     const TPOISuperSectorHeader *ssector );

BOOL8
poi_getPOISectorBitsetForRoadSector(TBitset               *poiBitset,
                                    TGenericMapInfo       *const poiMapInfo,
                                    const TGenericMapInfo *const roadMapInfo,
                                    const UINT32           roadSector);

typedef struct TPOISectorToRoadSectorTable {
  UINT32               poiSectorCount;
  UINT32              *poiSectorRoadSectorCounts;
  TRoadSectorIndex16 **poiSectorRoadSectors;
  TAllocatorHandle     allocator;
} TPOISectorToRoadSectorTable;

BOOL8
poi_calcPOISectorToRoadSectorTable(TPOISectorToRoadSectorTable *poiSectorToRoadSectorTable,
                                   TGenericMapInfo             *poiMapInfo,
                                   TAllocatorHandle             allocator);

void
poi_destroyPOISectorToRoadSectorTable(TPOISectorToRoadSectorTable *poiSectorToRoadSectorTable);

void
poi_getMapLocation( const TPOI *poi,
                    TMapLocation *mapLocation );

BOOL8
poi_findOnLayer0( UINT32           sourceId,
                  TGenericMapInfo *mapInfo,
                  UINT32          *output );


UINT32 poi_getNameSetCount(
  TGenericMapInfo *mapInfo);

BOOL8 poi_unpackNameSet(
  TPOINameSet        *nameSet,
  TPOINameSetIndex32  nameSetIndex,
  TGenericMapInfo    *mapInfo,
  TAllocatorHandle    allocator);

BOOL8 poi_unpackNameSetNoAllocate(
  TPOINameSet        *nameSet,
  TPOINameSetIndex32  nameSetIndex,
  TGenericMapInfo    *mapInfo,
  TAllocatorHandle    allocator);

void poi_freeUnpackedNameSet(
  TPOINameSet *nameSet);

void poi_getNameSetDisplayNames(
  TPOINameIndex32     *POIName1,
  TPOINameIndex32     *POIName2,
  TGenericMapInfo     *mapInfo,
  TPOINameSetIndex32   POINameSetIndex);

void poi_unpackNameSetDisplayNames(
  UTF16               *POIName1,
  UTF16               *POIName2,
  TGenericMapInfo     *mapInfo,
  TPOINameSetIndex32   POINameSetIndex,
  BOOL8                abbreviate);


UINT32 poi_getTypeCount(
  TGenericMapInfo *mapInfo);

TPOITypeId16 poi_getTypeIdForTypeIndex(
  TPOITypeIndex16   typeIndex,
  TGenericMapInfo  *mapInfo);

BOOL8 poi_findTypeIndexForTypeId(
  TPOITypeIndex16  *typeIndex,
  TPOITypeId16      typeId,
  TGenericMapInfo  *mapInfo);


UINT32 poi_getTypeSetCount(
  TGenericMapInfo *mapInfo);

BOOL8 poi_unpackTypeSet(
  TPOITypeSet        *typeSet,
  TPOITypeSetIndex16  typeSetIndex,
  TGenericMapInfo    *mapInfo,
  TAllocatorHandle    allocator);

void poi_freeUnpackedTypeSet(
  TPOITypeSet *typeSet);

void poi_unpackDisplayIconNames(
  char               *iconNames,
  TPOITypeSetIndex16  typeSetIndex,
  TGenericMapInfo    *mapInfo);

void poi_calcTypeSetFilterFromTypeFilter(
  TBitset         *typeSetFilterBitset,
  const TBitset   *typeFilterBitset,
  TGenericMapInfo *mapInfo);


UINT32 poi_getNameSetTypeSetCount(
  TGenericMapInfo *mapInfo);

void poi_unpackNameSetTypeSet(
  TPOINameSetTypeSet        *nameSetTypeSet,
  TPOINameSetTypeSetIndex32  nameSetTypeSetIndex,
  TGenericMapInfo           *mapInfo);

void poi_calcNameSetFilterFromTypeSetFilter(
  TBitset         *nameSetFilterBitset,
  const TBitset   *typeSetFilterBitset,
  TGenericMapInfo *mapInfo);


UINT32 poi_getSectorPlaceCount(
  TGenericMapInfo *mapInfo);

void poi_unpackSectorPlace(
  TPOISectorPlace        *sectorPlace,
  TPOISectorPlaceIndex16  sectorPlaceIndex,
  TGenericMapInfo        *mapInfo);


UINT32 poi_getInstanceComboCount(
  TGenericMapInfo *mapInfo);

void poi_unpackInstanceCombo(
  TPOIInstanceCombo        *instanceCombo,
  TPOIInstanceComboIndex32  instanceComboIndex,
  TGenericMapInfo          *mapInfo);


BOOL8 poi_unpackNameInstanceComboSet(
  TPOIInstanceComboSet *instanceComboSet,
  TPOINameIndex32       nameIndex,
  TGenericMapInfo      *mapInfo,
  TAllocatorHandle      allocator);

void poi_freeUnpackedInstanceComboSet(
  TPOIInstanceComboSet *instanceComboSet);


BOOL8 poi_findPOINameRange( TGenericMapInfo *mapInfo,
                            const UTF16     *identityName,
                            UINT32          *firstNameId,
                            UINT32          *lastNameId );


/** @param[in] pointer to a POI */
typedef void TGenericPOIFunc(TPOI *pPOI);
UINT32 poi_loopOnAll(TGenericPOIFunc *poiFunc);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* ENPOIP_H */
