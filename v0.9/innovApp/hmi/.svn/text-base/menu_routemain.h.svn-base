#ifndef MENU_ROUTEMAIN_H
#define MENU_ROUTEMAIN_H
#include "navibasewnd.h"
#include "ui_menu_routemenu070000.h"

namespace Ui {
class routemenu;
}

class Menu_RouteMain : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_RouteMain(QWidget *parent = NULL);
    ~Menu_RouteMain();
    void initcontrol();
public slots:
    void backSlot();
    void slotmenuHeadClose();
    void slotmenuAreaClose();
    void slotmenuRouteClose();
    void slotStopGuide();
    void slotmenuRouteOption();
    void gotonextwindow();
private:
    Ui::routemenu ui;
};

#endif // MENU_ROUTEMAIN_H
