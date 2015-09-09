#ifndef _BUS_MESSAGE_H_
#define _BUS_MESSAGE_H_

#include <stdio.h>
#include <dbusServer.h>
#include <dbusApplication.h>
#include <string>

#define BUSD_DBUS_SERVER_ID                (char*)"com.innov.busdaemon"
#define BUSD_DBUS_SERVER_OBJ               (char*)"/busdaemon"

#define BUSD_DBUS_NTF_ID_CARINFO_UPLOAD             7100
#define BUSD_DBUS_NTF_ID_POSINFO_UPLOAD             7101
#define BUSD_DBUS_NTF_ID_STATION_UPDATE             7102
#define BUSD_DBUS_NTF_ID_PASSENGER_UPDATE           7103
#define BUSD_DBUS_NTF_ID_STATION_PASSENGER_UPDATE   7104

using namespace std;

class BUSDCarINfoUpdateDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(BUSDCarINfoUpdateDbusNotifyC)

public:
    BUSDCarINfoUpdateDbusNotifyC() : DBusNotifyC(BUSD_DBUS_NTF_ID_CARINFO_UPLOAD)
	{
        m_filename = NULL;
        DBUS_STRING_MEMBER(m_filename);
	}

    ~BUSDCarINfoUpdateDbusNotifyC()
    {
        if(m_filename)
        {
            free(m_filename);
        }
    }	
    void print(FILE *fp) {fprintf(fp,"\tfilename=%s\r\n",m_filename);}
    
    char *m_filename;
    int m_num;
    int m_itime;
};

class BUSDPosInfoUpdateDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(BUSDPosInfoUpdateDbusNotifyC)

public:
    BUSDPosInfoUpdateDbusNotifyC() : DBusNotifyC(BUSD_DBUS_NTF_ID_POSINFO_UPLOAD)
	{
        m_filename = NULL;
        DBUS_STRING_MEMBER(m_filename);
	}

    ~BUSDPosInfoUpdateDbusNotifyC()
    {
        if(m_filename)
        {
            free(m_filename);
        }
    }	
    void print(FILE *fp) {fprintf(fp,"\tfilename=%s\r\n",m_filename);}
    
    char *m_filename;
};

class BUSDStationUpdateDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(BUSDStationUpdateDbusNotifyC)

public:
    BUSDStationUpdateDbusNotifyC() : DBusNotifyC(BUSD_DBUS_NTF_ID_STATION_UPDATE)
	{
        m_name = NULL;
        DBUS_STRING_MEMBER(m_name);
	}

    ~BUSDStationUpdateDbusNotifyC()
    {
        if(m_name)
        {
            free(m_name);
        }
    }	
    void print(FILE *fp)
	{
	    fprintf(fp,"\tnumber=%d\r\n",m_number);
	    fprintf(fp,"\tname=%s\r\n",m_name);
	    fprintf(fp,"\top_type=%d\r\n",m_op_type);
    }
    
	int m_number;
    char *m_name;
	int m_op_type; // 0: enter 1: exit
};

class BUSDPosPassengerDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(BUSDPosPassengerDbusNotifyC)

public:
    BUSDPosPassengerDbusNotifyC() : DBusNotifyC(BUSD_DBUS_NTF_ID_PASSENGER_UPDATE)
	{
        ;
	}

    ~BUSDPosPassengerDbusNotifyC()
    {
        ;
    }	
    void print(FILE *fp)
	{
	    fprintf(fp,"\tindex=%d\r\n",m_index);
	    fprintf(fp,"\taddress=%d\r\n",m_address);
	    fprintf(fp,"\tupnumber=%d\r\n",m_upNumber);
	    fprintf(fp,"\tdownname=%d\r\n",m_downNumber);
    }
    
    int m_index;
    int m_address;
	int m_upNumber;
	int m_downNumber;
};


class BUSDStationPassengerDbusNotifyC: public  DBusNotifyC
{
    DBUS_NOTIFY_OBJECT(BUSDStationPassengerDbusNotifyC)

public:
    BUSDStationPassengerDbusNotifyC() : DBusNotifyC(BUSD_DBUS_NTF_ID_STATION_PASSENGER_UPDATE)
	{
        ;
	}

    ~BUSDStationPassengerDbusNotifyC()
    {
        ;
    }	
    void print(FILE *fp)
	{
	    fprintf(fp,"\ttime=%d\r\n",m_time);
	    fprintf(fp,"\tnumber=%d\r\n",m_number);
	    fprintf(fp,"\tupnumber=%d\r\n",m_upNumber);
	    fprintf(fp,"\tdownname=%d\r\n",m_downNumber);
    }

    int m_time;
    int m_number;
    char m_name[128];
	int m_upNumber;
	int m_downNumber;
};

void SendPosPassengerNotify(int index, int address, int upnumber, int downnumber);
void SendPosInfoNotify(string &filename);
void DeviceManagerDrivingStateHandler(DBusNotifyC *pNotify);
void SendStationInfoNotify(int num, string &name, int type);
void SendStationPassengerNotify(int num, string &name, int upnumber, int downnumber);
void SendCanDataNotify(string &filename,int num);

#endif
