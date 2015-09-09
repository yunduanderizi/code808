#ifndef MAP_LANEINFO_H
#define MAP_LANEINFO_H

#include <QWidget>

#include "ui_map_laneinfo.h"

namespace Ui {
class map_laneinfo;
}

class map_laneinfo : public QWidget
{
protected:
    void paintEvent(QPaintEvent *event);
    Q_OBJECT

public:
    explicit map_laneinfo(QWidget *parent = 0);
    ~map_laneinfo();
    void repaint();
    void refresh();
    void drawEntryAndExits();
    void drawRoundabout();
private:
    Ui::map_laneinfo ui;
};

#endif // MAP_LANEINFO_H
