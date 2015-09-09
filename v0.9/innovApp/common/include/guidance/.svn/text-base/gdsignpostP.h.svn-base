#ifndef GDSIGNPOSTP_H
#define GDSIGNPOSTP_H

#include <guidance/gdlistP.h>
#include <guidance/gdjourneyinfoP.h>
#include <mapdata/enroadP.h>
#include <language/lacodepageP.h>
#include <routing/rtjourneyP.h>

#define MAX_SIGNS_PER_INTERSECTION 16

/// @internal
/// Sign post journey information.
typedef struct {
  /// @internal
  /// Sign post information. (TRoadSignPost is 1324 bytes)
  TRoadSignPost signPost;
  /// @internal
  /// Source index of the sign post node in the list of path segments
  INT32 sourcePathIndex;
  /// @internal
  /// The index of the last segment at which the signpost matches the path
  /// segments.  
  INT32 finalPathIndex;
  /// @internal
  /// TRUE if this sign post matches the path segments
  BOOL8 pathMatch;
} TGuideSignPost;


typedef struct {
  /// @internal
  /// The signposts for this intersection (stored in reverse driving order).
  TGuideSignPost guideSignPosts[MAX_SIGNS_PER_INTERSECTION];
  /// @internal
  /// Number of signposts in the signPosts array  
  INT32 numSignPosts;
  /// @internal
  /// The journey path through the intersection
  const TPath *pPath;
  /// @internal
  /// The journey leg number of the path through the intersection
  TJourneyLegNumber legNumber;
} TGuideSignPostIntersection;


extern void gdSignPost_addSignPostInfo(TGuideSignPostIntersection *pThis,
                                       INT32 firstPathIndex,
                                       INT32 lastPathIndex);

extern void gdSignPost_init(TGuideSignPostIntersection *pThis,
                            const TJourney *pJourney,
                            TJourneyLegNumber legNumber);

extern BOOL8 gdSignPost_mergeIntoGuideList(TGuideSignPostIntersection *pThis,
                                          TGuideList *pGuideList,
                                          const TGuideJourneyInfo *pJourneyInfo);

extern const UTF16 *gdSignPost_directionalAsString(const UTF16 *directional);

#endif /* GDSIGNPOSTP_H */
