#ifndef RESULTINFOMENU_H
#define RESULTINFOMENU_H


#include "navibasewnd.h"
#include "ui_menu_infoview020000.h"


namespace Ui {
class ResultInfoMenu;
}
enum ResultInfoClassId
{
    RESULT_INFO_KEY,
    RESULT_INFO_AROUND,
    RESULT_INFO_CROSS,
    RESULT_INFO_CATEGORY
};

class Menu_SearchResultInfo : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_SearchResultInfo(QWidget *parent = NULL);
    ~Menu_SearchResultInfo();

public:
    Ui::ResultInfoMenu ui;

public slots:

signals:
    void removeRoute();
};

class Menu_SearchResultInfoKey : public Menu_SearchResultInfo
{
    Q_OBJECT

public:
    Menu_SearchResultInfoKey(QWidget *parent = NULL);
    ~Menu_SearchResultInfoKey();

    void initResultInfo();

public slots:
    void backSlot();
    void viewSlot();
    void guideSlot();
    void gotonextwindow();
};

class Menu_SearchResultInfoAround : public Menu_SearchResultInfo
{
    Q_OBJECT
public:
    Menu_SearchResultInfoAround(QWidget *parent = NULL);
     ~Menu_SearchResultInfoAround();

    void initResultInfo();

public slots:
    void backSlot();
    void viewSlot();
    void guideSlot();
    void gotonextwindow();
};

class Menu_SearchResultInfoCross : public Menu_SearchResultInfo
{
    Q_OBJECT
public:
    Menu_SearchResultInfoCross(QWidget *parent = NULL);
    void initResultInfo();

public slots:
    void backSlot();
    void viewSlot();
    void guideSlot();
    void gotonextwindow();
private:
    QString strInputBakeup;
};


class Menu_SearchResultInfoCategory : public Menu_SearchResultInfo
{
    Q_OBJECT
public:
    Menu_SearchResultInfoCategory(QWidget *parent = NULL);
     ~Menu_SearchResultInfoCategory();

    void initResultInfo();

public slots:
    void backSlot();
    void viewSlot();
    void guideSlot();
    void gotonextwindow();
};


#endif // RESULTINFOMENU_H

