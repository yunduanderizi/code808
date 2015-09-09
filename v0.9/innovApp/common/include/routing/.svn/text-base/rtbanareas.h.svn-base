//
// Declaration of routing related geoban functions
//
// $Id: rtbanareas.h 9088 2007-12-07 02:15:28Z rohit.grover $
//

#ifndef RTBANAREAS_H
#define RTBANAREAS_H

#include <innovEngine_types.h>
#include <math/gerect.h>
#include <routing/rtbans.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @{

/// @addtogroup GeobanAreas
/// @ingroup Routing
///
/// @brief Polygonal areas that are avoided by calculated routes.
///
/// An avoidance area is an area of the map that routing calculations
/// will try (with configurable amounts of effort) to avoid. The
/// boundary of an avoidance area is a polygon with up to
/// #MAX_GEOBAN_POLYGON_VERTICES vertices.  The avoided area is either
/// that inside the polygon (#REGION_INSIDE) or that outside the
/// polygon (#REGION_OUTSIDE).  A REGION_INSIDE is sometimes called a
/// "geoban" while a REGION_OUTSIDE area is sometimes called a
/// "geofence".
///
/// Up to #MAX_GEOBAN_AREAS avoidance areas can be added.
///
/// Avoidance areas be either #GEOBAN_EXPENSIVE or #GEOBAN_FORBIDDEN.
/// Each expensive avoidance area has a particular combination of
/// entry and traversal costs that is specified when the area is
/// created.  Forbidden avoidance areas will never be routed through
/// (unless the route starts or ends in the area).
///
/// To add an avoidance area, first set up a #TAvoidanceArea object
/// with the desired parameters and then add it using #banAreas_add.
///
/// @section Geofences Geofence notes
///
/// Note that calculating a route that begins or ends outside a
/// REGION_OUTSIDE avoidance area is not fully supported.  The
/// calculation will succeed and produce a plausible route, but its
/// quality may be lower than a route calculated without the avoidance
/// area defined.  This caveat applies only to geofences with a
/// non-zero traversal cost.
///
/// In particular, it is not guaranteed that a route will detour to
/// pass through a geofence that it doesn't start in.
///
/// @section Example
///
/// @code
/// TAvoidanceArea    area;
/// UINT32            n;
/// TPos              vertices[10];
/// TPathGeobanHandle handle;
///
/// avoidanceArea_init(&area);
/// area.avoidedRegion = REGION_OUTSIDE;
/// area.geobanType    = GEOBAN_EXPENSIVE;
/// area.entryCost     = ENTRY_COST_MEDIUM;
/// area.traversalCost = TRAVERSAL_COST_MEDIUM;
/// avoidanceArea_definePolygon(&area, n, vertices);
/// handle = banAreas_add(&area);
/// @endcode
///
/// @{

/// The maximum number of avoidance areas.
#define MAX_GEOBAN_AREAS 50

/// @brief The maximum number of vertices than can define a polygonal
///        avoidance area.
#define MAX_GEOBAN_POLYGON_VERTICES 32

/// @brief A handle for a geoban area.
///
/// Values != 0 are valid, whereas a value of 0 is invalid.  The
/// handles are arbitrary, it cannot be assumed that they start at 1
/// and increase sequentially although in some implentations or
/// circumstances they may.  Handles may be recycled as objects are
/// deleted and added.
typedef UINTP TPathGeobanHandle;

/// @brief Entry costs for avoidance areas.
///
/// Any integer values between #ENTRY_COST_ZERO and #ENTRY_COST_HIGH
/// can be used.  The values of the members of this enumeration
/// provide examples.
typedef enum {
  ENTRY_COST_ZERO   = 0,        ///< no entry cost
  ENTRY_COST_LOW    = 1000,     ///< low entry cost
  ENTRY_COST_MEDIUM = 10000,    ///< medium entry cost
  ENTRY_COST_HIGH   = 100000,   ///< high entry cost
  ENTRY_COST_MAX    = 1000000   ///< maximum entry cost
} TAvoidanceAreaEntryCost;

/// @brief Traversal costs for avoidance areas.
///
/// The traversal cost is a multiplier and must be >=
/// #TRAVERSAL_COST_ZERO.  Furthermore, it is a fixed binary point
/// quantity with two bits of fraction.  To convert an integer to a
/// traversal cost, shift it left by two bits.
typedef enum {
  TRAVERSAL_COST_ZERO   =  1 << 2, ///< no traversal cost
  TRAVERSAL_COST_LOW    =  4 << 2, ///< low traversal cost
  TRAVERSAL_COST_MEDIUM =  8 << 2, ///< medium traversal cost
  TRAVERSAL_COST_HIGH   = 16 << 2, ///< high traversal cost
  TRAVERSAL_COST_MAX    = 255      ///< maximum traversal cost
} TAvoidanceAreaTraversalCost;

/// @brief Available avoidance area styles (ban or fence).
typedef enum {
  REGION_INSIDE,                ///< avoid the inside of the area (geoban)
  REGION_OUTSIDE                ///< avoid the outside of the area (geofence)
} TAvoidedRegion;

/// @brief The definition of an avoidance area.
typedef struct TAvoidanceArea {
  /// #GEOBAN_EXPENSIVE or #GEOBAN_FORBIDDEN
  TPathGeobanType             geobanType;
  /// the cost applied on entering the avoidance area
  TAvoidanceAreaEntryCost     entryCost;
  /// the cost multiplier for travelling within the avoidance area
  TAvoidanceAreaTraversalCost traversalCost;
  /// whether the avoidance area is inside or outside of the polygon
  TAvoidedRegion              avoidedRegion;
  /// the number of vertices defining the avoidance area
  UINT32                      numVertices;
  /// the map coordinates of the vertices defining the acoidance area
  TPos                        vertices[MAX_GEOBAN_POLYGON_VERTICES];
} TAvoidanceArea;

/*----------------------------------------------------------------*/
/* AVOIDANCE APIs */
/*----------------------------------------------------------------*/
//## In general I think this API is inconsistent.  The two avoidance types
//## should present similar APIs.  When the ban manager is introduced,
//## these APIs will need to change so that the different avoidance types
//## present a united API
//##
//## Also, this API currently presents no `this' objects.  This may
//## change as we move to a more object based approach


/// @brief Returns a handle of the first avoidance area.
///
/// Subsequent calls to banAreas_getNext() can be used to find all
/// avoidance areas.
///
/// @return TPathGeobanHandle of the first geoban area, or 0 if none exists
extern MAPCORE_API
TPathGeobanHandle banAreas_getFirst( void );


/// @brief Returns a handle of the next avoidance area.
///
/// Used in conjunction with banAreas_getFirst() to scan through the
/// list of avoidance areas.
///
/// @param  current Handle of a ban area to use as a basis for `get next'
/// @return Handle > 0 for the next geoban area if that area exists,
///         otherwise returns 0.
extern MAPCORE_API
TPathGeobanHandle banAreas_getNext(
  TPathGeobanHandle current );


/// @brief Find an avoidance area that contains a point.
///
/// There might be zero, one or more avoidance areas that contain the
/// point; at most one is returned by this function.  If there is more
/// than one area that contains the point, the choice of which is
/// returned is arbitrary.
///
/// Note that #REGION_OUTSIDE avoidance areas are treated the same as
/// #REGION_INSIDE areas by this functions, i.e. both are considered
/// to contain a point if the point is inside the area.
///
/// @param[in] mapPos the map coordinates at which to find an
///                   avoidance area
///
/// @return the handle of an avoidance area that contains the given
///         point, or 0 if there is no such area
///
extern MAPCORE_API
TPathGeobanHandle banAreas_findAtPoint(
  const TPos *mapPos );

/// @brief Find an avoidance area that affects a point.
///
/// This is almost the same as #banAreas_findAtPoint, but instead of
/// finding an area that contains the point, it finds an area that
/// affects the point.  The difference is only evident for
/// #REGION_OUTSIDE areas, which this function will return if the
/// point is \em outside the area.
///
/// @param[in] mapPos the map coordinates at which to find an
///                   avoidance area
///
/// @return the handle of an avoidance area that affects the given
///         point, or 0 if there is no such area
///
extern MAPCORE_API
TPathGeobanHandle banAreas_findAffectingPoint(
  const TPos *mapPos );

/// Add an avoidance area
///
/// @param  bbox The rectangular area to ban from routing. (map coordinates)
/// @param  geobanType Severity of the avoidance (banned or expensive)
/// @return A geoban handle that can be used as a reference to the geoban
///         area, or 0 if the area could not be added successfully.
extern MAPCORE_API
TPathGeobanHandle banAreas_addBan(
  const TRect* bbox,
  TPathGeobanType geobanType );

/// @brief Initialise an avoidance area object with defaults.
///
/// The avoidance area must be defined using
/// #avoidanceArea_definePolygon before it can be added.
///
/// The default values are:
///
/// <table>
/// <tr><td> geobanType    </td><td> #GEOBAN_EXPENSIVE    </td></tr>
/// <tr><td> entryCost     </td><td> #ENTRY_COST_HIGH     </td></tr>
/// <tr><td> traversalCost </td><td> #TRAVERSAL_COST_ZERO </td></tr>
/// <tr><td> avoidedRegion </td><td> #REGION_INSIDE       </td></tr>
/// <tr><td> numVertices   </td><td> 0                    </td></tr>
/// </table>
///
/// @param area the avoidance area to initialise
///
extern MAPCORE_API
void avoidanceArea_init(TAvoidanceArea *area);

/// @brief Define the area affected by an avoidance area.
///
/// If \p numVertices is greater than #MAX_GEOBAN_POLYGON_VERTICES
/// then only #MAX_GEOBAN_POLYGON_VERTICES of the supplied vertices
/// are used.
///
/// @param         area the avoidance area
/// @param  numVertices the number of vertices defining the polygon
///                     (must be between 3 and
///                     #MAX_GEOBAN_POLYGON_VERTICES, inclusive)
/// @param[in] vertices the map coordinates of each vertex defining
///                     the polygon; the contents of this array are
///                     copied into \p area
///
extern MAPCORE_API
void avoidanceArea_definePolygon(TAvoidanceArea *area,
                                 UINT32          numVertices,
                                 const TPos     *vertices);

/// @brief Add an avoidance area.
///
/// Up to #MAX_GEOBAN_AREAS avoidance areas can be added, with one
/// exception: when adding areas with a non-zero traversal cost, the
/// product of the traversal costs of all of the areas must be less
/// than 128.  This corresponds to 3 #TRAVERSAL_COST_LOW areas, 2
/// #TRAVERSAL_COST_MEDIUM areas or 1 #TRAVERSAL_COST_HIGH area.
///
/// @param[in] area the avoidance area to add; the contents of this
///                 object are copied, so it can be safely reused
///
/// @return a geoban handle that can be used to refer to the geoban
///         area, or 0 if the area could not be added successfully
///
extern MAPCORE_API
TPathGeobanHandle banAreas_add(const TAvoidanceArea *area);


/// Delete an avoidance area
///
/// @param handle Handle of the geoban area to be deleted
/// @return void
extern MAPCORE_API
void banAreas_deleteBan(
  TPathGeobanHandle handle );


/// Retrieve the bounding box and severity of an avoidance area
///
/// @param[in] handle Handle of the avoidance area to be unpacked
/// @param[out] bbox The bounding box of the avoidance area
/// @param[out] geobanType The severity of the avoidance area
/// @return void
extern MAPCORE_API
void banAreas_getBanDetail(
  TPathGeobanHandle handle,
  TRect* bbox,
  TPathGeobanType* geobanType );

/// @brief Retrieve the details of an avoidance area.
///
/// @param    handle a handle for an avoidance area
/// @param[out] area the avoidance area object to fill in
/// @param[out] bbox if not \c NULL, will receive the bounding box of
///                  the avoidance area
///
extern MAPCORE_API
void banAreas_queryArea(TPathGeobanHandle  handle,
                        TAvoidanceArea    *area,
                        TRect             *bbox);


/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif // RTBANAREAS_H
