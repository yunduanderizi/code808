#ifndef EXEDITBOX_H
#define EXEDITBOX_H


#ifndef EXEDITBOX_H_
#define EXEDITBOX_H_

#include <QWidget>
#include <QLineEdit>

class ExEditBox:public QLineEdit
{
    Q_OBJECT

public:
    ExEditBox(QWidget* parent);
    ~ExEditBox(){};

public:
    //void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
signals:
     //void mousePressedSignal();
     void keyPressedSignal();
};

#endif
#endif // EXEDITBOX_H
