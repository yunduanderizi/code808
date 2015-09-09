#ifndef TRFILEP_H
#define TRFILEP_H

#include <traffic/trfile.h>
#include <traffic/trlocalP.h>
#include <graphics/grbitmap.h>
#include <language/lainit.h>
#include <mapdata/enmfileP.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup TrafficFiles
/// @ingroup Traffic
/// @brief Interface to files required by the TMC system.
///
/// At present this icludes
/// - Event class bitmap file
/// - Event description text  (in multiple languages)
///
/// @{

/// Maximum length of any string read from a text file.
#define MAX_LOCAL_TEXT_LENGTH 250


#pragma pack(push, 1)
typedef struct TRDSHeader
{
     char     desc[16];
     UINT32   ver;          //version
     UINT32   date;
     UINT32   time;
     char     cc[4];        //country code
     char     sysName[16];  //system name
}TRDSHeader;

typedef struct TtypeTable
{
     UINT8  locID;
     UINT8  reserve;
     UINT16 storeID;
     UINT32 POIOffset;
}TtypeTable;

typedef struct  TBrand
{
  UINT16  brandID;
  UINT32  brandOffset;
  UINT16  brandCount;
}TBrand;

#pragma pack(pop)

typedef enum {
  RDS_SERVICE_PARKING,
  RDS_SERVICE_KTV,
  RDS_SERVICE_MAX = 64
} TRDSService;

#define TRAFFIC_MAX_PATH 200

UINT16 parkingCount, KTVCount, KTVvendorID;
TtypeTable *parkingRec, *KTVRec;

void tmcCloseRDSFile(void);
void RDS_shutdown(void);
BOOL8 tmcOpenRDSFile(const char *mapName);
char rdsMapName[MAX_BUNDLE_COMPONENT_BASENAME];

//@TPE E

BOOL8 tmcSetBitmapFile(const char *bitmapFileName);

/** Read a class bitmap from the bitmap file and return a pointer to it's
    structure.

    @param eventClass  The class ID of the event 
    @param large       Select the large icon
    @param detour      Select the detour-recommended icon
    @param avoided     Select the avoided icon
    @return   a pointer to the bitmap structure

*/
TBitmap *tmcGetEventBitmap (UINT16 eventClass, BOOL8 large, BOOL8 detour,
                            BOOL8 avoided);


/** Open the event text file and read in the index
    Uses currently configured voice file to determine language

    @return   non zero for success

*/
UINT32 tmcOpenTextFile (TLanguageCode lang, UINT8 metricUnits);

/** Close the event text file


*/
void tmcCloseTextFile (void);

TLanguageCode tmcCurrentLanguage(void);

BOOL8 tmcUnitsMetric(void);

BOOL8 tmcGetEventText(const TtmcExtendedEvent *ev,
                      UTF16 *text, UINT32 textLength,
                      BOOL8 *usingQuantifier);

BOOL8 tmcGetSupplementaryText(UINT32 index,
                              UTF16 *text, UINT32 textLength);

const UTF16 *tmcGetQuantifierText(UINT32 index);

const UTF16 *tmcGetHourQuantifierText(void);

/// @brief Retrieves the saved details for a PI code
///
/// Note that the return location table number is untranslated,
/// i.e. it must be passed through #tmcTranslateLTN before being used
/// to decode TMC messages.
///
/// @param[in]         pi the PI code
/// @param[out]       ltn set to the location table number as
///                       transmitted, i.e. untranslated
/// @param[out] encrypted set to the encrypted flag
/// @param[out]       sid set to the service identifier
///
/// @return \c TRUE if the PI code was found
UINT8 tmcGetLTNforPI (UINT16 pi, UINT8 *ltn, BOOL8 *encrypted, UINT8 *sid);

UINT8 tmcSaveLTNforPI (UINT16 pi, UINT8 ltn, BOOL8 encrypted, UINT8 sid);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* TRFILEP_H */
