#ifndef RTBANAREASP_H
#define RTBANAREASP_H

#include <routing/rtbanareas.h>
#include <innovEngine_types.h>
#include <math/gemath.h>
#include <mapdata/enroadP.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Represents the overlap of a line segment with an avoidance area.
typedef struct TGeobanOverlap {
  TPathGeobanHandle handle;     ///< the avoidance area

  /// the index of the line segment within its containing polyline, or
  /// #ENTIRE_OVERLAP if the entire polyline overlaps the avoidance
  /// area
  UINT32 shapeIndex;
  /// start of the overlap, as a proportion of the line segment
  TIntersectionPoint start;
  /// end of the overlap, as a proportion of the line segment
  TIntersectionPoint stop;
  /// proportion of the entire polyline that overlaps the avoidance area
  TIntersectionPoint proportion;
  UINT32 entryCost;             ///< copied from the avoidance area
  UINT8 traversalMultiplier;    ///< copied from the avoidance area
} TGeobanOverlap;

/// The value of \c shapeIndex for overlaps that cover an entire segment.
#define ENTIRE_OVERLAP ((UINT32)-1)

/// An array and count of avoidance area overlaps.
typedef struct TGeobanOverlapArray {
  UINT32          n;            ///< the number of overlaps in the array
  UINT32          arraySize;    ///< the number of overlaps the array can hold
  TGeobanOverlap *array;        ///< the array itself
} TGeobanOverlapArray;


// Initialise the ban area system and memory
// Must be called before any other banArea APIs are called
void banAreas_init( void );


// Retrieve the number of active geoban areas
INT32 banAreas_getNumBans( void );


// Retrieved the value of the changed flag.  This flag is used to help
// determine if a route needs to be re-calculated
BOOL8 banAreas_getHaveChanged( void );


// Set the changed flag
void banAreas_setHaveChanged( 
  BOOL8 changed );


// Re-enable all ban areas and mark them all as not having affected the
// route calculation
void banAreas_reenableAndReset( void );


// Disable any GEOBAN_FORBIDDEN type geobans that contain the specified
// point.
//
// This function is used when processing that start/end points of a route
// to ensure that we can route to and from forbidden areas if the user
// requires it.
//
// @param pos A lat long position
void banAreas_disableForbiddenAreasEnclosingPoint( 
  const TPos* pos );


TPathGeobanType validateShape( const TPos          *shape,
                               UINT32               shapeCount,
                               BOOL8                ignoreRouteDisabled,
                               const TRect         *bbox,
                               TGeobanOverlapArray *overlaps );

TPathGeobanType validateSegment( TRoadSegment        *segment,
                                 BOOL8                ignoreRouteDisabled,
                                 TGeobanOverlapArray *overlaps );


#ifdef __cplusplus
}
#endif

#endif // RTBANAREASP_H
