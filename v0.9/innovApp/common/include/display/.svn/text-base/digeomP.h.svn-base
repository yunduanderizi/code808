#ifndef DIGEOMP_H
#define DIGEOMP_H

#include <display/digeom.h>
#include <mapdata/engeomP.h>
#include <display/diviewportP.h>

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern BOOL8 Geom_Render( TGeom              *geom,
                          const TGeomContext *geomContext,
                          TViewport          *viewport,
                          BOOL8               assumeClipped);

extern BOOL8 Geom_RenderForChina( TGeom              *geom,
                                  const TGeomContext *geomContext,
                                  TViewport          *viewport,
                                  BOOL8               assumeClipped);


BOOL8 Geom_Init(TViewport *viewport);

void Geom_Free(void);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DICITYP_H */
