#include "dbusinterface.h"
#include "csingalobject.h"
#include "gpsinterface.h"
#include "gps/satellitemodule.h"
#include "engineinterface/engineinterface.h"

#if defined(ARM_LINUX)
#include "fence/fencemanager.h"
extern FenceManager *s_pfencemanager;
#endif
DBusServerC* g_pServer;
DBusClientProxyC* g_pClient;

void notifyinfo()
{

}

bool init_clientdbus()
{
    g_pClient = new DBusClientProxyC((char*)"com.YunTu.Navi", (char*)"/Navi");
    g_pClient->registerNotify(NOTF_ID_NAVI_ELECTRONIC_FENCE,CNaviElectronicFenceNotifyHandler,CNaviElectronicFenceNotifyC::factory);
    if(g_pClient->connect()<0)
    {
            printf("DBusClientProxyC Failed to connect to DBUS server.\r\n");
            return false;
    }
    return true;
}

void ShowMenuHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier()!=REQ_ID_NAVI_SHOWMENU)
        return;
    CNaviShowMenuC *testRequest=(CNaviShowMenuC*)request;
    CNaviCommonRepC *testReply	=new CNaviCommonRepC();

    CSingalObject::instance()->Translate(REQ_ID_NAVI_SHOWMENU,0,0);
    testReply->result = 0;
    reply=testReply;
}

void CarLocationHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier()!=REQ_ID_NAVI_CARLOCATION)
        return;
    CNaviCarLocationReqC *carRequest=(CNaviCarLocationReqC*)request;
    CNaviCarLocationRepC *testReply	=new CNaviCarLocationRepC();

    CSingalObject::instance()->Translate(REQ_ID_NAVI_CARLOCATION,0,0);

    testReply->cCurType = 0;
    strcpy(testReply->cAreaName,"area1");
    strcpy(testReply->cRoadName,"road1");
    strcpy(testReply->cityName,"city1");
    reply=testReply;
    reply->result = 0;
}

void GPSInfoReportCycleHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier()!=REQ_ID_NAVI_GPSINFO_REPORT_CYCLE)
        return;
    CNaviGPSInfoReportCycleReqC *gpsInfoReportCycReq=(CNaviGPSInfoReportCycleReqC*)request;
    CNaviCommonRepC *testReply	=new CNaviCommonRepC();

    CSingalObject::instance()->Translate(REQ_ID_NAVI_GPSINFO_REPORT_CYCLE,0,0);
    reply=testReply;
    testReply->result = 0;
}

void RoadBookHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier()!=REQ_ID_NAVI_ROADBOOK)
        return;
    CNaviRoadBookReqC *roadbookRequest=(CNaviRoadBookReqC*)request;
    RoadBookItems* pbookitem = roadbookRequest->GetBookItems();
    RoadBookItems* tempbookitem = pbookitem;
    while(tempbookitem->next)
    {
        tempbookitem->cDistName;
        tempbookitem->fX;
        tempbookitem->fY;
        tempbookitem=tempbookitem->next;
    }

    CSingalObject::instance()->Translate(REQ_ID_NAVI_ROADBOOK,0,0);

    CNaviCommonRepC *testReply	=new CNaviCommonRepC();
    reply=testReply;
    testReply->result = 0;

}

void RoadBook2Handler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier()!=REQ_ID_NAVI_ROADBOOK2)
        return;
    CNaviRoadBook2ReqC *roadbook2Request=(CNaviRoadBook2ReqC*)request;

    CSingalObject::instance()->Translate(REQ_ID_NAVI_ROADBOOK2,0,0);

    CNaviCommonRepC *testReply	=new CNaviCommonRepC();
    reply=testReply;
    testReply->result = 0;

}

void HospitalHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier()!=REQ_ID_NAVI_HOSPITAL)
        return;
    CNaviHospitalReqC *hospitalRequest=(CNaviHospitalReqC*)request;

    CSingalObject::instance()->Translate(REQ_ID_NAVI_HOSPITAL,0,0);

    CNaviCommonRepC *tmpReply	=new CNaviCommonRepC();
    reply=tmpReply;
    tmpReply->result = 0;
}

void GPSInfoHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_GPSINFO)
        return;
    CNaviGPSInfoRepC *gpsInfoReq = (CNaviGPSInfoRepC*)request;

    CSingalObject::instance()->Translate(REQ_ID_NAVI_GPSINFO, 0, 0);

    CNaviGPSInfoRepC *tmpReply =new CNaviGPSInfoRepC();
//    tmpReply->fX = 100.1111;			// 当前GPS坐标X
//    tmpReply->fY = 30.1111;				// 当前GPS坐标Y
    tmpReply->fX= g_satelliteModule->getLongitude();
    tmpReply->fY = g_satelliteModule->getLatitude();
    tmpReply->iTime = 0;			// 当前的GPS时间
    tmpReply->iSysTime = 0;		// 当前的系统时间

//  tmpReply->iOrientation = 30;         // 汽车行驶方位角

    tmpReply->iOrientation = g_satelliteModule->getHeading();         // 汽车行驶方位角
    tmpReply->iVelocity = 40;

    tmpReply->result = 0;
    reply=tmpReply;
}

void WindowDisplayControlInfoHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_WINDOWCONTROL)
        return;
    CNaviWindowDisplayControlReqC *windowControlReq = (CNaviWindowDisplayControlReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_WINDOWCONTROL, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply = tmpReply;
}

void GasStationHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_GASSTATION)
        return;
    CNaviGasStationReqC *gasStationReq =(CNaviGasStationReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_GASSTATION, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply = tmpReply;

}

void ParkHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_PARK)
        return;
    CNaviParkReqC *parkReq =(CNaviParkReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_PARK, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void LocateCarHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_LOCATECAR)
        return;
    CNaviLocateCarReqC *locateCarReq =(CNaviLocateCarReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_LOCATECAR, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;

}

void LocateHomeHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_LOCATEHOME)
        return;
    CNaviLocateHomeReqC *locateHomeReq =(CNaviLocateHomeReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_LOCATEHOME, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;

}

void DestinatonHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_DESTINATON)
        return;
    CNaviDestinatonReqC *desinatonReq =(CNaviDestinatonReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_DESTINATON, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;

}

void DestinatonRecordHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_DESTINATONRECORD)
        return;
    CNaviDestinatonRecordReqC *desinatonRecordReq =(CNaviDestinatonRecordReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_DESTINATONRECORD, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void NavigationHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_NAVIGATION)
        return;
    CNaviNavigationReqC *naviGation =(CNaviNavigationReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_NAVIGATION, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;

}

void RealTimeRoadHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_REALTIMEROAD)
        return;
    CNaviRealTimeRoadReqC *realTimeRoadReq =(CNaviRealTimeRoadReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_REALTIMEROAD, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void RoadReportHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_ROADREPORT)
        return;
    CNaviRoadReportReqC *roadReportReq =(CNaviRoadReportReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_ROADREPORT, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void CommonReqHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_COMMONREQ)
        return;
    CNaviCommonReqC *commReq =(CNaviCommonReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_COMMONREQ, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void AddressBookHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_ADDRESSBOOK)
        return;
    CNaviAddressBookReqC *addressBookReq =(CNaviAddressBookReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_ADDRESSBOOK, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void SurroundingSearchHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_SURROUNDINGSEARCH)
        return;
    CNaviSurroundingSearchReqC *surroundingSearchReq =(CNaviSurroundingSearchReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_SURROUNDINGSEARCH, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void DayNightModeHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_DAYNIGHTMODE)
        return;
    CNavDayNightModeReqC *dayNightModeReq =(CNavDayNightModeReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_DAYNIGHTMODE, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void RightButtonRotateHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_RIGHT_BUTTON_ROTATE)
        return;
    CNavRightButtonRotateReqC *rightButtonRotateReq =(CNavRightButtonRotateReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_RIGHT_BUTTON_ROTATE, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void MultiPointHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_MULTIPOINTGPSINFO)
        return;
    CNaviMultipointReqC *multiPointReq =(CNaviMultipointReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_MULTIPOINTGPSINFO, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void SatelliteHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_SATELLITE)
        return;
    CNaviSatelliteUIC *satelliteReq =(CNaviSatelliteUIC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_SATELLITE, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void ShutDownHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_NAVI_SHUTDOWN)
        return;
    CNavShutDownReqC *shutdownReq =(CNavShutDownReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_NAVI_SHUTDOWN, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 0;
    reply=tmpReply;
}

void GuiDayNightModeHandler(DBusRequestC *request, DBusReplyC *&reply)
{
    if(request->getIdentifier() != REQ_ID_GUI_DAYNIGHTMODE)
        return;
    CGuiDayNightModeReqC *guiDayNightModeReq =(CGuiDayNightModeReqC*)request;
    CSingalObject::instance()->Translate(REQ_ID_GUI_DAYNIGHTMODE, 0, 0);

    CNaviCommonRepC *tmpReply = new CNaviCommonRepC();
    tmpReply->result = 1;
    reply=tmpReply;
}

//send gps
int dbus_updategps(int itime,float x,float y)
{
    CNaviGPSInfoReportNotifyC notify;

    notify.fX = x;
    notify.fY = y;
    notify.iTime = itime;
    notify.iSysTime = 0;
    notify.iOrientation = 30;
    notify.iVelocity = 40;
    notify.ialtitude = 0;
    notify.ilatitude = 0;
    notify.ilongitude = 0;
    notify.cPositioning = 1;
    notify.cMode = 0;
    notify.cSatellitenum = 12;
    notify.cBDSatellitenum = 12;

    //printf("%f,%f\n",x,y);
    if(NULL != g_pServer)
    {
        g_pServer->sendNotify(&notify);
    }
    else
    {
        printf("g_pServer Failed to initialize.\r\n");
    }
}

int dbus_postfence(int itime,float x,float y,INT32 nhead)
{
    //printf("dbus_postfence x:%f,y:%f\n",x,y);
    for(int i = 0; i < MAXFENCE; i++)
    if(s_pfencemanager->checkrectarr(i,x,y,0,0))
    {
        CNaviElectronicFenceStateNotifyC notify;
        FenceStateInfo fenceinfo = s_pfencemanager->getRectFenceInfo(i);
        notify.iTime = itime;
        notify.etype = fenceinfo.etype;
        notify.eid = fenceinfo.eid;
        notify.epoint = fenceinfo.epoint;
        notify.estate = fenceinfo.estate;
        notify.edeviation = fenceinfo.edeviation;
        printf("FenceManager::instance()->checkrect(x,y) status changed.x:%f,y:%f\n",x,y);
        if(NULL != g_pServer)
        {
            g_pServer->sendNotify(&notify);
        }
        else
        {
            printf("g_pServer Failed to initialize.\r\n");
        }
    }

    for(int i = 0; i < MAXFENCE; i++)
    if(s_pfencemanager->checkpatharr(i,x,y,nhead,0,0))
    {
        CNaviElectronicFenceStateNotifyC notify;
        FenceStateInfo fenceinfo = s_pfencemanager->getPathFenceInfo(i);
        notify.iTime = itime;
        notify.etype = fenceinfo.etype;
        notify.eid = fenceinfo.eid;
        notify.epoint = fenceinfo.epoint;
        notify.estate = fenceinfo.estate;
        notify.edeviation = fenceinfo.edeviation;
        printf("FenceManager::instance()->checkpath(x,y) status changed.x:%f,y:%f\n",x,y);
        if(NULL != g_pServer)
        {
            g_pServer->sendNotify(&notify);
        }
        else
        {
            printf("g_pServer Failed to initialize.\r\n");
        }
    }

    for(int i = 0; i < MAXFENCE; i++)
    if(s_pfencemanager->checkcirclearr(i,x,y,0,0))
    {
        CNaviElectronicFenceStateNotifyC notify;
        FenceStateInfo fenceinfo = s_pfencemanager->getCircleFenceInfo(i);
        notify.iTime = itime;
        notify.etype = fenceinfo.etype;
        notify.eid = fenceinfo.eid;
        notify.epoint = fenceinfo.epoint;
        notify.estate = fenceinfo.estate;
        notify.edeviation = fenceinfo.edeviation;
        printf("FenceManager::instance()->checkrect(x,y) status changed.x:%f,y:%f\n",x,y);
        if(NULL != g_pServer)
        {
            g_pServer->sendNotify(&notify);
        }
        else
        {
            printf("g_pServer Failed to initialize.\r\n");
        }
    }

    for(int i = 0; i < MAXFENCE; i++)
    if(s_pfencemanager->checkpolygonarr(i,x,y,0,0))
    {
        CNaviElectronicFenceStateNotifyC notify;
        FenceStateInfo fenceinfo = s_pfencemanager->getPolygonFenceInfo(i);
        notify.iTime = itime;
        notify.etype = fenceinfo.etype;
        notify.eid = fenceinfo.eid;
        notify.epoint = fenceinfo.epoint;
        notify.estate = fenceinfo.estate;
        notify.edeviation = fenceinfo.edeviation;
        printf("FenceManager::instance()->checkrect(x,y) status changed.x:%f,y:%f\n",x,y);
        if(NULL != g_pServer)
        {
            g_pServer->sendNotify(&notify);
        }
        else
        {
            printf("g_pServer Failed to initialize.\r\n");
        }
    }
}

void dbus_postnaviinfo(char status,unsigned char tiptype,char* roadName, int distTurn, int distEnd)
{

    CNaviNavigationInfoNotifyC *notify = new CNaviNavigationInfoNotifyC();
    notify->cStatus = status;
    notify->cTipType = tiptype;
    memcpy(notify->cRoadName, roadName, sizeof(char)*50);
    notify->iDistTurn = distTurn;
    notify->iDistEnd = distEnd;				// 到目的地距离
    g_pServer->sendNotify(notify);
//    free((char*)roadName);
}

void CNaviElectronicFenceNotifyHandler(DBusNotifyC *pNotify)
{
    printf("Navi client NOTF_ID_NAVI_ELECTRONIC_FENCE recieve.\n");
    if(pNotify->getIdentifier()!=NOTF_ID_NAVI_ELECTRONIC_FENCE)
            return;
    CNaviElectronicFenceNotifyC *notify=(CNaviElectronicFenceNotifyC*)pNotify;
    s_pfencemanager->receivenotify(notify->eflag, notify->eid, notify->etype, notify->estate);
}


