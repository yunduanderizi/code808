#ifndef DIROADSUPERSECTORP_H
#define DIROADSUPERSECTORP_H

#include <display/diroadsupersector.h>
#include <display/disegment.h>
#include <mapdata/enroadP.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

///
/// Renders the current road supersector.
/// @param header The RoadMap that this super sector belongs to
/// @param network The maximum network to be renderered
/// @param roadMapLayer The road map layer to be renderered
/// @param maxRoadType The maximum road type to be renderered,
///                    or ROAD_TYPES if the maximum road type
///                    is to be determined using the viewport
/// @param pass Reserved. This parameter will disappear.
/// @param renderFlags Rendering flags that define rendering options.
///
extern void SuperSector_Render(TRoadSuperSectorHeader *pThis,
                               TRoadNetworkType network,
                               TRoadMapLayer roadMapLayer,
                               TRoadType maxRoadType,
                               struct TViewport *viewport,
                               SEGMENT_RENDERFLAGS renderFlags);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIROADSUPERSECTORP_H */
