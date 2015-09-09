#ifndef DIZBUFFERP_H
#define DIZBUFFERP_H

#include  <containers/gerbtree.h>
#include  <display/di3Dengi.h>
#include  <display/diviewport.h>
#include  <display/diZBuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TRBSolidPolyData{
  RGBCOLOUR fillColour, borderColour;
} TRBSolidPolyData;

typedef struct TRBTextPolyData{
  TTexture *texture;
  TPos     *cord;
} TRBTextPolyData;

typedef struct TRBPolygonNode {
  TRBTreeNode rbnode;
  unsigned  kind     :  2;  /* textured or solid polygon */    
  unsigned  zmin_s   : 12;  /* zmin in screen coordinates */   
  unsigned  zmax_s   : 12;  /* zmax in screen coordinates */   
  int       zmin_c   : 12;  /* zmin in centered coordinates */ 
  int       zmax_c   : 12;  /* zmax in centered coordinates */ 
  INT16     n;              /* number of vertices (>= 3) */
  TPos      *vert;
  union {
    TRBSolidPolyData solidPolyData;
    TRBTextPolyData  textPolyData;
  } data;
} TRBPolygonNode;

int PolygonDynDepthBuffer_Render( TPolygonDynDepthBuffer *pThis, TViewport *viewport );

#ifdef __cplusplus
}
#endif

#endif // DIZBUFFERP_H
