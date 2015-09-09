#ifndef ENSNAILTR_H
#define ENSNAILTR_H

#include <innovEngine_types.h>
#include <locationprovider/gpparse.h>

#ifdef __cplusplus
extern "C" {
#endif


/// The different rendering styles, we current support for the snail trail.
typedef enum TSnailTrailRenderingStyle {
  /// Render samples as dots
  SNAIL_TRAIL_RENDER_DOTS,
  SNAIL_TRAIL_RENDER_LINES
} TSnailTrailRenderingStyle;


/// Clears the stored snail trail positions. This way we can start from scratch,
/// if we want to.
extern MAPCORE_API
void snailTrail_clear(void);

/// Returns the currently used render style.
extern MAPCORE_API
TSnailTrailRenderingStyle snailTrail_getRenderStyle(void);

/// Sets the render style, that should be used for the snail trail.
///
/// @param[in]  style
///   The renderstyle that should be used.
extern MAPCORE_API
void snailTrail_setRenderStyle( TSnailTrailRenderingStyle style );

/// Returns the distance (in meters) between stored snail trail samples.
extern MAPCORE_API
UINT32 snailTrail_getSampleDistance(void);

/// Sets the distance that should be used between stored snail trail samples.
///
/// @param[in]  sampleDistance
///   The distance (in meters) between snail trail samples (applies to all
///   samples after the the next stored sample).
extern MAPCORE_API
void snailTrail_setSampleDistance( UINT32 sampleDistance );


#ifdef __cplusplus
}
#endif

#endif /* ENSNAILTR_H */
