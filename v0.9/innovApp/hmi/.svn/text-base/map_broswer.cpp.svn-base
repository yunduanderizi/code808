#include "map_broswer.h"
#include "windowinterface.h"
#include "globaldefine.h"
#include "engineinterface/engineinterface.h"

#include "engineinterface/interface_route.h"
#include "engineinterface/interface_map.h"
#include "engineinterface/interface_guide.h"
#include "QDebug"
#include "gps/satellitemodule.h"
#if defined(ARM_LINUX)
#include "dbus/dbusinterface.h"
#include "TTS/read_news.h"
#endif

Map_Browser::Map_Browser(QWidget *parent)
    :NaviBaseWnd(parent)
{
    ui.setupUi(this);
//    ui.btn_mapmode->setText(RESMAPMODE);
//    ui.btn_return->setText(RESCAR);
    ui.logistics_Button->setvisible(false);
    ui.btn_mapmode->setvisible(false);

    connect(ui.btn_route,SIGNAL(clicked()),this,SLOT(gotoRoute()),Qt::QueuedConnection);
    //connect(ui.btn_mapmode,SIGNAL(clicked()),this,SLOT(switchmapmode()),Qt::QueuedConnection);
    connect(ui.btn_zoomin,SIGNAL(clicked()),this,SLOT(zoomin()),Qt::QueuedConnection);
    connect(ui.btn_zoomout,SIGNAL(clicked()),this,SLOT(zoomout()),Qt::QueuedConnection);
    connect(ui.btn_setting,SIGNAL(clicked()),this,SLOT(gotoSetup()),Qt::QueuedConnection);
    connect(ui.btn_search,SIGNAL(clicked()),this,SLOT(gotoSearch()),Qt::QueuedConnection);
    connect(ui.btn_return,SIGNAL(clicked()),this,SLOT(returnCar()),Qt::QueuedConnection);
   // connect(ui.btn_cancel,SIGNAL(clicked()),this,SLOT(cancelRoute()),Qt::QueuedConnection);

//    updatemapmode();
    set2Dmapmode();
    updateMapView();
    this->refreshDisplay();
}

Map_Browser::~Map_Browser()
{
}

void Map_Browser ::gotoRoute()
{
    this->close();changewindow(ROUTEMAINMENU);
}

void Map_Browser::gotoSetup()
{
    this->close();changewindow(SETTINGMENU);
}

void Map_Browser::gotoSearch()
{
    this->close();changewindow(SEARCHMENU);
}

void Map_Browser::zoomin()
{
    ui.btn_zoomout->enabled(true);
    if(!mapZoomIn()) {
        ui.btn_zoomin->enabled(false);
    }
    if(mapIsScroll() == true)
        mapStartCarPosCount();
    refreshDisplay();
}

void Map_Browser::zoomout()
{
    ui.btn_zoomin->enabled(true);
    if(!mapZoomOut()) {
        ui.btn_zoomout->enabled(false);
    }
    if(mapIsScroll() == true)
        mapStartCarPosCount();
    refreshDisplay();
}

void Map_Browser::returnCar()
{
    mapStopCarPosCount();
//    updateMapView();
//    refreshDisplay();
    this->close();changewindow(MAPGUIDE);
}

void Map_Browser::updateMapView()
{

}

void Map_Browser::updateCarIconStatus()
{
    bool carIconSetVisible = false;
    ui.btn_return->setVisible(carIconSetVisible);
}

void Map_Browser::refreshDisplay()
{
    mapRefresh();
    repaint();
}

void Map_Browser::repaint()
{
    mapDrawRuler();
    update();
}

//void Map_Browser::switchmapmode()
//{
//    mapChangeMapMode();
////    updatemapmode();
//}

void Map_Browser::set2Dmapmode()
{
    if(mapGetMapMode() != MAPMODE_2D_NORTH)
    {
        mapSetMapMode(MAPMODE_2D_NORTH);
    }
}
