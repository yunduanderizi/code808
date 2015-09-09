#ifndef MSGBOX_ROUTEDIALOG_H
#define MSGBOX_ROUTEDIALOG_H

#include "ui_msg_threebtn.h"
#include "QMainWindow"

namespace Ui {
class msgboxthreebtn;
}

class MessageBoxThreeBtn : public QMainWindow
{
    Q_OBJECT

public:
    MessageBoxThreeBtn(QWidget *parent = NULL);
    ~MessageBoxThreeBtn();

protected:
    Ui::msgboxthreebtn ui;

signals:
    void closefatherWnd();

};

class MsgBox_RoutePOI : public MessageBoxThreeBtn
{
    Q_OBJECT
public:
    MsgBox_RoutePOI(QWidget *parent = NULL);

public slots:
    void btnSimStart();
    void btnGuideStart();
    void btnCancel();
};


class MsgBox_SimStop : public MessageBoxThreeBtn
{
    Q_OBJECT
public:
    MsgBox_SimStop(QWidget *parent = NULL);
public slots:
    void btnSimStart();
    void btnGuideStart();
    void btnCancel();

};

class MsgBox_SimStopFinish : public MessageBoxThreeBtn
{
    Q_OBJECT
public:
    MsgBox_SimStopFinish(QWidget *parent = NULL);
public slots:
    void btnSimStart();
    void btnGuideStart();
    void btnCancel();
};

class MsgBox_GuideStop : public MessageBoxThreeBtn
{
    Q_OBJECT
public:
    explicit MsgBox_GuideStop(QWidget *parent = NULL);
public slots:
    void btnSimStart();
    void btnGuideStart();
    void btnCancel();

};


#endif // MSGBOX_ROUTEDIALOG_H
