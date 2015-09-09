#ifndef TRCONFIGP_H
#define TRCONFIGP_H

#include <traffic/trconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup TrafficStartup
/// @ingroup Traffic
/// @brief Startup Configuration. At present unsure of what goes in
///        here.
///
/// @{

/** Tick call to monitor maplock situation on startup. This is
    called when status messages arrive without frequency or blockcount */
void tmcWaitingForMaplock (void);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* TRCONFIGP_H */
