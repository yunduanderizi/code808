#ifndef ENSIM_H
#define ENSIM_H

#include <locationprovider/lpjourneyloc.h>
#include <guidance/gdlist.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Simulator
/// This module creates synthetic NMEA data, and uses this to simulate
/// the driving of the current journey. The simulator "hijacks" any valid
/// NMEA messages, and replaces them with messages containing successive
/// positions that are calculated from the journey.
/// @ingroup LocationProviders
/// @{

/// The absolute maximum simulator speed that may be set (in m/s x 100) is
/// equal to the current land speed record. The current wheel driven internal
/// combustion engine powered land speed record was set by Bob Summers driving
/// Goldenrod at Bonneville Salt Flats (UT), United States, on November 13,
/// 1965. 409.277 mph (658.667 km/h) = 182.963 m/s = 18296 cm/s.
#define SIMULATOR_MAX_SPEED    18296

/// @deprecated
/// The default simulator speed in m/s. This is approximately 100 km/h.
#define SIMULATOR_DEFAULT_SPEED 28

/// Speed limits in most countries are different for urban roads, rural roads,
/// and motorways/freeways.
typedef enum {
  /// Speed limit for areas where pedestrians and motor traffic share the same
  /// space (carparks etc). E.g. 20 km/h.
  SPEED_LIMIT_SHARED_ZONE,
  /// Speed limit for urban and residential roads (built-up areas).
  /// E.g. 50 km/h.
  SPEED_LIMIT_URBAN,
  /// Speed limit for rural roads (open road, outside built-up areas/towns).
  /// E.g. 80 km/h.
  SPEED_LIMIT_RURAL,
  /// Speed limit for freeways and motorways. E.g. 120 km/h.
  SPEED_LIMIT_MOTORWAY,
  /// Speed limit for unsealed roads
  SPEED_LIMIT_UNSEALED,
  /// The total number of speed limit types.
  NUM_SPEED_LIMITS
} TRoadSpeedLimitType;


extern MAPCORE_API BOOL8 journeySimulator_reset(const TJourney *pJourney);

extern MAPCORE_API
BOOL8 journeySimulator_resetGuide(const TGuideList *pGuideList);

extern MAPCORE_API BOOL8 simulator_updateMS(INT32 time);

extern MAPCORE_API INT32 simulator_getSpeed(void);

extern MAPCORE_API BOOL8 simulator_setLocation(const TJourneyLocation *pLoc);

extern MAPCORE_API BOOL8 simulator_getLocation(TJourneyLocation *pLoc);

extern MAPCORE_API BOOL8 simulator_jumpToNextTurn(void);

extern MAPCORE_API BOOL8 simulator_jumpToPreviousTurn(void);

extern MAPCORE_API
UINT32 simulator_setSpeedLimit(TRoadSpeedLimitType speedLimitType,
                               UINT32 speedLimit);

extern MAPCORE_API
UINT32 simulator_getSpeedLimit(TRoadSpeedLimitType speedLimitType);

extern MAPCORE_API
BOOL8 tunnel_simulator_setLocation(const TTunnelSimLocation *pLoc);

extern MAPCORE_API
void tunnel_simulator_reset(void);

extern MAPCORE_API
BOOL8 tunnel_simulator_getLocation(TTunnelSimLocation *pLoc);

extern MAPCORE_API
BOOL8 tunnel_simulator_update(void);

extern MAPCORE_API
BOOL8 IsTunnelSimulationEx(TMapLocation *location, TTunnelSimLocation *pThis);

extern MAPCORE_API
void tunnel_simulator_setSpeed(UINT32 _speed);

DEPRECATED(
extern MAPCORE_API BOOL8 simulator_init(void)
);

DEPRECATED(
extern MAPCORE_API BOOL8 simulator_update(UINT16 time)
);

#ifndef NO_DEPRECATED_ROUTING

DEPRECATED(
extern MAPCORE_API BOOL8 simulator_reset(UINT32 UTC)
);

#endif /* NO_DEPRECATED_ROUTING */

DEPRECATED(
extern MAPCORE_API INT32 simulator_setSpeed(INT32 speedMetrePerSec)
);

/// @}

/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------*/

#endif
