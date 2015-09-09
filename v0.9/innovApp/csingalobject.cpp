#include "csingalobject.h"
#if defined(ARM_LINUX)
#include "dbus/NaviDbusMessage.h"
#endif
CSingalObject::CSingalObject()
{
}

CSingalObject::~CSingalObject()
{
}

void CSingalObject::setStrInputBakeup(QString &strObj)
{
    strInputBakeup = strObj;
}

QString CSingalObject::getStrInputBakeup()
{
    return strInputBakeup;
}

QString CSingalObject::length2String(unsigned int ulDist)
{
    QString strlength;
    if (ulDist < 1000)
    {
        strlength = QString("%1m").arg(ulDist);
    }
    else if(ulDist < 10000)
    {
        if(ulDist % 1000 > 0)
        {
            float fLastDist =float(ulDist)/1000;
            strlength = QString("%1km").arg(fLastDist,0, 'f', 1);
        }
        else
        {
            float fLastDist =float(ulDist)/1000;
            strlength = QString("%1km").arg(fLastDist);
        }
    }
    else
    {
        strlength = QString("%1km").arg(ulDist/1000);
    }
    return strlength;
}


void CSingalObject::Translate(int messageid,int a_ulParam1,int a_ulParam2)
{
    switch(messageid)
    {
#if defined(ARM_LINUX)
        case REQ_ID_NAVI_CARLOCATION:
            emit(carlocation());
        break;
        case REQ_ID_NAVI_GPSINFO:
            emit(gpsinfo());
        break;
        case REQ_ID_NAVI_WINDOWCONTROL:
        break;
        case REQ_ID_NAVI_GASSTATION:
        break;
        case REQ_ID_NAVI_PARK:
        break;
        case REQ_ID_NAVI_LOCATECAR:
        break;
        case REQ_ID_NAVI_LOCATEHOME:
        break;
        case REQ_ID_NAVI_DESTINATON:
        break;
        case REQ_ID_NAVI_DESTINATONRECORD:
        break;
        case REQ_ID_NAVI_NAVIGATION:
        break;
        case REQ_ID_NAVI_GPSINFO_REPORT_CYCLE:
        break;
        case REQ_ID_NAVI_ROADBOOK:
            emit(roadbook());
        break;
        case REQ_ID_NAVI_HOSPITAL:
        break;
        case REQ_ID_NAVI_REALTIMEROAD:
        break;
        case REQ_ID_NAVI_ROADREPORT:
        break;
        case REQ_ID_NAVI_COMMONREQ:
        break;
        case REQ_ID_NAVI_DAYNIGHTMODE:
        break;
        case REQ_ID_NAVI_RIGHT_BUTTON_ROTATE:
        break;
        case REQ_ID_NAVI_ADDRESSBOOK:
        break;
        case REQ_ID_NAVI_SURROUNDINGSEARCH:
        break;
        case REQ_ID_NAVI_ROADBOOK2:
        break;
        case REQ_ID_GUI_DAYNIGHTMODE:
        break;
        case REQ_ID_NAVI_SHOWMENU:
            emit(showmenu());
        break;
        case REQ_ID_NAVI_SHUTDOWN:
        break;
#endif
        default:
        break;
    }
}
