#ifndef MAP_GUIDE_H
#define MAP_GUIDE_H

#include "navibasewnd.h"
#include "ui_map_guide010200.h"
namespace Ui
{
    class GuideMap;
}

class Map_Guide : public NaviBaseWnd
{
    Q_OBJECT
public:
    Map_Guide(QWidget *parent = NULL);
    ~Map_Guide();

    void updateCarIconStatus();
    void setguideinfo();
    void repaint();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    QTimer *mainTimer;

    void updatemapmode();
public slots:
     void gotoSearch();
     void gotoSetup();
     void gotoRoute();
     void returnCar();
     void cancelRoute();
     void startsim();
     void stopSim();
     void startrealGuide();
     void stopGuide();
     void removeRoute();
     void simGuide();
     void updateGuide();
     void zoomin();
     void zoomout();
     void switchmapmode();
     void refreshDisplay();
     //slot for route delete dialog.

     void realGuide();
     void checksimFinish();
     void checkguidanceFinish();
     void gotoMapinRealGuide();
     void gotoMapinSimGuide();
     void updateMapView();
     void gpsLocationSlot();

     void updaterealview();
     void updatelaneview();
public:
    Ui::GuideMap ui;
};

#endif // MAP_GUIDE_H
