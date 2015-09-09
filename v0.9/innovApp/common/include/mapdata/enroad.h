#ifndef ENROAD_H
#define ENROAD_H

#include <innovEngine_types.h>
#include <math/gerect.h>
#include <language/lacodepage.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup RoadFiles
/// This module provides a number fo API calls that allow the map data to be
/// accessed in a way that is reasonably independent of the underlying
/// map data format.  Typically the binary map data is unpacked into a C
/// structure where it can be more easily accessed.  Objects such as road
/// segments and road addresses need to be unpacked from the map in the
/// right sequence to ensure that all the data they contain is valid. 
/// @ingroup MapData 
/// @{ 

/** @note
    Node IDs and Segment IDs are just UINT32s. These are not used outside the
    core - but some routing types require them to be public  */

/** @typedef UINT32 TSegmentID
    A segment ID. */
typedef UINT32 TSegmentID;

/** @typedef UINT32 TNodeID
    A node ID. */
typedef UINT32 TNodeID;

/** @def ROAD_MAX_NAME_SIZE
    Defines the longest length name of any type that the engine can 
    produce.  It should be used to define the size of character buffers 
    that are used to contain road or place names. */
#define ROAD_MAX_NAME_SIZE 72

// The maximum number of components making a junction view pictorial.
#define MAX_JUNCTION_IMAGE_TYPES 4

// The maximum number of segments for a junction view path.
#define ROAD_MAX_JUNCTION_VIEW_SEGMENTS 15

/** @def ADDRESS_MAX_NAME_SIZE
    Defines the longest length address that the engine can 
    produce.  It should be used to define the size of character buffers 
    that are used to contain entire addresses. */
#define ADDRESS_MAX_NAME_SIZE 128

/** @def PLACE_TYPES
    This is the depth of the place name hierarchy */
#define PLACE_TYPES 7

/** @def PLACE_TYPE_COUNTRY
    Index of the place column corresponding to a country name */
#define PLACE_TYPE_COUNTRY 6

/** @def PLACE_TYPE_STATE
    Index of the place column corresponding to a state / principality name */
#define PLACE_TYPE_STATE 5

/** @def PLACE_TYPE_LAST_MAJOR
    Last index of the place column corresponding to a city / district name */
#define PLACE_TYPE_LAST_MAJOR 4

/** @def PLACE_TYPE_FIRST_MAJOR
    First index of the place column corresponding to a  city / district name */
#define PLACE_TYPE_FIRST_MAJOR 4

/** @def PLACE_TYPE_LAST_MINOR
    Last index of the place column corresponding to a town / suburb name */
#define PLACE_TYPE_LAST_MINOR 4

/** @def PLACE_TYPE_FIRST_MINOR
    First index of the place column corresponding to a town / suburb name */
#define PLACE_TYPE_FIRST_MINOR 0

/** @def PLACE_TYPE_BUA
    Index of the place column corresponding to a built up area name */
#define PLACE_TYPE_BUA 0

/** @typedef enum TRoadType
    These are the different road types that can be assigned to individual 
    segments */
typedef enum {
  /** invalid (eg: off-road) */
  ROAD_TYPE_NO_ROAD = -1,
  /** motorway (TA FUNCRDCL 10) */
  ROAD_TYPE_MOTORWAY = 0,
  /** national highway (TA FUNCRDCL 20) */
  ROAD_TYPE_NATIONAL_HIGHWAY,
  /** regional highway (TA FUNCRDCL 30) */
  ROAD_TYPE_REGIONAL_HIGHWAY,
  /** important major road (TA FUNCRDCL 40) */
  ROAD_TYPE_IMPORTANT_MAJOR,
  /** major road (TA FUNCRDCL 50,51,52) */
  ROAD_TYPE_MAJOR,
  /** motorway (TA FUNCRDCL 60,70) */
  ROAD_TYPE_MINOR,
  /** motorway (TA FUNCRDCL 80) */
  ROAD_TYPE_LANE,
  /** motorway (TA FUNCRDCL 99) */
  ROAD_TYPE_PEDESTRIAN,
  /** The number of road types available */

  ROAD_TYPES
} TRoadType;

/* Lane Direction Information */
typedef enum {
  ROAD_LANE_DIR_NONE            = 0,
  ROAD_LANE_DIR_STRAIGHT        = 1,
  ROAD_LANE_DIR_SLIGHT_RIGHT    = 2,
  ROAD_LANE_DIR_RIGHT           = 4,
  ROAD_LANE_DIR_SHARP_RIGHT     = 8,
  ROAD_LANE_DIR_LEFT_UTURN      = 16,
  ROAD_LANE_DIR_SHARP_LEFT      = 32,
  ROAD_LANE_DIR_LEFT            = 64,
  ROAD_LANE_DIR_SLIGHT_LEFT     = 128,
  ROAD_LANE_DIR_RIGHT_UTURN     = 256
} TRoadLaneDirType;

// Total number of valid lane directions
// ignoring ROAD_LANE_DIR_NONE
#define ROAD_LANE_DIR_COUNT 9

// Maximum number of lanes supported
#define ROAD_MAX_LANES  18

/** @typedef enum TRoadHandedness
    This structure is used to define the handedness of the road network
    Left-hand roads occur in Britain, Japan, Australia and New Zealand
    Right-hand roads occur in Europe and North America */
typedef enum {
  /** invalid (eg: off-road) */
  ROAD_HANDEDNESS_LEFT = -1,
  ROAD_HANDEDNESS_UNKNOWN = 0,
  ROAD_HANDEDNESS_RIGHT = 1
} TRoadHandedness;

/// @typedef enum TVehicleType
/// List of all vehicle types supported by routing. A journey may consist of
/// multiple legs, and each leg may be via a different vehicle type or mode of
/// transport.
typedef enum TVehicleType {
  //
  /// Passenger car
  VEHICLE_TYPE_PASSENGER_CAR = 0,
  //
  /// Pedestrian
  VEHICLE_TYPE_PEDESTRIAN,
  //
  /// The total number of vehicle types supported by routing.
  NUM_ROUTE_VEHICLE_TYPES

} TVehicleType;


/// Two letter country id, e.g., "NZ"
typedef struct TCountryCode {
  char ID[2];
} TCountryCode;

typedef struct TRoadInfo {
  UTF16 name1[ROAD_MAX_NAME_SIZE];
  UTF16 name2[ROAD_MAX_NAME_SIZE];
  UTF16 placeNames[PLACE_TYPES][ROAD_MAX_NAME_SIZE];
} TRoadInfo;

/* forward declaration */
struct TViewport;

/** @typedef struct TRoadSegment 
    This keeps the display subsystem happy as it uses some private types...
    Essentially this declaration becomes an opaque type declaration int the 
    eyes of the public. */

typedef struct TRoadSegment TRoadSegment;
typedef INT32  TRoadNetworkType;
typedef UINT32 TRoadMapLayer;


/// @brief Test if a point is inside the bounds of a loaded road map.
///
/// This is a crude test that can be used more cheaply than snapToRoad() to
/// determine if there may be any map data near a given location.
///
/// @param[in] latlong The point to test
/// @return TRUE or FALSE appropriately
BOOL8 road_posIsInMapBounds(TPos latlong);


/** scans all road maps and determines a bounding box that encloses the
    entire map area.
    
    @param bbox Points to a Trect structure that contains the bounding 
    box on exit.
    
    @return TRUE if the bounding box was calculated successfully. */
extern MAPCORE_API BOOL8 road_calcMapBoundingBox( TRect *bbox );

/// MPC-4746
/// @brief Get a road detail infomation by position.
///
/// @param[in] TViewport *viewport
/// @param[in] TPos *mapPos the position that user want to get road infomation
/// @param[out] TRoadDetails *roadDetails the road detail infomation struct
/// @return TRUE if get infomation success or return FALSE
extern MAPCORE_API BOOL8 road_getRodeInfobyPos( struct TViewport *viewport, TPos *mapPos, TRoadInfo *roadInfo );

/// Average Speed is always stored in km/h
typedef UINT8 TRoadSpeed;
///
/// @typedef enum TRoadSpeedUnit
/// This enumeration defines the unit of meaurement for road speed TRoadSpeed
/// (segment forward/backward speed restriction, not applicable to average 
///  speed; which is always noted as km/h)
///
typedef enum {
  /// km/h
  ROAD_SPEED_UNIT_KM_PER_HOUR,
  /// miles/h
  ROAD_SPEED_UNIT_MILES_PER_HOUR
} TRoadSpeedUnit;

///for road_getRouteNumberOrNameBySegmentIDOrPos function
typedef enum {
	///get road number and name
	ROAD_NUMBER_AND_NAME,
	///get road name only
	ROAD_ONLY_NAME
};

/// ISO 3166-1 three-digit numeric country codes. See:
/// http://en.wikipedia.org/wiki/ISO_3166-1
typedef enum {
  ISO_COUNTRY_CODE_ALBANIA        =   8, /* Albania */
  ISO_COUNTRY_CODE_ANDORRA        =  20, /* Andorra */
  ISO_COUNTRY_CODE_AUSTRALIA      =  36, /* Australia */
  ISO_COUNTRY_CODE_AUSTRIA        =  40, /* Austria */
  ISO_COUNTRY_CODE_BELGIUM        =  56, /* Belgium */
  ISO_COUNTRY_CODE_BRUNEI         =  96, /* Brunei */ //@TPE
  ISO_COUNTRY_CODE_CANADA         = 124, /* Canada */
  ISO_COUNTRY_CODE_CHINA          = 156, /* China */ //@TPE
  ISO_COUNTRY_CODE_CROATIA        = 191, /* Croatia */
  ISO_COUNTRY_CODE_CZECH_REPUBLIC = 203, /* Czech Republic */
  ISO_COUNTRY_CODE_DENMARK        = 208, /* Denmark */
  ISO_COUNTRY_CODE_FINLAND        = 246, /* Finland */
  ISO_COUNTRY_CODE_FRANCE         = 250, /* France */
  ISO_COUNTRY_CODE_GERMANY        = 276, /* Germany */
  ISO_COUNTRY_CODE_HUNGARY        = 348, /* Hungary */
  ISO_COUNTRY_CODE_INDONESIA      = 360, /* Indonesia */ //@TPE
  ISO_COUNTRY_CODE_IRELAND        = 372, /* Ireland */
  ISO_COUNTRY_CODE_ITALY          = 380, /* Italy */
  ISO_COUNTRY_CODE_LIECHTENSTEIN  = 438, /* Liechtenstein */
  ISO_COUNTRY_CODE_LUXEMBOURG     = 442, /* Luxembourg */
  ISO_COUNTRY_CODE_MALAYSIA       = 458, /* Malaysia */ //@TPE
  ISO_COUNTRY_CODE_MONACO         = 492, /* Monaco */
  ISO_COUNTRY_CODE_MOLDOVA        = 498, /* Moldova */
  ISO_COUNTRY_CODE_NETHERLANDS    = 528, /* Netherlands */
  ISO_COUNTRY_CODE_NEW_ZEALAND    = 554, /* New Zealand */
  ISO_COUNTRY_CODE_NORWAY         = 578, /* Norway */
  ISO_COUNTRY_CODE_POLAND         = 616, /* Poland */
  ISO_COUNTRY_CODE_PORTUGAL       = 620, /* Portugal */
  ISO_COUNTRY_CODE_RUSSIA         = 643, /* Russia */
  ISO_COUNTRY_CODE_SAN_MARINO     = 674, /* San Marino */
  ISO_COUNTRY_CODE_SERBIA         = 688, /* Serbia */
  ISO_COUNTRY_CODE_SINGAPORE      = 702, /* Singapore */ //@TPE
  ISO_COUNTRY_CODE_SLOVAKIA       = 703, /* Slovakia */
  ISO_COUNTRY_CODE_SPAIN          = 724, /* Spain */
  ISO_COUNTRY_CODE_SWEDEN         = 752, /* Sweden */
  ISO_COUNTRY_CODE_SWITZERLAND    = 756, /* Switzerland */
  ISO_COUNTRY_CODE_TAIWAN         = 158, /* Taiwan */ //@TPE
  ISO_COUNTRY_CODE_THAILAND       = 764, /* Thailand */ //@TPE
  ISO_COUNTRY_CODE_TURKEY         = 792, /* Turkey */
  ISO_COUNTRY_CODE_UKRAINE        = 804, /* Ukraine */
  ISO_COUNTRY_CODE_MACEDONIA      = 807, /* Macedonia */
  ISO_COUNTRY_CODE_UNITED_KINGDOM = 826, /* United Kingdom */
  ISO_COUNTRY_CODE_UNITED_STATES  = 840, /* United States */
  ISO_COUNTRY_CODE_VATICAN_CITY   = 336  /* Holy See (Vatican City State) */
} TNumericISOCountryCode;

/// MPC-4746
/// @brief Get country location and zoom level by country name.
///
/// @param[in] UTF16 *countryName
/// @param[out] TPos country location
/// @param[out] UINT32 zoomLevel the zoom level contain the country
/// @return TRUE if get infomation success or return FALSE
extern MAPCORE_API BOOL8 road_getCountryLocationbyName( TNumericISOCountryCode ISOCountryCode, TPos *center, UINT32 *zoomLevel, struct TViewport *viewport );

/// MPC-4746
/// @brief Get country name and ISO code by position.
///
/// @param[in] TPos *pos
/// @param[out] UTF16 *countryName
/// @return TNumericISOCountryCode
extern MAPCORE_API TNumericISOCountryCode road_getCountryInfobyPos( TPos *pos, UTF16 *countryname );

/// MPC-4746
/// @brief Get country location and zoom level by country code and state name.
///
/// @param[in] TNumericISOCountryCode ISOCountryCode
/// @param[in] const UTF16 *stateName
/// @param[out] TPos center			state location
/// @param[out] UINT32 zoomLevel	the zoom level contain the state
/// @param[in] TViewport viewport
/// @return TRUE if get infomation success or return FALSE   
extern MAPCORE_API BOOL8 road_getStateLocationbyName( TNumericISOCountryCode ISOCountryCode, const UTF16 *stateName, TPos *center, UINT32 *zoomLevel, struct TViewport *viewport );

/// @MIC Start
/// @brief Check if have postal code data in map data.
/// @param[in] TNumericISOCountryCode ISOCountryCode
/// @return TRUE if have postal code data or return FALSE
extern MAPCORE_API BOOL8 road_checkPocodeData(TNumericISOCountryCode ISOCountryCode);

/// @brief Get road info or place info by position and snap range
///
/// @param[in] TPos *fromPos snap position
/// @param[in] UINT32 snapRange the unit is meter
/// @param[out] UTF16 *roadInfo (road name + place name or place name only) the format will follow every country's address format
/// @param[in] UINT32 infoLen max length of roadInfo
/// @return TRUE if get infomation success or return FALSE   
extern MAPCORE_API BOOL8 road_getRoadInfobyPosandRange(TPos *fromPos, UINT32 snapRange, UTF16 *roadInfo, UINT32 infoLen);

/// @brief Get route number by segmentID or position 
///
/// @param[in] UINT32 segmentID the segmentID of the road. If the segmentID is unknow, it will fill with 0;
/// @param[in] TPos *roadPos the pos of the road
/// @param[out] UINT16 *routeType
/// @param[out] UTF16 *simpleRouteName the full route number (string type)
/// @param[out] UTF16 *fullRodeName the full route number (string type)
/// @param[in] UINT8 maxNameLen the max length of simpleRouteName and fullRoadName (string type)
/// @param[in] UINT8 nflag the full route number or only road name(ROAD_NUMBER_AND_NAME¡¢ROAD_ONLY_NAME)
/// @return route number(digit type) or return -1;    
extern MAPCORE_API BOOL8 road_getRouteNumberOrNameBySegmentIDOrPos(UINT32  segmentID, 
                                                             TPos   *roadPos, 
                                                             UINT16 *routeType, 
                                                             UTF16  *simpleRouteName, 
                                                             UTF16  *fullRodeName, 
                                                             UINT8   maxNameLen,
															 UINT8   nflag);
//@MIC End

/// @} 

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------*/

#endif
