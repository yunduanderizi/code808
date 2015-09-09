#ifndef ENMFILEP_H
#define ENMFILEP_H

/* loaded chunk - is present in memory but may or may not be locked.
   unloaded chunk - is not present in memory and is always unlocked.
   locked chunk - is always present in memory.
   unlocked - data may or may not be loaded in memory.  If data is loaded it
              can be unloaded and replaced with other data. */

#include <mapdata/enmfile.h>
#include <mapdata/enmcacheP.h>
#include <io/geio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

#define VERSION_MAGIC_CHK 0xc0de0000

#define MAPHEADER_FLAG_BUNDLE 0x1 /** Bundle maps */

#define MAP_LAYOUT_VERSION 2 /* version for the map layout */

#define MAP_MAJOR_VERSION 140 //@TPE for RDS, Smart Search, Highway Info
#define MAP_MINOR_VERSION 1
#define MAP_MINOR_VERSION_NEWSK 2

#define FILEPOINTER_MASK    (((UINT32)1 << 31) - 1) /* based on TChunkFptr */
#define MAX_MAP_FILEPOINTER FILEPOINTER_MASK

typedef struct TMapFileHeader {
  UINT32 magic:16;
  UINT32 flags:4;
  UINT32 type:4;
  UINT32 layoutVersion:8;
  UINT8  majorVersion;
  UINT8  minorVersion;
  UINT16 spare;
  UINT32 epoch;  /* creation time; so map changes everytime it's created */
  UINT32 crc32;  /* crc32 of all map chunks. */
  UINT32 numChunks;
  UINT32 zeroStartIdx;  /* compressed range of zeroes in count field starts */
                        /* 
                         * in CHN search index map, it means
                         * record count of mapping table 
                         */
  UINT32 zeroCount;     /* number of zeroes in range */
                        /* 
                         * in CHN search index map, it means
                         * record count of feature code table 
                         */
  UINT32 rawMapSize;    /* uncompressed size of the map including headers */
} TMapFileHeader;

//@TPE
#pragma pack(push, 1)
typedef struct TSMapFileHeader {
  UINT32 magic:16;
  UINT32 flags:4;
  UINT32 type:4;
  UINT32 layoutVersion:8;
  UINT8  majorVersion;
  UINT8  minorVersion;
  UINT16 spare;
  UINT32 epoch;  /* creation time; so map changes everytime it's created */
} TSMapFileHeader;
#pragma pack(pop)
typedef struct TChunkFptr {
  UINT32 fptr:31;             /* chunk file pointer */
  UINT32 compressed:1;        /* Is the map compressed */
} TChunkFptr;

/* the values in the following enum should agree with the definition
   of loadFunctionTable[] */
typedef enum TChunkType {
  CHUNK_TYPE_ROAD_SECTOR,
  CHUNK_TYPE_ROAD_SUPER_SECTOR,
  CHUNK_TYPE_POI_SECTOR,
  CHUNK_TYPE_POI_SUPER_SECTOR,
  CHUNK_TYPE_GEOM_LOWER_SECTOR,
  CHUNK_TYPE_GEOM_UPPER_SECTOR,
  CHUNK_TYPE_INDETERMINATE,
} TChunkType;

/* Entries for the table describing the components of a bundle-map */
#define MAX_BUNDLE_COMPONENT_BASENAME 16
typedef struct TBundleComponentEntry {
  char   baseName[MAX_BUNDLE_COMPONENT_BASENAME]; /* basename of the component*/
  UINT32 epoch;
  UINT32 startOffset;
  UINT32 endOffset;
} TBundleComponentEntry;

struct TGenericMapInfo; // forward declaration

// Each chunk directory is followed by a parallel array of chunk counts,
// where each chunk count is three bytes
typedef UINT8 TChunkCountEntry[3];

// Define some helper macros to find the count field
#define CHUNKDIR_GET_INDEX(chunkDirType, chunk)                         \
  ((UINT32)(offsetof(chunkDirType, chunk) / sizeof(TChunkFptr)))

#define CHUNK_LOCKCOUNT(mapInfo, index)                         \
  (MAPCACHE_LOCKCOUNT(mapcache_lookup((mapInfo), (index))))

#define CHUNK_GET_COUNT(chunkDirType, mapInfo, chunk)                   \
  (getChunkCount(mapInfo, CHUNKDIR_GET_INDEX(chunkDirType, chunk)))

  // Since we have done away with the size field of
  // TChunk, we can calculate the size if (and only if) we
  // make the assumption that chunk directory entries are layed out
  // in order in an array. Then we can calculate the size
  // with a simple difference of consecutive fptrs.
  // Normally we'd make this a routine, but for performance
  // make it a macro. This can only be called for chunks that aren't
  // compressed. During map loading we need to know the file size, for
  // all chunks, the CHUNK_SIZE_DISK macro should be used for this
  // If it were a routine it's prototype would be.
  //   UINT32 chunk_size(const TChunk *pChunk);
  // where pChunk is the address of chunk dir entry.
#define CHUNK_SIZE_FILE(mapInfo, chunkIndex)                            \
  (assert((chunkIndex) <= (mapInfo)->chunkCount),                       \
   assert(mapfile_getChunkFptr(mapInfo, chunkIndex).compressed == 0),   \
   assert(mapfile_getChunkFptr(mapInfo, (chunkIndex + 1)).fptr >=       \
          mapfile_getChunkFptr(mapInfo, chunkIndex).fptr),              \
   (mapfile_getChunkFptr(mapInfo, ((chunkIndex) + 1)).fptr -            \
    mapfile_getChunkFptr(mapInfo, chunkIndex).fptr))

// This macro is identical to the one about but is ONLY used during map loading
// when the raw chunk size is needed. It should NOT be used during any innovEngine
// logic. Use CHUNK_SIZE_FILE and CHUNK_SIZE_MEM as required instead.
#define CHUNK_SIZE_DISK(mapInfo, chunkIndex)                            \
  (assert(mapfile_getChunkFptr(mapInfo, ((chunkIndex) + 1)).fptr >=     \
          mapfile_getChunkFptr(mapInfo, chunkIndex).fptr),              \
   (mapfile_getChunkFptr(mapInfo, ((chunkIndex) + 1)).fptr -            \
    mapfile_getChunkFptr(mapInfo, chunkIndex).fptr))

  // When we have files that are a different size
  // in memory from the size in the file (e.g., because
  // of compression) then we need to look at the header
  // of the actual data to get the in memory size.
#define CHUNK_SIZE_MEM(mapInfo, chunkIndex)                             \
  (assert(MAPCACHE_LOCKCOUNT(mapcache_lookup(mapInfo, chunkIndex))),    \
   ((mapfile_getChunkFptr(mapInfo, chunkIndex).compressed)?             \
    mapcache_getDataSize(mapcache_lookup(mapInfo, chunkIndex)):         \
    (mapfile_getChunkFptr(mapInfo, ((chunkIndex) + 1)).fptr -           \
     mapfile_getChunkFptr(mapInfo, chunkIndex).fptr)))

// TRUE if a given chunk is empty. FALSE otherwise
#define CHUNK_IS_EMPTY(mapInfo, chunkIndex)                             \
  (CHUNK_SIZE_DISK(mapInfo, chunkIndex) == 0)

/*----------------------------------------------------------------*/

/* Maximum number of load function callbacks */
#define LOAD_FUNCTION_TABLE_SIZE 8

typedef void TMapfileCallbackFunc(struct TGenericMapInfo *const mapInfo,
                                  UINT32 index,
                                  UINT32 layer, UINT32 indexWithinLayer,
                                  TBlockNotifyType notifyType);
extern TMapfileCallbackFunc *loadFunctionTable[LOAD_FUNCTION_TABLE_SIZE];

/*----------------------------------------------------------------*/

/* This function resolves a given mapInfo and chunkIndex pair into a
 * chunk-type (e.g. roadSectorHeader; POISuperSectorHeader etc.), and
 * when possible, this function also returns the layer and the index
 * within the layer for the chunk. */
TChunkType mapfile_resolveChunkType(struct TGenericMapInfo *const mapInfo,
                                    const UINT16            chunkIndex,
                                    UINT32                 *layerP,
                                    UINT32                 *indexWithinLayerP);

/*----------------------------------------------------------------*/

/* Ensures that the chunk data is loaded into RAM and locked.  If the
 * cache configuration is updated then this function calls the
 * appropriate callback handler to change any corresponding sector or
 * super sector header data.  returns the old locked count on exit.
 * if no memory can be found for the data then the application
 * terminates.
 */
UINT32 mapfile_lockChunk(struct TGenericMapInfo *const mapInfo,
                         UINT16                  chunkIndex);

/* Unlocks the map data chunk but does not unload it from memory. */
UINT32 mapfile_unlockChunk(struct TGenericMapInfo *const mapInfo,
                           UINT16                  chunkIndex);

#ifndef NO_DEPRECATED_ROUTING
/* This function returns the number of maps of a certain type available
   for loading.  mapType could be "road:" or "poly:". */
DEPRECATED(
UINT32 mapfile_mapCount(const char *mapType)
);
#endif // NO_DEPRECATED_ROUTING

/* This function returns the number of data chunks contained in a certain
   map file.  The map file concerned can contain any kind of data. */
UINT32 mapfile_chunkCount(const char *mapName);

/// loads the global chunk of a map into a buffer. This function
/// doesn't need the chunkDir to be present.
///
/// @param[in] mapInfo
///        mapInfo of the map file
/// @param[in] globals_buf
///        The buffer to be filled in with the contents of the global chunk
/// @param[in/out] sizep
///        The expected size of the global chunk. The actual size of
///        the data read is filled into this parameter before return.
///
/// @return MAPFILE_SUCCESS if successful
TMapfileStatus mapfile_loadGlobals(struct TGenericMapInfo *const mapInfo,
                                   void                   *const globals_buf,
                                   const UINT32            size);

/* Load the map header corresponding to the named mapfile.
 *
 * @param[in] mapName
 *        name of the backup map file.
 * @param[out] mapHeader
 *        the structure to be filled out
 * @param[in/out] componentTable
 *        For bundle-maps, this parameter is used to return the
 *        component table. Memory for this table should be freed by
 *        the caller. The caller may express a disinterest in
 *        receiving a component table for bundle-maps by passing a
 *        null into this parameter.
 */
IO_FILE *mapfile_loadHeader(const char             *const mapName,
                            const UINT32            offsetWithinBundle,
                            TMapFileHeader         *const mapFileHeader,
                            TBundleComponentEntry **componentTable);
OS_FILE 
mapfile_loadHeader_s(const char             *const mapName,
                     const UINT32            offsetWithinBundle);
/*
 * Loads the counts array for a map; called once during initialization.
 */
TMapfileStatus mapfile_loadCounts(struct TGenericMapInfo *const mapInfo);

/*
 * Resolves a <mapInfo, chunkIndex> into the corresponding
 * filePointer. The filepointer table is demand-loaded into memory if
 * previously absent.
 */
TChunkFptr mapfile_getChunkFptr(struct TGenericMapInfo *const mapInfo,
                                UINT16 chunkIndex);

///
/// Get the feature class for a given map
///
/// @return TMapFeatureClass for the map located at the specified mapInfo
///
#define mapfile_getFeatureClass( mapInfo ) mapInfo->featureClass;

/* Called once to initialise the map file module. */
BOOL8 mapfile_init(void);

/* This function gets executed each time the callback function needs to be called */
void mapfile_triggerCallback(const char *mapName, TMapfileType type,
                             TMapfileStatus status);

const char *mapfile_getBaseName(const char *const mapName);

UINT32 getChunkCount(struct TGenericMapInfo *const mapInfo,
                     const UINT16            chunkIndex);

UINT32 getExtChunkCount(struct TGenericMapInfo *const mapInfo,
                        const UINT16            chunkIndex );

const char *mapTypeToString(const TMapfileType mapType);
/*----------------------------------------------------------------*/

//@TPE Check whether smart search (smart keyboard and point address) map file format is correct
BOOL8 isSmartSearchFileOK(void);

UINT8 getSmartSearchFileCount(void);
UINT8 getAddressFileCount(void);
UINT8 getAddressNameFileCount(void);
BOOL8 getSmartMap(int index, UINT8 *path);
BOOL8 getAddressMap(int index, UINT8 *path);
BOOL8 getAddressNameMap(int index, UINT8 *path);

#ifdef __cplusplus
}
#endif

#endif /* ENMFILEP_H */
