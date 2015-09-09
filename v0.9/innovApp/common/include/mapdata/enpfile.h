#ifndef ENPFILE_H
#define ENPFILE_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif



/// @addtogroup PolyFiles
/// This module is responsible for loading and linking the available
/// polygons.  The actual file handling associated with polygon loading is
/// handled at the OS level.  Only the static data chunks are actually
/// loaded, and the dynamic chunks are loaded and unloaded as required.
/// @ingroup MapData 
/// @{ 

#ifndef NO_DEPRECATED_ROUTING
/// @deprecated
///    Use mapfile_load() instead.
///
/// Load multiple polygon maps (topo and other classes of polygons)
/// into memory and dynamically link them at different global map addresses.
/// The maps to load are discovered by calling the user supplied platform
/// function os_getMapName with the mapType set to "poly:" and "topo:".
/// @return Returns the number of polygon maps successfully loaded (which
/// include topo polygons as well as other polygons classes)
DEPRECATED(
extern MAPCORE_API UINT32 polyfile_load( void )
);
#endif // NO_DEPRECATED_ROUTING

/** Get the filename of a certain poly file.

    @return A pointer to the filename of the specified map file. */
extern MAPCORE_API char *polyfile_getFilename( UINT32 fileIndex );

/** Get the number of poly maps loaded.

    @return The number of poly maps loaded. */
extern MAPCORE_API UINT32 polyfile_getMapCount( void );

/** This function returns the number of polygon basemaps loaded.

    @return The number of polygon basemaps loaded. */
extern MAPCORE_API UINT32 polyfile_baseMapCount( void );

/// @} 


#ifdef __cplusplus
}
#endif

#endif /* ENPFILE_H */
