#include "msgbox_twobtn.h"
#include "globaldefine.h"
#include "engineinterface/engineinterface.h"
#include "engineinterface/interface_route.h"
MsgBox_TwoBtn::MsgBox_TwoBtn(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::Window| Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_ShowModal);
    ui.setupUi(this);
    this->move(400,100);
}

MsgBox_TwoBtn::~MsgBox_TwoBtn()
{
}


MsgBox_DelRote::MsgBox_DelRote(QWidget *parent) :
    MsgBox_TwoBtn(parent)
{
    ui.btn_ok->setText(CONFIRM_OK);
    ui.btn_cancel->setText(CONFIRM_CANCEL);
    ui.pic_info->setText(INFO_DELETEROUTE);
    connect(ui.btn_ok,SIGNAL(clicked()),this,SLOT(slotOK()),Qt::QueuedConnection);
    connect(ui.btn_cancel,SIGNAL(clicked()),this,SLOT(slotCancel()),Qt::QueuedConnection);
}

void MsgBox_DelRote::slotOK()
{
    routestopmanualSimulating();
    routeStopSimulating();
    routeRemoveRoute();
    emit closefatherWnd();
}

void MsgBox_DelRote::slotCancel()
{
    this->close();
}

