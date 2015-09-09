#ifndef RESULTLISTMENU_H
#define RESULTLISTMENU_H

#include "navibasewnd.h"
#include "ui_menu_resultlistmenu01010100.h"
#include "globaldefine.h"

namespace Ui {
class ResultListMenu;
}

class Menu_SearchResultList : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_SearchResultList(QWidget *parent = NULL);
    ~Menu_SearchResultList();
public:
    Ui::ResultListMenu ui;
};

class Menu_SearchResultListKey : public Menu_SearchResultList
{
    Q_OBJECT
public:
    Menu_SearchResultListKey(QWidget *parent = NULL);
    ~Menu_SearchResultListKey();

    int getLstSelItemIdx();

public slots:
    void backSlot();
    void itemSelectSlot(QListWidgetItem*);
    void upPageSlot();
    void downPageSlot();

private:
    int curPos;
    void initPage();
    void printOnePage(int nCurrentPageSum = PAGE_ITEM_SUM);
};

class Menu_SearchResultListAround : public Menu_SearchResultList
{
    Q_OBJECT

public:
    Menu_SearchResultListAround(QWidget *parent = NULL);
    ~Menu_SearchResultListAround();

    int getLstSelItemIdx();
public slots:
    void backSlot();
    void itemSelectSlot(QListWidgetItem*);
    void upPageSlot();
    void downPageSlot();

private:
    int curPos;
    void initPage();
    void printOnePage(int nCurrentPageSum = PAGE_ITEM_SUM);
};

class Menu_SearchResultListRoad : public Menu_SearchResultList
{
    Q_OBJECT
public:
    Menu_SearchResultListRoad(QWidget *parent = NULL);
    ~Menu_SearchResultListRoad();

    int getLstSelItemIdx();

public slots:
    void backSlot();
    void itemSelectSlot(QListWidgetItem*);
    void upPageSlot();
    void downPageSlot();

private:
    int curPos;
    void printOnePage(int nCurrentPageSum = PAGE_ITEM_SUM);
    void initPage();
    void fromRoad2Cross();
};

class Menu_SearchResultListCross : public Menu_SearchResultList
{
    Q_OBJECT

public:
    Menu_SearchResultListCross(QWidget *parent = NULL);
    ~Menu_SearchResultListCross();

    int getLstSelItemIdx();

public slots:
    void backSlot();
    void itemSelectSlot(QListWidgetItem*);

    void upPageSlot();
    void downPageSlot();

private:
    int curPos;
    void printOnePage(int nCurrentPageSum = PAGE_ITEM_SUM);
    void initPage();
};

#endif // RESULTLISTMENU_H
