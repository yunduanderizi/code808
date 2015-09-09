#ifndef TRAPIP_H
#define TRAPIP_H

#include <traffic/trapi.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUMBER_AVOIDED_EVENTS 50
UINT32 avoidedEventCodeList[MAX_NUMBER_AVOIDED_EVENTS];
UINT16 numAvoidedEventCodes;
/**
 * @brief Initialises the traffic core component
 *
 * This should be called prior to any other traffic function and must
 * be called before #routingSubsystem_initialise if traffic is to have
 * any effect on routing.
 *
 * @return \c TRUE if the traffic component was successfully
 *         initialised
 */
BOOL8 traffic_init(void);

BOOL8 trafficAPI_writeReciever(char c);

const char *trafficAPI_getRootPath(void);

#ifdef __cplusplus
}
#endif

#endif /* TRAPIP_H */
