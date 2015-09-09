#ifndef ENWORLDP_H
#define ENWORLDP_H

#include <mapdata/enworld.h>
#include <platform.h>
#include <innovEngine_types.h>

#ifdef __cplusplus
extern "C" {
#endif

// converts from map coordinates to metres.  The calculation performed is:
// scale = lookup_forwardScale[abs(latitude) >> 16];
// metres = (latitude * scale) >> 12;
// The scaling from latitude values to metres alters with latitude - at
// higher latitudes the scale factor decreases.  It is constant for
// longitudes (use the value at latitude 0).
extern INT32 World_GetForwardScale(UINT8 x);

// converts from metres to map coordinates.  The calculation performed is:
// scale = lookup_backwardScale[abs(latitude) >> 16];
// latitude = (metres * scale) >> 12;
// The scaling from metres to latitude alters with latitude - at
// higher latitudes the scale factor increases.  It is constant for
// longitudes (use the value at latitude 0).
extern INT32 World_GetBackwardScale(UINT8 x);

extern UINT32 approximateDistance(const TPos p1,
                                  const TPos p2);

extern void World_getForwardScaleRelative(TPos *scaleData,
                                          const TPos rawData,
                                          const TPos relativePos);
#ifdef __cplusplus
}
#endif

#endif /* ENWORLDP_H */
