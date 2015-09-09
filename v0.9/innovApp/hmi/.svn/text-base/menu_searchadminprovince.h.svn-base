#ifndef ADMINMENUPROVINCE_H
#define ADMINMENUPROVINCE_H
#include "navibasewnd.h"
#include <QListWidget>
#include "ui_menu_adminmenu01010101.h"
#include "globaldefine.h"

namespace Ui
{
    class AdminMenu;
}

class Menu_SearchAdminProvice : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_SearchAdminProvice(QWidget* parent);
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

private:
};

class Menu_SearchAdminProviceKeyWord : public Menu_SearchAdminProvice
{
    Q_OBJECT
public:
    Menu_SearchAdminProviceKeyWord(QWidget* parent = NULL);
public slots:
    void backSlotKeyWord();
    void itemSelectSlotKeyWord(QListWidgetItem*);
};

class Menu_SearchAdminProviceCross : public Menu_SearchAdminProvice
{
    Q_OBJECT
public:
    Menu_SearchAdminProviceCross(QWidget* parent = NULL);
signals:

public slots:
    void backSlotCross();
    void itemSelectSlotCross(QListWidgetItem*);
};

class Menu_SearchAdminProviceCategory : public Menu_SearchAdminProvice
{
    Q_OBJECT
public:
    Menu_SearchAdminProviceCategory(QWidget* parent = NULL);
signals:

public slots:
    void backSlotCategory();
    void itemSelectSlotCategory(QListWidgetItem*);
};

#endif // ADMINMENUPROVINCE_H
