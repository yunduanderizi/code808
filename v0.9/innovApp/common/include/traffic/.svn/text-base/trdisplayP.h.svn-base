#ifndef TRDISPLAYP_H
#define TRDISPLAYP_H

#include <display/diiconranklist.h>
#include <routing/rtjourney.h>
#include <traffic/trapi.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

UINT8 tmcEventIconImportance(TAppTrafficEventClass evClass);

extern MAPCORE_API
void tmcEventList_Render(struct TViewport *viewport,
                         TIconRankSet *rankSet,
                         const TJourney *currentJourney);
extern MAPCORE_API
void tmcEventList_RenderExtents(struct TViewport *viewport);

#ifdef __cplusplus
}
#endif

#endif
