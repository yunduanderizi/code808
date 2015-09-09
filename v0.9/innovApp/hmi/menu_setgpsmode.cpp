#include "menu_setgpsmode.h"
#include "gps/satellitemodule.h"
#include "windowinterface.h"
#include "engineinterface/engineinterface.h"

Menu_setGpsMode::Menu_setGpsMode(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);

    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_GPS,SIGNAL(clicked()),this,SLOT(gpsSlot()),Qt::QueuedConnection);
    connect(ui.btn_BD,SIGNAL(clicked()),this,SLOT(BD2Slot()),Qt::QueuedConnection);
    connect(ui.btn_BDGPS,SIGNAL(clicked()),this,SLOT(gpsAndBD2()),Qt::QueuedConnection);
}

Menu_setGpsMode::~Menu_setGpsMode()
{
}

void Menu_setGpsMode::backSlot()
{
    this->close();changewindow(SETTINGMENU);
}


void Menu_setGpsMode ::gpsSlot()
{

   g_satelliteModule->changeGPMode();
}

void Menu_setGpsMode :: BD2Slot()
{

   g_satelliteModule->changeBDMode();
}

void Menu_setGpsMode ::gpsAndBD2()
{
    g_satelliteModule->changeGNMode();

}
