#include "msgbox_threebtn.h"
#include "fontset.h"
#include "map_mainwindow.h"
#include "engineinterface/interface_search.h"
#include "engineinterface/engineinterface.h"
#include "engineinterface/interface_route.h"
#include "engineinterface/interface_guide.h"
#include "globaldefine.h"
#include "windowinterface.h"

#if defined(ARM_LINUX)
#include "TTS/read_news.h"
#endif

MessageBoxThreeBtn::MessageBoxThreeBtn(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowFlags(Qt::Window| Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_ShowModal);

    ui.setupUi(this);
    this->move(400,100);
}

MessageBoxThreeBtn::~MessageBoxThreeBtn()
{
}

MsgBox_RoutePOI::MsgBox_RoutePOI(QWidget *parent) :
    MessageBoxThreeBtn(parent)
{
    ui.btn_left->setText(UI_ROUTE_DIALOG_SIM);
    ui.btn_mid->setText(UI_ROUTE_DIALOG_GUIDE);
    ui.btn_right->setText(UI_ROUTE_DIALOG_CANCEL); //TODO

    connect(ui.btn_left,SIGNAL(clicked()),this,SLOT(btnSimStart()),Qt::QueuedConnection);
    connect(ui.btn_mid,SIGNAL(clicked()),this,SLOT(btnGuideStart()),Qt::QueuedConnection);
    connect(ui.btn_right,SIGNAL(clicked()),this,SLOT(btnCancel()),Qt::QueuedConnection);
}

void MsgBox_RoutePOI::btnSimStart()
{
    // simulate
    routeStartSimulating();
    emit closefatherWnd();
}

void MsgBox_RoutePOI::btnGuideStart()
{
    routeStartmanualSim();
    emit closefatherWnd();
}

void MsgBox_RoutePOI::btnCancel()
{
    routeRemoveRoute();
    emit closefatherWnd();
}

MsgBox_SimStop::MsgBox_SimStop(QWidget *parent) :
    MessageBoxThreeBtn(parent)
{
    ui.btn_left->setText(UI_ROUTE_DIALOG_REMOVE);
    ui.btn_mid->setText(UI_ROUTE_DIALOG_GUIDE);
    ui.btn_right->setText(UI_ROUTE_DIALOG_CANCEL);

    connect(ui.btn_left,SIGNAL(clicked()),this,SLOT(btnSimStart()),Qt::QueuedConnection);
    connect(ui.btn_mid,SIGNAL(clicked()),this,SLOT(btnGuideStart()),Qt::QueuedConnection);
    connect(ui.btn_right,SIGNAL(clicked()),this,SLOT(btnCancel()),Qt::QueuedConnection);
}

void MsgBox_SimStop::btnSimStart()
{
    // remove
    routeStopSimulating();
    routeRemoveRoute();
    close();//gotoMap();
}

void MsgBox_SimStop::btnGuideStart()
{
    // Guide
    routeStopSimulating();
    routeStartmanualSim();
    close();//gotoMap();
}

void MsgBox_SimStop::btnCancel()
{
    close();//gotoMap();
}

MsgBox_SimStopFinish::MsgBox_SimStopFinish(QWidget *parent) :
    MessageBoxThreeBtn(parent)
{

    ui.btn_left->setText(UI_ROUTE_DIALOG_REMOVE);
    ui.btn_mid->setText(UI_ROUTE_DIALOG_GUIDE);
    ui.btn_right->setText(UI_ROUTE_DIALOG_SIM);

    connect(ui.btn_left,SIGNAL(clicked()),this,SLOT(btnSimStart()),Qt::QueuedConnection);
    connect(ui.btn_mid,SIGNAL(clicked()),this,SLOT(btnGuideStart()),Qt::QueuedConnection);
    connect(ui.btn_right,SIGNAL(clicked()),this,SLOT(btnCancel()),Qt::QueuedConnection);
}

void MsgBox_SimStopFinish::btnSimStart()
{
    // remove
    routeRemoveRoute();
    close();//gotoMap();
}

void MsgBox_SimStopFinish::btnGuideStart()
{
    // Guide
    routeStartmanualSim();
    close();//gotoMap();
}

void MsgBox_SimStopFinish::btnCancel()
{
    routeStartSimulating();
    close();//gotoMap();
}

MsgBox_GuideStop::MsgBox_GuideStop(QWidget *parent) :
    MessageBoxThreeBtn(parent)
{
    ui.btn_left->setText(UI_ROUTE_DIALOG_REMOVE);
    ui.btn_mid->setText(UI_ROUTE_DIALOG_GUIDE);
    ui.btn_right->setText(UI_ROUTE_DIALOG_CANCEL);

    connect(ui.btn_left,SIGNAL(clicked()),this,SLOT(btnSimStart()),Qt::QueuedConnection);
    connect(ui.btn_mid,SIGNAL(clicked()),this,SLOT(btnGuideStart()),Qt::QueuedConnection);
    connect(ui.btn_right,SIGNAL(clicked()),this,SLOT(btnCancel()),Qt::QueuedConnection);
}

void MsgBox_GuideStop::btnSimStart()
{
    // remove
    routestopmanualSimulating();
    routeRemoveRoute();
    close();//gotoMap();
}

void MsgBox_GuideStop::btnGuideStart()
{
    // Sim
    routestopmanualSimulating();
    routeStartSimulating();
    close();//gotoMap();
}

void MsgBox_GuideStop::btnCancel()
{
    close();//gotoMap();
}
