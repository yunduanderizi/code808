/*****************************************************************************
 *  (c) 2014 innov China limited. all rights reserved.
 *
 *  module name: enmapinfoP.h
 *
 *  author(s): rohit grover <rohit.grover@innov.co.nz>
 *
 *  description:
 *    This module contains the private interface for the struct
 *    TGenericMapInfo which holds the metadata for a map.
 *****************************************************************************/

#ifndef ENMAPINFOP_H
#define ENMAPINFOP_H

#include <math/gerect.h>
#include <mapdata/enmfile.h>
#include <mapdata/enmfileP.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAPINFO_MAX_BACKUP_FILENAME 128
#define MAPINFO_MAX_BASENAME MAX_BUNDLE_COMPONENT_BASENAME

#define MAPINFO_MAX_ROAD_MAPS     128
#define MAPINFO_MAX_POI_MAPS      1024   /* Modify the count of MAX_POI from 512 to 600 for there is more than 512 in CHS data. [2011/2/11]*/
#define MAPINFO_MAX_GEOM_MAPS     256
#define MAPINFO_MAX_POSTCODE_MAPS  16
#define MAPINFO_MAX_POITYPE_MAPS    1
#define MAPINFO_MAX_ARCHIVE_MAPS   32
#define MAPINFO_MAX_SEARCHINDEX_MAPS 1024  /* Modify the count of MAX_POI from 512 to 600 for there is more than 512 in CHS data. [2011/2/11]*/

/* Struct TGenericMapInfo stores a concise summary about a
 * map. Information contained here can be used to reload a map on
 * demand. */
typedef struct TGenericMapInfo {
  struct TGenericMapInfo *lruPrev; /* for use with the LRU Dlist */
  struct TGenericMapInfo *lruNext; /* for use with the LRU Dlist */

  char  *backupFilename; /* the full path of the backup map file */
  UINT32 offsetWithinBundle; /* for maps contained within bundle-maps, this
                              * is the offset within the bundle file of the
                              * start of the map's contents. */
  char   baseName[MAPINFO_MAX_BASENAME]; /* for maps
                        * contained within bundle-maps, this is the
                        * basename of the component; else this is the
                        * basename for the map. */

  TMapfileType mapType;
  UINT16 index;  /* The tuple <maptype, index> identifies a map.  This
                  * field supplies an index into a per-type
                  * translation table to support fast lookups of
                  * mapInfos. */
  UINT16 flags;

  UINT8  majorVersion;
  UINT8  minorVersion;

  UINT16 hashcode; /* to store the following fixed value:
                    *
                    *  +---.---.---.---.---.---.---.---.---.---.---.---+
                    *  |  mapIndex rotated by 4 bits   |    maptype    |
                    *  +---.---.---.---.---.---.---.---.---.---.---.---+
                    *
                    * This value is later used in computing mapcache hash
                    * indices, precomputing it here saves some cycles.
                    */

  UINT32 crc32;    /* crc32 of all map chunks */

  UINT32 mapStartVa; /* Virtual address of the start of the map */
  UINT32 mapEndVa;   /* Virtual address of the end of the map */

  UINT32 chunkCount;   /* number of chunks in the map file */
  UINT32 zeroStartIdx; /* start of range of zeroes in the 'counts' array */
                       /* 
                        * in CHN search index map, it means
                        * record count of mapping table 
                        */
  UINT32 zeroCount;    /* number of elided zeroes in the 'counts' array */
                       /* 
                        * in CHN search index map, it means
                        * record count of feature code table 
                        */

  UINT32 numLockedChunks;  /* #chunks of this map locked in memory */
  UINT32 numLoadedChunks;  /* #chunks of this map loaded in memory */

  TMapFeatureClass featureClass;
  /*
   * PRIVATE FIELDS; These should be accessed only through macros
   */
  void             *_globals;  /* globals specific to the map type */
  TChunkCountEntry *_counts;   /* array of counts; initialized during map load*/
} TGenericMapInfo;


#define MAPINFO_FLAG_INITIALIZED    ((UINT16)0x0001)

#define MAPINFO_ISVALID(mapInfo)                        \
  (((mapInfo) != NULL) &&                               \
   ((mapInfo)->mapType < NUM_MAPTYPES) &&               \
   ((mapInfo)->flags & MAPINFO_FLAG_INITIALIZED))

/* Initialize state of the mapInfo table */
extern BOOL8 mapinfo_init(void);

/* Free all memory in use by mapInfos. */
extern void mapinfo_shutdown(void);

/* Allocate an unused mapInfo entry. The allocated entry is placed on
 * the appropriate type-based linked lists and translation tables.
 *
 *   @param[in] mapType
 *          The type of the new map.
 */
extern TGenericMapInfo *mapinfo_allocEntry(const TMapfileType mapType);

/* Release a slot in the MapInfo table. Presently, we take the
 * simplistic view that a free of a mapinfo entry happens only when
 * the loading of a map during device initialization fails. This
 * permits us to assume that the entry being freed is the same as the
 * most recently allocated entry, keeping our accounting simple.
 *
 *   @param[in] mapInfo
 */
extern void
mapinfo_freeEntry(TGenericMapInfo * const mapInfo);

/* Marks a map as initialized; also, assigns it a slot in the per-type
 * index translation table.
 *
 *   @param[in] mapInfo
 *          The map.
 */
extern void
mapinfo_markInitialized(TGenericMapInfo * const mapInfo);


/* Resolves <mapType, mapName> to a mapInfo
 *
 *   @param[in] mapType
 *          The type of map.
 *   @param[in] mapName
 *          The full name of the map.
 *   @return TGenericMapInfo *
 *          The corresponding mapInfo
 */
extern TGenericMapInfo *
mapinfo_resolveMapName(const TMapfileType mapType,
                       const char *const mapName);

/* Resolves <mapType, mapIndex> to a mapInfo
 *
 *   @param[in] mapType
 *          The type of map.
 *   @param[in] mapIndex
 *          The index of the map.
 *   @return TGenericMapInfo *
 *          The corresponding mapInfo
 */
extern TGenericMapInfo *
mapinfo_resolveMapIndex(const TMapfileType mapType,
                        const UINT16 mapIndex);

/* Resolves <mapType, mapIndex> to a search mapInfo
 *
 *   @param[in] mapIndex
 *          The index of the map.
 *   @return TGenericMapInfo *
 *          The corresponding mapInfo
 */
extern TGenericMapInfo *
mapinfo_resolveSearchMapIndex(const UINT16 poiMapIndex);

/* Resolves <mapType, mapIndex> to it is validate or not
 *
 *   @param[in] mapType
 *          The type of map.
 *   @param[in] mapIndex
 *          The index of the map.
 *   @return BOOL8
 *          TRUE/FALSE
 */
extern BOOL8
mapinfo_validateMapIndex(const TMapfileType mapType,
                         const UINT16 mapIndex);

/* Gets the number of initialized maps of a certain type.
 *
 *   @param[in] mapType
 *          The type.
 */
extern UINT32
mapinfo_getMapTypeCount(const TMapfileType mapType);

/* Tries to find a map info of the given type that matches 
 * the CRC and version data
 *
 * @param[in] maptype       The type of the map
 * @param[in] mapName       The basename of the map
 * @param[in] majorVersion  Major version
 * @param[in] minorVersion  Minor version
 * @param[in] crc32         Checksum
 * @param[in] hintIndex     Hint at the index we think the map 
 *                          is located at (-1 if we don't know)
 * @return TGenericMapInfo pointer or NULL if none matched
 */
TGenericMapInfo *
mapinfo_findMatchingMap( TMapfileType  mapType, 
                         const char   *mapName, 
                         UINT8         majorVersion,
                         UINT8         minorVersion,
                         UINT32        crc32,
                         INT32         hintIndex );

#ifdef __cplusplus
}
#endif

#endif /* ENMAPINFOP_H */
