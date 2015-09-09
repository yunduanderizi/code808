#include "stdio.h"
#include "stdlib.h"
#include "interface_search.h"
#include "engineinterface.h"
#include "navcoreRoute.h"
#include <mapdata/enpoiP.h> //@MIC
#include <math/enworld.h>
#include "navcoreFavourites.h"
#include "interface_logistics.h"
#include "interface_route.h"

TGeosearchObjectHandle g_objHandle;
TGeosearchQueryHandle g_queryHandle;
structRealLevel g_structRealLevel={0,0,0,0,0,0,0,0,0,0,0,0};
START_SEARCH_TYPE g_searchType = SCH_DEFAUT_SEARCH_TYPE;
AROUND_SCH_POI_TYPE g_aroundPoiPid = SCH_AROUND_POI_TYPE_DEFAULT;
CATEGORY_POI_TYPE g_categoryPoiPid = SCH_CATEGORY_POI_TYPE_DEFAULT;
TGeosearchResultHandle g_resultHandles[NUM_SEARCH_RESULT];
MiddlePOIResult g_middleResultArray[NUM_SEARCH_RESULT];
int g_searchResultSelectIdx = 0;

structAdminIdx g_structAdminIdx={0,0};
int g_adminTmpidx;
WCHAR g_arrayProvince[STR_LEN];
WCHAR g_arrayTmpProvince[STR_LEN];
WCHAR g_arrayCity[STR_LEN];
WCHAR g_arrayRoad[STR_LEN];

UINT32 g_resultCount;
UINT32 g_poiResultCount;

static bool IsValidForSearch(int index)
{
    TGeosearchResultType type = geosearchResult_getType(g_resultHandles[index]);

    return type != GEOSEARCH_RESULT_TYPE_INTERSECTION && type != GEOSEARCH_RESULT_TYPE_HOUSE_NUMBER
            && type != GEOSEARCH_RESULT_TYPE_POI && type != GEOSEARCH_RESULT_TYPE_USER_POI
            && type != GEOSEARCH_RESULT_TYPE_POI_TYPE && type != GEOSEARCH_RESULT_TYPE_USER_POI_TYPE
            && type != GEOSEARCH_RESULT_TYPE_LAT_LONG;
}

static bool getSearchRetTextLocName(int nIdx,structDetailInfo *pStructInfo)
{
    if(NULL == pStructInfo)
    {
        //Log
        return false;
    }

    TGeosearchResultHandle * pResultHandleItem = (TGeosearchResultHandle *)(g_resultHandles + nIdx);
    if(NULL == pResultHandleItem)
    {
        //log
        return false;
    }

    TGeosearchTextHandle textHandle;
    geosearchResult_getLocationName(*pResultHandleItem, &textHandle);
    geosearchText_getString(textHandle, STR_LEN, pStructInfo->name);
    geosearchObject_release(textHandle);

    return true;
}

static void getSelectResultName(int idx, WCHAR* str)
{
    TGeosearchTextHandle textHandle;                         // retrieve the text of the selected item
    geosearchResult_getName(g_resultHandles[idx], &textHandle);
    geosearchText_getString(textHandle, STR_LEN, str);
    geosearchObject_release(textHandle);
}

static void setRealType(int mask)
{
    g_structRealLevel.country = (mask & 0x01) ? 1 : 0;
    g_structRealLevel.province =  (mask & 0x01 << 1) ? 1 : 0;
    g_structRealLevel.city =  (mask & 0x01 << 2) ? 1 : 0;
    g_structRealLevel.subCity =  (mask & 0x01 << 3) ? 1 : 0;
    g_structRealLevel.postcode =  (mask & 0x01 << 4) ? 1 : 0;
    g_structRealLevel.dPostcode =  (mask & 0x01 << 5) ? 1 : 0;
    g_structRealLevel.road =  (mask & 0x01 << 6) ? 1 : 0;
    g_structRealLevel.houseNo =  (mask & 0x01 << 7) ? 1 : 0;//
    g_structRealLevel.intersection =  (mask & 0x01 << 8) ? 1 : 0;//
    g_structRealLevel.poi =  (mask & 0x01 << 9) ? 1 : 0;
    g_structRealLevel.poiType = (mask & 0x01 << 10) ? 1 : 0;
    g_structRealLevel.lat_lon = (mask & 0x01 << 11) ? 1 : 0;
}

static int MakeParams(TGeosearchObjectHandle objHandle, TGeosearchParametersHandle * params, const WCHAR * searchText)
{
    int count = 0;
    if(g_structRealLevel.country)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_COUNTRY, searchText, objHandle, params) ) {
            count++;
        }else {
            g_structRealLevel.country = FALSE;
        }
    }
    if(g_structRealLevel.province)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_STATE, searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.province = FALSE;
        }
    }
    if(g_structRealLevel.city)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_PLACE, searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.city = FALSE;
        }
    }
    if(g_structRealLevel.subCity)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_PLACE_SUB, searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.subCity = FALSE;
        }
    }
    if(g_structRealLevel.postcode)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_POSTAL_CODE, searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.postcode = FALSE;
        }
    }
    if(g_structRealLevel.dPostcode)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_DETAILED_POSTAL_CODE,
                                            searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.dPostcode = FALSE;
        }
    }
    if(g_structRealLevel.road)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_ROAD, searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.road = FALSE;
        }
    }
    if(g_structRealLevel.houseNo)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_HOUSE_NUMBER, searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.houseNo = FALSE;
        }
    }
    if(g_structRealLevel.intersection)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_INTERSECTION, searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.intersection = FALSE;
        }
    }
    if(g_structRealLevel.poi)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_POI, searchText, objHandle, params + count) ) {
            count++;
            //@MIC Start
            g_poiResultCount = 0;
            memset(g_middleResultArray, 0, sizeof(MiddlePOIResult)*NUM_SEARCH_RESULT); //NUM_SEARCH_RESULT
            //@MIC End
        }else {
            g_structRealLevel.poi = FALSE;
        }
    }
    if(g_structRealLevel.poiType)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_POI_TYPE, searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.poiType = FALSE;
        }
    }
    if(g_structRealLevel.lat_lon)
    {
        if ( geosearch_makeSearchParameters(GEOSEARCH_RESULT_TYPE_LAT_LONG, searchText, objHandle, params + count) ) {
            count++;
        }else {
            g_structRealLevel.lat_lon = FALSE;
        }
    }
    return count;
}

void searchInit()
{
    geosearch_makeWorldSearchLocation(NULL, &g_objHandle);     // 1st time set to default
    g_queryHandle = geosearch_getInvalidQueryHandle();
    g_resultCount = 0;
}

void searchReset()
{
    geosearchObject_release(g_objHandle);
    geosearch_makeWorldSearchLocation(NULL, &g_objHandle);     // 1st time set to default
}

void searchSetLevel(SEARCH_LEVEL type)
{
    switch (type)
    {
    case SCH_PROVINCE_LEVEL:
        setRealType(2);
        break;
    case SCH_CITY_LEVEL:
        setRealType(4);
        break;
    case SCH_NO_SEARCH_LEVEL:
        setRealType(0);
        break;
    case SCH_POI_LEVEL:
        setRealType( 0x01 << 9);
        break;
    case SCH_ROAD_LEVEL:
        setRealType(0x01 << 6);
        break;
    case SCH_CROSS_LOAD_LEVEL:
        setRealType(0x01 << 8);
        break;
    case SCH_POI_TYPE_LEVEL:
        setRealType(0x01 << 10);
    default:
        break;
    }
}

void searchSetType(START_SEARCH_TYPE enumSearchType)
{
    g_searchType = enumSearchType;
}

void searchCreateQuery(const WCHAR *searchText)
{
    TGeosearchParametersHandle params[NUM_PARAMS];
    int numParams = 1;
    TRect searchBounds;
    TPos tpos;
    INT32 heading = 0;
    routeGetCurrentLocation(tpos,heading);

    switch(g_searchType)
    {
        case SCH_POI_AROUND_SEARCH_TYPE:
        {
            geosearch_initSearchBounds(&searchBounds, &tpos, NEAR_POI_SCH_SIZE);
            VERIFY(geosearch_makeWorldSearchLocation(&searchBounds, &g_objHandle));
            VERIFY(geosearch_makePOISearchParameters(GEOSEARCH_RESULT_TYPE_NEAREST_POI, \
                   searchText, g_objHandle, g_aroundPoiPid, NEAR_POI_SCH_SIZE, 0, params));
            break;

        }
        case SCH_POI_CATEGORY_SEARCH_TYPE:
        {
           VERIFY(geosearch_makePOISearchParameters(GEOSEARCH_RESULT_TYPE_POI, searchText, \
                   g_objHandle, g_categoryPoiPid, 0, 0, params));
            break;
        }
        case SCH_DEFAUT_SEARCH_TYPE:
        {
            searchSetArondPoiPid(SCH_AROUND_POI_TYPE_DEFAULT);
            searchsetCategoryPoiPid(SCH_CATEGORY_POI_TYPE_DEFAULT);
            numParams = MakeParams(g_objHandle, params, searchText);
            break;
        }
        default:
            break;
    }

    if(numParams > 0)
    {
        geosearchObject_release(g_queryHandle);
        geosearch_makeSearchQuery(params, numParams, NUM_SEARCH_RESULT, &g_queryHandle); //NUM_SEARCH_RESULT

        for(int i = 0; i < numParams; i++)
        {
            geosearchObject_release(params[i]);
        }
    }
}

void searchExecuteQuery()
{
    UINT32 uQueryResultEnd = 0;
    UINT32 uQueryExeCount = 0;
    UINT32 uMiddRetCount = 0;

    for(;(uQueryResultEnd = geosearchQuery_execute(g_queryHandle)) > 0;)
    {
        uQueryExeCount ++;
        uMiddRetCount = GetMiddleResultCount(g_queryHandle);
        if(g_searchType == SCH_POI_CATEGORY_SEARCH_TYPE && (uQueryExeCount >= 1000 || uMiddRetCount >= 50))
        {
            printf("  searchFunc uQueryExeCount = %d,uMiddRetCount = %d\n" ,uQueryExeCount,uMiddRetCount);
            break;
        }
    }

    if(uQueryResultEnd == 0 || uMiddRetCount > 0)
    {
        if(uQueryResultEnd == 0)
        {
            for(UINT32 i = 0; i < g_resultCount; i++)                // clean everything before use
            {
                geosearchObject_release(g_resultHandles[i]);
            }
            g_resultCount = geosearchQuery_getResults(g_queryHandle, g_resultHandles);
            //qDebug() << "searchFunc m_resultCount: @" << g_resultCount;
            printf("  -- searchFunc m_resultCount: %d\n" ,g_resultCount);
        }

        if(g_structRealLevel.poi == true)
        {
            if(GetMiddleResultCount(g_queryHandle) > 0)
            {
                g_poiResultCount = 0;
                if(uMiddRetCount > 0)
                {
                    for(UINT32 i = 0; i < uMiddRetCount; i++)
                    {
                        MiddlePOIResult middleResult = GetPOIMiddleResultforPOISP(g_queryHandle, i);
                        g_middleResultArray[g_poiResultCount] = middleResult;
                        g_poiResultCount++;
                    }
                    //qDebug() << "searchFunc MiddlePOIResult: @" << g_poiResultCount;
                    printf("  -- searchFunc MiddlePOIResult: %d\n" ,g_poiResultCount);
                }
            }
        }
    }
}

void searchSetSelectIndex(int idx)
{
    g_searchResultSelectIdx = idx;
}

int searchGetSelectIndex()
{
    return g_searchResultSelectIdx;
}

void searchSelectObjHandleInSearchResult(int nSecletIdx)
{
    geosearchObject_release(g_objHandle);
    g_objHandle = g_resultHandles[nSecletIdx];
    geosearchObject_addRef(g_objHandle);
}

void searchInitProvince()
{
    searchReset();
    searchSetLevel(SCH_PROVINCE_LEVEL);
    searchSetType(SCH_DEFAUT_SEARCH_TYPE);
    searchCreateQuery((WCHAR*)NULL);
    searchExecuteQuery();
}

void searchInitCity()
{
    searchSetLevel(SCH_CITY_LEVEL);
    searchSetType(SCH_DEFAUT_SEARCH_TYPE);
    searchCreateQuery((WCHAR*)NULL);
    searchExecuteQuery();
}

void searchSelectObjHandleInAdmin()
{
    int nProvinceIdx = g_structAdminIdx.nProvinceIdx;
    int nCityIdx = g_structAdminIdx.nCityIdx;

    searchSetType(SCH_DEFAUT_SEARCH_TYPE);
    WCHAR searchText[STR_LEN];
    memset(searchText, 0, sizeof(searchText));

    searchSetLevel(SCH_PROVINCE_LEVEL);
    searchCreateQuery(searchText);
    searchExecuteQuery();
    getSelectResultName(nProvinceIdx, g_arrayProvince);
    searchSelectObjHandleInSearchResult(nProvinceIdx);

    searchSetLevel(SCH_CITY_LEVEL);
    searchCreateQuery(searchText);
    searchExecuteQuery();
    getSelectResultName(nCityIdx, g_arrayCity);
    searchSelectObjHandleInSearchResult(nCityIdx);
}

void searchSetAdminIndex(int nProvinceIdx, int nCityIdx)
{
    g_structAdminIdx.nCityIdx = nCityIdx;
    g_structAdminIdx.nProvinceIdx = nProvinceIdx;
}

void searchSetProvinceIdx(int idx)
{
    g_structAdminIdx.nProvinceIdx = idx;
}

void searchSetTmpProvinceIdx(int idx)
{
    g_adminTmpidx = idx;
}

void searchsetCityIdx(int idx)
{
    searchSetProvinceIdx(g_adminTmpidx);
    memcpy(g_arrayProvince, g_arrayTmpProvince, sizeof(g_arrayProvince));
    g_structAdminIdx.nCityIdx = idx;
}

void searchSetProvinceName()
{
    getSelectResultName(g_structAdminIdx.nProvinceIdx, g_arrayProvince);
}

void searchSetTmpProvinceName()
{
    getSelectResultName(g_adminTmpidx, g_arrayTmpProvince);
}

const WCHAR* searchGetProvinceName()
{
    return g_arrayProvince;
}

const WCHAR* searchGetTmpProvinceName()
{
    return g_arrayTmpProvince;
}

void searchSetCityName()
{
    getSelectResultName(g_structAdminIdx.nCityIdx, g_arrayCity);
}

const WCHAR* searchGetCityName()
{
    return g_arrayCity;
}

void searchSetArondPoiPid(AROUND_SCH_POI_TYPE poitypeid)
{
    g_aroundPoiPid = poitypeid;
}

void searchsetCategoryPoiPid(CATEGORY_POI_TYPE nPoiPid)
{
    g_categoryPoiPid = nPoiPid;
}

void searchSetRoadName(int idx)
{
    getSelectResultName(idx, g_arrayRoad);
}

const WCHAR* searchGetRoadName()
{
    return g_arrayRoad;
}

bool searchGetMidPoiRetInfo(int nIdx,structDetailInfo *pStructInfo)
{
    if(NULL == pStructInfo)
    {
        //Log
        return false;
    }

    MiddlePOIResult *pMidResult = (MiddlePOIResult *)(g_middleResultArray + nIdx);
    if(pMidResult == NULL)
    {
        //Log
        return false;
    }

    TPos pos;
    UINT32 poiID;
    TPos PosCurGpsloc;
    INT32 nHeading;
    int poitypeid = 0;
    TMapLocation mapLocation;

    GetResultNameforPOISP(pStructInfo->name, pMidResult->nameIdx, pMidResult->mapIdx);
    poitypeid = GetPOITypeIdforPOISP(pMidResult->typeSetIndex, pMidResult->mapIdx);
    poiID = GetPOIID(pMidResult->mapIdx, pMidResult->sectorID, pMidResult->nameIdx);
    GetPosforPOISP(&pos, poiID, pMidResult->mapIdx);
    GetMapLocationforPOISP(&mapLocation, poiID, pMidResult->mapIdx, pMidResult->nameIdx);
    routeGetCurrentLocation(PosCurGpsloc,nHeading);

    pStructInfo->uDistance = World_distance(&PosCurGpsloc,&pos);
    GetPOIInfoByIDandMapIdx(poiID,pMidResult->mapIdx,
                            pStructInfo->address,STR_LEN,
                            pStructInfo->tel,TLE_LEN);

//        qDebug() << "Name : " << QString::fromUtf16(pStructInfo->name) << "POIId : " << poitypeid;
    return true;
}

bool searchGetRetTextName(int nIdx,structDetailInfo *pStructInfo)
{
    if(NULL == pStructInfo)
    {
        //Log
        return false;
    }

    TGeosearchResultHandle * pResultHandleItem = (TGeosearchResultHandle *)(g_resultHandles + nIdx);
    if(NULL == pResultHandleItem)
    {
        //log
        return false;
    }

    TGeosearchTextHandle textHandle;
//    TMapLocation test;
//    geosearchResult_getMapLocation(*pResultHandleItem, &test);
    geosearchResult_getName(*pResultHandleItem, &textHandle);
    geosearchText_getString(textHandle, STR_LEN, pStructInfo->name);
    geosearchObject_release(textHandle);

    return true;
}

int searchGetResultCount()
{
    return g_resultCount;
}

int searchGetPoiResultCount()
{
    return g_poiResultCount;
}

void searchResetPoiResultCount()
{
    g_poiResultCount = 0;
}

bool searchResultGuidePOI()
{
    TPos pos;
    UINT32 poiID;
    TMapLocation mapLoc;
    poiID = GetPOIID(g_middleResultArray[g_searchResultSelectIdx].mapIdx,
                     g_middleResultArray[g_searchResultSelectIdx].sectorID,
                     g_middleResultArray[g_searchResultSelectIdx].nameIdx);

    GetMapLocationforPOISP(&mapLoc,
                           poiID,
                           g_middleResultArray[g_searchResultSelectIdx].mapIdx,
                           g_middleResultArray[g_searchResultSelectIdx].nameIdx);

    if(doRouteEnd(pos, &mapLoc))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool searchResultGuideGeo()
{
    TMapLocation mapLoc;
    TPos pos;
    geosearchResult_getMapLocation(g_resultHandles[g_searchResultSelectIdx], &mapLoc);
    if(doRouteEnd(pos, &mapLoc))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void searchClearFavourites()
{
    g_navFavourites->clearAll();
}

static TGeosearchResultType searchGetCurrPoiInfoGeo(int selidx, TRect *box, TPos *pos, TMapLocation *mapLoc)
{
    TGeosearchResultType searchResultType;
    searchResultType = geosearchResult_getType(g_resultHandles[selidx]);
    geosearchResult_getMapLocation(g_resultHandles[selidx], mapLoc);
    maplocation_getNetworkPos(mapLoc, pos);
    geosearch_initSearchBounds(box, pos, MIN_MAP_SIZE);
    return searchResultType;
}

static TGeosearchResultType searchGetCurrPoiInfoPoi(int selidx, TRect *box, TPos *pos, TMapLocation *mapLoc)
{
    TGeosearchResultType searchResultType;
    searchResultType = GEOSEARCH_RESULT_TYPE_POI;

    UINT32 foundID = GetPOIID(g_middleResultArray[selidx].mapIdx,
            g_middleResultArray[selidx].sectorID,
            g_middleResultArray[selidx].nameIdx);
    if(foundID <= 0){
        return GEOSEARCH_RESULT_TYPE_POI;
    }
    TPOI foundPOI;
    TGenericMapInfo *mapInfo = mapinfo_resolveMapIndex(MAPTYPE_POI, g_middleResultArray[selidx].mapIdx);
    foundID += mapInfo->mapStartVa;
    poi_unpackCompletePOI( &foundPOI, NULL, foundID );
    poi_getMapLocation( &foundPOI, mapLoc );
    maplocation_getNetworkPos(mapLoc, pos);
    geosearch_initSearchBounds(box, &mapLoc->user_pos/*&pos*/, MIN_MAP_SIZE);
    *pos = mapLoc->user_pos;
}

static void searchShowPoiIcon(TRect bBox, TPos pos ,TMapLocation mapLoc, TGeosearchResultType searchResultType)
{
    switch(searchResultType)
    {
    case GEOSEARCH_RESULT_TYPE_ROAD:
        g_navFavourites->add(navcoreFavourites::TYPE_ROAD, &bBox, &pos, MIN_MAP_SIZE);
        break;
    case GEOSEARCH_RESULT_TYPE_PLACE:
        g_navFavourites->add(navcoreFavourites::TYPE_PLACE, &bBox, &pos, MIN_MAP_SIZE);
        break;
    case GEOSEARCH_RESULT_TYPE_POSTAL_CODE:
        g_navFavourites->add(navcoreFavourites::TYPE_POSTCODE, &bBox, &pos, MIN_MAP_SIZE);
        break;
    case GEOSEARCH_RESULT_TYPE_POI:
        g_navFavourites->add(navcoreFavourites::ICON_TYPE_SEARCH_RESULT, &bBox, &mapLoc.user_pos/*&pos*/, MIN_MAP_SIZE);
        break;
    case GEOSEARCH_RESULT_TYPE_COUNTRY:
    case GEOSEARCH_RESULT_TYPE_STATE:
    case GEOSEARCH_RESULT_TYPE_POI_TYPE:
        break;
    default:
        g_navFavourites->add(navcoreFavourites::TYPE_ROAD, &bBox, &pos, MIN_MAP_SIZE);
        break;
    }
    g_navDisplay->gotoMap(pos , 250); //refresh in gotoMap();
}

void searchShowMapPoi()
{
    if(g_navFavourites == NULL) {return; }
    TMapLocation mapLoc;
    TGeosearchResultType searchResultType;
    int selIndex = 0;                                       // map location of a result
    TPos pos;                                               // position of a map location
    TRect bBox;                                             // bounding box of a map location
    selIndex = g_searchResultSelectIdx;

    searchResultType = searchGetCurrPoiInfoPoi(selIndex, &bBox, &pos ,&mapLoc);
    searchShowPoiIcon(bBox, pos, mapLoc, searchResultType);
}

void searchShowMapGeo()
{
    if(g_navFavourites == NULL) {return; }
    TMapLocation mapLoc;
    TGeosearchResultType searchResultType;
    int selIndex = 0;                                       // map location of a result
    TPos pos;                                               // position of a map location
    TRect bBox;                                             // bounding box of a map location
    selIndex = g_searchResultSelectIdx;

    searchResultType = searchGetCurrPoiInfoGeo(selIndex, &bBox, &pos ,&mapLoc);
    searchShowPoiIcon(bBox, pos, mapLoc, searchResultType);
}
