#ifndef NAVCORESATELLITE_H
#define NAVCORESATELLITE_H

#include <QObject>
#include <qtimer.h>
#include <vector>
#include "gps/posix_qextserialport.h"
#include "innovEngine_types.h"
#include "gps/gn_parse.h"
#include "gps/bd_parse.h"
#include "gps/gps_parse.h"

typedef struct MGPSGPRMC {
   UINT32 sentence;
   UINT32 utc;
   char status;
   float latitude; // degrees x 10^5
   float longitude; // degrees x 10^5
   INT32 speed; // x 10 ^ 4
   INT32 heading; // x 10 ^ 4
   UINT32 date;
   int GPSStatlliteNum;
   int BD2StatelliteNum;
 } MGPSGPRMC;

typedef enum TGPSMODE
{
    NAME_NONE_MODE =-1,
    NAME_GPS_MODE = 0,
    NAME_BD2_MODE,
    NAME_GPSANDBD2
}TGPSMODE;

class SatelliteModule;

extern SatelliteModule *g_satelliteModule;

class SatelliteModule : public QObject
{
    Q_OBJECT
public:
    explicit SatelliteModule(QObject *parent = 0);
    bool updatingGps;
     Posix_QextSerialPort* m_GpsCom;
     void openGPS();
     void openSimGPS();

     QByteArray GPS_STREAM;
     QList<QByteArray> Gps_list;//GPS 信息容器
     QTimer *simTimer;
     QTimer *m_gpsTimer;
     float m_startx;
     float m_starty;
     float m_heading;
signals:
    void postlocation(float flon,float flat,INT32 heading);
public slots:
     void readGpsDate();//读GPS设备数据
     void updateSimGPS();
     void setGpsMode(TGPSMODE tgpsMode);
     TGPSMODE getGpsMode();
     void changeGPMode();
     void changeBDMode();
     void changeGNMode();
     void processGPSData( char * GPSData);
     void processGNData( char * GNData);
     void processBDData( char * BDData);
     void dealSatelliteData(const MGPSGPRMC &m_satelliteData);
     float getLatitude();
     float getLongitude();
     int getHeading();

private:
     void readsimfile();
     TGPSMODE gpsMode;
     MGPSGPRMC gps_data;
     MGPSGPRMC gn_data;
     MGPSGPRMC bd_data;
     bool successParse ;
     navcoreGNParse m_gnParse;
     navcoreBDParse m_bdParse;
     navcoreGPSParse m_gpsParse;

};

#endif // NAVCORESATELLITE_H
