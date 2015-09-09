#ifndef INTERFACE_MAP_H
#define INTERFACE_MAP_H

#include "engineinterface/engineinterface.h"
#include "datatype.h"
#include "system_param.h"
#include  <ext/hash_map>
using namespace __gnu_cxx;

bool mapZoomOut();
bool mapZoomIn();
TPathGeobanHandle mapIsDefiningAvoidArea();
void mapStartEditArea();
void mapAddAvoidanceVertex();
void mapUpdateAvoidanceVertex(TPos &screenPos);
void mapFinishAvoidanceArea();
void mapDeleteAvoidanceArea();

void mapLoadDefaultColour();
void mapSetCoreColourScheme();
void mapResetColourScheme();

void mapStartCarPosCount();
void mapStopCarPosCount();
void mapCountCarPos();
bool mapIsScroll();
TPos mapSetCarIconPos(TPos mapPos, UINT8 heading);

bool mapMousePress(int x,int y);
bool mapMouseMove(int x, int y);
MAP_MODE mapGetMapMode();
void mapSetMapMode(MAP_MODE mapmode);
void mapRefresh();
void mapDrawRuler();
void mapGotoMap(TPos mapPos);
void mapChangeMapMode();
void mapSaveViewPort();

#endif // INTERFACE_MAP_H
