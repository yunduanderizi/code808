#ifndef RTTRAVELCOSTSP_H
#define RTTRAVELCOSTSP_H

#include <mapdata/enroadP.h>
#include <routing/rtoptionsP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// Travel cost structure that is used to hold all travel costs for a
/// particular route calculation
typedef struct TTravelCosts {
  TVehicleType  vehicleType;
  UINT32        freewayScaleFactor;
  UINT32        expresswayScaleFactor;
  UINT32        highwayNo1ScaleFactor;
  UINT32        highwayNo3ScaleFactor;
  UINT32        unsealedScaleFactor;
  UINT32        urbanScaleFactor;
  UINT32        ferryEntryCost;
  UINT32        ferryScaleFactor;
  UINT32        dualCarriagewayScaleFactor;
  UINT32        roundaboutScaleFactor;
  UINT32        shortestScaleFactor;
  UINT32        heuristicScalar;
} TTravelCosts;


/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// Use the options structure and default road speeds to calculate actual
// per metre road costs for each functional road class and form of way
BOOL8 travelCosts_setup( const TInternalRoutingOptions *options );


// Retrieve the per metre cost that should be applied to travel on a
// particular segment
UINT32 travelCosts_getSegmentScalar( TRoadSegment *segment );


// Retrieve the scaled cost of entering a ferry
UINT32 travelCosts_getFerryEntranceCost(void);


// Retireve the minimum per metre travel cost with the current options
UINT32 travelCosts_getMinScalar(void);


#ifdef __cplusplus
}
#endif

#endif // RTTRAVELCOSTSP_H
