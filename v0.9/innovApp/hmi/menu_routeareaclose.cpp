#include "menu_routeareaclose.h"
#include "windowinterface.h"
#include "globaldefine.h"
Menu_RouteAreaClose::Menu_RouteAreaClose(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(slotback()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_add,SIGNAL(clicked()),this,SLOT(addArea()),Qt::QueuedConnection);
    updatearealist();
}

Menu_RouteAreaClose::~Menu_RouteAreaClose()
{

}

void Menu_RouteAreaClose::addArea()
{
    this->close();changewindow(ROUTEAREAEDIT);
}

void Menu_RouteAreaClose::slotback()
{
    this->close();changewindow(ROUTEMAINMENU);
}

void Menu_RouteAreaClose::updatearealist()
{

}
