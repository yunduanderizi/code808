//
// Declaration of the journey calculator and functions for manipulating it
//
// $Id: rtjourneycalc.h 10429 2008-07-02 02:49:42Z marcus.spranger $
//

#ifndef RTJOURNEYCALC_H
#define RTJOURNEYCALC_H

#ifdef __cplusplus
extern "C" {
#endif
/// @addtogroup Routing
/// @{

/// @addtogroup JourneyCalculator
/// @ingroup Routing
/// @{

/*----------------------------------------------------------------*/
/* JOURNEY TYPES */
/*----------------------------------------------------------------*/
// A Journey is the logical partner of the itinerary type.  A journey
// calculator takes an itinerary as input and produces a journey as output.
//
// A journey is fundamentally related to a particular road network.  It
// contians paths showing how to traverse the network in a way that follows
// the input itinerary.
//
// +--------------------+
// |                    |
// |      Journey       |--+
// +--------------------+  |  
//    |     Journey Leg    |--+
//    +--------------------+  |
//       |        Path        |
//       +--------------------+
//
//## Similarly to the intinerary type, this slightly redundant approach is
//## chosen because of future plans to have alternative paths for the same
//## leg of the journey


/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// @typedef enum TJourneyCalculatorResult
/// Return type for the journeyCalculator initialise() and iterate()
/// functions.  Negative values indicate errors.
typedef enum {
  /// Bad data passed in as a parameter.  (e.g. bad index, empty itinerary)
  JOURNEYCALC_RESULT_BAD_PARAM = -4,
  /// APIs called out of order.  (e.g. journeyCalculator must be
  /// initialised before it is iterated)
  JOURNEYCALC_RESULT_BAD_SEQUENCE = -3,
  /// The journey calculator has finished, but one or more legs had
  /// calculation errors
  JOURNEYCALC_RESULT_ERROR = -1,
  /// Initialisation has completed okay, ready to start iterating
  JOURNEYCALC_RESULT_READY_TO_START = 0,
  /// The lowest not finished return value from journeyCalculator_iterate()
  /// Values from 2 to 254 represent progressive levels of completion
  JOURNEYCALC_RESULT_STARTED = 1,
  /// The journey calculator has finished, all legs were computed without
  /// error
  JOURNEYCALC_RESULT_FINISHED = 255
} TJourneyCalculatorResult;

/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif // RTJOURNEYCALC_H
