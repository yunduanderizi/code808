#ifndef ENINITP_H
#define ENINITP_H

#include <environ/eninit.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Return the number of times engine has been (re)initialised */
extern INT32 engine_initCount(void);

/* Returns true if the engine is currently in a valid state */
extern BOOL8 engine_isValid( void );

typedef void  TEngineShutdownProc(void *context);
typedef void *TEngineShutdownProcId;

#define ENGINE_SHUTDOWN_PROC_ID_NULL (NULL)

extern TEngineShutdownProcId
engine_addShutdownProc(TEngineShutdownProc *shutdownProc,
                       void                *context);

extern void
engine_removeShutdownProc(TEngineShutdownProcId shutdownProcId);

#ifdef __cplusplus
}
#endif

#endif /* ENINITP_H */
