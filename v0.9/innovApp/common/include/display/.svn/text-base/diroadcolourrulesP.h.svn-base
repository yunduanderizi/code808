#ifndef _DIROADCOLOURRULES_H_
#define _DIROADCOLOURRULES_H_

#include <graphics/grcolour.h>
#include <mapdata/enroadP.h>
#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Structure defining a route shield colour rule.
typedef struct TRoadColourRule {
  UINT32 minType;
  UINT32 maxType;
  RGBCOLOUR textColour;
  RGBCOLOUR fillColour;
  RGBCOLOUR borderColour;
  const char *bitmapName;
} TRoadColourRule;

/// Get a route shield colour rule for a particular map.
/// @param mapHeader The map the road is in (only used to identify the country).
/// @param routeType The road's route type.
/// @return Pointer to a colour rule.
TRoadColourRule *roadColourRules_getRule(const TGenericMapInfo *const mapInfo,
                                         UINT16 routeType);

#ifdef __cplusplus
}
#endif

#endif // _DIROADCOLOURRULES_H_
