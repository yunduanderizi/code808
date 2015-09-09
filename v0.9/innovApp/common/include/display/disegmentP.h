#ifndef DISEGMENTP_H
#define DISEGMENTP_H

#include <display/disegment.h>
#include <mapdata/enroadP.h>
#include <graphics/grcolour.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

/* BEGIN HACK */
struct TViewport;
/* END HACK */

#define SEGMENT_MAX_VERTICES 20

//typedef enum
enum
{
  SEGMENT_RENDERDEFAULT = 0,
  SEGMENT_RENDERGEOBAN = 1,
  SEGMENT_RENDERREGISTERINDISPLAYLIST = 2,
  SEGMENT_RENDERARROWS = 4,
  SEGMENT_RENDEROUTLINE = 8,
  SEGMENT_RENDERSKIPROADTYPES = 16,
  SEGMENT_RENDERTHINNED = 32,
  SEGMENT_RENDERDETAILED = 64,
  SEGMENT_RENDERLOOKUPSECONDPASS = 128,
  SEGMENT_RENDERHIDESEGMENT = 256,
  SEGMENT_RENDERTRIMSEGMENT = 512,
  SEGMENT_RENDERDOORPLATENUM = 1024,
  SEGMENT_RENDER_FORCE_32BITS = 0x7FFFFFFF
};
//} SEGMENT_RENDERFLAGS;

//=struct TViewport;


///
/// Determines how the current segment is to be rendered through the given viewport.
/// @param pThis A segment object.
/// @param viewport A viewport through which the type of visibility should be obtained.
///                 Cannot be NULL.
/// @return TRUE if the segment is seen simplified through the given viewport.
///
extern BOOL8 Segment_IsSeenSimplified(TRoadSegment *pThis, struct TViewport *viewport);

extern BOOL8 Segment_Render(TRoadSegment *pThis,
                            struct TViewport *viewport,
                            SEGMENT_RENDERFLAGS renderFlags);

///
/// Returns the number of vertices of the current segment.
/// @param pThis A segment object.
/// @return The number of vertices.
///
extern UINT32 Segment_GetVertexCount(const TRoadSegment *pThis);

///
/// Retrieves a specific vertex from the current segment.
/// Will assert if the vertex index is out of range.
/// @param pThis A segment object.
/// @param index The vertex number.
/// @param[out] p A vertex object into which to store the segment vertex.
///               Cannot be NULL.
///
extern void Segment_GetVertex(TRoadSegment *pThis, UINT32 index, TPos *p);

extern void Segment_AssertValid(const TRoadSegment *pThis);

extern BOOL8 Segment_IsVisible(TRoadSegment *pThis, struct TViewport *viewport);

extern void Segment_GetDisplayName(const TRoadSegment *pThis,
                                   UTF16 *name,
                                   UINT8 *len,
                                   BOOL8 *isARoute,
                                   UINT16 *routeType);

extern void Segment_GetDisplayNameEx(const TRoadSegment *pThis,
                                   UTF16 *name,
                                   UINT8 *len,
                                   BOOL8 *isARoute,
                                   UINT16 *routeType,
                                   struct TViewport *viewport);

extern BOOL8 Segment_GetVertices(TRoadSegment *pThis,
                                 TPos *vertices, UINT32 *vertSize,
                                 TRect *boundingRect);

extern void Segment_RenderShapepoints(TRoadSegment *pThis,
                                      struct TViewport *viewport,
                                      RGBCOLOUR colour);

extern void Segment_RenderDirection(TRoadSegment *pThis,
                                    struct TViewport *viewport);

extern void Segment_RenderUnpacked(TRoadSegment *pThis,
                                   struct TViewport *viewport);

extern BOOL8 Segment_OverlapRect(TRoadSegment *pThis,
                                 TRoadSectorHeader *sector,
                                 INT32 ixsf,
                                 INT32 iysf,
                                 struct TViewport *viewport);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DISEGMENTP_H */
