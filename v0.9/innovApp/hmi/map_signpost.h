#ifndef MAP_SIGNPOST_H
#define MAP_SIGNPOST_H

#include <QWidget>

namespace Ui {
class map_signpost;
}

class map_signpost : public QWidget
{
protected:
    void paintEvent(QPaintEvent *event);
    Q_OBJECT

public:
    explicit map_signpost(QWidget *parent = 0);
    ~map_signpost();
    void repaint();

private:
    Ui::map_signpost *ui;
};

#endif // MAP_SIGNPOST_H
