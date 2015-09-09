#ifndef DIJOURNEYLEGP_H
#define DIJOURNEYLEGP_H

#include <display/dijourneyleg.h>
#include <locationprovider/lpjourneyloc.h>
#include <routing/rtjourney.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

UINT32 JourneyLeg_GetSegmentCount(const TJourneyLeg *pThis);

void JourneyLeg_Render( const TJourneyLeg *pThis, 
                        struct TViewport  *viewport, 
                        TJourneyLocation  *journeyLoc, 
                        BOOL8             bBorder);

///
/// Retrieves the vertices of the specified segment index of the current journey leg.
/// @param[in] pThis          A journey leg object.
/// @param[in] segmentIndex   The zero-based segment index in the current journey leg.
/// @param[in] viewport       The viewport specifying in which context how
///                           the journey leg will be displayed. Can be NULL.
///                           Provide this parameter whenever possible everytime
///                           journey leg vertices are used for display purposes.
/// @param[out] seg            A pointer to a TRoadSegment object receiving the segment
///                           at this segment index. Can be NULL.
/// @param[out] output        A TPos array of at least MAX_VERTEX_COUNT size to receive the output
UINT32 JourneyLeg_GetVertices(const TJourneyLeg *pThis,
                              UINT32             segmentIndex,
                              struct TViewport  *viewport,
                              TRoadSegment      *segment,
                              TPos              *output);

#ifdef __cplusplus
}
#endif

#endif // DIJOURNEYLEGP_H
