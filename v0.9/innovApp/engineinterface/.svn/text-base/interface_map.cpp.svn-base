#include "interface_map.h"
#include "routing/rtbanareas.h"
#include "engineinterface/system_param.h"
#include "engineinterface/navcoreDisplay.h"
#include "interface_route.h"
TAvoidanceArea avoidanceArea;
TPathGeobanHandle definingAvoidanceArea;

static hash_map<const wchar_t*, ColourUnit, StrCmpHash> g_dayColourUnits;
static hash_map<const wchar_t*, ColourUnit, StrCmpHash> g_nightColourUnits;
ColourTagType g_currentColourType;
const static bool g_patternedPolygons = true;
int g_carposcount = -1;

bool mapZoomIn()
{
    return g_navDisplay->zoomToPoint(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.5);
}

bool mapZoomOut()
{
    return g_navDisplay->zoomToPoint(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 2);
}

TPathGeobanHandle mapIsDefiningAvoidArea()
{
    return definingAvoidanceArea;
}

void mapStartEditArea()
{
    TPos screenPos = { SCREEN_WIDTH/2, -SCREEN_HEIGHT/2};
    TPos mapPos;
    Viewport_ScreenToMap(&g_navDisplay->viewport, &screenPos, &mapPos);
    avoidanceArea_init(&avoidanceArea);
    avoidanceArea.avoidedRegion = REGION_INSIDE;
    avoidanceArea.geobanType = GEOBAN_EXPENSIVE;
    avoidanceArea.entryCost = (TAvoidanceAreaEntryCost)100000;
    avoidanceArea.traversalCost = TRAVERSAL_COST_ZERO;
    avoidanceArea.numVertices = 2;
    avoidanceArea.vertices[0] = mapPos;
    mapUpdateAvoidanceVertex(screenPos);
}

void mapAddAvoidanceVertex()
{
  UINT32 n = avoidanceArea.numVertices;
  avoidanceArea.vertices[n] = avoidanceArea.vertices[n - 1];
  ++avoidanceArea.numVertices;
}

void mapUpdateAvoidanceVertex(TPos &screenPos)
{
  TPos mapPos;
  UINT32 n = avoidanceArea.numVertices;

  if (n > 9) return;
  if (n < 1) return;
  Viewport_ScreenToMap(&g_navDisplay->viewport, &screenPos, &mapPos);

  if (n >= 2 && Vertex_Equal(&avoidanceArea.vertices[n - 2], &mapPos)) {
    // If the new vertex is the same as the previous one we perturb the position for the new vertex so that the
    // avoidance area can be successfully added.
    ++mapPos.x;
    ++mapPos.y;
  }
  avoidanceArea.vertices[n - 1] = mapPos;
  if (definingAvoidanceArea) {
    g_navRoute->deleteAvoidanceArea(definingAvoidanceArea);
  }
  definingAvoidanceArea =
    g_navRoute->addAvoidanceArea(&avoidanceArea);

}

void mapFinishAvoidanceArea()
{
  definingAvoidanceArea = 0;
}

void mapDeleteAvoidanceArea()
{
    if (definingAvoidanceArea) {
        g_navRoute->deleteAvoidanceArea(definingAvoidanceArea);
    }
}

void mapLoadDefaultColour()
{
    if(g_dayColourUnits.size()&&g_nightColourUnits.size())
    {
        return;
    }
    g_dayColourUnits.clear();
    g_nightColourUnits.clear();

    size_t maxTableSize = sizeof(g_roadColourTable) / sizeof(TMapColourEntry);
    for(size_t i = 0; i < maxTableSize; i++){                            //load road
        ColourUnit cUnit;
        cUnit.type = ROAD_ARC_MISC;
        RGBCOLOUR colour = g_navDisplay->getColour(g_roadColourTable[i].colourIndex);
        cUnit.colour.red = colour.red;
        cUnit.colour.green = colour.green;
        cUnit.colour.blue = colour.blue;
        cUnit.colour.alpha = colour.alpha;
        cUnit.typeIndex = i;
        if(g_dayColourUnits.find(g_roadColourTable[i].legend) == g_dayColourUnits.end())
        {
            g_dayColourUnits.insert(ColourPair(g_roadColourTable[i].legend, cUnit));
            g_nightColourUnits.insert(ColourPair(g_roadColourTable[i].legend, cUnit));
        }

    }
    TElevationPalette *pElevationPalette = g_navDisplay->getElevationPalette();
    for(size_t i = 0; i < numberOfGradients; i++){                       //load gradient
        ColourUnit cUnit;
        if(i < pElevationPalette->elevationThresholdsCount){
            cUnit.type = GRADIENT_ENABLE;
            TElevationPaletteEntry entry = pElevationPalette->elevationPalette[i];
            cUnit.colour.red = entry.r;
            cUnit.colour.green = entry.g;
            cUnit.colour.blue = entry.b;
            cUnit.colour.alpha = entry.elevation;
        }else{
            cUnit.type = GRADIENT_DISABLE;
        }
        if(g_dayColourUnits.find(STR_GRADIENT[i]) == g_dayColourUnits.end())
        {
            g_dayColourUnits.insert(ColourPair(STR_GRADIENT[i], cUnit));
            g_nightColourUnits.insert(ColourPair(STR_GRADIENT[i], cUnit));
        }

    }
    maxTableSize = sizeof(g_polyColourTable) /
            sizeof(TPolyMapColourEntry) - 1;                                   //last one is elevation, so skip
    for(size_t i = 0; i < maxTableSize; i++) {                           //load poly
        ColourUnit cUnit;
        cUnit.type = POLY_CITY;
        RGBCOLOUR colour = g_navDisplay->getPolygonFillColour(g_polyColourTable[i].polyClass);
        cUnit.colour.red = colour.red;
        cUnit.colour.green = colour.green;
        cUnit.colour.blue = colour.blue;
        cUnit.colour.alpha = colour.alpha;
        cUnit.typeIndex = i;
        if(g_dayColourUnits.find(g_polyColourTable[i].polyName) == g_dayColourUnits.end())
        {
            g_dayColourUnits.insert(ColourPair(g_polyColourTable[i].polyName, cUnit));
            g_nightColourUnits.insert(ColourPair(g_polyColourTable[i].polyName, cUnit));
        }

    }
    g_currentColourType = DAY;
}


void mapSetCoreColourScheme()
{
    hash_map<const wchar_t*, ColourUnit, StrCmpHash> *pMap = NULL;
    switch(g_currentColourType){
      case DAY:
        pMap = &g_dayColourUnits;
        break;
      case NIGHT:
        pMap = &g_nightColourUnits;
        break;
      default:
        return;
    }

     hash_map<const wchar_t*, ColourUnit, StrCmpHash>::iterator itor;
     /// Caution: must deal with Gradients first because resetElevationPalette()
     /// will reset the poly colours to default.
     TElevationPaletteEntry topoPalette[numberOfGradients];               //deal with gradient
     int j;

         for(j = 0; j < numberOfGradients; j++){
             itor = pMap->find(STR_GRADIENT[j]);
             if(itor != pMap->end()){
                 if(itor->second.type == GRADIENT_DISABLE){
                     break;
                 }
                 Colour *colour = &itor->second.colour;
                 topoPalette[j].r = colour->red;
                 topoPalette[j].g = colour->green;
                 topoPalette[j].b = colour->blue;
                 topoPalette[j].elevation = colour->alpha;
             }
         }

     if(j){
       g_navDisplay->resetElevationPalette();
       g_navDisplay->setTopoElevationGradients(topoPalette, j);
     }
     for(itor = pMap->begin(); itor != pMap->end(); itor++){
       ColourUnit *pUnit = &itor->second;
       switch(pUnit->type)
       {
       case POLY_CITY:
         g_navDisplay->setPolygonFillColour(g_polyColourTable[pUnit->typeIndex].polyClass,
           RGBA_Define((UINT8)pUnit->colour.red, (UINT8)pUnit->colour.green,
           (UINT8)pUnit->colour.blue, (UINT8)pUnit->colour.alpha));
         break;
       case ROAD_ARC_MISC:
         g_navDisplay->setColour(g_roadColourTable[pUnit->typeIndex].colourIndex,
           RGBA_Define((UINT8)pUnit->colour.red, (UINT8)pUnit->colour.green,
           (UINT8)pUnit->colour.blue, (UINT8)pUnit->colour.alpha));
         break;
       }
     }
}

void mapResetColourScheme()
{
    g_navDisplay->resetColour();
    mapLoadDefaultColour();
    g_navDisplay->setPatternedPolygon(g_patternedPolygons);
    g_navDisplay->saveViewport();
    g_navDisplay->refresh();
}


bool mapMousePress(int x, int y)
{
    return g_navDisplay->mousepress(x,y);
}

bool mapMouseMove(int x, int y)
{
    return g_navDisplay->mousemove(x, y);
}

MAP_MODE mapGetMapMode()
{
    return g_navDisplay->getmapmode();
}

void mapSetMapMode(MAP_MODE mapmode)
{
    g_navDisplay->setmapmode(mapmode);
}

void mapRefresh()
{
    g_navDisplay->refresh();
}

void mapDrawRuler()
{
    g_navDisplay->drawRuler();
}

void mapGotoMap(TPos mapPos)
{
    g_navDisplay->gotoMap(mapPos);
}

void mapChangeMapMode()
{
    g_navDisplay->changemapmode();
}

void mapSaveViewPort()
{
    g_navDisplay->saveViewport();
}

void mapStartCarPosCount()
{
    g_carposcount = 6;
}

void mapStopCarPosCount()
{
    g_carposcount = 0;
}

bool mapIsScroll()
{
    if(g_carposcount > 0)
    {
        return true;
    }
    return false;
}

void mapCountCarPos()
{
    if(g_carposcount > 0)
        g_carposcount--;
//    return m_carposcount;
}

TPos mapSetCarIconPos(TPos mapPos, UINT8 heading)
{
//    qDebug() << "setCarIconPos  start";
    g_navDisplay->setCarAngle(heading);

    if(routeIsManulSimulating)
    {
        TPos networkmapPos={0};
        matchNetworkLocation(networkmapPos, mapPos);
        g_navDisplay->setCarIconPos(networkmapPos, heading);
        return networkmapPos;
    }
    else
    {
        g_navDisplay->setCarIconPos(mapPos, heading);
        return mapPos;
    }
}
