#ifndef ENPITYPFILEP_H
#define ENPITYPFILEP_H

#include <mapdata/enmapinfoP.h>
#include <mapdata/enpoitypeP.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 140 - spirit */
#define POITYPEFILE_MAJOR_VERSION 140
#define POITYPEFILE_MINOR_VERSION 2
#define POITYPEFILE_GLOBAL_NAME "0000"

/** loads a global type map chunk directory */
TMapfileStatus   
  poitypefile_loadChunkDir   (TGenericMapInfo *const mapInfo);

/** loads the global data for a global type map */
TMapfileStatus   
  poitype_load               (TGenericMapInfo *const mapInfo);


/** find the best match POI type map for a global type id with the
 *  aid of a country ISO code, this allows loading country specific
 *  POI type overwrites for a country, by default it returns the
 *  "world's" version, which must be loaded at all times */
TGenericMapInfo *
  poitype_findBestMatchMapForGlobalType (
    UINT16                 globalId, 
    TNumericISOCountryCode country);

UINT32
poitypefile_lockTypeNameType(TGenericMapInfo *const mapInfo,
                             const UINT32 blocknr);

UINT32
poitypefile_unlockTypeNameType(TGenericMapInfo *const mapInfo,
                               const UINT32 blocknr);

#ifdef __cplusplus
}
#endif

/* ENPITYPFILEP_H */
#endif
