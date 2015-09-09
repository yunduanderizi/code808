#ifndef GDPREFTURNP_H
#define GDPREFTURNP_H

#include <guidance/gdintersectionP.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GUIDE_MAX_PREFERRED_TURNS 2
#define GUIDE_INVALID_TURN_INDEX -1

typedef enum {
  GUIDE_UNPREFERRED,
  GUIDE_NO_PREFERENCE,
  GUIDE_WEAKLY_PREFERRED,
  GUIDE_STRONGLY_PREFERRED
} TPreferredTurnType;

void getPreferredTurns(TGuideIntersection *pIntersection,
                       INT32 returnedPreferredTurns[],
                       TPreferredTurnType preferredTurnTypes[],
                       INT32 *numPreferredTurns,
                       BOOL8 *pCrossedComplexIntersection);

#ifdef __cplusplus
}
#endif

#endif /* GDPREFTURNP_H */
