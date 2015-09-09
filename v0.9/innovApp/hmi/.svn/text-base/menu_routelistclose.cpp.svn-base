#include "menu_routelistclose.h"
#include "windowinterface.h"
#include "globaldefine.h"
#include "engineinterface/interface_guide.h"
Menu_RouteListClose::Menu_RouteListClose(QWidget *parent) :
    NaviBaseWnd(parent)
{
    ui.setupUi(this);
    connect(ui.btn_back,SIGNAL(clicked()),this,SLOT(slotback()),Qt::QueuedConnection);
    connect(ui.btn_map,SIGNAL(clicked()),this,SLOT(mapSlot()),Qt::QueuedConnection);
    connect(ui.lst_route,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemSelectSlot(QListWidgetItem*)), Qt::QueuedConnection);
    updatelist();
}

Menu_RouteListClose::~Menu_RouteListClose()
{

}

void Menu_RouteListClose::slotback()
{
    this->close();changewindow(ROUTEMAINMENU);
}

void Menu_RouteListClose::updatelist()
{
    unsigned int ucount = guideGetTurnCount();
    for(unsigned idx = 0; idx < ucount; idx++)
    {
        wstring distanceFromStart;
        wstring roadname;
        wstring grade;
        wstring style;
        TSegmentID nsegmentid;
        guideGetTurnInfo(idx, distanceFromStart, roadname, grade, style,nsegmentid);
        QString str(QString::fromUtf16(reinterpret_cast<const unsigned short *>(roadname.c_str())));
        ui.lst_route->addItem(str);
    }
}

void Menu_RouteListClose::itemSelectSlot(QListWidgetItem*)
{
    int nidx = ui.lst_route->currentRow();
    wstring distanceFromStart;
    wstring roadname;
    wstring grade;
    wstring style;
    TSegmentID nsegmentid;
    guideGetTurnInfo(nidx, distanceFromStart, roadname, grade, style,nsegmentid);

    guideCloseSegment(nsegmentid);
}

