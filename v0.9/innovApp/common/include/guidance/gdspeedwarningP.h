#ifndef GDSPEEDWARNINGP_H
#define GDSPEEDWARNINGP_H

#include <guidance/gdspeedwarning.h>

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern void speedWarning_new(TSpeedWarning *pThis);

extern void speedWarning_clear(TSpeedWarning *pThis);

extern void speedWarning_checkSpeed(TSpeedWarning *pThis,
                                    UINT32 UTC);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* GDSPEEDWARNINGP_H */
