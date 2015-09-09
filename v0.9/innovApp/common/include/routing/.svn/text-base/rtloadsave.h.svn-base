//
// Declaration of journey fast restart functionality
//
// $Id: rtloadsave.h 4954 2006-07-20 04:18:31Z wongd $
//

#ifndef RTLOADSAVE_H
#define RTLOADSAVE_H

#include <routing/rtjourney.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @ingroup Deprecated
/// @{

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// @typedef enum TPathRestartErrType
/// Enumeration of results from #journey_load and #journey_save
typedef enum {
  /// success
  PATH_RESTART_SUCCESS,
  /// IO error
  PATH_RESTART_IO_ERROR,
  /// Could not find map. Must be correct map including version
  PATH_RESTART_MAP_MISSING,
  /// Restart data is invalid
  PATH_RESTART_DATA_INVALID,
  /// Tried to save an unfinished route
  PATH_RESTART_ROUTE_UNFINISHED,
  /// Ran out of memory
  PATH_OUT_OF_MEM
} TPathRestartErrType;


/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/

extern MAPCORE_API
TPathRestartErrType journey_save(const TJourney *pThis,
                                 OS_FILE fp,
                                 char *outputMapName,
                                 UINT32 outputMapNameSize,
                                 TJourneyLegNumber *outputLegNumber);

extern MAPCORE_API
TPathRestartErrType journey_load(TJourney *pThis,
                                 OS_FILE fp,
                                 char *outputMapName,
                                 UINT32 outputMapNameSize,
                                 TJourneyLegNumber *outputLegNumber);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* RTLOADSAVE_H */
