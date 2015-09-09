#include "menu_searchcategory.h"
#include "msgbox_onebtn.h"
#include "windowinterface.h"
#include "globaldefine.h"
#include <unistd.h>
Menu_SearchCategory::Menu_SearchCategory(QWidget *parent)
    :NaviBaseWnd(parent)
{
    ui.setupUi(this);
    //ui.uIButton_11->setvisible(false);
    //ui.uIButton_12->setvisible(false);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_gasstation,SIGNAL(clicked()),this,SLOT(gasStationSlot()),Qt::QueuedConnection);
    connect(ui.btn_bank,SIGNAL(clicked()),this,SLOT(bankSlot()),Qt::QueuedConnection);
    connect(ui.btn_carpark,SIGNAL(clicked()),this,SLOT(carParkSlot()),Qt::QueuedConnection);
    connect(ui.btn_hotel,SIGNAL(clicked()),this,SLOT(hotelSlot()),Qt::QueuedConnection);
    connect(ui.btn_restanrant,SIGNAL(clicked()),this,SLOT(restanrantSlot()),Qt::QueuedConnection);
    //connect(ui.btn_shop,SIGNAL(clicked()),this,SLOT(shopSlot()),Qt::QueuedConnection);
    connect(ui.btn_medic,SIGNAL(clicked()),this,SLOT(medicSlot()),Qt::QueuedConnection);
    connect(ui.btn_carservice,SIGNAL(clicked()),this,SLOT(carServiceSlot()),Qt::QueuedConnection);
    connect(ui.btn_service,SIGNAL(clicked()),this,SLOT(serviceSlot()),Qt::QueuedConnection);

    //connect(&m_pSearchThread, SIGNAL(finished()),this, SLOT(searchfinished()),Qt::DirectConnection);

}

Menu_SearchCategory::~Menu_SearchCategory()
{
}

void Menu_SearchCategory::backSlot()
{
    this->close();changewindow(SEARCHMENU);

}

void Menu_SearchCategory::gasStationSlot()
{
    searchAndShowResult(SCH_CATEGORY_POI_TYPE_GASSTATION);
}

void Menu_SearchCategory::bankSlot()
{
    searchAndShowResult(SCH_CATEGORY_POI_TYPE_BANK);
}

void Menu_SearchCategory::carParkSlot()
{
    searchAndShowResult(SCH_CATEGORY_POI_TYPE_PARKING);
}

void Menu_SearchCategory::hotelSlot()
{
    searchAndShowResult(SCH_CATEGORY_POI_TYPE_HOTEL);
}

void Menu_SearchCategory::restanrantSlot()
{
    searchAndShowResult(SCH_CATEGORY_POI_TYPE_RESTAURANT);
}

void Menu_SearchCategory::shopSlot()
{
    searchAndShowResult(SCH_CATEGORY_POI_TYPE_SUPERMAKET);
}

void Menu_SearchCategory::carServiceSlot()
{
    searchAndShowResult(SCH_CATEGORY_POI_TYPE_CARSERVICE);
}

void Menu_SearchCategory::serviceSlot()
{
    searchAndShowResult(SCH_CATEGORY_POI_TYPE_SERVICE);
}

void Menu_SearchCategory::medicSlot()
{
    searchAndShowResult(SCH_CATEGORY_POI_TYPE_HOSTPITAL);
}

void Menu_SearchCategory::searchAndShowResult(CATEGORY_POI_TYPE poitypeid)
{
    searchResetPoiResultCount();
    searchSetLevel(SCH_POI_LEVEL);
    startsearch(poitypeid);
    searchfinished();
}

void Menu_SearchCategory::startsearch(CATEGORY_POI_TYPE poitypeid)
{
    searchSetType(SCH_POI_CATEGORY_SEARCH_TYPE);
    searchsetCategoryPoiPid(poitypeid);
    searchCreateQuery((WCHAR*)NULL);
    search();
    //m_pSearchThread.start();

}

void Menu_SearchCategory::search()
{
    searchExecuteQuery();
}

void Menu_SearchCategory::searchfinished()
{

    if(searchGetPoiResultCount() > 0)
    {
        this->close();changewindow(CATEGORYRESULTLIST);

    }
    else
    {
        MsgBoxSearchNoItem* w = new MsgBoxSearchNoItem(this);
        w->show();
    }
}



