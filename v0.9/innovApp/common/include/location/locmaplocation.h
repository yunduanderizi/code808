#ifndef LOCMAPLOCATION_H
#define LOCMAPLOCATION_H

#include <innovEngine_types.h>
#include <language/ConvertUTF.h>
#include <mapdata/enroad.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOCATION_MAX_NAME_LENGTH (72 * 2)

#define LOCATION_MAX_MAP_NAME 64
  
/// @typedef enum TRoadSide
/// Enumerated list of sides of the road used by
/// journey_getLegDestinationSide() to return which side of the road the
/// destination point is on in relation to the driving direction.
typedef enum {
  SIDE_LEFT = -1,     //< destination on the left
  SIDE_UNKNOWN = 0,   //< destination is not on a particular side
  SIDE_RIGHT = 1      //< destination on the right
} TRoadSide;

//--------------------------------------------------------------------------------
/// @enum TMapLocationSource
/// @brief This enumeration contains symbolic values for the source
///        types to be stored in the map location structure.
/// NOTE: New values should ONLY be added at the end to ensure
///       on disk locations aren't affected
typedef enum TMapLocationSource {

  /// @brief This is reserved to indicate an invalid source.
  MAP_LOCATION_SOURCE_INVALID,
  
  /// @brief This indicates that a map location is initialised
  ///        from the map lock global state.
  MAP_LOCATION_SOURCE_MAP_LOCK,

  /// @brief This indicates that a map location is initialised
  ///        from raw GPS data
  MAP_LOCATION_SOURCE_RAW_GPS,

  /// @brief This indicates that a map location is initialised
  ///        from a snap
  MAP_LOCATION_SOURCE_SNAP,

  /// @brief This indicates that a map location is initialised
  ///        from a POI object. This is special due to additional
  ///        information regarding entry points
  MAP_LOCATION_SOURCE_POI,

  /// @brief This indicates that a map location is initialised
  ///        from a User POI object. 
  MAP_LOCATION_SOURCE_USER_POI,

  /// @brief This indicates that a map location is initialised
  ///        from a geosearch result.
  MAP_LOCATION_SOURCE_RESULT,

  /// @brief This indicates that a map location is initialised
  ///        from a place getting.
  MAP_LOCATION_SOURCE_GET_PLACE,

  // Must come last
  MAP_LOCATION_SOURCE_COUNT

} TMapLocationSource;


// A TMapLocation object should specify both a 
// user location which would represent a position on the map 
// that the user might have selected either via a search or by clicking
// on the screen or by loading from a favorite) as well as
// a network location (i.e. entry point) which corresponds to the location
// on the road network that needs to be used to arrive at the chosen
// user location.
//
// For a user location to be meaningful there is only one requirement, a 
// latitude and longitude. There might be other attributes associated with 
// the location but these are complementary.
//
// For the network location to be resolved

//Network related data associated with a 
//TMapLocation
typedef struct TMapLocationNetwork {

  // Location on the road network ( may not be the same as 
  // the user location)
  TPos    pos;

  // Orientation of the segment on the map (i.e. heading)
  INT32   orientation; 

  // Visible Road types at the point of location being resolved.
  TRoadType visibleRoadTypes;

  // Proportion along segment
  INT32    proportion;

  // Distance from the start node
  INT32   distToEndNode;

  // Primary Name of the road network
  UTF16     id_name[LOCATION_MAX_NAME_LENGTH];

  // Side of the segment when compared with the orientation
  // above (//TODO is this required)
  TRoadSide     sideOfLine;

  // Segment ID
  TSegmentID    segmentID;
  
  //-------------------------- UNUSED FOR NOW ---------------------------
  //Bounds for snapping Entry
  TRect     snapBounds;

} TMapLocationNetwork;

typedef struct TMapLocationMovement {

  // Driving heading along the segment 
  INT32   heading;  

  // Destination Node
  TNodeID   destNodeID;

} TMapLocationMovement;

typedef struct TMapLocation {

  // The source of this user location
  TMapLocationSource    source;

  UINT32 resultType;

  UINT32 magic;

  // Internal Object Identifier if a innovEngine object is known
  // Only used for POIs
  UINT32 _objectId;

  //--------------------------------------------------------------------
  // User Location lat/long
  TPos    user_pos;

  //--------------------------------------------------------------------
  // ** Additional attributes of this user location
  // ** can be saved and restored if needed. Only 
  // ** some of these need to be present 

  // Road Name/Place Name/POI name corresponding to 
  // this location (if any)
  UTF16   user_name[LOCATION_MAX_NAME_LENGTH];

  // House number (if applicable) of the location
  INT32   user_houseNumber;

  //--------------------------------------------------------------------
  // ** Additional details regarding the 'network'
  // ** location. These are calculated and cached if
  // ** possible

  // TRUE if the network location has been resolved
  BOOL8   _net_resolved;

  TMapLocationNetwork network;

  //--------------------------------------------------------------------
  // ** Additional details regarding any initial movement 
  // ** from this location
  
  // True if the movement data is valid
  BOOL8   mov_resolved;
 
  TMapLocationMovement movement; 

} TMapLocation;

// Structure to hold some persistent data related to a 
// map location. A TMapLocation can be degraded into one of these
// and can be re-constructed from one of these. 
// NOTE: The only time we should use this is when storing 
// data to a persistent storage media. 
typedef struct TMapLocationPersistData {

  // The integer representation of the TMapLocationSource for this
  // location
  UINT32 source;

  // Integer representation of the result type
  UINT32 resultType;
  
  // The name of this location
  UTF16 locationName[LOCATION_MAX_NAME_LENGTH];

  // The real world coordinates of the location
  TPos locationPos;

  //TODO
  UINT32 houseNumber;

  BOOL8 network_available;

  // Location Identity Name, this can be the POI name
  // if the location is a POI, the place name if it is 
  // a place etc.
  UTF16 locationIdentityName[LOCATION_MAX_NAME_LENGTH];

  // The location of this position on the road network
  TPos networkPos;

  // The orientation of the road segment for the entrance
  INT32 orientation;

  // Map Specific Data, should NEVER be created manually
  // only write these to disk if this structure was populated
  // via maplocation_getPersistData
  BOOL8 maphints_available;
  INT32   mapIndex;
  UINT16  mapVersion;
  UINT32  mapCRC;
  UTF16   mapName[LOCATION_MAX_MAP_NAME];
  INT32   objectOffset;
  INT32   prop;
  INT32   sol;

} TMapLocationPersistData;

typedef enum TMapLocationInitStatus {
  MAPLOC_INIT_ERROR           = -1, // Error Initialising the Map Location
  MAPLOC_INIT_ENGINE_INVALID  = 0,  // Cannot Initialise, try again after engine_init 
  MAPLOC_INIT_OK              = 1,  // Initialisation OK
  MAPLOC_INIT_OK_SNAPPED      = 2   // Initialisation OK, but a re-snap was required
} TMapLocationInitStatus;
  
BOOL8 maplocation_getPersistData( TMapLocation *mapLocation, 
                                  TMapLocationPersistData *persistentData );

TMapLocationInitStatus maplocation_initFromPersistData( TMapLocation *mapLocation, 
                                                        const TMapLocationPersistData *persistentData );

BOOL8 maplocation_isOnNetwork( TMapLocation *mapLocation );
BOOL8 maplocation_isMoving( TMapLocation *mapLocation );
BOOL8 maplocation_isValid( const TMapLocation *mapLocation ); 

void maplocation_init( TMapLocation *mapLocation );

void maplocation_initRandomLocation( TMapLocation *mapLocation );


void maplocation_initUserLocation( TMapLocation *mapLocation, 
                                   const TPos   *userPos,
                                   const UTF16  *roadName,
                                   UINT32        orientation,
                                   TRoadType     visibleRoadTypes );

void maplocation_initUserLocation2( TMapLocation      *mapLocation, 
                                    const TPos        *userPos,
                                    const UTF16       *roadName,
                                    UINT32             orientation,
                                    TRoadType          visibleRoadTypes,
                                    TMapLocationSource locationSource);

void maplocation_initUserLocationforSS( TMapLocation      *mapLocation, 
                                        const TPos        *userPos,
                                        const UTF16       *roadName,
                                        UINT32             orientation,
                                        TRoadType          visibleRoadTypes,
                                        UINT32             iRoadCode);

void maplocation_initUserPOI( TMapLocation  *mapLocation, 
                              const UTF16   *name,
                              const TPos    *pos );

BOOL8 maplocation_getLocationName( TMapLocation *mapLocation, 
                                   UTF16        *output,
                                   UINT16        len );

BOOL8 maplocation_setLocationName( TMapLocation *mapLocation, 
                                   const UTF16  *name );


BOOL8 maplocation_getNetworkRoadNames( TMapLocation *mapLocation, 
                                       UTF16 *name1Buffer,
                                       UINT32 name1BufferLen,
                                       UTF16 *name2Buffer,
                                       UINT32 name2BufferLen );

BOOL8 maplocation_getNetworkRoadName( TMapLocation *mapLocation,
                                      UTF16        *name,
                                      UINT32        bufferLen );

BOOL8 maplocation_getMovementHeading( TMapLocation *mapLocation, 
                                      INT32        *heading );

BOOL8 maplocation_getLocationPos( TMapLocation *mapLocation, 
                                  TPos         *pos );

BOOL8 maplocation_getNetworkPos( TMapLocation *mapLocation, 
                                 TPos         *pos );

BOOL8 maplocation_getMapCountryCode( TMapLocation *mapLocation, 
                                     TCountryCode *countryCode );

BOOL8 maplocation_getISOCountryCode( TMapLocation           *mapLocation, 
                                     TNumericISOCountryCode *isoCountryCode );


#ifdef __cplusplus
}
#endif

#endif //LOCMAPLOCATION_H
