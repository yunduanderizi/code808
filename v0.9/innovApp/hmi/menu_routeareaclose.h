#ifndef MENU_ROUTEAREACLOSE_H
#define MENU_ROUTEAREACLOSE_H

#include "navibasewnd.h"
#include "ui_menu_AreaClose070300.h"

namespace Ui {
class CloseAreaMenu;
}

class Menu_RouteAreaClose : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_RouteAreaClose(QWidget *parent = NULL);
    ~Menu_RouteAreaClose();
public slots:
    void addArea();
    void slotback();
    void updatearealist();
private:
    Ui::CloseAreaMenu ui;
};

#endif // MENU_ROUTEAREACLOSE_H

