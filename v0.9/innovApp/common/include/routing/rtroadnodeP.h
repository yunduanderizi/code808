#ifndef RTROADNODEP_H
#define RTROADNODEP_H

#include <mapdata/enroadP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// Unpack all the turn restrictions stored in a node. It is assumed that
// 'restrictions' is large enough to hold the full complement of
// restrictions possible at a node.
//
// NOTE: THE SECTOR CONTAINING pThis MUST BE LOCKED IN ITS ORIGINAL
// POSITION PRIOR TO CALLING THIS FUNCTION
//
// @param  pThis The node whose turn restrictions should be unpacked
// @param  restrictions Where the restrictions should be unpacked to
// @return the number or restrictions unpacked
UINT32 roadNode_unpackAllTurnRestrictions( 
  const TRoadNode* pThis,
  TRoadTurnRestriction *restrictions );


#ifdef __cplusplus
}
#endif

#endif // RTROADNODEP_H
