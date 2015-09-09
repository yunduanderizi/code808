#ifndef DIDISPLAYNAMESET_H
#define DIDISPLAYNAMESET_H

#include <routing/rtjourney.h>
#include <display/didisplayname.h>
#include <display/diroadnameposset.h>
#include <display/diviewporttransition.h>
#include <display/dihighlightlist.h>
#include <mapdata/enroad.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BEGIN HACK */
struct TViewport;
/* END HACK */

/// @addtogroup Display
/// @{

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

/// @addtogroup RoadLabelling
/// @ingroup UnUsed
/// @{

/// @addtogroup DisplayNameSet
/// @ingroup RoadLabelling
/// @{

#define DISPLAY_MAXNAMES      500
#define DISPLAY_MAXTEXT       (DISPLAY_MAXNAMES * 32)

///
/// @typedef struct TDisplayNameSet
///
typedef struct TDisplayNameSet
{
  UINT32 displayNameCount;
  TRoadDisplayName displayNames[DISPLAY_MAXNAMES];
  UTF16 displayNameText[DISPLAY_MAXTEXT];
  UTF16 *currentDisplayName;
  ViewportConfiguration viewport;
  TRoadNamePosSet roadNamePosSet1;
  TRoadNamePosSet roadNamePosSet2;
  TRoadNamePosSet *currentNamePosSet;
  TRoadNamePosSet *previousNamePosSet;
  THighlightList highlightList;
  TJourney *journey;
} TDisplayNameSet;

/// @}

/// @}

/// @}

/// @}

// Shouldnt this be privatE????

extern MAPCORE_API
void  DisplayNameSet_Init(TDisplayNameSet *pThis);

extern MAPCORE_API
void  DisplayNameSet_SetJourney(TDisplayNameSet *pThis, TJourney *journey);

extern MAPCORE_API
void  DisplayNameSet_Sort(TDisplayNameSet *pThis);

extern MAPCORE_API
const UTF16 *DisplayNameSet_AddSegment(TDisplayNameSet *pThis, 
                                       struct TViewport *viewport, 
                                       const struct TRoadSegment *segment);

extern MAPCORE_API
void DisplayNameSet_Show(TDisplayNameSet *pThis,
                         struct TViewport *viewport,
                         TRoadNamePosSet *currentSet,
                         TRoadNamePosSet *prevSet,
                         ViewTransitionType transitionType);

extern MAPCORE_API
void DisplayNameSet_ReviewNotation(TDisplayNameSet *pThis,
                                   struct TViewport *viewport,
                                   TPos *translationVector);

extern MAPCORE_API
void DisplayNameSet_BeginRender(TDisplayNameSet *pThis,
                                struct TViewport *viewport);

extern MAPCORE_API
void DisplayNameSet_Render(TDisplayNameSet *pThis,
                           struct TViewport *viewport);

#ifdef __cplusplus
}
#endif

#endif
