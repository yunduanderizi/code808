#ifndef DIPRESENTATION_H
#define DIPRESENTATION_H

#include <display/diworld.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Display
/// @{

///@addtogroup UnUsed
/// @ingroup Display
/// @{

///@addtogroup Presentation
/// @ingroup UnUsed
/// @{

///
/// Specifies how the presentation should be rendered
///
typedef struct PRESENTATION_RENDERPARAMS
{
  WORLD_RENDERPARAMS renderParams;
} PRESENTATION_RENDERPARAMS;

///
/// Represents a presentation
///
typedef struct TPresentation
{
  struct TViewport *viewport;
  PRESENTATION_RENDERPARAMS presParams;
} TPresentation;

/// @}

/// @}

/// @}

extern MAPCORE_API
void Presentation_New(TPresentation *pThis);

extern MAPCORE_API
void Presentation_BeginRender(TPresentation *pThis,
                              struct TViewport *viewport,
                              PRESENTATION_RENDERPARAMS renderParams);

extern MAPCORE_API
BOOL8 Presentation_GranularRender(TPresentation *pThis);

extern MAPCORE_API
void Presentation_EndRender(TPresentation *pThis);

#ifdef __cplusplus
}
#endif

#endif
