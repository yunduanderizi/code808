#include "menu_searcharound.h"
#include "msgbox_onebtn.h"
#include "windowinterface.h"
#include "globaldefine.h"
Menu_SearchAround::Menu_SearchAround(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);

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
}

Menu_SearchAround::~Menu_SearchAround()
{

}

void Menu_SearchAround::backSlot()
{
    this->close();changewindow(SEARCHMENU);

}

void Menu_SearchAround::gasStationSlot()
{
    searchAndShowResult(SCH_AROUND_POI_TYPE_GASSTATION);
}

void Menu_SearchAround::bankSlot()
{
    searchAndShowResult(SCH_AROUND_POI_TYPE_BANK);
}

void Menu_SearchAround::carParkSlot()
{
    searchAndShowResult(SCH_AROUND_POI_TYPE_PARKING);
}

void Menu_SearchAround::hotelSlot()
{
    searchAndShowResult(SCH_AROUND_POI_TYPE_HOTEL);
}

void Menu_SearchAround::restanrantSlot()
{
    searchAndShowResult(SCH_AROUND_POI_TYPE_RESTAURANT);
}

void Menu_SearchAround::shopSlot()
{
    searchAndShowResult(SCH_AROUND_POI_TYPE_SUPERMAKET);
}

void Menu_SearchAround::carServiceSlot()
{
    searchAndShowResult(SCH_AROUND_POI_TYPE_CARSERVICE);
}

void Menu_SearchAround::serviceSlot()
{
    searchAndShowResult(SCH_AROUND_POI_TYPE_SERVICE);
}

void Menu_SearchAround::medicSlot()
{
    searchAndShowResult(SCH_AROUND_POI_TYPE_HOSTPITAL);
}

void Menu_SearchAround::searchAndShowResult(AROUND_SCH_POI_TYPE poitypeid)
{
    searchResetPoiResultCount();
    searchSetLevel(SCH_POI_LEVEL);

    searchSetType(SCH_POI_AROUND_SEARCH_TYPE);
    searchSetArondPoiPid(poitypeid);
    searchCreateQuery((WCHAR *)NULL);
    searchExecuteQuery();

    if(searchGetPoiResultCount() > 0)
    {
        this->close();changewindow(AROUNDLIST);
    }
    else
    {
        MsgBoxSearchNoItem* w = new MsgBoxSearchNoItem(this);
        w->show();
    }
}

