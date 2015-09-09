#ifndef ENPOITYPE_H
#define ENPOITYPE_H

/* Public POI type file data structures and functions */

#include <innovEngine_types.h>
#include <language/lacodepage.h>
#include <containers/gebitset.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef UINT16 TPOITypeId16;

#define POITYPE_MAX_MAIN_ID 4096
#define POITYPE_MAX_NAME_LENGTH 72

#define POITYPE_BIT_COUNT (POITYPE_MAX_MAIN_ID + 1)

typedef struct TPOITypeFilter {
  BOOL8  initialized;
  UINT32 _count;
  UINT32 _enabledTypeBuffer[BITSET_WORD_COUNT(POITYPE_BIT_COUNT)];
} TPOITypeFilter;

typedef struct TPOITypeChild {
 UINT8             childCount;
 TPOITypeId16      childTypes[32];
} TPOITypeChild;

/// @brief These symbolic constants define the unique type IDs
///        that represent each of these listed types.
#define POITYPE_ID_NULL                           (0)
//NOTE keep the rest sorted alphabetically!
#define POITYPE_ID_ACCIDENT_BLACK_SPOT          (108)
#define POITYPE_ID_AIRLINE_ACCESS                (29)
#define POITYPE_ID_AIRPORT                       (47)
#define POITYPE_ID_AMUSEMENT_PARK                (79)
#define POITYPE_ID_ARTS_CENTRE                   (80)
#define POITYPE_ID_ATM                           (54)
#define POITYPE_ID_BANK                          (18)
#define POITYPE_ID_BEACH                         (55)
#define POITYPE_ID_BOWLING_CENTRE               (106)
#define POITYPE_ID_BREAKDOWN_SERVICE             (77)
#define POITYPE_ID_BUS_STATION                   (48)
#define POITYPE_ID_BUSINESS_SERVICE             (112)
#define POITYPE_ID_CAFE_OR_PUB                   (71)
#define POITYPE_ID_CAMPING_GROUND                (30)
#define POITYPE_ID_CAR_DEALER                    (84)
#define POITYPE_ID_CAR_RACETRACK                 (96)
#define POITYPE_ID_CAR_REPAIR_FACILITY            (1)
#define POITYPE_ID_CASINO                        (25)
#define POITYPE_ID_CASTLE                        (81)
#define POITYPE_ID_CEMETARY                      (99)
#define POITYPE_ID_CHURCH                        (82)
#define POITYPE_ID_CINEMA                        (26)
#define POITYPE_ID_CITY_CENTRE                   (45)
#define POITYPE_ID_CITY_HALL                     (14)
#define POITYPE_ID_COLLEGE_OR_UNIVERSITY         (44)
#define POITYPE_ID_COMMUNITY_CENTRE              (31)
#define POITYPE_ID_CONCERT_HALL                  (63)
#define POITYPE_ID_CONVENTION_CENTRE             (72)
#define POITYPE_ID_COURTHOUSE                    (60)
#define POITYPE_ID_CULTURAL_CENTRE               (10)
#define POITYPE_ID_DENTIST                       (69)
#define POITYPE_ID_DEPARTMENT_STORE              (17)
#define POITYPE_ID_DOCTOR                        (68)
#define POITYPE_ID_EMBASSY                       (32)
#define POITYPE_ID_EMERGENCY_CALL_STATION        (50)
#define POITYPE_ID_EMERGENCY_MEDICAL_SERVICE     (51)
#define POITYPE_ID_ENTERTAINMENT                 (78)
#define POITYPE_ID_EXHIBITION_CENTRE             (49)
#define POITYPE_ID_FERRY_TERMINAL                (28)
#define POITYPE_ID_FIRE_BRIGADE                  (52)
#define POITYPE_ID_FORTRESS                      (83)
#define POITYPE_ID_FRONTIER_CROSSING             (33)
#define POITYPE_ID_GENERAL_POI                   (76)
#define POITYPE_ID_GOLF_COURSE                   (85)
#define POITYPE_ID_GOVERNMENT_OFFICE             (34)
#define POITYPE_ID_HOLIDAY_AREA                  (86)
#define POITYPE_ID_HOSPITAL_OR_POLYCLINIC        (12)
#define POITYPE_ID_HOTEL_OR_MOTEL                 (5)
#define POITYPE_ID_ICE_SKATING_RINK              (57)
#define POITYPE_ID_IMPORTANT_TOURIST_ATTRACTION  (43)
#define POITYPE_ID_LIBRARY                       (87)
#define POITYPE_ID_LIGHTHOUSE                    (88)
#define POITYPE_ID_LEISURE_CENTRE                (73)
#define POITYPE_ID_MILITARY_CEMETARY             (89)
#define POITYPE_ID_MONUMENT                      (90)
#define POITYPE_ID_MOTORING_ORGANISATION_OFFICE  (35)
#define POITYPE_ID_MOUNTAIN_PASS                 (97)
#define POITYPE_ID_MOUNTAIN_PEAK                 (61)
#define POITYPE_ID_MUSEUM                         (8)
#define POITYPE_ID_MUSIC_CENTRE                  (67)
#define POITYPE_ID_NATURAL_RESERVE               (91)
#define POITYPE_ID_NIGHTLIFE                     (74)
#define POITYPE_ID_OPEN_PARKING_AREA             (36)
#define POITYPE_ID_OPERA                         (62)
#define POITYPE_ID_PARK_AND_RECREATIONAL_AREA    (59)
#define POITYPE_ID_PARK_AND_RIDE                (104)
#define POITYPE_ID_PARKING_GARAGE                 (4)
#define POITYPE_ID_PETROL_STATION                 (2)
#define POITYPE_ID_PHARMACY                      (16)
#define POITYPE_ID_PLACE_OF_WORSHIP              (24)
#define POITYPE_ID_POLICE_STATION                (13)
#define POITYPE_ID_POST_OFFICE                   (15)
#define POITYPE_ID_PUBLIC_PHONE                  (20)
#define POITYPE_ID_PUBLIC_RESTROOM              (105)
#define POITYPE_ID_PUBLIC_TRANSPORT_STOP         (98)
#define POITYPE_ID_RAILWAY_CROSSING             (111)
#define POITYPE_ID_RAILWAY_STATION               (46)
#define POITYPE_ID_RECREATION_FACILITY           (37)
#define POITYPE_ID_RENT_A_CAR_FACILITY            (3)
#define POITYPE_ID_RENT_A_CAR_PARKING            (95)
#define POITYPE_ID_REST_AREA                     (53)
#define POITYPE_ID_RESTAURANT                     (6)
#define POITYPE_ID_RESTAURANT_AREA               (56)
#define POITYPE_ID_ROAD_SIDE_DINER               (38)
#define POITYPE_ID_SAFETY_CAMERA                (100)
#define POITYPE_ID_SAFETY_CAMERA_AVERAGE        (101)
#define POITYPE_ID_SAFETY_CAMERA_MOBILE         (102)
#define POITYPE_ID_SAFETY_CAMERA_RED_LIGHT      (103)
#define POITYPE_ID_SAFETY_CAMERA_ERP            (117)  //for SEA
#define POITYPE_ID_SAFETY_CAMERA_TRAFFIC_VIDEO      (118)
#define POITYPE_ID_SAFETY_CAMERA_CN                 (330)  //for CN
#define POITYPE_ID_SAFETY_CAMERA_RADAR              (331)  //for CN
#define POITYPE_ID_SAFETY_CAMERA_CN_MOBILE          (332)  //for CN
#define POITYPE_ID_SAFETY_CAMERA_CN_RED_LIGHT       (333)  //for CN
#define POITYPE_ID_SAFETY_CAMERA_ONEWAY             (334)  //for CN
#define POITYPE_ID_SAFETY_CAMERA_TRAFFIC            (335)  //for CN
#define POITYPE_ID_SAFETY_CAMERA_FORBID             (336)  //for CN
#define POITYPE_ID_SAFETY_CAMERA_HIGHWAY_ENTRY_EXIT (337)  //for CN
#define POITYPE_ID_SAFETY_CAMERA_PUBLIC_BUS_ONLY    (338)  //for CN
#define POITYPE_ID_SAFETY_CAMERA_TURN_RESTRICT      (339)  //for CN
#define POITYPE_ID_SCENIC_OR_PANORAMIC_VIEW      (22)
#define POITYPE_ID_SCHOOL                        (39)
#define POITYPE_ID_SCHOOL_ZONE                  (110)
#define POITYPE_ID_SHOP                          (58)
#define POITYPE_ID_SHOPPING_CENTRE               (40)
#define POITYPE_ID_SKATING_RINK                  (65)
#define POITYPE_ID_SKI_LIFT_STATION              (21)
#define POITYPE_ID_SPEED_CHECK                  (109)
#define POITYPE_ID_SPORTS_CENTRE                 (11)
#define POITYPE_ID_SPORTS_HALL                   (92)
#define POITYPE_ID_STADIUM                       (41)
#define POITYPE_ID_SWIMMING_POOL                 (23)
#define POITYPE_ID_TENNIS_COURT                  (64)
#define POITYPE_ID_THEATRE                        (9)
#define POITYPE_ID_TOLL_GATE                     (42)
#define POITYPE_ID_TOURIST_INFORMATION_OFFICE     (7)
#define POITYPE_ID_TRAVEL_AGENCY                 (19)
#define POITYPE_ID_VETERINARIAN                  (70)
#define POITYPE_ID_WALKING_AREA                  (93)
#define POITYPE_ID_WATER_SPORT                   (66)
#define POITYPE_ID_WINERY                        (27)
#define POITYPE_ID_YACHT_BASIN                   (75)
#define POITYPE_ID_ZOO                           (94)

/// @brief The minimum POI type id that should be assigned
///        to user POI types.
#define POITYPE_MIN_USER_ID (0x8000)

typedef enum {
  REGION_GLOBAL,
  REGION_TW,
  REGION_CN
} TRegion;

//@MIC Start
extern MAPCORE_API
void poitype_setMaplayerFileName(const char *filename);

/// MPC-4746
/// @brief Get poi childTypeIDs by poi parentTypeID.
///
/// @param[in]  TPOITypeId16 parentTypeID
/// @param[in]  UINT8 mode 0-scan IG/IE/richPOI map, 1-scan IG map only, 2-scan IE map only, 3-scan richPOI map only 
/// @return TPOITypeChild the poi child type info of the special poi main type
extern MAPCORE_API
TPOITypeChild poitype_getSubtypeIDbyMainTypeID( TPOITypeId16 parentTypeID, UINT8 mode );

extern MAPCORE_API BOOL8 poitype_getAvailableTypeListbyRegion( TPOITypeFilter *typeList, TRegion region );
//@MIC End

extern MAPCORE_API BOOL8 poitype_getAvailableTypeList( TPOITypeFilter *typeList );

/// @addtogroup MapData
/// @{

extern MAPCORE_API INT32 poitype_getIconName( TPOITypeId16  typeId, 
                                              char         *iconname );

extern MAPCORE_API INT32 poitype_getIconNameWithVDCode( TPOITypeId16  typeId, 
                                                        char         *vendor,
                                                        char         *iconname );

extern MAPCORE_API INT32 poitype_getLocalisedTypeName( TPOITypeId16  typeId, 
                                                       UTF16        *output, 
                                                       UINT32        maxLength );

/// @}

#ifdef __cplusplus
}
#endif

/* ENPOITYPE_H */
#endif

