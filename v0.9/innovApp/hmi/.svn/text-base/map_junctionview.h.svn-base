#ifndef MAP_JUNCTIONVIEW_H
#define MAP_JUNCTIONVIEW_H

#include <QWidget>
#include "ui_map_junctionview.h"
namespace Ui {
class map_junctionview;
}

class map_junctionview : public QWidget
{
protected:
    void paintEvent(QPaintEvent *event);
    Q_OBJECT

public:
    explicit map_junctionview(QWidget *parent = 0);
    ~map_junctionview();
    void repaint();
    void refresh();
    void clear();
private:
    Ui::map_junctionview ui;
};

#endif // MAP_JUNCTIONVIEW_H
