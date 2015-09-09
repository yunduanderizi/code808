#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#include <innovEngine_sdk.h>
#include "navcoreGraphics.h"
#include "system_param.h"
#include "navcoreRoute.h"
#include "navcoreDisplay.h"

#include "datatype.h"
#include "navcoreFavourites.h"


extern TGdiPlatformGraphics g_defaultGraphics;
extern navcoreRoute *g_navRoute;
extern navcoreDisplay *g_navDisplay;

extern navcoreFavourites *g_navFavourites;
extern TArchiveOverlaySet g_overlaySet;

UINT8 *engineGetScreenPtr();
bool engineInitSystem();
void engineCreateContext();
void *engineGetPlatformGraphicsImage();
bool engineCheckSound();
UTF16 *engineSoundText();
int engineGetPicInfo(unsigned char* picturedate,long offset, long size );

#endif // ENGINEINTERFACE_H
