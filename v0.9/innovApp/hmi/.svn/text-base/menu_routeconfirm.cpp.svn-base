#include "menu_routeconfirm.h"

Menu_RouteConfirm::Menu_RouteConfirm(QWidget *parent)
    :NaviBaseWnd(parent)
{
    ui.setupUi(this);

    this->setAttribute(Qt::WA_ShowModal);
    connect(ui.btn_ok,SIGNAL(clicked()),this,SLOT(oKSlot()),Qt::QueuedConnection);
    connect(ui.btn_cancel,SIGNAL(clicked()),this,SLOT(cancleSlot()),Qt::QueuedConnection);
    initLabel();
}

Menu_RouteConfirm::~Menu_RouteConfirm()
{
}

void Menu_RouteConfirm::initLabel()
{
    ui.label->setText("remove current route now?");
}

void Menu_RouteConfirm::cancleSlot()
{
    close();
}

void Menu_RouteConfirm::oKSlot()
{
    close();
}
