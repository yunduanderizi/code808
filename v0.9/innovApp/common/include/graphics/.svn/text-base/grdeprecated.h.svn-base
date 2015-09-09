#ifndef GRDEPRECATED_H
#define GRDEPRECATED_H

#include <graphics/grcontext.h>
#include <graphics/grfontcollectionobj.h>
#include <graphics/grfontobj.h>
#include <language/lacodepage.h>

#ifdef __cplusplus
extern "C" {
#endif


/// @addtogroup Deprecated
/// @{

/// @addtogroup GraphicsAPI
/// @ingroup Deprecated
/// The monolithic Map Core graphics API.  To be deprecated post-Endeavour.
/// @{

DEPRECATED(extern MAPCORE_API UINT16 graph_rgbToColour(UINT8 r,
                                                       UINT8 g,
                                                       UINT8 b));

DEPRECATED(extern MAPCORE_API UINT8 graph_colour16ToColour8(UINT16 colour16));

DEPRECATED(extern MAPCORE_API UINT8 graph_colour16ToColour8(UINT16 colour16));

DEPRECATED(extern MAPCORE_API void graph_colour8ToRGB(UINT8 *r,
                                                      UINT8 *g,
                                                      UINT8 *b,
                                                      UINT8 c));

DEPRECATED(extern MAPCORE_API UINT16 graph_colour8ToColour16(UINT8 colour8));

#define /* UINT16 */ graph_rgbToColour16(/* UINT8 */ r, \
                                         /* UINT8 */ g, \
                                         /* UINT8 */ b) \
/* For faster conversion, with low accuracy... */ \
  (UINT16)( (((b) >> 3) <<  0) |   \
            (((g) >> 2) <<  5) |   \
            (((r) >> 3) << 11)     \
          )
#if 0
/* For slower conversion, with better accuracy... */ \
  (UINT16)( (((((b) < 0xfc) ? (b) + 0x04 : (b)) >> 3) <<  0) |   \
            (((((g) < 0xfe) ? (g) + 0x02 : (g)) >> 2) <<  5) |   \
            (((((r) < 0xfc) ? (r) + 0x04 : (r)) >> 3) << 11)     \
          )
#endif

#define /* UINT8 */ graph_rgbToColour8(/* UINT8 */ r, \
                                       /* UINT8 */ g, \
                                       /* UINT8 */ b) \
  (UINT8)(   ((((r) & 0xC0) >> 4) |    \
              (((g) & 0xC0) >> 2) |    \
               ((b) & 0xC0)            \
             ) |                       \
           ( ( ((r) & 0x3F) +          \
              (((g) & 0x3F) << 1) +    \
               ((b) & 0x3F)            \
             ) >> 6                    \
           )                           \
         )                           
  
/*
 * 16 bit colour index is made as follows:
 *
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |   red   |   green   |   blue  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |          |          |  
 *      |          |          +-----> 5 bits: msb of blue  component
 *      |          +----------------> 6 bits: msb or green component
 *      +---------------------------> 5 bits: msb of red   component
 */ 
static INLINE void graph_colour16ToRGB(UINT8 *r, UINT8 *g, UINT8 *b,  UINT16 c)
{
//  For faster conversion, with low accuracy...
  *b = (UINT8)((c &         0x1F) << 3);
  *g = (UINT8)((c & (0x3F <<  5)) >> 3); 
  *r = (UINT8)((c & (0x1F << 11)) >> 8); 
//  For slower conversion, with better accuracy (required for good alpha blending)
//  *b = (UINT8)(((c &         0x1F) << 3) | ((c &    0x1F) >> 2 ));
//  *g = (UINT8)(((c & (0x3F <<  5)) >> 3) | ((c & (3<< 9)) >> 9 ));
//  *r = (UINT8)(((c & (0x1F << 11)) >> 8) | ((c & (7<<13)) >> 13));
}


/** Returns the red, green, and blue components of a particular colour number
    using context's colour depth

    @param r A pointer to a variable that contains the red component 
    on exit.

    @param g A pointer to a variable that contains the green component 
    on exit.

    @param b A pointer to a variable that contains the blue component 
    on exit.

    @param c The colour number of the colour to be decomposed into
    colour components. */
DEPRECATED(extern MAPCORE_API void graph_colourToRGB( UINT8 *r,
                                                      UINT8 *g,
                                                      UINT8 *b,
                                                      UINT16 c ));

/** Convert 8 bit colour index into a colour index using the context's 
    depth.

    @param colour8 a colour index in 8 colour depth

    @return colour index using context depth */
DEPRECATED(extern MAPCORE_API UINT16 graph_colour8ToColour( UINT8 colour8 ));

/** Convert a colour index using the context's depth into a
    8 bits colour index

    @param colour a colour index in 8 colour depth

    @return 8 bits colour index */
DEPRECATED(extern MAPCORE_API UINT8 graph_colourToColour8( UINT16 colour ));

/** This function is called once to initialise the graphics system.
    Calling any graphics function before this one is not advised.
    graph_fontInit() must have been called before calling graph_init.
     
    @return Returns TRUE if the graphics subsystem was initialised 
    successfully. */
DEPRECATED(extern MAPCORE_API BOOL8 graph_init( void ));

/** Returns the details of the currently selected graphics context.  
    This function is used by various graphics functions to find the 
    location and dimensions of the 'screen' (which may be a user 
    supplied bitmap, or the physical screen).  Before switching 
    graphics context the old context should be saved so that it can 
    be restored later.

    @param saveContext Points to a user supplied TGraphContext 
    structure into which the details of the currently context are copied. */
DEPRECATED(extern MAPCORE_API void graph_getContext( TGraphContext *saveContext ));

/** Switches graphics context thus affecting all subsequent graphics
    function calls.  It can be used to restore a previously 
    saved context or to switch to a new user created context.

    @param newContext Points to user supplied details of the new
    graphics context.  It is generally a good idea to base the new
    context on the existing context and only make changes where
    necessary.

    @note
    NO CHECKS ARE MADE TO ENSURE THE VALIDITY OF THE CONTEXT.  IF IT 
    IS NOT SPECIFIED */
DEPRECATED(extern MAPCORE_API void graph_setContext( TGraphContext *newContext ));

/** Set the graphics clipping window.  All graphics are clipped so that no
    pixels outside the window are altered.  It is generally a good idea 
    to save the old graphics clip area so that is can be restored 
    afterwards.

    @note
    Redraw handlers in the gui system should not make alterations to the 
    clipping window as it is the sole means of preventing the contents 
    of one window from bleeding into another window.

    @note
    The location and extent of the graphics clipping window are not 
    checked against the physical dimensions of the screen or bitmap.

    @param r Specifies the new clipping window. */
DEPRECATED(extern MAPCORE_API void graph_setScreenClip(const TRect *r));

/** Returns the current clipping region.  When altering the clip 
    window it is generally a good idea to save the old graphics 
    clip area so that is can be restored afterwards.

    @param r Points to a TRect structure that contains the 
    clipping window on exit. */
DEPRECATED(extern MAPCORE_API void graph_getScreenClip(TRect *r));

/** Draws a HORIZONTAL line.  The line is clipped against the current 
    graphics clipping window.

    @param xmin minimum x pixel value for points on the line.

    @param xmax maximum x pixel value for points on the line.

    @param y y pixel coordinate of all points on the line.

    @param colour Colour number to use for line drawing. */
DEPRECATED(extern MAPCORE_API void graph_hline( INT32 xmin,
                                                INT32 xmax,
                                                INT32 y,
                                                INT16 colour));


/** Draws a vertical line.  The line is clipped against the current graphics
    clipping window.

    @param x x pixel value for all points on the line.

    @param ymin minimum y pixel value for points on the line.

    @param ymax maximum y pixel value for points on the line.
      
    @param colour Colour number to use for line drawing. */
DEPRECATED(extern MAPCORE_API void graph_vline( INT32 x,
                                                INT32 ymin,
                                                INT32 ymax,
                                                INT16 colour ));

/** Draws an ellipse outline - the axes of the ellipse are always oriented
    in the horizontal and vertical directions.  Builds the outline out 
    of 256 line segments, so larger ellipses might look a bit jagged.

    @param x1 x coordinate the centroid of the ellipse.

    @param y1 y coordinate of the centroid of the ellipse.

    @param tx Total width of the ellipse.

    @param ty Total height of the ellipse.

    @param colour Colour number to use. */
DEPRECATED(extern MAPCORE_API void graph_ellipse( INT32 x1,
                                                  INT32 y1,
                                                  UINT16 tx,
                                                  UINT16 ty,
                                                  INT16 colour ));

/** Draw a circle outline using the Bresenham circle drawing algorithm..

    @param x1 x coordinate the centre of the circle.

    @param y1 y coordinate of the centre of the circle.

    @param t The radius of the circle (not including the middle pixel).

    @param colour Colour number to use. */
  DEPRECATED(extern MAPCORE_API void graph_circle( INT32 x1,
                                                   INT32 y1,
                                                   UINT16 t,
                                                   INT16 colour ));


/** Draw a filled circle using the Bresenham circle drawing algorithm.

    @param x1 x coordinate the centre of the circle.

    @param y1 y coordinate of the centre of the circle.

    @param t The radius of the circle (not including the middle pixel).

    @param colour Colour number to use. */
DEPRECATED(extern MAPCORE_API void graph_fillCircle( INT32 x1,
                                                     INT32 y1,
                                                     UINT16 t,
                                                     INT16 colour ));

/** Arbitrary line drawing function that draws a line with any 
    thickness and rounded ends.  The line is clipped against the 
    current clipping region.

    @param x1 x coordinate of the start of the line.

    @param y1 y coordinate of the start of the line.

    @param x2 x coordinate of the end of the line.

    @param y2 y coordinate of the end of the line.

    @param t Specifies the number of pixels that are drawn 
    either side of the centre line (i.e. the total thickness of the 
    line is 1 + (2 * t) pixels).

    @param colour Specifies the colour number to use.

    @note
    Sub-pixel positioning is used to ensure that the thickness of 
    the line (measured perpendicular to the line) is as close as 
    possible to 1 + (2 * t). */
DEPRECATED(extern MAPCORE_API BOOL8 graph_line( INT32 x1,
                                                INT32 y1,
                                                INT32 x2,
                                                INT32 y2,
                                                UINT16 t,
                                                INT16 colour ));

/** Arbitrary line drawing function that draws a line with any 
    thickness and rounded ends.  The line is antialiased and clipped 
    against the current clipping region.

    @param x1 x coordinate of the start of the line.

    @param y1 y coordinate of the start of the line.

    @param x2 x coordinate of the end of the line.

    @param y2 y coordinate of the end of the line.

    @param t Specifies the number of pixels that are drawn EITHER SIDE 
    of the centre line (i.e. the total thickness of the line is 
    1 + (2 * t) pixels).

    @param colour Specifies the colour number to use.

    @note
    Sub-pixel positioning is used to ensure that the thickness of the 
    line (measured perpendicular to the line) is as close as possible 
    to 1 + (2 * t). */
DEPRECATED(extern MAPCORE_API BOOL8 graph_lineAA( INT32 x1,
                                                  INT32 y1,
                                                  INT32 x2,
                                                  INT32 y2,
                                                  UINT16 t,
                                                  INT16 colour ));

DEPRECATED(extern MAPCORE_API BOOL8 graph_lineThinAlpha( INT32 x1,
                                                         INT32 y1,
                                                         INT32 x2,
                                                         INT32 y2,
                                                         INT16 colour,
                                                         UINT16 alpha));

DEPRECATED(extern MAPCORE_API BOOL8 graph_lineThinAlphaDashed( INT32 x1,
                                                               INT32 y1,
                                                               INT32 x2,
                                                               INT32 y2,
                                                               INT16 colour,
                                                               UINT16 alpha,
                                                               UINT32 *pattern ));

/** This function is identical to graph_line but the ends of the line are
    NOT CLIPPED against the graphics clipping window or the screen first.

    @note
    This function must be used with extreme caution as it has no 
    protection for going outside screen memory.

    @note
    Sub-pixel positioning is used to ensure that the thickness of the 
    line (measured perpendicular to the line) is as close as possible 
    to 1 + (2 * t).
      
    @param x0 x coordinate of the start of the line.

    @param y0 y coordinate of the start of the line.

    @param x1 x coordinate of the end of the line.

    @param y1 y coordinate of the end of the line.

    @param t Specifies the number of pixels that are drawn EITHER SIDE of 
    the centre line (i.e. the total thickness of the line is 1 + (2 * t) 
    pixels).

    @param colour Specifies the colour number to use. */
DEPRECATED(extern MAPCORE_API void graph_lineDirect( INT32 x0,
                                                     INT32 y0,
                                                     INT32 x1,
                                                     INT32 y1,
                                                     UINT16 t,
                                                     INT16 colour));

DEPRECATED(extern MAPCORE_API void graph_lineDirectAA( INT32 x0,
                                                       INT32 y0,
                                                       INT32 x1,
                                                       INT32 y1,
                                                       UINT16 t,
                                                       INT16 colour));

  /** This function is identical to graph_lineDirect but the line is rendered
      with a bitmap pattern.

      @param x1 x coordinate of the start of the line.
      @param y1 y coordinate of the start of the line.
      
      @param x2 x coordinate of the end of the line.
      @param y2 y coordinate of the end of the line.
      
      @param t thickness of the line.  PLEASE NOTE THICKNESS OF 0 EG. 1 PIXEL
      IS CURRENTLY UNSUPPORTED FOR THIS FUNCTION.

      @param bitmap 8 bpp or 16 bpp uncompressed bitmap used for the pattern.
      The bitmap can be transparent. */
  DEPRECATED(extern MAPCORE_API void graph_lineDirectPattern( INT32 x1,
                                                              INT32 y1,
                                                              INT32 x2,
                                                              INT32 y2,
                                                              UINT16 t,
                                                              const TBitmap *bitmap));

/** This function is identical to graph_lineDirectAA but the line is rendered
    with a bitmap pattern. The edge of the line is anti-aliased using
    pixels taken from the pattern (pattern pixels are blended along the line
    edges with alpha values progressively fading out).

    @param x1 x coordinate of the start of the line.

    @param y1 y coordinate of the start of the line.

    @param x2 x coordinate of the end of the line.

    @param y2 y coordinate of the end of the line.

    @param t thickness of the line.  PLEASE NOTE THICKNESS OF 0 EG. 1 PIXEL
    IS CURRENTLY UNSUPPORTED FOR THIS FUNCTION.

    @param bitmap 8 bpp or 16 bpp uncompressed bitmap used for the pattern.
    The bitmap can be transparent. */
DEPRECATED(extern MAPCORE_API void graph_lineDirectPatternAA( INT32 x1,
                                                              INT32 y1,
                                                              INT32 x2,
                                                              INT32 y2,
                                                              UINT16 t,
                                                              const TBitmap *bitmap));

/** This line drawing function is a version of graph_line optimised for 
    drawing lines with a thickness parameter of 0.  As with graph_line 
    direct the ends of the line are NOT CLIPPED against the graphics 
    clipping window or the screen first.

    @note
    This function must be used with extreme caution as it has no 
    protection for going outside screen memory.
      
    @param x1 x coordinate of the start of the line.

    @param y1 y coordinate of the start of the line.

    @param x2 x coordinate of the end of the line.

    @param y2 y coordinate of the end of the line.

    @param colour Specifies the colour number to use.

    @param alpha a transparency value from 0 (fully transparent) to 256. */
DEPRECATED(extern MAPCORE_API void graph_lineThinDirectAlpha( INT32 x1,
                                                              INT32 y1,
                                                              INT32 x2,
                                                              INT32 y2,
                                                              INT16 colour,
                                                              UINT16 alpha ));

/** graph_clipLine clips a line to the current clipping rectangle.  This
    procedure uses the well known Cohen-Sutherland line clipping 
    algorithm.  It is used internally to perform line clipping by 
    graph_line.

    @param x1 x coordinate of the start of the line.

    @param y1 y coordinate of the start of the line.

    @param x2 x coordinate of the end of the line.

    @param y2 y coordinate of the end of the line.

    @param clipped Points to an INT16[4] array that on exit contains the 
    clipped coordinates (if the line passes through the current clip 
    region).

    @return If the line lies completely outside of the clip boundary, 
    the function returns FALSE.  Otherwise returns TRUE. */
DEPRECATED(extern MAPCORE_API BOOL8 graph_clipLine( INT32 x1,
                                                    INT32 y1,
                                                    INT32 x2,
                                                    INT32 y2,
                                                    INT16 *clipped));

DEPRECATED(extern MAPCORE_API void graph_lineThinDirectAA( INT32 x1,
                                                           INT32 y1,
                                                           INT32 x2,
                                                           INT32 y2,
                                                           INT16 colour));

/** This line drawing function is a version of graph_line optimised for 
    drawing lines with a thickness parameter of 0.  As with graph_line 
    direct the ends of the line are NOT CLIPPED against the graphics 
    clipping window or the screen first.

    @note
    This function must be used with extreme caution as it has no 
    protection for going outside screen memory.

    @note
    This pattern will be updated to reflect the current
    rotated bits when the line's end point is reached.
    The updated pattern shall then be used to draw
    any subsequent line starting from this end point.
    This ensures the dash pattern is uniform across line joints.

    @param x1 x coordinate of the start of the line.

    @param y1 y coordinate of the start of the line.

    @param x2 x coordinate of the end of the line.

    @param y2 y coordinate of the end of the line.

    @param colour Specifies the colour number to use.

    @param pattern Specifies a pointer to the 32-bit pattern used for 
    the dash style.

    @param alpha a transparency value from 0 (fully transparent) to 256 */
DEPRECATED(extern MAPCORE_API void graph_lineThinDirectAlphaDashed( INT32 x1,
                                                                    INT32 y1,
                                                                    INT32 x2,
                                                                    INT32 y2,
                                                                    INT16 colour,
                                                                    UINT16 alpha,
                                                                    UINT32 *pattern ));

/** Draws the border of a polygon.

    All the X values of the points should have been scaled by 
    POLYFILL_X_RESOLUTION.  Otherwise, the points should be in 
    screen coordinates.
      
    @param nvert The total number of vertices in the polygon, 
    including donut holes.

    @param point An array of nvert TPos points representing the 
    polygon vertices.

    @param colour The colour of the polygon borders.

    @param thickness Thickness of polygon borders */
DEPRECATED(extern MAPCORE_API void graph_polyLine( INT16 nvert,
                                                   const TPos* point,
                                                   INT16 colour,
                                                   INT16 thickness ));

/** Draws an antialiased polyline.

    All the X values of the points should have been scaled by 
    POLYFILL_X_RESOLUTION.  Otherwise, the points should be in screen 
    coordinates.

    @param nvert The total number of vertices in the polygon, including 
    donut holes.

    @param point An array of nvert TPos points representing the polygon 
    vertices.

    @param colour The colour of the polygon borders.

    @param thickness Thickness of polygon borders */
DEPRECATED(extern MAPCORE_API void graph_polyLineAA( INT16 nvert,
                                                     const TPos* point,
                                                     INT16 colour,
                                                     INT16 thickness ));

/** Draw a filled rectangle.

    @param r A pointer to a rectangular region that is filled with
    solid colour.

    @param colour Colour number to use. */
DEPRECATED(extern MAPCORE_API void graph_fill( const TRect *r,
                                               INT16 colour ));

/** Draw a filled rectangle by blending a new colour with existing pixels
    on the screen.  The function is optimised for solid colour 
    backgrounds - more complex backgrounds take longer to fill.

    @param r A pointer to a rectangular region that is filled with
    a combination of the existing and new colours.

    @param colour The colour to blend with the existing screen pixels.

    @param alpha A blending coefficient that specifies how much colour 
    from the original pixels shows through.  i.e. alpha==0x00 is the same 
    as a normal fill, whereas alpha=0xff makes no change to the image (i.e. 
    background is unaffected). */
DEPRECATED(extern MAPCORE_API void graph_fillAlpha( const TRect *r,
                                                    UINT16 colour,
                                                    UINT8 alpha ));

/** Fills a region with gradiated colour.

    @param r A pointer to a rectangular region that is filled with
    gradiated colour.  This function performs a lot of dithering so 
    can be quite slow, and may not produce good results for some 
    combinations of colours at the extreme end of colour ranges (because 
    the colours calculated during the interpolation cover a small range).

    @param vertex The colours drawn to the screen are calculated by 
    interpolating red, green, and blue between the colours at the 
    corners.  The colour at each vertex is specified using a colour 
    number.
      
    @par
    The vertex colours are ordered:
    - top-left
    - top-right
    - bottom-left
    - bottom-right
      
    @param accurate If set to TRUE the 'accurate' parameter enables a 
    higher quality colour matching algorithm for dithering but it is 
    much slower.

    @todo
    This function does not work correctly when the clipping window does 
    not fully encapsulate the fill area.  Change the vertex parameter to 
    INT16 *vertex. */
DEPRECATED(extern MAPCORE_API void graph_fillBlend( const TRect *r,
                                                    const INT16 vertex[4],
                                                    BOOL8 accurate ));

/** Fill the entire screen using a specified solid colour.

    @param c colour to use to fill the screen.

    @note
    This operation is not clipped. */
DEPRECATED(extern MAPCORE_API void graph_fillScreen( INT16 c ));

/** Clear the entire screen (with colour 0).
    
    @note
    This operation is not clipped.

    @todo this should be removed, it is only there for
    backward compatibility */
DEPRECATED(extern MAPCORE_API void graph_clearScreen( void ));

/** Draws a filled polygon.

    @par
    Based on code from: "Graphics Gems", 1990, Academic Press. Free for 
    unrestricted use.  The points describing the outside of the polygon 
    (polygon shell) as well as all donut holes must be closed. That is, 
    the first point must not replicated as the last point for shell and 
    each hole.  The points describing the donut holes (if any) 
    immediately follow the shell points.  The index within the point 
    array of the first point of each donut hole is kept in the array 
    donutList.

    All the X values of the points should have been scaled by 
    POLYFILL_X_RESOLUTION.  Otherwise, the points should be in screen 
    coordinates.

    It is recommended that the shell points be in a clockwise direction, 
    and donut points be in a counterclockwise direction. Currently this 
    is not necessary, but may be enforced in future implementations.

    @param nvert The total number of vertices in the polygon, including 
    donut holes.

    @param point An array of nvert TPos points representing the polygon 
    vertices.

    @param ndonuts Number of donut holes in the polygon.

    @param donutList A list of donut vertex indexes.
    
    @param colour The colour to fill the polygon.

    @todo
    THE NAME OF THIS FUNCTION SHOULD BE CHANGED TO graph_polyFill */
DEPRECATED(extern MAPCORE_API void graph_polyfill( INT16 nvert,
                                                   const TPos* point,
                                                   INT16 ndonuts,
                                                   const INT16* donutList,
                                                   INT16 colour ));

/** Draws a filled antialiased polygon.

    @par
    Based on code from: "Graphics Gems", 1990, Academic Press. Free 
    for unrestricted use.  The points describing the outside of the 
    polygon are not closed. That is, the first point is not replicated 
    as the last point.  The points describing the donut holes (if any) 
    immediately follow the shell points.  The index within the point 
    array of the first point of each donut hole is kept in the array 
    donutList.

    All the X values of the points should have been scaled by 
    POLYFILL_X_RESOLUTION.  Otherwise, the points should be in screen 
    coordinates.

    It is recommended that the shell points be in a clockwise direction, 
    and donut points be in a counterclockwise direction. Currently this 
    is not necessary, but may be enforced in future implementations.

    @param nvert The total number of vertices in the polygon, including 
    donut holes.

    @param point An array of nvert TPos points representing the polygon 
    vertices.

    @param ndonuts Number of donut holes in the polygon

    @param donutList A list of donut vertex indexes.

    @param colour The colour to fill the polygon.

    @todo
    THE NAME OF THIS FUNCTION SHOULD BE CHANGED TO graph_polyFill */
DEPRECATED(extern MAPCORE_API void graph_polyfillAA( INT16 nvert,
                                                     const TPos* point,
                                                     INT16 ndonuts,
                                                     const INT16* donutList,
                                                     INT16 colour ));

/** Draw a filled convex polygon.

    This function is an optimized version of graph_polyfill() which can
    be used when polygon is convex and without donuts holes.   It is in 
    that case, several times faster than graph_polyfill() (speed improvement
    depends on polygon shape).

    Like graph_polyfill(), all X values of the points should have been 
    scaled by POLYFILL_X_RESOLUTION before calling the function.

    Unlike graph_polyfill(), the polygon does not have to be closed
    by copying the first vertex into the last one although closing
    the polygon won't cause any harm.

    This function can draw polygons in clock-wise or anti-clockwise vertices
    order.

    @param nvert Number of vertices in the polygon.

    @param point An array of nvert TPos points representing the polygon 
    vertices.

    @param colour The colour to fill the polygon. */
DEPRECATED(extern MAPCORE_API void graph_polyConvexFill( INT16 nvert,
                                                         const TPos point[],
                                                         INT16 colour ));

/** Draw a filled convex antialiased polygon.

    @param nvert Number of vertices in the polygon.

    @param point An array of nvert TPos points representing the polygon 
    vertices.

    @param colour The colour to fill the polygon. */
DEPRECATED(extern MAPCORE_API void graph_polyConvexFillAA( INT16 nvert, 
                                                           const TPos point[],
                                                           INT16 colour ));

/** Draw a convex polygon filled with a pattern.

    Like graph_polyfill(), all X values of the points should have been 
    scaled by POLYFILL_X_RESOLUTION before calling the function.

    Unlike graph_polyfill(), the polygon does not have to be closed
    by copying the first vertex into the last one although closing
    the polygon won't cause any harm.

    This function can draw polygons in clock-wise or anti-clockwise vertices
    order.

    @param n Number of vertices in the polygon.

    @param v An array of nvert TPos points representing the polygon vertices
    in screen coordinates.

    @param bm A bitmap to describe the pattern.  The bitmap must be a 
    seamless tile and its size (height & width) must be a power of 2.
    Transparency flag is honored. */
DEPRECATED(extern MAPCORE_API void graph_polyConvexPattern( INT16 n,
                                                            const TPos v[],
                                                            const TBitmap *bm ));

  
/** Draw a concave polygon with holes filled with a pattern.

    All X values of the points should have been scaled by
    POLYFILL_X_RESOLUTION before calling the function.

    Like graph_polyfill(), the polygon needs to be closed
    by copying the first vertex into the last one.

    This function can draw polygons in clock-wise or anti-clockwise vertices
    order.

    @param nvert The total number of vertices in the polygon, including 
    donut holes.

    @param point An array of nvert TPos points representing the polygon 
    vertices.

    @param ndonuts Number of donut holes in the polygon.

    @param donutList A list of donut vertex indexes.

    @param bm A bitmap to describe the pattern.  The bitmap must be a 
    seamless tile and its size (height & width) must be a power of 2.
    Transparency flag is honored. */
DEPRECATED(extern MAPCORE_API void graph_polyPattern( INT16 nvert,
                                                      const TPos *point,
                                                      INT16 ndonuts,
                                                      const INT16 *donutList,
                                                      const TBitmap *bm ));


/** Draw a textured convex polygon.

    The function linearly interpolates texture coordinates between
    polygon vertices (aka affine texture mapping).  Note this this
    is not a true perspective correct texturing but it is faster
    and looks good enough in many cases.  Polygon does not have to
    be clipped prior to calling this routine.   However, rendering
    of polygon should be faster if polygon has been clipped (or
    at least does not extend far beyond drawing context boundaries).

    This function can draw polygons in clock-wise or anti-clockwise
    vertices order.

    @param n Number of vertices in the polygon.

    @param v An array of n points representing the polygon vertices
    positions in screen coordinates.

    @param tv An array of n points representing the texel coordinates
    of all vertices.

    @param tex Structure describing the texture to be used. The texture
    can be transparent or not, tiled or not.  If a tiled texture is used,
    the size in U, V have to be a power of 2.  Sizes in U, V don't have
    to be a power of 2 when texture is not tiled.  Transparent textures
    rendering is slightly slower than non-transparent.

    CAVEAT: when using a non-tiled texture, U, V coordinates of all
    vertices have to be less or equal to the texture size.  The function
    does not check that for performance reasons.  If U, V are beyond the
    texture size, function may access memory beyond what it is suppose
    to access.

    CAVEAT: since affine texture mapping is not a true perspective
    texture mapping, there might be a warping effect in the texture
    more or less visible depending on the size of the polygon and 
    coordinates of the vertices in textures. */
DEPRECATED(extern MAPCORE_API void graph_polyConvexTextured( INT16 n,
                                                             const TPos v[],
                                                             const TPos tv[],
                                                             const TTexture *tex ));

/** Draws a rotated bitmap.  This function sub samples pixels to ensure
    that the rotated result does not take on 'jagged' look.

    @param bm Points to the header of the bitmap to be rotated.  The 
    bitmap can be compressed or stored if necessary.

    @param srcXY This is the centre of rotation inside the bitmap being
    rotated.  i.e. The bitmap can be rotated about its top left corner by
    setting srcXY to {0,0} or can be rotated about its centre by setting
    srcXY to {width >> 1, -height >> 1}.

    @param destXY The point srcXY from inside the bitmap is mapped to destXY
    on the screen - therefore it represents the centre of rotation position
    on screen.

    @param angle This represents the rotation angle of the bitmap measured
    clockwise.  Values of 0-255 represent 0-359 degree rotations.

    @param blendWithBackground Pixels that are partially transparent as a
    result of sub-pixel rotation can be alpha blended with the content of the
    background context if blendWithBackground parameter is true.
    blendWithBackground should generally be true when blitting directly onto
    the screen. It should be false when blitting on a temporary context which
    is not the screen because it then does not make sense to blend
    the rotated bitmap with the context. */
DEPRECATED(extern MAPCORE_API void graph_rotoBlitBlend( const TBitmap *bm,
                                                        TPos srcXY,
                                                        TPos destXY,
                                                        UINT8 angle,
                                                        BOOL8 blendWithBackground ));

/** Macro graph_rotoBlit() is provided for backward compatibility.
    Avoid using graph_rotoBlit() because it might become deprecated in
    future version of the graphics lib.  Calling graph_rotoBlitBlend()
    should thus be preferred. */
DEPRECATED(extern MAPCORE_API void graph_rotoBlit( const TBitmap *bm,
                                                   TPos srcXY,
                                                   TPos destXY, 
                                                   UINT8 angle ));

/** This is a low level function used to plot scanlines or areas of 
    monochrome bitmap data to the screen.

    @todo
    THIS FUNCTION SHOULD BE CHANGED TO BLIT MONOCHROME BITMAPS.  i.e. IT 
    SHOULD BE PASSED A BITMAP HEADER INSTEAD OF A BLOCK OF SCANLINE DATA.

    @param r A pointer to a rectangular region that is filled with a colour
    version of the monochrome data.  Monochrome pixel data is output from 
    the left side of the rectangle for as many pixels as it takes to reach 
    the right side of the rectangle, or until dataWidth bytes of monochrome 
    data have been read for the scan line.

    @param data Points to a block of monochrome scan line data.  The 
    monochrome data is interpreted as a bit stream of pixels from MSb 
    to LSb.  i.e. pixels are organised in the following order:
    - bit 7 of byte 0
    - bit 6 of byte 0
    - ...
    - bit 1 of byte 0
    - bit 0 of byte 0
    - bit 7 of byte 1
    - bit 6 of byte 1
    - ...

    @param dataWidth This is the width of the monochrome data scan lines 
    in bytes.

    @param transparent The transparent parameter determines whether 
    logical colour 0 is treated as being a transparent colour or not.

    @param colours A pointer to a UINT16[2] array that contains the colour 
    numbers used to map pixels with logical colours 0 and 1 into colour 
    numbers on the coloured screen. */
DEPRECATED(extern MAPCORE_API void graph_monoBlit( const TRect *r,
                                                   const UINT8 *data,
                                                   UINT16 dataWidth,
                                                   BOOL8 transparent,
                                                   const UINT16 *colours));

/** This is a low level function used to plot scanlines or areas of 
    alpha-mapped monochrome bitmap data to the screen.

    @todo
    THIS FUNCTION SHOULD BE CHANGED TO BLIT MONOCHROME BITMAPS.  i.e. IT 
    SHOULD BE PASSED A BITMAP HEADER INSTEAD OF A BLOCK OF SCANLINE DATA.

    @param r A pointer to a rectangular region that is filled with a colour
    version of the monochrome data.  Monochrome pixel data is output from 
    the left side of the rectangle for as many pixels as it takes to 
    reach the right side of the rectangle, or until dataWidth bytes of 
    monochrome data have been read for the scan line.

    @param data Points to a block of 8bit alpha-mapped monochrome scan 
    line data.  The monochrome data is interpreted as an array of bytes, 
    each representing the transparency of a pixel (e.g. 0x00 means 
    fully opaque, 0xff is transparent).

    @param dataWidth This is the width of the monochrome data scan lines 
    in bytes.

    @param transparent The transparent parameter determines whether 
    logical colour 0 is treated as being a transparent colour or not.

    @param colours A pointer to a UINT16[2] array that contains the 
    colour numbers used to map pixels with logical colours 0 and 1 into 
    colour numbers on the coloured screen. */
DEPRECATED(extern MAPCORE_API void graph_monoBlitAlpha( const TRect *r,
                                                        const UINT8 *data,
                                                        UINT16 dataWidth,
                                                        BOOL8 transparent,
                                                        const UINT16 *colours ));

/** This is a low level function used to plot scanlines or areas of 
    monochrome bitmap data to the screen.

    @todo
    THIS FUNCTION SHOULD BE CHANGED TO BLIT MONOCHROME BITMAPS.  i.e. IT 
    SHOULD BE PASSED A BITMAP HEADER INSTEAD OF A BLOCK OF SCANLINE DATA.

    @param r A pointer to a rectangular region that is filled with a colour
    version of the monochrome data.  Coordinates are in sub-pixel space, 
    so screen X coordinates need to be multiplied by 3.  Monochrome pixel 
    data is output from the left side of the rectangle for as many 
    pixels as it takes to reach the right side of the rectangle, or until 
    dataWidth bytes of monochrome data have been read for the scan line.

    @param data Points to a block of monochrome scan line data.  The 
    monochrome data is interpreted as a bit stream of pixels from MSB to 
    LSB.  i.e. pixels are organised in the following order:
    - bit 7 of byte 0
    - bit 6 of byte 0
    - ...
    - bit 1 of byte 0
    - bit 0 of byte 0
    - bit 7 of byte 1
    - bit 6 of byte 1
    - ...

    @param dataWidth This is the width of the monochrome data scan lines 
    in bytes.

    @param transparent The transparent parameter determines whether 
    logical colour 0 is treated as being a transparent colour or not.

    @param colours A pointer to a UINT16[2] array that contains the colour 
    numbers used to map pixels with logical colours 0 and 1 into colour 
    numbers on the coloured screen. */
DEPRECATED(extern MAPCORE_API void graph_monoBlitSub( const TRect *r,
                                                      const UINT8 *data,
                                                      UINT16 dataWidth,
                                                      BOOL8 transparent,
                                                      const UINT16 *colours ));
  
/** This is a low level function used to plot scanlines or areas of 
    alpha-mapped monochrome bitmap data to the screen.

    @todo
    This function should be changed to blit monochrome bitmaps.  i.e. it 
    should be passed a bitmap header instead of a block of scanline data.

    @param r A pointer to a rectangular region that is filled with a colour
    version of the monochrome data.  Coordinates are in sub-pixel space, 
    so screen X coordinates need to be multiplied by 3.  Monochrome pixel 
    data is output from the left side of the rectangle for as many 
    pixels as it takes to reach the right side of the rectangle, or until 
    dataWidth bytes of monochrome data have been read for the scan line.

    @param data Points to a block of 8bit alpha-mapped monochrome scan 
    line data.  The monochrome data is interpreted as an array of bytes, 
    each representing the transparency of a pixel (e.g. 0x00 means fully 
    opaque, 0xff is transparent).

    @param dataWidth This is the width of the monochrome data scan lines 
    in bytes.

    @param transparent The transparent parameter determines whether 
    logical colour 0 is treated as being a transparent colour or not.

    @param colours A pointer to a UINT16[2] array that contains the 
    colour numbers used to map pixels with logical colours 0 and 1 into 
    colour numbers on the coloured screen. */
DEPRECATED(extern MAPCORE_API void graph_monoBlitSubAlpha( const TRect *r,
                                                           const UINT8 *data,
                                                           UINT16 dataWidth,
                                                           BOOL8 transparent,
                                                           const UINT16 *colours ));

/** Rectangle move (translation) function that copies the contents of
    the screen to another location.  Pieces of the original rectangle 
    that are not part of the copied rectangle remain unaltered.  The 
    destination rectangle can be partly or fully outside the clipping 
    area.

    @param r Specifies the location and size of the source rectangle.

    @param x The x translation amount (positive values are to the right).

    @param y The y translation amount (positive values are up). */
DEPRECATED(extern MAPCORE_API void graph_move( const TRect *r,
                                               INT32 x,
                                               INT32 y));

/** Draw a bitmap on screen and scale it with X and Y scaling 
    coefficients.  graph_scaleBlit() uses colour index 0 as transparent 
    colour if transparentFlag is set in the bitmap.

    @param bm Points to the bitmap to draw.

    @param x x co-ordinate of the top left corner.

    @param y y co-ordinate of the top left corner.

    @param scaleX Scaling factor in X. scaleX is a fixed point number
    with 7 bits before decimal point.  For example, following values will
    have the following effects:
    - 128 ... do not resize the bitmap
    - 64  ... make the bitmap twice as small
    - 256 ... make the bitmap twice as big

    @param scaleY scaling factor in Y (same remarks as scaleX).

    @param alpha a transparency value from 0 (fully transparent) to 256.

    @todo graph_scaleBlit() resize method does not use any interpolation or
    antialiasing whatsoever.  This could be improved at the cost of efficiency. */
DEPRECATED(extern MAPCORE_API void graph_scaleBlit( const TBitmap *bm,
                                                    INT32 x,
                                                    INT32 y,
                                                    UINT32 scaleX,
                                                    UINT32 scaleY,
                                                    UINT16 alpha ));

/** This function is called once to initialise the font system.  No calls
    to a font functions should be made before calling this routine.

    @param f_ptr should point to a valid (null-terminated) TFont array. */
DEPRECATED(extern MAPCORE_API void graph_initFonts( const TFont **f_ptr ));

/** This function returns the font pointer corresponding to 
    the input font name, size, bold and italic attributes.

    @param name name of the font to search

    @param size size of the font to search

    @param bold bold attribute of the font to search

    @param italic italic attribute of the font to search

    @return font pointer */
DEPRECATED(extern MAPCORE_API const TFont *graph_findFont( const char *name,
                                                           UINT32 size,
                                                           BOOL8 bold,
                                                           BOOL8 italic ));

/** Set the current font for a certain graphics context.

    @note
    Only those fonts that are available in the user supplied font array 
    specified to graph_initfonts can be used.  Fonts can be compiled 
    using the form compiler.

    @note
    If the font cannot be found, this function does not change the 
    current settings.

    @param c A pointer to the graphics context or NULL to use the current
    context.

    @param name The name of the desired font.

    @param size The POINT SIZE of the font.

    @param bold TRUE if the font should be in bold.

    @param italic TRUE if the font should be italic.

    @return TRUE if the font was successfully changed, FALSE otherwise. */
DEPRECATED(extern MAPCORE_API BOOL8 graph_setFont( TGraphContext *c,
                                                   const char *name,
                                                   UINT32 size,
                                                   BOOL8 bold,
                                                   BOOL8 italic ));

/** Allows the font pointer of a graphics context to be directly manipulated.

    @param c A pointer to the graphics context or NULL to use the current
    context.

    @param font Points directly to the new font to use.

    @note
    Use with great care as the pointer is not checked. */
DEPRECATED(extern MAPCORE_API void graph_setFontPtr( TGraphContext *c,
                                                     const TFont *font ));

/** Returns a pointer to the current font.

    @param c A pointer to the graphics context or NULL to use the current
    context.

    @return The current font pointer. */
DEPRECATED(extern MAPCORE_API const TFont* graph_getFontPtr( const TGraphContext *c ));

/** Verifies a font pointer against the font array.

    @param font Checks this font pointer against values in the fonts array.

    @return TRUE if the pointer is valid, otherwise FALSE. */
DEPRECATED(extern MAPCORE_API BOOL8 graph_verifyFontPtr( const TFont *font ));

/** Given an input font, this function returns available
    fonts with same characteristics as input font.  The boolean
    parameters (wantSame*) allow to select what we mean by
    "same characteristics".

    This function works as is an iterator.  Call it with an input
    font pointer to get the first available font size, then call it
    with a NULL pointers several times to get other available
    font until it returns 0 when no more available fonts are
    available.  This is kind of similar to how strtok() iterator
    works.

    For example, if we need to query all available fonts 
    with same name, same bold and same italic attributes but
    different sizes, we can use...

    graph_getFontSizes(font, TRUE, FALSE, TRUE, TRUE) 

    ... to get the first font that matches, and then call...

    graph_getFontSizes(NULL, TRUE, FALSE, TRUE, TRUE) 

    ... to get other fonts that match until it returns NULL.

    @param font Reference font to use to start iterating through 
    available fonts, NULL for subsequent calls.

    @param wantSameName If true, request that returned font have same 
    name as reference font.  If false, ignore font name.

    @param wantSameSize If true, request that returned fonts have same
    size as reference font.  If false, ignore font size.

    @param wantSameBold If true, request that returned fonts have same
    bold attribute as reference font.  If false, ignore bold attribute.

    @param wantSameItalic If true, request that returned fonts have same
    italic attribute as reference font.  If false, ignore italic attribute.

    @return returns a pointer to the next font that matches 
    requested fonts or NULL if there are no more fonts. */
DEPRECATED(extern MAPCORE_API const TFont *graph_getAvailableFonts( const TFont *font,
                                                                    BOOL8 wantSameName,
                                                                    BOOL8 wantSameSize,
                                                                    BOOL8 wantSameBold,
                                                                    BOOL8 wantSameItalic ));

/** Draws text using the current font.

    @param s Points to the text string to be drawn.

    @param x x Coordinate of the top left corner of the text.

    @param y y Coordinate of the top left corner of the text.

    @param colour The colour number of the text colour. */
DEPRECATED(extern MAPCORE_API void graph_text( const TLatin1Char *s,
                                               INT32 x,
                                               INT32 y,
                                               INT16 colour ));

/** Draws angled text using the current font.

    @param s Points to the text string to be drawn.

    @param x x Coordinate of the top left corner of the text (which is
    also the centre of rotation).

    @param y y Coordinate of the top left corner of the text (which is
    also the centre of rotation).

    @param colour The colour number of the text colour.

    @param angle The rotation angle of the text about x,y measured 
    CLOCKWISE.  Values of 0-255 represent angles 0-359 degrees. */
DEPRECATED(extern MAPCORE_API void graph_rotoText( const TLatin1Char *s,
                                                   INT32 x,
                                                   INT32 y,
                                                   INT16 colour,
                                                   UINT8 angle));

/** Calculate a text bounding box of a string in the current font, 
    typically so that it can be positioned within an icon according to 
    the configured justification.

    @param r On exit contains the bounding box of a string of text 
    displayed in the current font.  The top-left corner is always at 
    location (0, 0).

    @param s Points to the string to be displayed. */
DEPRECATED(extern MAPCORE_API void graph_textBounds( TRect *r, 
                                                     const TLatin1Char *s ));

/** Returns the height of the current font.  It estimates the height by 
    looking at a string of characters that typically extend far above 
    and below the baseline.

    @return The height (in pixels) of text displayed with the current font.

    @todo
    We should review this function and possibly remove it as the height
    returned is not always a good estimate for all strings and
    graph_textBounds performs the same function. */
  DEPRECATED(extern MAPCORE_API UINT16 graph_textHeight( void ));
  
  
/** Calculate the width of a string in the current font.

    @param s Points to the string to be displayed.

    @return The width (in pixels) of the string of text displayed with
    the current font.

    @todo
    We should review this function and possibly remove it as
    graph_textBounds performs the same function. */
DEPRECATED(extern MAPCORE_API UINT16 graph_textWidth( const TLatin1Char *s ));

/** Draws a bitmap with optional alpha blending.

    @param bitmap Points to a TBitmap header describing the bitmap.
    The bitmap can be stored or compressed.

    @param x X position of the left most pixel from the bitmap.

    @param y Y position of the top most pixel from the bitmap.

    @param alpha alpha blending value (between 0 and 256).  When alpha is 0,
    bitmap is copied onto the screen without alpha blending.  When alpha is
    equal to 256, bitmap is not visible (completely transparent).  
    Note that this is different from transparent attribute of bitmap
    which makes some pixels completely transparent or not.  Implementation
    is more efficient when alpha is null (no alpha blending) */
DEPRECATED(extern MAPCORE_API void bitmap_blitAlpha( const TBitmap *bitmap,
                                                     INT32 x,
                                                     INT32 y,
                                                     UINT16 alpha));

/** Draw a bitmap.

    @param bitmap Points to a TBitmap header describing the bitmap.
    The bitmap can be stored or compressed.

    @param x X position of the left most pixel from the bitmap.

    @param y Y position of the top most pixel from the bitmap.

    This macro is there for backward compatibility.  It could be
    suppressed in the future since it is possible to call 
    bitmap_blitAlpha() directly */
DEPRECATED(extern MAPCORE_API void bitmap_blit( const TBitmap *bitmap, 
                                                INT32 x,
                                                INT32 y));

/** Loads a bitmap from a file.

    @param filename The name of the .bm file containing the bitmap.

    @param bitmapName Points to the name of the bitmap being sought.

    @param workspace Points to a 32 BIT ALIGNED user supplied 
    workspace that is used to contain bitmap data.  The workspace 
    should be large enough to accommodate both the header of the 
    bitmap (TBitmap) as well as the actual bitmap data.

    @param workspaceSize Represents the total size of the available 
    workspace.  Typically (screen width * screen height) +sizeof(TBitmap).

    @return TRUE if the bitmap was loaded successfully.  The function 
    may fail if the file does not exists, the bitmap could not be 
    found, or the workspace is too small.
    
    @deprecated Use #Bitmap_Load instead. */
DEPRECATED(extern MAPCORE_API BOOL8 bitmap_load( const char *filename,
                                                 const char *bitmapName,
                                                 UINT8 *workspace,
                                                 UINT32 workspaceSize ));

/// Get a pointer to the default font collection.
/// @return pointer to the default font collection.
DEPRECATED(
extern MAPCORE_API
TFontCollection *graph_getDefaultFontCollection(void)
);

/// Get a pointer to the default graphics context.
/// @return pointer to the default graphics context.
DEPRECATED(
extern MAPCORE_API
TGraphContext *graph_getDefaultContext(void)
);

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif
