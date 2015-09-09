#ifndef DIZBUFFER_H
#define DIZBUFFER_H

#include <containers/gerbtree.h>
#include <display/diviewport.h>
#include <memory/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TPolygonDynDepthBuffer {
  UINT16           polygonCount; /* Number of polygons in the buffer */
  TRBTree          tree;         /* The Red-Black Tree containing 
                                    the polygons */
  TAllocatorHandle allocator;    /* Memory allocator for the tree */
} TPolygonDynDepthBuffer;

void PolygonDynDepthBuffer_Init( 
    TPolygonDynDepthBuffer *pThis );
void PolygonDynDepthBuffer_RecordSolidPol( 
    TPolygonDynDepthBuffer *pThis, 
    const TPol3d           *pol3d_c, 
    TCamera                *camera );
void PolygonDynDepthBuffer_RecordTexturedPol( 
    TPolygonDynDepthBuffer *pThis,
    const TPol3d           *pol3d_c,
    TCamera                *camera );
void PolygonDynDepthBuffer_Destroy( 
    TPolygonDynDepthBuffer *pThis );

#ifdef __cplusplus
}
#endif

#endif // DIZBUFFER_H
