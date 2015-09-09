#include "map_guide.h"
#include "windowinterface.h"
#include "msgbox_threebtn.h"
#include "globaldefine.h"
#include "engineinterface/engineinterface.h"

#include "engineinterface/interface_route.h"
#include "engineinterface/interface_map.h"
#include "engineinterface/interface_guide.h"
#include "QDebug"
#include "gps/satellitemodule.h"
#if defined(ARM_LINUX)
#include "dbus/dbusinterface.h"
#include "TTS/read_news.h"
#endif

#include "map_laneinfo.h"
#include "map_signpost.h"
#include "map_junctionview.h"

map_junctionview *p_mapjunctionview = NULL;
map_laneinfo *p_maplaneinfo = NULL;
map_signpost *p_mapsignpost= NULL;

void playSound(QString soundstr)
{
#if defined(ARM_LINUX)
    if(soundstr != "")
    {
        //QTime t;
        ReadThread* tmpReadThread=ReadThread::object();
        if(tmpReadThread!=NULL)
        {
            tmpReadThread->aisound_start(soundstr,0);
        }
    }
#endif
}

QString getguidetext()
{
    QString str = "";
    if(engineCheckSound())
    {
        str = QString::fromUtf16(engineSoundText());
        qDebug() <<"setguidetext :" <<str;
    }
    return str;
}

void guideTTS()
{
    QString str = getguidetext();
    playSound(str);
}

Map_Guide::Map_Guide(QWidget *parent)
    :NaviBaseWnd(parent)
{
    ui.setupUi(this);

    ui.btn_return->setvisible(false);
    ui.logistics_Button->setvisible(false);
    mainTimer = new QTimer(this);
    mainTimer->setInterval(1000);
    connect(mainTimer,SIGNAL(timeout()),this,SLOT(gpsLocationSlot()),Qt::QueuedConnection);

    connect(ui.btn_route,SIGNAL(clicked()),this,SLOT(gotoRoute()),Qt::QueuedConnection);
    connect(ui.btn_mapmode,SIGNAL(clicked()),this,SLOT(switchmapmode()),Qt::QueuedConnection);
    connect(ui.btn_zoomin,SIGNAL(clicked()),this,SLOT(zoomin()),Qt::QueuedConnection);
    connect(ui.btn_zoomout,SIGNAL(clicked()),this,SLOT(zoomout()),Qt::QueuedConnection);
    connect(ui.btn_setting,SIGNAL(clicked()),this,SLOT(gotoSetup()),Qt::QueuedConnection);
    connect(ui.btn_search,SIGNAL(clicked()),this,SLOT(gotoSearch()),Qt::QueuedConnection);
    connect(ui.btn_return,SIGNAL(clicked()),this,SLOT(returnCar()),Qt::QueuedConnection);

    updatemapmode();
    MAP_MODE md = mapGetMapMode();
    mapSetMapMode(md);
    updateMapView();

    p_mapjunctionview = new map_junctionview(this);
    p_mapjunctionview->move(140,0);
    p_mapjunctionview->hide();
    p_maplaneinfo = new map_laneinfo(this);
    p_maplaneinfo->move(140,0);
    p_maplaneinfo->hide();
    this->refreshDisplay();
    mainTimer->start();
}

Map_Guide::~Map_Guide()
{
}

void Map_Guide::mousePressEvent(QMouseEvent *event)
{
    return;
}

void Map_Guide::mouseReleaseEvent(QMouseEvent *event)
{
    mainTimer->stop();
    delete mainTimer;
    this->close();changewindow(MAPBROWSER);
}

void Map_Guide::mouseMoveEvent(QMouseEvent *event)
{
    return;
}

void Map_Guide ::gotoRoute()
{
    mainTimer->stop();
    delete mainTimer;
    this->close();changewindow(ROUTEMAINMENU);
}

void Map_Guide::gotoSetup()
{
    mainTimer->stop();
    delete mainTimer;
    this->close();changewindow(SETTINGMENU);
}

void Map_Guide::gotoSearch()
{
    mainTimer->stop();
    delete mainTimer;
    this->close();changewindow(SEARCHMENU);
}

void Map_Guide::zoomin()
{
    ui.btn_zoomout->enabled(true);
    if(!mapZoomIn()) {
        ui.btn_zoomin->enabled(false);
    }
    if(mapIsScroll() == true)
        mapStartCarPosCount();
    refreshDisplay();
}

void Map_Guide::zoomout()
{
    ui.btn_zoomin->enabled(true);
    if(!mapZoomOut()) {
        ui.btn_zoomout->enabled(false);
    }
    if(mapIsScroll() == true)
        mapStartCarPosCount();
    refreshDisplay();
}

void Map_Guide::returnCar()
{
    mapStopCarPosCount();
    updateMapView();
    refreshDisplay();
}

void Map_Guide::updateMapView()
{
    if(routeIsSimulating()) {
        gotoMapinSimGuide();
    } else {
        gotoMapinRealGuide();
    }
}

void Map_Guide::updateCarIconStatus()
{
    bool carIconSetVisible = false;
    ui.btn_return->setVisible(carIconSetVisible);
}

void Map_Guide::refreshDisplay()
{
    mapRefresh();
    repaint();
}

void Map_Guide::repaint()
{
    mapDrawRuler();
    update();
}

void Map_Guide::startsim()
{
    routeStartSimulating();
}

void Map_Guide::stopSim()
{
    routeStopSimulating();
}

void Map_Guide::startrealGuide()
{
    routeStartmanualSim();
}

void Map_Guide::stopGuide()
{
    routestopmanualSimulating();
}

void Map_Guide::removeRoute()
{
    routeRemoveRoute();
}

void Map_Guide::cancelRoute()
{
    if(routeIsSimulating() == true)
    {
        MsgBox_SimStop* w = new MsgBox_SimStop(this);
        w->show();
    }
    else if(routeIsManulSimulating() == true)
    {
        MsgBox_GuideStop* w = new MsgBox_GuideStop(this);
        w->show();
    }
    else
    {
        removeRoute();
    }
}


void Map_Guide::realGuide()
{
    if(guideRealGuidance())
    {
        setguideinfo();
    }
    if(routeIsManulSimulating())
    {
        checkguidanceFinish();
    }
}

void Map_Guide::simGuide()
{
    if(routeSimilation())
    {
        setguideinfo();
    }
    if(routeIsSimulating())
    {
        checksimFinish();
    }
}

void Map_Guide::updateGuide()
{
    if(routeIsDefiningNewJourney() == true)
    {
        if(routeIsSimulating() == true)
        {
            simGuide();
        }
        else if(routeIsManulSimulating() == true)
        {
            realGuide();
        }
    }
}

void Map_Guide::gpsLocationSlot()
{
    mapCountCarPos();
    //set location
 //   ui.lab_locate->setText(QString::number(g_satelliteModule->getLatitude(), 'f') + "\n"\
 //                     + QString::number(g_satelliteModule->getLongitude(), 'f'));
    //update guidance
    updateGuide();

    //refresh map
    updateMapView();

    updatelaneview();

    updaterealview();
    //about dbus voice

}

void Map_Guide::updatelaneview()
{
    if(guideIsLaneinfoAvailable())
    {
        if(!p_maplaneinfo->isVisible())
        {
            p_maplaneinfo->show();
        }
        else
        {
            if(guideIsIntoNewTurn())
            {
                p_maplaneinfo->refresh();
            }
        }
    }
    else
    {
        if(p_maplaneinfo->isVisible())
        {
            p_maplaneinfo->hide();
        }
    }
    return;
}

void Map_Guide::updaterealview()
{
    if(guideIsJunctionAvailable())
    {
        if(getishavejuction() == 1)
        {
            if(!p_mapjunctionview->isVisible())
            {
                //g_mapjunctionview->repaint();
                p_mapjunctionview->show();
            }
        }
    }
    else
    {
        if(p_mapjunctionview->isVisible())
            p_mapjunctionview->hide();
    }
    return ;
}

void Map_Guide::checksimFinish()
{
    //ensure we are in simulate status now.
    if(routeIsSimulating() && routeIsSimulationFinished())
    {
        routeStopSimulating();
        MsgBox_SimStopFinish *p_routeStop = new MsgBox_SimStopFinish(this);
        p_routeStop->show();
    }
}

void Map_Guide::checkguidanceFinish()
{
    //ensure we are in guidance status now.
    if(routeIsManulSimulating() && routeIsManulSimulationFinished())
    {
        routestopmanualSimulating();
        stopGuide();
        removeRoute();
    }
}

void Map_Guide::gotoMapinRealGuide()
{
    TPos gps_loc;
    INT32 heading = 0;
    routeGetCurrentLocation(gps_loc,heading);
    gps_loc = mapSetCarIconPos(gps_loc, heading);
//    if(mapIsScroll() == false)
        mapGotoMap(gps_loc);
}

void Map_Guide::gotoMapinSimGuide()
{
    TPos gps_loc;
    routeGetGpsLocation(gps_loc);
    gps_loc = mapSetCarIconPos(gps_loc, routeGetGpsHeading() * 256 / 3600000);
//    if(mapIsScroll() == false)
        mapGotoMap(gps_loc);
}

void Map_Guide::switchmapmode()
{
    mapChangeMapMode();
    updatemapmode();
}

void Map_Guide::updatemapmode()
{
    MAP_MODE md = mapGetMapMode();
    if(md == MAPMODE_2D_NORTH)
    {
        ui.btn_mapmode->setNormalIcon(RESMAPMODENU);
    }
    else if(md == MAPMODE_2D_CAR)
    {
        ui.btn_mapmode->setNormalIcon(RESMAPMODEHU);
    }
    else if(md == MAPMODE_3D_CAR)
    {
        ui.btn_mapmode->setNormalIcon(RESMAPMODE3D);
    }
}

void Map_Guide::setguideinfo()
{

    UTF16 *fullRoadName = (UTF16*)malloc(sizeof(UTF16)*72);
    memset(fullRoadName, 0, sizeof(fullRoadName));
    guideGetNextTurnStreetName(fullRoadName);

    INT32 distoturn, distoend;
    guideGetDistanceToTurn(&distoturn);
    distoend = guideGetDistanceToDist();
    guideTTS();
#if defined(ARM_LINUX)
    int status = guideGetInfoStatus();
    int dir = guideGetInfoDirection();
    QString roadname = QString::fromUtf16(fullRoadName);
    dbus_postnaviinfo(status, dir, (char*)roadname.toStdString().c_str(), distoturn, distoend);
#endif
    free((UTF16*)fullRoadName);
}
