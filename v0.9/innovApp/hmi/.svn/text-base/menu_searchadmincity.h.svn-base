#ifndef ADMINMENUCITY_H
#define ADMINMENUCITY_H
#include "navibasewnd.h"
#include <QListWidget>
#include "ui_menu_adminmenu01010101.h"
#include "globaldefine.h"

namespace Ui
{
    class AdminMenu;
}

class Menu_SearchAdminCity : public NaviBaseWnd
{
    Q_OBJECT
public:
    Menu_SearchAdminCity(QWidget* parent);
    void initAdminLabel();

public slots:
    void upPageSlot();
    void downPageSlot();

public:
    Ui::AdminMenu ui;

private:
    int curPos;
    void initPage();
    void printOnePage(int nCurrentPageSum = PAGE_ITEM_SUM);
};

class Menu_SearchAdminCityCross : public Menu_SearchAdminCity
{
    Q_OBJECT
public:
    Menu_SearchAdminCityCross(QWidget* parent = NULL);

signals:

public slots:
    void backSlotCross();
    void itemSelectSlotCross(QListWidgetItem*);
};

class Menu_SerachAdminCityKeyWord : public Menu_SearchAdminCity
{
    Q_OBJECT
public:
    Menu_SerachAdminCityKeyWord(QWidget* parent = NULL);

signals:

public slots:
    void backSlotKeyWord();
    void itemSelectSlotKeyWord(QListWidgetItem*);
};

class Menu_SearchAdminCityCategory : public Menu_SearchAdminCity
{
    Q_OBJECT
public:
    Menu_SearchAdminCityCategory(QWidget* parent = NULL);

signals:

public slots:
    void backSlotCategory();
    void itemSelectSlotCategory(QListWidgetItem*);
};

#endif // ADMINMENUCITY_H
