#ifndef ENPIFILE_H
#define ENPIFILE_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/// @addtogroup MapData
///    @{                 

/// @addtogroup POIFiles
/// This module is responsible for loading and linking the available
/// POI maps.  The actual file handling associated with map loading is
/// handled at a lower level.  Only the static data chunks are actually
/// loaded, and the dynamic chunks are loaded and unloaded as required.
/// @todo
/// AT PRESENT LOADING MAPS CAN TAKE SOME TIME.  AS AN INTERIM SOLUTION SOME
/// ADDITIONAL FUNCTIONS WILL BE PROVIDED THAT INITIATE THE LOADING PROCESS 
/// AND THEN CONTINUE IT UNTIL FINISHED IN A POLLED MANNER. 
/// @ingroup MapData 
/// @{ 




#ifndef NO_DEPRECATED_ROUTING
/** Load multiple POI maps into memory and dynamically link them at different
    global map addresses.  The maps to load are discovered by calling the 
    user supplied platform function os_getMapName with the mapType set 
    to "poi:".
    
    @return Returns the number of POI maps successfully loaded.
    
    @todo
    AT PRESENT THIS FUNCTION LARGE AMOUNT OF DATA INTO MEMORY THAT RESIDES 
    THERE FOR THE COMPLETE DURATION OF THE APPLICATION.  THIS CAUSES THE 
    MAP ENGINE TO USE A LARGE AMOUNT OF MEMORY AND IT WILL REQUIRE CHANGES 
    TO THE ENGINE TO ELIMINATE THIS PROBLEM.  THIS FUNCTION CALLS os_exit 
    IF A MAP COULD NOT BE LOADED.  THIS BEHAVIOR MAY BE CHANGED IN FUTURE 
    SO THAT THE FUNCTION RETURNS GRACEFULLY IF IT CANNOT LOAD ALL MAPS. */
DEPRECATED(
extern MAPCORE_API UINT32 poifile_load( void )
);

DEPRECATED(
extern MAPCORE_API UINT32 poifile_loadCustomer( void )
);
#endif // NO_DEPRECATED_ROUTING

/** Get the filename of a certain poi file.

    @return A pointer to the filename of the specified map file.*/
extern MAPCORE_API char *poifile_getFilename( UINT32 fileIndex );

/** Get the number of poi maps loaded.

    @return The number of poi maps loaded. */
extern MAPCORE_API UINT32 poifile_getMapCount( void );

/// @} 
/// @} 


#ifdef __cplusplus
}
#endif

#endif /* ENPIFILE_H */
