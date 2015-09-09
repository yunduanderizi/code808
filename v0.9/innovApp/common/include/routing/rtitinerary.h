//
// Declaration of the itinerary type and functions for manipulating it
//
// $Id: rtitinerary.h 9583 2008-03-17 01:09:00Z rohit.grover $
//

#ifndef RTITINERARY_H
#define RTITINERARY_H

#include <routing/rtoptions.h>
#include <location/locmaplocation.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @{


/// @addtogroup Itinerary
/// @ingroup Routing
/// @{

/*----------------------------------------------------------------*/
/* DEFINES */
/*----------------------------------------------------------------*/
/// @def ITINERARY_MAX_DESTINATIONS
/// This is the maximum number of destinations that an itinerary can hold.
/// Attempting to add more than this many destination will cause APIs to
/// fail.
#define ITINERARY_MAX_DESTINATIONS 16
//## This define cannot be changed by the app as the core depends on this
//## being defined to the particular value above

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// @typedef UINT32 TItineraryDestNumber
/// A type for indexing into an itinerary.
typedef UINT32 TItineraryDestNumber;

/// @typedef struct TItineraryDest
/// A destination is a location on the road network.  It forms part of an
/// itinerary and will after routing be the start/end point of a journey
/// leg in a calculated journey.
typedef struct TItineraryDest {
  /** @internal Not for use by applications. */
  TMapLocation     mapLocation;
  /** @internal Not for use by applications. */
  TRoutingOptions  options;
} TItineraryDest;

/// @typedef struct TItinerary
/// An Itinerary is a list of ordered destinations.  It does not include a
/// start location.  An itinerary can be turned into a routed journey by the
/// journeyCalculator
typedef struct TItinerary {
  /** @internal Not for use by applications. */
  TItineraryDestNumber currentDestination;
  /** @internal Not for use by applications. */
  TItineraryDestNumber destinationCount;
  /** @internal Not for use by applications. */
  TItineraryDest       destination[ITINERARY_MAX_DESTINATIONS];
} TItinerary;

/*----------------------------------------------------------------*/
/* ITINERARY FUNCTIONS */
/*----------------------------------------------------------------*/

/// Completely clear a destination. Set routing options to defaults.
///
/// @param[out] pThis The destination object to clear
extern MAPCORE_API
void destination_clear(
  TItineraryDest* pThis );

extern MAPCORE_API
void destination_setRoutingOptions( 
  TItineraryDest        *pThis,
  const TRoutingOptions *options );

/// Retrieve the TMapLocation of a destination
///
/// @param[in] pThis The destination whose network loc should be retrieved
/// @param[out] loc  The retrieved location (copied)
extern MAPCORE_API
void destination_getMapLocation(
  const TItineraryDest* pThis,
  TMapLocation*         loc );

/// Get the routing options for a destination.
extern MAPCORE_API
void destination_getRoutingOptions(
  const TItineraryDest *pThis,
  TRoutingOptions *options );

/// Completely clear an itinerary
///
/// @param[in] pThis The Itinerary object to clear
extern MAPCORE_API
void itinerary_clear(
  TItinerary* pThis );

/// Make a deep copy of an itinerary
///
/// @param[in]   pThis The source itinerary
/// @param[out] target The target itinerary
extern MAPCORE_API
void itinerary_clone(
  const TItinerary* pThis,
  TItinerary *target );

/// Add a new destination the end of an itinerary
///
/// @param[in] pThis The itinerary object to add the destination to
/// @param[in] dest  The destination to append to the itinerary
/// @return - TRUE if destination could be appended to the itinerary
///         - FALSE if the itinerary was full
extern MAPCORE_API
BOOL8 itinerary_addDestination(
  TItinerary* pThis,
  TItineraryDest* dest );

/// Initialize a destination with a location and 
/// routing options 
///
/// @param[in]   pThis        The source destination
/// @param[in]   mapLocation  TMapLocation of the destination
/// @param[in]   options      Routing options
extern MAPCORE_API
void destination_init( TItineraryDest        *pThis,
                       const TMapLocation    *mapLocation,
                       const TRoutingOptions *options );

/// Insert a new destination into the itinerary
///
/// @param[in] pThis The Itinerary object to insert the destination in to
/// @param[in] insertIndex Where in the itinerary to insert the destination
/// @param[in] dest  The destination to insert
/// @return - TRUE if destination could be inserted in to the itinerary
///         - FALSE if the itinerary was full
extern MAPCORE_API
BOOL8 itinerary_insertDestination(
  TItinerary* pThis,
  TItineraryDestNumber insertIndex,
  TItineraryDest* dest );


/// Delete a single network location from an itinerary
///
/// @param[in] pThis The Itinerary object with the destination to delete
/// @param[in] deleteIndex Which destination to delete
/// @return - TRUE if destination could be deleted from the itinerary
///         - FALSE if the deleteIndex was invalid
extern MAPCORE_API
BOOL8 itinerary_deleteDestination(
  TItinerary* pThis,
  TItineraryDestNumber deleteIndex );


/// Determine how many destinations are in an itinerary
///
/// @param[in] pThis Itinerary object whose destinations should be counted
/// @return The number of desintation in the itinerary
extern MAPCORE_API
UINT32 itinerary_getDestinationCount(
  const TItinerary* pThis );


/// Get the details of a destination in the itinerary
///
/// @param[in]  pThis The Itinerary object with the destination
/// @param[in]  index Index of the destination for which to get details
/// @param[out] dest  The retrieved destination (copied)
/// @return - TRUE if destination could be retrieved from the itinerary
///         - FALSE if `index' was invalid
extern MAPCORE_API
BOOL8 itinerary_getDestination(
  const TItinerary* pThis,
  TItineraryDestNumber index,
  TItineraryDest* dest );


/// Set the index of the current destination in the itinerary.  This is the
/// destination to which the user wants to travel next.  Destinations
/// before this index will be ignored when a route is calculated from this
/// itinerary.
///
/// @param[in]  pThis The Itinerary object with the destination
/// @param[in]  index Index of the destination to treat as current
/// @return - TRUE if the current destination was set as requested
///         - FALSE if `index' was invalid
extern MAPCORE_API
BOOL8 itinerary_setCurrentDestinationIndex(
  TItinerary* pThis,
  TItineraryDestNumber index );


/// Get the index of the current destination in the itinerary.  This is the
/// destination to which the user wants to travel next.  Destinations
/// before this index will be ignored when a route is calculated from this
/// itinerary.
///
/// @param[in]  pThis The Itinerary object with the destination
/// @param[out] index The index of the current destination
/// @return - TRUE if the returned current index is sensible
///         - FALSE if there are no destinations in the itinerary
extern MAPCORE_API
BOOL8 itinerary_getCurrentDestinationIndex(
  const TItinerary* pThis,
  TItineraryDestNumber* index );

/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif // RTITINERARY_H
