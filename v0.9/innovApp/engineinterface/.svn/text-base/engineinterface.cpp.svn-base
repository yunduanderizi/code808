#include "engineinterface.h"
#include "filenamelist.h"
#include <stdio.h>
#include "navcoreJourney.h"
#include "navcoreRoute.h"

#include <mapdata/enroadP.h>
#include <engineinterface/car.h>
#include "interface_logistics.h"
#include <string.h>
#include <locale.h>
#include "TTS/platformTTS.h"
#include "navcoreFavourites.h"
#include "goldstar.h"
#include "engineinterface/interface_route.h"

navcoreRoute *g_navRoute = new navcoreRoute();
navcoreDisplay *g_navDisplay = new navcoreDisplay();
navcoreFavourites *g_navFavourites = new navcoreFavourites();
static bool g_bSoundFlag = false;
static UTF16 g_soundText[100];
TLatLongGrid g_latLongGrid;
FILE *g_junctionfile= NULL;

void TTSCallbackFunctext(const UTF16 *fullText, UINT32 fullTextLength,
                               const UTF16 *shortText, UINT32 shortTextLength,
                               TSpeechPriority priority)
{
    if(100 < (fullTextLength + 1))
    {
        printf("error, bigger than 100!\n");
    }
    memcpy(g_soundText, fullText, (fullTextLength + 1)* sizeof(UTF16));
    g_bSoundFlag = true;
}

static void registerCallback()
{
    if (language_registerTokenCallback(NULL))
    {
        printf("register TOKEN success! \n");
    } else{
        printf("register TOKEN false! \n");
    }
    if(language_registerTTSCallback(&TTSCallbackFunctext))
    {
        printf("register TTS success! \n");
    } else{
        printf("register TTS false! \n");
    }
}

static void loadGrammar()
{
    TGrammarInterface grammarInterface;
    void *m_grammarLib = NULL;
    bool m_isGrammarInitialised = false;

    // Load the grammar library
    m_grammarLib = platformLoadGrammar(GRAMMER_FILE_PATH, &grammarInterface);
    UTF16 *wlookup = new UTF16[100];
    language_UTF8ToUTF16(wlookup, (UTF8 *)LOOKUP_FILE_PATH);
    if (m_grammarLib != NULL) {
        //lookup.bin :path is written later, not now.
      m_isGrammarInitialised = language_loadGrammar(&grammarInterface, wlookup) ? true : false;
      printf( "m_isGrammarInitialised : %d \n" , m_isGrammarInitialised);
    }
    delete [] wlookup;
}

static bool initMapData()
{
    //read road file
    for(int i = 0;i< 31;i++)
    {
        TMapfileStatus status;
        status = mapfile_load(roadmapdata[i],MAPTYPE_ROAD,MAP_FEATURE_CLASS_FULL);
        if(status != MAPFILE_SUCCESS)
        {
            printf("%s load fail\n",roadmapdata[i]);
            continue;
        }
    }

    for(int i = 0;i< 1;i++)
    {

        TMapfileStatus status;
        status = mapfile_load(poitypemapdata[i],MAPTYPE_POITYPE,MAP_FEATURE_CLASS_FULL);
        if(status != MAPFILE_SUCCESS)
        {
            printf("%s load fail\n",poitypemapdata[i]);
            continue;
        }
    }

    for(int i = 0;i< 32;i++)
    {
        TMapfileStatus status;
        status = mapfile_load(polymapdata[i],MAPTYPE_GEOM,MAP_FEATURE_CLASS_FULL);
        if(status != MAPFILE_SUCCESS)
        {
            printf("%s load fail\n",polymapdata[i]);
            continue;
        }
    }

    for(int i = 0;i< 62;i++)
    {
        TMapfileStatus status;
        status = mapfile_load(poimapdata[i],MAPTYPE_POI,MAP_FEATURE_CLASS_FULL);
        if(status != MAPFILE_SUCCESS)
        {
            printf("%s load fail\n",poimapdata[i]);
            continue;
        }
    }

    TMapfileStatus status;
    status =mapfile_load(RESOURCEARCHIVE,MAPTYPE_ARCHIVE,MAP_FEATURE_CLASS_FULL);
    if(status ==MAPFILE_SUCCESS)
    {
        g_junctionfile=fopen(RESOURCEARCHIVE,"rb");
    }

    // POI bitmap icon files to be loaded.
    POIMap_setGlobalBitmapFile(RESOURCEPOI);
    POIMap_setGlobalBrandBitmapFile(RESOURCEBRAND);
    Bin_setGlobalBinFile_CHN(RESOURCECHINA);

    return true;
}

UINT8 *engineGetScreenPtr()
{
    return g_defaultGraphics.pscreen;
}

bool engineInitSystem()
{
    engine_shutdown();
    if (!engine_init()) {
        //
    }
    initMapData();
#if defined(ARM_LINUX)
    loadGrammar();
    registerCallback();
#endif
    g_navDisplay->initContext();
    g_navDisplay->initViewport();
    g_navDisplay->initDisplay(SCREEN_WIDTH,SCREEN_HEIGHT);

    LatLongGrid_Init(&g_latLongGrid);
    LatLongGrid_SetStep(&g_latLongGrid, 100000, 100000);

    gps_init();
#if defined(ARM_LINUX)
    gps_setContext(NMEA_CONTEXT_GPS);
#else
    gps_setContext(NMEA_CONTEXT_MANUAL);
#endif

}

UTF16 *engineSoundText()
{
    return g_soundText;
}

bool engineCheckSound()
{
    if(g_bSoundFlag)
    {
        g_bSoundFlag = false;
        return true;
    }
    else
    {
        return false;
    }
}

void engineCreateContext()
{
     g_navDisplay->createcontext();
}

void *engineGetPlatformGraphicsImage()
{
    return g_defaultGraphics.image;
}

int engineGetPicInfo(unsigned char* picturedate,long offset, long size )
{
    if(g_junctionfile==NULL)
    {
        return -1;
    }
    fseek(g_junctionfile,offset,0);

    if(fread(picturedate,sizeof(char),size,g_junctionfile)==-1)
    {
        return -1;
    }

    return 0;
}

