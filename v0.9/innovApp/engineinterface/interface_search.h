#ifndef INTERFACE_SEARCH_H
#define INTERFACE_SEARCH_H

#include <innovEngine_sdk.h>
#include "navcoreGraphics.h"
#include "navcoreJourney.h"
#include"datatype.h"

void searchInit();
void searchReset();
void searchSetLevel(SEARCH_LEVEL);
void searchSetType(START_SEARCH_TYPE enumSearchType);
void searchCreateQuery(const WCHAR *);
void searchExecuteQuery();

void searchSetSelectIndex(int idx);
int searchGetSelectIndex();
void searchSelectObjHandleInSearchResult(int nSecletIdx);

void searchInitProvince();
void searchInitCity();
void searchSelectObjHandleInAdmin();
void searchSetAdminIndex(int nProvinceIdx, int nCityIdx);
void searchSetProvinceIdx(int);
void searchSetTmpProvinceIdx(int);
void searchsetCityIdx(int nCityIdx);
void searchSetProvinceName();
void searchSetTmpProvinceName();
void searchSetCityName();
const WCHAR* searchGetProvinceName();
const WCHAR* searchGetTmpProvinceName();
const WCHAR* searchGetCityName();

void searchSetArondPoiPid(AROUND_SCH_POI_TYPE poitypeid);
void searchsetCategoryPoiPid(CATEGORY_POI_TYPE nPoiPid);
void searchSetRoadName(int idx);
const WCHAR* searchGetRoadName();

bool searchGetMidPoiRetInfo(int nIdx,structDetailInfo *pStructInfo);
bool searchGetRetTextName(int nIdx,structDetailInfo *pStructInfo);
int searchGetResultCount();
int searchGetPoiResultCount();

void searchResetPoiResultCount();
bool searchResultGuidePOI();
bool searchResultGuideGeo();
void searchShowMapPoi();
void searchShowMapGeo();
void searchClearFavourites();

#endif // INTERFACE_SEARCH_H
