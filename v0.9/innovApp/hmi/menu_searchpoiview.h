#ifndef POIVIEWMENU_H
#define POIVIEWMENU_H
#include "navibasewnd.h"
#include "ui_map_infoview040200.h"

namespace Ui {
class Form;
}

class Menu_SearchPoiView : public NaviBaseWnd
{
    Q_OBJECT

public:
    Menu_SearchPoiView(QWidget *parent = NULL);
    ~Menu_SearchPoiView();

private:
    Ui::Form ui;

public slots:
    virtual void returnSlot()=0;

};

class Menu_SearchPoiViewKey: public Menu_SearchPoiView
{

    Q_OBJECT
public:
    Menu_SearchPoiViewKey(QWidget *parent = NULL);
    ~Menu_SearchPoiViewKey();

public slots:
    void returnSlot();
};

class Menu_SearchPoiViewAround: public Menu_SearchPoiView
{

    Q_OBJECT
public:
    Menu_SearchPoiViewAround(QWidget *parent = NULL);
    ~Menu_SearchPoiViewAround();

public slots:
    void returnSlot();
};

class Menu_SearchPoiViewCategory: public Menu_SearchPoiView
{
    Q_OBJECT
public:
    Menu_SearchPoiViewCategory(QWidget *parent = NULL);
    ~Menu_SearchPoiViewCategory();

public slots:
    void returnSlot();
};

class Menu_SearchPoiViewCross: public Menu_SearchPoiView
{

    Q_OBJECT
public:
    Menu_SearchPoiViewCross(QWidget *parent = NULL);
    ~Menu_SearchPoiViewCross();

public slots:
    void returnSlot();
};

#endif // POIVIEWMENU_H
