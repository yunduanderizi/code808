#include "interface_route.h"
#include "interface_guide.h"
#include <location/locmaplocationP.h>

TRoutingOptions g_options;
extern TPos g_gps_data;
INT32 g_heading = 0;
UTF16 g_currentstreetname[256];

void setPedstrianRouting(TRoutingOptions &m_options)
{
    routingOptions_setToPreset(&m_options,RT_OPTION_PEDESTRIAN);
}

bool doRouteEnd(const TPos &mapPos, TMapLocation * pMapLoc)
{
    TPos gps_loc = {0};
    gps_loc.y=gps_getLatitude();
    gps_loc.x=gps_getLongitude();
    routeCheckGpsLocation(gps_loc);
    g_navRoute->startNewRoute(gps_loc, NULL);
    if(g_navRoute->endNewRoute(mapPos, g_options, pMapLoc))
    {
        g_navDisplay->refresh();
        return true;
    }
    else
    {
        return false;
    }
}

void getAddressName(UTF16 * street)
{
    memcpy(street,g_currentstreetname,sizeof(g_currentstreetname));
}

void setstreetname()
{
    TMapLocation     m_maploc,network_loc;

    UTF16 street[256] = {0};
    maplock_getMapLocation(&m_maploc);
    matchNetworkLocation(network_loc,m_maploc);
    maplocation_getNetworkRoadName(&network_loc,street,GEOSEARCH_MAX_NAME_SIZE);
    memset(g_currentstreetname,0,sizeof(g_currentstreetname));
    memcpy(g_currentstreetname,street,sizeof(g_currentstreetname));
}

void addRoadAvoid()
{
    TPos pos = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    TPos screenPos, mapPos;
    TMapLocation mapLocation, *mapLoc;
    TRoadType maxRoadType;
    TRoadNetworkType maxRoadNetwork;
    TRoadMapLayer roadMapLayer;

    // Need to convert to map co-ordinates.
    Vertex_Define(&screenPos, pos.x, -pos.y);
    Viewport_ScreenToMap(&g_navDisplay->viewport, &screenPos, &mapPos);

    IRoadTheme *roadTheme = Viewport_GetRoadTheme(&g_navDisplay->viewport);
    IRoadTheme_GetMaxVisibleRoadData(roadTheme, &g_navDisplay->viewport,
    &maxRoadType, &maxRoadNetwork, &roadMapLayer);
    maplocation_initUserLocation( &mapLocation, &mapPos, NULL, -1, maxRoadType );

    mapLoc = &mapLocation;
    //ban the segment(s) at the specified network location
    banSegments_addMapLocation(mapLoc,GEOBAN_FORBIDDEN);
    g_navDisplay->refresh();
}

void matchNetworkLocation(TPos &network_loc,TPos &map_loc)
{
    TMapLocation pmap_Loc;

    maplocation_initUserLocation(&pmap_Loc, &map_loc, NULL, -1, ROAD_TYPES);
    if(maplocation_resolveOnNetwork(&pmap_Loc, NULL))
    {
  //      printf("Could not get aorigin network location from road location [%.5f, %.5f]\n");
    }
    network_loc.x = pmap_Loc.network.pos.x;
    network_loc.y = pmap_Loc.network.pos.y;
}

void matchNetworkLocation(TMapLocation &network_loc,TMapLocation &map_pos)
{
    TPos map_location;
    TMapLocation pmap_Loc;

    map_location.x =map_pos.user_pos.x;
    map_location.y =map_pos.user_pos.y;
    maplocation_initUserLocation(&pmap_Loc, &map_location, NULL, -1, ROAD_TYPES);
    if(maplocation_resolveOnNetwork(&pmap_Loc, NULL))
    {
        //printf("Could not get aorigin network location from road location [%.5f, %.5f]\n");
    }
    network_loc = pmap_Loc;
}

bool routeIsSimulating()
{
    return g_navRoute->isSimulating();
}

void routeStartSimulating()
{
    guidanceDateReset();
    g_navRoute->startSimulating();
}

void routeStopSimulating()
{
    if(routeIsSimulating() == true)
        g_navRoute->stopSimulating();
}

void routeStartmanualSim()
{
    guidanceDateReset();
    g_navRoute->startmanualSim();
}

bool routeIsManulSimulating()
{
    return g_navRoute->ismanulSimulating();
}

void routestopmanualSimulating()
{
    if(routeIsManulSimulating() == true)
        g_navRoute->stopmanualSimulating();
}

void routeRemoveRoute()
{
    g_navRoute->removeRoute();
}

bool routeIsDefiningNewJourney()
{
    return g_navRoute->isDefiningNewJourney();
}

bool routeIsSimulationFinished()
{
    return g_navRoute->isSimulationFinished();
}

bool routeIsManulSimulationFinished()
{
    return g_navRoute->ismanulsimulationFinished();
}

void routeSetOptions(TRoutingOptions &m_options)
{
   g_options = m_options ;
}

void routeSetOptionsAvoidFlag(TRoutingOptions &m_options )
{
    g_options.avoidTollsFlag = m_options.avoidTollsFlag;
    g_options.avoidUnsealedFlag = m_options.avoidUnsealedFlag;
    g_options.avoidFerriesFlag = m_options.avoidFerriesFlag;
    g_options.avoidDualCarriagewayUTurnsFlag = m_options.avoidDualCarriagewayUTurnsFlag;
    g_options.avoidFreewaysFlag = m_options.avoidFreewaysFlag;
    g_options.avoidOverpassFlag = m_options.avoidOverpassFlag;
}

void routeDefaultOptions()
{
    routingOptions_setToPreset(&g_options,RT_OPTION_CAR_FASTEST);

}

void routeSetRecommendRouting(TRoutingOptions &m_options)
{
    routingOptions_setToPreset(&m_options,RT_OPTION_CAR_FASTEST);

}

void routeSetFast(TRoutingOptions &m_options)
{
    routingOptions_setToPreset(&m_options,RT_OPTION_CAR_FASTEST);
}

void routeSetShortest(TRoutingOptions &m_options)
{
    routingOptions_setToPreset(&m_options,RT_OPTION_CAR_SHORTEST);
}

void routeSetEasiest(TRoutingOptions &m_options)
{
    routingOptions_setToPreset(&m_options,RT_OPTION_CAR_EASIEST);
}

void routeSetEconomiest(TRoutingOptions &m_options)
{
    routingOptions_setToPreset(&m_options,RT_OPTION_CAR_ECONOMICAL);
}

void routeSetExpressway(TRoutingOptions &m_options)
{
    routingOptions_setToPreset(&m_options,RT_OPTION_CAR_EXPRESSWAY);
}

void routeSetAvoidTolls(BOOL8 TollsFlag,TRoutingOptions &m_options)
{
    m_options.avoidTollsFlag = TollsFlag;
}
void routeSetAvoidUnsealed(BOOL8 UnsealedFlag,TRoutingOptions &m_options)
{
    m_options.avoidUnsealedFlag = UnsealedFlag;
}
void routeSetAvoidFerries(BOOL8 FerriesFlag,TRoutingOptions &m_options)
{
    m_options.avoidFerriesFlag = FerriesFlag;
}
void routeSetAvoidUturns(BOOL8 UturnsFlag,TRoutingOptions &m_options)
{
    m_options.avoidDualCarriagewayUTurnsFlag = UturnsFlag;
}
void routeSetAvoidFreeway(BOOL8 FreewayFlag,TRoutingOptions &m_options)
{
    m_options.avoidFreewaysFlag = FreewayFlag;
}

void routeCalculateWhetherOrNot()
{
    if(routeIsDefiningNewJourney())
    {
        TMapLocation loc;
        memset(&loc,0,sizeof(loc));
        TPos pos;
        g_navRoute->destination_getMapLocation(&loc);
        routeRemoveRoute();
        doRouteEnd(pos,&loc);
    }
}

void routeCheckGpsLocation(TPos &gps_loc)
{
    if(gps_loc.x <= 0 || gps_loc.y <= 0)
    {
        gps_loc.x = (116.28929 * 100000);  //latitude
        gps_loc.y = (39.96009 * 100000);   //lon
    }
}

void routeGetCurrentLocation(TPos &gps_loc,INT32 &heading)
{
    gps_loc.y=gps_getLatitude();
    gps_loc.x=gps_getLongitude();
    routeCheckGpsLocation(gps_loc);
    heading = g_heading;
}

void routeGetGpsLocation( TPos &gps_loc )
{
    gps_loc.y=gps_getLatitude();
    gps_loc.x=gps_getLongitude();
    routeCheckGpsLocation(gps_loc);
}

bool routeSimilation()
{
    g_navRoute->simulateRoute();
    //following codes will move to navDisplay later.
    TPos carPos;
    routeGetGpsLocation(carPos);

    return guideUpdateGuidanceInfo();
}

int routeGetGpsHeading()
{
    return gps_getHeading();
}
