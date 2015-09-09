#ifndef DIROADSECTORP_H
#define DIROADSECTORP_H

#include <display/diroadsector.h>
#include <display/di3Dengi.h>
#include <display/disegment.h>
#include <mapdata/enroadP.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

typedef enum
{
  SECTOR_RENDERDEFAULT = 0,
  SECTOR_RENDERANDRECORD = 1,
  SECTOR_RENDER_FORCE_32BITS = 0x7FFFFFFF
} SECTOR_RENDERFLAGS;

///
/// Returns the sector of the current segment.
/// The sector is automatically svm_lock'ed.
/// You must call RoadSector_Release() on the returned pointer
/// unless the returned value is NULL.
/// @param pThis A segment object.
/// @return pThis The sector to which this segment belongs.
///               Returns NULL if an error occurs.
///
extern TRoadSectorHeader *Segment_GetSector(TRoadSegment *pThis);

///
/// Releases the current road sector object.
/// As of 31/08/2004, only road sectors obtained through Segment_GetSector()
/// need to be released through this method.
/// @param pThis A road sector object (cannot be NULL).
///
extern void RoadSector_Release(TRoadSectorHeader *pThis);

extern void RoadSector_GetBoundingRect(const TRoadSectorHeader *pThis, TRect *rect);

extern BOOL8 RoadSector_IsVisible(TRoadSectorHeader *pThis, struct TViewport *viewport);

extern UINT32 RoadSector_Render(TRoadSectorHeader *pThis,
                                TRoadNetworkType network,
                                TRoadType maxRoadType,
                                struct TViewport *viewport,
                                SEGMENT_RENDERFLAGS renderFlags);

//extern const TPlaneIdx *RoadSector_GetClipPlanes(TRoadSectorHeader *pThis, TViewport *viewport);

extern void RoadSector_GetClipPlanes(TRoadSectorHeader *pThis,
                                     struct TViewport *viewport,
                                     TPlaneIdx *clipPlanes);

extern UINT32 RoadSector_LockContents(TRoadSectorHeader *pThis);

extern UINT32 RoadSector_UnlockContents(TRoadSectorHeader *pThis);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif
