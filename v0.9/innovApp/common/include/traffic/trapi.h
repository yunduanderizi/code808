#ifndef TRAPI_H
#define TRAPI_H

#include <innovEngine_types.h>
#include <mapdata/enroad.h>
#include <graphics/grbitmap.h>
#include <locationprovider/gpparse.h>
#include <language/latypes.h>
#include <routing/rtjourney.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Traffic
/// @{

/**
 * @name Public Types and Constants
 * @{
 */

/// @brief a traffic event identifier
///
/// Valid event identifiers are from #TRAFFIC_MIN_EVENT_ID to
/// #TRAFFIC_MAX_EVENT_ID inclusive.
typedef UINT16 TTrafficEventId;

/// The minimum value a traffic event identifier can take
#define TRAFFIC_MIN_EVENT_ID 1

/// The maximum value a traffic event identifier can take
#define TRAFFIC_MAX_EVENT_ID UINT16_MAX

/// A value that a traffic identifier will never take
#define TRAFFIC_INVALID_EVENT_ID 0

/**
 * @brief TMC event classes
 *
 * These are the classes that are publicly visible to the application
 * The events are actually categorised into somewhere around 40 classes 
 * internally as per ISO 14189-2.
 *
 * Only events of class #TRAFFIC_CLASS_TRAFFIC indicate a traffic
 * delay.
 *
 * The values of this enumeration are in no particular order.  To
 * establish an ordering for a collection of events, the values
 * returned by #traffic_getEventClassIconPriority can be used.
 */
///@TPE
typedef enum 
{
  TRAFFIC_CLASS_TRAFFIC = 0,    ///< affects traffic
  TRAFFIC_CLASS_CLEARANCE,      ///< clearance
  TRAFFIC_CLASS_FORECAST,       ///< forecast
  TRAFFIC_CLASS_RESTRICTION,    ///< road or lane restriction
  TRAFFIC_CLASS_ROADWORKS,      ///< roadworks
  TRAFFIC_CLASS_DANGER,         ///< danger
  TRAFFIC_CLASS_WARNING,        ///< warning
  TRAFFIC_CLASS_WEATHER,        ///< weather
  TRAFFIC_CLASS_INFORMATION,    ///< information
  TRAFFIC_CLASS_TRAFFIC_TW,
  TRAFFIC_CLASS_OBSTACLE_TW,
  TRAFFIC_CLASS_JAM_TW,
  TRAFFIC_CLASS_CONTROL_TW,
  TRAFFIC_CLASS_SIGN_TW,
  TRAFFIC_CLASS_ROADWORKS_TW,
  TRAFFIC_CLASS_DISASTER_TW,
  TRAFFIC_CLASS_HIGHWAY40,
  TRAFFIC_CLASS_HIGHWAY60,
  TRAFFIC_CLASS_HIGHWAY80,
  TRAFFIC_CLASS_SYSTEM,         ///< system
  TRAFFIC_APP_NUM_EVENT_CLASSES
}TAppTrafficEventClass;


/// The maximum number of Events in the Event list
#define MAX_TRAFFIC_EVENTS  512

/// The maximum length of a string describing an event
#define TRAFFIC_EVENT_TEXT_LENGTH   200
/// The maximum number of strings describing an event
#define TRAFFIC_EVENT_TEXT_STRINGS  5 // must be <= MAX_EVENTS_PER_MSG
/// The maximum number of extra strings that can be added to an event
#define TRAFFIC_EVENT_EXTRA_STRINGS 2 // must be <= MAX_SUPPLEMENTS_PER_MSG
/// The maximum length of a station name 
#define TRAFFIC_STATION_NAME_LENGTH 20

/// Structure used to advise Application of traffic event details 
typedef struct TAppTrafficEvent
{
  TTrafficEventId  eventId;     ///< Internal (unique) traffic event identifier
  UINT16  eventCode;            ///< Primary TMC event code   
  TAppTrafficEventClass eventClass;///< Event class 
  BOOL8   cannotBeAvoided;      ///< if the event cannot be routed around (currently
                                ///< this applies to events with an extent of 0)
  BOOL8   cannotBeLocated;      ///< if the location cannot be identified on map
                                ///< This means there are NO identifiable road segments
                                ///< associated with the event (eg Area locations)

  /// Age of event in minutes since reception.
  ///
  /// @deprecated Use #ageSeconds instead for more precision.
  UINT16  eventAge;

  /// Age of event in seconds since last update.
  UINT32  ageSeconds;

  UINT16  eventLocation;        ///< Primary TMC location ID 

  /**
   * @name Journey-related fields
   *
   * These fields relate to a specific journey and are only filled in if
   * one is supplied to #traffic_getEventDetailForJourney.
   *
   * @{
   */
  BOOL8   eventOnRoute;         ///< Is event on journey?
  BOOL8   eventAffectsRoute;    ///< Is event avoided by journey?

  /// @brief Approximate length of the route affected (in metres).
  UINT32  eventExtent;

  /// @brief Approximate distance to the event (in metres) or -1 if
  ///        this cannot be calculated.
  INT32   distanceToEvent;

  /// @brief The detour recommended status of the event.
  ///
  /// This is \c TRUE if the event is inside the bubble and is either
  /// a traffic class event on the journey or a clearance class event
  /// avoided by the journey.
  BOOL8   diversionRecommended;

  //@}

  /**
   * @name Event descriptor text arrays
   *
   * There can be up to #TRAFFIC_EVENT_TEXT_STRINGS text strings used to
   * describe the event itself, and up to #TRAFFIC_EVENT_EXTRA_STRINGS
   * strings added to the event description for extra information.
   *
   * Strings that aren't defined for a particular event are set to
   * zero-length (i.e. their first character is \c '\\0').
   *
   * @{
   */
  /// Descriptions of the event(s).
  UTF16  eventDescription[TRAFFIC_EVENT_TEXT_STRINGS][TRAFFIC_EVENT_TEXT_LENGTH];
  /// Extra info about the event(s).
  UTF16  eventExtraInfo[TRAFFIC_EVENT_EXTRA_STRINGS][TRAFFIC_EVENT_TEXT_LENGTH];
  //@}

  /**
   * @name Event Location Text
   *
   * These text strings are taken directly from the Country location
   * table.  As a result, they are available only in the language of
   * the host country.
   *
   * @{
   */
  /// @brief The name of the route the event affects, e.g. "A4", or
  ///        the name of the area for events at area locations,
  ///        e.g. "Hamburg".
  UTF16  eventRouteName[TRAFFIC_EVENT_TEXT_LENGTH];

  /// @brief The name of the portion of the route
  ///        that is affected, e.g. "Auckland > Hamilton".
  ///
  /// This is empty for the USA.
  UTF16  eventRouteText[TRAFFIC_EVENT_TEXT_LENGTH];

  /// @brief The name of the start of the event's extent.
  ///
  /// This is the location that will be encountered first when driving
  /// into the event.  It is empty for events at area locations.
  UTF16  eventLocationFromText[TRAFFIC_EVENT_TEXT_LENGTH];

  /// @brief The name of the end of the event's extent.
  ///
  /// This is the location that will be encountered last when driving
  /// out of the event.  It is empty for events at area locations and
  /// for events with zero extent.
  UTF16  eventLocationToText[TRAFFIC_EVENT_TEXT_LENGTH];

  /// @brief Is the event bidirectional?
  ///
  /// Bidirectional events affect both directions of travel, so the
  /// distiction between #eventLocationFromText and
  /// #eventLocationToText is meaningless.
  BOOL8  eventIsBidirectional;
  //@}

  /**
   * @name Event Speech Flags
   *
   * These flags tell the application which speech prompt to use for
   * the event notification
   * 
   * @{
   */
  TLanguageTrafficEventType   eventSpeechType; 
  TLanguageTrafficSituation   eventSpeechSituation;
  //@}

  
  /**
   * @name Service Provider
   *
   * The name (as broadcast by RDS message group 1) and frequency of
   * the station that broadcast the event
   *
   * @{
   */
  /// Transmitting station name
  UTF16  trafficServiceProviderName[TRAFFIC_STATION_NAME_LENGTH];
  /// @brief Transmitting station frequency
  ///
  /// Divide by 10 to get the frequency in MHz.
  UINT16 stationFrequency;
  //@}

  TPos   eventLatLong;          ///< LAT/LONG location of Primary event   

  /// bounding box of event's extent (map units)
  TRect  boundingBox;
} TAppTrafficEvent;


/// @brief Enumeration for TMC related flag in the station list.
///
/// A station is only useable if its status is #STATION_HAS_TMC.
typedef enum
{
  STATION_UNKNOWN_TMC = 0,      ///< TMC status is unknown at present
  STATION_HAS_TMC,              ///< transmits TMC that we can use
  STATION_ENCRYPTED_TMC,        ///< transmits encrypted TMC
  STATION_NO_LOCATION_TABLE,    ///< transmits TMC but we don't have the location table
  STATION_NO_TMC,               ///< does not transmit TMC
  STATION_BAD_ODA               ///< transmits somethings that looks like TMC but isn't
} TAppStationTMCStatus;

/// Structure used to transfer TMC station list details.
typedef struct TAppTrafficStationList
{
  UINT8  maxIndex;              ///< Total Number of station entries
  UINT8  entryIndex;            ///< Index of this entry
  BOOL8  currentStation;        ///< Tuner currently tuned to this frequency
  UINT16 frequency;             ///< Frequency; divide by 10 to get MHz
  UINT16 piCode;                ///< PI Code for station
  UINT8  signalStrength;        ///< Last measured signal strength
  TAppStationTMCStatus tmcFlag; ///< Does this station transmit TMC?
  UTF16  stationName[TRAFFIC_STATION_NAME_LENGTH]; ///< The name of the station
  BOOL8  encrypted;             ///< Does the station transmit encrypted data?
} TAppTrafficStationList;

/// Traffic subsystem status 
typedef enum 
{
  TRAFFIC_SYSTEM_INACTIVE = 0,
  TRAFFIC_SYSTEM_IDLE,
  TRAFFIC_SYSTEM_SCANNING,
  TRAFFIC_SYSTEM_RECEIVING
} TAppTrafficState;

/**
 * @brief Type for function to send a character to the RDS receiver.
 *
 * The function should return \c TRUE if the character was
 * successfully written to the receiver.
 */
typedef BOOL8 TAppTrafficWriteReceiver(char c);

//@}

/**
 * @name Initialisation API
 * @{
 */

/**
 * @brief Starts the processing of traffic events
 *
 * Once this function is called, the traffic component will begin to
 * decode traffic events received from an attached RDS receiver.  New
 * events can be polled for using the
 * #traffic_getNotifications and #traffic_getEventCount functions.
 * Note that traffic files should be loaded (using #mapfile_load)
 * before calling this function.
 *
 * @param[in]  writer  the function that the traffic component will
 *                     use to write characters to the RDS receiver
 * @param[in]  appPath the path to the directory in which the
 *                     traffic string files can be found; this string
 *                     is copied
 *
 * @return \c TRUE if the traffic component was successfully
 *         initialised
 */
extern MAPCORE_API
BOOL8 traffic_start(TAppTrafficWriteReceiver *writer,
                    const char *appPath);

/// @brief Sets the file to load traffic bitmaps from.
///
/// If this function is not called, or it is called with an empty
/// string, traffic bitmaps will be loaded from the file \c
/// tmcicons.bm in the \p appPath directory passed to #traffic_start.
///
/// @param bitmapFileName the name of the file to load traffic bitmaps
///                       from; this string is copied
///
/// @return \c TRUE if the bitmap file was successfully set
///
extern MAPCORE_API
BOOL8 traffic_setBitmapFile(const char *bitmapFileName);

/**
 * @brief Traffic initialisation from Application.
 *
 * The following tells the traffic subsystem the language and distance
 * units to use.  This may be called at any time (e.g. if user is
 * changing the language selection via menu option)
 *
 * @param lang         The language 
 * @param unitsMetric  non-zero if using metric units
 */
extern MAPCORE_API
void traffic_setLanguage (TLanguageCode lang, UINT8 unitsMetric);

/**
 * @brief Sets whether to avoid events of a particular class.
 *
 * If an event class's avoidance is \c FALSE then routing will not
 * take events of that class into account when calculating paths.  If
 * it \c TRUE then events of that class \e may affect routing
 * calculations; factors such as the specific type of event, diversion
 * advice in the traffic message itself and the extent of the event
 * are also taken into account.
 *
 * @param evclass  the event class to enable or disable avoidance for
 * @param enable \c TRUE to enable avoidance for the event class or \c
 *               FALSE to disable it
 */
extern MAPCORE_API
void traffic_setEventClassAvoidance(TAppTrafficEventClass evclass, BOOL8 enable);

/**
 * @brief Reports whether avoidance is enabled for an event class.
 *
 * @param evclass the event class to report on
 * @return \c TRUE if avoidance is enabled for the event class
 */
extern MAPCORE_API
BOOL8 traffic_isEventClassAvoided(TAppTrafficEventClass evclass);

/**
 * @brief Sets the traffic bubble size in kilometers.
 *
 * No events outside this bubble will be notified to the application.
 * Note that this does not affect the event list returned by
 * #traffic_getSortedEventList.
 *
 * The default size is 50km, the maximum is 500km and the minimum is
 * 20km.
 *
 * @param bubbleSize         in kilometers
 * @return \c TRUE if successful, \c FALSE if not
 */
extern MAPCORE_API
BOOL8 traffic_setBubbleSize (UINT16 bubbleSize);

/**
 * @brief Gets the traffic bubble size in kilometers.
 *
 * @return bubble size in kilometers
 */
extern MAPCORE_API
UINT16 traffic_getBubbleSize (void);

/// @brief Adds an authorised conditional access service.
///
/// The list of authorised conditional access services is reset to its
/// default whenever #traffic_start is called, so this function must
/// be called once for each authorised service (other than the
/// defaults) every time #traffic_start is called.  Furthermore, it
/// must be called \em after #traffic_start to have any effect.
///
/// @param mcc two-letter Navman map country code, eg. US, UK or FR
/// @param cc country code (1~15) or (1~9,0xA~0xF)
/// @param ltnbe location table number
/// @param sid service provider ID
/// @param svk service key
///
/// @return \c TRUE if successful, \c FALSE if not
///
extern MAPCORE_API
BOOL8 traffic_addAuthorisedService(const TCountryCode *mcc,
                                   UINT8 cc,
                                   UINT8 ltnbe,
                                   UINT8 sid,
                                   UINT8 svk);

//@}

/**
 * @name Traffic Journey API
 * @{
 */

/// @brief Adds a traffic event to the set of events to avoid for a
///        journey.
///
/// Changing the set of avoided traffic events affects subsequent
/// routing calculations targeting this journey; the path already
/// stored in the journey is not affected.
///
/// @param[in]  journey the journey
/// @param      eventId the traffic event to avoid
///
/// @return \c TRUE if the event was added to the set of events to
///         avoid for this journey
extern MAPCORE_API
BOOL8 traffic_addAvoidedEventToJourney(const TJourney  *journey,
                                       TTrafficEventId  eventId);

/// @brief Removes a traffic event from the set of events to be
///        avoided by a journey.
///
/// Changing the set of avoided traffic events affects subsequent
/// routing calculations targeting this journey; the path already
/// stored in the journey is not affected.
///
/// @param[in]  journey the journey
/// @param      eventId the traffic event to not avoid
///
/// @return \c TRUE if the event was removed from the set of events to
///         avoid for this journey
extern MAPCORE_API
BOOL8 traffic_removeAvoidedEventFromJourney(const TJourney  *journey,
                                            TTrafficEventId  eventId);

/// @brief Clears the set of traffic events to avoid for a journey.
///
/// Changing the set of avoided traffic events affects subsequent
/// routing calculations targeting this journey; the path already
/// stored in the journey is not affected.
///
/// @param journey the journey
///
/// @return \c TRUE if the set of events to avoid for the journey was
///         cleared
extern MAPCORE_API
BOOL8 traffic_clearEventsAvoidedByJourney(const TJourney *journey);

/// @brief Tests a traffic event for membership in the set of events
///        to avoid for a journey.
///
/// @param journey the journey
/// @param eventId the traffic event to test for
///
/// @return \c TRUE if the event is a member of the set of events to
///         avoid for this journey
extern MAPCORE_API
BOOL8 traffic_isEventAvoidedByJourney(const TJourney  *journey,
                                      TTrafficEventId  eventId);

/// @brief Fetches the traffic events that are to be avoided by a
///        journey.
///
/// @param[in]  journey the journey
/// @param[out]  events a pointer to an array to hold the returned
///                     event identifiers
/// @param    maxEvents the maximum number of event ids that can be
///                     stored in \p events
///
/// @return the number of events stored in \p events
extern MAPCORE_API
UINT32 traffic_getEventsAvoidedByJourney(const TJourney  *journey,
                                         TTrafficEventId *events,
                                         UINT32           maxEvents);

/// @brief Counts the traffic events that are to be avoided by a
///        journey.
///
/// @param[in] journey the journey
///
/// @return the number of events avoided by the journey
extern MAPCORE_API
UINT32 traffic_getNumEventsAvoidedByJourney(const TJourney *journey);


/// @brief Returns the total number of traffic events that 
/// have been notified to the application that are on the journey
/// This will include events that have been avoided
///
/// @param[in] journey the journey
///
/// @return the number of events on or affecting the journey
extern MAPCORE_API
UINT32 traffic_getNumEventsOnJourney(const TJourney *journey);

#if 0
These two APIs are not needed, but we might want them in the future.

/// @brief Determines if a traffic event has ever been notified as
///        being on a journey.
///
/// A traffic event is considered to have been on a journey if
/// #traffic_getNotifications has ever been called for \p journey
/// while the event is inside the bubble on that journey.
///
/// @note This \b includes events that have been driven past on the
///       current leg as well as event that were on previous legs of
///       the journey.
///
/// @param journey the journey
/// @param eventId the traffic event
///
/// @return \c TRUE if the event has ever been on the journey
///
extern MAPCORE_API
BOOL8 traffic_hasEventBeenOnJourney(const TJourney *journey,
                                    TTrafficEventId eventId);

/// @brief Removes a traffic event from the set of events that have
///        been notified as being on a journey.
///
/// An event can only be forgotten if it has not been avoided; calling
/// this function for an event that is avoided by the journey will
/// fail and return \c FALSE.
///
/// A consequence of forgetting an event is that if the event is ever
/// on the journey then it will be notified as new.  Another
/// consequence is that #traffic_hasEventBeenOnJourney will return \c
/// FALSE, at least until the next call to #traffic_getNotifications.
///
/// @param journey the journey
/// @param eventId the traffic event to forget
///
/// @return \c TRUE if the event is no longer in the notified set for
///         the journey or \c FALSE if it has been marked as avoided
///         and can not be removed
///
extern MAPCORE_API
BOOL8 traffic_forgetEventWasOnJourney(const TJourney *journey,
                                      TTrafficEventId eventId);
#endif

/// @brief Determines if a traffic event transitioned from being on
///        the journey to not being on it between the two most recent
///        calls to #traffic_getNotifications.
///
/// This includes the case of the user driving past the event, but
/// there won't be a notification for that case.
///
/// @param journey the journey
/// @param eventId the traffic event
///
/// @return \c TRUE if the event was not on the route for the most
///         recent notification but was for the notification before
///         that
///
extern MAPCORE_API
BOOL8 traffic_hasEventJustLeftJourney(const TJourney *journey,
                                      TTrafficEventId eventId);

//@}

/// @name Notification API
/// @{

/// @brief The reasons for which a traffic event can be notified
typedef enum TTrafficNotificationType {
  trafficNotificationNew,       ///< the event was newly added
  trafficNotificationChanged,   ///< the event was updated
  trafficNotificationDeleted    ///< the event was deleted
} TTrafficNotificationType;

/// @brief The data structure used for each notification
typedef struct TTrafficNotification {
  TTrafficNotificationType reason;  ///< the reason for the notification
  TTrafficEventId          eventId; ///< the event that is being notified
} TTrafficNotification;

/// @brief Fetches any pending traffic event notifications.
///
/// The notifications fetched by this function are any events inside
/// the bubble that have been added, updated or deleted since the most
/// recent call to either this function or
/// #traffic_resetNotifications.  This includes events that were
/// previously outside the bubble and are now inside it.
///
/// In addition to these events, events that are on the specified
/// journey and haven't been notified as being on that journey before
/// are notified as new events.
///
/// @param[in]           journey the journey to use to decide if
///                              previously notified events need to be
///                              renotified
/// @param[in]      bubbleCentre the map position to use as the centre
///                              of the bubble, or \c NULL to use the
///                              current position according to map
///                              lock
/// @param[out]    notifications an array to store the notifications
///                              in
/// @param[in]  maxNotifications the maximum number of notifications
///                              that can be stored in the array
///
/// @return     the number of notifications stored in the array
extern MAPCORE_API
UINT32 traffic_getNotifications(const TJourney       *journey,
                                const TPos           *bubbleCentre,
                                TTrafficNotification *notifications,
                                UINT32                maxNotifications);

/// @brief Requests that a notification be repeated.
///
/// Following a call to this function, the next call to
/// #traffic_getNotifications will return a notification for the
/// specified event (as long as the event still exists and is eligible
/// for notification.)
///
/// Notifications of deleted events are not guaranteed to be
/// repeatable because their event list entries can be reclaimed at
/// any time following their notification.  If a deleted event's event
/// list entry has been reclaimed at the time this function is called
/// then \c FALSE is returned.  If \c TRUE is returned then the
/// event's entry will not be reclaimed until it is notified again.
///
/// If the core is not given an opportunity to process TMC messages
/// between the call to #traffic_getNotifications and the call to
/// #traffic_repeatNotification then no event list entries will be
/// reclaimed and this function will always succeed to schedule an
/// event for notification.
///
/// @param eventId the event to repeat a notification for
///
/// @return \c TRUE if the notification can be repeated, or \c FALSE
///         if the event's event list entry has been reclaimed and it
///         can not be notified again
extern MAPCORE_API
BOOL8 traffic_repeatNotification(TTrafficEventId eventId);

/// @brief Resets the notification status of all traffic events.
///
/// Following a call to this function, the next call to
/// #traffic_getNotifications will return notifications for all events
/// in the event list.
extern MAPCORE_API
void traffic_resetNotifications(void);

//@}

/**
 * @name Traffic Status API
 * @{
 */

/**
 * @brief Traffic subsystem status. 
 *
 * @return  Current Status 
 */
extern MAPCORE_API
TAppTrafficState traffic_getStatus (void);

/**
 * @brief RDS statistics.
 *
 * @param  *rdsCount = total number of RDS messages received last second
 * @param  *tmcCount = total number of group 8A messages last second
 */
extern MAPCORE_API
void traffic_getRDSStatistics (UINT16 *rdsCount, UINT16 *tmcCount);

//@}

/**
 * @name Event List API
 * @{
 */

/**
 * @brief Retrieve the current active event count
 *
 * @return event count
 */
extern MAPCORE_API
UINT16 traffic_getEventCount (void);

/**
 * @brief Retrieve the current maximum number of events.
 *
 * @return maximum number of events that will be stored.
 */
extern MAPCORE_API
UINT16 traffic_getMaxEventCount (void);

/**
 * @brief Set the maximum number of events to store. This cannot be set
 *        larger than MAX_TRAFFIC_EVENTS.
 *
 * @param newMax the new maximum. Minimum of 32, maximum of 512. This
 *               value may be increased as necessary to meet
 *               implementation requirements.
 *
 * @return TRUE if the maximum was successfully changed, FALSE otherwise.
 */
extern MAPCORE_API
BOOL8 traffic_setMaxEventCount (UINT16 newMax);

/**
 * @brief Sort options for event list.
 *
 */
typedef enum
{
  /// @brief An unspecified, stable order.
  ///
  /// The particular order used depends on implementation details, but
  /// it is stable, i.e. the relative ordering of any two events will
  /// not change.
  EVENT_SORT_NONE = 0,

  /// @brief Sort by last update time, newest to oldest.
  ///
  /// Events with the same last update time are sorted as for
  /// #EVENT_SORT_NONE.
  EVENT_SORT_AGE,

  /// @brief Sort by road name.
  ///
  /// Route numbers are sorted numerically, taking into account the
  /// importance of the route in some countries.  Events that do not
  /// have a route number or road name sort after events that do and
  /// are sorted alphabetically by location name.
  ///
  /// Events with exactly the same location name are sorted as for
  /// #EVENT_SORT_DISTANCE.  Events with the same location name and
  /// the same distance are sorted as for #EVENT_SORT_NONE.
  EVENT_SORT_ROADNAME,

  /// @brief Sort by distance from a position, nearest to farthest.
  ///
  /// Which position is used as the origin and how the distance is
  /// calculated depends on the arguments passed to
  /// #traffic_getSortedEventList.
  ///
  /// Events with the same distance are sorted as for
  /// #EVENT_SORT_ROADNAME.  Events with the same distance and
  /// the same location name are sorted as for #EVENT_SORT_NONE.
  EVENT_SORT_DISTANCE
} TTrafficEventSortOrder;

/// @brief Gets the identifiers of the active events.
///
/// The identifiers of active events are stored in \p eventIds in the
/// order specified by \p sortOrder, up to a maximum of \p maxEvents.
///
/// Note that if distance sorting is requested then a valid position
/// must be available, either through the \p mapPos parameter or from
/// map lock.  If no position is available then the events will be
/// sorted by road name instead.
///
/// See #traffic_getEventDetailForJourney for an example of using this
/// function.
///
/// @param     sortOrder specifies the order in which to put events
///                      into the array
/// @param[in]   journey if this is not \c NULL then only events on
///                      this journey or avoided by it will be
///                      returned; additionally, sorting by distance
///                      will use the distance along the journey
///                      rather than a straight line if the position
///                      as specified by \p mapPos is on this journey
/// @param[in]    mapPos the map position to use for sorting by
///                      distance, or \c NULL to use the current map
///                      lock position.
/// @param[out] eventIds an array into which event identifiers will be
///                      stored
/// @param     maxEvents the maximum number of event identifiers to
///                      store in the array
/// @param[out] numOnJourney a pointer into which to store the number
///                          of events in the returned list that are
///                          actually on the supplied journey, or \c
///                          NULL if this information is not wanted
///
/// @return the number of event identifiers stored in \p eventIds
extern MAPCORE_API
UINT16 traffic_getSortedEventList(TTrafficEventSortOrder  sortOrder,
                                  const TJourney         *journey,
                                  const TPos             *mapPos,
                                  TTrafficEventId        *eventIds,
                                  UINT16                  maxEvents,
                                  UINT16                 *numOnJourney);

/// @brief Retrieve details of one of the current active events.
///
/// The journey-related fields of the supplied event structure are
/// only filled in if a non-NULL journey pointer is supplied.
///
/// @par Example:
/// @code
///  TTrafficEventId eventArray[300];
///  UINT16 eventCount;
///  TAppTrafficEvent ev;
///
///  eventCount = traffic_getSortedEventList(EVENT_SORT_ROADNAME, eventArray, 300);
///  for (int count = 0; count < eventCount; count++)
///  {
///     if (traffic_getEventDetailForJourney(eventArray[count],
///                                          activeJourney, NULL, &ev))
///     {
///       // Format and display the event detail
///     }
///  }
/// @endcode
///
/// @param      eventId the identifier of the event
/// @param[in]  journey the journey to use for filling in the
///                     journey-related fields of the event structure,
///                     or \c NULL to leave these fields unpopulated
/// @param[in]   mapPos only used if \p journey is not \c NULL: the
///                     map position to use when calculating the
///                     distance to the event along the journey, or \c
///                     NULL to use the current map lock position
/// @param[out]      ev a pointer to the event detail structure to
///                     fill in
///
/// @return \c TRUE if event details returned, \c FALSE if no event
extern MAPCORE_API
BOOL8 traffic_getEventDetailForJourney(TTrafficEventId   eventId,
                                       const TJourney   *journey,
                                       const TPos       *mapPos,
                                       TAppTrafficEvent *ev);

/// @brief Tests an event for inclusion in the bubble.
///
/// An event's bubble-status can not be determined if any of the
/// following is true:
///
/// \li there is no event corresponding to the supplied event
///     identifier
/// \li no bubble centre is supplied and there is no current GPS
///     position
///
/// @param    eventId the identifier of the event
/// @param[in] mapPos the centre of the bubble, or \c NULL to use the
///                   current GPS position
///
/// @return \c TRUE if the event is inside the bubble or \c FALSE if
///         the event is not inside the bubble or its bubble-status
///         cannot be determined
///
extern MAPCORE_API
BOOL8 traffic_isEventInBubble(TTrafficEventId  eventId,
                              const TPos      *mapPos);

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
 * @param          evclass the event class as returned in
 *                         TAppTrafficEvent structure
 * @param            large \c TRUE => large icon, \c FALSE => small icon
 * @param           detour \c TRUE => icon adorned with
 *                         detour-recommended symbol, \c FALSE =>
 *                         unadorned icon
 * @param[out]          bm pointer to a TBitmap structure to fill.  Must
 *                         have enough space for the entire bitmap.
 * @param[in,out] dataSize pointer to a variable that initially contains
 *                         the amount of storage pointed to by \c
 *                         bm->data and is updated to the amount
 *                         required if this is not sufficient
 * @return        \c TRUE if the icon has been filled in, \c FALSE if
 *                there is no icon for the specified event class or the
 *                icon requires more than \p *dataSize bytes
 */
extern MAPCORE_API
BOOL8 traffic_getEventClassIcon(TAppTrafficEventClass evclass,
                                BOOL8 large,
                                BOOL8 detour,
                                TBitmap *bm,
                                UINT32 *dataSize);

/// @brief Retreives the priority used when displaying an icon of the
///        given class.
///
/// Event class icons with lower priority values are displayed in
/// preference to those with higher values if they overlap.  This
/// ordering can be used by clients of this API to resolve similar
/// conflicts involving the display of multiple events at one
/// location.
///
/// @param eventClass the event class
///
/// @return the display priority of the event class; lower numbers
///         display in preference to higher numbers
///
extern MAPCORE_API
UINT32 traffic_getEventClassIconPriority(TAppTrafficEventClass eventClass);

//@}

/**
 * @name Tuner API
 * @{
 */

/**
 * @brief Retrieve the count of stations in the current list
 *
 * @return number of stations in list
 */
extern MAPCORE_API
UINT8 traffic_getStationListCount (void);

/**
 * @brief Retrieve the Details of a specific station List entry
 *
 * @param index  Entry to retrieve
 * @param station A pointer to the structure that will receive the details
 *
 * @return TRUE if station entry exists, FALSE if no entry for that index
 */

extern MAPCORE_API
BOOL8 traffic_getStationListEntry ( UINT8 index, TAppTrafficStationList *station);

/**
 * @brief Force a tune to a specific station
 *
 * @param station A pointer to the structure that contains details of the station
 *                to tune to
 *
 * @return TRUE if station entry exists, FALSE if no entry for that index
 */

extern MAPCORE_API
BOOL8 traffic_tuneToStation (TAppTrafficStationList *station);

/// @brief Tunes to a specific station and prevents tuning away from
///        it.
///
/// To restore normal tuning behaviour, call #traffic_scanTune.
///
/// @param station the details of the station to tune to, as returned
///                by #traffic_getStationListEntry
///
/// @return \c TRUE if the station can be tuned to, or \c FALSE if it
///         no longer exists in the station list
///
extern MAPCORE_API
BOOL8 traffic_lockToStation (TAppTrafficStationList *station);

/**
 * @brief Trigger an automatic re-tune. The system will tune to
 *        the strongest signal
 *
 * @return TRUE if successful, FALSE if not
 */

extern MAPCORE_API
BOOL8 traffic_scanTune (void);

/**
 * @brief Set preference for station
 *
 * @return TRUE if successful, FALSE if not
 */

extern MAPCORE_API
BOOL8 traffic_setPreferredStation (UINT16 picode);


/// @brief Initiates a full band scan, discarding any existing station
///        list.
///
/// Immediately following a call to the function, the station list
/// will contain zero entries and will remain empty until the band
/// scan is complete.
///
/// @return \c TRUE if a band scan was successfully initiated,
///         otherwise \c FALSE
///
extern MAPCORE_API
BOOL8 traffic_bandScan(void);

//@MIC Start
/**
   * @name Get TWN parking ID and POI ID 
   *
   * For TWN RDS-TMC, parking lot availabe spaces transmiited from
   * RDS-TMC, there's a mapping table for this service, here provide
   * APIs to get IDs
   * @{
   */
   
  /**
 * @brief get parking ID by POI ID
 *
 * @return parking ID if we can get the parking ID from pNavi_db.tmc
 *         otherwise return 0xFFFF
 */

extern MAPCORE_API
UINT16 traffic_getParkingID(UINT32 poiId);

 /**
 *@brief get POI ID by parking ID
 *
 * @return POI ID if we can get the POI ID from pNavi_db.tmc
 *         otherwise return 0xFFFFFFFF
 */

extern MAPCORE_API
UINT32 traffic_getParkingPOI(UINT16 parkingId,
                             char *rdsFileName);

/**
 *@brief get POI ID by KTV ID (vendorID + locID + storeID)
 *
 * @return get POI ID if we can get the get POI ID from KTV.txt
 *         otherwise return 0xFFFFFFFF
 */

extern MAPCORE_API
UINT32 traffic_getKtvPOI(UINT16 vendorID,
                         UINT16 locID,
                         UINT16 storeID,
                         char *rdsFileName); 
//@MIC End

//@}

/**
 * @name Debugging
 * @{
 */

/**
 * @brief Dumps information about a traffic event to the debugging
 *        stream.
 *
 * @param[in] ev a pointer to a structure describing a traffic event
 * @param     hasJourneyInfo \c TRUE if the information relating to a
 *                           specific journey has been populated (i.e.
 *                           by a successful call to
 *                           #traffic_isEventOnCurrentJourney)
 */
extern MAPCORE_API
void traffic_dumpEventInfo(const TAppTrafficEvent *ev, BOOL8 hasJourneyInfo);

extern MAPCORE_API
BOOL8 traffic_addAvoidedEventCodes(UINT32 eventCode);
//@}

/** @} */

#ifdef __cplusplus
}
#endif

#endif
