#ifndef RESULTLISTCATEGORYMENU_H
#define RESULTLISTCATEGORYMENU_H

#include "navibasewnd.h"
#include "ui_menu_SubListSearch010101.h"
#include "globaldefine.h"

namespace Ui {
class ResultListCategoryMenu;
}

class Menu_SearchResultListCategory : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_SearchResultListCategory(QWidget *parent = NULL);
    ~Menu_SearchResultListCategory();
public slots:
    void backSlot();
    void adminSlot();
    void itemSelectSlot(QListWidgetItem*);

    void upPageSlot();
    void downPageSlot();

private:
    int curPos;

private:
    Ui::ResultListCategoryMenu ui;
    int getLstSelItemIdx();

private:
    void initAdminLabel();
    void initPage();
    void printOnePage(int nCurrentPageSum = PAGE_ITEM_SUM);
};

#endif // RESULTLISTCATEGORYMENU_H
