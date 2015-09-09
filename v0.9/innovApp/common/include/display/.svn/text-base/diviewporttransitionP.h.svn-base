#ifndef DIVIEWPORTTRANSITIONP_H
#define DIVIEWPORTTRANSITIONP_H

#include <display/diviewporttransition.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

extern ViewTransitionType ViewportConfiguration_Compare(ViewportConfiguration *pThis,
                                                        ViewportConfiguration *previousViewport,
                                                        struct TViewport *viewport,
                                                        TPos *translationVector);

extern void ViewportConfiguration_FromViewport(ViewportConfiguration *pThis,
                                               struct TViewport *viewport);

extern void ViewportConfiguration_Copy(ViewportConfiguration *pThis,
                                       ViewportConfiguration *viewport);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIVIEWPORTTRANSITIONP_H */
