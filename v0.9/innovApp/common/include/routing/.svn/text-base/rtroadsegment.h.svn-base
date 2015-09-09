//
// Declaration of routing related TRoadSegment functions
//
// $Id: rtroadsegment.h 5457 2006-09-19 23:46:09Z grahams $
//

#ifndef RTROADSEGMENT_H
#define RTROADSEGMENT_H

#include <innovEngine_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @{
/// @addtogroup Path
/// @ingroup Routing
/// @{

#define MAX_VERTEX_COUNT  17

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// @typedef struct TSegmentVertices
/// Segment vertices type to hold the vertices (nodes and shapespoints)
/// that represent all or part of a segment.
typedef struct TSegmentVertices {
  UINT32 verticesCount;
  TPos   vertices[17];
  BOOL8  verticesTowardFirstNode;
} TSegmentVertices;

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif // RTROADSEGMENT_H
