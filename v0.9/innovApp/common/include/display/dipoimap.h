#ifndef DIPOIMAP_H
#define DIPOIMAP_H

#include "platform_types.h"
#include <innovEngine_types.h>
#include <math/gerect.h>
#include <graphics/grbitmap.h>

#ifdef __cplusplus
extern "C" {
#endif

extern MAPCORE_API
void POIMap_setGlobalPOIMode(UINT8 iconstyle);

extern MAPCORE_API
void POIMap_setGlobalBitmapFile(const char *filename);

extern MAPCORE_API
void POIMap_setGlobalBrandBitmapFile(const char *filename);
//@CHN S
extern MAPCORE_API 
TBitmap *GetGlobalPOI(int iType, int iStyle);
//@TWN
extern MAPCORE_API 
TBitmap *GetGlobalBitmap(const char * iconName, int iStyle);

extern MAPCORE_API
TBitmap *POIMap_FindBrandBitmap(const char *name);

typedef enum TPOIStyle {
  POI_STYLE_MAP,
  POI_STYLE_MAP_NIGHT,
  POI_STYLE_NORMAL,
  POI_STYLE_NIGHT,
  POI_STYLE_RED,
  POI_STYLE_RED_DAY,
  POI_STYLE_RED_NIGHT,
  POI_STYLE_MAX //@TWN
} TPOIStyle;

extern const char* poitype_icon[];
//@CHN E



//add for ap get poi icon by Edison at 2008.9.24
//#include <display/dibmdefine.h>


#ifdef __cplusplus
}
#endif

#endif
