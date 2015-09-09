#ifndef DIHIGHLIGHTLIST_H
#define DIHIGHLIGHTLIST_H

#include <mapdata/enroad.h>
#include <language/utf16.h>
#include <location/locmaplocation.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BEGIN HACK */
struct TViewport;
/* END HACK */

#define MAX_HILIGHT_SEGMENTS  40

/// @addtogroup Display
/// @{

///@addtogroup UnUsed
/// @ingroup Display
/// @{

///@addtogroup HighlightList
/// @ingroup UnUsed
/// @{


///
/// @typedef struct THighlightList
///
typedef struct THighlightList
{
  UTF16 highlightedName[ROAD_MAX_NAME_SIZE];
  UINT32 highlightSegments[MAX_HILIGHT_SEGMENTS]; // segment IDs
  UINT32 numHighlightSegments;
} THighlightList;

extern MAPCORE_API
void HighlightList_Init(THighlightList *pThis);

extern MAPCORE_API
void HighlightList_Render(THighlightList *pThis,
                          struct TViewport *viewport);

extern MAPCORE_API
void HighlightList_SetMapLocation( THighlightList *pThis, 
                                   TMapLocation   *mapLocation );

/// @}
/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif
