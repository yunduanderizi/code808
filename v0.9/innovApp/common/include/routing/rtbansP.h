#ifndef RTBANSP_H
#define RTBANSP_H

#include <routing/rtbans.h>
#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Have any bans (of any type) changed since the last time
/// bans_setChangedToFalse() was called?
BOOL8 bans_getHaveChanged( void );


/// Reset haveChanged flags for all ban types
void bans_setChangedToFalse( void );


/// Initialise the ban system.  Delegates to the _init() functions for each
/// ban type.
void bans_init( void );


#ifdef __cplusplus
}
#endif

#endif // RTBANSP_H
