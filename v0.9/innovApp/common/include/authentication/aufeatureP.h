#ifndef _AUFEATUREP_H_
#define _AUFEATUREP_H_

#include "innovEngine_types.h"
#include <authentication/aufeature.h>
#include <mapdata/enmfileP.h>
#include <mapdata/enmapinfoP.h>

// Various features within innovEngine that
// can be turned on or off based on map license
//typedef enum {
//  MAP_FEATURE_LANE_INFO,
//  MAP_FEATURE_BRAND_ICONS,
//  MAP_FEATURE_SPEED_LIMITS,
//  MAP_FEATURE_SIGNPOSTS,
//  MAP_FEATURE_HIGHEND_POLY,
//  MAP_FEATURE_COUNT
//} TMapFeature;

BOOL8 mapfeature_init( void );

BOOL8 mapfeature_isFeatureEnabled( const TGenericMapInfo *mapInfo, 
                                   TMapFeature            featureFlag );

#endif //_AUFEATUREP_H_
