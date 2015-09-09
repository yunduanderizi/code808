#ifndef MSGBOXONEBTN_H
#define MSGBOXONEBTN_H

#include "ui_msg_onebtn.h"

namespace Ui {
class msgboxonebtn;
}

class MsgBox_OneBtn : public QWidget
{
    Q_OBJECT

public:
    MsgBox_OneBtn(QWidget *parent = NULL);
    ~MsgBox_OneBtn();

protected:
    Ui::msgboxonebtn ui;

signals:
    void closefatherWnd();

};


class MsgBoxSearchNoItem : public MsgBox_OneBtn
{
    Q_OBJECT
public:
    MsgBoxSearchNoItem(QWidget *parent = NULL);
};

class MsgBoxCalculatFailPath : public MsgBox_OneBtn
{
    Q_OBJECT
public:
    MsgBoxCalculatFailPath(QWidget *parent = NULL);
};

#endif // MSGBOXONEBTN_H
