#ifndef RTAPIBASEP_H
#define RTAPIBASEP_H

#include <routing/rtapibase.h>
#include <routing/rtpathcalcP.h>
#include <routing/rtjourneycalcP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* GLOBAL OBJECTS */
/*----------------------------------------------------------------*/
// Global path calculator
extern TPathCalculator apibase_pathCalculator;

// Global journey calculator
extern TJourneyCalculator apibase_journeyCalculator;


/*----------------------------------------------------------------*/
/* GLOBAL INITIALISATION */
/*----------------------------------------------------------------*/
// Global initialisation of the routing subsystem (replaces path_init()).
// This must be called before any other routing/avoidance API is called.
//
// @return TRUE if initialisation succeeds
BOOL8 routingSubsystem_initialise( void );


#ifdef __cplusplus
}
#endif

#endif // RTAPIBASEP_H
