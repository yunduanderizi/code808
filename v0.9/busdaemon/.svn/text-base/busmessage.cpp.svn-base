#include "busmessage.h"
#include <stdlib.h>
#include <string>
#include "McuMessageHandler.h"
#include "DeviceManagerDbusMessage.h"

using namespace std;

extern DBusServerC    *g_pDbusServer;
extern McuMessageHandler mcuHandler;

void SendPosPassengerNotify(int index, int address, int upnumber, int downnumber)
{
    BUSDPosPassengerDbusNotifyC notify;
    
    notify.m_index = index;
    notify.m_address = address;
    notify.m_upNumber = upnumber;
    notify.m_downNumber = downnumber;
    printf("--SendPosPassengerNotify---(%d,%d,%d,%d)\n",index,address,upnumber,downnumber);
    g_pDbusServer->sendNotify(&notify);

    return;
}

void SendPosInfoNotify(string &filename)
{
    BUSDPosInfoUpdateDbusNotifyC notify;
    
    notify.m_filename = (char *)malloc(filename.length() + 1);
    memset(notify.m_filename, 0, filename.length() + 1);
    filename.copy(notify.m_filename, filename.length());
    
    printf("-11111--SendPosInfoNotify----(%s,)\n",notify.m_filename);
    g_pDbusServer->sendNotify(&notify);

    return;
}

void SendCanDataNotify(string &filename,int num)
{
    BUSDCarINfoUpdateDbusNotifyC notify;
    
    notify.m_filename = (char *)malloc(filename.length() + 1);
    memset(notify.m_filename, 0, filename.length() + 1);
    filename.copy(notify.m_filename, filename.length());
    notify.m_num = num;
    notify.m_itime = (int)time(NULL);
    printf("-11111--SendCanDataNotify----(%s,%d)\n",notify.m_filename,num);
    g_pDbusServer->sendNotify(&notify);

    return;
}

void DeviceManagerDrivingStateHandler(DBusNotifyC *pNotify)
{
    if(pNotify->getIdentifier() != DEVICE_MANAGER_NOTF_ID_DRIVING_STATE)
    {
        return;
    }
    DeviceDrivingStateDbusNotifyC *Notify= (DeviceDrivingStateDbusNotifyC *)pNotify;
    
    int flag = Notify->flag;
    
    mcuHandler.SendLEDState(flag);
    
    return;
}

void SendStationInfoNotify(int num, string &name, int type)
{
    BUSDStationUpdateDbusNotifyC  notify;

    notify.m_number = num;
    notify.m_name = (char *)malloc(name.length() + 1);
    memset(notify.m_name, 0, name.length() + 1);
    name.copy(notify.m_name, name.length());
    notify.m_op_type = type;
    printf("---SendStationInfoNotify--len=%d--(%d,%d,%s)\n",name.length(),num,type,notify.m_name);
    g_pDbusServer->sendNotify(&notify);

    return;
}

void SendStationPassengerNotify(int num, string &name, int upnumber, int downnumber)
{
    BUSDStationPassengerDbusNotifyC notify;

    notify.m_time = (int)time(NULL);
    notify.m_number = num;
    memset(notify.m_name, 0, 128);
    memcpy(notify.m_name, name.c_str(), name.size());
    notify.m_upNumber = upnumber;
    notify.m_downNumber = downnumber;
    printf("--11111-SendStationPassengerNotify----(%d,%s,%s,%d,%d)\n",num,name.c_str(), notify.m_name, upnumber,downnumber);   

    g_pDbusServer->sendNotify(&notify);

    return;
}

