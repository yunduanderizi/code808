#ifndef TRAUTHP_H
#define TRAUTHP_H

#include <platform.h>
#include <mapdata/enroadP.h>
#include <traffic/trtimeP.h>

/// The service key value that indicates the service key is unknown.
#define NO_SERVICE_KEY 0xFF

UINT8 tmcGetAuthorisedServiceKey(const TCountryCode *mcc,
                                 UINT8 cc,
                                 UINT8 ltnbe,
                                 UINT8 sid,
                                 TRDSTime currentTime);

BOOL8 tmcAuthInit(void);

BOOL8 tmcAddAuthorisedService(const TCountryCode *mcc,
                              UINT8 cc,
                              UINT8 ltnbe,
                              UINT8 sid,
                              UINT8 svk);

#endif
