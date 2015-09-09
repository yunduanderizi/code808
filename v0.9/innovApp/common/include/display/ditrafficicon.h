#ifndef DITRAFFICICON_H
#define DITRAFFICICON_H

#include <display/diiconranklist.h>
#include <routing/rtjourney.h>
#include <traffic/trapi.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup TrafficIcons
/// @ingroup Icons
/// @{

/// @brief Gets the details of the traffic event represented by a
///        displayed icon
///
/// @param[in]    pThis the displayed icon
/// @param[in]  journey the journey to use for calculation of
///                     journey-related information, or \c NULL if
///                     this information is not wanted
/// @param[out]   event pointer to the structure that will be
///                     populated with information about the event
///
/// @return \c TRUE if \p pThis is a traffic icon and its details can
///         be retrieved
extern MAPCORE_API
BOOL8 IconDisplayRank_GetTrafficDetails(const TIconDisplayRank *pThis,
                                        const TJourney         *journey,
                                        TAppTrafficEvent       *event);

#ifdef __cplusplus
}
#endif

#endif
