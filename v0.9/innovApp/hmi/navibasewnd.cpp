#include "navibasewnd.h"
#include "windowinterface.h"

NaviBaseWnd::NaviBaseWnd(QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowFlags(Qt::FramelessWindowHint);

    this->move(297,48);
}

NaviBaseWnd::~NaviBaseWnd()
{

}

void NaviBaseWnd::gotoMap()
{
    changewindow(MAPGUIDE);
}

void NaviBaseWnd::mapSlot()
{
    this->close();gotoMap();
}
