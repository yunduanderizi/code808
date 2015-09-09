#ifndef INTERFACE_ROUTE_H
#define INTERFACE_ROUTE_H

#include "engineinterface/engineinterface.h"

bool routeIsSimulating();
void routeStartSimulating();
void routeStopSimulating();
void routeStartmanualSim();

bool routeIsManulSimulating();
void routestopmanualSimulating();
void routeRemoveRoute();
bool routeIsDefiningNewJourney();
bool routeIsSimulationFinished();
bool routeIsManulSimulationFinished();

void routeSetOptions(TRoutingOptions &m_options);
void routeSetOptionsAvoidFlag(TRoutingOptions &m_options );
void routeDefaultOptions();
void routeSetRecommendRouting(TRoutingOptions &m_options);
void routeSetFast(TRoutingOptions &m_options);
void routeSetShortest(TRoutingOptions &m_options);
void routeSetEasiest(TRoutingOptions &m_options);
void routeSetEconomiest(TRoutingOptions &m_options);
void routeSetExpressway(TRoutingOptions &m_options);

void routeSetAvoidTolls(BOOL8 TollsFlag,TRoutingOptions &m_options);
void routeSetAvoidUnsealed(BOOL8 UnsealedFlag,TRoutingOptions &m_options);
void routeSetAvoidFerries(BOOL8 FerriesFlag,TRoutingOptions &m_options);
void routeSetAvoidUturns(BOOL8 UturnsFlag,TRoutingOptions &m_options);
void routeSetAvoidFreeway(BOOL8 FreewayFlag,TRoutingOptions &m_options);
void routeCalculateWhetherOrNot();
void routeGetCurrentLocation(TPos &gps_loc,INT32 &heading);
void routeCheckGpsLocation(TPos &gps_loc);
bool routeSimilation();
void routeGetGpsLocation(TPos &gps_loc);
int routeGetGpsHeading();


void setPedstrianRouting(TRoutingOptions &m_options);
bool doRouteEnd(const TPos &mapPos, TMapLocation * pMapLoc);
void getAddressName(UTF16 * STREET );
void setstreetname();
void addRoadAvoid();
void matchNetworkLocation(TPos &network_loc,TPos &map_loc);
void matchNetworkLocation(TMapLocation &network_loc,TMapLocation &map_pos);

#endif // INTERFACE_ROUTE_H
