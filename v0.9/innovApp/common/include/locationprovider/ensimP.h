#ifndef ENSIMP_H
#define ENSIMP_H

#include <locationprovider/ensim.h>
#include <guidance/gdlist.h>
#include <locationprovider/lpjourneyloc.h>
#include <math/mtangleP.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  /// @internal
  /// The journey is not valid.
  JOURNEY_SIM_JOURNEY_INVALID = -100,
  /// @internal
  /// The path is invalid.
  JOURNEY_SIM_PATH_INVALID,
  /// @internal
  /// The simulator updated successfully.
  JOURNEY_SIM_OK = 0,
  /// @internal
  /// The simulator is at the end of the journey.
  JOURNEY_SIM_END_OF_JOURNEY
} TJourneySimResult;

typedef enum {
  /// @internal
  /// The path is invalid.
  TUNNEL_SIM_PATH_INVALID = -100,
  /// @internal
  /// The simulator updated successfully.
  TUNNEL_SIM_OK = 0,
  /// @internal
  /// The simulator is at the end of the tunnel.
  TUNNEL_SIM_END_OF_TUNNEL
} TTunnelSimResult;

typedef struct TJourneySimulator {
  /// @internal
  /// The guidance list being simulated.
  const TGuideList *pGuideList;
  /// @internal
  /// Current journey location
  TJourneyLocation loc;
  /// @internal
  /// The journey being simulated.
  const TJourney *pJourney;
  /// @internal
  /// Current speed in metres per second.
  INT32 speed;
  /// @internal
  /// Latitude/longitude coordinates.
  TPos position;
  /// @internal
  /// Heading in degrees (0-255)
  THeading heading;
  /// @internal
  /// Speed limits for each type of road in m/s x 100 (urban, rural,
  /// motorways/freeways, etc.).
  INT32 speedLimit[NUM_SPEED_LIMITS];
  /// @internal
  /// Acceleration in m/s^2 x 100.
  INT32 acceleration;
  /// @internal
  /// Deceleration (breaking performance) in m/s^2 x 100.
  INT32 deceleration;
  /// @internal
  /// Size of the "bubble of influence" around the simulator in m x 100. Roads
  /// and guidance instructions within this distance of the vehicle can
  /// influence the current speed. We don't care about anything further away.
  UINT32 bubbleDistance;
  /// @internal
  /// Distance in m x 100,000 to move the car from the current position. If our
  /// speed is very low and simulator updates are very fast, we need to record
  /// cumulative tiny movements, or we would never move anywhere! (Note: the
  /// smallest distance we can move is 1/100,000 th of a metre. c.f. 1/100 m/s
  /// x 1/1000 s = 1/100,000 m).
  INT32 distanceRem;
  /// @internal
  /// Speed remainder in m/s x 100,000. If our acceleration is set to a very
  /// small value and simulator updates are very fast, we need to record
  /// cumulative tiny changes in speed, or our speed would never change! (Note:
  /// the smallest speed change we can have is 1/100,000 m/s. c.f. 1/100 m/s^2
  /// x 1/1000 s = 1/100,000 m/s).
  INT32 speedRem;
} TJourneySimulator;


typedef struct TTunnelSimulator {
  /// @internal
  /// Current tunnel location
  TTunnelSimLocation loc;
  /// @internal
  /// Current speed in metres per second.
  INT32 speed;
  /// @internal
  /// Latitude/longitude coordinates.
  TPos position;
  /// @internal
  /// Heading in degrees (0-255)
  THeading heading;
} TTunnelSimulator;

extern void tunnelSim_init(TTunnelSimulator *pThis);

extern TTunnelSimResult tunnelSim_updateTime(TTunnelSimulator *pThis);

extern BOOL8 tunnelSim_setLocation(TTunnelSimulator *pThis,
                                    const TTunnelSimLocation *pLoc);

extern BOOL8 tunnelSim_getLocation(const TTunnelSimulator *pThis,
                                    TTunnelSimLocation *pLoc);

extern INT32 tunnel_simulator_getLongitude(void);

extern INT32 tunnel_simulator_getLatitude(void);

extern INT32 tunnel_simulator_getHeading(void);

extern INT32 tunnel_simulator_getSpeed(void);

extern void journeySim_init(TJourneySimulator *pThis);

extern TJourneySimResult journeySim_reset(TJourneySimulator *pThis,
                                          const TGuideList *pGuideList);

extern TJourneySimResult journeySim_updateTime(TJourneySimulator *pThis,
                                               INT32 milliseconds);

extern BOOL8 journeySim_setLocation(TJourneySimulator *pThis,
                                    const TJourneyLocation *pLoc);

extern BOOL8 journeySim_getLocation(const TJourneySimulator *pThis,
                                    TJourneyLocation *pLoc);

extern UINT32 journeySim_setSpeedLimit(TJourneySimulator *pThis,
                                       TRoadSpeedLimitType roadType,
                                       UINT32 speedLimit);

extern UINT32 journeySim_getSpeedLimit(const TJourneySimulator *pThis,
                                       TRoadSpeedLimitType roadType);

extern BOOL8 journeySim_jumpToNextTurn(TJourneySimulator *pThis);

extern BOOL8 journeySim_jumpToPreviousTurn(TJourneySimulator *pThis);


extern INT32 simulator_getLongitude(void);

extern INT32 simulator_getLatitude(void);

extern INT32 simulator_getAltitude(void);

extern INT32 simulator_getHeading(void);

extern UINT8 simulator_getFixType(void);

extern UINT32 simulator_getUTC(void);


#ifdef __cplusplus
}
#endif

#endif /* ENSIMP_H */
