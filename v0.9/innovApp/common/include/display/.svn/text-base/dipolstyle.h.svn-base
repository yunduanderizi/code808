#ifndef DIPOLSTYLE_H
#define DIPOLSTYLE_H

#include <graphics/grbitmap.h>
#include <display/dicolour.h>
#include <display/dielevation.h>
#include <graphics/grcolour.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

/// @addtogroup GeometricPrimitives
/// @ingroup UnUsed
/// @{

/// @addtogroup Polygon
/// @ingroup GeometricPrimitives
/// @{

/** Type of polygon rendering */
typedef enum TPolKind {
  POL_SOLID,
  POL_PATTERN,
  POL_TEXTURED,
  /* Same as above but also draw thin lines for polygon borders */
  POL_SOLID_WITH_BORDER,
  POL_PATTERN_WITH_BORDER,
  POL_TEXTURED_WITH_BORDER
} TPolKind;

/** Type of an entry in polygon style palette.  Polygon can either
    be filled with a solid colour or with a bitmap pattern.
    Even if a pattern is specified, pattern filling can be globally
    enabled or disabled by calling display_enablePatternPol().
    So even if a polygon has a pattern, a fill solid colour still needs
    to be given.  Pattern can for example be used when display elevation
    polygons so parks and elevation can be visible simultaneously on
    top of each other.  Pattern can be typically disabled when elevation
    display is disabled so parks (and other polygons classes) are filled
    with solid colour which looks sharper than a pattern */
typedef struct TPolStyle
{
  /** solid or pattern polygon, draw borders? */
  TPolKind kind;
  /** colour used for borders (if relevant) */
  RGBCOLOUR borderColour;
  /** colour if using solid polygon style. 
      A fill colour needs to be specified 
      even for pattern polygons as pattern
      fill may be globally disabled */
  RGBCOLOUR fillColour;
  /** seamless bitmap for pattern fill if poly
      rendered as pattern (not used for solid
      polygons */
  const TBitmap *pattern;
} TPolStyle;

/// @}
/// @}


/// @addtogroup Map
/// @{

/// @addtogroup Polygons
/// @ingroup Map
/// @{


///
/// Enumeration of all supported polygon class
/// @note Following polygon classes have entries in polygon style palette 
/// Existing numbers must now change when adding a new class or else
/// maps would have to be recreated.
///
typedef enum TPolClass
{
  /// The default polygon class. Reserved.
  POL_CLASS_DEFAULT        =  0,
  /// The park polygon class.
  POL_CLASS_PARK           =  1,
  /// The water polygon class.
  POL_CLASS_WATER          =  2,
  /// The building polygon class.
  POL_CLASS_BUILDING       =  3,
  /// The acedemic area polygon class.
  POL_CLASS_ACADEMIC       =  4,
  /// The airport runway polygon class.
  POL_CLASS_RUNWAY         =  5,
  /// The racecourse polygon class.
  POL_CLASS_RACECOURSE     =  6,
  /// The private polygon class.
  POL_CLASS_PRIVATE        =  7,
  /// The hospital polygon class.
  POL_CLASS_HOSPITAL       =  8,
  /// The pedestrian area polygon class.
  POL_CLASS_PEDESTRIAN     =  9,
  /// The wharf area polygon class.
  POL_CLASS_WHARF          = 10,
  /// The sand area polygon class.
  POL_CLASS_SAND           = 11,
  /// The moor polygon class.
  POL_CLASS_MOOR           = 12,
  /// The woodland polygon class.
  POL_CLASS_WOODLAND       = 13,
  /// The coverage polygon class. Reserved.
  POL_CLASS_COVERAGE       = 14,
  /// The airport polygon class..
  POL_CLASS_AIRPORT_GROUND = 15,
  /// The congestion area polygon class.
  POL_CLASS_CONGESTION     = 16,
  /// The golf course polygon class.
  POL_CLASS_GOLF_COURSE    = 17,
  /// The built-up-area polygon class.
  POL_CLASS_BUILT_UP_AREA  = 18,

  POL_CLASS_CITY_START         = 19,

  POL_CLASS_CITY_BUILDING_DEFAULT = 19,
  POL_CLASS_CITY_BUILDING_COMMERCIAL,
  POL_CLASS_CITY_BUILDING_CULTURAL,
  POL_CLASS_CITY_BUILDING_FACTORY,
  POL_CLASS_CITY_BUILDING_PUBLIC_OFFICE,
  POL_CLASS_CITY_BUILDING_HOSPITAL,
  POL_CLASS_CITY_BUILDING_HOTEL,
  POL_CLASS_CITY_BUILDING_MULTI_CARPARK,
  POL_CLASS_CITY_BUILDING_OTHER,
  POL_CLASS_CITY_BUILDING_PED_DECK,
  POL_CLASS_CITY_BUILDING_WORSHIP,
  POL_CLASS_CITY_BUILDING_RAILWAY_PLATFORM,
  POL_CLASS_CITY_BUILDING_RAILWAY,
  POL_CLASS_CITY_BUILDING_RAILWAY_UG,
  POL_CLASS_CITY_BUILDING_SCHOOL,
  POL_CLASS_CITY_BUILDING_SUBWAY_PLATFORM,
  POL_CLASS_CITY_BUILDING_SUBWAY,
  POL_CLASS_CITY_BUILDING_SUBWAY_UG,

  POL_CLASS_CITY_TBLOCK_DEFAULT,
  POL_CLASS_CITY_TBLOCK_SHORE,
  POL_CLASS_CITY_TBLOCK_DIVIDER,
  POL_CLASS_CITY_TBLOCK_FACTORY,
  POL_CLASS_CITY_TBLOCK_FOREST,
  POL_CLASS_CITY_TBLOCK_GARDEN_PATH,
  POL_CLASS_CITY_TBLOCK_GRASS,
  POL_CLASS_CITY_TBLOCK_GREEN_URBAN,
  POL_CLASS_CITY_TBLOCK_HOSPITAL,
  POL_CLASS_CITY_TBLOCK_MARSH,
  POL_CLASS_CITY_TBLOCK_PAVED,
  POL_CLASS_CITY_TBLOCK_RAILWAY,
  POL_CLASS_CITY_TBLOCK_SAND,
  POL_CLASS_CITY_TBLOCK_SCHOOL,
  POL_CLASS_CITY_TBLOCK_POOL,
  POL_CLASS_CITY_TBLOCK_WALKWAY,
  POL_CLASS_CITY_TBLOCK_WATER,

  POL_CLASS_CITY_ROADS,

  POL_CLASS_PALETTE_MAX,  /* Max number of entries in polygon class palette */
  /* Elevation class is a bit special (no entry in polygon style palette) */
  POL_CLASS_ELEVATION = 128,
  POL_CLASS_MAX,
  POL_CLASS_INVALID /* Invalid polygon class */
} TPolClass;

typedef enum TArcClass
{
  /// Line colour for land arcs
  ARC_CLASS_LAND           =  0,
  /// Line colour for water arcs
  ARC_CLASS_WATER          =  1,
  /// Line colour for rail arcs
  ARC_CLASS_RAIL           =  2,
  /// Line colour for pedestrian arcs
  ARC_CLASS_PEDESTRIAN     =  3,
  /// Line colour for boundary arcs
  ARC_CLASS_BOUNDARY       =  4,
  /// Line colour for ferry arcs
  ARC_CLASS_FERRY          =  5,
  /// Line colour for wharf arcs
  ARC_CLASS_WHARF          =  6,
  /// Line colour for proposed road arcs
  ARC_CLASS_PROPOSED_ROAD  =  7,
} TArcClass;

///
/// Represents a polygon display theme object
///
typedef struct TPolyDisplayTheme
{
  BOOL8 isPatternEnabled;
  BOOL8 polClassEnabled[POL_CLASS_MAX];
  /// Palette of polygon style for each polygon class
  TPolStyle polStyleTable[POL_CLASS_PALETTE_MAX];
  struct TElevationPalette *elevationPalette;
} TPolyDisplayTheme;

/// @}

/// @}

/// @}




extern MAPCORE_API
void PolyDisplayTheme_Init(TPolyDisplayTheme *pThis, TElevationPalette *elevationPalette);

extern MAPCORE_API
BOOL8 PolyDisplayTheme_IsClassEnabled(TPolyDisplayTheme *pThis, TPolClass polClass);

extern MAPCORE_API
void PolyDisplayTheme_EnableClass(TPolyDisplayTheme *pThis, TPolClass polClass, BOOL8 enable);

extern MAPCORE_API
TPolStyle *PolyDisplayTheme_GetClassStyle(TPolyDisplayTheme *pThis, TPolClass polClass);

extern MAPCORE_API
void PolyDisplayTheme_SetClassStyle(TPolyDisplayTheme *pThis, TPolClass polClass, const TPolStyle *style);

extern MAPCORE_API
void PolyDisplayTheme_EnablePattern(TPolyDisplayTheme *pThis, BOOL8 enable);

extern MAPCORE_API
BOOL8 PolyDisplayTheme_IsPatternEnabled(TPolyDisplayTheme *pThis);

extern MAPCORE_API
RGBCOLOUR PolyDisplayTheme_GetElevationColour(TPolyDisplayTheme *pThis, INT32 elevation);

extern MAPCORE_API
UINT32 PolyDisplayTheme_GetNumElevations(const TPolyDisplayTheme *pThis);

extern MAPCORE_API
void PolyDisplayTheme_GetElevationTable(const TPolyDisplayTheme *pThis,
                                                   UINT32 size,
                                                   TElevationPaletteEntry *ep);

#ifdef __cplusplus
}
#endif

#endif
