#ifndef ENWORLD_H
#define ENWORLD_H

#include <innovEngine_types.h>
#include <math/gerect.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Calculate the distance in metre between 2 input points specified
 * in map units.
 *
 * The center of the 2 points is used as center of projection
 * to project lat/long to metre
 *
 * @param pos1[input] first input point in map units
 * @param pos2[input] second input point in map units
 * @return distance in metre between pos1 and pos2
 */
UINT32 World_distance(const TPos *pos1, const TPos *pos2);

UINT32 World_distanceToRect( const TPos *pos, const TRect *rect );

TPos World_metresToMap(const TPos *pos1_m,
                       const INT32 latitude);

TPos World_mapToMetres(const TPos *pos1,
                       const TPos *pos2,
                       const INT32 latitude);


#ifdef __cplusplus
}
#endif

#endif /* ENWORLD_H */
