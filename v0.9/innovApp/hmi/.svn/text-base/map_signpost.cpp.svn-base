#include "map_signpost.h"
#include "ui_map_signpost.h"
#include "engineinterface/interface_guide.h"
#include "qpainter.h"
#include "guidance/gdinstructionP.h"

map_signpost::map_signpost(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::map_signpost)
{
    ui->setupUi(this);
}

map_signpost::~map_signpost()
{
    delete ui;
}

void map_signpost::paintEvent(QPaintEvent *event)
{
    UTF16 pSignPostStr[100];
    QString signpoststr;

    memset(pSignPostStr,0,sizeof(pSignPostStr));

    if(!guideGetSignPostString(pSignPostStr))
        return ;

    signpoststr =QString::fromUtf16(pSignPostStr);
    QPainter painter(this);
    painter.setPen(Qt::blue);
    painter.drawText(rect(),signpoststr);
}

void map_signpost::repaint()
{
    update();
}
