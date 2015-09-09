#ifndef DILINEP_H
#define DILINEP_H

#include <display/diline.h>
#include <innovEngine_types.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void DisplaySegment_Init(TDisplaySegment *displaySegment);

//extern TDisplayList displayList;

extern UINT32 DisplayList_GetSegmentCount(TDisplayList *displayList);
extern TDisplaySegment *DisplayList_GetDisplaySegment(TDisplayList *displayList, UINT32 n);
extern TDisplaySegment *DisplayList_StartSegment(TDisplayList *displayList);
extern void DisplayList_AddSegmentLine(TDisplayList *displayList, TDisplayLine *displayLine);
extern void DisplayList_ValidateSegment(TDisplayList *displayList, BOOL8 validate);

extern void   DisplayList_Reset(TDisplayList *pThis);
extern BOOL8  DisplayList_HasLabel(TDisplayList *pThis, const UTF16 *name);
extern BOOL8  DisplayList_ContainsRoad(TDisplayList *pThis, const UTF16 *name);
extern void   DisplayList_SnapToRoad(TDisplayList *pThis, const UTF16 *name, TPos *output, const TPos *calcPos);
extern void   DisplayList_AddLabel(TDisplayList *pThis, const UTF16 *myName, TDisplayLine bestRoad);
extern void   DisplayList_GetRoadCenter(TDisplayList *pThis, const UTF16 *name, INT32* x, INT32* y);
extern BOOL8  DisplayList_CheckLabelOverlapping(TDisplayList *pThis, const TDisplayLine *roadLine);
extern UINT32 DisplayList_CountIntersectionsWithSegments(TDisplayList *pThis, const TDisplayLine *roadLine, UINT32 maxInt);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DILINEP_H */
