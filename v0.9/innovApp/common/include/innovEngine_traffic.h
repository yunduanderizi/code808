#ifndef MAPCORE_TRAFFIC_H
#define MAPCORE_TRAFFIC_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Traffic
    
    @brief This library interface provides support for the handling of
    Traffic Messaging Channel (TMC) data streams that can be used
    by the Map Core for the purposes of providing user information
    such as traffic incidents or weather.
    
    In addition to displaying this information the Map Core may also
    use this information to calculate alternate routes or guidance.
    The application can find out when traffic events are added,
    updated and deleted by polling for notifications.  It may then,
    for added and updated events, call a further procedure with
    current active journey to determine whether or not that event is
    on the current route.  It may also, for updated and deleted
    events, find out if that event had any effect on the current
    route.  It is then the application's responsibility to decide
    whether or not to re-route.

    @ref traffic_changes_132

    @ref traffic_changes_130

    @ref traffic_changes_120

    @ref traffic_changes_119
*/
/** @{ */

#include <traffic/trapi.h>
#include <traffic/trdeprecated.h>

/** @} */

/** @page traffic_changes_132 Traffic API changes from 1.30 to 1.32

This page summarises the changes to the core traffic API between core
versions 1.30 and 1.32.

@section Added

- #TAppTrafficStationList.encrypted
- #traffic_addAuthorisedService

*/

/** @page traffic_changes_130 Traffic API changes from 1.20 to 1.30

This page summarises the changes to the core traffic API between core
versions 1.20 and 1.30.

@section Removed

- \c TAppTrafficEvent.eventLocationText

@section Deprecated

- #TAppTrafficEvent.eventAge
- #traffic_processChar
- #traffic_getTrafficMessageType
- #traffic_registerLocationFiles

@section Changed

The following struct fields are now UTF-16 rather than Latin-1:

- #TAppTrafficEvent.eventDescription
- #TAppTrafficEvent.eventExtraInfo
- #TAppTrafficEvent.eventRouteName
- #TAppTrafficEvent.eventRouteText
- #TAppTrafficEvent.trafficServiceProviderName
- #TAppTrafficStationList.stationName

The function #traffic_getSortedEventList takes an extra parameter (\p
numOnJourney).

@section Added

- #TAppTrafficEvent.ageSeconds
- #TAppTrafficEvent.eventLocationFromText
- #TAppTrafficEvent.eventLocationToText
- #TAppTrafficEvent.eventIsBidirectional
- #traffic_bandScan
- #traffic_lockToStation
- #traffic_setBitmapFile

*/

/** @page traffic_changes_120 Traffic API changes from 1.19 to 1.20

This page summarises the changes to the core traffic API between core
versions 1.19 and 1.20.

@section Removed

No API removals.

@section Deprecated

No additonal deprecations.

@section Changed

- #TRAFFIC_EVENT_TEXT_LENGTH increased from 128 to 200
- #TRAFFIC_EVENT_TEXT_STRINGS increased from 3 to 5

@section Added

- #TAppTrafficEvent.boundingBox
- #traffic_hasEventJustLeftJourney

 */

/** @page traffic_changes_119 Traffic API changes from 1.18 to 1.19

This page summarises the changes to the core traffic API between core
versions 1.18 and 1.19.

@section Removed

The functions \c traffic_isEventOnCurrentJourney and \c
traffic_didEventAffectJourney have been removed because they were
never used and are superseded by #traffic_getEventDetailForJourney.

@section Deprecated

The following functions have been deprecated.  See the documentation
for each function for details of its replacement or browse the \ref
deprecated "deprecated list".

- #TAppTrafficCallback
- #TAppTrafficEventAction
- #TAppTrafficProcessResult
- #TTrafficEventAvoidancePolicy
- #traffic_currentRouteCancelled
- #traffic_findNearestEvent
- #traffic_getEventAvoidancePolicy
- #traffic_getEventDetail
- #traffic_getEventIcon
- #traffic_initialiseTrafficProcessing
- #traffic_isEventClassDisplayed
- #traffic_newRouteCalculationComplete
- #traffic_setEventAvoidancePolicy
- #traffic_setEventClassDisplay
- #traffic_sortEventList

@section Added

- #traffic_addAvoidedEventToJourney
- #traffic_clearEventsAvoidedByJourney
- #traffic_getEventClassIcon
- #traffic_getEventClassIconPriority
- #traffic_getEventDetailForJourney
- #traffic_getEventsAvoidedByJourney
- #traffic_getNumEventsAvoidedByJourney
- #traffic_getNotifications
- #traffic_getSortedEventList
- #traffic_isEventAvoidedByJourney
- #traffic_isEventInBubble
- #traffic_removeAvoidedEventFromJourney
- #traffic_repeatNotification
- #traffic_resetNotifications
- #traffic_start

Although the following are not strictly part of the traffic API, they
have been added to support traffic functionality.

- #IconDisplayRank_GetPOIDetails
- #IconDisplayRank_GetTrafficDetails
- #Viewport_IsTrafficEventClassDisplayed
- #Viewport_SelectIcon
- #Viewport_SetTrafficEventClassDisplay
- #journey_clone

*/

#ifdef __cplusplus
}
#endif

#endif

/* End of mapcore_traffic.h */
