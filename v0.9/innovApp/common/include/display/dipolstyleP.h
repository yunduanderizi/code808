#ifndef DIPOLSTYLEP_H
#define DIPOLSTYLEP_H

#include <display/dipolstyle.h>
#include <mapdata/engeomP.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL8 PolyDisplayTheme_GetGeomPolyStyle( 
    TPolyDisplayTheme *pThis, 
    const TGeomHeader *geomHeader, 
    TPolStyle         *style );

#ifdef __cplusplus
}
#endif

#endif /* DIPOLSTYLEP_H */
