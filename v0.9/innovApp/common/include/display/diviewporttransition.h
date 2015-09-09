#ifndef DIVIEWPORTTRANSITION_H
#define DIVIEWPORTTRANSITION_H

#include <innovEngine_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Display
/// @{

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

/// @addtogroup DisplayTransition
/// @ingroup UnUsed
/// @{

///
/// Identifies a viewport transition
///
typedef enum ViewTransitionType
{
  /// The viewport didn't change at all
  StaticView,
  /// The viewport was panned in 2D
  Panning2DView,
  /// The viewport was panned in 3D
  Panning3DView,
  /// The viewport still is 2D but a transformation other than panning occured
  Other2DView,
  /// The viewport still is 3D but a transformation other than panning occured
  Other3DView,
  /// The viewport changed in a way difficult to interpret.
  UnknownTransition
} ViewTransitionType;

///
/// Identifies a viewport configuration
/// A viewport configuration is a snapshot of a viewport at a particular moment.
/// It does not hold a reference on the viewport it's been created from (TODO: change that?)
///
typedef struct ViewportConfiguration
{
  BOOL8 displayType; /// 0: 2D display 1: 3D display
  UINT32 mapSize;
  UINT8 mapAngle;
  TPos screenCenterLatLong;
} ViewportConfiguration;

/// @}

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif
