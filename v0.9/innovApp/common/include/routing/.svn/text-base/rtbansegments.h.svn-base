//
// Declaration of routing related segment ban functions
//
// $Id: rtbansegments.h 9578 2008-03-16 22:16:30Z rohit.grover $
//

#ifndef RTBANSEGMENTS_H
#define RTBANSEGMENTS_H

#include <routing/rtbans.h>
#include <routing/rtpath.h>
#include <location/locmaplocation.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @{


/// @addtogroup GeobanSegments
/// @ingroup Routing
/// @{

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// @typedef enum TDetourResult
/// Enumeration of named error results from
/// path_addSegmentGeobansForDetour().
typedef enum {
  /// The user is not locked to a route, so no detour
  DETOUR_FAILED_NOT_LOCKED_TO_ROUTE = -1,

  /// Route data could not be retrieved, so no detour
  DETOUR_FAILED_BAD_ROUTE_DATA = -2,

  /// Bad map data (or possibly programmer error) -- should not happen
  DETOUR_FAILED_INTERNAL_ERROR = -3,

  /// The Limit on the number of banned segments allowed has been reached.
  /// Perhaps too many detours without a fresh route?  The actual length of
  /// route banned is still returned in the bannedLength parameter.
  DETOUR_FAILED_MAX_SEGMENTS_BANNED = -4,

  /// All okay, The requested length of route (or up to the next
  /// destination) was banned.  The actual length of route banned is
  /// returned in the bannedLength parameter.
  /// NOTE: The actual route length banned will differ from the requested
  /// as only whole segments are banned, we never ban the segment the car
  /// is currently on and we never ban past/over a destination.
  /// NOTE: The detour ban is not an outright ban.  It just makes travel
  /// along these roads very expensive.
  DETOUR_SUCCEEDED = 0

} TDetourResult;

/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
/// Ban the segment(s) at the specified network location.
/// 
/// Both the layer 0 and layer 1 segments corresponding to this network
/// locations will be avoided with the specified ban severity
/// 
/// @param  location Point on segment of road network
/// @param  banType Ban severity GEOBAN_FORBIDDEN or GEOBAN_EXPENSIVE
/// @return TRUE if any segments are banned, FALSE otherwise
extern MAPCORE_API
BOOL8 banSegments_addMapLocation( 
  TMapLocation* location,
  TPathGeobanType banType );


/// Examine upcoming segments in a path and add them the the list of banned
/// segments so that next time a re-route occurs it will detour around
/// these segments
///
/// Note the the segments aren't banned outright, just made very expensive
/// so that the route will avoid travelling over them if another way can be
/// found.
///
/// Also note that this function doesn't actually perform the route
/// calculation for the detour, it just bans segment in preparation.
///
/// @param path Path for which some segements should be detoured around
/// @param baseSegmentIndex Base index into the path from which to
///   consider segments for the detour. (Usually the current car position)
/// @param distFromBaseIndexToDetourStart Start avoiding segments this many
///   <b>metres</b> down the segment list from the base index
/// @param detourLength The length of segments to avoid in <b>metres</b>
/// @param actualLengthBanned (output, optional) The actual length of
///   segments banned in metres. (NULL is okay.)
/// @return A return code of enumerated type TDetourResult
///   Positive (or 0) for success negative on error.
extern MAPCORE_API
TDetourResult banSegments_addDetourSegments(
  const TPath* path,
  UINT32  baseSegmentIndex,
  UINT32  distFromBaseIndexToDetourStart,
  UINT32  detourLength,
  UINT32* actualLengthBanned );
//## This API is very difficult to use in it current form.  It really needs
//## to be wrapped up with some functions that retrieve the current path
//## and do some maplocking to identify the current position on that path.


/// This is a much easier to use version of
/// banSegments_addDetourSegments().  It adds segments for a detour ban
/// starting from the currently maplocked position on the current leg of
/// the journey maplock is currently tracking.
///
/// This is a hack API that should be in some layer above routing/maplock.
/// It adds a dependency in routing on maplock YUCK!!!
///
/// @param  detourLength Preferred length of detour from current position
/// @return A return code of enumerated type TDetourResult
///         Positive (or 0) for success negative on error.
extern MAPCORE_API
TDetourResult banSegments_addForImmediateDetour(
  UINT32 detourLength );


/// Clear all GEOBAN_DETOUR avoided segments (sections of road).  These are
/// the avoided segments that have been added by
/// banSegments_addSegmentsForDetour().
/// 
/// This function should be called when the ban segments from detours
/// should no longer affect route calculations.
extern MAPCORE_API
void banSegments_clearDetourBans( void );


/// Un-ban all avoided segments (sections of road).  This includes all
/// segments avoided by calling the banSegments_addBanAtPoint() and
/// banSegments_addSegmentsForDetour().
/// 
/// This function does <b>not</b> clear geoban areas, just segments.
extern MAPCORE_API
void banSegments_clearAllBans( void );


/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif // RTBANSEGMENTS_H
