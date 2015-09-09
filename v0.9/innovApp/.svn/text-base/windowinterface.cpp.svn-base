#include "windowinterface.h"
#include "hmi/menu_searchadminprovince.h"
#include "hmi/menu_searchadmincity.h"
#include "hmi/menu_searcharound.h"
#include "hmi/menu_searchcross.h"
#include "hmi/menu_searchkeyword.h"
#include "hmi/map_mainwindow.h"
#include "hmi/navibasewnd.h"
#include "hmi/menu_searchresultinfo.h"
#include "hmi/menu_searchresultlist.h"
#include "hmi/menu_search.h"
#include "hmi/menu_searchcategory.h"
#include "hmi/menu_searchresultlistcategory.h"
#include "hmi/menu_setversion.h"
#include "hmi/menu_routeconfirm.h"
#include "hmi/menu_setgpsmode.h"
#include "hmi/menu_setup.h"
#include "hmi/menu_searchpoiview.h"
#include "hmi/map_guide.h"
#include "hmi/map_broswer.h"

#include "hmi/menu_routemain.h"
#include "hmi/menu_routestrategy.h"
#include "hmi/menu_routeheadclose.h"
#include "hmi/menu_routeareaclose.h"
#include "hmi/map_areacloseedit.h"
#include "hmi/menu_routelistclose.h"

#include "hmi/menu_setmapmode.h"
#include "hmi/menu_setsound.h"
extern QMainWindow *g_mainwindow;
#define SHOW_WINDOW_CODE(MENUID,WindowName)\
    case MENUID:\
{\
    WindowName* w = new WindowName(g_mainwindow);\
    w->move(0,0);\
    w->show();\
}\
    break;

QMainWindow* getMainWindow()
{
    return g_mainwindow;
}

bool changewindow(unsigned long nwndid)
{
    switch(nwndid)
    {
    SHOW_WINDOW_CODE(SEARCHMENU,Menu_Search);

    SHOW_WINDOW_CODE(KEYWORDMENU,Menu_SearchKeyWord);
    SHOW_WINDOW_CODE(KEYWORDLIST,Menu_SearchResultListKey);
    SHOW_WINDOW_CODE(KEYRESULTINFO,Menu_SearchResultInfoKey);
    SHOW_WINDOW_CODE(KEYWORDADMINPROVICE,Menu_SearchAdminProviceKeyWord);
    SHOW_WINDOW_CODE(KEYWORDADMINCITY,Menu_SerachAdminCityKeyWord);

    SHOW_WINDOW_CODE(AROUNDMENU,Menu_SearchAround);
    SHOW_WINDOW_CODE(AROUNDLIST,Menu_SearchResultListAround);
    SHOW_WINDOW_CODE(AROUNDRESULTINFO,Menu_SearchResultInfoAround);


    SHOW_WINDOW_CODE(ROADMENU,Menu_SearchCross);
    SHOW_WINDOW_CODE(ROADLIST,Menu_SearchResultListRoad);

    SHOW_WINDOW_CODE(CROSSLIST,Menu_SearchResultListCross);
    SHOW_WINDOW_CODE(CROSSRESULTINFO,Menu_SearchResultInfoCross);
    SHOW_WINDOW_CODE(CROSSPROVICE,Menu_SearchAdminProviceCross);
    SHOW_WINDOW_CODE(CROSSCITY,Menu_SearchAdminCityCross);

    SHOW_WINDOW_CODE(CATEGORYMENU,Menu_SearchCategory);
    SHOW_WINDOW_CODE(CATEGORYRESULTLIST,Menu_SearchResultListCategory);
    SHOW_WINDOW_CODE(CATEGORYRESULTINFO,Menu_SearchResultInfoCategory);
    SHOW_WINDOW_CODE(CATEGORYPROVICE,Menu_SearchAdminProviceCategory);
    SHOW_WINDOW_CODE(CATEGORYCITY,Menu_SearchAdminCityCategory);

    SHOW_WINDOW_CODE(ROUTECOMFIRMMENU,Menu_RouteConfirm);

    SHOW_WINDOW_CODE(SETTINGMENU,Menu_Setup);
    SHOW_WINDOW_CODE(VERSIONMENU,Menu_SetVersion);
    SHOW_WINDOW_CODE(GPSMENU,Menu_setGpsMode);
    SHOW_WINDOW_CODE(MAPMODEMENU,Menu_SetMapMode);
    SHOW_WINDOW_CODE(SOUNDMENU,Menu_SetSound);

    SHOW_WINDOW_CODE(KEYWORDPOIVIEW,Menu_SearchPoiViewKey);
    SHOW_WINDOW_CODE(AROUNDPOIVIEW,Menu_SearchPoiViewAround);
    SHOW_WINDOW_CODE(CATEGORYPOIVIEW,Menu_SearchPoiViewCategory);
    SHOW_WINDOW_CODE(CROSSPOIVIEW,Menu_SearchPoiViewCross);
    SHOW_WINDOW_CODE(MAPGUIDE,Map_Guide);
    SHOW_WINDOW_CODE(MAPBROWSER,Map_Browser);


    SHOW_WINDOW_CODE(ROUTEMAINMENU,Menu_RouteMain);
    SHOW_WINDOW_CODE(ROUTESTRATEGY,Menu_RouteStrategy);
    SHOW_WINDOW_CODE(ROUTEHEADCLOSE,Menu_RouteHeadClose);
    SHOW_WINDOW_CODE(ROUTEAREACLOSE,Menu_RouteAreaClose);
    SHOW_WINDOW_CODE(ROUTEAREAEDIT,Map_AreaCloseEdit);


    SHOW_WINDOW_CODE(ROUTERLISTCLOSE,Menu_RouteListClose);
    SHOW_WINDOW_CODE(ROUTEROADEDIT,Map_RoadCloseEdit);


    }

}
