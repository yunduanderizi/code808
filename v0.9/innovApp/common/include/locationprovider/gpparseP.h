#ifndef GPPARSEP_H
#define GPPARSEP_H

#include <locationprovider/gpparse.h>

#ifdef __cplusplus
extern "C" {
#endif


// BEGIN PRIVATE

// speed threshold for accurate heading values from the GPS system
// units: 1/10000th of a knot
#define GPS_TOO_SLOW_FOR_HEADING 5000


/*----------------------------------------------------------------*/
// the various GPS sentences are decoded into these structures so that
// values can be read directly as numerical values.

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

/*----------------------------------------------------------------*/

BOOL8 gps_isValidGPS(void);

INT32 gps_getMMPS(void);

// Forward declaration
struct TViewport;

extern void gpsStats_render(const struct TViewport *viewport);

// END PRIVATE

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------*/

#endif /* GPPARSEP_H */
