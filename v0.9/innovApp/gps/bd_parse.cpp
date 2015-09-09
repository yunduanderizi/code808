#include "bd_parse.h"
#include<stdlib.h>

static UINT32 FromUnfixToFx = 0;
static INT8 lastBDType;

// contains a partially decoded GPGGA sentence
static TGPSBDGGA partialBDGGA;
// contains a fully decoded BDGGA sentence
static TGPSBDGGA gps_BDGGA;

// contains a partially decoded BDRMC sentence
static TGPSBDRMC partialBDRMC;
// contains a fully  decoded BDRMC sentence
static TGPSBDRMC gps_BDRMC;

// contains a partially decoded BDGSA sentence
static TGPSBDGSA partialBDGSA;
// contains a fully  decoded BDGSA sentence
static TGPSBDGSA gps_BDGSA;

// contains up to 3 partially decoded BDGSV sentences
static TGPSBDGSV partialBDGSV;
// contains up to 3 fully decoded BDGSV sentences
static TGPSBDGSV gps_BDGSV;

// contains partially decoded proprietary PTTKT message
static BDTPTTKT BD_partialPTTKT;

// externally available information
static BDTPTTKT BD__PTTKT;

static UINT8 gsvFieldCount = 0;
static UINT8 gsvMessageCount = 0;

navcoreBDParse ::navcoreBDParse()
{
    bool successParse = FALSE;
    UINT32 utc =0;
    INT32 latitude = GPS_BADVALUE; // degrees x 10^5
    INT32 longitude = GPS_BADVALUE; // degrees x 10^5
}

navcoreBDParse :: ~navcoreBDParse()
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

void navcoreBDParse::initBDGGA()
{
    memset(&partialBDGGA,0,sizeof(partialBDGGA));
    partialBDGGA.utc = TIME_BAD_VALUE;
    partialBDGGA.latitude = GPS_BADVALUE;
    partialBDGGA.longitude = GPS_BADVALUE;
    partialBDGGA.quality = '0';
    partialBDGGA.altitude = GPS_BADVALUE;
}

void navcoreBDParse::initBDGSA(void)
{
  memset(&partialBDGSA,0,sizeof(partialBDGSA));
  partialBDGSA.fixType = 0;
  partialBDGSA.pdop = GPS_BADVALUE;
  partialBDGSA.hdop = GPS_BADVALUE;
  partialBDGSA.vdop = GPS_BADVALUE;
}

void navcoreBDParse::initBDRMC(void)
{
  memset(&partialBDRMC,0,sizeof(partialBDRMC));
  partialBDRMC.utc = TIME_BAD_VALUE;
  partialBDRMC.latitude = GPS_BADVALUE;
  partialBDRMC.longitude = GPS_BADVALUE;
  partialBDRMC.speed = GPS_BADVALUE;
  partialBDRMC.heading = GPS_BADVALUE;
  partialBDRMC.date = DATE_BAD_VALUE;
}

void navcoreBDParse ::initBDGSV(void)
{
  memset(&partialBDGSV,0,sizeof(partialBDGSV));
}

bool navcoreBDParse :: parseFile()
{

    FILE *gpsFile;
    char lineBuf[ 80 ];
    int lineNumber = 0;
    //m_gpsLocs.clear(); //clear the vector
    TGPSBDRMC gpsLine;
    char logFileName[ MAX_PATH ]="//home//marcus.x//gp.LOG";
    if( (gpsFile = fopen(logFileName, "rt")) != NULL)
    {
      // parse all lines of GPS log file
      while(fgets(lineBuf,80,gpsFile)){
            printf("linebuf:%s",lineBuf);
            if(!BD_processSentence(lineBuf)){
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

BOOL8 navcoreBDParse ::BD_processSentence(char *sentence)
{
    BOOL8 complete;
    // first check the checksum
    if ( !checkBDChecksum(sentence) ) {
        return FALSE;
    }

    complete = FALSE;

    if ( strncmp(sentence,"$BDGGA",6) == 0 )
    {
        processBDGGA(sentence);
        complete = TRUE;
    }
    else if ( strncmp(sentence,"$BDGSA",6) == 0 )
    {
        processBDGSA(sentence);
        complete = TRUE;

    }
    else if ( strncmp(sentence,"$BDRMC",6) == 0 )
    {
        processBDRMC(sentence);
        complete = TRUE;

    }
    else if ( strncmp(sentence,"$BDGSV",6) == 0 )
    {
        processBDGSV(sentence);
        complete = TRUE;

    }
    else if (strncmp (sentence,"$PTTKT",6) == 0)
    {
        if (processPTTKT(sentence))
            complete = TRUE;

    }
    else if ( strncmp(sentence,"$BDGLL",6) == 0 )
    {
        // we don't actually use GLL, VTG or MSS, but we return TRUE to the app here
        // so that it knows we received a valid NMEA sentence
        complete = TRUE;
        lastBDType = NMEA_MESSAGE_TYPE_INVALID;

    }
    else if ( strncmp(sentence,"$BDVTG",6) == 0 )
    {
        complete = TRUE;
        lastBDType = NMEA_MESSAGE_TYPE_INVALID;

    }
    else if ( strncmp(sentence,"$BDMSS",6) == 0 )
    {
        complete = TRUE;
        lastBDType = NMEA_MESSAGE_TYPE_INVALID;
    }
    else if ( strncmp(sentence,"$BDZDA",6) == 0 )
    {
        complete = TRUE;
        lastBDType = NMEA_MESSAGE_TYPE_INVALID;
    }

    return complete;
}

bool navcoreBDParse ::checkBDChecksum(char *sentence)
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

bool navcoreBDParse ::processBDGGA(const char *sentence)
{
    UINT32 count;
    UINT8 fieldCount;
    char work[FIELD_BUFFER_LENGTH];
    const char *field;
    double outLat  = 0;
    double outLon = 0;

    lastBDType = NMEA_MESSAGE_TYPE_GGA;

    field = sentence;
    fieldCount = 0;

    // initialise BDGGA message values to default
    count = partialBDGGA.sentence;
    initBDGGA();
    partialBDGGA.sentence = count;
    successParse = FALSE;
    // UTC
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 6 ) {
        if ( (partialBDGGA.utc = decodeTimeValue(work) ) != TIME_BAD_VALUE ) {
          fieldCount++;
        }
      }
    }

    // latitude
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 7 ) {
        if ( (partialBDGGA.latitude = decodeLatLong(work)) != GPS_BADVALUE ) {
          fieldCount++;
        }
      }
    }
    // latitude direction (N,S)
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        fieldCount++;
        if ( (*field == 'S') && (partialBDGGA.latitude != GPS_BADVALUE) ) {
          partialBDGGA.latitude = -partialBDGGA.latitude;
        }
      }
    }

    // longitude
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 7 ) {
        if ( (partialBDGGA.longitude = decodeLatLong(work)) != GPS_BADVALUE ) {
          fieldCount++;
        }
      }
    }
    // longitude direction (E,W)
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        fieldCount++;
        if ( (*field == 'W') && (partialBDGGA.longitude != GPS_BADVALUE) ) {
          partialBDGGA.longitude = -partialBDGGA.longitude;
        }
      }
    }

    // quality
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        partialBDGGA.quality = work[0];
        if ( work[0] != '0' ) {
          fieldCount++;
        }
      }
    }

    // SV's used
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        partialBDGGA.satellitesUsed = (UINT8)atoi(work);
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
        if ( (partialBDGGA.altitude = decodeFloatValue(work)) != GPS_BADVALUE ) {
          fieldCount++;
        }
      }
    }
    // altitude direction (+,-)
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        fieldCount++;
        if ( (*field == '-') && (partialBDGGA.altitude != GPS_BADVALUE) ) {
          partialBDGGA.altitude = -partialBDGGA.altitude;
        }
      }
    }
    if(partialBDGGA.utc>0&&partialBDGGA.latitude!=GPS_BADVALUE&&partialBDGGA.longitude!=GPS_BADVALUE)
    {
        successParse = TRUE;
        utc = partialBDGGA.utc;
        latitude = partialBDGGA.latitude;
        longitude=partialBDGGA.longitude;
    }
    else
    {
        utc = 0;
        latitude = GPS_BADVALUE;
        longitude = GPS_BADVALUE;
    }

    if ( fieldCount >= 1 ) {
      partialBDGGA.sentence++;
      memcpy(&gps_BDGGA,&partialBDGGA,sizeof(partialBDGGA));
      return TRUE;
    }
      return FALSE;
}

bool navcoreBDParse ::processBDGSA(const char *sentence)
{
    UINT32 count;
      UINT8 i;
      UINT8 fieldCount;
      char work[FIELD_BUFFER_LENGTH];
      const char *field;

      lastBDType = NMEA_MESSAGE_TYPE_GSA;

      field = sentence;
      fieldCount = 0;

      // initialise the BDGSA sentence parameters to defaults
      count = partialBDGSA.sentence;
      initBDGSA();
      partialBDGSA.sentence = count;

      // mode
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          partialBDGSA.mode = work[0];
          fieldCount++;
        }
      }

      // fix type
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          partialBDGSA.fixType = (UINT8)(work[0] - '0');
          if ( (work[0] == '2') || (work[0] == '3') ) {
            fieldCount++;
          }
        }
      }

      // sv ID's
      for ( i = 0; i < 12; i++ ) {
        partialBDGSA.satelliteIDs[i] = 0;
        if ( (field = findNextField(field)) != NULL ) {
          if ( readField(work,field) >= 1 ) {
            // must be at least 3 valid fields for a fix
            partialBDGSA.satellitesUsed++;
            partialBDGSA.satelliteIDs[i] = (UINT8)atoi(work);
            if(atoi(work)>0 )
            {
                BD2StatelliteNum++;
            }

            fieldCount++;
          } else {
            partialBDGSA.satelliteIDs[i] = 0;
          }
        }
      }

      // pdop
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 3 ) {
          if ( (partialBDGSA.pdop = decodeFloatValue(work)) != GPS_BADVALUE ) {
            fieldCount++;
          }
        }
      }

      // hdop
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 3 ) {
          if ( (partialBDGSA.hdop = decodeFloatValue(work)) != GPS_BADVALUE ) {
            fieldCount++;
          }
        }
      }

      // vdop
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 3 ) {
          if ( (partialBDGSA.vdop = decodeFloatValue(work)) != GPS_BADVALUE ) {
            fieldCount++;
          }
        }
      }

      if ( fieldCount >= 1 ) {
        partialBDGSA.sentence++;
        memcpy(&gps_BDGSA,&partialBDGSA,sizeof(partialBDGSA));
        return TRUE;
      }

      return FALSE;

}

bool navcoreBDParse::processBDRMC(const char *sentence)
{
    UINT32 count;
     UINT8 fieldCount;
     char work[FIELD_BUFFER_LENGTH];
     const char *field;
     double  outLat  = 0;
     double  outLon = 0;

     lastBDType = NMEA_MESSAGE_TYPE_RMC;

     field = sentence;
     fieldCount = 0;

     // initialise BDRMC sentence values
     count = partialBDRMC.sentence;
     initBDRMC();
     partialBDRMC.sentence = count;
     successParse = FALSE;
     effectiveGPS = FALSE;
     // UTC
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 6 ) {
         if ( (partialBDRMC.utc = decodeTimeValue(work) ) != TIME_BAD_VALUE ) {
           fieldCount++;
         }
       }
     }

     // status
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 1 ) {
         partialBDRMC.status = work[0];
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
         if ( (partialBDRMC.latitude = decodeLatLong(work)) != GPS_BADVALUE ) {
           fieldCount++;
         }
       }
     }
     // latitude direction (N,S)
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 1 ) {
         fieldCount++;
         if ( (*field == 'S') && (partialBDRMC.latitude != GPS_BADVALUE) ) {
           partialBDRMC.latitude = -partialBDRMC.latitude;
         }
       }
     }

     // longitude
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 7 ) {
         if ( (partialBDRMC.longitude = decodeLatLong(work)) != GPS_BADVALUE ) {
           fieldCount++;
         }
       }
     }
     // longitude direction (E,W)
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 1 ) {
         fieldCount++;
         if ( (*field == 'W') && (partialBDRMC.longitude != GPS_BADVALUE) ) {
           partialBDRMC.longitude = -partialBDRMC.longitude;
         }
       }
     }

     // ground speed
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 3 ) {
         if ( (partialBDRMC.speed = decodeFloatValue(work)) != GPS_BADVALUE ) {
           fieldCount++;
         }
       }
     }

     // heading
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 3 ) {
         if ( (partialBDRMC.heading = decodeFloatValue(work)) != GPS_BADVALUE ) {
           fieldCount++;
         }
       }
     }

     // date
     if ( (field = findNextField(field)) != NULL ) {
       if ( readField(work,field) >= 6 ) {
         if ( (partialBDRMC.date = time_stringToUlong(NULL,work)) != TIME_BAD_VALUE ) {
           fieldCount++;
         }
       }
     }

     if(partialBDRMC.utc>0&&partialBDRMC.latitude!=GPS_BADVALUE&&partialBDRMC.longitude!=GPS_BADVALUE)
     {
         successParse = TRUE;
         utc = partialBDRMC.utc;
         latitude = partialBDRMC.latitude;
         longitude = partialBDRMC.longitude;
         speed = partialBDRMC.speed;
         heading = partialBDRMC.heading;
         date = partialBDRMC.date;
     }
     else
     {
         utc = 0;
         latitude = GPS_BADVALUE;
         longitude = GPS_BADVALUE;
     }

     if ( fieldCount >= 1 ) {
       partialBDRMC.sentence++;
       //@TPE S
       /*LOG_INFO(0, ("processBDRMC (before CN-condition +[%d, %d]",
            partialBDRMC.latitude,
            partialBDRMC.longitude));*/
   #ifdef _WIN32
       if (strncmp(gloadMapName,"CN",2)==0){
   /*      LOG_INFO(0, ("processBDRMC+[%d, %d]",
            partialBDRMC.latitude,
            partialBDRMC.longitude));*/
         ConverCHNCoords( partialBDRMC.latitude/100000.0, partialBDRMC.longitude/100000.0, &outLat, &outLon);
         partialBDRMC.latitude = (outLat*100000.0);
         partialBDRMC.longitude = (outLon*100000.0);
   /*     LOG_INFO(0, ("processBDRMC-[%d, %d]",
          partialBDRMC.latitude,
          partialBDRMC.longitude));*/
       }
   #endif
       //@TPE E
       memcpy(&gps_BDRMC,&partialBDRMC,sizeof(partialBDRMC));
       return TRUE;
     }

     return FALSE;

}


const char * navcoreBDParse ::findNextField(const char * sentence)
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


UINT8 navcoreBDParse::readField(char *dest, const char *sentence)
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
INT32 navcoreBDParse :: decodeIntegerValue(const char *text)
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

INT32 navcoreBDParse::decodeTimeValue(const char *text)
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

INT32 navcoreBDParse::decodeLatLong(const char *text) {
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

INT32 navcoreBDParse::decodeFloatValue(const char *text)
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



bool navcoreBDParse ::processBDGSV(const char *sentence)
{
    UINT32 count;
    UINT8 i;
    UINT8 index;
    INT32 message;
    INT32 messages;
    INT32 value;
    char work[FIELD_BUFFER_LENGTH];
    const char *field;

    lastBDType = NMEA_MESSAGE_TYPE_GSV;

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
            //set the flag indicating if the full BDGSV set of sentences have been received
            gps_fullBDGSV=FALSE;
            if (message==1){
              gps_Rx_BDGSV1=TRUE;
              gps_Rx_BDGSV2=FALSE;
              gps_Rx_BDGSV3=FALSE;
          partialBDGSV.satelliteTracked = 0;
        }
      }
    }

      //@MIC
      gps_Rx_BDGSV4=FALSE;
      gps_Rx_BDGSV5=FALSE;
      gps_Rx_BDGSV6=FALSE;
      //@MIC
      if (messages==1){
        gps_fullBDGSV=TRUE;
      }
    }
    if ((message==2) && (gps_Rx_BDGSV1==TRUE)){
      gps_Rx_BDGSV2=TRUE;
      gps_Rx_BDGSV3=FALSE;
      //@MIC
      gps_Rx_BDGSV4=FALSE;
      gps_Rx_BDGSV5=FALSE;
      gps_Rx_BDGSV6=FALSE;
      //@MIC
      if (messages==2){
        gps_fullBDGSV=TRUE;
      }
    }
    if ((message==3) && (gps_Rx_BDGSV2==TRUE)){
      gps_Rx_BDGSV3=TRUE;
      //@MIC
      gps_Rx_BDGSV4=FALSE;
      gps_Rx_BDGSV5=FALSE;
      gps_Rx_BDGSV6=FALSE;
      //@MIC
      if (messages==3){
        gps_fullBDGSV=TRUE;
      }
    }
    //@MIC
    if ((message==4) && (gps_Rx_BDGSV3==TRUE)){
      gps_Rx_BDGSV4=TRUE;
      gps_Rx_BDGSV5=FALSE;
      gps_Rx_BDGSV6=FALSE;
      if (messages==4){
        gps_fullBDGSV=TRUE;
      }
    }
    if ((message==5) && (gps_Rx_BDGSV4==TRUE)){
      gps_Rx_BDGSV5=TRUE;
      gps_Rx_BDGSV6=FALSE;
      if (messages==5){
        gps_fullBDGSV=TRUE;
      }
    }
    if ((message==6) && (gps_Rx_BDGSV5==TRUE)){
      gps_Rx_BDGSV6=TRUE;
      if (messages==6){
        gps_fullBDGSV=TRUE;
      }
    }
    //@MIC


    // iBDore messages with null message or message count fields
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
      count = partialBDGSV.sentence;
      initBDGSV();
      partialBDGSV.sentence = count;
    }
    // increment message count
    gsvMessageCount++;

    // # satellites being tracked
    if ( (field = findNextField(field)) != NULL ) {
      if ( readField(work,field) >= 1 ) {
        if ( (value = decodeIntegerValue(work)) != GPS_BADVALUE ) {
          partialBDGSV.satelliteCount = (UINT8)value;
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
            partialBDGSV.prn[index] = (UINT8)value;
            gsvFieldCount++;
          }
        }
      }

      // elevation
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          if ( (value = decodeIntegerValue(work)) != GPS_BADVALUE ) {
            partialBDGSV.elevation[index] = (UINT8)value;
            gsvFieldCount++;
          }
        }
      }

      // azimuth
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          if ( (value = decodeIntegerValue(work)) != GPS_BADVALUE ) {
            partialBDGSV.azimuth[index] = (UINT16)value;
            gsvFieldCount++;
          }
        }
      }

      // snr
      if ( (field = findNextField(field)) != NULL ) {
        if ( readField(work,field) >= 1 ) {
          if ( (value = decodeIntegerValue(work)) != GPS_BADVALUE ) {
            partialBDGSV.snr[index] = (UINT8)value;
            if ( value > 0 ) {
              ++partialBDGSV.satelliteTracked;
            }
            gsvFieldCount++;
          } else {
            partialBDGSV.snr[index] = 0;
          }
        }
      }
    }

    /* A complete sentence has all the appropriate fields filled */
    /* A complete SET of sentences is also true if: */
    /* if ( (message == messages) && (gsvMessageCount == messages) ) */
    if ( gsvFieldCount >= messages ) {
      // perhaps copy sentences across to valid sentences when they become valid
      partialBDGSV.sentence++;
      memcpy(&gps_BDGSV,&partialBDGSV,sizeof(partialBDGSV));
      return TRUE;
    }

    return FALSE;
  }

// convert 2 UPPERCASE hex chars to UINT8 binary
UINT8 navcoreBDParse:: hexToBinary (const char *hex) {
  UINT8 rval;

  rval = ((*hex < 'A') ? ((*hex - '0') << 4) : ((*hex - 'A' + 10) << 4));
  hex++;
  rval += ((*hex < 'A') ? (*hex - '0') : (*hex - 'A' + 10));
  return (rval);
}

// parses the PTTKT sentence into the BD_partialPTTKT structure which is
// copied over to the externally visible PTTKT structure if any non null
// fields are decoded.
BOOL8 navcoreBDParse ::processPTTKT(const char *sentence)
{
  char  work[FIELD_BUFFER_LENGTH];
  UINT8 index;
  const char *field, *wrk;

  lastBDType = NMEA_MESSAGE_TYPE_TRAFFIC;

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
           BD_partialPTTKT.tmcData[index] = hexToBinary (wrk);
            wrk += 2;
          }

          BD_partialPTTKT.lastMessage = TMC_MSG_TYPE_DATA;
          memcpy (&BD__PTTKT,&BD_partialPTTKT,sizeof(BD__PTTKT));
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
              BD_partialPTTKT.hardwarePresent = (UINT8)atoi(work);
              break;
            case 1: // tuned frequency (decimal number)
              BD_partialPTTKT.frequency = (UINT16)atoi(work);
              break;
            case 2: // Block Count (decimal number)
              BD_partialPTTKT.blockCount = (UINT8)atoi(work);
              break;
            case 3: // Traffic Message Count
              BD_partialPTTKT.tmcCount = (UINT8)atoi(work);
              break;
            case 4: // error rate (decimal number)
              BD_partialPTTKT.errorRate = (UINT8)atoi(work);
              break;
          }
        } else {
          return FALSE;
        }
      }
      BD_partialPTTKT.lastMessage = TMC_MSG_TYPE_STATUS;
      memcpy (&BD__PTTKT,&BD_partialPTTKT,sizeof(BD__PTTKT));
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
              BD_partialPTTKT.tuneStatus = (UINT8)atoi(work);
              break;
          }
        } else {
          return FALSE;
        }
      }

      BD_partialPTTKT.lastMessage = TMC_MSG_TYPE_TUNE;
      memcpy (&BD__PTTKT,&BD_partialPTTKT,sizeof(BD__PTTKT));
      return TRUE;
    }
    else if (strncmp (work,"WAKE",4)==0)
    {
      // The WAKE command is generated when
      // the (Wince) target comes out of sleep mode
      BD_partialPTTKT.lastMessage = TMC_MSG_TYPE_WAKEUP;
      BD__PTTKT.lastMessage = TMC_MSG_TYPE_WAKEUP;
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
              BD_partialPTTKT.stnCurrent = (BOOL8)atoi(work);
              break;
            case 1: // Station Count (decimal number)
              BD_partialPTTKT.stnCount = (UINT8)atoi(work);
              break;
            case 2: // station Index (decimal number)
              BD_partialPTTKT.stnIndex = (UINT8)atoi(work);
              break;
            case 3: // station Frequency (decimal number)
              BD_partialPTTKT.stnFrequency = (UINT16)atoi(work);
              break;
            case 4: // station PI code (hex)
              wrk = work;
              BD_partialPTTKT.stnPI = (hexToBinary (wrk) << 8);
              wrk+=2;
              BD_partialPTTKT.stnPI += (hexToBinary (wrk));
              break;
            case 5: // station signal strength
              BD_partialPTTKT.stnSignal = (UINT8)atoi(work);
              break;
            case 6: // station flag
              BD_partialPTTKT.stnFlag = (UINT8)atoi(work);
              break;
            case 7: // station name
              strcpy (BD_partialPTTKT.stnName, work);
              break;
          }
        } else
        {
          return FALSE;
        }
      }
      BD_partialPTTKT.lastMessage = TMC_MSG_TYPE_LIST;
      memcpy (&BD__PTTKT,&BD_partialPTTKT,sizeof(BD__PTTKT));
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
              BD_partialPTTKT.stnFrequency = (UINT16)atoi(work);
              break;
            case 1: // station PI code (hex)
              wrk = work;
              BD_partialPTTKT.stnPI = (hexToBinary (wrk) << 8);
              wrk+=2;
              BD_partialPTTKT.stnPI += (hexToBinary (wrk));
              break;
            case 2: // station signal strength
              BD_partialPTTKT.stnSignal = (UINT8)atoi(work);
              break;
          }
        } else
        {
          return FALSE;
        }
      }
      BD_partialPTTKT.lastMessage = TMC_MSG_TYPE_SIGSTRENGTH;
      memcpy (&BD__PTTKT,&BD_partialPTTKT,sizeof(BD__PTTKT));
      return TRUE;
    }
    else if (strncmp (work,"RESET",5)==0)
    {
      // return the RESET command sent to the hardware
     BD_partialPTTKT.lastMessage = TMC_MSG_TYPE_RESET;
      BD__PTTKT.lastMessage = TMC_MSG_TYPE_RESET;
      return TRUE;
    }
  }
  return FALSE;
}


