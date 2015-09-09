#ifndef DICOLOUR_H
#define DICOLOUR_H

#include <graphics/grcolour.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Display
/// @{

/// @addtogroup ColourPalette
/// @ingroup Display
/// @{

///
/// Through this enumerated type colours are referenced by a unique
/// colour index into a colour number lookup table (the colour palette).
///
typedef enum TDisplayColours
{
  /** Transparent colour index. Reserved */
  DISPLAY_COLOUR_NONE_INDEX = -1,

  // 16 misc colour indices

  /** The colour used for route highlighting */
  DISPLAY_COLOUR_MISC_ROUTE_INDEX,
  /** This colour is used as the interior colour of a small box drawn
      if no POI icon can be found in the map */
  DISPLAY_COLOUR_MISC_POI_INDEX,
  /** This colour is used as the exterior colour of a small box drawn
      if no POI icon can be found in the map */
  DISPLAY_COLOUR_MISC_POI_OUTLINE_INDEX,
  /** Default colour to use for major road label borders if no rule
      is found */
  DISPLAY_COLOUR_MISC_MAJOR_LABEL_BORDER_INDEX,
  /** Default colour to use for major road label fill colour if no
      colour rule is found in the map */
  DISPLAY_COLOUR_MISC_MAJOR_LABEL_FILL_INDEX,
  /** Default colour to use for major road label text if no
      colour rule is found in the map */
  DISPLAY_COLOUR_MISC_MAJOR_LABEL_TEXT_INDEX,
  /** Colour used to highlight future turn arrows */
  DISPLAY_COLOUR_MISC_TURN_FUTURE_INDEX,
  /** June 29th, 2004 - TURN_HILIGHT IS OBSOLETE AND SHALL NO LONGER BE 
      USED */
  DISPLAY_COLOUR_MISC_TURN_HILIGHT_INDEX,
  /** Fill colour used for a box placed at the centres of level 0 settlements
      @par
      THIS COLOUR IS NOT USED AND MAY BE REALLOCATED */
  DISPLAY_COLOUR_MISC_SETTLEMENT_TOWN_INDEX,
  /** Fill colour used for a box placed at the centres of level 1 settlements
      @par
      THIS COLOUR IS NOT USED AND MAY BE REALLOCATED */
  DISPLAY_COLOUR_MISC_SETTLEMENT_CITY_INDEX,
  /** Fill colour used for a box placed at the centres of level 2 settlements
      @par
      THIS COLOUR IS NOT USED AND MAY BE REALLOCATED */
  DISPLAY_COLOUR_MISC_SETTLEMENT_MAJOR_CITY_INDEX,
  /** Fill colour used for a box placed at the centres of level 3 settlements
      @par
      THIS COLOUR IS NOT USED AND MAY BE REALLOCATED */
  DISPLAY_COLOUR_MISC_SETTLEMENT_NATIONAL_CITY_INDEX,
  /** Border colour used for a box placed at the centres of settlements
      @par
      THIS COLOUR IS NOT USED AND MAY BE REALLOCATED */
  DISPLAY_COLOUR_MISC_SETTLEMENT_OUTLINE_INDEX,
  /** This colour is used to fill turn arrows that are on the route behind 
      the current vehicle position. */
  DISPLAY_COLOUR_MISC_TURN_PAST_INDEX,
  /** The colour used to draw the currently selected road. */
  DISPLAY_COLOUR_MISC_ROAD_SELECT_INDEX,
  /** The colour of the small direction arrows drawn on top of the route. */
  DISPLAY_COLOUR_MISC_ROUTE_DIRECTION_INDEX,

  // 16 road colours (each has a border colour and a fill colour)

  /** road class 0 (motorway) fill colour */
  DISPLAY_COLOUR_ROAD_MOTORWAY_INDEX,
  /** road class 1 (national highway) fill colour */
  DISPLAY_COLOUR_ROAD_NATIONALHIGHWAY_INDEX,
  /** road class 2 (regional highway) fill colour */
  DISPLAY_COLOUR_ROAD_REGIONALHIGHWAY_INDEX,
  /** road class 3 (important major road) fill colour */
  DISPLAY_COLOUR_ROAD_IMPORTANTMAJOR_INDEX,
  /** road class 4 (major road) fill colour */
  DISPLAY_COLOUR_ROAD_MAJOR_INDEX,
  /** road class 5 (minor road) fill colour */
  DISPLAY_COLOUR_ROAD_MINOR_INDEX,
  /** road class 6 (lane) fill colour */
  DISPLAY_COLOUR_ROAD_LANE_INDEX,
  /** road class 7 (now redesignated as private roads) fill colour */
  DISPLAY_COLOUR_ROAD_PEDESTRIAN_INDEX,
  /** road class 0 (motorway) border colour */
  DISPLAY_COLOUR_ROAD_BORDER_MOTORWAY_INDEX,
  /** road class 1 (national highway) border colour */
  DISPLAY_COLOUR_ROAD_BORDER_NATIONALHIGHWAY_INDEX,
  /** road class 2 (regional highway) border colour */
  DISPLAY_COLOUR_ROAD_BORDER_REGIONALHIGHWAY_INDEX,
  /** road class 3 (important major road) border colour */
  DISPLAY_COLOUR_ROAD_BORDER_IMPORTANTMAJOR_INDEX,
  /** road class 4 (major road) border colour */
  DISPLAY_COLOUR_ROAD_BORDER_MAJOR_INDEX,
  /** road class 5 (minor road) border colour */
  DISPLAY_COLOUR_ROAD_BORDER_MINOR_INDEX,
  /** road class 6 (lane) border colour */
  DISPLAY_COLOUR_ROAD_BORDER_LANE_INDEX,
  /** road class 7 (now redesignated as private roads) border colour */
  DISPLAY_COLOUR_ROAD_BORDER_PEDESTRIAN_INDEX,

  // 16 arc classes

  DISPLAY_COLOUR_ARC_START,
  /** Line colour for land arcs */
  DISPLAY_COLOUR_ARC_LAND_INDEX = DISPLAY_COLOUR_ARC_START,
  /** Line colour for water arcs */
  DISPLAY_COLOUR_ARC_WATER_INDEX,
  /** Line colour for rail arcs */
  DISPLAY_COLOUR_ARC_RAIL_INDEX,
  /** Line colour for pedestrian arcs */
  DISPLAY_COLOUR_ARC_PEDESTRIAN_INDEX,
  /** Line colour for boundary arcs */
  DISPLAY_COLOUR_ARC_BOUNDARY_INDEX,
  /** Line colour for ferry arcs */
  DISPLAY_COLOUR_ARC_FERRY_INDEX,
  /** Line colour for wharf arcs */
  DISPLAY_COLOUR_ARC_WHARF_INDEX,
  /** Line colour for proposed road arcs */
  DISPLAY_COLOUR_ARC_PROPOSED_ROAD_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_ARC_CLASS8_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_ARC_CLASS9_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_ARC_CLASS10_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_ARC_CLASS11_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_ARC_CLASS12_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_ARC_CLASS13_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_ARC_CLASS14_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_ARC_CLASS15_INDEX,

  // 16 more misc classes

  /** This colour is used for highlighting toll roads */
  DISPLAY_COLOUR_MISC_ROAD_TOLL_INDEX,
  /** This colour is used for highlighting controlled access roads (freeways) */
  DISPLAY_COLOUR_MISC_ROAD_FREEWAY_INDEX,
  /** This colour is used for highlighting unsealed or poor quality roads */
  DISPLAY_COLOUR_MISC_ROAD_UNSEALED_INDEX,
  /** This colour is used for highlighting roads that cannot be driven on */
  DISPLAY_COLOUR_MISC_ROAD_NON_DRIVEABLE_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_MISC_ROAD_FERRY_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_MISC_CLASS21_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_MISC_CLASS22_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_MISC_CLASS23_INDEX,
  /** This colour is used for highlighting toll road borders */
  DISPLAY_COLOUR_MISC_ROAD_BORDER_TOLL_INDEX,
  /** This colour is used for highlighting controlled access road (freeway) borders */
  DISPLAY_COLOUR_MISC_ROAD_BORDER_FREEWAY_INDEX,
  /** This colour is used for highlighting unsealed or poor quality road borders */
  DISPLAY_COLOUR_MISC_ROAD_BORDER_UNSEALED_INDEX,
  /** This colour is used for highlighting borders of roads that cannot be driven on */
  DISPLAY_COLOUR_MISC_ROAD_BORDER_NON_DRIVEABLE_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_MISC_ROAD_BORDER_FERRY_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_MISC_CLASS29_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_MISC_CLASS30_INDEX,
  /** NOT CURRENTLY ALLOCATED */
  DISPLAY_COLOUR_MISC_CLASS31_INDEX,

  /** Colour used to highlight current turn arrow */
  DISPLAY_COLOUR_MISC_TURN_CURRENT_INDEX,

  // Traffic colours

  /** Colour of road segments with slow traffic */
  DISPLAY_COLOUR_TRAFFIC_SLOW_INDEX,
  /** Colour of road segments with heavy traffic */
  DISPLAY_COLOUR_TRAFFIC_HEAVY_INDEX,
  /** Colour of road segments that are blocked to traffic */
  DISPLAY_COLOUR_TRAFFIC_BLOCKED_INDEX,
  /** Colour of chevrons on traffic affected segments */
  DISPLAY_COLOUR_TRAFFIC_CHEVRONS_INDEX,
  /** Colour of highway with speed <40 */
  DISPLAY_COLOUR_TRAFFIC_HIGHWAY_40_INDEX,
  /** Colour of highway with <=40 speed <60 */
  DISPLAY_COLOUR_TRAFFIC_HIGHWAY_60_INDEX,
  /** Colour of highway with <=60 speed <80 */
  DISPLAY_COLOUR_TRAFFIC_HIGHWAY_80_INDEX,
  
  

  /** This colour is used for distant roads in 3D */
  DISPLAY_COLOUR_MISC_DISTANT_ROADS_IN_3D_INDEX,

  // Non-highway road label colours

  /** Text colour for non-highway road labels */
  DISPLAY_COLOUR_LABEL_TEXT_INDEX,
  /** Fill colour for non-highway road labels */
  DISPLAY_COLOUR_LABEL_FILL_INDEX,
  /** 3D light colour for non-highway road labels */
  DISPLAY_COLOUR_LABEL_FRAME_INDEX,
  /** 3D dark colour for non-highway road labels */
  DISPLAY_COLOUR_LABEL_FRAME_SHADOW_INDEX,
  /** Anchor colour for non-highway road labels */
  DISPLAY_COLOUR_LABEL_ANCHOR_INDEX,
  /** Anchor border colour for non-highway road labels */
  DISPLAY_COLOUR_LABEL_ANCHOR_BORDER_INDEX,
  /** Text colour for settlement labels */
  DISPLAY_COLOUR_LABEL_SETTLEMENT_TEXT_INDEX,
  /** Fill colour for settlement labels */
  DISPLAY_COLOUR_LABEL_SETTLEMENT_FILL_INDEX,

  // Sky and fog colours

  /** Colour of top of sky gradient */
  DISPLAY_COLOUR_SKY_GRADIENT_UPPER_INDEX,
  /** Colour of top of sky gradient */
  DISPLAY_COLOUR_SKY_GRADIENT_LOWER_INDEX,

  /** Solid (roof) colour of textured 3D buildings */
  DISPLAY_COLOUR_TEXTURED_3D_BUILDING_INDEX,
  /** Solid colour of 3D buildings */
  DISPLAY_COLOUR_SOLID_3D_BUILDING_INDEX,
  /** Transparent colour of 3D buildings */
  DISPLAY_COLOUR_TRANSPARENT_3D_BUILDING_INDEX,
  /** Colour of wireframe 3D buildings */
  DISPLAY_COLOUR_WIREFRAME_3D_BUILDING_INDEX,

  /** Solid fill colour used instead of pattern in areas with no map coverage */
  DISPLAY_COLOUR_END_OF_WORLD_INDEX,

  // Debug colours

  /** Frames per second display text */
  DISPLAY_COLOUR_FPS_TEXT_INDEX,
  /** Frames per second display fill */
  DISPLAY_COLOUR_FPS_FILL_INDEX,
  /** GPS stats text colour */
  DISPLAY_COLOUR_GPS_STATS_TEXT_INDEX,
  /** GPS stats fill colour */
  DISPLAY_COLOUR_GPS_STATS_FILL_INDEX,
  /** GPS arrow fill colour */
  DISPLAY_COLOUR_GPS_ARROW_FILL_INDEX,
  /** GPS arrow border colour */
  DISPLAY_COLOUR_GPS_ARROW_BORDER_INDEX,

  /** The border colour for all direction arrows in 2D. */
  DISPLAY_COLOUR_MISC_TURN_BORDER_INDEX,
  /** one way arrow fill colour */
  DISPLAY_COLOUR_ONE_WAY_ARROW_FILL_INDEX,
  /** one way arrow border colour */
  DISPLAY_COLOUR_ONE_WAY_ARROW_BORDER_INDEX,
  /** lat/lon grid colour */
  DISPLAY_COLOUR_LAT_LON_GRID_INDEX,

  /** highway shield default text colour */
  DISPLAY_COLOUR_HIGHWAY_SHIELD_TEXT_INDEX,
  /** highway shield default border colour */
  DISPLAY_COLOUR_HIGHWAY_SHIELD_BORDER_INDEX,
  /** highway shield default fill colour */
  DISPLAY_COLOUR_HIGHWAY_SHIELD_FILL_INDEX,

  /** City map ARC colours */
  DISPLAY_COLOUR_CITY_START,
  DISPLAY_COLOUR_CITY_BUILDING_DECORATIVE = DISPLAY_COLOUR_CITY_START,
  DISPLAY_COLOUR_CITY_BUILDING_RIDGE,
  DISPLAY_COLOUR_CITY_TBLOCK_BORDER,
  DISPLAY_COLOUR_CITY_TBLOCK_SUB_DIV,
  DISPLAY_COLOUR_CITY_RAIL_DEFAULT,
  DISPLAY_COLOUR_CITY_RAIL_DEFAULT_UNDERGROUND,
  DISPLAY_COLOUR_CITY_RAIL_TRAM,
  DISPLAY_COLOUR_CITY_RAIL_TRAM_UNDERGROUND,
  DISPLAY_COLOUR_CITY_RAIL_SUBWAY,
  DISPLAY_COLOUR_CITY_RAIL_SUBWAY_UNDERGROUND,

  // **** ALWAYS PUT THIS LAST
  DISPLAY_COLOURS
} TDisplayColours;

///
/// Represents a colour palette object
///
typedef struct TColourPalette
{
  /*
  * Colour are stored as 8 bits index regardless of the
  * actual depth of physical display.  8 bits index are
  * be converted to 16 bits index by display_getColour().
  * An alternative would be to store R, G, B value in this
  * table and let display_getColour() convert R, G, B into
  * a 8 or 16 bits colour index depending on the depth of
  * the display context.  However, that would make this
  * table 3 times bigger. Storing this palette in 8 bits
  * is good enough.
  *
  * This palette is initialized by display_colourInit() 
  * and can be customized by the application when calling
  * display_setColour()
  */
  RGBCOLOUR lookup_displayColour[DISPLAY_COLOURS];
} TColourPalette;

/// @}

/// @}

extern MAPCORE_API
void ColourPalette_Init(TColourPalette *pThis);

extern MAPCORE_API
RGBCOLOUR ColourPalette_GetColour(const TColourPalette *pThis, TDisplayColours index);

extern MAPCORE_API
void ColourPalette_SetColour(TColourPalette *pThis, TDisplayColours index, RGBCOLOUR rgbColour);




#ifdef __cplusplus
}
#endif

#endif
