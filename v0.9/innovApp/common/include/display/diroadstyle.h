#ifndef DIROADSTYLE_H
#define DIROADSTYLE_H

#include <innovEngine_types.h>
#include <innovEngine_mapdata.h>
#include <graphics/grcolour.h>
#include <mapdata/enroad.h>
#include <display/diobjmodel.h>
#include <display/disegment.h>
#include <display/dicolour.h>
#include <display/digeopen.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BEGIN HACK */
struct TViewport;
/* END HACK */

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

///
/// @addtogroup Map
/// @ingroup UnUsed
/// @{
///

/// @addtogroup Road
/// @ingroup Map
/// @{

/// @addtogroup RoadStyles
/// @ingroup Road
/// @{

///
/// Road rendering mode (native, route, current route, highlighted,
/// slow traffic, heavy traffic, blocked)
///
typedef enum ROADRENDERMODE
{
  /// Native
  ROADRENDERMODE_NATIVE,
  /// Route
  ROADRENDERMODE_ROUTELEG,
  /// Current route
  ROADRENDERMODE_CURRENTROUTELEG,
  /// Highlighted
  ROADRENDERMODE_HIGHLIGHT,
  /// Slow traffic
  ROADRENDERMODE_TRAFFIC_SLOW,
  /// heavy traffic
  ROADRENDERMODE_TRAFFIC_HEAVY,
  /// blocked due to traffic
  ROADRENDERMODE_TRAFFIC_BLOCKED,
  /// highway sepped < 40
  ROADRENDERMODE_TRAFFIC_HIGHWAY40,
  /// 40<= highway sepped < =60
  ROADRENDERMODE_TRAFFIC_HIGHWAY60,
  /// 60< highway sepped < =80
  ROADRENDERMODE_TRAFFIC_HIGHWAY80

} ROADRENDERMODE;

///
/// Road display categories.
/// As of today this is still not used, but should be used.
///
typedef enum TRoadDisplayCategory
{
  /// Toll roads
  ROADDISPLAYCATEGORY_TOLL = 1,
  /// Non driveable roads
  ROADDISPLAYCATEGORY_NONDRIVEABLE = 2,
  /// Freeways
  ROADDISPLAYCATEGORY_FREEWAY = 4,
  /// Unsealed roads
  ROADDISPLAYCATEGORY_UNSEALED = 8,
  /// Ferry routes
  ROADDISPLAYCATEGORY_FERRY = 16, 
  ROADDISPLAYCATEGORY_FORCE_32BITS = 0x7FFFFFFF
} TRoadDisplayCategory;

//#include "../componentmodel/objmodel.h"

/// @class IRoadTheme
/// Interface that application developers can implement
/// to extend MapCore's default road renderer.
/// It is possible to implement the interface in either C or C++.
/// The example that follows defines a custom renderer acting as a
/// simple passthrough, just calling the default viewport's road renderer methods,
/// but actually prevents antialiasing to occur by overriding the GetSegmentPen
/// method.
/// @code
/// //
/// // customrenderer.h
/// //
/// typedef struct TMyRoadTheme
/// {
///   // Declare as many interfaces as necessary here.
///   // Interfaces must be the first members of the object structure.
///
///   // IRoadTheme virtual function table declaration follows
///   ICOM_VFIELD_MULTI(IRoadTheme);
///
///   // TMyRoadTheme-specific declarations here
///   IRoadTheme *defaultTheme; // MapCore-provided default renderer
/// }
///
/// #ifdef __cplusplus
/// extern "C" {
/// #endif
///
/// void MyRoadTheme_Create(TMyRoadTheme *pThis, IRoadTheme *defaultTheme);
///
/// #ifdef __cplusplus
/// }
/// #endif
/// @endcode
/// Here follows the main implementation file:
/// @code
/// // customrenderer.c
/// #include "innovEngine.h"
///
/// static void MyRoadTheme_Reset(IRoadTheme *iface);
///
/// static void MyRoadTheme_SetRoadTypeColour(IRoadTheme *iface, TRoadType roadType, RGBCOLOUR colour);
/// 
/// // Virtual function table, shared by all instances of MyRoadTheme
/// static ICOM_VTABLE(IRoadTheme) MyRoadTheme_IRoadTheme_VTable =
/// {
///   ICOM_MSVTABLE_COMPAT_DummyRTTIVALUE // leave this here, please
///   // VTable initialization begins here, the order is of course critical!
///   MyRoadTheme_Reset,
///   MyRoadTheme_SetRoadTypeColour,
///   MyRoadTheme_SetRoadCategoryColour,
///   MyRoadTheme_GetRoadCategoryColour,
///   MyRoadTheme_Get2DThickness,
///   MyRoadTheme_Get3DThickness,
///   MyRoadTheme_RegisterPass,
///   MyRoadTheme_RenderLine,
///   MyRoadTheme_RegisterMode,
///   MyRoadTheme_RenderSegment,
///   MyRoadTheme_useShapePoints,
///   MyRoadTheme_GetSegmentPen,
///   MyRoadTheme_GetMaxVisibleRoadData
/// }
///
/// //
/// // Constructor of the MyRoadTheme custom renderer
/// //
/// void MyRoadTheme_Create(TMyRoadTheme *pThis, IRoadTheme *defaultTheme)
/// {
///   pThis->lpVtbl = &MyRoadTheme_IRoadTheme_VTable;
///   pThis->defaultTheme = defaultTheme;
/// }
///
/// static void MyRoadTheme_Reset(IRoadTheme *iface)
/// {
///   ICOM_THIS(IRoadTheme, iface); // declare 'pThis' as a pointer to TMyRoadTheme
///   IRoadTheme_Reset(pThis->defaultTheme); // call base implementation
/// }
///
/// static void MyRoadTheme_SetRoadTypeColour(IRoadTheme *iface, TRoadType roadType, RGBCOLOUR colour)
/// {
///   ICOM_THIS(IRoadTheme, iface); // declare 'pThis' as a pointer to TMyRoadTheme
///   IRoadTheme_SetRoadTypeColour(pThis->defaultTheme, roadType, colour); // call base implementation
/// }
///
/// static void MyRoadTheme_GetSegmentPen(IRoadTheme *iface, TGeometricPen *geoPen, struct TViewport *viewport)
/// {
///   ICOM_THIS(IRoadTheme, iface); // declare 'pThis' as a pointer to TMyRoadTheme
///   IRoadTheme_GetSegmentPen(pThis->defaultTheme, geoPen, viewport); // call base implementation
///   //
///   // Overwrite antialiasing flag in the graphics pen of the geometric pen
///   //
///   geoPen->pen.antialised = FALSE;
/// }
///
/// // Implement all other methods here
/// [...]
///
/// @endcode
/// Calling code consuming the customized passthrough renderer:
/// @code
/// #include "innovEngine.h"
/// 
/// TMyRoadTheme roadTheme;
/// 
/// void ReplaceRenderer(TViewport *viewport)
/// {
///   MyRoadTheme_Create(&roadTheme, Viewport_GetRoadTheme(viewport)); 
///   Viewport_SetRoadTheme(viewport, GETINTERFACE(&roadTheme, IRoadTheme));
/// }
/// @endcode
/// Except at construction time where the actual type of the object of course
/// needs to be known, calling code and even code within MapCore should NEVER
/// manipulate the ReplaceRenderer object directly but instead always work through
/// IRoadTheme. Otherwise that would defeat the purpose of having an interface
/// allowing implementations to be interchanged.
///

#undef MINTERFACE
#define MINTERFACE IRoadTheme
MDECLARE_INTERFACE(IRoadTheme)
{
  MBEGIN_INTERFACE

  MSTDMETHOD(Reset)(MTHIS) MPURE;
  MSTDMETHOD(SetRoadTypeColour)(MTHIS_ TRoadType roadType, RGBCOLOUR colour) MPURE;
  MSTDMETHOD(SetRoadCategoryColour)(MTHIS_ TRoadDisplayCategory rdCategory, RGBCOLOUR colour) MPURE;
  MSTDMETHOD_(RGBCOLOUR,GetRoadCategoryColour)(MTHIS_ TRoadDisplayCategory rdCategory) MPURE;
  MSTDMETHOD_(UINT8,Get2DThickness)(MTHIS_ TRoadType roadType) MPURE;
  MSTDMETHOD_(UINT8,Get3DThickness)(MTHIS_ TRoadType roadType) MPURE;
  MSTDMETHOD(RegisterPass)(MTHIS_ UINT8 pass, struct TViewport *viewport) MPURE;
  MSTDMETHOD_(BOOL8,RenderLine)(MTHIS_ struct TViewport *viewport,
                            TPos *v1,
                            TPos *v2,
                            TRoadSegment *segment,
                            SEGMENT_RENDERFLAGS renderFlags) MPURE;
  MSTDMETHOD(RegisterMode)(MTHIS_ ROADRENDERMODE renderMode) MPURE;
  MSTDMETHOD_(BOOL8,RenderSegment)(MTHIS_ TRoadSegment *segment,
                                       struct TViewport *viewport,
                                       SEGMENT_RENDERFLAGS renderFlags) MPURE;
  MSTDMETHOD_(BOOL8,useShapePoints)(MTHIS_ TRoadSegment *segment,
                                    struct TViewport *viewport) MPURE;
  MSTDMETHOD(GetSegmentPen)(MTHIS_ TRoadSegment *segment,
                                     TGeometricPen *geoPen,
                                     struct TViewport *viewport) MPURE;

  MSTDMETHOD_(BOOL8,GetMaxVisibleRoadData)(MTHIS_ struct TViewport *viewport,
                                              TRoadType *maxRoadType,
                                              TRoadNetworkType *maxRoadNetwork,
                                              TRoadMapLayer *roadMapLayer) MPURE;

  MEND_INTERFACE
};

// C virtual function call helper macros
#define IRoadTheme_Reset(p) \
      (p)->lpVtbl->Reset(p)
#define IRoadTheme_SetRoadTypeColour(p, roadType, colour) \
      (p)->lpVtbl->SetRoadTypeColour(p, roadType, colour)
#define IRoadTheme_SetRoadCategoryColour(p, rdCategory, colour) \
      (p)->lpVtbl->SetRoadCategoryColour(p, rdCategory, colour)
#define IRoadTheme_GetRoadCategoryColour(p, rdCategory) \
      (p)->lpVtbl->GetRoadCategoryColour(p, rdCategory)
#define IRoadTheme_Get2DThickness(p, roadType) \
      (p)->lpVtbl->Get2DThickness(p, roadType)
#define IRoadTheme_Get3DThickness(p, roadType) \
      (p)->lpVtbl->Get3DThickness(p, roadType)
#define IRoadTheme_RegisterPass(p, pass, viewport) \
      (p)->lpVtbl->RegisterPass(p, pass, viewport)
#define IRoadTheme_RenderLine(p, viewport, v1, v2, segment, renderFlags) \
      (p)->lpVtbl->RenderLine(p, viewport, v1, v2, segment, renderFlags)
#define IRoadTheme_RegisterMode(p, renderMode) \
      (p)->lpVtbl->RegisterMode(p, renderMode)
#define IRoadTheme_RenderSegment(p, segment, viewport, renderFlags) \
      (p)->lpVtbl->RenderSegment(p, segment, viewport, renderFlags)
#define IRoadTheme_useShapePoints(p, segment, viewport) \
      (p)->lpVtbl->useShapePoints(p, segment, viewport)
#define IRoadTheme_GetSegmentPen(p, segment, geoPen, viewport) \
      (p)->lpVtbl->GetSegmentPen(p, segment, geoPen, viewport)
#define IRoadTheme_GetMaxVisibleRoadData(p, viewport, roadType, networkType, mapLayer) \
      (p)->lpVtbl->GetMaxVisibleRoadData(p, viewport, roadType, networkType, mapLayer)

///
/// The default object responsible for rendering roads.
/// MapCore provides this object and it is the default road renderer
/// for all viewports.
///
typedef struct TRoadDisplayTheme
{
  ICOM_VFIELD_MULTI(IRoadTheme);

  UINT8 thePass;
  ROADRENDERMODE roadRenderMode;
  INT16 directionArrowDistance;

  RGBCOLOUR lookupRoadFillColours[ROAD_TYPES];
  RGBCOLOUR lookupRoadBorderColours[ROAD_TYPES];
  RGBCOLOUR lookupCurrentRouteColours[ROAD_TYPES];
  RGBCOLOUR lookupRouteColours[ROAD_TYPES];
  RGBCOLOUR lookupSelectColours[ROAD_TYPES];
  RGBCOLOUR lookupSlowTrafficColours[ROAD_TYPES];
  RGBCOLOUR lookupHeavyTrafficColours[ROAD_TYPES];
  RGBCOLOUR lookupBlockedTrafficColours[ROAD_TYPES];
  RGBCOLOUR lookupHighway40Colours[ROAD_TYPES];
  RGBCOLOUR lookupHighway60Colours[ROAD_TYPES];
  RGBCOLOUR lookupHighway80Colours[ROAD_TYPES];

  RGBCOLOUR lookupColourToll;
  RGBCOLOUR lookupColourFreeway;
  RGBCOLOUR lookupColourUnsealed;
  RGBCOLOUR lookupColourFerry;
  RGBCOLOUR lookupColourNonDriveable;

  // this variable is set to either lookupRoadFillColours or lookupRouteColours
  // depending on whether the route or the road network is being drawn
  RGBCOLOUR *lookupColours;

  // road line thicknesses as passed to graph_line are pre-calculated into
  // this array
  UINT8 lookupThicknessTable[ROAD_TYPES + 1];
  TColourPalette *colourPalette;
} TRoadDisplayTheme;

/// @}

/// @}

/// @}

/// @}

extern MAPCORE_API
void RoadStyles_Create(TRoadDisplayTheme *pThis, TColourPalette *colourPalette);

#ifdef __cplusplus
}
#endif

#endif
