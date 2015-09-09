#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "navibasewnd.h"
#include "ui_map_broswer080000.h"
namespace Ui
{
    class MapView;
}

class Map_Browser : public NaviBaseWnd
{
    Q_OBJECT
public:
    Map_Browser(QWidget *parent = NULL);
    ~Map_Browser();

    void updateCarIconStatus();
    void setguideinfo();
    void repaint();

private:
//    void updatemapmode();
    void set2Dmapmode();
public slots:
     void gotoSearch();
     void gotoSetup();
     void gotoRoute();
     void returnCar();
     void zoomin();
     void zoomout();
//     void switchmapmode();
     void refreshDisplay();
     //slot for route delete dialog.

     void updateMapView();
public:
    Ui::MapView ui;
};

#endif // MAPVIEW_H
