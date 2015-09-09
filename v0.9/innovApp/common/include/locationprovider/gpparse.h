#ifndef GPPARSE_H
#define GPPARSE_H

// this module receives NMEA strings one character at a time and parses
// them into one of 4 structures depending on the type of sentence received.
// 4 sentences are recognized, these are:
// $GPGGA
// $GPRMC
// $GPGSA
// $GPGSV

#include <innovEngine_types.h>
#include <language/utf16.h>
#include <time/tmtime.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup GPS
/// Parse a GPS positioning stream and retrieve details.
/// @ingroup LocationProviders
/// @{

/** this value is used in various GPS fields to denote a bad value */
// Want to use a proper GPS_BADVALUE here that doesn't break
// latitude and longitude, but GPS_BADVALUE is also used for speed
// so must use a different system there.
// #define GPS_BADVALUE ( (INT32)0x80000000L )
#define GPS_BADVALUE ( (INT32)0xFFFFFFFFL )

/** This value if the maximum value that the UTC can be. It is equal
    to the number of milliseconds in 24 hours. */
#define GPS_MAXIMUM_UTC (UINT32)86400000L
#define GPS_INVALID_UTC (UINT32)(GPS_MAXIMUM_UTC + 1)
#define GPS_HEADING_BADVALUE (INT32)3600001L

/** Odometer callback function prototype. */
typedef void TOdometerFunc( INT32 lon,
                            INT32 lat,
                            UINT32 time,
                            UINT32 speed );

/** Enumeration of the types of NMEA sentence received.
    Currently only 5 sentences are parsed, these are:
    - $GPGGA
    - $GPRMC
    - $GPGSA
    - $GPGSV
    - $PTTKT */
typedef enum {
  /** The sentence is of an invalid or unrecognised type */
  NMEA_MESSAGE_TYPE_INVALID = -1,
  /** The sentence is Global Positioning System Fix Data (GGA) */
  NMEA_MESSAGE_TYPE_GGA = 0,
  NMEA_MESSAGE_TYPE_GLL,
  /** The sentence is GPS DOP and Active Satellites data (GSA) */
  NMEA_MESSAGE_TYPE_GSA,
  /** The sentence is GPS Satellites in view data (GSV) */
  NMEA_MESSAGE_TYPE_GSV,
  /** The sentence is Recommended Minimum Specific GPS/TRANSIT Data (RMC) */
  NMEA_MESSAGE_TYPE_RMC,
  NMEA_MESSAGE_TYPE_VTG,
  /** the sentence is proprietary NMEA traffic message */
  NMEA_MESSAGE_TYPE_TRAFFIC
} NMEA_MESSAGE_TYPES;
 
/** Enumeration of the possible sources for GPS data. */
typedef enum TGPSContext {
  /** Stop return valid data */
  NMEA_CONTEXT_NONE = -1,
  /** Return data obtained from the Primary GPS reciever */
  NMEA_CONTEXT_GPS = 0,
  /** Return data from the simulator */
  NMEA_CONTEXT_SIMULATOR,
  /** Return data from the 3D flyover */
  NMEA_CONTEXT_FLYOVER,
  /** Return data from a log file */
  NMEA_CONTEXT_LOGFILE,
  /** Only allow manual updates of the GPS data */
  NMEA_CONTEXT_MANUAL,
  /** Return data from the tunnel simulator */
  NMEA_TUNNEL_SIMULATOR
} TGPSContext;

/** Statistics maintained by the GPS parser. */
typedef struct TGPSStats {
  /// number of times a GPS string overlows the 80 char buffer
  UINT32 bufferOverflows;
  /// number of times a processSentence() gets a checksum error
  UINT32 checksumFailures;
  /// number of times processSentence() parses the NMEA message correctly
  UINT32 understood;
  /// number of times processSentence() is called
  UINT32 total;
} TGPSStats;

/** Enum to indicate last traffic message type */
typedef enum TtmcMessageType {
  TMC_MSG_TYPE_DATA = 0,
  TMC_MSG_TYPE_STATUS,
  TMC_MSG_TYPE_TUNE,
  TMC_MSG_TYPE_LIST,
  TMC_MSG_TYPE_RESET,
  TMC_MSG_TYPE_SIGSTRENGTH,
  TMC_MSG_TYPE_WAKEUP
} TtmcMessageType;

  /* The enum and TStationList structure below are here
     Temporarily until some header inclusion order issues are worked out */
  typedef enum TstnTMCStatus
  {
     STN_0 = 0,
     STN_1,
     STN_2,
     STN_3,
     STN_4
  } TstnTMCStatus;

  typedef struct TStationList
  {
    UINT8     maxIndex;           ///< Total Number of station entries
    UINT8     entryIndex;         ///< Index of this entry
    BOOL8     currentStation;     ///< Tuner currently tuned to this frequency
    UINT16    frequency;          ///< Frequency with implied decmal point
    UINT16    piCode;             ///< PI Code for station
    UINT8     signalStrength;     ///< last measured signal strength
    TstnTMCStatus tmcFlag;        ///< Does this station transmit TMC 
    UTF16     stationName[20];
} TStationList;

/** typedef for traffic callback function */
typedef void TTrafficFunc (TtmcMessageType msgType);

/** This function informs the GPS module to begin file playback.
    The log file should already have been opened with a call to 
    gps_openLoadFile.  If a valid logfile is open, the context is 
    changed to to NMEA_CONTEXT_LOGFILE.
    
    @return Returns FALSE if the log file has not been opened, otherwise 
    returns TRUE.
    
    @par Example:
    @code
    if ( !gps_startReplay() ) {
    os_printf("No log file open!");
    }
    @endcode
    
    @todo THIS FUNCTION SHOULD PROBABLY BE REMOVED FROM THE GPS API */
extern MAPCORE_API BOOL8 gps_startReplay( void );

/** Stops the log file from replaying, and closes the file.
    Returns context to the previous state (before the call to 
    gps_startReplay).
    
    @return Always returns TRUE
    
    @par Example:
    @code
    if ( !gps_stopReplay() ) {
    os_printf("Couldn't close log file!");
    }
    @endcode
    
    @todo THIS FUNCTION SHOULD PROBABLY BE REMOVED FROM THE GPS API */
extern MAPCORE_API BOOL8 gps_stopReplay( void );

/** This function opens a text file, and any subsequent complete NMEA 
    sentences are saved to this file. If a file is already open for 
    saving, it is first closed.
    
    @param filename Points to the name of the file to open.
    
    @return A pointer to the opened file, or NULL if the file was not 
    opened successfully.
    
    @par Example:
    @code
    OS_FILE *outFile;
    outFile = gps_openSaveFile("c:\\gps_logs\\log.txt");
    @endcode
    
    @todo THIS FUNCTION SHOULD PROBABLY BE REMOVED FROM THE GPS API */
extern MAPCORE_API OS_FILE gps_openSaveFile( const char *filename );

/** This function opens a text file for reading. The file should contain 
    valid NMEA sentences.  If a file is already open, it is first closed.
    
    @param filename Points to the name of the file to open.
    
    @return A pointer to the opened file, or NULL if the file was not 
    opened successfully.
    
    @par Example:
    @code
    OS_FILE *inFile;
    inFile = gps_openLoadFile("c:\\gps_logs\\log.txt");
    @endcode
    
    @todo THIS FUNCTION SHOULD PROBABLY BE REMOVED FROM THE GPS API */
extern MAPCORE_API OS_FILE gps_openLoadFile( const char *filename );

/** This function closes the currently open log file.  That is, the file 
    that was opened by a call to the function gps_openSaveFile
    
    @par Example:
    @code
    OS_FILE *outFile;
    outFile = gps_openSaveFile("c:\\gps_logs\\log.txt");
    ...
    gps_closeSaveFile();
    @endcode
    
    @todo THIS FUNCTION SHOULD PROBABLY BE REMOVED FROM THE GPS API. */
extern MAPCORE_API void gps_closeSaveFile( void );

/** This function closes the currently open replay file.  That is, the 
    file that was opened by a call to the function gps_openLoadFile.
    
    @par Example:
    @code
    OS_FILE *inFile;
    inFile = gps_openLoadFile("c:\\gps_logs\\log.txt");
    ...
    gps_closeLoadFile();
    @endcode
    
    @todo THIS FUNCTION SHOULD PROBABLY BE REMOVED FROM THE GPS API. */
extern MAPCORE_API void gps_closeLoadFile( void );

/** Determines if the GPS is currently replaying a log file.
    
    @return Returns TRUE is a log file is open, and replaying (ie: after 
    a call to gps_startReplay).  Otherwise, returns FALSE
    
    @par Example:
    @code
    if ( gps_isReplay() ) {
    os_printf("Currently replaying");
    } else {
    os_printf("NOT currently replaying");
    }
    @endcode
    
    @todo THIS FUNCTION SHOULD PROBABLY BE REMOVED FROM THE GPS API. */
extern MAPCORE_API BOOL8 gps_isReplaying( void );

/** Used to determine if the GPS is currently recording a log file.
    Logging occurs automatically when a save file has been opened.
    That is, there is no logging equivalent to gps_startReplay and 
    gps_stopReplay.
    
    @return Returns TRUE is a log file is open. Otherwise, returns FALSE.
    
    @par Example:
    @code
    if ( gps_isLogging() ) {
    os_printf("Currently logging");
    } else {
    os_printf("NOT currently logging");
    }
    @endcode
    
    @todo THIS FUNCTION SHOULD PROBABLY BE REMOVED FROM THE GPS API. */
extern MAPCORE_API BOOL8 gps_isLogging( void );

/** Attempts to decode a stream of characters representing a NMEA string. 
    If there is a log file open, a call to this function will read a line 
    from the log file, and process that. The first 6 characters determine 
    the type of NMEA string.  If the checksum is valid, the sentence is 
    passed to one of the functions processGPGGA, processGPGSA, processGPRMC 
    or processGPGSV, as appropriate.
    
    @param sentence Points to the memory location of the NMEA string. If 
    replaying, this is ignored, and the next line from the replay file is 
    used instead.
    
    @return Returns TRUE if the sentence is complete, and the checksum is 
    valid. Otherwise, returns FALSE.
    
    @par Example:
    @code
    BOOL8 gps_processChar(UINT8 ch) {
      BOOL8 complete;
    
      // have not initially decoded a GPS sentence
      complete = FALSE;
    
      if ( (ch < 32) || (ch > 126) ) {
        // control character
        if ( gpsChars > 0 ) {
           gpsString[gpsChars] = 0;
           complete = gps_processSentence(gpsString);
        }
        gpsChars = 0;
      } else {
        // valid character
        if ( gpsChars < 254 ) {
          gpsString[gpsChars++] = (UINT8)ch;
        } else {
          // reset character count to zero
          gpsChars = 0;
        }
      }
      return complete;
    }
    @endcode */
extern MAPCORE_API BOOL8 gps_processSentence( char *sentence );

/** Processes a stream of characters and passes what appears to be complete
    NMEA sentence to gps_processSentence. Any control characters force the
    sentence to be parsed.

    @param ch A single character.  This is appended to an internal structure 
    which is cleared when an entire sentence is completed.
    
    @return Returns TRUE if a valid sentence is completed.  Otherwise, 
    returns FALSE.
    
    @par Example:
    @code
    ReadFile(GPSPort->hPort, &Byte, 1, &dwBytesTransferred, 0);
    // here we should process the data through the GPS engine, or send it to the main gui window..
    if (dwBytesTransferred == 1) {
      // if the port is enabled, ie. not simulating or replaying a track
      if (!GPSPort->IsSimulating() && !gps_isReplaying() && gps_processChar((UINT8)Byte)) {
    ?
      }
    }
    }
    @endcode */
extern MAPCORE_API BOOL8 gps_processChar( UINT8 ch );

/** Initializes the GPS module. This involves resetting the GPS structures,
    and opening any log or replay files that are returned by os_mapName.
    This function should be called once at startup.

    @return Returns TRUE unless the log or replay file (if any) are not 
    opened properly.
    
    @par Example:
    @code
    if ( !gps_init() ) {
      os_exit(-1);
    }
    @endcode */
extern MAPCORE_API BOOL8 gps_init( void );

/** Returns the type of the last valid NMEA message.
    
    @return Returns the message type, as an enumerated type, 
    NMEA_MESSAGE_TYPES.
    
    @par Example:
    @code
    switch ( gps_lastMessageType() ) {
      case NMEA_MESSAGE_TYPE_RMC:
        ?
    case NMEA_MESSAGE_TYPE_GSV:
        ?
    case NMEA_MESSAGE_TYPE_GSA:
        ?
    default:
        ?
    }
    @endcode */
extern MAPCORE_API INT8 gps_lastMessageType( void );

/** Uses the RMC message to determine if the current GPS position is valid.

    @return TRUE if the current RMC status is "A". */
extern MAPCORE_API BOOL8 gps_isValid( void );

/** Returns the longitude as determined by the RMC message.

    @return Returns the longitude in WGS84 co-ordinates of 1/100000th of a 
    degree. */
extern MAPCORE_API INT32 gps_getLongitude( void );

/** Sets the longitude used when the contect is NMEA_CONTEXT_MANUAL.

    @param newLon longitude in WGS84 co-ordinates of 1/100000th of a 
    degree. */
extern MAPCORE_API void gps_setLongitude( INT32 newLon );

/** Returns the latitude as determined by the RMC message.

    @return Returns the latitude in WGS84 co-ordinates of 1/100000th of a 
    degree. */
extern MAPCORE_API INT32 gps_getLatitude( void );

/** Sets the latitude used when the contect is NMEA_CONTEXT_MANUAL.

    @param newLat the latitude in WGS84 co-ordinates of 1/100000th of a 
    degree. */
extern MAPCORE_API void gps_setLatitude( INT32 newLat );

/** Returns the last valid altitude as determined by the GGA message.
    
    @return Returns the altitude in 1/10th of a millimeter 
    ( = meter / 10^4 ).  If no valid GGA mesasges have been received, will 
    return GPS_BADVALUE. */
extern MAPCORE_API INT32 gps_getAltitude( void );

/** Set the altitude used when the contect is NMEA_CONTEXT_MANUAL.
    
    @param newAlt the altitude in 1/10th of a millimeter 
    ( = meter / 10^4 ). */
extern MAPCORE_API void gps_setAltitude( INT32 newAlt );

/** Returns the heading as determined by the RMC message, filtered
    by speed to avoid random directions when not moving.

    @return Returns the heading in 1/10000th of a degree.
    That is, a circle is divided into 3600000 parts.
    A heading of 0 (or 3600000) is True North,  900000 is True East,
    1800000 is True South, and 2700000 is True West. */
extern MAPCORE_API INT32 gps_getHeading( void );

/** Returns the heading as determined by the RMC message.

    @return Returns the heading in 1/10000th of a degree.
    That is, a circle is divided into 3600000 parts.
    A heading of 0 (or 3600000) is True North,  900000 is True East,
    1800000 is True South, and 2700000 is True West. */
extern MAPCORE_API INT32 gps_getRawHeading( void );

/** Sets the heading used when the contect is NMEA_CONTEXT_MANUAL.

    @param newHead the heading in byte-degrees. That is, there is 256 
    byte-degrees in a circle.  A heading of 0 (or 256) is True North, 
    64 byte-degrees is True East, 128 byte-degrees is True South and 192 
    byte-degrees is True West. */
extern MAPCORE_API void gps_setHeading( UINT8 newHead );

/** Returns the speed as determined by the RMC message.

    @return Returns the speed as recorded by the RMC message. The units 
    used are 1/10000th of a knot. */
extern MAPCORE_API INT32 gps_getSpeed( void );

/** Sets the speed used when the contect is NMEA_CONTEXT_MANUAL.

    @param newSpeed the speed as units of 1/10000th of a knot. */
extern MAPCORE_API void gps_setSpeed( INT32 newSpeed );

/** Converts RMC speed into kilometers per hour using imath_knotsToKPH.

    @return Returns the current GPS speed in whole kilometers per hour. */
extern MAPCORE_API INT32 gps_getKPH( void );

/** Converts RMC speed into meters per hour after whole kilometers per hour 
    have been removed.  This effectively gives the first three decimal 
    places of the km/h.
    
    @return Remainder (in meters) after gps_getKPH
    
    @par Example:
    @code
    INT32 speed_kph;
    INT32 speed_kph_decimal
    speed_kph = gps_getKPH();
    speed_kph_decimal = gps_getKPHremainder();
    os_printf("%i.%03i kph", speed_kph, speed_kph_decimal);
    @endcode */
extern MAPCORE_API INT32 gps_getKPHremainder( void );

/** Converts RMC speed into kilometers per hour using imath_knotsToMPH.

    @return Returns the current GPS speed in whole miles per hour. */
extern MAPCORE_API INT32 gps_getMPH( void );

/** Converts RMC speed into 1/1000th of a mile per hour after whole whole
    miles per hour have been removed. This effectively gives the first three
    decimal places of the miles per hour.

    @return Remainder (in 1/1000th miles / hour) after gps_getMPH.

    @par Example:
    @code
    INT32 speed_mph;
    INT32 speed_mph_decimal
    speed_mph = gps_getMPH();
    speed_mph_decimal = gps_getMPHremainder();
    os_printf("%i.%03i miles per hour", speed_mph, speed_mph_decimal);
    @endcode */
extern MAPCORE_API INT32 gps_getMPHremainder( void );

/** Converts RMC speed into meters per second using imath_knotsToMPS.

    @return Current speed in meters per second. */
extern MAPCORE_API INT32 gps_getMPS( void );

/** Converts RMC speed into feet per second using imath_knotsToFPS.

    @return Current speed in feet per second. */
extern MAPCORE_API INT32 gps_getFPS( void );

/** Returns the last UTC as reported by RMC or GGA message.  The NMEA 
    format is HHMMSS.SSS.  This is converted into milliseconds.

    @return Milliseconds since midnight. */
extern MAPCORE_API UINT32 gps_getUTC( void );

/** Set the time used when the contect is NMEA_CONTEXT_MANUAL.

    @param newTime Time as milliseconds since midnight. */
extern MAPCORE_API void gps_setUTC( UINT32 newTime );

/** Calculated the date from the latest RMC message.

    @return The date is returned as a 32 bit packed value. */
extern MAPCORE_API UINT32 gps_getDate( void );

/** Get the UTC GPS time as seconds and microseconds since the start of the
    Unix time epoch (midnight, 00:00:00, January 1, 1970, UTC).

    @note
    This function provides an alternative to #gps_getDate and #gps_getUTC.

    @param[out] time UTC time as the number of seconds and microseconds since
                the start of the epoch. Use NULL if the time is not required.
    @return
      - The number of seconds elapsed since the start of the epoch.
      - Less than zero if the GPS time is not valid. */
extern MAPCORE_API INT32 gps_getTime(TTime *time);

/** Uses the GSA message to determine the fix type.  For example, 2D fix.

    @return Returns GSA Mode. That is, 1 = fix not available, 2 = 2D, 
    3 = 3D. */
extern MAPCORE_API UINT8 gps_getFixType( void );

/** Sets the current fix type, overwriting the GSA derived message.

    @param newFixType The value of the destired fix type. That is, 1 = fix 
    not available, 2 = 2D, 3 = 3D. */
extern MAPCORE_API void gps_setFixType( UINT8 newFixType );

/** Returns the number of satellites currently visible, as reported by the 
    GSV message.  In optimal situations, this may be slightly more than 12.
    
    @return The number of visible satellites */
extern MAPCORE_API UINT8 gps_satelliteCount( void );

/** Returns the elevation of a given satellite. Each visible satellite has
    an elevation, azimuth, strength and PRN number, reported in the GSV 
    message.  This function find the degrees, ranging from 0 to 90.

    @param x the index of the satellite.

    @return The elevation, in degrees, of the xth satellite above the 
    horizon. */
extern MAPCORE_API UINT8 gps_getSatElevation( UINT32 x );

/** Returns the azimuth of a given satellite. Each visible satellite has
    an elevation, azimuth, strength and PRN number, reposted by the GSV 
    message.  This function returns degrees, ranging from 0 to 359.
    
    @param x the index of the satellite.
    
    @return The azimuth of the xth satellite in degrees. */
extern MAPCORE_API UINT16 gps_getSatAzimuth( UINT32 x );

/** The number of satellites used to find the GPS position is calculated 
    from the GSA string.  The GSA sentence contains the PRN used in the 
    solution (if any).  The number of PRN numbers is counter to find this 
    value.

    @return The number of satellites used to calculate the GPS position. */
extern MAPCORE_API UINT8 gps_satellitesUsed( void );

/** The PRN numbers of the satellites used is returned by this function.

    @param x The index (up to gps_satellitesUsed()) of the satellite.

    @return the prn number of the xth satellite used. */
extern MAPCORE_API UINT8 gps_getSatID( UINT32 x );

/** This function returns the Pseudo-Random Noise (PRN) number for a given 
    satellite.  This number is unique for each satellite, and is a useful 
    identifier for each.

    @param x The index of the satellite.

    @return The PRN number of the xth satellite. */
extern MAPCORE_API UINT8 gps_getSatPrn( UINT32 x );

/**  Returns the strength, calculated as a signal to noise ratio (SNR).

     @param x The index of the satellite.

     @return Strength of the xth satellite. */
extern MAPCORE_API UINT8 gps_getSatSnr( UINT32 x );

/** Returns the number of satellites with a SNR of greater than 0.
    These satellites may be used in resolving the GPS position.

    @return Number of satellites with SRN > 0. */
extern MAPCORE_API UINT8 gps_satellitesTracked( void );

/** Returns the number of satellites with a SNR of greater than 0.
    These satellites may be used in resolving the GPS position.
    The difference between this function and that above is that this
    gets its data from gps_GPGSC not partialGPGSV.
    
    @return Number of satellites with SRN > 0. */
extern MAPCORE_API UINT8 gps_FullsatellitesTracked( void );

/** Returns TRUE if all GPGSV messages in sequence have been received
    in order that GPGSV results are processed only when all messages have
    been decoded.

    @return TRUE if all GPGSV messages in sequence have been received, 
    FALSE otherwise. */
extern MAPCORE_API BOOL8 gps_allSentencesReceived( void );

/** Force the Engine to treat the GPS as valid.  It will be valid until 
    the next GPS update, when it will be overwritten with a real 
    value.  NB: Use with caution! */
extern MAPCORE_API void gps_forceValidGPS( void );

/** Change the context that the system retrieves it's gps data from.

    @param newContext The target context */
extern MAPCORE_API void gps_setContext( TGPSContext newContext );

/** Get the current context that the system is retrieving it's gps data 
    from.

    @return The target context */
extern MAPCORE_API TGPSContext gps_getContext( void );

/** Register an odometer callback function.

    @param func Pointer to a function used for odometer/tripmeter support. */
extern MAPCORE_API void gps_registerOdometer( TOdometerFunc* func );

  extern MAPCORE_API TtmcMessageType gps_getTrafficMessageType (void);

/** This function sets the callback routine for traffic messages.

    @param func  function pointer. */
extern MAPCORE_API void gps_setTrafficCallback ( TTrafficFunc *func);

/** This function returns a pointer to a UINT8[8] array which
    contains the last raw traffic data message received by the gpparse
    code.

    @return UINT8 * */
extern MAPCORE_API UINT8 *gps_getTrafficMessage(void);

/** This function returns the block error percentage 
    for the last second of traffic data reception.

    @return UINT8  error percent updated every second. */
extern MAPCORE_API UINT8 gps_getTrafficErrorRate(void);

/** This function returns the RDS message count for the last second
    of traffic message data collection

    @return UINT8  msg count for previous second
    updated once per second
*/
extern MAPCORE_API UINT8 gps_getTrafficRDSCount (void);

/** This function returns the TMC message count for the last second
    of traffic message data collection

    @return UINT8  msg count for previous second
    updated once per second
*/

extern MAPCORE_API UINT8 gps_getTrafficTrafficCount (void);

/** This function returns a flag to indicate the status of the RDS FM receiver

    @return UINT8  =1 for hardware present, 0 for not present
    updated once per second. */

extern MAPCORE_API UINT8 gps_getTrafficHardwareStatus (void);

/** This function returns the currently tuned frequency of
    the traffic RDS FM receiver

    @return UINT16  currently tuned frequency updated once per second. */
extern MAPCORE_API UINT16 gps_getTrafficTunedFrequency(void);

/** This function returns success / failure flag from the last
    tune command.

    @return UINT8 0/1. */
extern MAPCORE_API UINT8 gps_getTrafficTuneStatus(void);

/** This function returns a station list */
extern MAPCORE_API void gps_getTrafficStationListEntry (TStationList *station);

/** This function returns signal strength information */
extern MAPCORE_API void gps_getTrafficSignalStrength (UINT16 *freq, UINT16 *pi, UINT8 *sig);

/** Get gps stats.

    @return see TGPSStats for description of values returned. */
extern MAPCORE_API TGPSStats gpsStats_get(void);

/** Reset gps stats to zero. Also set to zero by gps_init()

    @return void. */
extern MAPCORE_API void gpsStats_reset(void);

/** Enable/disable gpsStats display

    @param[in] onOff TRUE to enable, FALSE to disable
    @return previous setting so caller can save it and restore it. */
extern MAPCORE_API BOOL8 gpsStats_enable(BOOL8 onOff);

/** Set the location in screen coordinates where to
    render gps stats.

    @param[in] pos_s position where to render gps stats in
    screen coordinates
    @return previous location so caller can save it and
    restore it. */
extern MAPCORE_API TPos gpsStats_setPos(const TPos *pos_s); 

/** This function returns HDOP value on GSA */
extern MAPCORE_API UINT32 gps_getHDOP (void);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* GPPARSE_H */
