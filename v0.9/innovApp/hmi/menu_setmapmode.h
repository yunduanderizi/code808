#ifndef MENU_SETTINGMAPMODE_H
#define MENU_SETTINGMAPMODE_H

#include "navibasewnd.h"
#include "ui_menu_settingMapmode040600.h"
#include "engineinterface/engineinterface.h"

namespace Ui
{
class setmapmodemenu;
}

class Menu_SetMapMode : public NaviBaseWnd
{
    Q_OBJECT
public:
    Menu_SetMapMode(QWidget *parent = NULL);
    ~Menu_SetMapMode();
signals:

public slots:
    void backSlot();
    void daySlot();
    void nightSlot();
    void lightSwitchSlot();
    void guidelineSlot();
    void daynightSlot();


private :
    void setmapmode();



private:
    Ui::setmapmodemenu ui;
};

#endif // MENU_SETTINGMAPMODE_H
