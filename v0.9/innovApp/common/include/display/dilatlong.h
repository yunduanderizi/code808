#ifndef DILATLONG_H
#define DILATLONG_H

#include <platform_types.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif


/// @addtogroup LatLongGrid
/// @ingroup Display
/// @{

//
// Step of lat/long grid in X and Y (map coordinates)
// If step is <= 0  then lat/long grid is disabled
//

///
/// @internal
/// @typedef struct TLatLongGrid TLatLongGrid
/// @brief Stepping intervals for the lat-long grid.
/// @param sx Stepping size for the x-axis.
/// @param sy Stepping size for the y-axis.
///
typedef struct TLatLongGrid
{
  INT32 sx;
  INT32 sy;
} TLatLongGrid;

///
/// @}
///


extern MAPCORE_API
BOOL8 LatLongGrid_Init(TLatLongGrid *pThis);

extern MAPCORE_API
void  LatLongGrid_Render(TLatLongGrid *pThis, struct TViewport *viewport);

extern MAPCORE_API
void  LatLongGrid_SetStep(TLatLongGrid *pThis, INT32 stepX, INT32 stepY);

extern MAPCORE_API
void  LatLongGrid_GetStep(TLatLongGrid *pThis, INT32 *stepX, INT32 *stepY);

#ifdef __cplusplus
}
#endif

#endif
