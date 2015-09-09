#ifndef MTANGLEP_H
#define MTANGLEP_H

#include <math/mtangle.h>
#include <platform.h>
#include <innovEngine_types.h>

#ifdef __cplusplus
extern "C" {
#endif

// Basic types. Note that TAngle and THeading are identical
typedef UINT32 TAngle;
typedef TAngle THeading;
typedef struct TDeflection {
  TAngle angle;
  BOOL8 clockwiseOrZero;
} TDeflection; 

// All headings and angles must be in the range [0..ANGLE_RANGE)
#define ANGLE_RANGE 256
#define ANGLE_360 (ANGLE_RANGE)
#define ANGLE_180 (ANGLE_RANGE >> 1)
#define ANGLE_90 (ANGLE_RANGE >> 2)
#define ANGLE_45 (ANGLE_RANGE >> 3)
#define ANGLE_0 0
#define ANGLE_REVERSE(a) (((a)+(ANGLE_RANGE/2))%ANGLE_RANGE)
#define ANGLE_NEGATIVE(a) (ANGLE_RANGE-(a)-1)

//#ifndef STATICMETHOD
//#define STATICMETHOD
//#endif

/** @internal
    Return a heading from point 1 to point 2 in the range [0..ANGLE_RANGE)
    p1 and p2 are assumed to be in the same scale, e.g., metres (not lat/long!)

    @param[in] p1
    @param[in] p2
    @return Heading in range [0..ANGLE_RANGE) from point 1 to point 2. */
STATICMETHOD THeading Heading_New(const TPos p1, const TPos p2);

/** @internal
    Take a heading and rotate it 180 (real) degrees

    @param[in][out] heading.  Heading is assumed to be in the range [0..ANGLE_RANGE)
    @return Heading in range [0..ANGLE_RANGE) from point 1 to point 2. */
THeading Heading_Rotate180(const THeading heading);

/** @internal
    Take a heading and add an angle. Angles are assumed to be
    clockwise and in the range [0..ANGLE_RANGE)

    @param[in] heading.  Heading is assumed to be in the range 0..ANGLE_RANGE-1.
    @param[in] angle.  Angle is assumed to be in the range 0..ANGLE_RANGE-1.
    @return Heading in range [0..ANGLE_RANGE). */
THeading Heading_AddAngle(const THeading heading, const TAngle angle);

/** @internal
    Take a heading and subtract an angle. Angles are assumed to be
    clockwise and in the range [0..ANGLE_RANGE)

    @param[in] heading.  Heading is assumed to be in the range 0..ANGLE_RANGE-1.
    @param[in] angle.  Angle is assumed to be in the range 0..ANGLE_RANGE-1
    @return Heading in range [0..ANGLE_RANGE). */
THeading Heading_SubtractAngle(const THeading heading, const TAngle angle);

/** @internal
    Get the internal angle difference between heading heading1 and heading2

    Note that the difference between two headings takes the smallest 
    (internal) angle. E.g.,consider

    heading1 = 192
    heading2 = 96

                      external
    heading1 --------x
             internal \
                       \
                      heading2

    Then the internal angle is 96, the external angle is 160. The
    return value is 96.
 
    @param[in] heading1
    @param[in] heading2
    @return Difference in heading in the range [0..ANGLE_RANGE/2].

    @note This corresponds to the old guidance routine getHeadingDifference. */
TAngle Heading_GetInternalAngleTo(const THeading heading1,
                                  const THeading heading2);

/** @internal
    Get the difference between heading heading1 and heading2 calculated
    clockwise from heading1 to heading2.

    @param heading1 (input)
    @param heading2 (input)
    @return difference in heading in the range [0..ANGLE_RANGE). */
TAngle Heading_GetClockwiseDifference(const THeading heading1,
                                      const THeading heading2);

/** @internal
    Create a new deflection from an angle and whether it is to be considered 
    clockwise or not.  If the angle is zero, then the clockwise variable is 
    ignored in generating the deflection.

    @param[in] angle
    @param[in] clockwise.  Whether the angle is clockwise.  Ignored if the angle 
    is zero.
    @return a deflection. */
STATICMETHOD TDeflection Deflection_New(const TAngle angle,
                                        const BOOL8 clockwise);

/** @internal
    Normalises a deflection (which is both a deflection angle
    and a clockwise/anticlockwise direction) into an angle which
    is always clockwise.

    @param[in] deflection
    @return a clockwise angle in the range [0..ANGLE_RANGE) that represents
    the input deflection. */
TAngle Deflection_ToNormalisedAngle(const TDeflection deflection);

/** @internal
    Returns the (absolute) angle of deflection. The information
    about whether the angle is clockwise or not is discarded. That
    is, an angle of 30 anticlockwise is returned as and angle 30 (with
    assumed clockwise direction).

    @param[in] deflection (input)
    @return the absolute deflection angle. */
TAngle Deflection_Angle(const TDeflection deflection);

/** @internal
    Whether a deflection is strictly clockwise or not.

    @param deflection (input)
    @return TRUE if a deflection is strictly clockwise. */
BOOL8 Deflection_IsClockwise(const TDeflection deflection);

/** @internal
    Whether a deflection is strictly clockwise or zero
    (i.e., no deflection)

    @param[in] deflection
    @return TRUE if a deflection is clockwise or zero. */
BOOL8 Deflection_IsClockwiseOrZero(const TDeflection deflection);

/** @internal
    Whether a deflection is strictly anticlockwise or not.

    @param[in] deflection
    @return TRUE if a deflection is anti-clockwise. */
BOOL8 Deflection_IsAntiClockwise(const TDeflection deflection);

/** @internal
    Whether a deflection is strictly anticlockwise or zero
    (i.e., anticlockwise or no deflection)

    @param[in] deflection
    @return TRUE if a deflection is anti-clockwise or zero. */
BOOL8 Deflection_IsAntiClockwiseOrZero(const TDeflection deflection);

/** @internal
    whether a deflection is strictly clockwise or zero
    (i.e., no deflection)

    @param[in] deflection (input)
    @return TRUE if a deflection is clockwise. */
BOOL8 Deflection_IsClockwiseOrZero(const TDeflection deflection);

/** @internal
    Determine the angle between heading1 and heading2.
    This relative heading is set to a negative value if heading1 is
    to the left of heading2, positive otherwise. 

    The determination of whether the heading is
    to the left or to the right is determined by the source
    heading. Consider a path taken through an intersection,
    starting from the direction of the source heading
    and exiting via the reference heading. The direction
    of the relative heading is considered with reference
    to that path. For example, consider (when ANGLE_RANGE = 256)
    sourceHeading = 192
    heading = 160
    referenceHeading = 96


    sourceHeading --------x
                         / \
                        /   \
                 heading     referenceHeading

    The relative heading returned would be to the right and
    hence the returned result would be 64.

    @param[in] sourceHeading
    @param[in] heading
    @param[in] referenceHeading.  All input headings are expected within the 
    range [0..ANGLE_RANGE)

    @return The deflection heading between heading1 and the reference
    heading. The deflection contains both the relative angle
    and whether the deflection is clockwise or anticlockwise. */
STATICMETHOD TDeflection Deflection_Calculate(const THeading sourceHeading,
                                              const THeading heading,
                                              const THeading referenceHeading);

#ifdef __cplusplus
}
#endif

#endif /* MTANGLEP_H */
