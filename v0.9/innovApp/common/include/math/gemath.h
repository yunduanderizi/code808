#ifndef GEMATH_H
#define GEMATH_H

// this module provides a number of generic integer maths functions
// that are not provided in the language specification or in standard libraries.

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Math
/// This module provides a number of generic integer maths functions
/// that are not provided in the language specification or in standard
/// libraries.
/// @ingroup Utilities
/// @{

/** These are the kinds of intersections that can be returned by
    imath_boxSegmentIntersect

    @todo
    MAYBE LATER EXPAND RESULTS TO SPECIFY TOP, BOTTOM LEFT, RIGHT 
    ETC. */
typedef enum {
  /** no intersection with the line segment and the box at all */
  BOX_INTERSECT_NONE = 0,
  /** the linse segments intersects with the box but is not fully enclosed 
  by it */
  BOX_INTERSECT_BOUNDARY,
  /** the line segment is fully enclosed inside the box */
  BOX_INTERSECT_ENCLOSED
} TBoxIntersect;

/** Square root algorithm based on a free integer sqrt algorithm from:

    @par
    http://www.tu-chemnitz.de/~arndt/joerg.htmlinline.

    @param x 32 bit unsigned argument.

    @return The integer square root of the argument (discard decimals) */
extern MAPCORE_API UINT32 imath_sqrt( UINT32 x );
  
/** 64 bit integer square root

    @param x 64 bit unsigned argument.

    @return The integer square root of the argument (discard decimals) */
extern MAPCORE_API UINT32 imath_sqrt64( UINT64 x );

/** Calculate the length of a vector
    @param x X component of the vector.

    @param y Y component of the vector.

    @return Returns the length of the vector. */
extern MAPCORE_API UINT32 imath_vectorLength( INT32 x, INT32 y );

/** Find the length of a vector using a 64 bit square root.

    @note Be aware of side effects, x & y will be evaluated twice each. */
#define imath_vectorLength64( x, y )                                      \
  imath_sqrt64( (UINT64)( (INT64)(x)*(x) + (INT64)(y)*(y) ) )

/** Inverse tangent function that takes into account the line direction
    vector when calculating angles.  This function uses an internal atan 
    lookup table.

    @param[in] y y Component of the line direction vector.

    @param[in] x x Component of the line direction vector.

    @return The angle between a line up the positive y axis and a line from 
    the origin through the given point (x,y).  Angles are byte values from
    0 - 255 (0 - 360) degrees CLOCKWISE (note the difference between the 
    sense of the angle returned by this function and the sense of the 
    angles used by sin and cos).  A value of zero represents NORTH UP. */
extern MAPCORE_API UINT8 imath_atan2( INT32 y, INT32 x );

/** Calculates the inverse tangent.

    @param x 20.12 signed fixed point argument (The argument is equivalent 
    to 4096 * atan(angle) ).

    @return The angle corresponding to the fixed point value.  Angles are 
    byte values from 0 - 255 (0 - 360) degrees CLOCKWISE (note the 
    difference between the sense of the angle returned by this function and 
    the sense of the angles used by sin and cos).  A value of zero 
    represents NORTH UP.

    @par
    THIS FUNCTION CAN ONLY RETURN ANGLES WITHIN 1 PERIOD OF THE TAN CURVE.

    @todo THIS MACRO WILL BE REPLACED WITH A FUNCTION WHEN GLOBAL DATA 
    POINTERS ARE REMOVED.  WE SHOULD CHANGE THIS FUNCTION TO PRODUCE 
    ANGLES CONSISTENT WITH THE SIN AND COS FUNCTIONS. */
extern MAPCORE_API INT16 imath_atan( UINT8 x );

/// A 1.16 fixed point number representing the point on a line at
/// which an intersection occurs as a proportion of the line's length.
/// Values of this type should always be between 0 and 1, inclusive.
typedef INT32 TIntersectionPoint;

/// The number of bits in the fractional part of intersection point
/// values.
#define INTERSECTION_POINT_FRACTIONAL_BITS 16

/// The value of a #TIntersectionPoint that corresponds to 1.
#define MAX_INTERSECTION_POINT (1 << INTERSECTION_POINT_FRACTIONAL_BITS)

/** Determines whether two FINITE line segments intersect anywhere 
    BETWEEN their start and end points.

    @param x1 Start point x of the first line segment.
    @param y1 Start point y of the first line segment.

    @param x2 End point x of the first line segment.
    @param y2 End point y of the first line segment.

    @param x3 Start point x of the second line segment.
    @param y3 Start point y of the second line segment.

    @param x4 End point x of the second line segment.
    @param y4 End point y of the second line segment.

    @param[out] u1 Intersection point if the lines intersect; 1.16
                   fixed point.  Pass \c NULL if this is not required.

    @return TRUE if the line segments intersect, false otherwise. */
extern MAPCORE_API BOOL8 imath_intersect( INT32 x1, INT32 y1,
                                          INT32 x2, INT32 y2,
                                          INT32 x3, INT32 y3,
                                          INT32 x4, INT32 y4,
                                          TIntersectionPoint *u1 );

/* not used! */
#if 0
/** An alternative to imath_intersect that determines whether two FINITE 
    line segments intersect anywhere BETWEEN their start and end points.

    @param a1 Start point of the first line segment.

    @param a2 End point of the first line segment.

    @param b1 Start point of the second line segment.

    @param b2 End point of the second line segment. */
static INLINE void imath_intersectPos(TPos *a1, TPos *a2, TPos *b1, TPos *b2) {
  imath_intersect(a1->x, a1->y, a2->x, a2->y, b1->x, b1->y, b2->x, b2->y);
}
#endif

/** Determines whether the boundaries of a rectangular
    box defined by it's diagonal 2 points intersects with a
    line segment defined by 2 points.

    @param bxmin lower left x coordinate of the box
    @param bymin lower left y coordinate of the box

    @param bxmax top right x coordinate of the box
    @param bymax top right y coordinate of the box

    @param sx1 Start point x of the line segment.
    @param sy1 Start point y of the line segment.

    @param sx2 End point x of the line segment.
    @param sy2 End point y of the line segment.

    @return
    - BOX_INTERSECT_NONE if no intersection between the segment and the box.
    - BOX_INTERSECT_BOUNDARY if segment partially intersects the box.
    - BOX_INTERSECT_ENCLOSED if the segment is inside the box. */
extern MAPCORE_API TBoxIntersect imath_boxSegmentIntersect( INT32 bxmin,
                                                            INT32 bymin,
                                                            INT32 bxmax,
                                                            INT32 bymax,
                                                            INT32 sx1,
                                                            INT32 sy1,
                                                            INT32 sx2,
                                                            INT32 sy2 );

/** Determines whether a point (p) is on the left or right of a line through
    two points (1) and (2).  Points on the left hand side of the line 
    correspond to points in the left hand quadrants when the entire 
    cartesian plane is rotated so that the line from (1) to (2) is in the 
    direction of the positive y axis.

    @param x1 First point x on the line.
    @param y1 First point y on the line.

    @param x2 Second point x on the line.
    @param y2 Second point y on the line.

    @param xp x Coordinate of the point to test.
    @param yp y Coordinate of the point to test.

    @return Only the sign of the result is actually used.
    - < 0: p is on the left of the line from 1 -> 2
    - > 0: p is on the right of the line from 1 -> 2.
    - == 0: p is sitting on the line */
extern MAPCORE_API INT64 imath_calcLineLeftOrRight( INT32 x1,
                                                    INT32 y1,
                                                    INT32 x2,
                                                    INT32 y2,
                                                    INT32 xp,
                                                    INT32 yp );

/** Calculates the closest point on the line segment joining (1) and (2) 
    to point (p).

    @param xi On exit contains the x coordinate of the closest point on the 
    line.
    @param yi On exit contains the y coordinate of the closest point on the 
    line.

    @param x1 x coordinate of point (1) at the start of the line segment.
    @param y1 y coordinate of point (1) at the start of the line segment.

    @param x2 x coordinate of point (2) at the end of the line segment.
    @param y2 y coordinate of point (2) at the end of the line segment.

    @param xp x coordinate of the point near the line.
    @param yp y coordinate of the point near the line.

    @return The function returns an 8 bit value between 0 and 255 that 
    describes how far along the line the intersection point i is located.
      
    @note Return value == 0 if the intersection point is at point (1).
    @note Return value == 256 if the intersection point is at point (2). */
extern MAPCORE_API UINT16 imath_calcLinePerpendicularIntersect( INT32 *xi,
                                                                INT32 *yi,
                                                                INT32 x1,
                                                                INT32 y1,
                                                                INT32 x2,
                                                                INT32 y2,
                                                                INT32 xp,
                                                                INT32 yp );

/** Finds the point where two (non-parallel) line segments would intersect 
    if they were continuous lines.

    @param xi On exit contains the x coordinate of any intersection point.  
    If the lines are parallel then xi contains the average of ax and cx.
    @param yi On exit contains the y coordinate of any intersection point.  
    If the lines are parallel then xi contains the average of ay and cy.

    @param ax The x coordinate of the first point through which the first 
    line passes.
    @param ay The y coordinate of the first point through which the first 
    line passes.

    @param bx The x coordinate of the second point through which the first 
    line passes.
    @param by The y coordinate of the second point through which the first 
    line passes.

    @param cx The x coordinate of the first point through which the first 
    line passes.
    @param cy The x coordinate of the first point through which the first 
    line passes.

    @param dx The x coordinate of the first point through which the first 
    line passes.
    @param dy The x coordinate of the first point through which the first 
    line passes. */
extern MAPCORE_API void imath_lineExtrapolateIntersect( INT32 *xi,
                                                        INT32 *yi,
                                                        INT32 ax,
                                                        INT32 ay,
                                                        INT32 bx,
                                                        INT32 by,
                                                        INT32 cx,
                                                        INT32 cy,
                                                        INT32 dx,
                                                        INT32 dy );

/** Calculate the cosine of an angle.

    @param x The angle as a byte value from 0 - 255 (0 - 360) degrees 
    ANTICLOCKWISE.  A value of zero represents a line along the +x axis.

    @return The cosine of an angle as a 20.12 fixed point quantity ( i.e. 
    4096 * cos(angle) ). */
extern MAPCORE_API INT16 imath_cos( UINT8 x );

/** Calculate the sine of an angle.

    @param x The angle as a byte value from 0 - 255 (0 - 360) degrees 
    ANTICLOCKWISE.  A value of zero represents a line along the +x axis.

    @return The sine of an angle as a 20.12 fixed point quantity ( i.e. 
    4096 * sin(angle) ). */
extern MAPCORE_API INT16 imath_sin( UINT8 x );

/** Calculate log base 2

    @param x input integer value

    @return Returns floor value of log base 2 of input x integer 
    For example: log2(1) -> 0, log2(2) -> 1, log2(3) -> 1, log2(4) -> 2...
    Although log2(0) is undefined in theory, function will return 0. */
extern MAPCORE_API INT32 imath_log2( UINT32 x );

/** Calculate log base 2 for integer values less than 4096. For small
    constants, use of the following macro may allow computation of the
    log value at compile time.

    @param x input integer value.
    @return Returns floor value of log base 2 of input x integer.
*/
#define LOG2(n)                                 \
  (assert((n) < 4096),                          \
   assert((n) > 0),                             \
   (((n) < 32)                                  \
    ?                                           \
      (((n) < 8)                                \
       ?                                        \
         (((n) < 2)                             \
          ?                                     \
            0                                   \
          :                                     \
            (((n) < 4) ? 1 : 2)                 \
          )                                     \
       :                                        \
         (((n) < 16) ? 3 : 4)                   \
       )                                        \
    :                                           \
      (((n) < 512)                              \
       ?                                        \
         (((n) < 128)                           \
          ?                                     \
            (((n) < 64) ? 5 : 6)                \
          :                                     \
            (((n) < 256) ? 7 : 8)               \
          )                                     \
       :                                        \
         (((n) < 1024)                          \
          ?                                     \
            9                                   \
          :                                     \
            (((n) < 2048) ? 10 : 11)            \
          )                                     \
       )                                        \
    )                                           \
   )

/** Calculates rounded value of EXP(x) in 24.8 fixed point format
    where x is a 24.8 signed quantity.

    Input values outside this range are clipped to these limits.

    @param x input signed value in 24.8 fixed point.  exp is 
    correctly computed when input x is in between -1600 to +1600
    (which corresponds to -6.25 to +6.25) but function will saturate
    when x is beyond those values.

    @return exp() of input value in 24.8 fixed point format. */
extern MAPCORE_API INT32 imath_exp( INT32 x );

/** Check whether 2 64 bits integers have the same sign.

    @param a first 64 bits integer

    @param b second 64 bits integer

    @return TRUE if a and b have same sign, FALSE otherwise. */
#define imath_sameSign64(a, b) (((INT64)((UINT64)a ^ (UINT64)b)) >= 0)
  
/** Check whether 2 32 bits intergers have the same sign.

    @param a first 32 bits integer

    @param b second 32 bits integer

    @return TRUE if a and b have same sign, FALSE otherwise. */
#define imath_sameSign(a, b) ((INT32)((UINT32)a ^ (UINT32)b) >= 0)

/** Returns the minimum of x and y.

    @param x Any signed or unsigned type as argument 1.

    @param y Any signed or unsigned type as argument 2.

    @return minimum of x and y */
//static INLINE INT32 imath_min(INT32 x, INT32 y) {
//  return x < y ? x : y;
//}
#define imath_min(x,y) ((x) < (y) ? (x) : (y))

/** Returns the maximum of x and y.

    @param x Any signed or unsigned type as argument 1.

    @param y Any signed or unsigned type as argument 2.

    @return minimum of x and y */
//  static INLINE INT32 imath_max(INT32 x, INT32 y) {
//    return x > y ? x : y;
//  }
#define imath_max(x,y) ((x) > (y) ? (x) : (y))

/** Return the absolute value.

    @note This is a macro, need to be aware of side effects.

    @param x Signed value for which the absolute value is required.

    @return The absolute value of x. */
#define imath_abs(x) ((x) < 0 ? (-(x)) : (x))





/// @}

#ifdef __cplusplus
}
#endif

#endif
