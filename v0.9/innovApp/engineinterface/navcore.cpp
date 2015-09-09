
/********************************************************************************
  navcore.cpp

  This is the parent navcore class. It performs the global navcore initializing
  required to do graphics/display/routing/geocoding


Date: 07.12.2004
Author: Michelle Lorenz - mlorenz@21com.com
********************************************************************************/
#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
/*
#include <dos.h>
#include <process.h>
#include <sys\timeb.h>
#include <sys/types.h>
#include <signal.h>
*/
#include <time.h>

#include <innovEngine_sdk.h>

#include "navcore.h"
//#include "navcoreLogging.h"

// simon was here
#include "navcoreDisplay.h"
#include "navcoreGraphics.h"
#include <trail/ensnailtr.h>
#include <logging/sdebug.h>
#include <display/didebugP.h>
#include "system_param.h"


typedef struct TFontMap {
    TFont tFont[DISPLAY_FONT_PALETTE_SIZE];
    //HFONT hFont[DISPLAY_FONT_PALETTE_SIZE];
} TFontMap;

TFontMap g_mapFontMap;

TFont *fonts[DISPLAY_FONT_PALETTE_SIZE + 1] = {
    &g_mapFontMap.tFont[DISPLAY_FONT_ROAD_LABEL],
    &g_mapFontMap.tFont[DISPLAY_FONT_MOTORWAY_LABEL],
    &g_mapFontMap.tFont[DISPLAY_FONT_AA_LABEL_IMP_0],
    &g_mapFontMap.tFont[DISPLAY_FONT_AA_LABEL_IMP_1],
    &g_mapFontMap.tFont[DISPLAY_FONT_AA_LABEL_IMP_2],
    &g_mapFontMap.tFont[DISPLAY_FONT_AA_LABEL_IMP_3],
    &g_mapFontMap.tFont[DISPLAY_FONT_AA_LABEL],
    &g_mapFontMap.tFont[DISPLAY_FONT_FPS],
    &g_mapFontMap.tFont[DISPLAY_FONT_3D_LABEL],
    &g_mapFontMap.tFont[DISPLAY_FONT_ROUTEFLAG_LABEL],
    NULL
};


//
// Map Layer Session Configuration file object
//
//extern SessionConfiguration *g_navMapCfg;
//extern navcoreLanguage      *g_navSoundcfg;
//extern navcoreSimulator     *g_navSimulator;
//extern navcoreGeocode       *g_navGeocode;
//extern navcoreLogging       *g_navLogging;

//extern TFontMap g_mapFontMap;
static char bmPOIType[260];
static char bmPOIBrand[260];
static char bmRoadShield[260];
static char binCHNFilename[260];

#define LOAD_MAPS( fileTypeEnum, mapTypeEnum, mapTypeString )                   \
  successCount = 0;                                                             \
  mapCount = g_navMapCfg->GetMapFileCount(fileTypeEnum);                        \
                                                                                \
  for (i = 0; i < mapCount; i++) {                                              \
    fileInfo = g_navMapCfg->GetMapFileInfo(fileTypeEnum,i);                     \
    if ( (TMapFeatureClass)fileInfo.type == MAP_FEATURE_CLASS_LOW ){            \
      LOG_INFO(0, ("%s - Map [%s] has been registered as low end",              \
                    mapTypeString, fileInfo.name.c_str()) );                    \
                                                                                \
    }                                                                           \
    if ( mapfile_load( fileInfo.name.c_str(),                                   \
                       mapTypeEnum,                                             \
                       (TMapFeatureClass)fileInfo.type) == MAPFILE_SUCCESS ){   \
      successCount++;                                                           \
    }                                                                           \
  }                                                                             \
  LOG_INFO(0, ("%s loaded (%u of %u)", mapTypeString, successCount, mapCount));



navcore::navcore()
    :screen(NULL)
    ,oldLength(0)
    ,colourDepth(16)
{
  FontCollection_Define(&fontCollection, (const TFont**)fonts);
}

navcore::~navcore()
{
}

/*********************************************************************************
  void navcore::init()

  Description:  Performs all the global initialization required to do any
          graphics/display/geocoding/routing. This essentially
          initializes the innovEngine engine.

*********************************************************************************/
void navcore::init()
{
    initViewport();
}

void navcore::createcontext()
{
    static unsigned int c = 0;
    screen = new UINT8[SCREEN_WIDTH * SCREEN_HEIGHT * 4];
    memset(screen,0,SCREEN_WIDTH * SCREEN_HEIGHT * 4 * sizeof(UINT8));

    GraphContext_RegisterPlatformGraphics(NULL, (TPlatformGraphics*)&g_defaultGraphics);
}

/*********************************************************************************
  bool navcore::initContext()

  Description:  Initializes the graphics context

*********************************************************************************/
bool navcore::initContext()
{
    GraphContext_Define(&context,
                        screen,
                        colourDepth,
                        SCREEN_WIDTH, SCREEN_HEIGHT,
                        graph_extents);

    context.font = fontCollection.fonts[0];

    // initially blank screen
    GraphContext_Fill(&context, RGB_Define(0,0,0));

    return true;

}
/***************************************************************************************
  bool navcore::initViewport()

  Description: Intiialize the viewport
***************************************************************************************/
bool navcore::initViewport()
{
    UINT32 width, height;

//    platform_getScreenSize(&width,&height);

    Viewport_Create(&viewport, &context);
    Viewport_SetDisplayMode(&viewport, Viewport2D);
    Viewport_SetVerticalAngle(&viewport, 56);

    FontPalette_Create(Viewport_GetFontPalette(&viewport));

    for (UINT32 i = 0; i < (UINT32)DISPLAY_FONT_PALETTE_SIZE; i++) {
      FontPalette_SetEntry(Viewport_GetFontPalette(&viewport),
                           (TDisplayFontIndex)i,
                           FontCollection_GetFontByIndex(&fontCollection, i));
    }
    renderParams.members = RENDERPARAMS_FLAGS;
    renderParams.renderFlags = (WORLD_RENDERFLAGS)WORLD_RENDERDEFAULT;

    //set patterned polygons, enabled for both viewports
    //currently disabled by default by the core
    PolyDisplayTheme_EnablePattern(Viewport_GetPolyTheme(&viewport), TRUE);


    return true;
}

/***************************************************************************************
  void navcore::saveViewport()

  Description: Save the current viewport.
***************************************************************************************/
void navcore::saveViewport()
{
  TPos currentPos;

  Viewport_GetMapOrigin(&viewport, &currentPos);
  Viewport_SetMapOrigin(&viewport, &currentPos);
}

/***************************************************************************************
  void navcore::set3DViewport()

  Description: Save the current viewport and then set it to the 3D viewport
***************************************************************************************/
void navcore::set3DViewport()
{
  Viewport_SetDisplayMode(&viewport, Viewport3D);
  saveViewport();
}

/***************************************************************************************
  void navcore::getViewport()

  Description: Gets the current viewport
***************************************************************************************/
TViewport *navcore::getViewport()
{
    return &viewport;
}


/***************************************************************************************
  void navcore::set2DViewport()

  Description: Save the current viewport and then set it to the 2D viewport
***************************************************************************************/
void navcore::set2DViewport()
{
  Viewport_SetDisplayMode(&viewport, Viewport2D);
  saveViewport();
}

// End of navCore.cpp
