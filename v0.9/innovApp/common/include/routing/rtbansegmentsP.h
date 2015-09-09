#ifndef RTBANSEGMENTSP_H
#define RTBANSEGMENTSP_H

#include <routing/rtbansegments.h>
#include <routing/rtexpansionP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
BOOL8 banSegments_getHaveChanged( void );

void banSegments_setChangedToFalse( void );

INT32 banSegments_getNumBans( void );

void banSegments_init( void );

BOOL8 banSegments_getBanSegmentCost( 
  TSegmentID segId, 
  TExpansionCost *cost );

TPathGeobanType banSegments_getBanSeverity( 
  UINT32 segmentId );

#ifdef __cplusplus
}
#endif

#endif // RTBANSEGMENTSP_H
