#include "guidethread.h"
#include "globaldefine.h"
#include "engineinterface/engineinterface.h"
#include "engineinterface/interface_route.h"
#include "engineinterface/interface_map.h"
#include "engineinterface/interface_guide.h"
#include <QDebug>

void playSound(QString soundstr)
{
#if defined(ARM_LINUX)
    if(soundstr != "")
    {
        //QTime t;
        ReadThread* tmpReadThread=ReadThread::object();
        if(tmpReadThread!=NULL)
        {
            tmpReadThread->aisound_start(soundstr,0);
        }
    }
#endif
}

QString getturndirection()
{
    QString turn_direction;
    QString cmd;
    if(guideIsEndOfJourney())
    {
        turn_direction = LANG_GOSTRAIGHT;
    }
    else
    {
        TGuideTurnDirection guidedirection =guideGetTurndDirection();
        switch (guidedirection)
        {
        case TURNDIRECTION_LEFT:
            turn_direction =LANG_TURNLEFT;
            break;
        case TURNDIRECTION_STRAIGHT:
            turn_direction = LANG_GOSTRAIGHT;
            break;
        case TURNDIRECTION_RIGHT:
            turn_direction = LANG_TURNRIGHT;
            break;
        case TURNDIRECTION_CURRENT_ROAD:
            turn_direction = LANG_NONE;
            break;
        }
    }
    return turn_direction;
}


QString getguidetext()
{
    QString str = "";
    if(engineCheckSound())
    {
        str = QString::fromUtf16(engineSoundText());
        qDebug() <<"setguidetext :" <<str;
    }
    return str;
}

void guideTTS()
{
    QString str = getguidetext();
    playSound(str);
}

GuideThread::GuideThread()
{

}

void GuideThread::run()
{
    while(1)
    {
        //guideTTS();
        sleep( 1);
    }
}
