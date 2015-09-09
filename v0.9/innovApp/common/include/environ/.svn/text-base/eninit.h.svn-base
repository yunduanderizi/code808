#ifndef ENINIT_H
#define ENINIT_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Initialisation
/// @{



/** Initialises the MapCore library.
    This function should be called once prior to calling any other MapCore function.
    This function <b>must not</b> be called when unloading or reloading maps.
    @return TRUE if initialisation was successful. */
extern MAPCORE_API BOOL8 engine_init(void);

/** Shuts down the MapCore library. */
extern MAPCORE_API void engine_shutdown(void);

/// @}
#ifdef __cplusplus
}
#endif

#endif
