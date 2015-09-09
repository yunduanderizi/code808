#ifndef LATYPES_H
#define LATYPES_H

#include <platform_types.h>
#include <language/ConvertUTF.h>
#include <math/geconvert.h>
#include <mapdata/enroad.h>
#include <geosearch/geosearch.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Language
/// @{

/** Mapcore 140 speech file version identifier */
#define SPEECH_FILE_VERSION_V8 12

/* Maximum length of a phonetic string */
#define PHONE_MAX_SIZE         200

/** @typedef enum TLanguageCode
 *  Multilingual maps contain text in a number of different languages,
 *  and the map engine is built to return text and voice guidance in
 *  one language at a time.
 *
 *  CAVEAT: if this enum is updated, corresponding enum in map tool
 *          chain should also be updated in:
 *          $PROJECT_SRC/mapprod-LATEST/compilers/encoder/common/types.h
 *          The order of the enums _must_ match or there will be hell to pay
 */
typedef enum {
  /** An invalid language code */
  LANGUAGE_CODE_INVALID = 255,
  /** US English */
  LANGUAGE_CODE_US = 0,
  /** American English */
  LANGUAGE_CODE_AMERICAN = 0,
  /** UK English */
  LANGUAGE_CODE_UK = 1,
  /** British English */
  LANGUAGE_CODE_BRITISH = 1,
  /** Dutch */
  LANGUAGE_CODE_DUTCH,
  /** French */
  LANGUAGE_CODE_FRENCH,
  /** German */
  LANGUAGE_CODE_GERMAN,
  /** Italian */
  LANGUAGE_CODE_ITALIAN,
  /** Spanish */
  LANGUAGE_CODE_SPANISH,
  /** Danish */
  LANGUAGE_CODE_DANISH,
  /** Swedish */
  LANGUAGE_CODE_SWEDISH,
  /** Finnish */
  LANGUAGE_CODE_FINNISH,
  /** Czech */
  LANGUAGE_CODE_CZECH,
  /** Portuguese  */
  LANGUAGE_CODE_PORTUGUESE,
  /** Bulgarian */
  LANGUAGE_CODE_BULGARIAN,
  /** Russian */
  LANGUAGE_CODE_RUSSIAN,
  /** Polish  */
  LANGUAGE_CODE_POLISH,
  /** Maori */
  LANGUAGE_CODE_MAORI,
  /** Norwegian */
  LANGUAGE_CODE_NORWEGIAN,
  /** Australian */
  LANGUAGE_CODE_AUSTRALIAN,
  /** Turkish */
  LANGUAGE_CODE_TURKISH,
  /** Greek */
  LANGUAGE_CODE_GREEK,
  /** Hungarian */
  LANGUAGE_CODE_HUNGARIAN,
  /** Mandarin */
  LANGUAGE_CODE_MANDARIN,
  /** Cantonese */
  LANGUAGE_CODE_CANTONESE,
  /** Estonian */
  LANGUAGE_CODE_ESTONIAN,
  /** Spanish American */
  LANGUAGE_CODE_SPANISH_AMERICAN,
  /** Japanese */
  LANGUAGE_CODE_JAPANESE,
  /** Latvian */
  LANGUAGE_CODE_LATVIAN,
  /** Lithuanian */
  LANGUAGE_CODE_LITHUANIAN,
  /** Malay */
  LANGUAGE_CODE_MALAY,
  /** Romanian */
  LANGUAGE_CODE_ROMANIAN,
  /** Serbian (Latin Alphabet) */
  LANGUAGE_CODE_SERBIAN,
  /** Thai */
  LANGUAGE_CODE_THAI,
  /** Catalan */
  LANGUAGE_CODE_CATALAN,
  /** Croatian */
  LANGUAGE_CODE_CROATIAN,
  /** Brazilian Portuguese */
  LANGUAGE_CODE_BRAZILIAN_PORTUGUESE,
  /** French Canadian */
  LANGUAGE_CODE_FRENCH_CANADIAN,
  /** Arabic (Arabic Alphabet) */
  LANGUAGE_CODE_ARABIC,
  /** Persian */
  LANGUAGE_CODE_PERSIAN,
  /** Hebrew */
  LANGUAGE_CODE_HEBREW,
  /** Esperanto */
  LANGUAGE_CODE_ESPERANTO,
  /** Basque */
  LANGUAGE_CODE_BASQUE,
  /** Welsh */
  LANGUAGE_CODE_WELSH,
  /** Irish */
  LANGUAGE_CODE_IRISH,
  /** Gaelic */
  LANGUAGE_CODE_GAELIC,
  /** Korean */
  LANGUAGE_CODE_KOREAN,
  /** Flemish */
  LANGUAGE_CODE_FLEMISH,
  /** Galician */
  LANGUAGE_CODE_GALICIAN,
  /** Raeto-Romance */
  LANGUAGE_RAETO_ROMANCE,
  /** Valencian */
  LANGUAGE_VALENCIAN,
  /** Frisian */
  LANGUAGE_FRISIAN,
  /** Slovak */
  LANGUAGE_SLOVAK,
  /** Albanian */
  LANGUAGE_ALBANIAN,
  /** Macedonian Latin */
  LANGUAGE_MACEDONIAN_LATIN,
  /** Russian Latin */
  LANGUAGE_RUSSIAN_LATIN,
  /** Bulgarian Latin */
  LANGUAGE_BULGARIAN_LATIN,
  /** Belarusian Latin */
  LANGUAGE_BELARUSIAN_LATIN,
  /** Belarusian */
  LANGUAGE_BELARUSIAN,
  /** Moldavian */
  LANGUAGE_MOLDAVIAN,
  /** Ukrainian */
  LANGUAGE_UKRAINIAN,
  /** Ukrainian Latin */
  LANGUAGE_UKRAINIAN_LATIN,
  /** Bosnian */
  LANGUAGE_BOSNIAN,
  /** Greek Latin */
  LANGUAGE_GREEK_LATIN,
  /** Macedonian */
  LANGUAGE_MACEDONIAN,
  /** Serbian (Cyrillic) */
  LANGUAGE_SERBIAN_CYRILLIC,
  /** Slavic */
  LANGUAGE_SLAVIC,
  /** Slovenian */
  LANGUAGE_SLOVENIAN,
  /** Indonesian */
  LANGUAGE_INDONESIAN,
  /** Chinese (Simplified) */
  LANGUAGE_CHINESE_SIMPLIFIED,
  /** Chinese (Latin Alphabet) */
  LANGUAGE_CHINESE_LATIN,
  /** Chinese (Traditional) */
  LANGUAGE_CHINESE_TRADITIONAL,
  /** Afrikaans */
  LANGUAGE_AFRIKAANS,
  /** Arabic (Latin Alphabet) */
  LANGUAGE_ARABIC_LATIN,
  /** Hebrew (Latin Alphabet) */
  LANGUAGE_HEBREW_LATIN,
  /** Xhosa */
  LANGUAGE_XHOSA,
  /** Zulu */
  LANGUAGE_ZULU,
  /** Luxembourgish */
  LANGUAGE_LUXEMBOURGISH,
  /** Maltese */
  LANGUAGE_MALTESE,
  /** Taiwan ZuYin */
  //LANGUAGE_CHINESE_CHP,
  /** This value must always come last */
  LANGUAGE_NUM_CODES
} TLanguageCode;

/// The style of an instruction. Turn style greatly influences the kind of
/// guidance instructions given to the user.
typedef enum {
  /// There is no turn.
  LANGUAGE_STYLE_NO_TURN,
  // The instruction is a start of a journey leg.
  LANGUAGE_STYLE_INITIAL,
  /// The turn is graded. That is, it may be sharp, slight, medium etc.
  LANGUAGE_STYLE_GRADED_TURN,
  /// The turn is leaving a roundabout.
  LANGUAGE_STYLE_ROUNDABOUT_EXIT,
  /// The turn is exiting a freeway. Tele Atlas define 'freeway' as a
  /// controlled access road - a dual carriageway that is connected to the rest
  /// of the road network by grade separated crossings. This includes roads
  /// officially designated as motorways, and other major roads.
  LANGUAGE_STYLE_FREEWAY_EXIT,
  /// The turn is a 'good' u-turn such as at a dual carriageway u-turn bay. To
  /// give a "bad" u-turn, use the LANGUAGE_STYLE_BAD_UTURN.
  LANGUAGE_STYLE_UTURN,
  /// The turn is a 'bad' u-turn (e.g. "Perform a u-turn when possible").
  LANGUAGE_STYLE_BAD_UTURN,
  /// The 'turn' is the end of a single leg journey.
  LANGUAGE_STYLE_TERMINATION,
  /// The 'turn' is the end of one leg of a multi-leg journey.
  LANGUAGE_STYLE_INTERMEDIATE_DESTINATION,
  /// The 'turn' is the end of the last leg of a multi-leg journey.
  LANGUAGE_STYLE_FINAL_DESTINATION,
  /// The 'turn' is entering a ferry, unsealed road etc.
  LANGUAGE_STYLE_ATTRIBUTE_CHANGE,
  /// The turn is a merge onto (for example) a controlled access road.
  LANGUAGE_STYLE_MERGE,
  /// The turn is a curve (advisory).
  LANGUAGE_STYLE_CURVE,
  // The turn is a lane change.
  LANGUAGE_STYLE_LANE,
  // The signs should be followed.
  LANGUAGE_STYLE_FOLLOW_SIGNS,
  // Approaching an important road or POI.
  LANGUAGE_STYLE_APPROACHING,
  /// The road joins another.
  LANGUAGE_STYLE_JOIN,
  /// GPS lost notice
  LANGUAGE_STYLE_GPS_LOST,
  /// Route notice (e.g. "Calculating route")
  LANGUAGE_STYLE_CALC_ROUTE,
  /// Re-route notice (e.g. "Re-calculating route")
  LANGUAGE_STYLE_RECALC_ROUTE,
  /// Speed warning notice
  LANGUAGE_STYLE_SPEED_WARNING,
  /// Reroute completed notice
  LANGUAGE_STYLE_REROUTE_COMPLETED,
  /// Reroute prompt
  LANGUAGE_STYLE_REROUTE_PROMPT,
  /// Volume prompt
  LANGUAGE_STYLE_VOLUME_PROMPT,
  /// Speed camera alert
  LANGUAGE_STYLE_SPEED_CAM_WARNING,
  /// This value should always come last.
  LANGUAGE_STYLE_MAX
} TLanguageStyle;


/// Grades are used to indicate the severity of a graded turn instruction.
/// Severity may relate to factors such as the change in heading between the
/// instruction source and destination roads, and to the total number of roads
/// connected to the intersection.
/// @note
/// Grade is only applicable for #LANGUAGE_STYLE_GRADED_TURN instructions.
typedef enum {
  /// There is no turn.
  LANGUAGE_GRADE_NO_TURN,
  /// The turn is a slight turn.
  LANGUAGE_GRADE_SLIGHT,
  /// The turn is a medium turn.
  LANGUAGE_GRADE_MEDIUM,
  /// The turn is a sharp turn.
  LANGUAGE_GRADE_SHARP,
  /// Reassurance "Continue Left" etc.
  LANGUAGE_GRADE_CONTINUE,
  /// "Keep Left" etc. Used at bifurcations.
  LANGUAGE_GRADE_KEEP,
  /// This value should always come last.
  LANGUAGE_GRADE_MAX
} TLanguageGrade;


/// Direction of a turn. That is, left, right or straight ahead.
typedef enum {
  /// We don't care about the turn direction, or we do not know
  LANGUAGE_DIRECTION_IMMATERIAL,
  /// The turn is to the left
  LANGUAGE_DIRECTION_LEFT,
  /// There is no turn.
  LANGUAGE_DIRECTION_STRAIGHT,
  /// The turn is to the right.
  LANGUAGE_DIRECTION_RIGHT,
  /// The turn follows the current road.
  LANGUAGE_DIRECTION_CURRENT_ROAD,

  ///
  /// Cardinal directions: sometimes we want to specify direction as a cardinal
  /// The order has to be the same as in the vocabulary
  ///
  LANGUAGE_DIRECTION_FIRST_CARDINAL,  // marker - which cardinal is first...
  LANGUAGE_DIRECTION_NORTH = LANGUAGE_DIRECTION_FIRST_CARDINAL,
  LANGUAGE_DIRECTION_SOUTH,
  LANGUAGE_DIRECTION_EAST,
  LANGUAGE_DIRECTION_WEST,
  LANGUAGE_DIRECTION_NORTHEAST,
  LANGUAGE_DIRECTION_NORTHWEST,
  LANGUAGE_DIRECTION_SOUTHEAST,
  LANGUAGE_DIRECTION_SOUTHWEST,
  LANGUAGE_DIRECTION_LAST_CARDINAL = LANGUAGE_DIRECTION_SOUTHWEST
} TLanguageDirection;


/// A set of attributes associated with an instruction
typedef UINT32 TLanguageFlags;

#define LANGUAGE_FLAG_MOTORWAY          0x00000001
#define LANGUAGE_FLAG_FERRY             0x00000002
#define LANGUAGE_FLAG_PRIVATE           0x00000004
#define LANGUAGE_FLAG_UNSEALED          0x00000008
#define LANGUAGE_FLAG_TOLL              0x00000010
#define LANGUAGE_FLAG_BRIDGE            0x00000020
#define LANGUAGE_FLAG_TUNNEL            0x00000040

#define LANGUAGE_FLAG_ATTRIBUTE_CHANGE ( LANGUAGE_FLAG_MOTORWAY \
                                         | LANGUAGE_FLAG_FERRY \
                                         | LANGUAGE_FLAG_PRIVATE \
                                         | LANGUAGE_FLAG_UNSEALED \
                                         | LANGUAGE_FLAG_TOLL \
                                         | LANGUAGE_FLAG_BRIDGE \
                                         | LANGUAGE_FLAG_TUNNEL )

/// These will be used for "at the... " instructions
#define LANGUAGE_FLAG_TRAFFIC_LIGHTS    0x00000080
#define LANGUAGE_FLAG_ROUNDABOUT        0x00000100
#define LANGUAGE_FLAG_TOLL_BOOTH        0x00000200
#define LANGUAGE_FLAG_PETROL_STATION    0x00000400
#define LANGUAGE_FLAG_PARKING_LOT       0x00000800
#define LANGUAGE_FLAG_CITY_CENTRE       0x00001000
#define LANGUAGE_FLAG_AIRPORT           0x00002000
#define LANGUAGE_FLAG_MOTORWAY_ENTRANCE 0x00004000
#define LANGUAGE_FLAG_END_OF_ROAD       0x00008000
#define LANGUAGE_FLAG_HIGHWAY           0x00010000
#define LANGUAGE_FLAG_OVERPASS          0x00020000
#define LANGUAGE_FLAG_UNDERPASS         0x00040000
#define LANGUAGE_FLAG_SLOWWAY           0x00080000
#define LANGUAGE_FLAG_FASTWAY           0x00100000

#define LANGUAGE_FLAG_ROAD_TYPES ( LANGUAGE_FLAG_TRAFFIC_LIGHTS \
                                    | LANGUAGE_FLAG_ROUNDABOUT \
                                    | LANGUAGE_FLAG_TOLL_BOOTH \
                                    | LANGUAGE_FLAG_PETROL_STATION \
                                    | LANGUAGE_FLAG_PARKING_LOT \
                                    | LANGUAGE_FLAG_CITY_CENTRE \
                                    | LANGUAGE_FLAG_AIRPORT \
                                    | LANGUAGE_FLAG_MOTORWAY_ENTRANCE \
                                    | LANGUAGE_FLAG_END_OF_ROAD \
									| LANGUAGE_FLAG_HIGHWAY \
									| LANGUAGE_FLAG_OVERPASS \
									| LANGUAGE_FLAG_UNDERPASS \
									| LANGUAGE_FLAG_SLOWWAY \
									| LANGUAGE_FLAG_FASTWAY)

#define LANGUAGE_FLAG_HAS_LANEINFO      0x02000000

/// Insert the distance in subsequent instruction
#define	LANGUAGE_FLAG_SUBSEQUENT_DISTANCE	0x04000000

/// Flags to indicate the source of the name strings. TRUE if they are from
/// signposts, FALSE if they are road segment names.
#define LANGUAGE_FLAG_SIGNPOST_ROAD_NAME    0x08000000
#define LANGUAGE_FLAG_SIGNPOST_ROUTE_NUMBER 0x10000000

/// Insert "little joining words" before road and place names. E.g. If
/// LANGUAGE_PREPS_ROADS is set, we might say "Exit left into Onewa Road".
/// If it is unset, we will say "Exit left, Onewa Road".
#define LANGUAGE_FLAG_USE_PREPS_ROADS   0x20000000

/// Some countries should override the default language
/// setting when announcing exits. The following
/// two values should be mutually exclusive. Either one
/// or neither should be set.
#define LANGUAGE_FLAG_USE_JUNCTIONS 0x40000000
#define LANGUAGE_FLAG_USE_EXITS     0x80000000

/// The rule for Taiwan & China
/// to supported the exit-bridge(enter-bridge already defined),
///                  exit-overpass,
///                  enter-overpass,
///                  exit-tunnel(enter-tunnel already defined)
///                  enter-highway
///                  exit-highway
/// #define LANGUAGE_FLAG_OVERPASS        (LANGUAGE_FLAG_PRIVATE   | LANGUAGE_FLAG_BRIDGE)
/// #define LANGUAGE_FLAG_EXIT_OVERPASS   (LANGUAGE_FLAG_USE_EXITS | LANGUAGE_FLAG_OVERPASS)
/// #define LANGUAGE_FLAG_HIGHWAY         (LANGUAGE_FLAG_PRIVATE   | LANGUAGE_FLAG_BRIDGE    | LANGUAGE_FLAG_MOTORWAY)
/// #define LANGUAGE_FLAG_EXIT_HIGHWAY    (LANGUAGE_FLAG_USE_EXITS | LANGUAGE_FLAG_HIGHWAY)
/// #define LANGUAGE_FLAG_EXIT_BRIDGE     (LANGUAGE_FLAG_USE_EXITS | LANGUAGE_FLAG_BRIDGE)
/// #define LANGUAGE_FLAG_EXIT_TUNNEL     (LANGUAGE_FLAG_USE_EXITS | LANGUAGE_FLAG_TUNNEL)

/** The type of traffic event that has occured. Traffic events should be
    mapped onto one of these possibilities, as these are the only events
    that the language module is aware of. */
typedef enum {
  /** The traffic is slow */
  TRAFFIC_EVENT_SLOW,
  /** The traffic has improved. See also TLanguageTrafficSituation. */
  TRAFFIC_EVENT_IMPROVED,
  /** Stationary traffic */
  TRAFFIC_EVENT_STATIONARY,
  /** Queuing traffic */
  TRAFFIC_EVENT_QUEUING,
  /** A delay */
  TRAFFIC_EVENT_DELAY,
  /** The situation in general */
  TRAFFIC_EVENT_SITUATION,
  /** A generic 'event' */
  TRAFFIC_EVENT_EVENT,
  /** Highway speed <40' */
  TRAFFIC_EVENT_HIGHWAY40,
  /** 40<=Highway speed<60' */
  TRAFFIC_EVENT_HIGHWAY60,
  /** 60<=Highway speed <80' */
  TRAFFIC_EVENT_HIGHWAY80,

  TRAFFIC_EVENT_TRAFFIC_TW, 
  TRAFFIC_EVENT_OBSTACLE_TW,
  TRAFFIC_EVENT_JAM_TW,
  TRAFFIC_EVENT_CONTROL_TW,
  TRAFFIC_EVENT_SIGN_TW,
  TRAFFIC_EVENT_ROADWORKS_TW,
  TRAFFIC_EVENT_DISASTER_TW,
  /** A generic set of 'events' */
  TRAFFIC_EVENT_MULTIPLE_EVENTS
} TLanguageTrafficEventType;

/// The traffic situation that has occured or changed.
/// This enumeration is used to modify a TLanguageTrafficEventType.
typedef enum {
  /** The event is new */
  TRAFFIC_SITUATION_NEW,
  /** The event caused the route to change */
  TRAFFIC_SITUATION_ROUTE_CHANGED,
  /** The event has cleared entirely */
  TRAFFIC_SITUATION_CLEARED,
  /** The situation is better for the driver */
  TRAFFIC_SITUATION_BETTER,
  /** The situation is worse for the driver */
  TRAFFIC_SITUATION_WORSE,
  /** The situation has changed */
  TRAFFIC_SITUATION_CHANGED
} TLanguageTrafficSituation;

/// A summary of a traffic event that has occured or changed
typedef struct TSpokenTrafficData {
  /// The type of event that has occured or changed
  TLanguageTrafficEventType eventType;
  /// The type of change in the traffic situation
  TLanguageTrafficSituation situation;
  /// Set to TRUE if there is currently only one active traffic
  ///  event of the type 'event', otherwise FALSE.
  /// The following varies significantly by language, as language stuff
  ///  is wont to do.
  /// In general, if 'singular' is set to TRUE, the definate article will
  ///  be used ( eg: "The Traffic Event" ).  Otherwise, the indefinate
  ///  article will be used ( eg: "A Traffic Event" ).  Some grammars require
  ///  indefinate article, in which case the 'singular' value is ignored.
  BOOL8 singular;
  /// Set to TRUE if the instruction should refer to the route
  ///  instead of the distance, otherwise FALSE.
  BOOL8 onRoute;
  /// Distance to the event. Set to zero will report no distance information.
  TUserDistance distance;
} TSpokenTrafficData;


/// This structure holds the information needed for the language engine to be
/// able to accurately describe the lanes to drive in.
typedef struct TLanguageLanes {
  // is the lane the 'left', 'right' or 'centre' lane?
  TLanguageDirection lanePosition;
  // is it the '1st', '2nd', '5th' lane?
  INT32 laneNumber;
  // are they the '2 left lanes', the '4 right lanes', the '2 centre lanes'?
  // (in combination with lanePosition for the side)
  INT32 laneRange;
} TLanguageLanes;


/// This structure holds all the information that the language engine is aware of.
typedef struct TLanguageInstruction {
  /// The turn style controls the overall form of the turn instruction
  TLanguageStyle style;
  /// The vehicle type for which the instruction shall be created. This parameter
  /// is used only in instructions, that have different ways of saying the instruction
  /// depending on the vehicle type.
  TVehicleType vehicleType;
  /// Only used for style #LANGUAGE_STYLE_GRADED_TURN
  TLanguageGrade grade;

  /// How many turns to mention
  ///   - Use -1 for next e.g. "Take the next exit"
  ///   - Use 0 for single e.g. "Take the exit"
  ///   - Use positive number for ordinal. 1 -> "Take the first exit", through
  ///     to a maximum of 9 -> "Take the ninth exit"
  INT16 intermediateTurns;

  /// Direction is kind of important
  TLanguageDirection direction;

  /// Driving distance to the turn expressed as whole units
  /// plus optionally a decimal or fractional part
  TUserDistance distance;

  /// Number of flags to mention. Similar to intermediateTurns but for flags.
  INT16 intermediateFlags;
  /// Modification flags
  TLanguageFlags flags;

  /// Information needed to describe the lanes
  TLanguageLanes laneInfo;

  /// Signposted route number. Null-terminated string in UTF16
  UTF16 routeNumber[ROAD_MAX_NAME_SIZE];
  /// Phonetic representation of signposted route number for articulation with
  /// a L&H+ complient TTS engine. Null-terminated string in UTF16
  UTF16 routeNumberPhonetic[PHONE_MAX_SIZE];

  /// Signposted route directional. N, E, S or W
  UTF16 routeDirectional[ROAD_MAX_NAME_SIZE];

  /// Signposted exit number. NULL-terminated string in UTF16
  UTF16 exitNumber[ROAD_MAX_NAME_SIZE];
  /// Phonetic representation of signposted exit number for articulation with
  /// a L&H+ complient TTS engine. NULL-terminated string in UTF16
  UTF16 exitNumberPhonetic[PHONE_MAX_SIZE];

  /// Primary roadname. NULL-terminated string in UTF16.
  UTF16 roadName[ROAD_MAX_NAME_SIZE];
  /// Phonetic representation of roadname for articulation with a L&H+
  /// complient TTS engine. NULL-terminated string in UTF16.
  UTF16 roadNamePhonetic[PHONE_MAX_SIZE];

  /// House number for the destination
  UINT32 locationHouseNumber;
  /// Location name mto use in the voice instruction
  UTF16 locationName[GEOSEARCH_MAX_NAME_SIZE];
  /// Phonetic representation of the location name
  UTF16 locationPhonetic[PHONE_MAX_SIZE];

  /// Placename to use in the text instruction. NULL-terminated string in
  /// UTF16.
  UTF16 placeName[ROAD_MAX_NAME_SIZE];
  /// Phonetic representation of placename to use in the text instruction for
  /// articulation with a L&H+ complient TTS engine. NULL-terminated string
  /// in UTF16.
  UTF16 placeNamePhonetic[PHONE_MAX_SIZE];
} TLanguageInstruction;


/// The types of language phrases.
typedef enum TLanguagePhraseType {
  /// A geosearch result completely in a place.
  LANGUAGE_PHRASE_IN_PLACE,
  /// A geosearch result passing through a place.
  LANGUAGE_PHRASE_THRU_PLACE,
  /// A geosearch result near a place.
  LANGUAGE_PHRASE_NEAR_PLACE,
  /// A geosearch result completely in or located
  /// near a place (e.g. UK postal code).
  LANGUAGE_PHRASE_IN_OR_NEAR_PLACE,
  /// A geosearch result completely in of passing
  /// through a place (e.g. UK postal code).
  LANGUAGE_PHRASE_IN_OR_THRU_PLACE,
  /// A geosearch result passing through or located
  /// near a place (e.g. UK postal code).
  LANGUAGE_PHRASE_THRU_OR_NEAR_PLACE,
  /// A geosearch result completely in or passing
  /// through or located near a place (e.g. UK
  /// postal code).
  LANGUAGE_PHRASE_IN_OR_THRU_OR_NEAR_PLACE,
  /// The decimal point character.
  LANGUAGE_PHRASE_DECIMAL_POINT,
  /// The north cardinal letter (upper case).
  LANGUAGE_PHRASE_CARDINAL_N,
  /// The south cardinal letter (upper case).
  LANGUAGE_PHRASE_CARDINAL_S,
  /// The east cardinal letter (upper case).
  LANGUAGE_PHRASE_CARDINAL_E,
  /// The west cardinal letter (upper case).
  LANGUAGE_PHRASE_CARDINAL_W
} TLanguagePhraseType;

/// This structure holds the information needed for the language engine to be
/// able to accurately build a phrase as text.
typedef struct TLanguagePhrase {
  /// The type of language phrase.
  TLanguagePhraseType type;
  /// The number of positional arguments.
  UINT32              argc;
  /// The positional arguments of the phrase.
  const UTF16*const*  argv;
} TLanguagePhrase;


/// @}

#ifdef __cplusplus
}
#endif

#endif // LATYPES_H
