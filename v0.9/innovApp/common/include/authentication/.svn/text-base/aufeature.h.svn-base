#ifndef AUFEATURE_H
#define AUFEATURE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  MAP_FEATURE_CLASS_FULL,
  MAP_FEATURE_CLASS_LOW,
  MAP_FEATURE_CLASS_COUNT  
} TMapFeatureClass;

typedef enum {
  MAP_FEATURE_LANE_INFO,
  MAP_FEATURE_BRAND_ICONS,
  MAP_FEATURE_SPEED_LIMITS,
  MAP_FEATURE_SIGNPOSTS,
  MAP_FEATURE_HIGHEND_POLY,
  MAP_FEATURE_COUNT
} TMapFeature;

typedef struct TMapFeatureArray
{
  TMapFeature mapFeature[MAP_FEATURE_COUNT];
  UINT8 fCount;  
} TMapFeatureArray;


extern  MAPCORE_API BOOL8 mapfeature_Configure(TMapFeatureArray featureArray);

#ifdef __cplusplus
}
#endif

#endif // AUFEATURE_H
