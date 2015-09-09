#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_basemap000000.h>
#include <QMouseEvent>
#include <QMainWindow>
#include <innovEngine_sdk.h>
#include <qtimer.h>
#include "engineinterface/engineinterface.h"
#if defined(ARM_LINUX)
    #include "windowManagerApp.h"
#endif

namespace Ui {
class MainWindow;
}
#if defined(ARM_LINUX)
class MapWindow : public QMainWindow,public ManagedWindowApp
#else
class MapWindow : public QMainWindow
#endif
{
    Q_OBJECT


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:

    explicit MapWindow(QWidget *parent = 0);
    ~MapWindow();
    int managerShow(int /*flag*/);
    int managerHide(int /*flag*/);
    int managerBack(int /*flag*/);
    int hasSubWindow();

    int managerKey(int /*keyCode*/,int /*modifier*/);
    void repaint();
    void changetimerint(int inter);
    void mousemovepro(int x, int y);

private:
    QTimer *refreshTimer;
public slots:
    void refreshDisplay();
private:
    Ui::MainWindow ui;
//    int m_icarIconStatus;
};

#endif // MAINWINDOW_H
