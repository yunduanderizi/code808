#include "menu_setup.h"
#include "windowinterface.h"
#include "globaldefine.h"
Menu_Setup::Menu_Setup(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    ui.btn_recoved->enabled(false);
    ui.btn_traffic->enabled(false);
    ui.btn_voiceset->enabled(false);
    ui.btn_carset->enabled(false);
    ui.xingkong->enabled(false);
    connect(ui.btn_returnback,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_version,SIGNAL(clicked()),this,SLOT(versionSlot()),Qt::QueuedConnection);
    connect(ui.btn_gps,SIGNAL(clicked()),this,SLOT(gpsSlot()),Qt::QueuedConnection);
    connect(ui.btn_traffic,SIGNAL(clicked()),this,SLOT(pathSlot()),Qt::QueuedConnection);
    connect(ui.btn_mapset,SIGNAL(clicked()),this,SLOT(mapModeSlot()),Qt::QueuedConnection);
    connect(ui.btn_voiceset,SIGNAL(clicked()),this,SLOT(soundSlot()),Qt::QueuedConnection);
}

void Menu_Setup::backSlot()
{
    this->close();gotoMap();
}

void Menu_Setup::versionSlot()
{

    this->close();changewindow(VERSIONMENU);
}

void Menu_Setup ::gpsSlot()
{

    this->close();changewindow(GPSMENU);
}

void Menu_Setup ::pathSlot()
{
  //  this->close();changewindow(ROUTEMAINMENU);
}

void Menu_Setup ::mapModeSlot()
{
    this->close();changewindow(MAPMODEMENU);
}

void Menu_Setup ::soundSlot()
{
    this->close();changewindow(SOUNDMENU);
}
