#ifndef BD_PARSE_H
#define BD_PARSE_H

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


typedef struct TGPSBDRMC {
    UINT32 sentence;
    UINT32 utc;
    char status;
    INT32 latitude; // degrees x 10^5
    INT32 longitude; // degrees x 10^5
    INT32 speed; // x 10 ^ 4
    INT32 heading; // x 10 ^ 4
    UINT32 date;
} TGPSBDRMC;

typedef struct TGPSBDGGA {
  UINT32 sentence;
  UINT32 utc;
  INT32 latitude; // degrees x 10^5
  INT32 longitude; // degrees x 10^5
  char quality;
  UINT8 satellitesUsed;
  INT32 altitude; // x 10^4
} TGPSBDGGA;

typedef struct TGPSBDGSA {
  UINT32 sentence;
  char mode;
  UINT8 fixType;
  UINT8 satellitesUsed;
  UINT8 satelliteIDs[12];
  INT32 pdop; // x 10^4
  INT32 hdop; // x 10^4
  INT32 vdop; // x 10^4
} TGPSBDGSA;

typedef struct TGPSBDGSV {
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
} TGPSBDGSV;

/** Structure for maintaining internal traffic message state */
typedef struct BDTPTTKT {
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
} BDTPTTKT;


class navcoreBDParse
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

    navcoreBDParse();
    ~navcoreBDParse();
    void processBDSFile(char *logFile);
    bool checkBDChecksum(char *sentence);

    BOOL8 BD_processSentence(char *sentence);
    void clear( void );
    bool parseFile( );

private:
    bool processBDGSV(const char *sentence);
    bool processBDGGA(const char *sentence);
    bool processBDGSA(const char *sentence);
    bool processBDRMC(const char *sentence);
    BOOL8 processPTTKT(const char *sentence);

    const char *findNextField(const char *sentence);
    UINT8 readField(char *dest, const char *sentence);
    INT32 decodeTimeValue(const char *text);
    INT32 decodeLatLong(const char *text);
    INT32 decodeFloatValue(const char *text);
    UINT8 hexToBinary (const char *hex);
    INT32 decodeIntegerValue(const char *text);

    void initBDGSV();
    void initBDGGA();
    void initBDGSA();
    void initBDRMC();



private:
    //set to true when the final BDGSV message is received
    BOOL8 gps_Rx_BDGSV1;
    BOOL8 gps_Rx_BDGSV2;
    BOOL8 gps_Rx_BDGSV3;
    //@MIC
    BOOL8 gps_Rx_BDGSV4;
    BOOL8 gps_Rx_BDGSV5;
    BOOL8 gps_Rx_BDGSV6;
    //@MIC
    BOOL8 gps_fullBDGSV;

};

#endif // BD_PARSE_H
