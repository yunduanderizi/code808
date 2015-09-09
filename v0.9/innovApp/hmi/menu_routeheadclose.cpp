#include "menu_routeheadclose.h"
#include "windowinterface.h"
#include "globaldefine.h"
#include "engineinterface/interface_guide.h"
Menu_RouteHeadClose::Menu_RouteHeadClose(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(slotback()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_1km,SIGNAL(clicked()),this,SLOT(close1km()),Qt::QueuedConnection);
 //   connect(ui.btn_2km,SIGNAL(clicked()),this,SLOT(close2km()),Qt::QueuedConnection);
    connect(ui.btn_5km,SIGNAL(clicked()),this,SLOT(close5km()),Qt::QueuedConnection);
    connect(ui.btn_10km,SIGNAL(clicked()),this,SLOT(close10km()),Qt::QueuedConnection);
    connect(ui.btn_15km,SIGNAL(clicked()),this,SLOT(close20km()),Qt::QueuedConnection);
    connect(ui.btn_20km,SIGNAL(clicked()),this,SLOT(close20km()),Qt::QueuedConnection);
    connect(ui.btn_25km,SIGNAL(clicked()),this,SLOT(close20km()),Qt::QueuedConnection);
    connect(ui.btn_30km,SIGNAL(clicked()),this,SLOT(close20km()),Qt::QueuedConnection);
    connect(ui.btn_35km,SIGNAL(clicked()),this,SLOT(close20km()),Qt::QueuedConnection);
    connect(ui.btn_40km,SIGNAL(clicked()),this,SLOT(close50km()),Qt::QueuedConnection);
    connect(ui.btn_45km,SIGNAL(clicked()),this,SLOT(close50km()),Qt::QueuedConnection);
 //   connect(ui.btn_50km,SIGNAL(clicked()),this,SLOT(close50km()),Qt::QueuedConnection);
}

Menu_RouteHeadClose::~Menu_RouteHeadClose()
{

}

void Menu_RouteHeadClose::slotback()
{
    this->close();changewindow(ROUTEMAINMENU);
}

void Menu_RouteHeadClose::close1km()
{
    guideBanfrontSegments(1000);
    this->close();gotoMap();
}

void Menu_RouteHeadClose::close2km()
{
    guideBanfrontSegments(2000);
    this->close();gotoMap();
}

void Menu_RouteHeadClose::close5km()
{
    guideBanfrontSegments(5000);
    this->close();gotoMap();
}

void Menu_RouteHeadClose::close10km()
{
    guideBanfrontSegments(10000);
    this->close();gotoMap();
}

void Menu_RouteHeadClose::close20km()
{
    guideBanfrontSegments(20000);
    this->close();gotoMap();
}

void Menu_RouteHeadClose::close50km()
{
    guideBanfrontSegments(50000);
    this->close();gotoMap();
}
