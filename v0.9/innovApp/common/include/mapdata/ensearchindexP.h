#ifndef ENSEARCHINDEXP_H
#define ENSEARCHINDEXP_H

#include <mapdata/enmapinfoP.h>
#include <platform.h>
#include <mapdata//ensearchindex.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Returns the which bit of character(high/low) of specific character in current mapInfo. */
UINT8 searchindex_whichBit(TGenericMapInfo *const mapInfo,
                          UTF16           *const utfString);

UINT32 searchindex_getChunkCountMapping(TGenericMapInfo *const mapInfo);

UINT32 searchindex_getChunkCountFeature(TGenericMapInfo *const mapInfo);

UINT32 searchindex_getChunkCount(TGenericMapInfo *const mapInfo);

UINT32 searchindex_getGlobalChunkIndex(TGenericMapInfo *const mapInfo, UINT32 index);

UINT32 searchindex_getMappingChunkIndex(TGenericMapInfo *const mapInfo, UINT32 index);

UINT32 searchindex_getFeatureChunkIndex(TGenericMapInfo *const mapInfo, UINT32 index);

/* Get relative index base it's chunk in */
UINT32 searchindex_getGlobalRelativeIndex(TGenericMapInfo *const mapInfo, UINT32 chunkIndex, UINT32 index);

UINT32 searchindex_getMappingRelativeIndex(TGenericMapInfo *const mapInfo, UINT32 chunkIndex, UINT32 index);

UINT32 searchindex_getFeatureRelativeIndex(TGenericMapInfo *const mapInfo, UINT32 chunkIndex, UINT32 index);

BOOL8 searchindex_getMappingTable(TGenericMapInfo *const mapInfo, const UINT32 index, TSearchIndexMapping *siMapping);

BOOL8 searchindex_getFeatureTable(TGenericMapInfo *const mapInfo, UINT32 index, TSearchIndexFeature *siFeature);

UINT32 searchindex_getRecordCountInMappingChunk(TGenericMapInfo *const mapInfo, UINT32 chunkIndex);

UINT32 searchindex_getRecordCountInFeatureChunk(TGenericMapInfo *const mapInfo, UINT32 chunkIndex);

#ifdef __cplusplus
}
#endif

#endif /* ENSEARCHINDEXP_H */
