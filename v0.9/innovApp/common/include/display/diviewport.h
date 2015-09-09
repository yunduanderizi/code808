#ifndef DIVIEWPORT_H
#define DIVIEWPORT_H

#include <innovEngine_types.h>
#include <graphics/grcontext.h>
#include <display/digeopen.h>
#include <display/diroadstyle.h>
#include <display/dipoitypelist.h>
#include <display/diiconranklist.h>
#include <display/didoorplatenum.h>
#include <display/difonts.h>
#include <display/di3Dengi.h>
#include <display/dionewaybuffer.h>
#include <display/dilod.h>
#include <display/disettlementnameset.h>
#include <display/diZBuffer.h>
#include <display/dielevation.h>
#include <traffic/trapi.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Display
/// @{

/// @addtogroup Viewport
/// @ingroup Display
/// @{


///
/// @name Public Types and Constants
/// @{
///

/// Maximum map size in metre (see Viewport_GetMapSize and Viewport_SetMapSize)
#define MAX_MAP_SIZE ( (UINT32)(1280L * 1280L) * 4 )

/// Minimum map size in metre (see Viewport_GetMapSize and Viewport_SetMapSize)
#define MIN_MAP_SIZE ( (UINT16)200 )

///
/// The type of viewport
///
typedef enum TViewportType
{
  /// A 2D viewport where object should be rendered with a simplified representation
  Viewport2D,
  /// A 3D viewport where normal transformation and projection rules apply
  Viewport3D
} TViewportType;

///
/// Represents a viewport object.
/// All fields in this structure are private.
/// Never read or overwrite these fields directly since MapCore may
/// change field names without notice. Always use accessor functions.
/// Inform MapCore if an accessor function does not exist yet and should be exposed.
///
typedef struct TViewport
{
  /// The type of the viewport
  TViewportType type;
  /// A reference to the underlying 2D rasterization object used by the viewport
  TGraphContext *graphContext;
  /// A reference to the active geometric pen
  TGeometricPen *geoPen;
  /// The latest world coordinates given to either Viewport_MoveTo or Viewport_LineTo.
  TPos currentPos;
  /// The latest world coordinates position. See Viewport_MoveTo and Viewport_LineTo.
  TPos currentProjectedPos;
  /// The active set of clipping planes.
  const TPlaneIdx *clipPlanes;

  /*
   * record the start time at beginning of display to be able to display
   * FPS (frame per second) at the end of map display
   */
  UINT32 fpsStartTime;

  struct TViewport2D
  {
    // map rotation
    INT32 cosa,sina;
    // rotation angle 0-255 = 0-360 degrees, 0 degrees north up
    UINT8 angle; // = 0;
    // map stretching in horizontal and vertical directions (convert map -> metres)
    INT32 xsf,ysf;
    // inverse (screen to map) scale factors (convert metres -> map)
    INT32 ixsf,iysf;
    // forward transformation matrix
    INT32 a11,a12,a21,a22;
    // reverse transformation matrix
    INT32 b11,b12,b21,b22;

    /*
    * Number of bits to left shifts world coordinates in order to avoid
    * fixed point arithmetics overflows but still keep an acceptable precision
    */
    INT32 shifts;

    // map origin
    INT32 xm; // = 0
    INT32 ym; // = 0

    // current size of the map (metres)
    INT32 mapSize;

    /*
    * Coefficient which depends on mapSize and which is used 
    * to alter map projection at very high zoom level (world level)
    * to avoid rescaling map in X as we pan in latitude.
    */
    INT32 expMapSize;

    // these variables determine the origin of the map
    // bitmap which has it's top left corner at (0,0) and it's
    // bottom right corner at (mapWidth_s - 1,-mapHeight_s + 1)
    INT32 mapXOrigin; // = 160;
    INT32 mapYOrigin; // = 120;

    // current display width
    UINT32 mapWidth_s;  // = 320;
    UINT32 mapHeight_s;  // = 240;
    // display width in lat/lon. Added by James Ng, 05/06/03.
    INT32 mapWidth_LatLon;
    INT32 mapHeight_LatLon;
    /* Smallest non rotated rectangle which includes the entire visible region */
    TRect visibleRect;

    // Computed members
    /* initialise these with some values just in case they are used before they are initialised
       in display_init */
    UINT32 display_size; //  = 320; /* largest value of mapWidth_s or mapHeight_s */
  } v2D;

  struct TViewport3D // TODO: v2D/v3D should be declared within a union
  {
    TCamera camera; // shouldn't be a pointer - we should have an actual camera here
  } v3D;

  struct TOneWayArrowBuffer oneWayBuffer;
  struct TDetailOfLevel lod;
  struct TRoadDisplayTheme roadDisplayTheme;
  IRoadTheme *roadTheme;
  struct TSettlementNameSet settlementNameSet;
  struct TDisplayNameSet displayNameSet;
  struct TPOITypeFilter poiTypeFilter;
  struct TIconRankSet iconRankSet;

  /**
  * Add by Shadow in order to deposit doorplate number information for subsequently displaying.
  */
  struct TDoorPlateNumSet doorPlateNumSet;

  struct TFontPalette fontPalette;
  struct TDisplayList displayList;
  struct TPolyDisplayTheme polyDisplayTheme;
  struct TColourPalette colourPalette;
  struct TElevationPalette elevationPalette;
  struct TPolygonDynDepthBuffer polygonDynDepthBuffer;
  UINT32 modifyTransactionCount;
  /// application configuration of event classes to display or not
  BOOL8 trafficEventClassDisplay[TRAFFIC_APP_NUM_EVENT_CLASSES];

  // journey flag display properties
  const TBitmap *journeyStartFlag;
  const TBitmap *journeyWaypointFlag;
  RGBCOLOUR journeyWaypointFlagColour;
  RGBCOLOUR journeyWaypointFlagColourShadow;
  TPos journeyWaypointFlagShadowOffset;
  TPos journeyWaypointFlagNumberLocation;
  const TBitmap *journeyEndFlag;

  TRoadType roadMaxDisplayType;

  /// Enable road blocked icons if true
  BOOL8 isRoadBlockIconEnabled;

  /// Scale factor used when blitting POI icons
  UINT32 poiScaleFactor;
  /// Alpha transparency used when blitting POI icons
  UINT8 poiAlpha;

  /// TRUE if we are currently rendering a 2D city map
  BOOL8 cityMapVisible;

  /// If cityMapVisible is TRUE then this holds the 
  /// bounding box of the currently visible 2D City map
  TRect cityMapBounds;

  /// Pattern used to fill the area outside the map, or NULL to specify a solid 
  /// fill with DISPLAY_COLOUR_END_OF_WORLD.
  const TBitmap *endOfWorldPattern;

  /// TRUE if 3D landmarks are in "ghost" mode
  BOOL8 alphaBuildings;

  /// TRUE if you don't want to cluster POIs, but want to have the most important
  /// been picked
  BOOL8         disablePoiClusters;

  ///@TPE map will not display any icons except search result if true
  BOOL8 isOnlyShowSearchResultIcon;
} TViewport;

///
/// Specifies how map lines are rendered through viewport
/// map-oriented rendering methods.
/// See Viewport_LineTo for more information.
///
typedef enum VIEWPORT_RENDERLINEFLAGS
{
  /// Render everything except the segment geometry for a line segment 
  RENDERLINE_IGNOREROADS = 1,
  /// Render the line and register it consequently in the viewport's display list for
  /// labeling purposes.
  RENDERLINE_REGISTERINDISPLAYLIST = 2,
  /// @internal
  /// Force the usage of screen thickness and not map thickness
  /// For instance, this flag is used for rendering road segments that are
  /// at a medium distance to the observer.
  /// This is an internal flag and should not be used by application code.
  RENDERLINE_USEPIXELTHICKNESS = 4,
  /// Render the line and assume that start and end projected map coordinates
  /// fit in the viewport and do not require further clipping.
  /// -not used-
  RENDERLINE_ZEROLENGTHLINES = 8
//,RENDERLINE_ASSUMECLIPPED = 8
} VIEWPORT_RENDERLINEFLAGS;


/// @} 

/// @}

struct TSettlementNameSet;
struct TDisplayNameSet;
struct TRoadDisplayTheme;
struct TPOITypeFilter;
//@}


extern MAPCORE_API
void Viewport_Create(TViewport *pThis, TGraphContext *graphContext);

extern MAPCORE_API
void Viewport_Init(TViewport *pThis);

extern MAPCORE_API
void Viewport_Create2D(TViewport *pThis,TGraphContext *graphContext,TPos *mapOrigin,UINT32 mapSize);

extern MAPCORE_API 
void Viewport_Create3D(TViewport *pThis,TGraphContext *graphContext,TPos *mapOrigin,UINT32 observationDistance,UINT32 verticalAngle);

extern MAPCORE_API
void Viewport_ConvertMetersToMap(const TViewport *pThis, INT32 *xs, INT32 *ys);

extern MAPCORE_API
void Viewport_ConvertMapToMeters(const TViewport *pThis, INT32 *xs, INT32 *ys);

extern MAPCORE_API
UINT32 Viewport_GetMaximumSize(const TViewport *pThis);

extern MAPCORE_API
TGraphContext *Viewport_GetGraphContext(const TViewport *pThis);

extern MAPCORE_API
TColourPalette *Viewport_GetColourPalette(TViewport *pThis);

extern MAPCORE_API
TFontPalette *Viewport_GetFontPalette(TViewport *pThis);

extern MAPCORE_API
INT32 Viewport_ForwardScale(const TViewport *pThis, UINT8 n);

extern MAPCORE_API
INT32 Viewport_BackwardScale(const TViewport *pThis, UINT8 n);

extern MAPCORE_API
void Viewport_BeginRender(TViewport *pThis);

extern MAPCORE_API
void Viewport_EndRender(TViewport *pThis);

extern MAPCORE_API
void Viewport_MapToScreen(const TViewport *pThis, const TPos *mapPos, TPos *screenPos);

extern MAPCORE_API
void Viewport_MapToScreen3D_c(const TViewport *pThis,
                              const TPos *pos_w, TPos *pos_s);

extern MAPCORE_API
void Viewport_ScreenToMap(const TViewport *pThis, const TPos *screenPos, TPos *mapPos);

extern MAPCORE_API
UINT32 Viewport_GetObservationDistance(const TViewport *pThis);

extern MAPCORE_API
void Viewport_SetObservationDistance(TViewport *pThis, UINT32 dist);

extern MAPCORE_API
FLOAT64 Viewport_GetVerticalAngle(const TViewport *pThis);

extern MAPCORE_API
void Viewport_SetVerticalAngle(TViewport *pThis, FLOAT64 va);

// Is this really public???
extern MAPCORE_API
void Viewport_SetMapOrigin(TViewport *pThis, TPos *vertex);

extern MAPCORE_API
void Viewport_SetMapAngle(TViewport *pThis, UINT8 newAngle);

extern MAPCORE_API
TIconRankSet *Viewport_GetIconRankSet(TViewport *pThis);

extern MAPCORE_API
struct TPolyDisplayTheme *Viewport_GetPolyTheme(TViewport *pThis);

extern MAPCORE_API
struct TDisplayNameSet *Viewport_GetDisplayNameSet(TViewport *pThis);

extern MAPCORE_API
void   Viewport_SetMapSize(TViewport *pThis, UINT32 newSize);

extern MAPCORE_API
UINT32 Viewport_GetMapSize(const TViewport *pThis);

extern MAPCORE_API
void Viewport_SetSize(TViewport *pThis, const TPos *size);

extern MAPCORE_API
void Viewport_GetSize(const TViewport *pThis, TPos *size);

extern MAPCORE_API
void Viewport_SetOrigin(TViewport *pThis, TPos *origin);

extern MAPCORE_API
void Viewport_GetOrigin(const TViewport *pThis, TPos *origin);

extern MAPCORE_API
void Viewport_SetDimensions(TViewport *pThis, UINT32 width, UINT32 height);

extern MAPCORE_API
void Viewport_SetAngle(TViewport *pThis, UINT8 angle);

extern MAPCORE_API
void   Viewport_PanScreen(TViewport *pThis, TPos *vertex);

extern MAPCORE_API
UINT8  Viewport_GetMapAngle(const TViewport *pThis);

extern MAPCORE_API
void   Viewport_GetMapOrigin(const TViewport *pThis, TPos *vertex);

extern MAPCORE_API
BOOL8  Viewport_IsVisiblePos(const TViewport *pThis, TPos *pos);

extern MAPCORE_API
BOOL8 Viewport_IsVisibleRect(const TViewport *pThis, const TRect *r_w);

extern MAPCORE_API
const TRect *Viewport_GetVisibleRect(const TViewport *pThis);

extern MAPCORE_API
void Viewport_SetMapAngle(TViewport *pThis, UINT8 newAngle);

extern MAPCORE_API
TElevationPalette *Viewport_GetElevationPalette(TViewport *pThis);

extern MAPCORE_API
void Viewport_SetElevationPalette(TViewport *pThis, 
                                  const TElevationPaletteEntry ep[], UINT32 n);

extern MAPCORE_API
TDetailOfLevel *Viewport_GetLOD(TViewport *pThis);

extern MAPCORE_API
IRoadTheme *Viewport_GetRoadTheme(TViewport *pThis);

extern MAPCORE_API
void Viewport_SetRoadTheme(TViewport *pThis, IRoadTheme *);

extern MAPCORE_API
void Viewport_SetTrafficEventClassDisplay(TViewport *pThis,
                                          TAppTrafficEventClass eventClass,
                                          BOOL8 enable);

extern MAPCORE_API
BOOL8 Viewport_IsTrafficEventClassDisplayed(const TViewport *pThis,
                                            TAppTrafficEventClass eventClass);

extern MAPCORE_API
const TIconDisplayRank *Viewport_SelectIcon(const struct TViewport *pThis,
                                            const TPos             *screenPos);

extern MAPCORE_API
BOOL8 Viewport_SetDisplayMode(TViewport *pThis, TViewportType mode);

extern MAPCORE_API
TViewportType Viewport_GetDisplayMode(const TViewport *pThis);


extern MAPCORE_API
TPOITypeFilter *Viewport_GetPOITypeFilter(TViewport *pThis);

extern MAPCORE_API
TRoadType Viewport_GetMaxVisibleRoadType(const TViewport *pThis, const TPos *screenPos);

extern MAPCORE_API
void Viewport_CalcBoundingParameters(const TViewport *pThis,
                                     TPos *centroid,
                                     UINT32 *size,
                                     const TRect *bbox);

extern MAPCORE_API
void Viewport_SetEndOfWorldPattern(TViewport *pThis, const TBitmap *pattern);

///@TPE enable/disable only show search result icons
extern MAPCORE_API
void Viewport_EnableOnlySearchResultIcons(struct TViewport *pThis, BOOL8 enable);

///@TPE check whether only show search result icons
extern MAPCORE_API
BOOL8 Viewport_IsEnabledOnlySearchResultIcons(struct TViewport *pThis);

extern MAPCORE_API
const TIconDisplayRank *Viewport_SelectCluster(struct TViewport *pThis,
                                               const TPos             *screenPos);

extern MAPCORE_API
const TIconDisplayRank *Viewport_SelectNearCluster(struct TViewport *pThis,
                                               const TPos             *screenPos);
extern MAPCORE_API
void Viewport_DisablePoiClusters(
  TViewport *pThis,
  BOOL8     disablePoiClusters );

#ifdef __cplusplus
}
#endif

#endif
