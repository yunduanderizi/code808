#include "map_mainwindow.h"
#include "engineinterface/interface_map.h"
#include "gps/satellitemodule.h"
#include <QDebug>
#include "engineinterface/interface_search.h"
#include <QString>
#include "windowinterface.h"
#include "globaldefine.h"
#include "msgbox_threebtn.h"
#include "map_guide.h"

#define MAIN_TIMER_NORMAL 1000
#if defined(ARM_LINUX)
    #define MAIN_TIMER_MOUSEMOVE 30
#else
    #define MAIN_TIMER_MOUSEMOVE 30
#endif

MapWindow::MapWindow(QWidget *parent) :
    QMainWindow(parent)
{
#if defined(ARM_LINUX)
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    ui.setupUi(this);
    this->move(297,48);


    refreshTimer = new QTimer(this);
    refreshTimer->setInterval(1000);
    connect(refreshTimer,SIGNAL(timeout()),this,SLOT(refreshDisplay()),Qt::QueuedConnection);

//    this->refreshDisplay();
    refreshTimer->start();
    //MapView *p_mapview = new MapView(this);
    Map_Guide *p_mapview = new Map_Guide(this);
    p_mapview->move(0,0);
    p_mapview->show();
//    p_mapsignpost = new map_signpost();
//    p_mapsignpost->show();
//    p_mapsignpost->move(10,400);
}

MapWindow::~MapWindow()
{
}

void MapWindow::mousePressEvent(QMouseEvent *event)
{
    if(mapIsDefiningAvoidArea())
    {
        TPos screenPos;
       Vertex_Define(&screenPos,event->x(),-event->y());
       mapUpdateAvoidanceVertex(screenPos);
       mapAddAvoidanceVertex();
       refreshDisplay();
       return;
    }
    if(mapMousePress(event->x(),event->y()))
    {
        changetimerint(MAIN_TIMER_MOUSEMOVE);
    }
}

void MapWindow::mouseReleaseEvent(QMouseEvent *event)
{
    changetimerint(MAIN_TIMER_NORMAL);
}

void MapWindow::mouseMoveEvent(QMouseEvent *event)
{
    //carPos_timer->start();
    if(mapIsDefiningAvoidArea()) return;
    mousemovepro(event->x(), event->y());
}

void MapWindow::mousemovepro(int x, int y)
{
    if(mapIsDefiningAvoidArea()) return;
    mapStartCarPosCount();
    mapMouseMove(x, y);
}

void MapWindow::paintEvent(QPaintEvent *)
{
    int x,y,w,h;
    x = this->x();
    y = this->y();
    w = this->width();
    h = this->height();

    QPainter painter;
    painter.begin( this );
    QImage *qimage = (QImage *)engineGetPlatformGraphicsImage();
    painter.drawImage(0,0,*qimage);

    painter.end();
}

void MapWindow::changetimerint(int inter)
{
    if(refreshTimer->interval() != inter) {
        refreshTimer->setInterval(inter);
    }
}

void MapWindow::refreshDisplay()
{
    mapRefresh();
    repaint();
}

void MapWindow::repaint()
{
    mapDrawRuler();
    update();
}

int MapWindow::managerHide(int flag)
{
    printf("managerHide.\r\n");
    if(flag == 2)
        return 0;
    this->hide();
    if(flag == 1)
        return 1;
    else
        return 0;
}

int MapWindow::managerBack(int /*flag*/)
{
    printf("managerBack.\r\n");
    this->hide();
    return 0;
}

int MapWindow::hasSubWindow()
{
    printf("hasSubWindow.\r\n");
}

int MapWindow::managerKey(int /*keyCode*/,int /*modifier*/)
{
    printf("managerKey.\r\n");
}

int MapWindow::managerShow(int /*flag*/)
{
    printf("managerShow.\r\n");
    this->show();
    this->raise();
    return 0;
}
