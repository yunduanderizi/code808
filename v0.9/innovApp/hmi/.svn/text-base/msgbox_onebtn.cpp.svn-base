#include "msgbox_onebtn.h"
#include "map_mainwindow.h"
#include "engineinterface/interface_search.h"
#include "windowinterface.h"
#include "globaldefine.h"

MsgBox_OneBtn::MsgBox_OneBtn(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::Window| Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_ShowModal);
    ui.setupUi(this);
    this->move(400,100);

}
MsgBox_OneBtn::~MsgBox_OneBtn()
{
}

MsgBoxSearchNoItem::MsgBoxSearchNoItem(QWidget *parent) :
    MsgBox_OneBtn(parent)
{

    ui.btn_ok->setText(CONFIRM_OK);
    ui.pic_info->setText(RESULT_NULL);

    connect(ui.btn_ok,SIGNAL(clicked()),this,SLOT(close()),Qt::QueuedConnection);
}


MsgBoxCalculatFailPath::MsgBoxCalculatFailPath(QWidget *parent) :
    MsgBox_OneBtn(parent)
{
    ui.btn_ok->setText(CONFIRM_OK);
    ui.pic_info->setText(FAILTURE_PATH);

    connect(ui.btn_ok,SIGNAL(clicked()),this,SLOT(close()),Qt::QueuedConnection);
}
