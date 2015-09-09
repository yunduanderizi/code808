#ifndef ENUTILP_H
#define ENUTILP_H

#include <mapdata/enutil.h>
#include <mapdata/enroadP.h>
#include <language/lacodepage.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
/* Compare two strings s1 and s2 up to a pair of dollar signs.
   This will ignore directionals in out name string format

   @param s1 The first string
   @param s1 The second string
   
   @return 0 if equal as defined, non-zero otherwise. */
extern INT32 string_compareWithoutPrefix(const UTF16* s1,
                                         const UTF16* s2);


/* Given two segments guess to see if they
   are part of the same dual-carriageway. */
extern BOOL8 segments_IsDualCarriageWay(const TRoadSegment *segment1,
                                        const TRoadSegment *segment2);


//TODO add comments 
INT32 util_decodeVarSizeListAddress( 
  UINT8   *addr,
  INT32   lastId,
  UINT32  maxCount,
  UINT8   *numBytes,
  BOOL8   *complete );

#ifdef __cplusplus
}
#endif

#endif /* ENUTILP_H */
