#ifndef TRJOURNEYEVENTSP_H
#define TRJOURNEYEVENTSP_H

#include <traffic/trapi.h>
#include <containers/gestaticheapP.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_JOURNEY_EVENTS 32

typedef struct TJourneyEventEntry {
  TTrafficEventId eventId;
  BOOL8 avoided;
  UINT32 lastNotificationId;
} TJourneyEventEntry;

typedef struct TJourneyEvents {
  UINT32 numEvents;
  TJourneyEventEntry entries[MAX_JOURNEY_EVENTS];
} TJourneyEvents;

typedef enum {
  journeyEventNothing,
  journeyEventJustNotified,
  journeyEventAvoided
} TJourneyEventStatus;

BOOL8 journeyEvents_init(void);

TJourneyEvents *journeyEvents_fromForeignStorage(
  const TStaticHeap *storage);

BOOL8 journeyEvents_addEvent(TJourneyEvents *events, TTrafficEventId eventId);

BOOL8 journeyEvents_setEventAvoided(TJourneyEvents  *events,
                                    TTrafficEventId  eventId,
                                    BOOL8            avoided);

BOOL8 journeyEvents_setLastNotificationId(TJourneyEvents *events,
                                          TTrafficEventId eventId,
                                          UINT32 lastNotificationId);

BOOL8 journeyEvents_removeEvent(TJourneyEvents  *events,
                                TTrafficEventId  eventId);

BOOL8 journeyEvents_clear(TJourneyEvents *events);

BOOL8 journeyEvents_clearAvoided(TJourneyEvents *events);

TJourneyEventStatus journeyEvents_eventStatus(const TJourneyEvents *events,
                                              TTrafficEventId       eventId);

UINT32 journeyEvents_lastNotificationId(const TJourneyEvents *events,
                                        TTrafficEventId       eventId);

UINT32 journeyEvents_getEvents(const TJourneyEvents *events,
                               TJourneyEventEntry   *eventEntries,
                               UINT32                maxEvents);

UINT32 journeyEvents_getAvoidedEvents(const TJourneyEvents *events,
                                      TTrafficEventId      *eventIds,
                                      UINT32                maxEvents);

UINT32 journeyEvents_getNumEvents(const TJourneyEvents *events);

UINT32 journeyEvents_getNumAvoidedEvents(const TJourneyEvents *events);

#ifdef __cplusplus
}
#endif

#endif
