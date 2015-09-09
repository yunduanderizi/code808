#ifndef DIHIGHLIGHTLISTP_H
#define DIHIGHLIGHTLISTP_H

#include <display/dihighlightlist.h>
#include <mapdata/enroadP.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

/* I think these are really supposed to be private...  In fact at least one 
   of the methods uses a type private to Map Core. */
extern MAPCORE_API
void HighlightList_AddSegment(THighlightList *pThis,
                              UINT32 segmentID);

extern MAPCORE_API
void HighlightList_SubmitSegment(THighlightList *pThis,
                                 const TRoadSegment *segment,
                                 const UTF16 *name);

extern MAPCORE_API
UINT32 *HighlightList_GetHighlightSegments(THighlightList *pThis,
                                           UINT32 *count);

#ifdef __cplusplus
}
#endif

#endif /* DIHIGHLIGHTLISTP_H */
