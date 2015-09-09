#ifndef GDLISTP_H
#define GDLISTP_H

#include <guidance/gdlist.h>
#include <guidance/gdintersectionP.h>
#include <locationprovider/lpjourneyloc.h>

#ifdef __cplusplus
extern "C" {
#endif

extern BOOL8 guideList_add(TGuideList *pThis,
                           const TGuideInstruction *pGuideInstruction,
                           INT32 index);

extern BOOL8 guideList_remove(TGuideList *pThis,
                             INT32 index);

extern UINT16* guideList_malloc(TGuideList *pThis, UINT32 size);

extern INT32 guideList_pathToGuide(const TGuideList *pThis,
                                   INT32 pathIndex,
                                   TJourneyLegNumber legNumber);

extern void guideList_dump(const TGuideList *pThis);

extern void guideList_diagnosticDump(const TGuideList *pThis);

extern BOOL8 guideList_getInstrLocation(const TGuideList *pThis,
                                        INT32 index,
                                        TJourneyLocation *pLoc);

extern INT32 guideList_getLocationInstrIndex(const TGuideList *pThis,
                                             const TJourneyLocation *pLoc,
                                             BOOL8 *turnInProgress);

// @TPE internal
// Get the driving distance and estimated travel time between current locations and
// the destination on a journey.
extern BOOL8 guideList_timeAndDistanceToDestination(const TGuideList *pThis,
                                       const TJourneyLocation *pLoc,
                                       UINT32 *distanceToDestination,
                                       UINT32 *timeToDestination);

extern BOOL8 guideList_timeAndDistanceToInstr(const TGuideList *pThis,
                                              INT32 index,
                                              const TJourneyLocation *pLoc,
                                              UINT32 *distanceToInstr,
                                              UINT32 *timeToInstr);

extern BOOL8 guideList_isOffRamp(const TGuideList *pThis,
                                 const TGuideIntersection *pIntersection);

extern TSegmentID guideList_findNextNamedRoad( const TGuideList *pThis,
                                               TGuideIntersection *intersection,
                                               const TGuideInstruction *turn );


extern BOOL8 analyseExitBifurcation(const TGuideIntersection *pIntersection,
                                    const TJourney *pJourney,
                                    TJourneyLegNumber legNumber,
                                    TGuideTurnDirection *pEntryDir,
                                    TGuideTurnDirection *pExitDir);

extern UINT32 lanesCountLaneBits( UINT32 laneBits, UINT32 numBits, UINT32 * whichBit );

extern BOOL8 guideList_isEnterExpressWay(const TGuideList *pThis,
								         const TGuideIntersection *pIntersection);

#ifdef __cplusplus
}
#endif

#endif /* GDLISTP_H */
