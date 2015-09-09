#ifndef KEYWORDMENU_H
#define KEYWORDMENU_H

#include "navibasewnd.h"
#include "ui_menu_keywordSearch010100.h"
namespace Ui
{
    class InputMenu;
}



class Menu_SearchKeyWord : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_SearchKeyWord(QWidget *parent = NULL);
    ~Menu_SearchKeyWord();

    void startsearch();
    void initAdminLabel();
public slots:
    void backSlot();
    void adminSlot();
    void searchSlot();
public:
    Ui::InputMenu ui;
};

#endif // KEYWORDMENU_H
