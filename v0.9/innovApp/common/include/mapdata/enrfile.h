#ifndef ENRFILE_H
#define ENRFILE_H

#include <mapdata/enmfile.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup MapData 
/// @{ 

/// @addtogroup RoadFiles 
/// This module is responsible for loading and linking the available
///    road maps. The actual file handling associated with road loading is
///    handled at the OS level.  Only the static data chunks are actually
///    loaded, and the dynamic chunks are loaded and unloaded as required.
///    @todo
///    AT PRESENT LOADING MAPS CAN TAKE SOME TIME.  AS AN INTERIM SOLUTION SOME
///    ADDITIONAL FUNCTIONS WILL BE PROVIDED THAT INITIATE THE LOADING PROCESS 
///    AND THEN CONTINUE IT UNTIL FINISHED IN A POLLED MANNER. 
/// @ingroup MapData 
/// @{ 

#ifndef NO_DEPRECATED_ROUTING
/// @deprecated
///    Use mapfile_load() instead.
///
/// load multiple road maps into memory and dynamically link them at 
/// different global map addresses.  The maps to load are discovered by 
/// calling the user supplied platform function os_getMapName with the 
/// mapType set to "road:".
/// 
/// @return Returns the number of road maps successfully loaded.
/// 
/// @todo
/// AT PRESENT THIS FUNCTION LARGE AMOUNT OF DATA INTO MEMORY THAT RESIDES 
/// THERE FOR THE COMPLETE DURATION OF THE APPLICATION.  THIS CAUSES THE 
/// MAP ENGINE TO USE A LARGE AMOUNT OF MEMORY AND IT WILL REQUIRE CHANGES 
/// TO THE ENGINE TO ELIMINATE THIS PROBLEM.  THIS FUNCTION CALLS os_exit 
/// IF A MAP COULD NOT BE LOADED.  THIS BEHAVIOR MAY BE CHANGED IN FUTURE 
/// SO THAT THE FUNCTION RETURNS GRACEFULLY IF IT CANNOT LOAD ALL MAPS.
DEPRECATED(
extern MAPCORE_API UINT32 roadfile_load( void )
);

DEPRECATED(
extern MAPCORE_API void roadfile_mapCount(INT32 *detailedLoadedCount,
                                          INT32 *baseLoadedCount,
                                          INT32 *conflictCount,
                                          INT32 *errorCount)
);
#endif // NO_DEPRECATED_ROUTING


/// @} 
/// @} 

#ifdef __cplusplus
}
#endif

#endif /* ENRFILE_H */
