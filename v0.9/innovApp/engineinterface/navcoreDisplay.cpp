//#include "StdAfx.h"

#include "navcoreDisplay.h"
#include "navcoreRoute.h"
#include "navcoreFavourites.h"
#include "car.h"
#include "goldstar.h"
#include "display/dimaptile.h"
#include "interface_logistics.h"
#include <trail/ensnailtr.h>
#include <logging/sdebug.h>
#include <location/locmaplocationP.h>
#include <mapdata/enroadP.h>
#include <locationprovider/enmlockP.h>

#include <QDebug>

extern navcoreRoute *g_navRoute;

extern "C" RDBG_declareFlag( diTileDirectory )

RDBG_declareFlag(diPrePath);

/********************************************************************************
navcoreDisplay::navcoreDisplay( )

Description:   Constructor.  Initialize local variables. All display
initialization is done in InitDisplay.
********************************************************************************/
navcoreDisplay::navcoreDisplay()
    :m_renderSnailTrail(TRUE)
    ,m_pDecompBuffer(NULL)
    ,m_bDisplayInitialised(false)
    ,m_mapangle_flag(false)
    ,m_mapmode(MAPMODE_2D_NORTH)
{
}

navcoreDisplay::~navcoreDisplay()
{
  m_bDisplayInitialised = false;
}

/********************************************************************************
void navcoreDisplay::initBuffer( int width, int height )
Input:         Screen width and height
Description:   Initializes the bitmap buffers to screen size.
********************************************************************************/
void navcoreDisplay::initBuffer(int width, int height)
{
//  this->clearCarIcon();
  if (m_pDecompBuffer != NULL) {
    delete m_pDecompBuffer;
    m_pDecompBuffer = NULL;

    m_pDecompBuffer = new UINT16[ width * height ];
  } else {
    m_pDecompBuffer = new UINT16[ width * height ];
  }
}

/*********************************************************************************
void navcoreDisplay::setOrigin( )

Description:   Determines what to display on startup. Searches for the first
road from the loaded maps and sets that as the initial
start display. Private Method
*********************************************************************************/
void navcoreDisplay::setOrigin()
{
  TPos vertex;
  TMapLocation mapLoc;

  // Get a random location and use it as the origin
  maplocation_initRandomLocation(&mapLoc);
  maplocation_getLocationPos(&mapLoc, &vertex);

  Viewport_SetMapOrigin(&viewport, &vertex);
}

/********************************************************************************
void navcoreDisplay::initDisplay(int screenWidth, int screenHeight)
Input:         Screen width and height
Description:   Initialize the map display.
********************************************************************************/
void navcoreDisplay::initDisplay(int screenWidth, int screenHeight)
{
    initBuffer(screenWidth, screenHeight);
    Sky_SetBitmap(&m_sky, NULL);
    TPos carPos;
    //    carPos.x = 0;
    //    carPos.y = 0;
    carPos.x = 11677689;
    carPos.y = 2957555;

    CarIcon_Create(&m_carIcon,&carPos,0,NULL);
    CarIcon_SetBitmap(&m_carIcon,&myCarIcon);
    Geoban_New(&m_geoban);

    /* set decompression buffer for bitmaps */
    graph_setDecompBuffer( ( UINT8 * )m_pDecompBuffer, screenWidth * screenHeight );
    /* set graphic bitmap cache */
    graph_setCacheBuffer( ( UINT8 * )m_cacheBuffer, sizeof( m_cacheBuffer ) );
    setOrigin();
    Viewport_SetMapAngle(&viewport, 0);
    LOD_SetLevel(&viewport.lod, 0);

    // 1 x magnification (no scaling)
    // Actually (128 * 2 ^ n where n could be a value from -2 to +2)
    Viewport_SetPoiScale(&viewport, 128);// Set up POIs for display.  On startup, all POIs are displayed.
    if (!poitype_getAvailableTypeList(Viewport_GetPOITypeFilter(&viewport))){
        LOG_ERROR(0,("Could not initialise POI type list"));
    }

    // Display opaque POI icons on startup.
    Viewport_SetPoiTransparency(&viewport, 0);

    // 1 x magnification (no scaling)
    // Actually (128 * 2 ^ n where n could be a value from -2 to +2)
    Viewport_SetPoiScale(&viewport, 128);
}

bool navcoreDisplay::isInitialised()
{
  return m_bDisplayInitialised;
}

/********************************************************************************
void navcoreDisplay::zoomToPoint(int x, int y, double zoomFactor)
Input:         xy screen coordinate and the factor of zoom
Description:   Change the size of the display and focus the zoom on an
xy point.
********************************************************************************/
bool navcoreDisplay::zoomToPoint(int x, int y, double zoomFactor)
{

    //Refashion this function. Edison add at 2008.8.25
    INT32 cx, cy;
    TPos screenPos, mapPos, to, from;

    cx = SCREEN_WIDTH / 2; //Map Centre
    cy = SCREEN_HEIGHT / 2;

    Vertex_Define(&screenPos, x, -y);
    Viewport_ScreenToMap(&viewport, &screenPos, &mapPos);

    Viewport_SetMapOrigin(&viewport, &mapPos);

    //Zoom in
    //Now zoom just calls display_setMapSize, and resets the map.
    //However, once the mapsize is changed we will then need to pan
    //to the right point to get 'fixed-point zoom'.
    bool result = zoom(zoomFactor);
    //get the x,y mapdisplay coordinates, from the given lat/lon
    // Viewport_MapToScreen(&viewport, &mapPos, &screenPos);

    Vertex_Define(&to, cx, cy);
    //Vertex_Define(&from, abs(screenPos.x), abs(screenPos.y));
    //pan(to, from);   //to center of map

    refresh();
    return result;

}

void navcoreDisplay::zoomToPoint(TPos mapPos, double zoomFactor)
{

}

void navcoreDisplay::gotoMap(TPos mapPos)
{
    Viewport_SetMapOrigin(&viewport, &mapPos);
}

// Goto a map position whose bounding box is constrained by size.  This
// will set the map size if necessary.
void navcoreDisplay::gotoMap(TPos mapPos, unsigned int size)
{
    TPos screenPos;

    Viewport_SetMapOrigin(&viewport, &mapPos);
    Viewport_SetMapSize(&viewport, size);

    refresh();

    setCrossHairType(FIXED_CROSSHAIR);
    Vertex_Define(&screenPos, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    showCrossHairs(screenPos);
}

// Goto a map position whose bounds are defined as bounding box TRect.
void navcoreDisplay::gotoMap(TRect *bBox)
{
  TPos mapPos;
  UINT32 size;

  Viewport_CalcBoundingParameters(&viewport, &mapPos, &size, bBox);
  Viewport_SetMapOrigin(&viewport, &mapPos);
  Viewport_SetMapSize(&viewport, size);

  refresh();
}

/********************************************************************************
void navcoreDisplay::showCrossHairs(TPos crossHair)
Input:         Position of the cross hair
Description:   Draws cross hairs on the screen, centred on the input
parameter position, using the graph_context functions
********************************************************************************/

void navcoreDisplay::showCrossHairs(TPos crossHair)
{

}

/********************************************************************************
void navcoreDisplay::drawRulerPos(int size, int yPos)
Input:   size of the current map length (in metres) and desired vertical
position for the ruler
Description: Draws a ruler over the map display, showing size in metres/km.
********************************************************************************/
void navcoreDisplay::drawRulerPos(int size, int yPos)
{

    UTF16 label[256];
    UTF16 unit[256];
    int ymin, ymax, vLength=10;
    TPos txtPos;

    //are we drawing the ruler at the top or bottom of the map display?
    if (yPos==0) {
        ymin=yPos-vLength;
        ymax=yPos;
        txtPos.y=ymax-7;
    } else {
        ymin=yPos;
        ymax=yPos+vLength;
        txtPos.y=ymin+37;
    }

    //in the case where size corresponds to screen_height, need to convert it
    //to metres
    if (SCREEN_HEIGHT>SCREEN_WIDTH) {
        size = (SCREEN_HEIGHT / SCREEN_WIDTH) * size;
    }

    //draw the ruler and ticks
    GraphContext_DrawHLine(&context,10,80,yPos,RGB_FromInternal(0, context.depth));
    GraphContext_DrawVLine(&context,10,ymin,ymax,RGB_FromInternal(0, context.depth));

//    GraphContext_DrawHLine(&context,SCREEN_WIDTH/2,SCREEN_WIDTH,yPos,
//                           RGB_FromInternal(0, context.depth));

    GraphContext_DrawVLine(&context,80,ymin,ymax,
                           RGB_FromInternal(0, context.depth));

    //set the unit
    if (size >= 10000) {
        size /= 1000;
        wcscpy((wchar_t*)unit, L"k");
        utf16_strcat(unit, (UTF16*)L"m");
    } else {
        wcscpy((wchar_t*)unit, L"m");
    }
    size /= 10;  //cause it's only 1cm long.
    utf16_itoa(size,label);
    utf16_strncat(label, unit, 2);
    int a = utf16_strlen(label);
    //print the distances
    txtPos.x = 12 + (6-a)/2.0 * 70/6.0;
//    txtPos.x = 15;
    GraphContext_DrawText(&context, (TFont *)context.font, label, &txtPos, RGB_FromInternal(0, context.depth));

}


/********************************************************************************
void navcoreDisplay::drawRuler()
Description: Draws a rule of the map size in m/km along the top of the map
*********************************************************************************/
void navcoreDisplay::drawRuler()
{
    int distance;
    int xsf, ysf, xMetres,yMetres;
    TPos screenPos1, screenPos2, mapPos1, mapPos2;

    //If in 2D draw a ruler along the top of the screen
    if (viewport.type == Viewport2D) {
        distance = Viewport_GetMapSize(&viewport);
//        drawRulerPos(distance,0);
    }

    //draw the ruler for the bottom of the display
    if (viewport.type == Viewport3D) {

        //the bottom left and right corner coordinates
        Vertex_Define(&screenPos1,0,-SCREEN_HEIGHT);
        Vertex_Define(&screenPos2,SCREEN_WIDTH,-SCREEN_HEIGHT);

        //conver them to map points
        Viewport_ScreenToMap(&viewport,&screenPos1,&mapPos1);
        Viewport_ScreenToMap(&viewport,&screenPos2,&mapPos2);

        Viewport_ConvertMapToMeters(&viewport,&xsf,&ysf);

        //The core should be doing this not the app
        xMetres = ((mapPos1.x-mapPos2.x) * xsf) >> 12;
        yMetres = ((mapPos1.y-mapPos2.y) * ysf) >> 12;
        //get the euclidean distance
        distance = imath_sqrt((xMetres*xMetres)+(yMetres*yMetres));
    }

    drawRulerPos(distance,-SCREEN_HEIGHT+30);
}

/********************************************************************************
BOOL8 navcoreDisplay_renderCallback( struct WORLD_RENDERPARAMS *pParams )
Description: called by World_Render, allows app to render map displayables
*********************************************************************************/
static BOOL8 navcoreDisplay_renderCallback(struct WORLD_RENDERPARAMS *pParams)
{
        navcoreDisplay *pThis;
        pThis = (navcoreDisplay*)pParams->context;
        if (pParams->renderStage == RENDERSTAGE_ROUTES) {
            g_navRoute->renderAllJourneys(&pThis->viewport);
        } else if (pParams->renderStage == RENDERSTAGE_HIGHLIGHTED_ROADS) {
            //HighlightList_Render(&pThis->viewport.displayNameSet.highlightList, &pThis->viewport);
        } else if (pParams->renderStage == RENDERSTAGE_GEOBANS) {
            Geoban_Render(pThis->getGeoban(), &pThis->viewport);
        } else if (pParams->renderStage == RENDERSTAGE_LATLONG_GRID) {
            //LatLongGrid_Render(pThis->getLatLongGrid(), &pThis->viewport);
        } else if (pParams->renderStage == RENDERSTAGE_SKY) {
            Sky_Render(pThis->getSky(), &pThis->viewport);
        } else if (pParams->renderStage == RENDERSTAGE_FAVOURITES) {
            renderMutiDestions(&mygoldstar,&pThis->viewport);
        } else if (pParams->renderStage == RENDERSTAGE_CARICON) {
            g_navRoute->renderAllJourneyFlags(&pThis->viewport);
            CarIcon_Render(pThis->getCarIcon(), &pThis->viewport);
        } else if (pParams->renderStage == RENDERSTAGE_MAP_TILES) {
            if (pThis->viewport.type == Viewport2D) {
                if (RDBG_testFlag(diTileDirectory)) {
                    const char *start, *end;
                    char buffer[260];
                    int i;

                    RDBG_getFlagStrValue(diTileDirectory, &start, &end);
                    if (start && end) {
                        for (i = 0; &start[i] != end; i++) {
                            buffer[i] = start[i];
                        }
                        buffer[i] = '\0';

                        maptile_setTileDirectory(buffer);
                    }
                }

                if (maptile_getTileDirectory()) {
                    const TRect rect = { -18000000, -9000000, 18000000, 9000000 };
                    char tileStr[9] = "";

                    maptile_render(&pThis->viewport, &rect, tileStr, sizeof(tileStr), NULL, NULL);
                }
            }
        }

        return TRUE;
}

/********************************************************************************
void navcoreDisplay::refresh( )
Description: update and redraw the map display bitmap
*********************************************************************************/
void navcoreDisplay::refresh()
{
    WORLD_RENDERPARAMS params;
    params.members = RENDERPARAMS_FLAGS | RENDERPARAMS_CALLBACK | RENDERPARAMS_CONTEXT;
    params.renderFlags = (WORLD_RENDERFLAGS)WORLD_RENDERDEFAULT;
    if ( m_renderSnailTrail ) {
        params.renderFlags = (WORLD_RENDERFLAGS)(params.renderFlags | WORLD_RENDERSNAILTRAIL);
    } else {
        params.renderFlags = (WORLD_RENDERFLAGS)(params.renderFlags & ~WORLD_RENDERSNAILTRAIL);
    }
    params.renderCallback = navcoreDisplay_renderCallback;
    params.context = this;

    Viewport_BeginRender(&viewport);
#ifdef _DEBUG//for speedcam enhancement
    INT32 distToSpeedcam;
    UINT8 heading;
    UINT16 speedLimit;
    TSegmentID preferredPath[512] = {0};
    UINT16 expandSegmentNum = 0;
    if(RDBG_testFlag(diPrePath)) {
        maplock_getPreferredPath(500, 20, preferredPath, &expandSegmentNum, &distToSpeedcam, &heading, &speedLimit, NULL);
    }
#endif

    World_Render(&params, &viewport);
#ifdef _DEBUG//for speedcam enhancement
    if(RDBG_testFlag(diPrePath)) {
        if (params.renderFlags & WORLD_RENDERROADS && (expandSegmentNum > 0) ) {
            World_RenderPreferredPath(viewport, (params.renderFlags & WORLD_RENDERROADLABELS) != 0, preferredPath, expandSegmentNum);
        }
    }
#endif
    Viewport_EndRender(&viewport);
}

bool navcoreDisplay::zoom(double k)
{
    double mapSize;
    UINT32 dist;

    if (viewport.type == Viewport3D) {
        dist = Viewport_GetObservationDistance(&viewport);
        mapSize = dist * k;
        Viewport_SetObservationDistance(&viewport, (UINT32)(mapSize));
    } else {
        mapSize = Viewport_GetMapSize(&viewport) * k;
//        qDebug() << "mapSize :" << mapSize;
        Viewport_SetMapSize(&viewport, ( int )mapSize );
    }
    if(mapSize >= MAX_MAP_SIZE ||mapSize <= MIN_MAP_SIZE)
    {
        return false;
    }
    return true;
}

void navcoreDisplay::pan(TPos to, TPos from)
{
}


bool navcoreDisplay::clickedOnMap(TPos screenPos)
{
    if (screenPos.x > SCREEN_XOFFSET &&
            screenPos.x < SCREEN_WIDTH + SCREEN_XOFFSET &&
            screenPos.y > SCREEN_YOFFSET &&
            screenPos.y < SCREEN_HEIGHT + SCREEN_YOFFSET)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void navcoreDisplay::rememberCenter()
{
    TPos screenPos;
    screenPos.x = SCREEN_WIDTH / 2;
    screenPos.y = -SCREEN_HEIGHT / 2;
    Viewport_ScreenToMap(&viewport, &screenPos, &m_center);
}

bool navcoreDisplay::mousepress(int x,int y)
{
    TPos screenPos;

    Vertex_Define(&screenPos,x,y);
    if (clickedOnMap( screenPos ))
    {
        TPos mousePos;
        Vertex_Define(&mousePos, x - SCREEN_XOFFSET, -(y - SCREEN_YOFFSET));
        Viewport_ScreenToMap(&viewport, &mousePos, &m_initMouseMapPos);
        refresh();
        rememberCenter();
        return true;
    }
    else
    {
        return false;
    }
}

bool navcoreDisplay::mousemove(int x, int y)
{
    TPos screenPos, mapPos, absScreenPos; //need an absolute screenPos, and
    //a mapcore screenPos (-y)

    Vertex_Define(&absScreenPos, x, y);
    if(clickedOnMap(absScreenPos))
    {
        Vertex_Define(&absScreenPos, x - SCREEN_XOFFSET, y - SCREEN_YOFFSET);
        Vertex_Define(&screenPos, x - SCREEN_XOFFSET, -(y - SCREEN_YOFFSET));
        Viewport_ScreenToMap(&viewport, &screenPos, &mapPos);
        TPos mapOrigin;                                  // recompute map origin by moving vector
        Viewport_GetMapOrigin(&viewport, &mapOrigin);
        mapOrigin.x += m_initMouseMapPos.x - mapPos.x;
        mapOrigin.y += m_initMouseMapPos.y - mapPos.y;
        Viewport_SetMapOrigin(&viewport, &mapOrigin);
        m_xy = absScreenPos;
        return true;
    }
    else
    {
        return false;
    }
}

void navcoreDisplay::aim(TPos to, TPos from)
{
}


/*****************************************************************************
void navcoreDisplay::ChangeMapMode(int newMode)

Input:   newMode - mode to change to
Description:   this function performs the changes necessary in moving
around the main screen to accomodate the different views
******************************************************************************/
void navcoreDisplay::changemapmode()
{
  //Change the viewport mode

    TPos currentpos;
    if(m_mapmode == MAPMODE_2D_NORTH)
    {
//        Viewport_SetDisplayMode(viewport,Viewport2D);
        m_mapmode = MAPMODE_2D_CAR;
        m_mapangle_flag = true;
    }
    else if(m_mapmode == MAPMODE_2D_CAR)
    {
        Viewport_SetDisplayMode(&viewport,Viewport3D);
        m_mapmode = MAPMODE_3D_CAR;
        m_mapangle_flag = true;
    }
    else
    {
        Viewport_SetDisplayMode(&viewport,Viewport2D);
        m_mapmode = MAPMODE_2D_NORTH;
//        m_mapangle_flag = false;
    }
    Viewport_GetMapOrigin(&viewport,&currentpos);
    Viewport_SetMapOrigin(&viewport,&currentpos);
//    refresh();
}


void navcoreDisplay::setmapmode(MAP_MODE mapmode)
{
    TPos currentpos;
    if(mapmode == MAPMODE_2D_NORTH)
    {
        Viewport_SetDisplayMode(&viewport,Viewport2D);
        m_mapangle_flag = false;
    }
    else if(mapmode == MAPMODE_2D_CAR)
    {
        Viewport_SetDisplayMode(&viewport,Viewport2D);
        m_mapangle_flag = true;
    }
    else
    {
        Viewport_SetDisplayMode(&viewport,Viewport3D);
        m_mapangle_flag = true;
    }
    //m_mapmode = mapmode;
    Viewport_GetMapOrigin(&viewport,&currentpos);
    Viewport_SetMapOrigin(&viewport,&currentpos);
//    refresh();
}

/************************************************************************************
void navcoreDisplay::rotateDisplay( int angle)

Input: angle is rotated 8 degrees clockwise or anti clockwise
Description: rotates the map display to the given angle. Works in 2D and 3D

************************************************************************************/
void navcoreDisplay::rotateDisplay(int angle)
{
  // angle is currently 8, or -8
  Viewport_SetMapAngle(&viewport,Viewport_GetMapAngle(&viewport) + UINT8(angle) );
  this->refresh();
}

// As above but not used for rotating rather for reorienting the display (heading up).
void navcoreDisplay::setDisplayHeading(UINT8 angle)
{
  Viewport_SetMapAngle(&viewport, angle);
}

/************************************************************************************
void navcoreDisplay::set3dTilt( int angle)

Input: angle to tilt the 3d display
Description:   Tilts the 3d display up or down by 3 degrees. (variable passed in from
navtestDisplay.

************************************************************************************/
void navcoreDisplay::set3dTilt(int angle)
{
  // only use if in 3D
  float tilt;

  if (viewport.type == Viewport3D ) {
    tilt = (float)Viewport_GetVerticalAngle(&viewport) + angle;
    LOG_DEBUG(0, ("Vertical angle tilt = %f",tilt));
    Viewport_SetVerticalAngle(&viewport, tilt);
    this->refresh();
  }
}

void navcoreDisplay::setDetailLevel(int detailLevel)
{
  LOD_SetLevel(&viewport.lod, detailLevel);
}

// Retrieve across all active maps loaded, a count of the number of displayable POI categories.
int navcoreDisplay::getPOITypeCount(void)
{
  if (m_bDisplayInitialised) {
    return POITypeFilter_getTotalTypeCount(Viewport_GetPOITypeFilter(&viewport));
  } else { // The display (and the core) not initialised?
    return 0;
  }
}

// Retrieve the POI category information for the nth POI in this index.

/*int navcoreDisplay::getPOIName(TPOIDisplayType *poiCategoryName,
                               int index)
{
  if (displayInitialised) {
    //
    // If an invalid index is supplied then return 1 else return 0.
    //
    return (int)displayPoiTypeList_getTypeDetails(&viewport.poiTypeList.poiTypeList, index, poiCategoryName);
  }   else {
    return 0;
  }
}
*/

// Enable POI category for display
int navcoreDisplay::enableCategoryForDisplay(int typeId)
{
  POITypeFilter_setFlag(Viewport_GetPOITypeFilter(&viewport), typeId, TRUE);
  return 0;
}

// Disable POI category for display
int navcoreDisplay::disableCategoryForDisplay(int typeId)
{

  POITypeFilter_setFlag(Viewport_GetPOITypeFilter(&viewport), typeId, FALSE);
  return 0;
}

// Retrieve the level of detail
int navcoreDisplay::getLevelOfDetail(void)
{
  return (int)LOD_GetLevel(&viewport.lod);
}


// Retrieve the current level of alpha transparency for POI display
int navcoreDisplay::getPOITransparency(void)
{
  return Viewport_GetPoiTransparency(&viewport);
}

// Set the level of alpha transparency for POI display.
bool navcoreDisplay::setPOITransparency(int alphaLevel)
{
  // Do a simple range check and default to opaque (level 0) if
  // the alphaLevel is invalid
  if (alphaLevel >= 0 && alphaLevel <= 256) {
    Viewport_SetPoiTransparency(&viewport, (UINT8)alphaLevel);
    return true;
  } else {
    Viewport_SetPoiTransparency(&viewport, 0);
    return false;
  }
}

// Retrieve the POI scale factor (between -2 and +2).
int navcoreDisplay::getPOIScaleFactor(void)
{
  UINT32 currentScaleFactor = Viewport_GetPoiScale(&viewport);

  // This code makes the gross assumption that value set for
  // scaling are based around 128 shifted a maximum of 2 to the
  // left or right giving a scale factor range of 1/4 to 4 times
  // base size.
  if (currentScaleFactor == 32) {
    return -2;
  } else if (currentScaleFactor == 64) {
    return -1;
  } else if (currentScaleFactor == 128) {
    return 0;
  } else if (currentScaleFactor == 256) {
    return 1;
  } else if (currentScaleFactor == 512) {
    return 2;
  } else {
    // Some bogus non power of two value just return zero.
    LOG_DEBUG(0, ( "Bogus POI scale factor (%d), return 0 instead.",
      currentScaleFactor ));
    return 0;
  }
}

// Set the POI icon scale factor.
void navcoreDisplay::setPOIScaleFactor(int scaleFactor)
{
  // Yeah I know, not the most concise and elegant code...
  switch (scaleFactor)
  {
  case -2: // One quarter size
    Viewport_SetPoiScale(&viewport, 32);
    break;

  case -1: // One half size
    Viewport_SetPoiScale(&viewport, 64);
    break;

  case 0: // Standard size
    Viewport_SetPoiScale(&viewport, 128);
    break;

  case 1: // Double size
    Viewport_SetPoiScale(&viewport, 256);
    break;

  case 2: // Quadruple size
    Viewport_SetPoiScale(&viewport, 512);
    break;

  default:
    LOG_WARN(0, ("Illegal POI icon scale factor (%d)", scaleFactor ));
    break;
  }
}

bool navcoreDisplay::isPolyClassEnabled(TPolClass polyClass)
{
  if (PolyDisplayTheme_IsClassEnabled(Viewport_GetPolyTheme(&viewport), polyClass)) {
    return true;
  } else {
    return false;
  }
}

void navcoreDisplay::setPolygonClass(TPolClass polyClass, BOOL8 enable)
{
  PolyDisplayTheme_EnableClass(Viewport_GetPolyTheme(&viewport), polyClass, enable);
}

bool navcoreDisplay::isPatternedPolyEnabled(void)
{
  if (PolyDisplayTheme_IsPatternEnabled(Viewport_GetPolyTheme(&viewport))) {
    return true;
  } else {
    return false;
  }
}

void navcoreDisplay::setPatternedPolygon(BOOL8 enable)
{
  PolyDisplayTheme_EnablePattern(Viewport_GetPolyTheme(&viewport), enable);
}

// Get the font for a particular map legend display element.
const TFont *navcoreDisplay::getFont(TDisplayFontIndex index)
{
  return FontPalette_GetEntry(Viewport_GetFontPalette(&viewport), index);
}

// Get the font for a particular map legend display element.
void navcoreDisplay::setFont(TDisplayFontIndex index, const TFont *font)
{
  FontPalette_SetEntry(Viewport_GetFontPalette(&viewport), index, font);
}

// Get the colour for a particular map legend display element.
RGBCOLOUR navcoreDisplay::getColour(TDisplayColours index)
{
  RGBCOLOUR rgbColour;
  rgbColour = ColourPalette_GetColour(Viewport_GetColourPalette(&viewport), index);
  return rgbColour;
}

// Set the colour for the map display legend of a particular item.
void navcoreDisplay::setColour(TDisplayColours index, RGBCOLOUR colourVal)
{
  ColourPalette_SetColour(Viewport_GetColourPalette(&viewport), index, colourVal );
}

// Reset the map legend colour table to its default values
void navcoreDisplay::resetColour(void)
{
  ColourPalette_Init(Viewport_GetColourPalette(&viewport));
  PolyDisplayTheme_Init(Viewport_GetPolyTheme(&viewport), Viewport_GetElevationPalette(&viewport));
}

void navcoreDisplay::resetElevationPalette(void)
{
  PolyDisplayTheme_Init(Viewport_GetPolyTheme(&viewport), Viewport_GetElevationPalette(&viewport));
}

BOOL8 navcoreDisplay::isPolygonClassEnabled(TPolClass polyClass)
{
  return ( BOOL8 )PolyDisplayTheme_IsClassEnabled(Viewport_GetPolyTheme(&viewport), polyClass);;
}

RGBCOLOUR navcoreDisplay::getPolygonFillColour(TPolClass polyClass)
{
  return PolyDisplayTheme_GetClassStyle(Viewport_GetPolyTheme(&viewport), polyClass)->fillColour;
}

RGBCOLOUR navcoreDisplay::getPolygonBorderColour(TPolClass polyClass)
{
  return PolyDisplayTheme_GetClassStyle(Viewport_GetPolyTheme(&viewport), polyClass)->borderColour;
}

TPolKind navcoreDisplay::getPolygonKind(TPolClass polyClass)
{
  return PolyDisplayTheme_GetClassStyle(Viewport_GetPolyTheme(&viewport), polyClass)->kind;
}

const TBitmap * navcoreDisplay::getPolygonBitmap(TPolClass polyClass)
{
  return PolyDisplayTheme_GetClassStyle(Viewport_GetPolyTheme(&viewport), polyClass)->pattern;
}

void navcoreDisplay::setPolygonFillColour(TPolClass polyClass, RGBCOLOUR colour)
{
  TPolStyle *polyRenderStyle = PolyDisplayTheme_GetClassStyle(Viewport_GetPolyTheme(&viewport), polyClass);

  polyRenderStyle->fillColour = colour;

  PolyDisplayTheme_SetClassStyle(Viewport_GetPolyTheme(&viewport), polyClass, polyRenderStyle);
}

void navcoreDisplay::setTopoElevationGradients(TElevationPaletteEntry * pElevationTable,
                                               int size)
{
  TPolStyle polStyle;
  Viewport_SetElevationPalette(&viewport,pElevationTable,size);

  polStyle.kind = POL_SOLID;
  polStyle.fillColour = PolyDisplayTheme_GetElevationColour(Viewport_GetPolyTheme(&viewport), 0);

  polStyle.pattern = NULL;
  PolyDisplayTheme_SetClassStyle(Viewport_GetPolyTheme(&viewport),
    POL_CLASS_COVERAGE,
    &polStyle);
}

void navcoreDisplay::setCarAngle(int heading)
{
    if(m_mapangle_flag)
    {
        if(m_mapmode == MAPMODE_2D_NORTH)
        {
            m_mapangle_flag = false;
            Viewport_SetMapAngle(&viewport, (UINT8)0);
        }
        else
        {
            Viewport_SetMapAngle(&viewport, heading);
        }
    }
}

void navcoreDisplay::setCarIconPos(TPos mapPos, UINT8 heading)
{
  CarIcon_SetPosition(&m_carIcon, mapPos.x, mapPos.y);
  CarIcon_SetHeading(&m_carIcon, heading);
}

void navcoreDisplay::setCarIcon( )
{
  CarIcon_SetBitmap(&m_carIcon,&myCarIcon);
}

void navcoreDisplay::clearCarIcon( )
{
  CarIcon_SetBitmap(&m_carIcon,NULL);
}

bool navcoreDisplay::onCarIcon(TPos screenPos, TPos carPos)
{
  UINT16 w, h;

  w = m_carIcon.carIconBitmapPtr->width / 2;
  h = m_carIcon.carIconBitmapPtr->height / 2;

  if (screenPos.x <= carPos.x + w && screenPos.x >= carPos.x - w &&
    screenPos.y <= carPos.y + h && screenPos.y >= carPos.y - h) {
      return true;
    }

    return false;
}

TCarIcon *navcoreDisplay::getCarIcon()
{
  return &m_carIcon;
}

TGeoban *navcoreDisplay::getGeoban()
{
  return &m_geoban;
}

TLatLongGrid *navcoreDisplay::getLatLongGrid()
{
  return &m_latLongGrid;
}

TSky *navcoreDisplay::getSky()
{
  return &m_sky;
}

BOOL8 navcoreDisplay::getSnailTrailToggle() const
{
  return m_renderSnailTrail;
}

void navcoreDisplay::setSnailTrailToggle( BOOL8 on )
{
  m_renderSnailTrail = on;
}



/**********************************************************************
void navcoreDisplay::resizeDisplay(int width, int height)
Input: new screen dimensions
Description: Resizes the map display
**********************************************************************/
void navcoreDisplay::resizeDisplay(int width, int height)
{
//  platform_setScreenSize(width,height);

  Viewport_SetDimensions(&viewport, width, height);
  Camera_SetViewport(&viewport.v3D.camera, 0, (INT32)(1 - height), (INT32)(width - 1), 0);
}

/************************************************************************
void navcoreDisplay::setRoadLabeling( TJourney *journey)
Input: journey object
Description: Sets the road labelling
************************************************************************/
//void navcoreDisplay::setRoadLabeling( TJourney *journey)
//{
//  DisplayNameSet_SetJourney(&m_displayNameSet,journey);
//}

/************************************************************************
void navcoreDisplay::setRenderFlags( SEGMENT_RENDERFLAGS renderFlag )
Input: renderflag (should be either SEGMENT_RENDERSHAPEPOINTS/SEGMENT_RENDERNODES)
Description: Sets the segment rendering flags, determining if shape
points and nodes are to be dislayed
************************************************************************/
//void navcoreDisplay::setRenderFlags(DEBUG_DISPLAY_FLAGS renderFlag)
//{
//  DebugDisplay_SetRenderFlags(renderFlag);
//}

/************************************************************************
SEGMENT_RENDERFLAGS navcoreDisplay::getRenderFlags(  )
Description: returns the current debug segment rendering flags
************************************************************************/
//DEBUG_DISPLAY_FLAGS navcoreDisplay::getRenderFlags()
//{
//  return DebugDisplay_GetRenderFlags();
//}

void navcoreDisplay::showSegmentNodeID( TPos mapPos )
{
//   BOOL8 rc;
//   TRoadSegment segment;
//   TPos fnode, lnode;
//   TPos fspos, lspos;
//    UTF16 segNodeIDTxt[MAX_PATH];
//    TPos txtPos;
//   RGBCOLOUR lineColour = RGB_Define(0,0,255);
//   RGBCOLOUR posColour = RGB_Define(255,0,0);
//    RGBCOLOUR textColour = RGB_Define(0,0,0);
//    refresh();

//   if ( !DebugDisplay_GetSegmentAndPos( &mapPos, &segment, &fnode, &lnode ))
//   {
//      return;
//   }
//   Viewport_MapToScreen(&viewport,&fnode,&fspos);

//   if( lnode.x != 0 || lnode.y != 0 ){
//        Viewport_MapToScreen(&viewport,&lnode,&lspos);
//       GraphContext_DrawLine( &context, fspos.x, fspos.y, lspos.x, lspos.y,  1,  lineColour);
//   }

//    GraphContext_DrawFilledCircle( &context, fspos.x, fspos.y, 3, posColour);

//   _snwprintf( segNodeIDTxt, sizeof(segNodeIDTxt), L"#Segment:%d(%d->%d)", segment.ID, segment.firstNode, segment.lastNode );
//   Vertex_Define(&txtPos, 22, -2 );
//   GraphContext_DrawText( &context, (TFont *) context.font, segNodeIDTxt, &txtPos, textColour);


}
// Highlight the road located at this position
void navcoreDisplay::setHighlightRoadLocation(const TPos mapPos)
{
  BOOL8 rc;
  TMapLocation mapLocation;

  maplocation_initUserLocation( &mapLocation, &mapPos, NULL, -1, ROAD_TYPES );
  rc = maplocation_resolveOnNetwork( &mapLocation, NULL );

  HighlightList_Init(&viewport.displayNameSet.highlightList);

  if (rc) {
    HighlightList_SetMapLocation(&viewport.displayNameSet.highlightList, &mapLocation);
  }

//  DebugDisplay_DumpRoadDetails(&mapPos);
}

void navcoreDisplay::resetHighLightRoadLocation(void)
{
  HighlightList_Init(&viewport.displayNameSet.highlightList);

  HighlightList_SetMapLocation(&viewport.displayNameSet.highlightList, NULL);
}

// End of navcoreDisplay.cpp
