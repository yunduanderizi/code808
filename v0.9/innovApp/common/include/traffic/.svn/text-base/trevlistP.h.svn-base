#ifndef TREVLISTP_H
#define TREVLISTP_H

#include <traffic/trevlist.h>
#include <containers/gestaticheap.h>
#include <display/diroadstyle.h>
#include <mapdata/enroadP.h>
#include <routing/rtjourney.h>
#include <traffic/trapi.h>
#include <traffic/trlocalP.h>
#include <traffic/trlocformatP.h>
#include <traffic/trloctableP.h>
#include <traffic/trsortP.h>
#include <traffic/trtimeP.h>

#ifdef __cplusplus
extern "C" {
#endif

#if TRAFFIC_ICONS_AT_PRIMARY_LOCATION
#define MAX_ICONS_PER_MSG 4 // bidirectional events get two icons at
                            // each end
#else
#define MAX_ICONS_PER_MSG 2
#endif

typedef enum
{
  waitingForLock = 0,
  outsideBubble,
  insideBubble
} TTrafficBubbleStatus;

typedef struct TtmcEventListEntry {
  TtmcCompleteEvent e;
  TAppTrafficEventClass eventAppClass;
  TtmcSortFmtLoc    fmtLocSort; ///< road number sorting information
  TRoadTmcLocationSegment *affectedSegments;
  UINT16            numAffectedSegments;
  UINT8             numIcons;
  TPos              iconPos[MAX_ICONS_PER_MSG];
  TRect             boundingBox;     ///< bounding box of the extent
  TRDSTime          timeReceived;  // UTC time event first received
  TRDSTime          lastUpdated;   // event modified in some way
  TRDSTime          lastRefreshed; // last received time (UTC) for deletion code
  TTrafficNotificationType notificationType;
  UINT32            lastNotificationId;
  TTrafficBubbleStatus bubbleStatus;
  BOOL8             cannotBeAvoided; ///< if the event cannot be routed around
  BOOL8             notify;
  UTF16             *eventSource; // station name
  UINT16            sourceFrequency; ///< station frequency
  UINT16            eventID;
  // fields below are filled in after segment list is created
  // The delay is only non-zero if there is an implicit delay time
  // in the event, the length is 
  // a) any distance implicit in the event code OR
  // b) the accumulated length of the segments involved
  UINT16            eventDelay;  // delay in minutes expected (if available)
  UINT32            eventLength; // Approximate length of event in metres
  ROADRENDERMODE    renderMode;  // logically ?? determined display colour
  // Speech Prompt support variables
  TLanguageTrafficEventType   eventSpeechType; 
  TLanguageTrafficSituation   eventSpeechSituation;
} TtmcEventListEntry;

BOOL8 tmcEventList_isInitialised( void);

BOOL8 tmcEventList_initialise(void);

/** Given a pointer to a decoded event, current utc time and a 
    pointer to a result structure. This procedure updates the current event
    list with information from the new event (either updates an existing copy,
    adds a new event, or deletes one or several existing events)

    @param ev A pointer to a fully decoded event (the result of the event parser)

    @param utcTime The "current" RDS UTC time in our internal format

    @param result A pointer to the standard result structure
*/

void tmcUpdateEventList (TtmcCompleteEvent *ev, TRDSTime utcTime, TtmcResult *result);

/** Perform time-based processing, such as event ageing.

    @param utcTime The current RDS utc time

    @param result  A pointer to the standard result structure
*/
void tmcEventList_tick(TRDSTime utcTime, TtmcResult *result);

UINT16 tmcClearEventList (void);

#define MAX_EVENTS_PER_SEGMENT 10

UINT16 tmcEventList_findEventsAffectingSegment(const TRoadSegment *segment,
                                               UINT16 maxNumEvents);
BOOL8 tmcEventList_isSegmentAffected(const TRoadSegment *segment);

BOOL8 tmcEventList_calculateRoutingCost(const TRoadSegment *segment,
                                        const TStaticHeap *storage,
                                        UINT32 *scaleFactor,
                                        INT32 *shiftFactor);

UINT16 tmcEventList_getMaxEventCount(void);
BOOL8 tmcEventList_setMaxEventCount(UINT16 newMax);
UINT16 tmcEventList_getEventCount(void);
UINT16 tmcEventList_getActiveEventCount(void);
TtmcEventListEntry *tmcEventList_getEntry(UINT16 index);

BOOL8 tmcEventListEntry_isActive(const TtmcEventListEntry *entry);

INT8 tmcEventList_fillInEventJourneyInfo(UINT16 eventID,
                                         const TJourney *journey,
                                         const TPos *mapPos,
                                         TAppTrafficEvent *appEvent);

UINT32 tmcEventListEntry_getDistanceToEvent(const TtmcEventListEntry *entry,
                                            const TJourney *journey,
                                            const TPos *mapPos);

UINT32 tmcEventListEntry_getDistanceToEventInStraightLine(
  const TtmcEventListEntry *entry, const TPos *mapPos);

BOOL8 tmcEventListEntry_getDistanceToEventAlongJourney(
  const TtmcEventListEntry *entry, const TJourney *journey,
  const TPos *mapPos, UINT32 *distance);

BOOL8 tmcEventListEntry_isOnJourney(const TtmcEventListEntry *entry,
                                    const TJourney *journey,
                                    const TPos *mapPos);
BOOL8 tmcEventListEntry_didAffectJourney(const TtmcEventListEntry *entry,
                                         const TJourney *journey);
BOOL8 tmcEventListEntry_isDetourRecommended(
  const TtmcEventListEntry *entry,
  const TJourney *journey,
  const TPos *mapPos,
  BOOL8 isOnJourney);

UINT8 tmcEventListEntry_getAdjustedDuration(const TtmcEventListEntry *entry,
                                            TRDSTime utcTime);

// find array entry from event ID
TtmcEventListEntry *tmcEventList_findEventByID(UINT16 eventID);

UINT16 tmcEventList_getBubbleSize (void);
BOOL8  tmcEventList_setBubbleSize (UINT16 bubbleSize);

BOOL8 tmcEventList_isEventInBubble(TTrafficEventId  eventId,
                                   const TPos      *mapPos);

void tmcEventList_applyTimeDelta (INT32 delta);

UINT32 tmcEventList_getNotifications(const TJourney *journey,
                                     const TPos *mapPos,
                                     TTrafficNotification *notifications,
                                     UINT32 maxNotifications);
BOOL8 tmcEventList_repeatNotification(TTrafficEventId eventId);
void tmcEventList_resetNotifications(void);
UINT32 tmcEventList_getLastNotificationId(void);

#ifdef __cplusplus
}
#endif

#endif /* TREVLISTP_H */
