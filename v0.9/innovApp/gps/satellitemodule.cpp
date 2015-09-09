#include "gps/satellitemodule.h"
#include "innovEngine_types.h"
#include "gps/gps_parse.h"
#include "gps/gn_parse.h"
#include "gps/bd_parse.h"
#include "math.h"
#include "engineinterface/engineinterface.h"
#if defined(ARM_LINUX)
#include "dbus/dbusinterface.h"
#endif

//TPos g_gps_data{11628929, 3996009};   //test
SatelliteModule *g_satelliteModule = new SatelliteModule();

SatelliteModule::SatelliteModule(QObject *parent) :
    QObject(parent)
{
    gpsMode = NAME_GPS_MODE;
    successParse = FALSE ;
}

void SatelliteModule::openGPS()
{

     m_GpsCom =new Posix_QextSerialPort("/dev/ttyGPS",QextSerialBase::Polling);
        //初始化com1端口

     //set port name
     m_GpsCom->setPortName("/dev/ttyGPS");
     m_GpsCom->setBaudRate(BAUD9600); //波特率设置，我们设置为9600
     m_GpsCom->setDataBits(DATA_8); //数据为设置，我们设置为8位数据位。
     m_GpsCom->setParity(PAR_NONE); //奇偶校验设置 为无
     m_GpsCom->setStopBits(STOP_1); //设置一位停止位
     m_GpsCom->setFlowControl(FLOW_OFF); //数据流控制设置，无
     m_GpsCom->setTimeout(10);//设置延时

       if(m_GpsCom->open(QIODevice::ReadWrite))
           printf("open /dev/ttyGPS success!!!");
       else{
           printf("open /dev/ttyGPS failed!!!");
       }

        m_gpsTimer = new QTimer(this);//创建一个定时器
        m_gpsTimer->start(1000);
        updatingGps=true;
        connect(m_gpsTimer,SIGNAL(timeout()),this,SLOT(readGpsDate()),Qt::QueuedConnection);
        printf("*********************************************%s[%d]:",__FILE__,__LINE__);
}

void SatelliteModule::openSimGPS()
{
    readsimfile();
    m_gpsTimer = new QTimer(this);//创建一个定时器
    m_gpsTimer->start(1000);
    updatingGps=true;
    updateSimGPS();
    connect(m_gpsTimer,SIGNAL(timeout()),this,SLOT(updateSimGPS()),Qt::QueuedConnection);
}

void SatelliteModule::readsimfile()
{
    m_startx = 116.28929;
    m_starty = 39.96009;
}

void SatelliteModule::readGpsDate()
{
   
   printf("FILE ********************************************%s[%d]:\n",__FILE__,__LINE__);
   
    QByteArray GPS_Data;
    int newGpsMsg = 0;
    if(updatingGps){
        newGpsMsg = m_GpsCom->bytesAvailable();
        printf("**************updatingGps====%d\n",updatingGps);
    }
    if(newGpsMsg)
    {
        printf("*************newGpsMsg====%d\n",newGpsMsg);
        GPS_Data+= m_GpsCom->read(newGpsMsg);
         
//        qDebug()<<"*************************QByteArray = "<<QByteArray.data()<<"\n";
    }
    else{
        //ui->label->setText(GPS_Data);
        return;//没有数据，就返回
    }
//gps/gn parse


    QList<QByteArray>::iterator iter;
    char ch='\n';
    Gps_list = GPS_Data.split(ch);
    TGPSMODE GPSMode =getGpsMode();
        printf("******************************************************\n");
        printf("gps original data is %s\n",GPS_Data.data());
        printf("********************GPSMode========%d\n",GPSMode);
    for(iter=Gps_list.begin();iter!=Gps_list.end();++iter)
    {

        QByteArray tmpArray=*iter;
        printf("*********readGpsDate*******parse data is %s\n",tmpArray.data());
        if(GPSMode == NAME_GPSANDBD2 )
        {
            printf("*************** NAME_GPSANDBD2***********************\n");
            processGNData(tmpArray.data());
        }
        else if(GPSMode == NAME_GPS_MODE)
        {
             printf("*************** NAME_GPS_MODE***********************\n");
            processGPSData(tmpArray.data());
        }
        else if(GPSMode == NAME_BD2_MODE)
        {
            printf("*************** NAME_BD2_MODE***********************\n");
            processBDData(tmpArray.data());
        }

    }
    if(successParse == TRUE)
    {
        if(GPSMode == NAME_GPSANDBD2 )
        {
            printf("*************** NAME_GPSANDBD2***********************\n");
            if(m_gnParse.effectiveGPS)
            {
                dealSatelliteData(gn_data);
            }

        }
        else if(GPSMode == NAME_GPS_MODE)
        {
            printf("*************** NAME_GPS_MODE***********************\n");
            if(m_gpsParse.effectiveGPS)
            {
                printf("gps[x,y] is %f,%f\n",gps_data.longitude,gps_data.latitude);

                //set realgpsinfo to gps_data;
                dealSatelliteData(gps_data);
            }

        }
        else if(GPSMode == NAME_BD2_MODE)
        {
            if(m_bdParse.effectiveGPS)
            {
                dealSatelliteData(bd_data);
            }

        }
    }
}

void SatelliteModule::updateSimGPS()
{
    float lon = m_startx;
    float lat = m_starty;
    m_starty+=0.001;
    if(m_starty > 40.01)
    {
        m_starty = 39.85;
    }
    if(m_heading >359)
    {
        m_heading = 0;
    }
    else
    {
        m_heading+=5;
    }
#if defined(ARM_LINUX)
    dbus_postfence(0,lon,lat,m_heading);
#endif
    gps_setLongitude(lon * 100000);
    gps_setLatitude(lat * 100000);
    gps_setHeading(m_heading);
    //dbus_postfence(0,gps_data.longitude,gps_data.latitude );
    emit(postlocation(lon, lat,m_heading));
//    printf("[gps][x,y] emit ");
}

void SatelliteModule::changeGPMode()
{
#if defined(ARM_LINUX)
     char* ch = "$CFMOD,0*5F\r\n";
    m_GpsCom->write(ch);
    setGpsMode(NAME_GPS_MODE);
#endif
}

void SatelliteModule ::changeBDMode()
{
#if defined(ARM_LINUX)
    char* ch = "$CFMOD,1*5E\r\n";
    m_GpsCom->write(ch);
    setGpsMode(NAME_BD2_MODE);
#endif
}

void SatelliteModule :: changeGNMode()
{
#if defined(ARM_LINUX)
    char* ch = "$CFMOD,4*5B\r\n";
    m_GpsCom->write(ch);
    setGpsMode(NAME_GPSANDBD2);
#endif
}

void SatelliteModule :: setGpsMode(TGPSMODE tgpsMode)
{
    gpsMode=tgpsMode;
}

TGPSMODE SatelliteModule :: getGpsMode()
{
    return gpsMode;
}

void SatelliteModule :: processGPSData(char *GPSData)
{

    m_gpsParse.gps_processSentence(GPSData);
    successParse =m_gpsParse.successParse;
    gps_data.utc=m_gpsParse.utc;
    gps_data.latitude = float(m_gpsParse.latitude)/100000.0;
    gps_data.longitude=float(m_gpsParse.longitude)/100000.0;
    gps_data.speed = m_gpsParse.speed;
    gps_data.heading =m_gpsParse.heading*256/3600000;
    gps_data.date = m_gpsParse.date;
    gps_data.GPSStatlliteNum =m_gpsParse.GPSStatlliteNum;
    gps_data.BD2StatelliteNum = m_gpsParse.BD2StatelliteNum;
    printf("*********gps_data.utc====%lf\n",gps_data.utc);
    printf("*********gps_data.latitude====%f\n",gps_data.latitude);
    printf("*********gps_data.longitude====%f\n",gps_data.longitude);
    printf("*********gps_data.speed====%d\n",gps_data.speed);
    printf("*********gps_data.heading====%d\n",gps_data.heading);
    printf("*********gps_data.GPSStatlliteNum====%d\n",gps_data.GPSStatlliteNum);
    printf("*********gps_data.BD2StatelliteNum====%d\n",gps_data.BD2StatelliteNum);

}

void SatelliteModule :: processBDData(char *BDData)
{

    m_bdParse.BD_processSentence(BDData);
    successParse = m_bdParse.successParse;
    bd_data.utc=m_bdParse.utc;
    bd_data.latitude = float(m_bdParse.latitude)/100000.0;
    bd_data.longitude=float(m_bdParse.longitude)/100000.0;
    bd_data.speed = m_bdParse.speed;
    bd_data.heading = m_bdParse.heading*256/3600000;
    bd_data.date = m_bdParse.date;
    bd_data.GPSStatlliteNum =m_bdParse.GPSStatlliteNum;
    bd_data.BD2StatelliteNum = m_bdParse.BD2StatelliteNum;

}

void SatelliteModule ::processGNData(char *GNData)
{
    m_gnParse.gn_processSentence(GNData);
    successParse = m_gnParse.successParse;
    gn_data.utc=m_gnParse.utc;
    gn_data.latitude = float(m_gnParse.latitude)/100000.0;
    gn_data.longitude=float(m_gnParse.longitude)/100000.0;
    gn_data.speed = m_gnParse.speed;
    gn_data.heading = m_gnParse.heading*256/3600000;
    gn_data.date = m_gnParse.date;
    gn_data.GPSStatlliteNum =m_gnParse.GPSStatlliteNum;
    gn_data.BD2StatelliteNum = m_gnParse.BD2StatelliteNum;
}

void SatelliteModule :: dealSatelliteData(const MGPSGPRMC &m_satelliteData)
{
#if defined(ARM_LINUX)
        dbus_updategps(m_satelliteData.utc, m_satelliteData.longitude,m_satelliteData.latitude);
        dbus_postfence(m_satelliteData.utc,m_satelliteData.longitude ,m_satelliteData.latitude,m_satelliteData.heading);
        gps_setLongitude(m_satelliteData.longitude * 100000);
        gps_setLatitude(m_satelliteData.latitude * 100000);
        gps_setHeading(m_heading);
        emit(postlocation(m_satelliteData.longitude, m_satelliteData.latitude,m_satelliteData.heading));
#endif
}

float SatelliteModule :: getLatitude()
{
    switch(gpsMode)
    {
        case NAME_GPS_MODE :
            return gps_data.latitude;
            break;
        case NAME_BD2_MODE :
            return bd_data.latitude;
            break;
        case  NAME_GPSANDBD2 :
            return gn_data.latitude;
            break;
    }
}

float SatelliteModule :: getLongitude()
{
    switch (gpsMode)
    {
        case NAME_GPS_MODE :
            return gps_data.longitude;
            break;
        case NAME_BD2_MODE :
            return bd_data.longitude;
            break;
        case NAME_GPSANDBD2 :
            return gn_data.longitude;
            break;
    }
}

int SatelliteModule :: getHeading()
{
    switch (gpsMode)
    {
        case NAME_GPS_MODE :
            return gps_data.heading;
            break;
        case NAME_BD2_MODE :
            return bd_data.heading;
            break;
        case NAME_GPSANDBD2 :
            return gn_data.heading;
            break;
    }
}
