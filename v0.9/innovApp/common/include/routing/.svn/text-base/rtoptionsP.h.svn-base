//
// Declaration of the path calculation options type and its associated
// functions
//
// $Id: rtoptionsP.h 10771 2008-08-02 12:13:15Z Woody.lee $
//

#ifndef RTOPTIONSP_H
#define RTOPTIONSP_H

#include <routing/rtoptions.h>
#include <mapdata/enroadP.h>

#ifdef __cplusplus
extern "C" {
#endif



/// Structure that will contain a set of consistent routing options and some
/// additional internal options, depending on e.g. the vehicle type or routing
/// mode.
typedef struct TInternalRoutingOptions {

  // flags that are carried over from TRoutingOptions:

  TVehicleType  vehicleType;
  UINT8         shortestDistancePreference;
  UINT8         freewayUsagePreference;
  UINT8         expresswayUsagePreference;
  UINT8         highwayNo1UsagePreference;
  UINT8         highwayNo3UsagePreference;
  UINT8         unsealedUsagePreference;
  UINT8         urbanAvoidancePreference;
  UINT8         ferryUsagePreference;
  BOOL8         turnCostPreference;

  BOOL8         avoidTolls;
  BOOL8         avoidUnsealed;
  BOOL8         avoidFerries;
  BOOL8         avoidDCUTurns;
  BOOL8         avoidFreeways;
  BOOL8         avoidOverpass;

  // flags that depend implicitely on TRoutingOptions:

  BOOL8         switchRoadNetwork;
  BOOL8         avoidResidential;
  BOOL8         avoidMotorways;
  BOOL8			avoidExpressWay;
  TRoadType     mostMinorRoadType;
  BOOL8         prohibitDestinationOnMotorway;

  BOOL8         closedEndsApply;
  BOOL8         oneWayRestrictionsApply;
  BOOL8         turnRestrictionsApply;
  BOOL8         dcUTurnRestrictionsApply;
  BOOL8         restAreaDetourRestrictionsApply;
  BOOL8         trafficEventsApply;

} TInternalRoutingOptions;


/// Derives the internal routing options and flags from the public routing
/// options.
///
/// @param[in]   options
///   Pointer to the public routing options which were passed to the path
///   calculator.
/// @param[out]  internalOptions
///   Pointer to the structure, where we will store the derived settings. The
///   caller owns the object.
void routingOptions_setupInternalOptions( 
  const TRoutingOptions   *options,
  TInternalRoutingOptions *internalOptions );



#ifdef __cplusplus
}
#endif

#endif // RTOPTIONSP_H
