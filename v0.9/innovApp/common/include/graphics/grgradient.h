#ifndef __GRGRADIENT_H__
#define __GRGRADIENT_H__

#include <innovEngine_types.h>
#include <graphics/grcontext.h>

// Fills a region with gradiated colour, based on the set colours for each vertex.
// Vertices are ordered (top-left, top-right, bottom-left, bottom-right)
// The 'accurate' parameter enables a higher quality, but much slower algorithm.
extern MAPCORE_API
void GraphContext_FillRectBlend(TGraphContext *pThis,
                                const TRect *r,
                                RGBCOLOUR vertex[4],
                                BOOL8 accurate);

#endif // __GRGRADIENT_H__
