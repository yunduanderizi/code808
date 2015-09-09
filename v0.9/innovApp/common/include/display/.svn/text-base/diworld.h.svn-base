#ifndef DIWORLD_H
#define DIWORLD_H

#include <math/gerect.h>
#include <graphics/grbitmap.h>
#include <innovEngine_types.h>
#include <mapdata/enpoi.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Maximum level at which roads are being displayed
 * Some maps have road layer 2 (new maps) whereas other
 * maps may only have road layer 0 & 1.  If layer 2 
 * is not populated, there is too much data at layer 1
 * to render roads efficiently enough at very high
 * altitude.  So maximum zoom level to render roads
 * depends whether road layer 2 is populated or not:
 * - use MAX_ROAD_SIZE if road layer 2 is populated
 * - use MAX_ROAD_SIZE_NO_LEVEL2 if road layer 2 is not populated
 */
#define MAX_ROAD_SIZE            1800000
#define MAX_ROAD_SIZE_NO_LEVEL2   600000

/* 
 * Do not display road labels anymore above this high
 * zoom level (they clutter display more than they are
 * useful at continent zoom level)
 */
#define MAX_ROAD_SIZE_LABEL       800000

/*
 * Don't render road thickness above this zoom level
 * to reduce road clutter at very high zoom level.
 * It also speeds up road rendering.
 */
#define MAX_ROAD_SIZE_ONE_PASS   80000

/*
 * Maximum level at which POIS are displayed
 * after the map gets bigger than this POIs turn off
*/
#define POI_MAX_SIZE 150000

//Edison add difeature_chn.cpp for china feature, please load it
#define PARSERBIN(renderParams, viewport, pBinFilename) ParserBin(renderParams, viewport, pBinFilename)


/// @addtogroup UnUsed
/// @ingroup Display
/// @{

///
/// @addtogroup Map
/// @ingroup UnUsed
/// @{
///

/// @addtogroup World
/// @ingroup Map
/// @{

///
/// @typedef enum WORLD_RENDERFLAGS
/// Enumerated values for rendering map layers.
///
typedef enum WORLD_RENDERFLAGS
{
  /// Render road layer
  WORLD_RENDERROADS           = 1,
  /// Render POI icons
  WORLD_RENDERPOIS            = 1 << 1,
  /// Render elevation polygons
  WORLD_RENDERELEVATION       = 1 << 2,
  /// Render Normal Polygons
  WORLD_RENDERPOLYS           = 1 << 3,
  /// Render 3D landmarks
  WORLD_RENDERLANDMARKS       = 1 << 4,         
  /// Render 2D City maps
  WORLD_RENDERCITYMAPS        = 1 << 5,         
  /// Render settlement names
  WORLD_RENDERSETTLEMENTNAMES = 1 << 6,
  /// Render one-way street icons (you must specify WORLD_RENDERROADS as well)
  WORLD_RENDERONEWAYICONS     = 1 << 7,
  /// Render blocked road icons (you must specify WORLD_RENDERROADS as well)
  WORLD_RENDERBLOCKROADICONS  = 1 << 8,
  /// Render road labels (you must specify WORLD_RENDERROADS as well)
  WORLD_RENDERROADLABELS      = 1 << 9,
  /// Render snail trails
  WORLD_RENDERSNAILTRAIL      = 1 << 10,
  /// Render Safety cameras
  WORLD_RENDERSAFETYCAM       = 1 << 11,
    ///@TPE  Render Traffic event icons
  WORLD_RENDERTRAFFIC         = 1 << 12,
  // Render Cluster
  WORLD_RENDERCLUSTER         = 1 << 13,
  // Render Extra Feature
  WORLD_RENDERFEATURE         = 1 << 14,
  ///@MIC Render Detail - Neo
  WORLD_RENDERDETAIL          = 1 << 15,
  /// Render doorplate.
  WORLD_RENDERDOORPLATE       = 1 << 16
} WORLD_RENDERFLAGS;

///
/// @def The default level of rendering
///
#define WORLD_RENDERDEFAULT ( WORLD_RENDERROADS | WORLD_RENDERPOIS | \
                              WORLD_RENDERELEVATION | WORLD_RENDERPOLYS | \
                              WORLD_RENDERSETTLEMENTNAMES | WORLD_RENDERONEWAYICONS | \
                              WORLD_RENDERROADLABELS | WORLD_RENDERSNAILTRAIL | \
                              WORLD_RENDERCITYMAPS | WORLD_RENDERLANDMARKS | \
                              WORLD_RENDERSAFETYCAM | WORLD_RENDERCLUSTER | \
                              WORLD_RENDERFEATURE | WORLD_RENDERTRAFFIC | WORLD_RENDERDETAIL)

///
/// @typedef enum WORLD_RENDERSTAGE
/// <B><I>To be documented...</I></B>
///
typedef enum WORLD_RENDERSTAGE
{
  /// Stage where routes and guidelists should be rendered
  RENDERSTAGE_ROUTES,
  /// Stage where the highlighted road should be rendered
  RENDERSTAGE_HIGHLIGHTED_ROADS,
  /// Stage where the snail trail will be rendered (has been done already at that time)
  RENDERSTAGE_TRAILS,
  /// Stage where the geobans should be rendered
  RENDERSTAGE_GEOBANS,
  /// Stage where the lat/long grid should be rendered
  RENDERSTAGE_LATLONG_GRID,
  /// Stage where the sky should be rendered
  RENDERSTAGE_SKY,
  /// Stage where favourites should be rendered
  RENDERSTAGE_FAVOURITES,
  /// Stage where the car icon should be rendered
  RENDERSTAGE_CARICON,
  /// Stage where raster map data (e.g. aerial photography) should be rendered
  RENDERSTAGE_MAP_TILES
} WORLD_RENDERSTAGE;

/// Indicates that the renderFlags member of the WORLD_RENDERPARAMS structure is valid
#define RENDERPARAMS_FLAGS    1

/// Indicates that the renderCallback member of the WORLD_RENDERPARAMS structure is valid
#define RENDERPARAMS_CALLBACK 2

/// Indicates that the context member of the WORLD_RENDERPARAMS structure is valid
#define RENDERPARAMS_CONTEXT  4

/// Indicates that the renderStage member of the WORLD_RENDERPARAMS structure is valid
#define RENDERPARAMS_STAGE  8

struct WORLD_RENDERPARAMS;

///
/// Callback function used when rendering
///
typedef BOOL8 TRenderCallback(struct WORLD_RENDERPARAMS *renderParams);

///
/// Allows the caller to control how the world is rendered.
/// See World_Render for more information.
/// 
typedef struct WORLD_RENDERPARAMS
{
  /// Indicates which members of this structure are valid
  UINT32 members;
  /// Layers to be rendered
  WORLD_RENDERFLAGS renderFlags;
  /// Callback function during rendering
  TRenderCallback *renderCallback;
  /// Pointer that will be passed as a parameter to the callback function everytime it is called
  void *context;
  /// A WORLD_RENDERSTAGE value that defines at which stage of the rendering the callback function is being called
  WORLD_RENDERSTAGE renderStage;
} WORLD_RENDERPARAMS;

/// @}

/// @}

/// @}

extern MAPCORE_API
void World_Render(WORLD_RENDERPARAMS *renderParams, struct TViewport *viewport);

extern MAPCORE_API
void World_GetBoundingRect(TRect *rect);

extern MAPCORE_API
TBitmap *World_FindPOIBitmap(const char* iconList);

//Edison add difeature_chn.cpp for china feature, please load it
extern MAPCORE_API 
void ParserBin(WORLD_RENDERPARAMS *renderParams, struct TViewport *viewport, const char* pFile);

extern MAPCORE_API
TPOIMapLayer World_GetCurrentPOIMapLayer(UINT32 mapSize, INT32 detailLevel);

extern MAPCORE_API
void World_RenderPreferredPath(struct TViewport *viewport, BOOL8 renderlabels, UINT32* segments, UINT32 segNum);

#ifdef __cplusplus
}
#endif

#endif

