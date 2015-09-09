#ifndef GERECT_H
#define GERECT_H

// This module provides a number of generic functions to manage rectangle
// structures.

#include <platform_types.h>
#include <innovEngine_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup UnUsed
/// @ingroup Display
/// @{


/// @addtogroup GeometricPrimitives
/// @ingroup UnUsed
/// @{

/// @addtogroup Rect
/// Functions that operate on rectangular regions.
/// @ingroup GeometricPrimitives
/// @{

/** Rectangle code has assertions to check that rectangles are valid
    Enable this by default on the simulator only as we don't want
    fatal error on target and don't want to slow down software on
    target either.

    @param r rectangle to check for validity. */
#ifdef SIMULATOR
#  define rect_assertValid(r) assert(rect_valid(r))
#else
#  define rect_assertValid(r) ((void)0)
#endif
  
/** These enumerations are used primarily by the graphical user interface to
    represent the 5 regions into which windows are divided.  The 
    rect_leftovers function can be used to calculate the first four of 
    these regions given a rectangular window outline and its interior.  The 
    rect_leftovers function assumes that a small rectangle A is contained 
    inside a large rectangle B.  It returns the areas created as A is cut 
    out of B. */
typedef enum {
  /** This rectangle has the width of B and extends from the top of A to
      the top of B. */
  REGION_TOP = 0,
  /** This rectangle has the width of B and extends from the bottom of B
      to the bottom of A. */
  REGION_BOTTOM,
  /** This rectangle has the height of A and extends from the left of B to
      the left of A. */
  REGION_LEFT,
  /** This rectangle has the height of A and extends from the right of A to 
      the right of B. */
  REGION_RIGHT,
  /** This rectangle is the same as A. */
  REGION_INTERIOR
} TRectRegion;

/** Contains the data required to describe a rectangular area.  It is used
    throughout the graphical user interface and map engine.  X pixel 
    coordinates extend from xmin through xmax, and Y pixel coordinates 
    extend from ymin through ymax. */
typedef struct TRect {
  /** Left most rectangle x pixel coordinate. */
  INT32 xmin;
  /** Bottom rectangle y pixel coordinate (inclusive). */
  INT32 ymin;
  /** Right most rectangle x pixel coordinate (inclusive). */
  INT32 xmax;
  /** Top most rectangle x pixel coordinate (inclusive). */
  INT32 ymax;
} TRect;

/** Find the intersection of two rectangular regions if such an intersection
    exists.

    @param outputRect Pointer to a TRect structure to contain the output 
    rectangular region representing the intersection.

    @param inputRect1 Pointer to a TRect structure containing the first 
    rectangle (must be valid according to rect_valid).

    @param inputRect2 Pointer to a TRect structure containing the second 
    rectangle (must be valid according to rect_valid).

    @return If the function returns FALSE then no intersection exists and
    the contents of outputRect are undefined. */
extern MAPCORE_API BOOL8 rect_clip( TRect *outputRect,
                                    const TRect *inputRect1,
                                    const TRect *inputRect2 );
  
/** Find whether two rectangular regions intersect.  Unlike rect_clip(),
    it does not compute the actual intersection.

    @param inputRect1 Pointer to a TRect structure containing the first 
    rectangle (must be valid according to rect_valid).

    @param inputRect2 Pointer to a TRect structure containing the second 
    rectangle (must be valid according to rect_valid).

    @return Function returns TRUE if rectangles intersect and FALSE if
    they don't. */
extern MAPCORE_API BOOL8 rect_intersect( const TRect *inputRect1,
                                         const TRect *inputRect2 );
  
/** Expand a given rectangle to include the point (x, y).

    @param rect rectangle to expand (input/output parameter)

    @param x value to expand in X.

    @param y value to expand in Y.  */
extern MAPCORE_API void rect_expand( TRect *rect,
                                     INT32 x,
                                     INT32 y );
  
/** Given a rectangular region 'parent' and another region completely 
    enclosed by parent, this function calculates the 4 rectangular 
    regions that represent everything in the parent outside the child.  
    Four rectangular regions are returned representing the top, bottom, 
    left and right leftover areas.

    @param parent Points to a TRect structure containing the parent 
    rectangle (which must be valid according to rect_valid).

    @param child Points to a TRect structure containing the rectangle that 
    is cut of the parent.  Child must be valid according to rect_valid.

    @param r Points to an array of 4 TRect structures to contain the output
    rectangles.  The rectangles in the output array correspond to the first 
    four rectangles of the TRectRegion enumerated values.

    @note
    THE RECTANGLES RETURNED ARE NOT NECESSARILY VALID AND SHOULD BE CHECKED 
    USING THE rect_valid FUNCTION BEFORE THEY ARE USED. */
extern MAPCORE_API void rect_leftovers( const TRect *parent,
                                        const TRect *child,
                                        TRect *r );

/** Validates a rectangle.

    @param r Points to a Trect structure containing the rectangle to be 
    validated.

    @return TRUE if the rectangle concerned appears to be valid, otherwise 
    returns FALSE.  Valid rectangles have xmin <= xmax and ymin <= ymax. */
static INLINE BOOL8 rect_valid( const TRect *r ) {
  return (BOOL8)(r->xmin <= r->xmax && r->ymin <= r->ymax);
}

/** This is a convenient macro used to initialise a TRect structure to 
    specific values.

    @todo
    NOTE THAT THE PARAMETERS ARE PASSED IN AN ORDER THAT IS INCONSISTENT 
    WITH THE ORDER OF STRUCTURE MEMBERS OF TRect.  THIS SHOULD BE CHANGED 
    IN FUTURE BUT WILL REQUIRE MODIFICATIONS TO ALL CODE THAT USES THIS 
    FUNCTION.

    @param xmi X min value

    @param xma X max value (inclusive)

    @param ymi Y min value (inclusive)

    @param yma Y max value

    @param r Points to a TRect that on exit contains a rectangle enclosing 
    all values min through max. */
static INLINE void rect_make( INT32 xmi,
                              INT32 xma,
                              INT32 ymi,
                              INT32 yma,
                              TRect *r ) {
  r->xmin = xmi;
  r->xmax = xma;
  r->ymin = ymi;
  r->ymax = yma;
  rect_assertValid(r);
}

/** Translates a rectangle structure by an x,y amount.

    @param s Points to a TRect structure to translate.

    @param x Translation amount in the x direction.

    @param y Translation amount in the y direction. */
static INLINE void rect_trans( TRect *s,
                               INT32 x,
                               INT32 y ) {
  rect_assertValid(s);
  s->xmin += x;
  s->xmax += x;
  s->ymin += y;
  s->ymax += y;
}
  
/** Similar to the translate function but the output rectangle is different 
    to the input rectangle.  The input rectangle is not altered.

    @param d Points to the destination rectangle to contain a translated 
    version of the source.

    @param s Points to the source rectangle that is translated and place in 
    the destination.

    @param x Translation amount in the x direction.

    @param y Translation amount in the y direction. */
static INLINE void rect_add( TRect *d,
                             const TRect *s,
                             INT32 x,
                             INT32 y ) {
  rect_assertValid(s);
  d->xmin = s->xmin + x;
  d->xmax = s->xmax + x;
  d->ymin = s->ymin + y;
  d->ymax = s->ymax + y;
}

/** Enlarges a rectangular region.

    @param r Points to the rectangle to be enlarged.

    @param e The amount by which the rectangle is enlarged in all 
    directions.  If e is negative then the rectangle is shrunk. */
static INLINE void rect_enlarge( TRect *r, INT32 e ) {
  rect_assertValid(r);
  r->xmin -= e;
  r->xmax += e;
  r->ymax += e;
  r->ymin -= e;
}

/** Copies a rectangle structure.

    @param d Points to a TRect structure to contain the destination 
    rectangle.

    @param s Points to a TRect structure containing the source rectangle. */
static INLINE void rect_cpy( TRect *d, const TRect *s ) {
  rect_assertValid(s);
  d->xmin = s->xmin;
  d->xmax = s->xmax;
  d->ymin = s->ymin;
  d->ymax = s->ymax;
}

/** Determines whether a point lies inside a rectangle.

    @param r Points to a rectangle to test the point against.

    @param x X coordinate of the candidate point.

    @param y Y coordinate of the candidate point.

    @return TRUE if the point x,y is contained within the rectangles 
    area. */
static INLINE BOOL8 rect_inside( const TRect *r,
                                 INT32 x,
                                 INT32 y ) {
  rect_assertValid(r);
  return (BOOL8)((x >= r->xmin) && 
    (x <= r->xmax) && 
    (y >= r->ymin) && 
    (y <= r->ymax));
}
  
/** Determine whether rectangle r1 is completely inside rectangle r2.

    @param r1 Pointer to the rectangle to be insideness tested.

    @param r2 Pointer to the potentially enclosing rectangle.

    @return TRUE if r1 is completely inside r2, otherwise return FALSE */
static INLINE BOOL8 rect_insideRect( const TRect *r1, const TRect *r2 ) {
  rect_assertValid(r1);
  rect_assertValid(r2);
  return (BOOL8)((r1->xmin >= r2->xmin) && (r1->xmax <= r2->xmax) &&
    (r1->ymin >= r2->ymin) && (r1->ymax <= r2->ymax));
}

/// Creates a rectangle from a given position and a half size in X & Y
///
///  @param[out] r     Pointer to the rectangle to be initialised
///  @param[in]  p     Pointer to the start position
///  @param[in]  hsX   Half size in the X direction
///  @param[in]  hsY   Half size in the Y direction
///
static INLINE void rect_makeFromPos( TRect *r, 
                                     const TPos *p,
                                     UINT32 hsX,
                                     UINT32 hsY ) {

  r->xmin = p->x - hsX;
  r->xmax = p->x + hsX;
  r->ymin = p->y - hsY;
  r->ymax = p->y + hsY;
  rect_assertValid(r);

}

/// @}
/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif
