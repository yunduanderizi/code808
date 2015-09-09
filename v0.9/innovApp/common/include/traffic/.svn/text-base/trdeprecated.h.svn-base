#ifndef TRDEPRECATED_H
#define TRDEPRECATED_H

#include <innovEngine_types.h>
#include <traffic/trapi.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup Traffic_deprecated Traffic (deprecated)
 * @ingroup Deprecated
 * @brief Deprecated Traffic API
 *
 * Please use the functions described in \ref Traffic instead of those
 * described below.
 */
/** @{ start of group */

#define TRAFFIC_APP_RENOTIFICATION_INTERVAL 1

/// @brief Application callback return value
///
/// @deprecated Use #traffic_getNotifications instead of callbacks
typedef enum
{
  TRAFFIC_EVENT_PROCESSED = 0, ///< no further notifications for this event
  TRAFFIC_EVENT_RENOTIFY       ///< application will be notified again
} TAppTrafficProcessResult;

/**
 * @brief Event actions about which the application can be notified.
 *
 * @deprecated Use #traffic_getNotifications instead of callbacks
 */
typedef enum {
  TRAFFIC_EVENT_ADDED,            ///< the event has been newly added
  TRAFFIC_EVENT_UPDATED,          ///< the event has been modified
  TRAFFIC_EVENT_DELETED,          ///< the event has been deleted
  TRAFFIC_MULTIPLE_EVENTS_DELETED ///< multiple events have been deleted
} TAppTrafficEventAction;

/**
 * @brief Application event callback definition.
 *
 * \p eventID is the unique identifier of the event, unless \p action
 * is #TRAFFIC_MULTIPLE_EVENTS_DELETED, in which case it is
 * meaningless.
 *
 * \p action is the reason for the notification, i.e. whether the
 * event is new, changed or deleted.
 *
 * In the case of new and modified events, further information about
 * the event can be obtained by calling
 * #traffic_isEventOnCurrentJourney.
 *
 * @deprecated Use #traffic_getNotifications instead of callbacks
 */
typedef TAppTrafficProcessResult TAppTrafficCallback(
  UINT16 eventID, TAppTrafficEventAction action);

/**
 * @brief Avoidance policies that can be set for individual events.
 *
 * An event's avoidance policy determines how it affects path
 * calculation during routing.
 *
 * The default policy, \c TRAFFIC_EVENT_AVOIDANCE_POLICY_DEFAULT,
 * indicates that the event should be judged on its merits, taking
 * into account its class and any policy set for that class,
 * heuristics, and information in the message containing the event.
 *
 * The other policies are to unconditionally avoid the event, i.e. to
 * ensure that a calculated path will never pass through it, and to
 * ignore the event, i.e. to cause a calculated path to not be
 * affected by the event, specified by \c
 * TRAFFIC_EVENT_AVOIDANCE_POLICY_AVOID and \c
 * TRAFFIC_EVENT_AVOIDANCE_POLICY_IGNORE, respectively.
 *
 * @deprecated Events are now avoided or not on an individual journey
 *             basis.  Use the #traffic_addAvoidedEventToJourney
 *             function and its friends.
 */
typedef enum {
  /// the event will affect routing as usual, based on its class etc.
  TRAFFIC_EVENT_AVOIDANCE_POLICY_DEFAULT = 0,
  /// the event will be avoided unconditionally
  TRAFFIC_EVENT_AVOIDANCE_POLICY_AVOID,
  /// the event will not affect routing
  TRAFFIC_EVENT_AVOIDANCE_POLICY_IGNORE
} TTrafficEventAvoidancePolicy;

#ifndef NO_DEPRECATED_ROUTING
/**
 * @brief Updates the list of traffic location files
 *
 * This causes the traffic subsystem to refresh its knowledge of which
 * location files are available.  It should be called before
 * #traffic_start.  An appropriate time would be during map
 * loading.
 *
 * The available files will be queried using the os_mapName()
 * function.
 *
 * @return the number of traffic files successfully registered
 *
 * @deprecated Use mapfile_load()
 */
DEPRECATED(
extern MAPCORE_API
UINT32 traffic_registerLocationFiles(void)
);
#endif // NO_DEPRECATED_ROUTING

/**
 * @brief Traffic initialisation from Application.
 *
 * The following sets up the application callback and event storage. The
 * application is responsible for allocating storage for the event
 * descriptor. This WILL NOT immediately result in traffic events, as
 * the system needs to obtain map lock prior to initial tuning of the FM
 * receiver 
 *
 * @param[in] cb     Pointer to the Application callback processor
 * @param[in] writer Pointer to the function that will write
 *                   characters to the RDS receiver
 * @return    non-zero indicates success
 *
 * @deprecated Use #traffic_start and then #traffic_getNotifications
 *             instead of callbacks
 */
DEPRECATED(
extern MAPCORE_API
UINT32 traffic_initialiseTrafficProcessing(TAppTrafficCallback *cb,
                                           TAppTrafficWriteReceiver *writer,
                                           UINT8 *appPath)
);

/**
 * @brief Traffic initialisation from Application.
 *
 * The following tells the traffic subsystem whether or not to display
 * an event class as icons on the map screen.
 *
 * @note This only affects icon display, not event notification or
 *       routing.
 *
 * @param evclass  the event class to enable or disable display of
 * @param enable \c TRUE to enable display of the event class or \c
 *               FALSE to disable it
 *
 * @deprecated Use #Viewport_SetTrafficEventClassDisplay
 */
DEPRECATED(
extern MAPCORE_API
void traffic_setEventClassDisplay(TAppTrafficEventClass evclass, BOOL8 enable)
);

/**
 * @brief Returns whether an event class is displayed.
 *
 * @param evclass the event class to report on
 * @return \c TRUE if the event class is displayed
 *
 * @deprecated Use #Viewport_IsTrafficEventClassDisplayed
 */
DEPRECATED(
extern MAPCORE_API
BOOL8 traffic_isEventClassDisplayed(TAppTrafficEventClass evclass)
);

/**
 * @brief Sort the current event list
 * @param      arraySize size of the Application provided UINT16 array
 * @param      indexArray Pointer to the above array
 * @param      sortOrder  pre-defined sort order  
 *
 * @return event count
 *
 * Will sort current eventID's (max arraySize)and place the sorted list in the array
 *
 * @deprecated Use #traffic_getSortedEventList
 */
DEPRECATED(
extern MAPCORE_API
UINT16 traffic_sortEventList (UINT16 arraySize, 
                              UINT16 *indexArray,
                              TTrafficEventSortOrder sortOrder)
);

/// @brief Retrieve details of one of the current active events.
///
/// The journey-related fields of the supplied event structure are
/// filled in using the deprecated default journey.
///
/// @param      eventId the identifier of the event
/// @param[out] ev      a pointer to the event detail structure to
///                     fill in
///
/// @return \c TRUE if event details returned, \c FALSE if no event
///
/// @deprecated Use #traffic_getEventDetailForJourney
DEPRECATED(
extern MAPCORE_API
BOOL8 traffic_getEventDetail(TTrafficEventId eventId, TAppTrafficEvent *ev)
);

/**
 * @brief New Route Calculation Completed
 *
 * @deprecated Call #traffic_getNotifications to get pending
 *             notifications
 */
DEPRECATED(
extern MAPCORE_API
void traffic_newRouteCalculationComplete (void)
);

/**
 * @brief Current Route Cancelled
 *
 * @deprecated Use #traffic_resetNotifications
 */
DEPRECATED(
extern MAPCORE_API
void traffic_currentRouteCancelled (void)
);

/**
 * @brief Retrieve the icon associated with an event class.
 *
 * This will fill the memory pointed at by bm with the bitmap, not
 * return a pointer, as the bitmaps are not cached.
 *
 * If this function returns \c FALSE then either there is no icon for
 * the specified event class or not enough storage has been provided for
 * the icon.  These two cases can be differentiated by looking at what
 * happens to the variabled pointed to by \p dataSize.  If there is
 * simply no icon available, \c *dataSize is not changed.  If there is
 * no enough storage for the icon, \c *dataSize is set to the amount of
 * storage required.
 *
 * @param         evclass  the event class as returned in
 *                         TAppTrafficEvent structure
 * @param[out]    bm       pointer to a TBitmap structure to fill.  Must
 *                         have enough space for the entire bitmap.
 * @param[in,out] dataSize pointer to a variable that initially contains
 *                         the amount of storage pointed to by \c
 *                         bm->data and is updated to the amount
 *                         required if this is not sufficient
 * @return        \c TRUE if the icon has been filled in, \c FALSE if
 *                there is no icon for the specified event class or the
 *                icon requires more than \p *dataSize bytes
 *
 * @deprecated Use #traffic_getEventClassIcon and pass \c FALSE for
 *             the \p detour parameter
 */
DEPRECATED(
extern MAPCORE_API
BOOL8 traffic_getEventIcon (TAppTrafficEventClass evclass, BOOL8 large, TBitmap *bm,
                            UINT32 *dataSize)
);

/**
 * @brief Sets the avoidance policy for a specific event.
 *
 * @param eventID the ID of the event to set avoidance for
 * @param policy  the desired avoidance policy
 * @return \c TRUE if the event's avoidance policy was set or \c FALSE
 *         if there was a problem setting it, e.g. if the event no
 *         longer exists
 *
 * @deprecated Use #traffic_addAvoidedEventToJourney et. al.
 */
DEPRECATED(
extern MAPCORE_API
BOOL8 traffic_setEventAvoidancePolicy(UINT16 eventID,
                                      TTrafficEventAvoidancePolicy policy)
);

/**
 * @brief Reports the avoidance policy for a specifc event.
 *
 * @param eventID the ID of the event
 * @return the event's avoidance policy
 *
 * @deprecated Use #traffic_addAvoidedEventToJourney et. al.
 */
DEPRECATED(
extern MAPCORE_API
TTrafficEventAvoidancePolicy traffic_getEventAvoidancePolicy(UINT16 eventID)
);

/**
 * @brief Retrieves information about the traffic event that is nearest
 *        to a map position.
 *
 * If there is no event sufficiently close (within a 92680 map unit
 * square) to the specified position then no information is retrieved
 * and \c FALSE is returned.
 *
 * @param[out] ev a pointer to a structure to hold information about the
 *                located event
 * @param[in]  mapPos a pointer to the map coordinates near which a
 *                    traffic event will be located
 * @return     \c TRUE if an event was found and \p ev has been
 *             populated, otherwise \c FALSE, in which case \p ev is not
 *             populated
 *
 * @deprecated Use #Viewport_SelectIcon, then
 *             #IconDisplayRank_GetTrafficDetails
 */
DEPRECATED(
extern MAPCORE_API
BOOL8 traffic_findNearestEvent(TAppTrafficEvent *ev, const TPos *mapPos)
);

/**
 * @brief Character Processing routine for Traffic serial port.
 *
 * @param character from serial port
 * @return TRUE if processing resulted in a complete traffic NMEA message
 *
 * @deprecated Use #gps_processChar
 */
DEPRECATED(
extern MAPCORE_API
BOOL8 traffic_processChar(UINT8 ch)
);

/**
 * @brief Returns type of message last processed.
 *
 * @return message type in a complete traffic NMEA message
 *
 * @deprecated Use #gps_lastMessageType and #gps_getTrafficMessageType
 */
DEPRECATED(
extern MAPCORE_API
TtmcMessageType traffic_getTrafficMessageType (void)
);


// @}

#ifdef __cplusplus
}
#endif

#endif
