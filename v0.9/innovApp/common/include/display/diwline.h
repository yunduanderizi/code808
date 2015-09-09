#ifndef DIWLINE_H
#define DIWLINE_H

#include <innovEngine_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TLine
{
  TPos *p1;
  TPos *p2;
} TLine;

extern MAPCORE_API
void Line_Define(TLine *pThis, TPos *p1, TPos *p2);

#ifdef __cplusplus
}
#endif

#endif
