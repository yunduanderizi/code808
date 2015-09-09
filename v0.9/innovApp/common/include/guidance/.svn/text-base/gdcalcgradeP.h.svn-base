#ifndef GDCALCGRADEP_H
#define GDCALCGRADEP_H

#include <guidance/gdintersectionP.h>
#include <math/mtangleP.h>
#include <guidance/gdinstruction.h>
#include <guidance/gdprefturnP.h>

#ifdef __cplusplus
extern "C" {
#endif

void calculateTurnDirBifurcation(const TGuideIntersection *pIntersection,
                                 TGuideTurnDirection *turnDirection,
                                 TAngle *relativeDestAngle);

TGuideTurnGrade calculateTurnDirAndGrade(const TGuideIntersection *pIntersection,
                                         const INT32 preferredTurnIndices[],
                                         const TPreferredTurnType preferredTurnTypes[],
                                         const INT32 numPreferredTurns,
                                         const BOOL8 mustGiveGuidance,
                                         TGuideTurnDirection *turnDirection,
                                         TAngle *relativeDestAngle);

void calculateDirNumFromDest(INT16* pLeft, 
                             INT16* pRight, 
                             const TGuideIntersection* pIntersection);

#ifdef __cplusplus
}
#endif

#endif /* GDCALCGRADEP_H */
