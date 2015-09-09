#include "map_junctionview.h"
#include "engineinterface/interface_guide.h"
#include "qpainter.h"
#include "engineinterface/datatype.h"

map_junctionview::map_junctionview(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}

map_junctionview::~map_junctionview()
{
}

void map_junctionview::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    QImage qimage1,qimage2;
    long size1,size2;
    unsigned char picture1[JUNCTIONVIEW_PIC_SIEZ] = {0};
    unsigned char picture2[JUNCTIONVIEW_PIC_SIEZ] = {0};

    painter.begin( this );
    if(!guideGetJunctionViewDate(picture1,&size1,picture2,&size2))
    {
        return;
    }
    qimage1.loadFromData(picture1,size1);
    qimage2.loadFromData(picture2,size2);

    painter.drawImage(0,0,qimage2);
    painter.drawImage(0,0,qimage1);

    painter.end();
}
