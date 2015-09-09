#ifndef LPJOURNEYLOCP_H
#define LPJOURNEYLOCP_H

#include <locationprovider/lpjourneyloc.h>
#include <math/mtangleP.h>

#ifdef __cplusplus
extern "C" {
#endif

extern BOOL8 journeyLocation_getFromPathIndex(TJourneyLocation *pThis,
                                              const TJourney *pJourney,
                                              TJourneyLegNumber legNumber,
                                              UINT32 pathSegIndex,
                                              TRoadSegment *pSegment);

BOOL8 journeyLocation_getFromPathIndexProp(TJourneyLocation *pThis,
                                           const TJourney *pJourney,
                                           TJourneyLegNumber legNumber,
                                           UINT32 pathSegIndex,
                                           TRoadSegment *pSegment,
                                           UINT32 proportion);

extern BOOL8 journeyLocation_getFromLegOrigin(TJourneyLocation *pThis,
                                              const TJourney *pJourney,
                                              TJourneyLegNumber legNumber,
                                              TRoadSegment *pSegment);

extern BOOL8 journeyLocation_getFromLegDestination(TJourneyLocation *pThis,
                                                   const TJourney *pJourney,
                                                   TJourneyLegNumber legNumber,
                                                   TRoadSegment *pSegment);

extern BOOL8 journeyLocation_move(TJourneyLocation *pThis,
                                  const TJourney *pJourney,
                                  INT32 distance,
                                  INT32 *distanceMoved);

extern BOOL8 journeyLocation_move2(TJourneyLocation *pThis,
                                  const TJourney *pJourney,
                                  INT32 distance,
                                  INT32 *distanceMoved,
                                  UINT8 *passPoint);

extern INT32 journeyLocation_compare(const TJourneyLocation *pLoc1,
                                     const TJourneyLocation *pLoc2);

extern BOOL8 journeyLocation_getPosition(TJourneyLocation *pThis,
                                         TRoadSegment *pSegment,
                                         TPos *pPos,
                                         THeading *pHeading);
extern BOOL8 tunnel_move(TTunnelSimLocation *pThis,
                                  INT32 distance,
                                  INT32 *distanceMoved);

extern BOOL8 tunnel_getPosition(TTunnelSimLocation *pThis,
                                         TRoadSegment *pSegment,
                                         TPos *pPos,
                                         THeading *pHeading);

#ifdef __cplusplus
}
#endif

#endif /* LPJOURNEYLOCP_H */
