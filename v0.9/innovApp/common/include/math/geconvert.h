#ifndef GECONVERT_H
#define GECONVERT_H

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

// TDistanceCombination was a bad name, and it clashes with TDistanceUnits
//#define TDistanceCombination TDistanceCombination

/// The maximum number of rounding rules allowed in the system
/// Additional insertions beyond this number will be ignored
#define MAX_NUMBER_ROUNDING_RULES 10

/** Available distance units. Nautical units are alpha only. 
    DIST_UNIT_METRIC uses kilometers and meters
    DIST_UNIT_IMPERIAL uses the default imperial for the language
    DIST_UNIT_IMPERIAL_FEET uses miles and feet
    DIST_UNIT_IMPERIAL_YARDS uses miles and yards
    DIST_UNIT_IMPERIAL_DECIMAL uses decimal miles. eg: 0.2 miles
    DIST_UNIT_IMPERIAL_FRACTIONAL uses fractions of a mile. eg: 1/4 mile
    DIST_UNIT_IMPERIAL_METERS uses miles and meters
    DIST_UNIT_NAUTICAL uses nautical miles */
typedef enum {
  DIST_UNIT_METRIC,
  DIST_UNIT_METRIC_COARSE,
  DIST_UNIT_IMPERIAL,
  DIST_UNIT_IMPERIAL_FEET,
  DIST_UNIT_IMPERIAL_YARDS,
  DIST_UNIT_IMPERIAL_DECIMAL,
  DIST_UNIT_IMPERIAL_FRACTIONAL,
  DIST_UNIT_IMPERIAL_METERS,
  DIST_UNIT_NAUTICAL
} TDistanceCombination;

typedef enum {
  DIST_SUBUNIT_FRACTION,
  DIST_SUBUNIT_DECIMAL
} TSubDistanceUnit;

/// The following types and conversion API allow a distance (in metres) to
/// be present to the user sensibly rounded and converted to familiar
/// units.

/// Enumeration of supported fractional values
typedef enum {
  /// Should only occur with DISTANCEFORMAT_WHOLE
  FRACTION_ZERO,          // =  0 / 24
  FRACTION_ONE_EIGHTH,    // =  3 / 24
  FRACTION_ONE_QUARTER,   // =  6 / 24
  FRACTION_ONE_THIRD,     // =  8 / 24
  FRACTION_ONE_HALF,      // = 12 / 24
  FRACTION_TWO_THIRDS,    // = 16 / 24
  FRACTION_THREE_QUARTERS,// = 18 / 24
  /// only used internally
  FRACTION_ONE_WHOLE      // = 24 / 24
} TFractionValue;

/// Enumeration of user distance styles
typedef enum {
  DISTANCESTYLE_METRIC_1,
  DISTANCESTYLE_IMPERIAL_YARDS_1,
  DISTANCESTYLE_IMPERIAL_FRACTIONAL_1
} TUserDistanceStyle;

/// Enumeration of number types
typedef enum {
  DISTANCEFORMAT_WHOLE,
  DISTANCEFORMAT_DECIMAL,
  DISTANCEFORMAT_FRACTION
} TDistanceFormat;

/// Enumeration of distance metrics
typedef enum {
  DISTANCEUNITS_KILOMETRES,
  DISTANCEUNITS_METRES,
  DISTANCEUNITS_MILES,
  DISTANCEUNITS_YARDS,
  DISTANCEUNITS_FEET
} TDistanceUnits;

/// A type representing a converted and rounded distance suitable for
/// presentation to a user.  Depending on the format of the number up to
/// three number fields are used to hold component parts of the number.
typedef struct TUserDistance {
  /// The type of number represented - whole, decimal or fractional
  TDistanceFormat format;

  /// The units in which the distance is represented - m, km, mi, yd or ft
  TDistanceUnits units;

  /// The number of whole units -- always valid 
  /// This field is 0 for distances < 1 and for numbers represented
  /// entirely as a fraction.
  UINT32 wholePart;

  /// The part of the number after the decimal seperator 
  /// -- only valid for format DISTANCEFORMAT_DECIMAL
  UINT32 decimalPart;

  /// Components of the fractional part of the number 
  /// -- only valid for format DISTANCEFORMAT_FRACTION
  TFractionValue fractionalPart;

} TUserDistance;


/// A type representing a method for rounding a distance
/// The rounding rule needs to know the maximum distance that 
/// this rule is valid for, the units to use, and the amount
/// to round to. If the rounding is going to be done to units
/// less than one, the type of sub-unit needs to be specified
typedef struct {
  /// The maximum distance for which this rounding rule is valid.
  /// set to -1 for no limit
  INT32 distanceMax;
  /// The unit to be returned (Meters, miles etc)
  TDistanceUnits unit;
  /// The format of the sub-units (decimal, fractional etc.)
  TSubDistanceUnit subUnit;  
  /// The step size - 10 time the whole number of units to round to.
  /// If the step size is less then 10, sub-units may be returned.
  /// Rounding of the sub-units are from the following table:
  /// step size  | decimal inventory | fractional inventory
  ///     1      | Full (0.1 - 0.9)  | Full (1/4, 1/3, 1/2, 2/3, 3/4)
  ///     2      | 0.2, 0.4, 0.6, 0.8| Quarters (1/4, 1/2, 3/4)
  ///     3      | 0.3, 0.6          | Thirds (1/3, 2/3)
  ///    4-5     | 0.5               | Halves (1/2)
  ///    6-10    | None              | None
  /// Thus a value of 6 through 10 will not use any sub-units for rounding
  /// and return the nearest whole unit. Values above 10 will round the
  /// whole units. A value of 20 will round to the nearest 2. 
  /// Setting step to 100 will round to the nearest 10 etc.
  /// Finally, a value of 1/8 may be used only if step size is 1, and there 
  /// are no whole units.
  UINT32 step;
} TRoundingRule;

//make sure popeye/blackeye apps build.
/** @deprecated
    Language distance units. Metric or imperial. */
#define TDISTUNITS 1
typedef TDistanceCombination TDistUnits;

/** Convert a fixed point or integer distance from miles to kilometres 
    by simple scaling (estimates 1 mile = 1.609375 km).

    @param miles Distance in miles.

    @return Equivalent distance in km. */
extern MAPCORE_API INT32 imath_milesToKm( INT32 miles );

/** Convert a fixed point or integer distance from kilometres to miles 
    by simple scaling (estimates 1 km = 0.62109375 miles).

    @param km Distance in km.

    @return Equivalent distance in miles. */
extern MAPCORE_API INT32 imath_kmToMiles( INT32 km );

/** Convert a fixed point or integer speed from knots to kilometres per 
    hour by simple scaling (estimates 1 knot = 1.852050781 KPH).

    @param knots Speed in knots

    @return Equivalent speed in kilometres per hour. */
extern MAPCORE_API INT32 imath_knotsToKPH( INT32 knots );

/** Convert a fixed point or integer speed from knots to miles per hour 
    by simple scaling (estimates 1 knot = 1.150878906 MPH).

    @param knots Speed in knots.

    @return Equivalent speed in miles per hour. */
extern MAPCORE_API INT32 imath_knotsToMPH( INT32 knots );

/** Convert a fixed point or integer distance from metres to feet by 
    simple scaling (estimates 1 m = 3.280761719 feet).

    @param meters Distance in metres.

    @return Equivalent distance in feet. */
extern MAPCORE_API INT32 imath_metersToFeet( INT32 meters );

/** Convert a fixed point or integer distance from feet to metres by 
    simple scaling (estimates 1 foot = 0.30480957 m).

    @param feet Distance in feet.

    @return Equivalent distance in metres. */
extern MAPCORE_API INT32 imath_feetToMeters( INT32 feet );

/** Convert a fixed point or integer distance from yards to metres by 
    simple scaling (estimates 1 yard = 35/32 meters).

    @param yards Distance in yards.

    @return Equivalent distance in metres. */
extern MAPCORE_API INT32 imath_yardsToMeters(INT32 yards);

/** Convert a fixed point or integer distance from metres to yards by 
    simple scaling (Estimates 1 meter = 117/128 yards).

    @param meters Distance in meters.

    @return Equivalent distance in yards. */
extern MAPCORE_API INT32 imath_metersToYards(INT32 meters);

/** Rounds a meters value to the nearest major unit of
    the specified type.  For example, if the distance type is 
    DIST_IMPERIAL it will round the meters to the nearest mile
    and return the value in meters.

    NOTE: Uses division, so isn't optimised for speed.

    @param distance (input) The distance to be converted
    @param distanceType (input) Currently supported:
    -- DIST_UNIT_METRIC
    -- DIST_UNIT_IMPERIAL
    -- DIST_UNIT_IMPERIAL_FEET
    -- DIST_UNIT_IMPERIAL_YARDS
    -- DIST_UNIT_IMPERIAL_DECIMAL
    -- DIST_UNIT_IMPERIAL_FRACTIONAL
    -- DIST_UNIT_IMPERIAL_METERS
    -- DIST_UNIT_NAUTICAL

    @return the meters rounded to the nearest major unit. */
extern INT32 imath_roundMetersToMajorUnit( const INT32 distance,
                                           const TDistanceCombination distanceType );

/** Convert a fixed point or integer speed from metres per second to knots 
    by simple scaling (estimates 1 knot = 0.514404297 m/s).

    @param knots Speed in knots

    @return Equivalent speed in metres per second. */
extern MAPCORE_API INT32 imath_knotsToMPS( INT32 knots );

/** Convert a fixed point or integer speed from knots to metres per second 
    by simple scaling (estimates 1 mps = 1.94384765625 knot).

    @param mps Speed in metres per second

    @return Equivalent speed in knots. */
extern MAPCORE_API INT32 imath_MPSToKnots(INT32 mps);

/** Convert a fixed point or integer speed from knots to feet per second 
    by simple scaling (estimates 1 knot = 1.687805176 ft/s).

    @param knots Speed in knots.

    @return Equivalent speed in feet per second. */
extern MAPCORE_API INT32 imath_knotsToFPS( INT32 knots );

/** Convert a fixed point or integer speed from kilometres per hour to 
    knots by simple scaling (estimates 1 kph = 0.540039063 knots).

    @param knots Speed in knots

    @return Equivalent speed in kilometres per hour */
extern MAPCORE_API INT32 imath_kphToKnots( INT32 knots );

/** Convert a fixed point or integer speed from kilometres per hour to
    metres per second by simple scaling (estimates 1 kph = 0.27734375m/s).

    @param kph Speed in kilomtres per hour.

    @return Equivalent speed in metres per second */
extern MAPCORE_API INT32 imath_kphToMetresPerSecond(INT32 kph);

/** Convert a fixed point or integer speed from metres per second to
    kph by simple scaling (estimates 1 m/s = 3.6015625 kph).

    @param ms Speed in metres/second.

    @return Equivalent speed in kilometres per hour.  */
  extern MAPCORE_API INT32 imath_metresPerSecondToKPH(INT32 ms);

/** Convert a fixed point or integer speed from miles per hour to knots 
    by simple scaling (estimates 1 kph = 0.868976242 knots).

    @param knots Speed in miles per hour.

    @return Equivalent speed in knots */
extern MAPCORE_API INT32 imath_mphToKnots( INT32 knots );

/// Convert and round a metre distance using one of a number of styles.
///
/// It is expected that this API will be used when presenting distances to
/// the user as a text string.  To avoid localisation issues such as
/// whether ',' or '.' is the decimal seperator, the rounded distance is
/// returned in parts rather than as a string.
///
/// @param[in]  distMetres The distance to convert/round
/// @param[in] style The conversion and rounding style to use
/// @param[out] out The result TUserDistance - either a whole number, a
///             decmial number of a fractional number.
extern MAPCORE_API
void distance_getUserDistance( 
  UINT32 distMetres,
  TUserDistanceStyle style,
  TUserDistance* out );

/// Convert and round a metre distance using one of a number of styles.
///
/// It is expected that this API will be used when presenting distances to
/// the user as a text string.  To avoid localisation issues such as
/// whether ',' or '.' is the decimal seperator, the rounded distance is
/// returned in parts rather than as a string.
///
/// Note: Similar to distance_getUserDistance, except that this function
/// accepts a TDistanceCombination as an input
///
/// @param[in]  distMetres The distance in meters to convert/round
/// @param[in] style The conversion and rounding style to use
/// @param[out] out The result TUserDistance - either a whole number, a
///             decmial number of a fractional number.
void distance_getUserRoundedDistance( 
  UINT32 distMetres,
  TDistanceCombination style,
  TUserDistance* out );

/// Find out the values of the given rounding rule
/// @param[in] index Index of the rounding rule
/// @param[out] out The rounding rule
extern MAPCORE_API
void distance_getRoundingRule( UINT8 index, TRoundingRule* out );

/// Add a rounding rule to the list of rules. It will be inserted into 
/// the list of rules in an order based on the distanceMax field
/// @param[in] in The rounding rule to add
extern MAPCORE_API
void distance_addRoundingRule( TRoundingRule in );

/// remove a rounding rule from the list of rules
/// @param[in] index Index of the rounding rule to remove
extern MAPCORE_API
void distance_deleteRoundingRule( UINT8 index );

/// Find out how many rounding rules are in the current list
/// @return Count of the number of rounding rules
extern MAPCORE_API
UINT8 distance_countRoundingRule( void );

/// Convert and round a metre distance using the current rounding rules.
///
/// It is expected that this API will be used when presenting distances to
/// the user as a text string.  To avoid localisation issues such as
/// whether ',' or '.' is the decimal separator, the rounded distance is
/// returned in parts rather than as a string.
///
/// @param[in]  distMetres The distance to convert/round
/// @param[out] out The result TUserDistance - either a whole number, a
///             decmial number of a fractional number.
extern MAPCORE_API
void distance_getRoundedDistance( 
  UINT32 distMetres,
  TUserDistance* out );

/// Find out if the given distance is equal to zero
/// @param[in] dist A pointer to the user distance
/// @return TRUE if the user distance is zero, otherwise FALSE
extern MAPCORE_API
BOOL8 distance_isZero(  const TUserDistance *dist );

#ifdef UNIT_TESTS
extern MAPCORE_API
INT32 distance_testDistanceConversion( INT32 threshold, INT32* failureDistance );
#endif

/// @}

#ifdef __cplusplus
}
#endif

#endif
