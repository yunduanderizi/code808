#ifndef TRLOCALP_H
#define TRLOCALP_H

#include <memory/allocator.h>
#include <traffic/trlocal.h>
#include <traffic/trtimeP.h>
#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup TrafficLocal
/// @ingroup Traffic
/// @brief Implementation wide definitions for TMC internal use.
///
/// @{

#define TRAFFIC_MAX_PATH 200
#define TMC_KTV_MAX_COUNT 56

// Miscellaneous Defines
#define INVALID_TMC_EVENT   (UINT32)0xFFFFFFFF
#define NO_TEXT               0

// Open Data Application ID's 
#define ALERT_C_ODA     0x0D45
#define TMC_ODA         0xCD46

// Inter-road location ID range
#define INTER_ROAD_MIN  64512
#define INTER_ROAD_MAX  65532

// How often (in minutes) to perform age management on event list
// NOTE: This interval MUST be a factor of 15 (ie 1,3,5,15)so that
// event lifetimes are aged correctly. It is set
// to 3 so that the removal of events that have not been refreshed for
// 15 minutes is more gradual if the User happens to be in the event list
#define EVENT_LIST_AGEING_INTERVAL_MINUTES 3


// A typedef to provide a format for a RAW RDS message
typedef struct TtmcMessage
{
   char msgData[8];
} TtmcMessage;

// Event Class as defined by ISO 14819-2, encoded
// into a 6 bit field in the extended event descriptor
typedef enum 
{
   TMC_EVT_CLASS_LEVEL_OF_SERVICE = 1,
   TMC_EVT_CLASS_EXPECTED_LEVEL_OF_SERVICE,
   TMC_EVT_CLASS_ACCIDENT,
   TMC_EVT_CLASS_INCIDENT,
   TMC_EVT_CLASS_CLOSURE_RESTRICTION,
   TMC_EVT_CLASS_CARRIAGEWAY_RESTRICTION,
   TMC_EVT_CLASS_EXIT_RESTRICTION,
   TMC_EVT_CLASS_ENTRY_RESTRICTION,
   TMC_EVT_CLASS_TRAFFIC_RESTRICTION,
   TMC_EVT_CLASS_CARPOOL_INFO,
   TMC_EVT_CLASS_ROADWORKS,
   TMC_EVT_CLASS_OBSTRUCTION,
   TMC_EVT_CLASS_DANGEROUS,
   TMC_EVT_CLASS_ROAD_CONDITION,
   TMC_EVT_CLASS_TEMPERATURE,
   TMC_EVT_CLASS_PRECIPITATION,
   TMC_EVT_CLASS_WIND,
   TMC_EVT_CLASS_ACTIVITIES,
   TMC_EVT_CLASS_SECURITY_ALERT,
   TMC_EVT_CLASS_DELAY,
   TMC_EVT_CLASS_CANCELLATION,
   TMC_EVT_CLASS_TRAVEL_TIME,
   TMC_EVT_CLASS_DANGEROUS_VEHICLE,
   TMC_EVT_CLASS_EXCEPTIONAL_VEHICLE,
   TMC_EVT_CLASS_EQUIPMENT_STATUS,
   TMC_EVT_CLASS_LIMIT,
   TMC_EVT_CLASS_PARKING_RESTRICTION,
   TMC_EVT_CLASS_PARKING,
   TMC_EVT_CLASS_AUDIO_REFERENCE,
   TMC_EVT_CLASS_SERVICE,
   TMC_EVT_CLASS_SPECIAL,

   TMC_EVT_CLASS_LEVEL_OF_SERVICE_FORECAST,
   TMC_EVT_CLASS_WEATHER_FORECAST,
   TMC_EVT_CLASS_ROAD_CONDITION_FORECAST,
   TMC_EVT_CLASS_ENVIRONMENT_FORECAST,
   TMC_EVT_CLASS_WIND_FORECAST,
   TMC_EVT_CLASS_TEMPERATURE_FORECAST,
   TMC_EVT_CLASS_DELAY_FORECAST,
   TMC_EVT_CLASS_CANCELLATION_FORECAST,
   TMC_EVT_CLASS_HIGHWAY40,
   TMC_EVT_CLASS_HIGHWAY60,
   TMC_EVT_CLASS_HIGHWAY80,
   TMC_EVT_CLASS_TRAFFIC_TW,
   TMC_EVT_CLASS_OBSTACLE_TW,
   TMC_EVT_CLASS_JAM_TW,
   TMC_EVT_CLASS_CONTROL_TW,
   TMC_EVT_CLASS_SIGN_TW,
   TMC_EVT_CLASS_ROADWORK_TW,
   TMC_EVT_CLASS_DISASTER_TW
} TtmcEventClass;
///@TPE

// Generic Event Nature, 
// 2 bit field in the extended event descriptor
typedef enum
{
   TMC_EVT_NATURE_INFORMATION = 0,
   TMC_EVT_NATURE_FORECAST,
   TMC_EVT_NATURE_SILENT
} TtmcEventNature;

// Event Lifetime is used for automatic ageing
// of the event, 2 bit field. The first 2 usually refer to the
// actual event, the last two are used to refer to the actual
// message rather than the event
typedef enum
{
   TMC_EVT_LIFE_DYNAMIC = 0,
   TMC_EVT_LIFE_LONGLIFE,
   TMC_EVT_LIFE_MSG_DYNAMIC,  
   TMC_EVT_LIFE_MSG_LONGLIFE
} TtmcDurationType;

// The tmc spec includes a default directionality for an event
// this can be overidden by additional message parameters
// single bit field
typedef enum
{
   TMC_EVT_DIR_UNIDIRECTIONAL = 0,
   TMC_EVT_DIR_BIDIRECTIONAL
} TtmcEventDirection;

// pre-defined event urgency, can be used by receiver logic
// to assign priority to event notification to user (2 bit field)
typedef enum
{
   TMC_EVT_URGENCY_NORMAL = 0,
   TMC_EVT_URGENCY_URGENT,
   TMC_EVT_URGENCY_EXTREME
} TtmcEventUrgency;

// Some events can have optional extra information in the form of a 
// 5 or 8 bit number. The interpretation of this is fixed by specification
// for that event. The following are possible, encoded into a 4 bit field
typedef enum
{
   TMC_EVT_INFO_NONE = 0,
   TMC_EVT_INFO_SMALL_NUMBER,
   TMC_EVT_INFO_NUMBER,
   TMC_EVT_INFO_DISTANCE,
   TMC_EVT_INFO_PERCENT,
   TMC_EVT_INFO_SPEED,
   TMC_EVT_INFO_DURATION,
   TMC_EVT_INFO_TEMPERATURE,
   TMC_EVT_INFO_TIME,
   TMC_EVT_INFO_WEIGHT,
   TMC_EVT_INFO_LENGTH,
   TMC_EVT_INFO_MILLIMETRES,
   TMC_EVT_INFO_MHZ,
   TMC_EVT_INFO_KHZ
} TtmcExtraInfo;


// Structure of an event decription, created by using original
// 11 bit event code together with extended info from table lookup in tmcevent.c
typedef struct TtmcDEvent {
    BITFIELD32 tmcEvent:11;               /* As per ISO 14819-2 direct from TMC rx */
    BITFIELD32 tmcSpare:4;
    BITFIELD32 tmcExtraInfoType:4;        /* There MAY be extra info */
    BITFIELD32 tmcEventBidirectional:1;   /* Bidirectional event */
    BITFIELD32 tmcEventDurationType:2;    /* Very generic duration */
    BITFIELD32 tmcEventUrgency:2;         /* */
    BITFIELD32 tmcEventNature:2;          /* forecast / event */
    BITFIELD32 tmcEventClass:6;           /* Generic Class of event */
} TtmcDEvent;

// bit shift defines for creating and retrieving the constituent parts of an event
#define EVT_EXTRA_INFO_SHIFT  15
#define EVT_DIRECTION_SHIFT   19
#define EVT_LIFE_SHIFT        20
#define EVT_URGENCY_SHIFT     22
#define EVT_NATURE_SHIFT      24
#define EVT_CLASS_SHIFT       26

// retrieval defines, get the various event bits back from the extended event UINT32
#define TMC_EVENT_CLASS (e) \
  (TtmcEventClass) (((UINT32)e >> EVT_CLASS_SHIFT) & 0x3F)

#define TMC_EVENT_NATURE (e) \
  (TtmcEventNature) (((UINT32)e >> EVT_NATURE_SHIFT) & 0x03)

#define TMC_EVENT_URGENCY (e) \
  (TtmcEventUrgency) (((UINT32)e >> EVT_URGENCY_SHIFT) & 0x03)

#define TMC_EVENT_LIFE (e) \
  (TtmcDurationType) (((UINT32)e >> EVT_LIFE_SHIFT) & 0x03)

#define TMC_EVENT_DIRECTION (e) \
  (TtmcEventDirection) (((UINT32)e >> EVT_DIRECTION_SHIFT) & 0x01)

#define TMC_EVENT_XINFO (e) \
  (TtmcEventExtraInfo) (((UINT32)e >> EVT_EXTRA_INFO_SHIFT) & 0x0F)

#define TMC_EVENT (e) \
  (UINT16) ((UINT32)e & 0x03FF)

// union allows access to whole extended event value as a UINT32 
typedef union TtmcEvent
{
        TtmcDEvent        ev;
        UINT32                  value;
} TtmcEvent;

// Location Descriptors. These refer to different ways of
// interpreting a location ID as per the spec
typedef enum TtmcLocationType
{
   locEmpty = 0,
   locPrimary,
   locDestination,
   locDiversion,
   locInterRoad
} TtmcLocationType;

// The internal representation of a location now
// has a bit of extra stuff packed around it
typedef struct TtmcLocation
{
   TtmcLocationType locationType;
   BITFIELD32 locationCC  :4;
   BITFIELD32 locationLTN :6;
   BITFIELD32 locationID  :16;
} TtmcLocation;

void tmcLocation_setFLT(TtmcLocation *loc, UINT16 flt);

// Event Duration descriptor
typedef struct TtmcDuration
{
   TtmcDurationType  durationType;
   UINT8             durationValue;
} TtmcDuration;

// Event extent descriptor
typedef enum TtmcExtentType
{
   extentLocations = 0,
   extentOffset,
   extentDistance
} TtmcExtentType;

typedef struct TtmcExtent
{
   TtmcExtentType extentType;
   UINT8          extentValue;
} TtmcExtent;

// Events need to be extended even more in reality
// because they include (possibly) an extra quantifier such as qty, length, weight, percent,
// length of route affected (implicit in event code or added later) and a speed limit advice
typedef struct TtmcExtendedEvent
{
   TtmcEvent        event;          // base event
   TtmcExtraInfo    extraType;      // An event may have extra information (%, Qty, etc)
   INT16            extraValue;     // The CONVERTED value ie real Qty, not binary index
} TtmcExtendedEvent;

// There is a need to keep additional information for each event, this includes the
// speech index appropriate, specific distances and delays. These used to be detected
// using the text indexes, but for Endeavor these have been replaced.

#define EV_NON_SPECIFIC 0
#define EV_SPECIFIC_LEN 1
#define EV_SPECIFIC_DLY 2
#define EV_ROAD_CLOSED  4

typedef struct 
{
  UINT8   eventSpeech;
  UINT8   eventInfoType; // bitmask of above defines
  UINT16  eventInfo;
} TEventSupplementaryInfo;

/**
 * The values of this enumeration correspond to the values of the
 * direction bit as defined in the RDS-TMC standard (ISO
 * 14819-1:2003(E) section 5.3.4.2)
 */
typedef enum {
  TMC_DIRECTION_POSITIVE = 0,
  TMC_DIRECTION_NEGATIVE = 1
} TtmcDirection;

const char *tmcDirToString(TtmcDirection dir);
BOOL8 tmcDirIsPos(TtmcDirection dir);
TtmcDirection tmcDirOpposite(TtmcDirection dir);

// Now finally everything together, the MESSAGE consists of a PRIMARY location
// and PRIMARY event (index 0 in the arrays), and the direction and extent of the
// primary event apply to all the other events if any. Event Duration is related to 
// each event individually, however explicit start and stop times if given relate
// to the whole message. Any extra locations are used for diversion information
// only. Given the maximum message length, there can be a max of 8 locations and
// 10 events, so we have to allow for this

#define PRIMARY_EVENT_INDEX     0
#define PRIMARY_LOCATION_INDEX  0
#define MAX_EVENTS_PER_MSG      10 // must be >= TRAFFIC_EVENT_TEXT_STRINGS
#define MAX_SUPPLEMENTS_PER_MSG 2  // must be >= TRAFFIC_EVENT_EXTRA_STRINGS
#define MAX_LOCATIONS_PER_MSG   8
#define MAX_EVENT_EXTENT        31 // maximum chain size for locations

// A structure that contains a decoded TMC event
typedef struct TtmcCompleteEvent
{
   TtmcExtendedEvent *events;
   TtmcLocation      *locations;
   UINT8             numEvents;
   UINT8             numLocations;
   // Variables associated with the whole message, some are based on data
   // associated with the primary event, which may be updated with further
   // data
   TtmcEventUrgency  urgency;   // copied from primary event definition
   UINT8             diversion; //   "
   UINT8             direction; //   "
   TtmcDuration      duration;  //   "
   TtmcExtent        extent; // This is data from initial message blocks only
   TtmcExtent        offset; // Future extension to TMC, offset from primary location
   BOOL8             invertDirectionality; // label 1 code 2
   UINT8             lengthAffected;       // label 2
   UINT8             speedAdvice;          // label 3
   UINT16            supplement[2];        // allow for two lots of
                                           // supplementary
                                           // information (indexes)
   // below are for time based deletion
   TRDSTime          startTime;  // label 7
   TRDSTime          finishTime; // label 8
} TtmcCompleteEvent;

UINT16 tmcEvent_getEvent(const TtmcExtendedEvent *ev);
UINT16 tmcEvent_getEventClass(const TtmcExtendedEvent *ev);
UINT16 tmcEvent_getEventNature(const TtmcExtendedEvent *ev);
UINT16 tmcEvent_getEventBidirectional(const TtmcExtendedEvent *ev);
UINT16 tmcEvent_getPrimaryEvent(const TtmcCompleteEvent *event);
UINT16 tmcEvent_getPrimaryEventClass(const TtmcCompleteEvent *event);
UINT16 tmcEvent_getPrimaryEventNature(const TtmcCompleteEvent *event);
UINT16 tmcEvent_getEventExtent(const TtmcCompleteEvent *event);
BOOL8 tmcEvent_isBidirectional(const TtmcCompleteEvent *event);
BOOL8 tmcEvent_hasLengthAffected(const TtmcCompleteEvent *ce);
UINT8 tmcEvent_getLengthAffected(const TtmcCompleteEvent *ce);
void tmcEvent_setLengthAffected(TtmcCompleteEvent *ce, UINT8 length);
UINT8 tmcEvent_getAdjustedDuration(const TtmcCompleteEvent *ce,
                                   TRDSTime utcLastRefreshed,
                                   TRDSTime utcCurrentTime,
                                   UINT32 localTimeOffsetMinutes);
UINT8 tmcEvent_eventMatches (TtmcCompleteEvent *evNew,
                    TtmcCompleteEvent *evList);

UINT8 tmcEvent_eventMatchesExact (TtmcCompleteEvent *evNew,
                         TtmcCompleteEvent *evList);

BOOL8 tmcEvent_copyTtmcCompleteEvent(TAllocatorHandle allocator,
                            TtmcCompleteEvent *dst,
                            TtmcCompleteEvent *src);
// The message group ID's
typedef enum TtmcGroup
{
   TMC_GROUP_0A=0,
   TMC_GROUP_0B,
   TMC_GROUP_1A,
   TMC_GROUP_1B,
   TMC_GROUP_2A,
   TMC_GROUP_2B,
   TMC_GROUP_3A,
   TMC_GROUP_3B,
   TMC_GROUP_4A,
   TMC_GROUP_4B,
   TMC_GROUP_5A,
   TMC_GROUP_5B,
   TMC_GROUP_6A,
   TMC_GROUP_6B,
   TMC_GROUP_7A,
   TMC_GROUP_7B,
   TMC_GROUP_8A,
   TMC_GROUP_8B,
   TMC_GROUP_9A,
   TMC_GROUP_9B,
   TMC_GROUP_10A,
   TMC_GROUP_10B,
   TMC_GROUP_11A,
   TMC_GROUP_11B,
   TMC_GROUP_12A,
   TMC_GROUP_12B,
   TMC_GROUP_13A,
   TMC_GROUP_13B,
   TMC_GROUP_14A,
   TMC_GROUP_14B,
   TMC_GROUP_15A,
   TMC_GROUP_15B
} TtmcGroup;

// data we need to store that relates to the SERVICE being received
typedef struct TtmcServiceData
{
   UINT16             activeODA;                // non zero idicates ODA active
   UINT16             countryCode;              // unique country code for transmission
   UINT16             programID;                // PI code from block 1
   TtmcGroup          messageGroup;             // the message group used for reception
   UINT8              tableNumber;              // location table number
   UINT8              tablePresent;             // true if location table found
   UINT8              encryptedService;         // if non zero, this is the table index for an encrypted service
   UINT8              serviceIdentifier;        // defines the information provider
   UINT8              coverage;                 // service coverage
   UINT8              transmissionMode;         // transmission parameters
   UINT8              gap,Ta,Tw,Td;             //    ""
   UINT8              afi;                      //    ""
   UINT8              encid;                    // encryption key
   UINT8              serviceKey;               // service key
   UINT8              initialisedFromFile;      // LTN loaded from LTN file
} TtmcServiceData;

/// Result type for message processing routines
typedef enum TtmcProcess
{
  tmcNone = 0,                  ///< nothing to process
  tmcNewEvent,                  ///< new event in list        
  tmcEventChange,               ///< an event has changed
  tmcEventDeleted,              ///< an event has been deleted
  tmcMultipleEventsDeleted,     ///< multiple events have been deleted
  tmcTimeChange,                ///< time message
  tmcTune,                      ///< tuning information received
  tmcNewPI,                     ///< change of station
  tmcNewODA,                    ///< new ODA now in operation (from group 3A msg)
  tmcEncryptedSupport,          /// Special for encrypted table number
  tmcBadODA                     /// For alert-C transmission in France or no location table 
                                /// information with event in Taiwan
} TtmcProcess;

// Structure used to return processing results
// this will need to be extended to cope with whatever
typedef struct TtmcResult
{
  TtmcProcess process;          ///< processing required
  UINT16      eventIndex;       ///< index of new or changed event
  UINT16      eventID;          ///< id of new, changed or deleted event
  TtmcGroup   msgGroup;         ///< the message group 
  UINT16      piCode;           ///< piCode
} TtmcResult;

// END PRIVATE

/// @}

#ifdef __cplusplus
}
#endif

#endif /* TRLOCALP_H */
