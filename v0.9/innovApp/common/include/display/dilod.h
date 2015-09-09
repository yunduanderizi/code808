#ifndef DILOD_H
#define DILOD_H

#include <mapdata/enroad.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BEGIN HACK */
struct TViewport;
/* END HACK */

/// @addtogroup Display


///
/// TDetailOfLevel contains various thresholds
/// which depends on the level of details.
///
typedef struct TDetailOfLevel {
  INT32 level;

  /*
   * Computed thresholds which depend on detail level (above value)
   * and sometimes camera parameters such as latitude of centre of
   * projection or distance of observation for example.
   */
  INT32 road_mapLayer0;
  INT32 road_mapLayer1;
  INT32 road_skipNetworkDepth;
  INT32 road_farOrCloseDepth;
  INT32 road_useShapePointLowDepth;
  INT32 road_useShapePointHighDepth;
  INT32 road_depthFactorThin;
  INT32 road_skipOneWayArrowDepth;
  INT32 road_maxVisibleDepth[ROAD_TYPES];

  INT32 settlement_mapLayer0Depth;
  INT32 settlement_mapLayer1Depth;
  INT32 settlement_mapLayer2Depth;

  INT32 poi_mapLayer0Depth;
  INT32 poi_mapLayer1Depth;
  INT32 poi_mapLayer2Depth;
  INT32 poi_maxDepth;
  
  INT32 poly_mapLayer0Depth;
  INT32 poly_mapLayer1Depth;
  INT32 poly_mapLayer2Depth;
  INT32 poly_level0Depth;
  INT32 poly_level1Depth;
  INT32 poly_level2Depth;
  
  INT32 arc_mapLayer0Depth;
  INT32 arc_mapLayer1Depth;
  INT32 arc_mapLayer2Depth;
  INT32 arc_mapLayer3Depth;
  INT32 arc_maxDepth;

  INT32 onewayarrow_maxDepth;
  INT32 turnarrow_maxDepth;
} TDetailOfLevel;



/// @}

extern MAPCORE_API
void LOD_GetRoadTypesVisibility(UINT32 detail, UINT32 *visibilityLimits);

extern MAPCORE_API
BOOL8 LOD_IsRoadTypeVisible(UINT32 detail, TRoadType roadType, UINT32 mapSize);

extern MAPCORE_API
TRoadType LOD_GetMaxVisibleRoadType(TDetailOfLevel *pThis, UINT32 boost, UINT32 mapSize);

extern MAPCORE_API
TRoadType LOD_GetMaxVisibleRoadTypeDistanceWise(TDetailOfLevel *pThis, INT32 distance);

extern MAPCORE_API
void LOD_Refresh(TDetailOfLevel *pThis, struct TViewport *viewport);

extern MAPCORE_API
INT32 LOD_GetLevel(TDetailOfLevel *pThis);

extern MAPCORE_API
void LOD_SetLevel(TDetailOfLevel *pThis, INT32 level);

#ifdef __cplusplus
}
#endif

#endif
