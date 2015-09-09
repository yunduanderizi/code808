#ifndef GRLOOKUPP_H
#define GRLOOKUPP_H

#include <graphics/grlookup.h>
#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @internal Table of red component of the palette.  THIS POINTER SHOULD BE 
    REMOVED WHEN ALL GLOBAL DATA POINTERS ARE ELIMINATED */
extern UINT8 const lookup_screenR[256];

/** @internal Table of green component of the palette.  THIS POINTER SHOULD BE 
    REMOVED WHEN ALL GLOBAL DATA POINTERS ARE ELIMINATED */
extern UINT8 const lookup_screenG[256];

/* @internal Table of blue component of the palette.  THIS POINTER SHOULD BE 
    REMOVED WHEN ALL GLOBAL DATA POINTERS ARE ELIMINATED */
extern UINT8 const lookup_screenB[256];
  
/* @internal 8 bits to 16 bits colour depth conversion table.  THIS POINTER 
    SHOULD BE REMOVED WHEN ALL GLOBAL DATA POINTERS ARE ELIMINATED */
extern UINT16 const lookup_colourDepth8To16[256];

#ifdef __cplusplus
}
#endif

#endif /* GRLOOKUPP_H */
