#ifndef GN_PARSE_H
#define GN_PARSE_H

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


typedef struct TGPSGNRMC {
    UINT32 sentence;
    UINT32 utc;
    char status;
    INT32 latitude; // degrees x 10^5
    INT32 longitude; // degrees x 10^5
    INT32 speed; // x 10 ^ 4
    INT32 heading; // x 10 ^ 4
    UINT32 date;
} TGPSGNRMC;

typedef struct TGPSGNGGA {
  UINT32 sentence;
  UINT32 utc;
  INT32 latitude; // degrees x 10^5
  INT32 longitude; // degrees x 10^5
  char quality;
  UINT8 satellitesUsed;
  INT32 altitude; // x 10^4
} TGPSGNGGA;

typedef struct TGPSGNGSA {
  UINT32 sentence;
  char mode;
  UINT8 fixType;
  UINT8 satellitesUsed;
  UINT8 satelliteIDs[12];
  INT32 pdop; // x 10^4
  INT32 hdop; // x 10^4
  INT32 vdop; // x 10^4
} TGPSGNGSA;

typedef struct TGPSGNGSV {
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
} TGPSGNGSV;

/** Structure for maintaining internal traffic message state */
typedef struct TPTTKT {
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
} TPTTKT;


class navcoreGNParse
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

    navcoreGNParse();
    ~navcoreGNParse();
    void processGNSFile(char *logFile);
    bool checkGNChecksum(char *sentence);

    BOOL8 gn_processSentence(char *sentence);
    void clear( void );
    bool parseFile( );

private:
    bool processGNGSV(const char *sentence);
    bool processGNGGA(const char *sentence);
    bool processGNGSA(const char *sentence);
    bool processGNRMC(const char *sentence);
    BOOL8 processPTTKT(const char *sentence);

    const char *findNextField(const char *sentence);
    UINT8 readField(char *dest, const char *sentence);
    INT32 decodeTimeValue(const char *text);
    INT32 decodeLatLong(const char *text);
    INT32 decodeFloatValue(const char *text);
    UINT8 hexToBinary (const char *hex);
    INT32 decodeIntegerValue(const char *text);

    void initGNGSV();
    void initGNGGA();
    void initGNGSA();
    void initGNRMC();



private:
    //set to true when the final GNGSV message is received
    BOOL8 gps_Rx_GNGSV1;
    BOOL8 gps_Rx_GNGSV2;
    BOOL8 gps_Rx_GNGSV3;
    //@MIC
    BOOL8 gps_Rx_GNGSV4;
    BOOL8 gps_Rx_GNGSV5;
    BOOL8 gps_Rx_GNGSV6;
    //@MIC
    BOOL8 gps_fullGNGSV;

};

#endif // GN_PARSE_H
