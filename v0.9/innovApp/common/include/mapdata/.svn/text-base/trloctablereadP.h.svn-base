#ifndef TRLOCTABLEREADP_H
#define TRLOCTABLEREADP_H

#include <mapdata/enroadP.h>
#include <mapdata/enmapinfoP.h>
#include <traffic/trloctableP.h>
#include <traffic/trlocalP.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TtmcFullLocation {
  TtmcLocationInfo location;
  UTF16 roadNumber[TRAFFIC_FILE_MAX_TEXT_LENGTH];
  UTF16 roadName[TRAFFIC_FILE_MAX_TEXT_LENGTH];
  UTF16 firstName[TRAFFIC_FILE_MAX_TEXT_LENGTH];
  UTF16 secondName[TRAFFIC_FILE_MAX_TEXT_LENGTH];
  UTF16 negativeEndName[TRAFFIC_FILE_MAX_TEXT_LENGTH];
  UTF16 positiveEndName[TRAFFIC_FILE_MAX_TEXT_LENGTH];
} TtmcFullLocation;

#define TRAFFIC_FILE_MAX_NUM_DCAS 25

typedef struct TtmcLocationFile {
  OS_FILE file;
  TtmcLocationFileHeader header;

  /// @brief the number of road maps that contain locations from this
  ///        location file
  UINT8 numRoadMaps;

  /// @brief the location table index entries corresponding to the
  ///        location file for each road maps that contains such an
  ///        entry
  ///
  /// There are \c numRoadMaps valid entries in this array.  To find a
  /// particular location, call #tmcLocationFile_lockMapIndexes, then
  /// #road_unsafeFindTmcLocation, then
  /// ##tmcLocationFile_unlockMapIndexes.
  TRoadTmcLocationTableIndexRow mapIndexes[TRAFFIC_FILE_MAX_NUM_DCAS];

  TtmcLocationFormat locationFormat;
  TtmcRoadNameSortRules roadNameSortRules;
  TtmcLocationFileLocationCodes locationCodes;
  UINT32 locationDataOffset;
} TtmcLocationFile;

#ifndef NO_DEPRECATED_ROUTING
DEPRECATED(
UINT32 tmcLocationFile_registerLocationFiles(void)
);
#endif // NO_DEPRECATED_ROUTING

/// Status values for looking up location files.
typedef enum TtmcLocationFileResult {
  LOCATION_FILE_NOT_FOUND,      ///< No location file matches
  LOCATION_FILE_FOUND,          ///< One location file matches
  LOCATION_FILE_AMBIGUOUS       ///< More than one location file matches
} TtmcLocationFileResult;

const char *tmcLocationFile_identityString(const TtmcLocationFile *file);

/// @brief Check for existence of location file in the registered file
///        list (without opening the file).
///
/// @param[in] mapCountryCode
///        the two-letter map country code to use for disambiguation,
///        if known; may be \c NULL
/// @param countryCode the TMC country code
/// @param locationTableNumber the TMC location table number
///
/// @return whether there exists exactly one location file satisfying
///         the criteria
///
TtmcLocationFileResult tmcLocationFile_doesFileExist(
  const TCountryCode *mapCountryCode,
  UINT8               countryCode,
  UINT8               locationTableNumber);

/// @brief Gets a location file in the registered file list.
///
/// @param[in]  mapCountryCode the two-letter map country code to use
///                            for disambiguation, if known; may be \c
///                            NULL
/// @param         countryCode the TMC country code
/// @param locationTableNumber the TMC location table number
/// @param[out]           file receives a pointer to the location
///                            file, but only if the function returns
///                            \c found; must not be \c NULL
///
/// @return a status indicating whether a location file can be
///         uniquely identified from the criteria
///
TtmcLocationFileResult tmcLocationFile_getLocationFile(
  const TCountryCode  *mapCountryCode,
  UINT8                countryCode,
  UINT8                locationTableNumber,
  TtmcLocationFile   **file);

BOOL8 tmcLocationFile_open(TtmcLocationFile *f, const char *fileName);
void tmcLocationFile_close(TtmcLocationFile *f);

void tmcLocationFile_reset(void);
INT32 tmcLocationFile_findLocationIndex(const TtmcLocationFile *f,
                                        UINT16 locationCode);
BOOL8 tmcLocationFile_loadLocation(const TtmcLocationFile *file,
                                   UINT16 index, TtmcLocationInfo *location);
BOOL8 tmcLocationFile_loadFullLocation(const TtmcLocationFile *file,
                                       UINT16 index,
                                       TtmcFullLocation *fullLocation);
const TtmcLocationFormat *
tmcLocationFile_getLocationFormat(const TtmcLocationFile *f);

const TtmcRoadNameSortRules *
tmcLocationFile_getRoadNameSortRules(const TtmcLocationFile *f);

TMapfileStatus traffic_load(const char *const mapInfo);
//@TPE
TMapfileStatus rds_load(const char *const mapName);

#ifdef __cplusplus
}
#endif

#endif
