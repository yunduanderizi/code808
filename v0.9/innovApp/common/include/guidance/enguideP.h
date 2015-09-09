#ifndef ENGUIDEP_H
#define ENGUIDEP_H

#include <innovEngine_types.h>
#include <routing/rtpath.h>
#include <mapdata/enroadP.h>
#include <math/mtangleP.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Shape points on roundabouts can be very close together. This means heading
calculations are inaccurate. In the worst case, exits can appear to be on
the wrong side of the road. To try and avoid this, we ignore these close
shape points. */
#define MIN_VERTEX_SEPARATION 3

/* We need a higher tolerance level on freeways because angles tend to be very
narrow and roads tend to be very straight. */
#define MIN_VERTEX_SEPARATION_FREEWAY 5

//@MRS Start
/* We need expand beyond the shape points, if the segment is curved.20m is enought.
*/
#define MAX_VERTEX_SEPARATION  20
//@MRS End

/// return codes for getUnpackedSegmentHeading() result field
typedef enum {
  HEADING_OK = 0,
  HEADING_DEADEND,
  HEADING_NOSEGMENTS,
  HEADING_LIMITREACHED
} TUnpackHeadingStatus;


/*----------------------------------------------------------------*/

extern THeading guide_getSegmentHeading(TSegmentID segment, TNodeID nodeID);

extern THeading guide_getSegmentHeadingAtDist(TSegmentID segment, 
                                              TNodeID nodeID,
                                              INT32 dist, 
                                              TUnpackHeadingStatus *headingResult);

extern BOOL8 guide_clockMost(TSegmentID segmentID1,
                             TNodeID nodeID1,
                             TSegmentID segmentID2,
                             TNodeID nodeID2);

extern THeading guide_HeadingNodeToNode( TNodeID nodeID1, TNodeID nodeID2 );

extern BOOL8 guide_closedAtNode(const TRoadSegment* segment, TNodeID nodeID);

extern BOOL8 guide_restrictedTowardNode(const TRoadSegment* segment,
                                        TNodeID nodeID);

extern BOOL8 guide_restrictedAwayFromNode(const TRoadSegment* segment,
                                          TNodeID nodeID);

extern void guide_logPos(TPos pos);

extern void guide_logJunctionViewStatus(TGuideInstruction *instruction);

extern THeading guide_getUnpackedSegmentHeading(TRoadSegment *segment,
                                                TNodeID nodeID,
                                                BOOL8 nearRoundabout,
                                                INT32 minDistance,
                                                TUnpackHeadingStatus *result);

extern UINT16 segmentLength(const TPath *pPath,
                            INT32 pathIndex);

extern UINT32 segmentPathLength(const TPath *pPath,
                                INT32 pathSegIndex,
                                TRoadSegment *segment);

extern BOOL8 segmentEntryExitNodes(const TPath *pPath,
                                   INT32 pathIndex,
                                   TNodeID *entryNode,
                                   TNodeID *exitNode);

extern UINT32 guide_unpackPhoneticString(const TRoadStringID *inputStringID,
                                         TLanguageCode inputLangCode,
                                         BOOL8 allowTranslation,
                                         TLanguageCode outputLangCode,
                                         UTF16 *outputBuffer);

/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* ENGUIDEP_H */
