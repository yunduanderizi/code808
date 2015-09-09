#include "menu_setversion.h"
#include "windowinterface.h"
#include "globaldefine.h"

Menu_SetVersion::Menu_SetVersion(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    //ui.uIPicture_2->setText(UI_SET_VERSION);

    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(backSlot()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    initVersionInfo();
}

void Menu_SetVersion::initVersionInfo()
{
    ui.pic_versioninfo->setText(SOFTWAREVERSION);
}

void Menu_SetVersion::backSlot()
{

    this->close();changewindow(SETTINGMENU);
}
