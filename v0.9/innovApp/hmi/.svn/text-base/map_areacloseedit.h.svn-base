#ifndef MAP_AREACLOSEEDIT_H
#define MAP_AREACLOSEEDIT_H
#include "navibasewnd.h"
#include "ui_map_AreaCloseEdit070401.h"
namespace Ui
{
    class areaclosemenu;
}

class Map_AreaCloseEdit : public NaviBaseWnd
{
    Q_OBJECT

public:
    Map_AreaCloseEdit(QWidget *parent = NULL);
    ~Map_AreaCloseEdit();
    bool bedit;
public:
    Ui::areaclosemenu ui;
public slots:
    void goback();
    void editarea();
    void zoomin();
    void zoomout();
};

class Map_RoadCloseEdit : public NaviBaseWnd
{
    Q_OBJECT

public:
    Map_RoadCloseEdit(QWidget *parent = NULL);
    ~Map_RoadCloseEdit();
public:
    Ui::areaclosemenu ui;
public slots:
    void goback();
    void editroad();
    void zoomin();
    void zoomout();
};

#endif // MAP_AREACLOSEEDIT_H
