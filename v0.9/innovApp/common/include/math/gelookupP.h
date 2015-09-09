#ifndef GELOOKUPP_H
#define GELOOKUPP_H

/* Lookup tables used by gemath.c */

#include <math/gelookup.h>
#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern INT32 const lookup_exp[201];

extern INT16 const lookup_cos[256];

extern INT16 const lookup_sin[256];

extern UINT8 const lookup_atan[129];

#ifdef __cplusplus
}
#endif

#endif
