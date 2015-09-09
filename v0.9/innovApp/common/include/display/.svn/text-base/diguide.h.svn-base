#ifndef DIGUIDE_H
#define DIGUIDE_H

#include <guidance/gdcontext.h>
#include <innovEngine_types.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

#define TURNARROW_MIN_HEIGHT 1
#define TURNARROW_MAX_HEIGHT 15
#define TURNARROW_DEFAULT_HEIGHT 3

extern MAPCORE_API
STATICMETHOD BOOL8 TurnArrow_SetHeight(UINT32 height);

extern MAPCORE_API
STATICMETHOD UINT32 TurnArrow_GetHeight(void);

extern MAPCORE_API
void GuideContext_Render(TGuideContext *pThis, struct TViewport *viewport);

#ifdef __cplusplus
}
#endif

#endif
