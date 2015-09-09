#ifndef MENU_SETUP_H
#define MENU_SETUP_H

#include "navibasewnd.h"
#include "ui_menu_setting040000.h"
namespace Ui
{
class SetupMenu;
}

class Menu_Setup : public NaviBaseWnd
{
    Q_OBJECT
public:
    Menu_Setup(QWidget *parent = NULL);

signals:

public slots:
    void backSlot();
    void versionSlot();
    void gpsSlot();
    void pathSlot();
    void mapModeSlot();
    void soundSlot();
private:
    Ui::SetupMenu ui;
};

#endif // MENU_SETUP_H
