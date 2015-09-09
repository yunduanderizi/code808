#ifndef _DILOOKUP_H_
#define _DILOOKUP_H_

// This module provides several lookup tables that are used by the
// display module to convert data between map coordinates and metres and
// visa versa.  The tables are automatically generated.

/*----------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#include <platform.h>

/*----------------------------------------------------------------*/

// converts from map coordinates to metres.  The calculation performed is:
// scale = lookup_forwardScale[abs(latitude) >> 16];
// metres = (latitude * scale) >> 12;
// The scaling from latitude values to metres alters with latitude - at
// higher latitudes the scale factor decreases.  It is constant for
// longitudes (use the value at latitude 0).
  extern INT32 const lookup_forwardScale[138];

// converts from metres to map coordinates.  The calculation performed is:
// scale = lookup_backwardScale[abs(latitude) >> 16];
// latitude = (metres * scale) >> 12;
// The scaling from metres to latitude alters with latitude - at
// higher latitudes the scale factor increases.  It is constant for
// longitudes (use the value at latitude 0).
  extern INT32 const lookup_backwardScale[138];

/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------*/

#endif // already included

