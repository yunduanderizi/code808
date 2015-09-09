#ifndef DIROADNAMEPOSSET_H
#define DIROADNAMEPOSSET_H

#include <display/diroadnamepos.h>
#include <display/diviewporttransition.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Display
/// @{

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

/// @addtogroup RoadLabelling
/// @ingroup UnUsed
/// @{

/// @addtogroup RoadNamePosSet
/// @ingroup RoadLabelling
/// @{

#ifndef DISPLAY_MAXNAMES
#define DISPLAY_MAXNAMES      500
#define DISPLAY_MAXTEXT       (DISPLAY_MAXNAMES * 32)
#endif

///
/// @typedef struct TRoadNamePosSet
///
typedef struct TRoadNamePosSet
{
  UINT32 namePosCount; // Number of available TRoadNamePos entries
  TRoadNamePos namePos[DISPLAY_MAXNAMES];
  UTF16 *currentNamePosName;
  UTF16 namePosText[DISPLAY_MAXTEXT];
  ViewportConfiguration viewport;
} TRoadNamePosSet;

/// @}

/// @}

/// @}

/// @}
extern MAPCORE_API
void RoadNamePosSet_Init(TRoadNamePosSet *pThis);

extern MAPCORE_API
void RoadNamePosSet_Show(TRoadNamePosSet *pThis,
                         struct TViewport *viewport,
                         TRoadNamePosSet *nameSetWhereToRegister,
                         ViewTransitionType transitionType);

#ifdef __cplusplus
}
#endif

#endif
