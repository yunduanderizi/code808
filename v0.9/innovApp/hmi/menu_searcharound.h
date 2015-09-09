#ifndef AROUNDMENU_H
#define AROUNDMENU_H

#include "navibasewnd.h"
#include "ui_menu_aroundsearch010200.h"
#include "engineinterface/interface_search.h"

namespace Ui {
class AroundMenu;
}

class Menu_SearchAround : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_SearchAround(QWidget *parent = NULL);
    ~Menu_SearchAround();
public slots:
    void backSlot();
    void gasStationSlot();
    void bankSlot();
    void carParkSlot();
    void hotelSlot();
    void restanrantSlot();
    void shopSlot();
    void carServiceSlot();
    void serviceSlot();
    void medicSlot();
    void searchAndShowResult(AROUND_SCH_POI_TYPE);
private:
    Ui::AroundMenu ui;
};

#endif // AROUNDMENU_H
