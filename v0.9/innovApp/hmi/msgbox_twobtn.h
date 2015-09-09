#ifndef MSGBOX_TWOBTN_H
#define MSGBOX_TWOBTN_H
#include "ui_msg_twobtn.h"

namespace Ui {
class msgboxtwobtn;
}

class MsgBox_TwoBtn : public QWidget
{
    Q_OBJECT

public:
    MsgBox_TwoBtn(QWidget *parent = NULL);
    ~MsgBox_TwoBtn();

protected:
    Ui::msgboxtwobtn ui;
};


class MsgBox_DelRote : public MsgBox_TwoBtn
{
    Q_OBJECT
public:
    MsgBox_DelRote(QWidget *parent = NULL);

public slots:
    void slotOK();
    void slotCancel();
signals:
    void closefatherWnd();
};

#endif // MSGBOX_TWOBTN_H
