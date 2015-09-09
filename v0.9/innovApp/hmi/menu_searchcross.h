#ifndef CROSSMENU_H
#define CROSSMENU_H

#include "navibasewnd.h"
#include "ui_menu_keywordSearch010100.h"
namespace Ui
{
    class InputMenu;
}

class Menu_SearchCross : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_SearchCross(QWidget *parent = NULL);
    ~Menu_SearchCross();

    void startsearch();
    void search();
    void initAdminLabel();
public slots:
    void backSlot();
    void adminSlot();
    void searchSlot();
public:
    Ui::InputMenu ui;
};

#endif // CROSSMENU_H
