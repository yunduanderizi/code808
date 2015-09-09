//
// Declaration of types and functions for the single stop A* route calculator
//
// $Id: rtpathcalc.h 10429 2008-07-02 02:49:42Z marcus.spranger $
//

#ifndef RTPATHCALC_H
#define RTPATHCALC_H

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @{
/// @addtogroup Path
/// @ingroup Routing
/// @{

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// @internal
/// Path calculator return type
typedef enum {
  /// We can't route to the destination, as it might be dangerous or it's just
  /// not allowed.
  PATHCALC_RESULT_DESTINATION_PROHIBITED = -10,
  /// The distance between start and end location is too big.
  PATHCALC_RESULT_DISTANCE_TOO_BIG = -9,
  /// API called with bad input.
  PATHCALC_RESULT_BAD_PARAM = -8,
  /// API called out of order.
  PATHCALC_RESULT_BAD_SEQUENCE = -7,
  /// Routing ran out of memory.
  PATHCALC_RESULT_OUT_OF_MEM = -6,
  /// No route possible (at least not with the current routing options).
  PATHCALC_RESULT_NOT_CONNECTED = -5,
  /// The end location couldn't be snapped onto the road network.
  PATHCALC_RESULT_BAD_END_LOCATION = -4,      
  /// The start location couldn't be snapped onto the road network.
  PATHCALC_RESULT_BAD_START_LOCATION = -3,
  /// No path to re-route to.
  PATHCALC_RESULT_NO_EXISTING_PATH = -2,
  /// General error condition
  PATHCALC_RESULT_ERROR = -1,
  /// Initialisation okay
  PATHCALC_RESULT_READY_TO_START = 0,
  /// Started calculating
  PATHCALC_RESULT_STARTED = 1,

  // part way through calculation
  /* 2 -> 254 */                           

  /// Finished successfully - a route was found
  PATHCALC_RESULT_FINISHED = 255
} TPathCalculatorResult;

/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif // RTPATHCALC_H
