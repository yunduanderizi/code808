#include "menu_routemain.h"
#include "windowinterface.h"
#include "globaldefine.h"
#include "msgbox_twobtn.h"
#include "engineinterface/interface_route.h"
Menu_RouteMain::Menu_RouteMain(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    ui.btn_routeoption->enabled(false);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);

    connect(ui.btn_headclode,SIGNAL(clicked()),this,SLOT(slotmenuHeadClose()),Qt::QueuedConnection);
    connect(ui.btn_areaclose,SIGNAL(clicked()),this,SLOT(slotmenuAreaClose()),Qt::QueuedConnection);
    connect(ui.btn_routeclose,SIGNAL(clicked()),this,SLOT(slotmenuRouteClose()),Qt::QueuedConnection);
    connect(ui.btn_stopguide,SIGNAL(clicked()),this,SLOT(slotStopGuide()),Qt::QueuedConnection);
    connect(ui.btn_routeoption,SIGNAL(clicked()),this,SLOT(slotmenuRouteOption()),Qt::QueuedConnection);
    initcontrol();
}

Menu_RouteMain::~Menu_RouteMain()
{

}

void Menu_RouteMain::initcontrol()
{
    if(routeIsSimulating() || routeIsManulSimulating())
    {
        ui.btn_headclode->enabled(true);
        ui.btn_stopguide->enabled(true);
    }
    else
    {
        ui.btn_headclode->enabled(false);
        ui.btn_stopguide->enabled(false);
    }
}

void Menu_RouteMain::backSlot()
{
    this->close();gotoMap();
}

void Menu_RouteMain::slotmenuHeadClose()
{
    this->close();changewindow(ROUTEHEADCLOSE);
}

void Menu_RouteMain::slotmenuAreaClose()
{
    this->close();changewindow(ROUTEAREACLOSE);
}

void Menu_RouteMain::slotmenuRouteClose()
{
    this->close();changewindow(ROUTEROADEDIT);
}

void Menu_RouteMain::gotonextwindow()
{
    this->close();gotoMap();
}

void Menu_RouteMain::slotStopGuide()
{
    MsgBox_DelRote* w = new MsgBox_DelRote(this);
    connect(w, SIGNAL(closefatherWnd()), this, SLOT(gotonextwindow()));
    w->show();
}

void Menu_RouteMain::slotmenuRouteOption()
{
    this->close();changewindow(ROUTESTRATEGY);
}
