#ifndef GDCALCTURNSTYLEP_H
#define GDCALCTURNSTYLEP_H

#include <guidance/gdlist.h>
#include <guidance/gdintersectionP.h>

#ifdef __cplusplus
extern "C" {
#endif

TGuideTurnStyle calculateTurnStyle(const TGuideList *pThis,
                                   TGuideIntersection *pIntersection,
                                   TGuideIntersection *pNextIntersection,
                                   TJourneyLegNumber legNumber);

#ifdef __cplusplus
}
#endif

#endif /* GDCALCTURNSTYLE_H */
