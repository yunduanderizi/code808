#ifndef GPS_PARSE_H
#define GPS_PARSE_H

#include<stdio.h>
#include<vector>
#include"platform_types.h"
#include <QObject>
#include <innovEngine_sdk.h>


#define MAX_PATH          260
#define MAX_FIELD_LENGTH 16
#define FIELD_BUFFER_LENGTH (MAX_FIELD_LENGTH + 1)
#define TIME_BAD_VALUE (UINT32)0xFFFFFFFFL
#define DATE_BAD_VALUE (UINT32)0xFFFFFFFFL
#define GPS_BADVALUE ( (INT32)0xFFFFFFFFL )
#define FILTER_GPS_TIMES_FORM_UNFIX_TO_FIX 3


using namespace std;

typedef struct TGPSGPRMC {
    UINT32 sentence;
    UINT32 utc;
    char status;
    INT32 latitude; // degrees x 10^5
    INT32 longitude; // degrees x 10^5
    INT32 speed; // x 10 ^ 4
    INT32 heading; // x 10 ^ 4
    UINT32 date;
} TGPSGPRMC;

typedef struct TGPSGPGGA {
  UINT32 sentence;
  UINT32 utc;
  INT32 latitude; // degrees x 10^5
  INT32 longitude; // degrees x 10^5
  char quality;
  UINT8 satellitesUsed;
  INT32 altitude; // x 10^4
} TGPSGPGGA;

typedef struct TGPSGPGSA {
  UINT32 sentence;
  char mode;
  UINT8 fixType;
  UINT8 satellitesUsed;
  UINT8 satelliteIDs[12];
  INT32 pdop; // x 10^4
  INT32 hdop; // x 10^4
  INT32 vdop; // x 10^4
} TGPSGPGSA;

typedef struct TGPSGPGSV {
  UINT32 sentence;
  UINT8 satelliteCount;
  //UINT8 prn[12];
  //UINT8 elevation[12];
  //UINT16 azimuth[12];
  //UINT8 snr[12];
  UINT8 prn[27];
  UINT8 elevation[27];
  UINT16 azimuth[27];
  UINT8 snr[27];
  UINT8 satelliteTracked;
} TGPSGPGSV;

/** Structure for maintaining internal traffic message state */
typedef struct GNTPTTKT {
  TtmcMessageType lastMessage;
  UINT8           tmcData[8];
  UINT8           hardwarePresent;
  UINT8           blockCount;
  UINT8           tmcCount;
  UINT8           errorRate;
  UINT16          frequency;
  UINT8           tuneStatus;
  // for station list
  UINT8           stnCount;
  UINT8           stnIndex;
  BOOL8           stnCurrent;
  UINT16          stnFrequency;
  UINT16          stnPI;
  UINT8           stnSignal;
  UINT8           stnFlag;
  char            stnName[20];
} GNTPTTKT;

class navcoreGPSParse
{
public:

    bool successParse ;
    UINT32 utc;
    INT32 latitude; // degrees x 10^5
    INT32 longitude; // degrees x 10^5
    INT32 speed; // x 10 ^ 4
    INT32 heading; // x 10 ^ 4
    UINT32 date;
    UINT8 satellitesUsed;
    int GPSStatlliteNum;
    int BD2StatelliteNum;
    bool effectiveGPS;

    navcoreGPSParse();
    ~navcoreGPSParse();
    void processGPSFile(char *logFile);
    bool checkGPSChecksum(char *sentence);

    BOOL8 gps_processSentence(char *sentence);
    void clear( void );
    bool parseFile( );



public:
    vector<TGPSGPRMC> m_gpsLocs;
    vector<TGPSGPRMC> m_maplockLocs;
private:
    bool processGPGSV(const char *sentence);
    bool processGPGGA(const char *sentence);
    bool processGPGSA(const char *sentence);
    bool processGPRMC(const char *sentence);
    BOOL8 processPTTKT(const char *sentence);

    const char *findNextField(const char *sentence);
    UINT8 readField(char *dest, const char *sentence);
    INT32 decodeTimeValue(const char *text);
    INT32 decodeLatLong(const char *text);
    INT32 decodeFloatValue(const char *text);
    UINT8 hexToBinary (const char *hex);
    INT32 decodeIntegerValue(const char *text);

    void initGPGSV();
    void initGPGGA();
    void initGPGSA();
    void initGPRMC();



private:
    //set to true when the final GPGSV message is received
    BOOL8 gps_Rx_GPGSV1;
    BOOL8 gps_Rx_GPGSV2;
    BOOL8 gps_Rx_GPGSV3;
    //@MIC
    BOOL8 gps_Rx_GPGSV4;
    BOOL8 gps_Rx_GPGSV5;
    BOOL8 gps_Rx_GPGSV6;
    //@MIC
    BOOL8 gps_fullGPGSV;

};




#endif // GPS_PARSE_H
