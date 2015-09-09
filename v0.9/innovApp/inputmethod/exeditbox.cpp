//#include <Qt.h>
//#include <qevent.h>
//#include <qnamespace.h>
//#include <qnamespace.h>
#include "exeditbox.h"
#include <QKeyEvent>
//#include <QtGui/QKeyEvent>

ExEditBox::ExEditBox(QWidget* parent)
        :QLineEdit(parent)
{

}
/*
void ExEditBox::mousePressEvent(QMouseEvent *event)
{
     emit mousePressedSignal();
}
*/
void ExEditBox::keyPressEvent(QKeyEvent * event)
{
    //emit keyPressedSignal();
    //Qt::Key_Backspace	0x01000003
    //if(toPlainText() != NULL)
    //    this->setPlainText(this->toPlainText().left(this->toPlainText().count() - 1));
    //update();
    //backspace ()
    if(event->key() == Qt::Key_Backspace)//0x01000003)//Key_Backspace)
    {
        backspace();
    }
    else
    {
        emit keyPressedSignal();
    }
    return;

}
