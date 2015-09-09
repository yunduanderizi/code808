#ifndef GEOFUZZY_API_H
#define GEOFUZZY_API_H

#include <innovEngine_types.h>
#include <language/utf16.h>
#include "geoFuzzyType.h"

#ifdef __cplusplus
extern "C" {
#endif

extern MAPCORE_API
BOOL8 addr_Open();

/**
 * MPC-4746
 * @brief find out the smart keyboard idx of a state
 * @param[in] stateName the state that user want to search
 * @return TRUE if success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_SearchSKMap(UTF16 *stateName);

/**
 * MPC-4746
 * @brief use smart keyboard search
 * @param[in] in_SearchCondition the parameter o smart keyboard search
 * @param[out] out_SearchResult the result of smart keyboard search 
 * @return TRUE if success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_SearchSmartKeboard (const APIS_SEARCH_CONDITION_IN &in_SearchCondition,
                               APIS_SEARCH_RESULT_OUT &out_SearchResult);

/**
 * MPC-4746
 * @brief move to the first result
 * @param[out] _info the infomation of the city
 * @return TRUE if success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_MoveCityFirst(APIS_ADDR_ITEM_CITY_DISTRICT &_info);

/**
 * MPC-4746
 * @brief move to the next result
 * @param[out] _info the infomation of the city
 * @return TRUE if success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_MoveCityNext(APIS_ADDR_ITEM_CITY_DISTRICT &_info);

/**
 * MPC-4746
 * @brief move to the first result
 * @param[out] _info the infomation of the road
 * @return TRUE if success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_MoveRoadFirst(APIS_ADDR_ITEM_ROAD &_info);

/**
 * MPC-4746
 * @brief move to the next result
 * @param[out] _info the infomation of the road
 * @return TRUE if success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_MoveRoadNext(APIS_ADDR_ITEM_ROAD &_info);

/**
 * MPC-4746
 * @brief get all smart search road result
 * @param[out] _result the all infomation of the result road
 * @return the count of resultSet
 */
extern MAPCORE_API
UINT16 addr_GetRoadResultSet(SMART_ROAD_RESULT_SET &resultSet);

/**
 * MPC-4746
 * @brief check the smart keyboard search index file is ready
 * @return TRUE if file is ready or return FALSE
 */
extern MAPCORE_API 
BOOL8 addr_CheckSKIdxFile();

/**
 * MPC-4746
 * @brief Close smart keyboard search file
 * @return TRUE if close file success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_Close();

/**
 * MPC-4746
 * @brief Set the smart keyboard search config
 * @return TRUE if set config success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_SetBaseConfig(APIS_ADDR_SEARCH_BASE_CONFIG_IN &in_BaseConfig);

/**
 * @brief List all city/county name within a state or all district name within a city/county
 * @param[in] stateName that search within which state
 * @param[in] parentAreaName that list city or district within which state or city
 * @param[in] getAreaMode that list city or distrcit name mode
 * @param[out] areaList the result list of city or district name
 * @return TRUE if success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_GetAreaNameList(UTF16                        *stateName,
                           UTF16                        *parentAreaName,
                           TGeofuzzyAreaListMode         getAreaMode,
                           struct TGeofuzzyAreaNameList *areaList);

/**
 * MPC-4746
 * @brief Start to search house number
 * @return TRUE if search success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_SearchHouseNumber(APIS_HOUSE_PARAMETER &parameter, 
                             APIS_HOUSE_RESULT    &result);

/**
 * MPC-4746
 * @brief Use smart keyboard search to search address from voice command
 * @return TRUE if search success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_VoiceCommandSmartKeyboardSearch(const UTF16 *queryString, 
                                           SMART_ADDRESS_SET *addrSet);

/**
 * MPC-4746
 * @brief check the address data(lane, alley, house number) on the road
 * @return TRUE if has address data or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_CheckAddressData(APIS_HOUSE_PARAMETER &parameter); 

/**
 * MPC-4746
 * @brief get the address data(lane, alley, house number) on the road
 * @if have no input and the anyInput will be set to FALSE
 * @return TRUE if get address data success or return FALSE
//
//four format address in current mapdata =>
//1. road + lane + alley + house number
//2. road + lane         + house number
//3. road        + alley + house number
//4. road                + house number
//=======================================================================
//this API will support four input content =>
//case 1:
//xx road (return lane or alley data and the house number on the road)
//
//case 2:
//xx road xx lane (return alley data and the house number on the lane)   
//
//case 3:
//xx road xx lane xx alley (return the house number on the alley)
//
//case 4:
//xx road xx alley (return the house number on the alley)
//
 */
extern MAPCORE_API
BOOL8 addr_GetRoadAddressData(APIS_HOUSE_PARAMETER &parameter, 
                              APIS_ADDRESS_DATA &result, 
                              BOOL8 anyInput); 

/**
 * MPC-4746
 * @brief find out the point house number idx of a state
 * @param[in] stateName the state that user want to search
 * @return TRUE if success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_SearchHNMap(UTF16 *stateName);

extern MAPCORE_API
void addr_SetHNRoadMapIdx(UINT32 mapIdx);

/**
 * @this api just for atf, now.
 * @brief get the address data count(lane, alley, house number) on the road
 * @If we want to get the data after get the count, we can use the addr_GetSingleAddressData API to get
 * @lane/alley/house number data.
 * @param[in] parameter the address data that user wnat to get
 * @param[in] charData True get character data only or FALSE get digit and character data
 * @param[out] result the address data count
 * @return TRUE if get address data success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_GetAddressDataCount(APIS_HOUSE_PARAMETER    &parameter, 
                               BOOL8                    charData,
                               UINT16                  *laneCount,
                               UINT16                  *alleyCount,
                               UINT16                  *houseCount);

/**
 * @this api just atf, now.
 * @brief get house number data
 * @param[in] count the count of the address data that want to get 
 * @param[in] dataType: 1 - lane , 2 - alley , 3 - house number
 * @param[out] data[] the data container that store result data
 * @return TRUE if get data success or return FALSE
 & @if count == 1 also return FALSE
 */
extern MAPCORE_API
BOOL8 addr_GetAddressData(UINT16                        count,  
                          UINT8                         dataType,
                          TGeofuzzyDynamicAddressData   data[]);

/**
 * @brief get house number data 
 * @param[in] roadMapIdx: A road map index.
 * @param[in] roadSegmentID: A road segmentID.
 * @param[in] searchBounds: A bounding box in map coordinates to be used to filter results.
 * @param[out] results: vector container that store result data
 * @return TRUE if get data success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_getHouseNumberItems(const UINT32 roadMapIdx,
                                const UINT32 roadSegmentID,
                                const struct TRect searchBounds,
                                THOUSEITEMLIST &results);

/**
 * @brief release house number data 
 * @param[in] items: vector container that be released. 
 * @return TRUE if get data success or return FALSE
 */
extern MAPCORE_API
BOOL8 addr_freeHouseNumberItems(THOUSEITEMLIST &items);

/**
 * @brief Get the first item in a list.
 * @param[in] items: Points to the list.
 * @return the first item in the doubly linked list (or NULL if none exists).
 */
extern MAPCORE_API
const THOUSEITEM* addr_getHouseItemListBegin(const THOUSEITEMLIST &items);


/**
 * @brief Get the last item in a list.
 * @param[in] items: Points to the list.
 * @return the last item in the doubly linked list (or NULL if none exists).
 */
extern MAPCORE_API
const THOUSEITEM* addr_getHouseItemListEnd(const THOUSEITEMLIST &items);

/**
 * @brief Get the next item.
 * @param[in] items: Points to the current item.
 * @return the next item in the doubly linked list (or NULL if none exists).
 */
extern MAPCORE_API
const THOUSEITEM* addr_getHouseItemNext(const THOUSEITEM* item);


/**
 * @brief Get the previous item.
 * @param[in] items: Points to the current item.
 * @return the previous item in the doubly linked list (or NULL if none exists).
 */
extern MAPCORE_API
const THOUSEITEM* addr_getHouseItemPrevious(const THOUSEITEM* item);

#ifdef __cplusplus
}
#endif

#endif
