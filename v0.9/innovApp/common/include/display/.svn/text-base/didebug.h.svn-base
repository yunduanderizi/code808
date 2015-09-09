#ifndef DIDEBUG_H
#define DIDEBUG_H

#include <innovEngine_types.h>
#include <mapdata/enroad.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SIMULATOR
//To display these by default, uncomment the lines at the end of display_init( ... )
//in dideprecated.c
typedef enum DEBUG_DISPLAY_FLAGS
{

  DEBUG_SHOW_SECTOR_BOUNDS = 1,
  // Show shapepoints
  DEBUG_SHOW_SHAPEPOINTS = 2,
  // Show nodes
  DEBUG_SHOW_NODES = 4,
  DEBUG_SHOW_SMART_HEIGHTLIGHT = 8, 
  // show small crosses at the location of each polygon vertex (very useful to 
  // debug polygon routines or inspect polygon data).  Vertices and donuts have 
  // a different colour.
  DEBUG_SHOW_VERTEX = 16,
  // show polygon edges for debug purposes
  DEBUG_SHOW_EDGES = 32,
  // show segment direction for debug purposes
  DEBUG_SHOW_DIRECTIONS = 64,

  DEBUG_SHOW_POLY_LOWER_SECTOR_BOUNDS = 128,
  DEBUG_SHOW_POLY_UPPER_SECTOR_BOUNDS = 256,
  DEBUG_SHOW_FORCE_32BITS = 0x7FFFFFFF
} DEBUG_DISPLAY_FLAGS;

extern DEBUG_DISPLAY_FLAGS debugRenderFlags;

extern MAPCORE_API
void DebugDisplay_SetRenderFlags(DEBUG_DISPLAY_FLAGS renderFlags);

extern MAPCORE_API
DEBUG_DISPLAY_FLAGS DebugDisplay_GetRenderFlags( void ) ;

extern MAPCORE_API
BOOL8 DebugDisplay_DumpRoadDetails(const TPos *pos);

extern MAPCORE_API
BOOL8 DebugDisplay_DumpTimeZoneDetails(const TPos *pos);

extern MAPCORE_API
BOOL8 DebugDisplay_GetSegmentAndPos(const TPos *pos,
                                    TRoadSegment *pSegment, TPos *pfNode, TPos* plNode);
#endif /* SIMULATOR */

#ifdef __cplusplus
}
#endif

#endif
