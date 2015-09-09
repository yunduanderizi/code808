#ifndef ENARCHIVEP_H
#define ENARCHIVEP_H

#include <mapdata/enmfile.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------- support for loading archive files -----------*/

#define MAX_ARCHIVE_COMPONENT_BASENAME 32  /* max size of the string storing
                                      basenames for components; we
                                      should have a multiple of 4 for
                                      this value. */

/* The following structure defines the entries making up the archive's index. */
typedef struct TArchiveIndexEntry {
  UINT32 startOffset;
  UINT32 endOffset;
  char   basename[MAX_ARCHIVE_COMPONENT_BASENAME];
} TArchiveIndexEntry;


#define ARCHIVEINFO_MAX_BASENAME 16 /* max basename of the archive file. */

/* An instance of the following structure is initialized for each
 * archive map loaded into mapcore. */
typedef struct TArchiveInfo {
  char  *backupFilename; /* the full path of the backup map file */
  UINT32 offsetWithinBundle; /* for maps contained within bundle-maps, this
                          * is the offset within the bundle file of the
                          * start of the map's contents. */
  char   baseName[ARCHIVEINFO_MAX_BASENAME]; /* for maps
                          * contained within bundle-maps, this is the
                          * basename of the component; else this is the
                          * basename for the map. */
  UINT32 numComponents;  /* number of chunks in the map file */
  UINT32 crc32;          /* crc32 of all archive data */
} TArchiveInfo;

#define ARCHIVEINFO_MAX_ENTRIES   16
#define INVALID_ARCHIVEINFO_INDEX UINT16_MAX

/* Resolves <archiveIndex> to a archiveInfo
 *
 *   @param[in] archiveIndex
 *          The index of the archiveMap.
 *   @return TArchiveInfo *
 *          The corresponding archiveInfo
 */
extern TArchiveInfo *
archive_resolveMapIndex(const UINT16 archiveIndex);

/* Gets the number of initialized archive maps. */
extern UINT32
archive_getMapCount(void);

/* Looks up an archive map based on the two-letter country code.
 *
 *   @param[in] countryCode
 *          The 2 letter country code.
 *
 *   @return an index identifying the archive map;
 *          We return INVALID_ARCHIVEINFO_INDEX if the map isn't found.
 */
extern UINT16
archive_lookupByCountryCode(const char *const countryCode);


/*
 * Given an archive map (identified through its archiveInfo) and a
 * componentIndex, this function looks up corresponding component in
 * the archive and returns information necessary to access it.
 *
 *   @param[in]  archiveInfo
 *   @param[in]  componentIndex
 *   @param[out] filenamePP
 *          The filename to read from in order to fetch the
 *          archive-component's data. The filename so returned is a
 *          weak reference; the caller SHOULD NOT attempt to free the
 *          pointer. Note: If the archive is containined in a bundle,
 *          the filename returned will be that of the containing
 *          bundle (with the offsets adjusted appropriately).
 *   @param[out] offsetP
 *          The offset into the containing file at which the
 *          component's data resides.
 *   @param[out] sizeP
 *          The size of the component's data.
 *
 *   @return MAPFILE_SUCCESS or MAPFILE_ERROR
 */
TMapfileStatus
archive_resolveComponentIndex(const TArchiveInfo *archiveInfo,
                              const UINT16   componentIndex,
                              const char   **const filenamePP,
                              UINT32        *const offsetP,
                              UINT32        *const sizeP);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ENARCHIVEP_H */
