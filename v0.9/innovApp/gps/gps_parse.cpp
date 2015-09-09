#include "gps_parse.h"
#include <stdlib.h>


static UINT32 FromUnfixToFx = 0;
static INT8 lastGPSType;
// contains a partially decoded GPGGA sentence
static TGPSGPGGA partialGPGGA;
// contains a fully decoded GPGGA sentence
static TGPSGPGGA gps_GPGGA;

// contains a partially decoded GPRMC sentence
static TGPSGPRMC partialGPRMC;
// contains a fully  decoded GPRMC sentence
static TGPSGPRMC gps_GPRMC;

// contains a partially decoded GPGSA sentence
static TGPSGPGSA partialGPGSA;
// contains a fully  decoded GPGSA sentence
static TGPSGPGSA gps_GPGSA;

// contains up to 3 partially decoded GPGSV sentences
static TGPSGPGSV partialGPGSV;
// contains up to 3 fully decoded GPGSV sentences
static TGPSGPGSV gps_GPGSV;

// contains partially decoded proprietary PTTKT message
static GNTPTTKT partialPTTKT;

// externally available information
static GNTPTTKT gps_PTTKT;

static UINT8 gsvFieldCount = 0;
static UINT8 gsvMessageCount = 0;

navcoreGPSParse ::navcoreGPSParse()
{
    successParse = FALSE;
    utc = TIME_BAD_VALUE;
    latitude = GPS_BADVALUE; // degrees x 10^5
    longitude = GPS_BADVALUE; // degrees x 10^5
    speed = GPS_BADVALUE; // x 10 ^ 4
    heading = GPS_BADVALUE; // x 10 ^ 4
    date = GPS_BADVALUE;
    satellitesUsed = 0;
    GPSStatlliteNum = 0;
    BD2StatelliteNum = 0;
    effectiveGPS = FALSE;
}

navcoreGPSParse :: ~navcoreGPSParse()
{


}

void navcoreGPSParse::initGPGGA()
{
    memset(&partialGPGGA,0,sizeof(partialGPGGA));
    partialGPGGA.utc = TIME_BAD_VALUE;
    partialGPGGA.latitude = GPS_BADVALUE;
    partialGPGGA.longitude = GPS_BADVALUE;
    partialGPGGA.quality = '0';
    partialGPGGA.altitude = GPS_BADVALUE;
}

void navcoreGPSParse::initGPGSA(void)
{
  memset(&partialGPGSA,0,sizeof(partialGPGSA));
  partialGPGSA.fixType = 0;
  partialGPGSA.pdop = GPS_BADVALUE;
  partialGPGSA.hdop = GPS_BADVALUE;
  partialGPGSA.vdop = GPS_BADVALUE;
}

void navcoreGPSParse::initGPRMC(void)
{
  memset(&partialGPRMC,0,sizeof(partialGPRMC));
  partialGPRMC.utc = TIME_BAD_VALUE;
  partialGPRMC.latitude = GPS_BADVALUE;
  partialGPRMC.longitude = GPS_BADVALUE;
  partialGPRMC.speed = GPS_BADVALUE;
  partialGPRMC.heading = GPS_BADVALUE;
  partialGPRMC.date = DATE_BAD_VALUE;
}

void navcoreGPSParse ::initGPGSV(void)
{
  memset(&partialGPGSV,0,sizeof(partialGPGSV));
}


bool navcoreGPSParse :: parseFile()
{
    FILE *gpsFile;
    char lineBuf[ 80 ];
    int lineNumber = 0;
    m_gpsLocs.clear(); //clear the vector
    TGPSGPRMC gpsLine;
    char logFileName[ MAX_PATH ]="//home//marcus.x//gp.LOG";
    if( (gpsFile = fopen(logFileName, "rt")) != NULL)
    {
      // parse all lines of GPS log file
      while(fgets(lineBuf,80,gpsFile)){
            printf("linebuf:%s",lineBuf);
            if(!gps_processSentence(lineBuf)){
                printf("%d processSentence failed: %s",lineNumber++,lineBuf);
            }
        }

    }
    else
    {
        return false;
    }

      fclose(gpsFile);
//    if(m_gpsLocs.size() == 0 ) return false;
      return true;
}

BOOL8 navcoreGPSParse ::gps_processSentence(char *sentence)
{
    BOOL8 complete;
//    char replayBuffer[85];
//    UINT8 counter = 0;
//    UINT32 numCharRead;

    // first check the checksum
    if ( !checkGPSChecksum(sentence) ) {
        return FALSE;
    }
    complete = FALSE;

    if ( strncmp(sentence,"$GPGGA",6) == 0 )
    {
      processGPGGA(sentence);
      complete = TRUE;
    }
    else if ( strncmp(sentence,"$GPGSA",6) == 0 )
    {
        processGPGSA(sentence);
        complete = TRUE;

    }
    else if ( strncmp(sentence,"$GPRMC",6) == 0 )
    {
        processGPRMC(sentence);
        complete = TRUE;

    }
    else if ( strncmp(sentence,"$GPGSV",6) == 0 )
    {
        processGPGSV(sentence);
        complete = TRUE;

      }
    else if (strncmp (sentence,"$PTTKT",6) == 0)
    {
        if (processPTTKT(sentence))
        complete = TRUE;

    }
    else if ( strncmp(sentence,"$GPGLL",6) == 0 )
    {
        // we don't actually use GLL, VTG or MSS, but we return TRUE to the app here
        // so that it knows we received a valid NMEA sentence
        complete = TRUE;
        lastGPSType = NMEA_MESSAGE_TYPE_INVALID;

      }
    else if ( strncmp(sentence,"$GPVTG",6) == 0 )
    {
        complete = TRUE;
        lastGPSType = NMEA_MESSAGE_TYPE_INVALID;

      }
    else if ( strncmp(sentence,"$GPMSS",6) == 0 )
    {
        complete = TRUE;
        lastGPSType = NMEA_MESSAGE_TYPE_INVALID;
      }
    else if ( strncmp(sentence,"$GPZDA",6) == 0 )
    {
        complete = TRUE;
        lastGPSType = NMEA_MESSAGE_TYPE_INVALID;
      }

    return complete;
}

bool navcoreGPSParse ::checkGPSChecksum(char *sentence)
{
    char *digit;
    UINT8 nmea;
    UINT8 calculated;
    static char const digits[] = "0123456789ABCDEF0123456789abcdef";

    calculated = 0;
    while ( (*sentence != 0) && (*sentence != '*') ) {
        if ( *sentence != '$' ) {
            calculated ^= *sentence;
        }
        sentence++;
    }

    if ( *sentence != '*' ) {
        return FALSE;
    }

    // initialise checksum
    nmea = 0;

    // get the high nibble value
    char *digitConvert=const_cast<char *>(digits);
    digit = strchr(digitConvert,sentence[1]);
    if (digit != NULL) {
        nmea = (UINT8)(nmea | (((digit - digits) & 0xF) << 4));
    }

    // get the low nibble value
    digit = strchr(digitConvert,sentence[2]);
    if (digit != NULL) {
        nmea = (UINT8)(nmea | ((digit - digits) & 0xF));
    }

    return (BOOL8)(nmea == calculated);
}

bool navcoreGPSParse ::processGPGGA(const char *sentence)
{
    UINT32 count;
    UINT8 fieldCount;
    char work[FIELD_BUFFER_LENGTH];
    const char *field;
    double outLat  = 0;
    double outLon = 0;

    lastGPSType = NMEA_MESSAGE_TYPE_GGA;

    field = sentence;
    fieldCount = 0;

    // initialise GPGGA message values to default
    count = partialGPGGA.sentence;
    initGPGGA();
    partialGPGGA.sentence = count;

    // UTC
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 6 ) {
        if ( (partialGPGGA.utc = decodeTimeValue(work) ) != TIME_BAD_VALUE ) {
          fieldCount++;
        }
      }
    }

    // latitude
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 7 ) {
        if ( (partialGPGGA.latitude = decodeLatLong(work)) != GPS_BADVALUE ) {
          fieldCount++;
        }
      }
    }
    // latitude direction (N,S)
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        fieldCount++;
        if ( (*field == 'S') && (partialGPGGA.latitude != GPS_BADVALUE) ) {
          partialGPGGA.latitude = -partialGPGGA.latitude;
        }
      }
    }

    // longitude
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 7 ) {
        if ( (partialGPGGA.longitude = decodeLatLong(work)) != GPS_BADVALUE ) {
          fieldCount++;
        }
      }
    }
    // longitude direction (E,W)
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        fieldCount++;
        if ( (*field == 'W') && (partialGPGGA.longitude != GPS_BADVALUE) ) {
          partialGPGGA.longitude = -partialGPGGA.longitude;
        }
      }
    }

    // quality
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        partialGPGGA.quality = work[0];
        if ( work[0] != '0' ) {
          fieldCount++;
        }
      }
    }

    // SV's used
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        partialGPGGA.satellitesUsed = (UINT8)atoi(work);
        fieldCount++;
      }
    }

    // Horizontal Dilution (Ignore)
    if ( (field = findNextField(field)) != NULL ) {
      readField(work,field);
    }

    // altitude
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 3 ) {

        if ( (partialGPGGA.altitude = decodeFloatValue(work)) != GPS_BADVALUE ) {
          fieldCount++;
        }
      }
    }
    // altitude direction (+,-)
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        fieldCount++;
        if ( (*field == '-') && (partialGPGGA.altitude != GPS_BADVALUE) ) {
          partialGPGGA.altitude = -partialGPGGA.altitude;
        }
      }
    }

    if ( fieldCount >= 1 ) {
      partialGPGGA.sentence++;
      //@TPE S
    /*LOG_INFO(0, ("processGPGGA (before CN-condition +[%d, %d]",
           partialGPRMC.latitude,
           partialGPRMC.longitude));*/
//  #ifdef _WIN32
//      if (strncmp(gloadMapName,"CN",2)==0){
//  /*      LOG_INFO(0, ("processGPGGA +[%d, %d]",
//           partialGPRMC.latitude,
//           partialGPRMC.longitude));*/
//        ConverCHNCoords( partialGPGGA.latitude/100000.0, partialGPGGA.longitude/100000.0, &outLat, &outLon);
//        partialGPGGA.latitude  = (outLat*100000.0);
//        partialGPGGA.longitude = (outLon*100000.0);
//  /*     LOG_INFO(0, ("processGPGGA -[%d, %d]",
//          partialGPRMC.latitude,
//          partialGPRMC.longitude));*/
//      }
//  #endif
      //@TPE E
      memcpy(&gps_GPGGA,&partialGPGGA,sizeof(partialGPGGA));
      return TRUE;
    }
      return FALSE;
}

bool navcoreGPSParse ::processGPGSA(const char *sentence)
{
    UINT32 count;
      UINT8 i;
      UINT8 fieldCount;
      char work[FIELD_BUFFER_LENGTH];
      const char *field;

      lastGPSType = NMEA_MESSAGE_TYPE_GSA;

      field = sentence;
      fieldCount = 0;

      // initialise the GPGSA sentence parameters to defaults
      count = partialGPGSA.sentence;
      initGPGSA();
      partialGPGSA.sentence = count;

      // mode
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          partialGPGSA.mode = work[0];
          fieldCount++;
        }
      }

      // fix type
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          partialGPGSA.fixType = (UINT8)(work[0] - '0');
          if ( (work[0] == '2') || (work[0] == '3') ) {
            fieldCount++;
          }
        }
      }

      // sv ID's
      for ( i = 0; i < 12; i++ ) {
        partialGPGSA.satelliteIDs[i] = 0;
        if ( (field = findNextField(field)) != NULL ) {
          if ( readField(work,field) >= 1 ) {
            // must be at least 3 valid fields for a fix
            partialGPGSA.satellitesUsed++;
            partialGPGSA.satelliteIDs[i] = (UINT8)atoi(work);
            if(atoi(work)>0 )
            {
                GPSStatlliteNum++;
            }
            fieldCount++;
          } else {
            partialGPGSA.satelliteIDs[i] = 0;
          }
        }
      }

      // pdop
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 3 ) {
          if ( (partialGPGSA.pdop = decodeFloatValue(work)) != GPS_BADVALUE ) {
            fieldCount++;
          }
        }
      }

      // hdop
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 3 ) {
          if ( (partialGPGSA.hdop = decodeFloatValue(work)) != GPS_BADVALUE ) {
            fieldCount++;
          }
        }
      }

      // vdop
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 3 ) {
          if ( (partialGPGSA.vdop = decodeFloatValue(work)) != GPS_BADVALUE ) {
            fieldCount++;
          }
        }
      }

      if ( fieldCount >= 1 ) {
        partialGPGSA.sentence++;
        memcpy(&gps_GPGSA,&partialGPGSA,sizeof(partialGPGSA));
        return TRUE;
      }

      return FALSE;

}

bool navcoreGPSParse::processGPRMC(const char *sentence)
{
    UINT32 count;
     UINT8 fieldCount;
     char work[FIELD_BUFFER_LENGTH];
     const char *field;
     double  outLat  = 0;
     double  outLon = 0;

     lastGPSType = NMEA_MESSAGE_TYPE_RMC;

     field = sentence;
     fieldCount = 0;

     successParse = FALSE;
     effectiveGPS = FALSE;

     // initialise GPRMC sentence values
     count = partialGPRMC.sentence;
     initGPRMC();
     partialGPRMC.sentence = count;

     // UTC
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 6 ) {
         if ( (partialGPRMC.utc = decodeTimeValue(work) ) != TIME_BAD_VALUE ) {
           fieldCount++;
         }
       }
     }

     // status
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 1 ) {
         partialGPRMC.status = work[0];
         if ( work[0] == 'A' ) {
           fieldCount++;
           //@TPE The counter is recorded when GPS status is fixed.
           if(FromUnfixToFx < FILTER_GPS_TIMES_FORM_UNFIX_TO_FIX)
             FromUnfixToFx++;
           effectiveGPS = TRUE;
         }
         else
             //@TPE The counter is reset when GPS status is unfixed.
             FromUnfixToFx = 0;
       }
     }

     // latitude
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 7 ) {
         if ( (partialGPRMC.latitude = decodeLatLong(work)) != GPS_BADVALUE ) {
           fieldCount++;
         }
       }
     }
     // latitude direction (N,S)
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 1 ) {
         fieldCount++;
         if ( (*field == 'S') && (partialGPRMC.latitude != GPS_BADVALUE) ) {
           partialGPRMC.latitude = -partialGPRMC.latitude;
         }
       }
     }

     // longitude
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 7 ) {
         if ( (partialGPRMC.longitude = decodeLatLong(work)) != GPS_BADVALUE ) {
           fieldCount++;
         }
       }
     }
     // longitude direction (E,W)
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 1 ) {
         fieldCount++;
         if ( (*field == 'W') && (partialGPRMC.longitude != GPS_BADVALUE) ) {
           partialGPRMC.longitude = -partialGPRMC.longitude;
         }
       }
     }

     // ground speed
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 3 ) {
         if ( (partialGPRMC.speed = decodeFloatValue(work)) != GPS_BADVALUE ) {
           fieldCount++;
         }
       }
     }

     // heading
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 3 ) {
         if ( (partialGPRMC.heading = decodeFloatValue(work)) != GPS_BADVALUE ) {
           fieldCount++;
         }
       }
     }

     // date
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 6 ) {
         if ( (partialGPRMC.date = time_stringToUlong(NULL,work)) != TIME_BAD_VALUE ) {
           fieldCount++;
         }
       }
     }



     if(partialGPRMC.utc>0&&partialGPRMC.latitude!=GPS_BADVALUE&&partialGPRMC.longitude!=GPS_BADVALUE)
     {
         successParse = TRUE;
         utc = partialGPRMC.utc;
         latitude = partialGPRMC.latitude;
         longitude = partialGPRMC.longitude;
         speed = partialGPRMC.speed;
         heading = partialGPRMC.heading;
         date = partialGPRMC.date;
     }
     else
     {
         utc = 0;
         latitude = GPS_BADVALUE;
         longitude = GPS_BADVALUE;
     }

     if ( fieldCount >= 1 ) {
       partialGPRMC.sentence++;
       //@TPE S
       /*LOG_INFO(0, ("processGPRMC (before CN-condition +[%d, %d]",
            partialGPRMC.latitude,
            partialGPRMC.longitude));*/
   #ifdef _WIN32
       if (strncmp(gloadMapName,"CN",2)==0){
   /*      LOG_INFO(0, ("processGPRMC+[%d, %d]",
            partialGPRMC.latitude,
            partialGPRMC.longitude));*/
         ConverCHNCoords( partialGPRMC.latitude/100000.0, partialGPRMC.longitude/100000.0, &outLat, &outLon);
         partialGPRMC.latitude = (outLat*100000.0);
         partialGPRMC.longitude = (outLon*100000.0);
   /*     LOG_INFO(0, ("processGPRMC-[%d, %d]",
          partialGPRMC.latitude,
          partialGPRMC.longitude));*/
       }
   #endif
       //@TPE E
       memcpy(&gps_GPRMC,&partialGPRMC,sizeof(partialGPRMC));
       return TRUE;
     }

     return FALSE;

}


const char * navcoreGPSParse ::findNextField(const char * sentence)
{
    char ch;

     if ( sentence == NULL ) {
       return NULL;
     }

     while ( (ch = *sentence) != 0 ) {
       sentence++;
       if ( ch == ',' ) {
         return sentence;
       }
       if ( ch == '*' ) {
         return NULL;
       }
     }

     return NULL;
}


UINT8 navcoreGPSParse::readField(char *dest, const char *sentence)
{
    UINT8 len;
     char ch;

     // no data extracted initially
     len = 0;

     // only extract valid sentences
     if ( sentence != NULL ) {
       while ( (ch = *sentence) != 0 ) {
         if ( len >= MAX_FIELD_LENGTH ) {
           dest[len] = 0;
           return len;
         }
         if ( ch == ',' ) {
           dest[len] = 0;
           return len;
         }
         if ( ch == '*' ) {
           dest[len] = 0;
           return len;
         }
         dest[len] = ch;
         len++;
         sentence++;
       }
     }

     // must be nul terminated
     dest[len] = 0;

     return 0;
}

// return the integer value of the field ignoring any digits
// after the decimal place.
INT32 navcoreGPSParse :: decodeIntegerValue(const char *text)
{
  char work[FIELD_BUFFER_LENGTH];
  INT32 value;
  UINT8 len;

  len = 0;
  while ( (text[len] != 0) && (text[len] != '.') ) {
    len++;
  }

  if ( len > 0 ) {
    memcpy(work,text,len);
    work[len] = 0;
    value = (INT32)atoi(work);
  } else {
    return GPS_BADVALUE;
  }

  return value;
}

INT32 navcoreGPSParse::decodeTimeValue(const char *text)
{
    static char timeString[FIELD_BUFFER_LENGTH];
      static char timeFull[FIELD_BUFFER_LENGTH];
      char *dotChr;
      INT32 seconds;
      INT32 milliseconds;
      UINT32 dotPos;
      UINT32 i;
      UINT32 multiplier;

      // where is the decimal point
      char * textconvert =const_cast<char *>(text);
      dotChr = strchr(textconvert,'.');

      if (dotChr != NULL) {
        // calc how far into the string we found the decimal point
        dotPos = (UINT8)(dotChr - text);
        if (dotPos > 6) {
          dotPos = 6;
        }

        // take up to 6 chars of time string
        memcpy(timeString,text,dotPos);
        timeString[dotPos] = 0;
      } else {
        // we found no decimal point, take the first 3 chars
        strncpy(timeString,text,6);
        timeString[6] = 0;
      }

      // make sure the time string is exactly 6 characters long
      // fill with 6-string length zero chars and then concatenate time string
      timeFull[0] = 0;
      for (i = 0; i < 6 - strlen(timeString); i++) {
        strcat(timeFull,"0");
      }
      strcat(timeFull,timeString);

      // seconds part of the time has format HHMMSS
      seconds = time_stringToUlong(timeFull,NULL);

      // take next two digits after the decimal point as millisecs
      milliseconds = 0;
      if (dotChr != NULL) {
        // set ourselves up to convert milliseconds
        dotChr++;
        multiplier = 100;

        for (i=0;i<3;i++) {
          if (dotChr[i] >= '0' && dotChr[i] <='9') {
            // add in the correct component of milliseconds
            milliseconds += multiplier * (dotChr[i] - '0');
          } else {
            // non digit char encountered, or NULL terminator
            break;
          }

          // next digit is ten less
          multiplier /= 10;
        }
      }

      // complete time is the seconds part plus milliseconds
      return seconds * (INT32)1000L + milliseconds;
}

INT32 navcoreGPSParse::decodeLatLong(const char *text) {
  char work[FIELD_BUFFER_LENGTH];
  INT32 degrees;
  INT32 minutes;
  UINT8 pos;
  UINT8 len;
  UINT8 i;

  /* find . separator */
  len = 0;
  while ( (text[len] != 0) && (text[len] != '.') ) {
    len++;
  }
  if ( text[len] != '.' ) {
    return GPS_BADVALUE;
  }

  if ( len >= 3 ) {
    /* any characters 2 before . are degrees */
    memcpy(work,text,len-2);
    work[len - 2] = 0;
    degrees = atoi(work);
    /* text now points to minutes part */
    text += len - 2;
    len = 2;
  } else {
    degrees = 0;
  }

  /* minutes before . */
  memcpy(work,text,len);
  work[len] = 0;
  pos = len;
  /* skip . */
  text += len + 1;

  /* minutes after . */
  for ( i = 0; i < 4; i++ ) {
    if ( *text != 0 ) {
      work[pos] = *text;
      text++;
    } else {
      work[pos] = '0';
    }
    pos++;
  }
  /* terminate string */
  work[pos] = 0;

  minutes = pos ? atoi(work) : 0;
  /* convert to 100k * fractions of a degree */
  minutes += minutes << 2; /* 5 * minutes */
  minutes /= 30; /* minutes now */

  /* returns degrees * 100k */
  return (degrees * (INT32)100000L) + minutes;
}

INT32 navcoreGPSParse::decodeFloatValue(const char *text)
{
    char work[FIELD_BUFFER_LENGTH];
      INT32 value;
      UINT8 pos;
      UINT8 len;
      UINT8 i;

      /* find . separator */
      len = 0;
      while ( (text[len] != 0) && (text[len] != '.') ) {
        len++;
      }
      if ( text[len] != '.' ) {
        return GPS_BADVALUE;
      }

      if ( len > 0 ) {
        /* any characters before . are value */
        memcpy(work,text,len);
        work[len] = 0;
        value = (INT32)10000L * atoi(work);
        /* text now points */
        text += len;
        len = 2;
      } else {
        value = 0;
      }

      /* skip . */
      text++;

      /* part after . */
      pos = 0;
      for ( i = 0; i < 4; i++ ) {
        if ( *text != 0 ) {
          work[pos] = *text;
          text++;
        } else {
          work[pos] = '0';
        }
        pos++;
      }
      /* terminate string 44*/
      work[pos] = 0;

      value += (pos ? atoi(work) : 0);

      /* returns value * 10K */
      return value;
}



bool navcoreGPSParse ::processGPGSV(const char *sentence)
{
    UINT32 count;
    UINT8 i;
    UINT8 index;
    INT32 message;
    INT32 messages;
    INT32 value;
    char work[FIELD_BUFFER_LENGTH];
    const char *field;

    lastGPSType = NMEA_MESSAGE_TYPE_GSV;

    field = sentence;
    message = GPS_BADVALUE;
    messages = GPS_BADVALUE;

    // messages
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        messages = decodeIntegerValue(work);
      }
    }

    // message #
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        message = decodeIntegerValue(work);
        if (message == 1) {
            //set the flag indicating if the full GPGSV set of sentences have been received
            gps_fullGPGSV=FALSE;
            if (message==1){
              gps_Rx_GPGSV1=TRUE;
              gps_Rx_GPGSV2=FALSE;
              gps_Rx_GPGSV3=FALSE;
          partialGPGSV.satelliteTracked = 0;
        }
      }
    }

      //@MIC
      gps_Rx_GPGSV4=FALSE;
      gps_Rx_GPGSV5=FALSE;
      gps_Rx_GPGSV6=FALSE;
      //@MIC
      if (messages==1){
        gps_fullGPGSV=TRUE;
      }
    }
    if ((message==2) && (gps_Rx_GPGSV1==TRUE)){
      gps_Rx_GPGSV2=TRUE;
      gps_Rx_GPGSV3=FALSE;
      //@MIC
      gps_Rx_GPGSV4=FALSE;
      gps_Rx_GPGSV5=FALSE;
      gps_Rx_GPGSV6=FALSE;
      //@MIC
      if (messages==2){
        gps_fullGPGSV=TRUE;
      }
    }
    if ((message==3) && (gps_Rx_GPGSV2==TRUE)){
      gps_Rx_GPGSV3=TRUE;
      //@MIC
      gps_Rx_GPGSV4=FALSE;
      gps_Rx_GPGSV5=FALSE;
      gps_Rx_GPGSV6=FALSE;
      //@MIC
      if (messages==3){
        gps_fullGPGSV=TRUE;
      }
    }
    //@MIC
    if ((message==4) && (gps_Rx_GPGSV3==TRUE)){
      gps_Rx_GPGSV4=TRUE;
      gps_Rx_GPGSV5=FALSE;
      gps_Rx_GPGSV6=FALSE;
      if (messages==4){
        gps_fullGPGSV=TRUE;
      }
    }
    if ((message==5) && (gps_Rx_GPGSV4==TRUE)){
      gps_Rx_GPGSV5=TRUE;
      gps_Rx_GPGSV6=FALSE;
      if (messages==5){
        gps_fullGPGSV=TRUE;
      }
    }
    if ((message==6) && (gps_Rx_GPGSV5==TRUE)){
      gps_Rx_GPGSV6=TRUE;
      if (messages==6){
        gps_fullGPGSV=TRUE;
      }
    }
    //@MIC


    // ignore messages with null message or message count fields
    if ( (messages == GPS_BADVALUE) || (message == GPS_BADVALUE) ) {
      return FALSE;
    }

    // check that message number is in range
    if ( (message < 1) || (message > 6/*3*/) ) {
      return FALSE;
    }

    // check that number of messages is in range
    if ( (messages < 1) || (messages > 6/*3*/) ) {
      return FALSE;
    }

    // for message number 1 initialise gsv rx parameters
    if ( message == 1 ) {
      gsvFieldCount = 0;
      gsvMessageCount = 0;
      count = partialGPGSV.sentence;
      initGPGSV();
      partialGPGSV.sentence = count;
    }
    // increment message count
    gsvMessageCount++;

    // # satellites being tracked
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        if ( (value = decodeIntegerValue(work)) != GPS_BADVALUE ) {
          partialGPGSV.satelliteCount = (UINT8)value;
          gsvFieldCount++;
        }
      }
    }

    for ( i = 0; i < 4; i++ ) {
      index = (UINT8)(((message - 1) << 2) + i);

      // PRN number
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          if ( (value = decodeIntegerValue(work)) != GPS_BADVALUE ) {
            partialGPGSV.prn[index] = (UINT8)value;
            gsvFieldCount++;
          }
        }
      }

      // elevation
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          if ( (value = decodeIntegerValue(work)) != GPS_BADVALUE ) {
            partialGPGSV.elevation[index] = (UINT8)value;
            gsvFieldCount++;
          }
        }
      }

      // azimuth
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          if ( (value = decodeIntegerValue(work)) != GPS_BADVALUE ) {
            partialGPGSV.azimuth[index] = (UINT16)value;
            gsvFieldCount++;
          }
        }
      }

      // snr
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          if ( (value = decodeIntegerValue(work)) != GPS_BADVALUE ) {
            partialGPGSV.snr[index] = (UINT8)value;
            if ( value > 0 ) {
              ++partialGPGSV.satelliteTracked;
            }
            gsvFieldCount++;
          } else {
            partialGPGSV.snr[index] = 0;
          }
        }
      }
    }

    /* A complete sentence has all the appropriate fields filled */
    /* A complete SET of sentences is also true if: */
    /* if ( (message == messages) && (gsvMessageCount == messages) ) */
    if ( gsvFieldCount >= messages ) {
      // perhaps copy sentences across to valid sentences when they become valid
      partialGPGSV.sentence++;
      memcpy(&gps_GPGSV,&partialGPGSV,sizeof(partialGPGSV));
      return TRUE;
    }

    return FALSE;
  }







// convert 2 UPPERCASE hex chars to UINT8 binary
UINT8 navcoreGPSParse:: hexToBinary (const char *hex) {
  UINT8 rval;

  rval = ((*hex < 'A') ? ((*hex - '0') << 4) : ((*hex - 'A' + 10) << 4));
  hex++;
  rval += ((*hex < 'A') ? (*hex - '0') : (*hex - 'A' + 10));
  return (rval);
}

// parses the PTTKT sentence into the partialPTTKT structure which is
// copied over to the externally visible PTTKT structure if any non null
// fields are decoded.
BOOL8 navcoreGPSParse ::processPTTKT(const char *sentence)
{
  char  work[FIELD_BUFFER_LENGTH];
  UINT8 index;
  const char *field, *wrk;

  lastGPSType = NMEA_MESSAGE_TYPE_TRAFFIC;

  field = sentence;

  // MESSAGE SUBTYPE
  if ( (field = findNextField(field)) != NULL ) {
    readField(work,field);
    if (strncmp (work,"DATA",4)==0)
    {
      // contains one field, which is 16 hex characters
      if ((field = findNextField(field)) != NULL)
      {
        if (readField(work,field) == 16)
        {
          // convert 16 hex characters  into UINT8 buffer
          wrk = work;
          for (index = 0; index < 8; index++) {
            partialPTTKT.tmcData[index] = hexToBinary (wrk);
            wrk += 2;
          }

          partialPTTKT.lastMessage = TMC_MSG_TYPE_DATA;
          memcpy (&gps_PTTKT,&partialPTTKT,sizeof(gps_PTTKT));
          return TRUE;
        }
      }
      return FALSE;
    }
    else if (strncmp (work,"STAT",4)==0)
    {
      // contains 5 fields
      for (index = 0; index < 5; index++) {
        if ((field = findNextField(field)) != NULL) {
          readField(work,field);
          switch (index) {
            case 0:
              partialPTTKT.hardwarePresent = (UINT8)atoi(work);
              break;
            case 1: // tuned frequency (decimal number)
              partialPTTKT.frequency = (UINT16)atoi(work);
              break;
            case 2: // Block Count (decimal number)
              partialPTTKT.blockCount = (UINT8)atoi(work);
              break;
            case 3: // Traffic Message Count
              partialPTTKT.tmcCount = (UINT8)atoi(work);
              break;
            case 4: // error rate (decimal number)
              partialPTTKT.errorRate = (UINT8)atoi(work);
              break;
          }
        } else {
          return FALSE;
        }
      }
      partialPTTKT.lastMessage = TMC_MSG_TYPE_STATUS;
      memcpy (&gps_PTTKT,&partialPTTKT,sizeof(gps_PTTKT));
      return TRUE;
    }
    else if (strncmp (work,"TUNE",4)==0)
    {
      // returns the TUNE command sent to the hardware
      // need to get the tune status from this

      // contains 2 fields (only the last is of interest)
      for (index = 0; index < 2; index++) {
        if ((field = findNextField(field)) != NULL) {
          readField(work,field);
          switch (index) {
            case 0: // frequency
              break;
            case 1: // status (0 or 1)
              partialPTTKT.tuneStatus = (UINT8)atoi(work);
              break;
          }
        } else {
          return FALSE;
        }
      }

      partialPTTKT.lastMessage = TMC_MSG_TYPE_TUNE;
      memcpy (&gps_PTTKT,&partialPTTKT,sizeof(gps_PTTKT));
      return TRUE;
    }
    else if (strncmp (work,"WAKE",4)==0)
    {
      // The WAKE command is generated when
      // the (Wince) target comes out of sleep mode
      partialPTTKT.lastMessage = TMC_MSG_TYPE_WAKEUP;
      gps_PTTKT.lastMessage = TMC_MSG_TYPE_WAKEUP;
      return TRUE;
    }
    else if (strncmp (work,"LIST",4)==0)
    {
      // contains 8 fields
      for (index = 0; index < 8; index++)
      {
        if ((field = findNextField(field)) != NULL)
        {
          readField(work,field);
          switch (index)
          {
            case 0: // Flag to indicate tuned freq
              partialPTTKT.stnCurrent = (BOOL8)atoi(work);
              break;
            case 1: // Station Count (decimal number)
              partialPTTKT.stnCount = (UINT8)atoi(work);
              break;
            case 2: // station Index (decimal number)
              partialPTTKT.stnIndex = (UINT8)atoi(work);
              break;
            case 3: // station Frequency (decimal number)
              partialPTTKT.stnFrequency = (UINT16)atoi(work);
              break;
            case 4: // station PI code (hex)
              wrk = work;
              partialPTTKT.stnPI = (hexToBinary (wrk) << 8);
              wrk+=2;
              partialPTTKT.stnPI += (hexToBinary (wrk));
              break;
            case 5: // station signal strength
              partialPTTKT.stnSignal = (UINT8)atoi(work);
              break;
            case 6: // station flag
              partialPTTKT.stnFlag = (UINT8)atoi(work);
              break;
            case 7: // station name
              strcpy (partialPTTKT.stnName, work);
              break;
          }
        } else
        {
          return FALSE;
        }
      }
      partialPTTKT.lastMessage = TMC_MSG_TYPE_LIST;
      memcpy (&gps_PTTKT,&partialPTTKT,sizeof(gps_PTTKT));
      return TRUE;
    }
    else if (strncmp (work,"SGNL",4)==0)
    {
      // contains 3 fields freq, PI, signal
      // station list entries used for storage
      for (index = 0; index < 3; index++)
      {
        if ((field = findNextField(field)) != NULL)
        {
          readField(work,field);
          switch (index)
          {
            case 0: // station Frequency (decimal number)
              partialPTTKT.stnFrequency = (UINT16)atoi(work);
              break;
            case 1: // station PI code (hex)
              wrk = work;
              partialPTTKT.stnPI = (hexToBinary (wrk) << 8);
              wrk+=2;
              partialPTTKT.stnPI += (hexToBinary (wrk));
              break;
            case 2: // station signal strength
              partialPTTKT.stnSignal = (UINT8)atoi(work);
              break;
          }
        } else
        {
          return FALSE;
        }
      }
      partialPTTKT.lastMessage = TMC_MSG_TYPE_SIGSTRENGTH;
      memcpy (&gps_PTTKT,&partialPTTKT,sizeof(gps_PTTKT));
      return TRUE;
    }
    else if (strncmp (work,"RESET",5)==0)
    {
      // return the RESET command sent to the hardware
      partialPTTKT.lastMessage = TMC_MSG_TYPE_RESET;
      gps_PTTKT.lastMessage = TMC_MSG_TYPE_RESET;
      return TRUE;
    }
  }
  return FALSE;
}

