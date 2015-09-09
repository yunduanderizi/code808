#ifndef ENMFILE_H
#define ENMFILE_H

#include <platform_types.h>
#include <innovEngine_types.h>
#include <authentication/aufeature.h>

#ifdef __cplusplus
extern "C" {
#endif

//@todo move these into the platform. Unfortunately
// because the platform layer is intimately tied
// with the app, it can't be changed easily.
#define DOS_DIR_SEPARATOR "\\"
#define UNIX_DIR_SEPARATOR "/"

/// @addtogroup MapData 
/// @{ 
///The load status of a given map
typedef enum {
  /** map file was successfully loaded */
  MAPFILE_SUCCESS,
  /** not loaded due to error */
  MAPFILE_ERROR,
  /** not loaded due to conflict */
  MAPFILE_CONFLICT,
  /** not loaded due to missing map name */
  MAPFILE_NONAME,
  /** not loaded due to an IO error */
  MAPFILE_IO_ERROR,
  /** not loaded due to invalid version */
  MAPFILE_INVALID_VERSION,
  /** not loaded due to memory allocation errors */
  MAPFILE_NO_MEM,
  /** not loaded due to missing dependencies */
  MAPFILE_MISSING_DEP,
  /** Total number of status values, must come last */
  MAPFILE_STATUS_COUNT
} TMapfileStatus;

///The various types of map files
typedef enum {
  /** Road files */
  MAPTYPE_ROAD,
  /** POI files */
  MAPTYPE_POI,
  /** Geometry files */
  MAPTYPE_GEOM,
  /** Postcode files */
  MAPTYPE_POSTCODE,
  /** Traffic files */
  MAPTYPE_TRAFFIC,
  /** Global POI type file */
  MAPTYPE_POITYPE,
  /** Archive files */
  MAPTYPE_ARCHIVE,
  //@TPE S
  /** SmartKeyboard **/
  MAPTYPE_SMARTKEYBOARD,
  /** PointAddress **/
  MAPTYPE_POINTADDRESS,
  MAPTYPE_POINTADDRESSNAME,
  /** Highway Information **/
  MAPTYPE_HIGHWAY,
  /** RDS **/
  MAPTYPE_RDS,
  /** Search index **/
  MAPTYPE_SEARCHINDEX,
  //@TPE E
  /* NOTE: retain the following as the last entry in the enum */
  NUM_MAPTYPES
} TMapfileType;
#define MAPTYPE_INVALID NUM_MAPTYPES

/// @deprecated
///    When using the function mapfile_load() a callback is no longer needed.
///
/// Registers a callback function that will be called during map loading.
/// The function will be called only ONCE for each mapfile and
/// will include the status of the load
/// for that file. 
///
///   @param[in] context An arbitrary context pointer which will get passed 
///                            to the callback function.
///   @param[in] callback
///              pointer to the 
///              required callback function.
DEPRECATED(
extern MAPCORE_API void mapfile_registerCallback(
  void *context, 
  void (*callback)(void *, const char *mapName, TMapfileType type, TMapfileStatus status)
  )
);

extern MAPCORE_API
void mapfile_dumpLoadStatistics( void );

extern MAPCORE_API
TMapfileStatus mapfile_load(const char      *mapName,
                            TMapfileType     mapType,
                            TMapFeatureClass featureClass);

extern MAPCORE_API
BOOL8 mapfile_hasSpeedLimits( void );

extern MAPCORE_API
UINT32 mapfile_getLoadMapCount( TMapfileType mapType );

extern MAPCORE_API
BOOL8 mapfile_getVendorCode(TPos *currentPos, char *vendor);

const char *maptype_asText( TMapfileType type );

BOOL8 test_regionAndLanguage(const char* mapBaseName);

void set_poiAddressSearchType(BOOL8 poiSearchAddress);

BOOL8 get_poiAddressSearchType();

void set_useKeyword(BOOL8 useKeyword);

BOOL8 get_useKeyword();
/// @} 

#ifdef __cplusplus
}
#endif

#endif /* ENMFILE_H */
