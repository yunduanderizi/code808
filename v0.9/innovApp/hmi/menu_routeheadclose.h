#ifndef MENU_ROUTEHEADCLOSE_H
#define MENU_ROUTEHEADCLOSE_H

#include "navibasewnd.h"
#include "ui_menu_headclose070200.h"

namespace Ui {
class HeadCloseMenu;
}

class Menu_RouteHeadClose : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_RouteHeadClose(QWidget *parent = NULL);
    ~Menu_RouteHeadClose();
public slots:

    void slotback();
    void close1km();
    void close2km();
    void close5km();
    void close10km();
    void close20km();
    void close50km();
private:
    Ui::HeadCloseMenu ui;
};

#endif // MENU_ROUTEHEADCLOSE_H
