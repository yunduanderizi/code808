#include "menu_setsound.h"
#include "windowinterface.h"

Menu_SetSound::Menu_SetSound(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.btn_timesadd,SIGNAL(clicked()),this,SLOT(addbroadcastFrequencySlot()),Qt::QueuedConnection);
    connect(ui.btn_timesdec,SIGNAL(clicked()),this,SLOT(decbroadcastFrequencySlot()),Qt::QueuedConnection);
    connect(ui.btn_addvolumn,SIGNAL(clicked()),this,SLOT(addbroadcastVolumeSlot()),Qt::QueuedConnection);
    connect(ui.btn_decvolumn,SIGNAL(clicked()),this,SLOT(downbroadcastVolumeSlot()),Qt::QueuedConnection);
}

Menu_SetSound::~Menu_SetSound()
{

}

void Menu_SetSound::backSlot()
{
    this->close();changewindow(SETTINGMENU);
}

void Menu_SetSound :: addbroadcastFrequencySlot()
{

}

void Menu_SetSound :: decbroadcastFrequencySlot()
{

}

void Menu_SetSound :: downbroadcastVolumeSlot()
{

}

void Menu_SetSound ::addbroadcastVolumeSlot()
{

}
