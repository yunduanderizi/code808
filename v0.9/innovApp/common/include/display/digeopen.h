#ifndef DIGEOPEN_H
#define DIGEOPEN_H

#include <graphics/grpen.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Display
/// @{

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

/// @addtogroup GeoPen
/// @ingroup UnUsed
/// @{

///
/// Defines a geometric pen.
/// A geometric pen is a pen that works in map units.
/// You can make use of geometric pens in conjunction with
/// Viewport_MoveTo, Viewport_LineTo, Viewport_GetPen and Viewport_SetPen.
/// These functions work with the geometric pen that is currently
/// active on the viewport.
/// Viewport_LineTo and Viewport_MoveTo are designed so that calling code
/// never has to deal with viewport-specific (typically, screen) coordinates
/// and work with map coordinates in all circumstances.
/// Because MapCore has a very special 2D viewport that explicitly specifies well-known
/// thickness values in pixels for specific road types, the 'thickness' field
/// in the TPen object can be used with viewports that expect thickness values
/// in pixels.
///
typedef struct TGeometricPen
{
  /// The rasterization pen that defines most component of the geometric pen
  TPen pen;
  /// The thickness of the geometric pen in map units
  UINT8 mapThickness;
} TGeometricPen;

/// @}

/// @}

/// @}

extern MAPCORE_API
void GeoPen_New(TGeometricPen *pThis, TPen *pen, UINT8 mapThickness);

#ifdef __cplusplus
}
#endif

#endif
