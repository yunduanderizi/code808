#include "map_areacloseedit.h"
#include "windowinterface.h"
#include "globaldefine.h"
#include "engineinterface/interface_map.h"
#include "engineinterface/interface_route.h"
Map_AreaCloseEdit::Map_AreaCloseEdit(QWidget *parent)
    :NaviBaseWnd(parent)
{
    ui.setupUi(this);
    bedit = false;
    if(mapGetMapMode() != MAPMODE_2D_NORTH)
    {
        mapSetMapMode(MAPMODE_2D_NORTH);
    }
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(goback()),Qt::QueuedConnection);
    connect(ui.btn_edit,SIGNAL(clicked()),this,SLOT(editarea()),Qt::QueuedConnection);

    connect(ui.btn_zoomin,SIGNAL(clicked()),this,SLOT(zoomin()),Qt::QueuedConnection);
    connect(ui.btn_zoomout,SIGNAL(clicked()),this,SLOT(zoomout()),Qt::QueuedConnection);
}

Map_AreaCloseEdit::~Map_AreaCloseEdit()
{
}

void Map_AreaCloseEdit::goback()
{
    mapDeleteAvoidanceArea();
    this->close();changewindow(ROUTEAREACLOSE);
}

void Map_AreaCloseEdit::editarea()
{
    if(bedit == false)
    {
        mapStartEditArea();
        ui.btn_edit->setText(UI_ROUTE_CLOSEAREAOK);
        bedit = true;
    }
    else
    {
        mapFinishAvoidanceArea();
        ui.btn_edit->setText(UI_ROUTE_CLOSEAREAEDIT);
        bedit = false;
        goback();
    }
}

void Map_AreaCloseEdit::zoomin()
{
    mapZoomIn();
    mapRefresh();
    repaint();
}

void Map_AreaCloseEdit::zoomout()
{
    mapZoomOut();
    mapRefresh();
    repaint();
}


Map_RoadCloseEdit::Map_RoadCloseEdit(QWidget *parent)
    :NaviBaseWnd(parent)
{
    ui.setupUi(this);
    ui.btn_edit->setText(UI_ROUTE_CLOSEROAD);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(goback()),Qt::QueuedConnection);
    connect(ui.btn_edit,SIGNAL(clicked()),this,SLOT(editroad()),Qt::QueuedConnection);

    connect(ui.btn_zoomin,SIGNAL(clicked()),this,SLOT(zoomin()),Qt::QueuedConnection);
    connect(ui.btn_zoomout,SIGNAL(clicked()),this,SLOT(zoomout()),Qt::QueuedConnection);
}

Map_RoadCloseEdit::~Map_RoadCloseEdit()
{
}

void Map_RoadCloseEdit::goback()
{
    this->close();changewindow(ROUTEMAINMENU);
}

void Map_RoadCloseEdit::editroad()
{
    addRoadAvoid();
}

void Map_RoadCloseEdit::zoomin()
{
    mapZoomIn();
    mapRefresh();
    repaint();
}

void Map_RoadCloseEdit::zoomout()
{
    mapZoomOut();
    mapRefresh();
    repaint();
}





