//
// Declaration of routing related generic ban functions
//
// $Id: rtbans.h 3795 2005-12-22 00:57:27Z TAllsopp $
//

#ifndef RTBANS_H
#define RTBANS_H

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @{


/// @addtogroup  GeobanAreas
/// @ingroup Routing
/// @{

/// Enumerated list of Geoban area types (Geoban areas are areas that
/// cannot be routed through).  Geoban areas are drawn on the map using
/// filled polygons.
typedef enum {
  GEOBAN_NONE = 0,      ///< this area has no geoban
  GEOBAN_DETOUR = 1,    ///< this area/segment is part of a banned route
  GEOBAN_EXPENSIVE = 2, ///< These areas can be used but at a very high cost
  GEOBAN_FORBIDDEN = 3  ///< These areas cannot be used at all
} TPathGeobanType;

/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif // RTBANS_H
