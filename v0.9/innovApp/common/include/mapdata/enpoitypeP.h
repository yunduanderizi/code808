#ifndef ENPOITYPEP_H
#define ENPOITYPEP_H

/* Private POI type file data structures and functions */

#include <mapdata/enpoitype.h>
#include <mapdata/englobalP.h>
#include <mapdata/entokenP.h>
#include <mapdata/enmapinfoP.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup MapData
///    @{

typedef UINT16 TPoiTypeNameIndex16;
typedef UINT16 TPoiTypeNameSetIndex16;

typedef struct TBinPoiTypeGlobals {
  UINT8         flags;        /* bit flags: hierachy info? */
} TBinPoiTypeGlobals;

typedef struct TBinPoiTypeGlobals TPoiTypeMapGlobals;

typedef struct TPoiType {
  /* id and flag parent */
  BITFIELD32 id        :15;
  BITFIELD32 flagParent:1;

  /* parent id, if flagParent set */
  BITFIELD32 parent    :16;

  /* name set and flag brand type */
  BITFIELD32 nameSetId :15; 
  BITFIELD32 flagBrand :1;

  /* type icon index */
  BITFIELD32 icon      :16;

  /* calculate size */
  UINT8      size;
} TPoiType;

/* header for token chunk */
typedef struct TBinTypeTokenHeader {
  UINT8      sizeHeader;
  UINT8      languageCodes[3];
  UINT32     tokPartBlobSz;  

  UINT16     implicitSepTokenId;
  UINT16     sepTokenCount;

  /* Offset of each section in chunk (0 means not populated) */
  UINT16     offsetMostFrequent;
  UINT16     offsetTokenParts;
  UINT16     offsetExactSpIdx;
  UINT16     offsetPhonemeIdx;
  UINT16     offsetEquivSpell;
  UINT16     offsetDataBlob;

  /* Count of each section */
  UINT16     mostFreqTokenCount;
  UINT16     tokenPartCount;
  UINT16     equivSpellCount;
  UINT16     altTokenCount;
} TBinTypeTokenHeader;

/* Name type for tokens and names */
typedef enum {
  POITYPE_NAME_TYPE_TYPENAME
} TPoiTypeNameType;

/* @todo: share with road place name sets/poi name sets/poi type sets, etc */

#define POITYPE_NAME_SET_BUFFER_SIZE (sizeof(TAllocatorHandle)/sizeof(TPoiTypeNameIndex16))

/** @note this structure cannot be moved or copied */
typedef struct TPoiTypeNameSet {
  TPoiTypeNameIndex16   *names;
  UINT16                 officialNameCount;
  UINT16                 officialExonymCount;
  UINT16                 alternateNameCount;
  union {
    TAllocatorHandle     allocator;
    TPoiTypeNameIndex16  names[POITYPE_NAME_SET_BUFFER_SIZE];
  }                      _buffer;
} TPoiTypeNameSet;
/* @todo: end of todo */

#define POITYPE_TYPE_SET_BUFFER_SIZE (sizeof(UINTP)/sizeof(UINT16))

/** @note this structure cannot be moved or copied */
typedef struct TPoiTypeTypeSet {
  UINT16             *types;
  UINT16              typeCount;
  union {
    TAllocatorHandle  allocator;
    UINT16            types[POITYPE_TYPE_SET_BUFFER_SIZE];
  }                   _buffer;
} TPoiTypeTypeSet;

#define POITYPE_CHUNK_COUNT_NAME_SET         1
#define POITYPE_CHUNK_COUNT_TYPE_NAME_TYPE   1
#define POITYPE_CHUNK_COUNT_TOKENISED_NAME   8
#define POITYPE_CHUNK_COUNT_TOKEN            8
#define POITYPE_CHUNK_COUNT_TYPE_ICON        1
#define MAX_NAME_SIZE                       70

typedef enum TPoiTypeChunkIndex {
  POITYPE_CHUNK_IDX_POI_TYPE,                       /*  0 */
  POITYPE_CHUNK_IDX_TYPE_ICON,                      /*  1 */
  POITYPE_CHUNK_IDX_TYPE_ICON_INDEX          =      
    POITYPE_CHUNK_IDX_TYPE_ICON +
    POITYPE_CHUNK_COUNT_TYPE_ICON,                  /*  2 */
  POITYPE_CHUNK_IDX_TYPE_NAME_SET_HEADER     =
    POITYPE_CHUNK_IDX_TYPE_ICON_INDEX +
    POITYPE_CHUNK_COUNT_TYPE_ICON,                  /*  3 */
  POITYPE_CHUNK_IDX_TYPE_NAME_SET,                  /*  4 */
  POITYPE_CHUNK_IDX_TYPE_NAME_TYPE           =
    POITYPE_CHUNK_IDX_TYPE_NAME_SET +
    POITYPE_CHUNK_COUNT_NAME_SET,                   /*  5 */
  POITYPE_CHUNK_IDX_TOKENISED_TYPE_NAME      =
    POITYPE_CHUNK_IDX_TYPE_NAME_TYPE +
    POITYPE_CHUNK_COUNT_TYPE_NAME_TYPE,             /*  6 */
  POITYPE_CHUNK_IDX_TYPE_TOKEN               =      /* 14 */
    POITYPE_CHUNK_IDX_TOKENISED_TYPE_NAME +
    POITYPE_CHUNK_COUNT_TOKENISED_NAME,
  POITYPE_CHUNK_IDX_TOKEN_HEADER             =      /* 22 */
    POITYPE_CHUNK_IDX_TYPE_TOKEN          +
    POITYPE_CHUNK_COUNT_TOKEN,
  POITYPE_CHUNK_IDX_TOKEN_HAS_ALT,                  /* 23 */
  POITYPE_CHUNK_IDX_ALT_TOKEN,                      /* 24 */
  POITYPE_CHUNK_IDX_COUNT                           /* 25 */
} TPoiTypeChunkIndex;

/* A MapHeader is an interpretation of the chunk directory table which
 * makes available symbolic names to refer to the first few
 * chunks. Macros such as <maptype>_CHUNK_ADDRESS allow us to
 * translate these names in the mapHeader into chunk-directory
 * indices. */
typedef struct TPoiTypeMapHeader {
  /* These chunks at the start of the chunk directory */
  TChunkFptr poiTypes;
  TChunkFptr typeIcons;
  TChunkFptr typeIconIndex[POITYPE_CHUNK_COUNT_TYPE_ICON];
  TChunkFptr typeNameSetHeader;
  TChunkFptr typeNameSet[POITYPE_CHUNK_COUNT_NAME_SET];
  TChunkFptr typeNameType[POITYPE_CHUNK_COUNT_TYPE_NAME_TYPE];
  TChunkFptr tokenisedTypeName[POITYPE_CHUNK_COUNT_TOKENISED_NAME];
  TChunkFptr tokenHeader;
  
  /* tokenised type name, tokens, alternate, token parts, etc.
   * .
   * .
   * . 
   */
} TPoiTypeMapHeader;

#define POITYPE_GLOBALS(mapInfo)                            \
  (assert(((mapInfo))->mapType == MAPTYPE_POITYPE),         \
   ((TPoiTypeMapGlobals *)((mapInfo)->_globals)))

#define POITYPE_CHUNK_INDEX(chunkName)                                  \
  ((offsetof(TPoiTypeMapHeader, chunkName)) / sizeof(TChunkFptr))

#define POITYPE_CHUNK_SIZE_MEM(mapInfo, chunkName)              \
  CHUNK_SIZE_MEM(mapInfo, POITYPE_CHUNK_INDEX(chunkName))
#define POITYPE_CHUNK_IS_EMPTY(mapInfo, chunkName)              \
  CHUNK_IS_EMPTY(mapInfo, POITYPE_CHUNK_INDEX(chunkName))


BOOL8 
poitype_init (void);

///
/// \brief Initializes a TTokenDataSource object which is required
///        when interacting with tokenised names. This allows the
///        token processing logic to be independent of the map
///        file. Each map file is required to implement
///        it's own init function
/// @param      tokenSource[out] Pointer to the object to populate
/// @param         mapHeader[in] The poi type map header.
///
/// @return TRUE if the object was successfully initialised.
///
BOOL8 
poitype_initTokenDataSource (
      TTokenDataSource       *const tokenSource,
      TGenericMapInfo        *const mapInfo,
      const TPoiTypeNameType  nameType);

BOOL8
poitype_unpackType (
    TPoiType        *output,
    TGenericMapInfo *const mapInfo,
    UINT16           typeId);

INT32
poitype_unpackIconName (
    char            *output,
    TGenericMapInfo *const mapInfo,
    UINT16           index);

/* unpack global type name in current language, else return default */
INT32
poitype_unpackTypeName (
    UTF16           *output,
    TGenericMapInfo *const mapInfo,
    UINT16           index);

/* unpack global type name set for a global type */
UINT8
poitype_unpackTypeNameSet (
    TPoiTypeNameSet        *nameSet,
    TPoiTypeNameSetIndex16  nameSetIndex,
    TGenericMapInfo        *mapInfo,
    TAllocatorHandle        allocator);

void
poitype_freeUnpackedTypeNameSet(TPoiTypeNameSet *nameSet);

BOOL8
poitype_unpackTypeNameTypeSet(
  TPoiTypeTypeSet     *typeSet,
  TPoiTypeNameIndex16  typeNameIndex,
  TGenericMapInfo     *mapInfo,
  TAllocatorHandle     allocator);

void
poitype_freeUnpackedTypeSet(
  TPoiTypeTypeSet *typeSet);
//@MIC
BOOL8 poitype_getMaplayer(const char *);

/// @}

#ifdef __cplusplus
}
#endif

/* ENPOITYPEP_H */
#endif
