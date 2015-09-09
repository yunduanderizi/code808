#ifndef DBUSINTERFACE_H
#define DBUSINTERFACE_H

#include "dbusMessage.h"
#include "dbusServer.h"
#include "dbusApplication.h"
#include "dbusDebug.h"
#include "dbusClientProxy.h"
#include "NaviDbusMessage.h"
#include "debugMonitor.h"
#include "stdio.h"
#include "gps/gn_parse.h"
#include "gps/gps_parse.h"
#include "gps/bd_parse.h"

extern DBusServerC* g_pServer;
extern DBusClientProxyC* g_pClient;

//bool init_serverdbus();
bool init_clientdbus();
void ShowMenuHandler(DBusRequestC *request, DBusReplyC *&reply);
void CarLocationHandler(DBusRequestC *request, DBusReplyC *&reply);
void GPSInfoReportCycleHandler(DBusRequestC *request, DBusReplyC *&reply);
void RoadBookHandler(DBusRequestC *request, DBusReplyC *&reply);
void RoadBook2Handler(DBusRequestC *request, DBusReplyC *&reply);
void HospitalHandler(DBusRequestC *request, DBusReplyC *&reply);
void GPSInfoHandler(DBusRequestC *request, DBusReplyC *&reply);
void WindowDisplayControlInfoHandler(DBusRequestC *request, DBusReplyC *&reply);
void GasStationHandler(DBusRequestC *request, DBusReplyC *&reply);
void ParkHandler(DBusRequestC *request, DBusReplyC *&reply);
void LocateCarHandler(DBusRequestC *request, DBusReplyC *&reply);
void LocateHomeHandler(DBusRequestC *request, DBusReplyC *&reply);
void DestinatonHandler(DBusRequestC *request, DBusReplyC *&reply);
void DestinatonRecordHandler(DBusRequestC *request, DBusReplyC *&reply);
void NavigationHandler(DBusRequestC *request, DBusReplyC *&reply);
void RealTimeRoadHandler(DBusRequestC *request, DBusReplyC *&reply);
void RoadReportHandler(DBusRequestC *request, DBusReplyC *&reply);
void CommonReqHandler(DBusRequestC *request, DBusReplyC *&reply);
void AddressBookHandler(DBusRequestC *request, DBusReplyC *&reply);
void SurroundingSearchHandler(DBusRequestC *request, DBusReplyC *&reply);
void DayNightModeHandler(DBusRequestC *request, DBusReplyC *&reply);
void RightButtonRotateHandler(DBusRequestC *request, DBusReplyC *&reply);
void MultiPointHandler(DBusRequestC *request, DBusReplyC *&reply);
void SatelliteHandler(DBusRequestC *request, DBusReplyC *&reply);
void ShutDownHandler(DBusRequestC *request, DBusReplyC *&reply);
void GuiDayNightModeHandler(DBusRequestC *request, DBusReplyC *&reply);



void notifyinfo();
int dbus_updategps(int itime,float x,float y);
int dbus_postfence(int itime,float x,float y,INT32 nhead);
void dbus_postnaviinfo(char status,unsigned char tiptype,char* roadName, int distTurn, int distEnd);
void CNaviElectronicFenceNotifyHandler(DBusNotifyC *pNotify);
#endif // DBUSINTERFACE_H
