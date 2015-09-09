#include "menu_setmapmode.h"
#include "windowinterface.h"
#include "engineinterface/interface_map.h"
#include "globaldefine.h"

static bool NIGHT_STATUS=false;

Menu_SetMapMode::Menu_SetMapMode(QWidget *parent) :
    NaviBaseWnd(parent)
{
    mapLoadDefaultColour();
    ui.setupUi(this);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_leftmapmode,SIGNAL(clicked()),this,SLOT(daynightSlot()),Qt::QueuedConnection);
    connect(ui.btn_rightmapmode,SIGNAL(clicked()),this,SLOT(daynightSlot()),Qt::QueuedConnection);
    setmapmode();
//    connect(ui.btn_lightswitch,SIGNAL(clicked()),this,SLOT(lightSwitchSlot()),Qt::QueuedConnection);
//    connect(ui.btn_guideline,SIGNAL(clicked()),this,SLOT(guidelineSlot()),Qt::QueuedConnection);
}

void Menu_SetMapMode::setmapmode()
{
    if(NIGHT_STATUS)
    {
        ui.pic_text_6->setText(NIGHT_MODE);
    }
    else
    {
        ui.pic_text_6->setText(DAY_MODE);
    }
}

Menu_SetMapMode::~Menu_SetMapMode()
{

}

void Menu_SetMapMode::backSlot()
{
    this->close();changewindow(SETTINGMENU);
}

void Menu_SetMapMode ::nightSlot()
{

    mapSetCoreColourScheme();
    mapSaveViewPort();
    mapRefresh();
    repaint();
}

void Menu_SetMapMode ::daySlot()
{
    mapResetColourScheme();
    repaint();
}

void Menu_SetMapMode ::daynightSlot()
{
    NIGHT_STATUS = !NIGHT_STATUS;
    if(NIGHT_STATUS)
    {
        ui.pic_text_6->setText(NIGHT_MODE);
        mapSetCoreColourScheme();
        mapSaveViewPort();
        mapRefresh();
    }
    else
    {
        ui.pic_text_6->setText(DAY_MODE);
        mapResetColourScheme();
    }
    repaint();
}

void Menu_SetMapMode ::lightSwitchSlot()
{

}

void Menu_SetMapMode ::guidelineSlot()
{

}



