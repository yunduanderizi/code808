#ifndef DIDEBUGP_H
#define DIDEBUGP_H

#include <display/didebug.h>
#include <display/diviewport.h>
#include <graphics/grcolour.h>
#include <math/gerect.h>

#ifdef __cplusplus
extern "C" {
#endif
   
// Types of object in the debug display list
typedef enum {
  DEBUG_DRAW_TYPE_POINT,
  DEBUG_DRAW_TYPE_LINE,
  DEBUG_DRAW_TYPE_RECT,
  DEBUG_DRAW_TYPE_TEXT  
} TDebugDrawType;

typedef struct TDebugDrawTextData {
  TPos       pos;
  char*      str;
  RGBCOLOUR  colour;
  RGBCOLOUR  bgColour;
  //Calculated Fields to minimize label collision 
  TPos       screenPos;
  TRect      bounds;
} TDebugDrawTextData;

typedef struct TDebugDrawPointData {
  TPos       pos;
  UINT16     width;
  BOOL8      fill;
  RGBCOLOUR  colour;
} TDebugDrawPointData;

typedef struct TDebugDrawLineData {
  TPos       start;
  TPos       end;
  UINT16     width;
  RGBCOLOUR  colour;
} TDebugDrawLineData;

typedef struct TDebugDrawRectData {
  TPos       min;
  TPos       max;
  UINT16     width;
  RGBCOLOUR  colour;
} TDebugDrawRectData;


// Node type on the debug display list
typedef struct TDebugDrawNode {

  TDebugDrawType type;  
  union {
    TDebugDrawPointData point;
    TDebugDrawLineData  line;
    TDebugDrawRectData  rect;
    TDebugDrawTextData  text;
  } data;
  struct TDebugDrawNode *next;
} TDebugDrawNode;

#ifdef SIMULATOR

// Clears the debug draw list
#define DEBUG_DRAW_CLEAR() \
  _debug_clearDrawList( FALSE )

// Render the draw list
#define DEBUG_DRAW_RENDER( viewport )\
  _debug_renderDrawList( viewport, TRUE ); \
  _debug_renderDrawList( viewport, FALSE); \
  _debug_clearDrawList( TRUE );

// Adds a point to the debug draw list
#define DEBUG_DRAW_ADD_POINT( pos, radius, fill, colour, isTemp ) \
  _debug_addDrawPoint( pos, radius, fill, colour, isTemp )

// Adds a rectangle to the debug draw list
#define DEBUG_DRAW_ADD_RECT( rect, thickness, colour, isTemp ) \
  _debug_addDrawRect( rect, thickness, colour, isTemp )

// Adds a line to the debug draw list
#define DEBUG_DRAW_ADD_LINE( start, end, thickness, colour, isTemp ) \
  _debug_addDrawLine( start, end, thickness, colour, isTemp )

// Adds a text label to the debug draw list
#define DEBUG_DRAW_ADD_TEXT( pos, string, colour, bgcolour, isTemp ) \
  _debug_addDrawText( pos, string, colour, bgcolour, isTemp )
 
// Renders the bounding box of a given map
#define DEBUG_DRAW_ADD_MAP_BOUNDS( viewport, bounds, label, colour ) \
  _debug_renderMapBounds( viewport, bounds, label, colour )

// Renders a segment
#define DEBUG_DRAW_ADD_SEGMENT( segment, colour, isTemp ) \
  _debug_addDrawSegment( segment, colour, isTemp )


#else 

#define DEBUG_DRAW_CLEAR()                                           ((void)0)
#define DEBUG_DRAW_ADD_POINT( pos, radius, fill, colour, isTemp )    ((void)0)
#define DEBUG_DRAW_ADD_RECT( rect, thickness, colour, isTemp )       ((void)0)
#define DEBUG_DRAW_ADD_LINE( start, end, thickness, colour, isTemp ) ((void)0)
#define DEBUG_DRAW_ADD_TEXT( pos, string, colour, bgcolour, isTemp ) ((void)0)
#define DEBUG_DRAW_RENDER( viewport )                                ((void)0)
#define DEBUG_DRAW_ADD_MAP_BOUNDS( viewport, bounds, label, colour ) ((void)0) 
#define DEBUG_DRAW_ADD_SEGMENT( segment, colour, isTemp )            ((void)0) 

#endif

/* NOTE: These functions should never be called directly. Use the above macros
 * as these are only available in the simulator 
 */
extern void _debug_addDrawPoint( const TPos*       point, 
                                 UINT16            radius, 
                                 BOOL8             fill, 
                                 const RGBCOLOUR*  colour,
                                 BOOL8             useTemp );

extern void _debug_addDrawRect( const TRect*       bbox, 
                                UINT16             thickness,
                                const RGBCOLOUR*   colour,
                                BOOL8              useTemp );

extern void _debug_addDrawLine( const TPos*        start, 
                                const TPos*        end, 
                                UINT16             thickness,
                                const RGBCOLOUR*   colour,
                                BOOL8              useTemp );

extern void _debug_addDrawText( const TPos*        point,
                                const char*        string,
                                const RGBCOLOUR*   textColour,
                                const RGBCOLOUR*   bgColour,
                                BOOL8              useTemp );

extern void _debug_renderDrawList( TViewport*       viewport,
                                   BOOL8            useTemp );

extern void _debug_clearDrawList( BOOL8 useTemp );

extern void _debug_renderMapBounds( const TViewport* viewport,
                                    const TRect*     bounds,
                                    const char*      label,
                                    RGBCOLOUR        colour );

extern void _debug_addDrawSegment( TRoadSegment     *segment,
                                   const RGBCOLOUR  *colour,
                                   BOOL8             useTemp );


#ifdef __cplusplus
}
#endif

#endif /* DIDEBUGP_H */
