#ifndef MENU_VERSION_H
#define MENU_VERSION_H

#include "navibasewnd.h"
#include "ui_menu_settingVersion040700.h"


namespace Ui
{
class VersionMenu;
}

class Menu_SetVersion : public NaviBaseWnd
{
    Q_OBJECT
public:
    Menu_SetVersion(QWidget *parent = NULL);
    void initVersionInfo();
signals:

public slots:
    void backSlot();
private:
    Ui::versionmenu ui;
};

#endif // MENU_VERSION_H
