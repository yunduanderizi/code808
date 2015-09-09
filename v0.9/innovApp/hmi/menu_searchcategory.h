#ifndef CATEGORYMENU_H
#define CATEGORYMENU_H

#include "navibasewnd.h"
#include "ui_menu_CategorySearch010400.h"
#include "engineinterface/interface_search.h"
#include "searchthread.h"

namespace Ui {
class CategoryMenu;
}

class Menu_SearchCategory : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_SearchCategory(QWidget *parent = NULL);
    ~Menu_SearchCategory();
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
    void searchAndShowResult(CATEGORY_POI_TYPE);
    void startsearch(CATEGORY_POI_TYPE);
    void search();
    void searchfinished();
private:
  //SearchThread m_pSearchThread;
  Ui::CategoryMenu ui;
};

#endif // CATEGORYMENU_H
