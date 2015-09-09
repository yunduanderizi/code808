//
// Declaration of routing related segment list functions
//
// $Id: rtsegmentlist.h 9241 2008-01-14 21:33:22Z marcus.spranger $
//

#ifndef RTSEGMENTLIST_H
#define RTSEGMENTLIST_H

#include <mapdata/enroad.h>
#include "rtitinerary.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @{
/// @addtogroup JourneyCalculator
/// @ingroup Routing
/// @{

/*----------------------------------------------------------------*/
/* DEFINES */
/*----------------------------------------------------------------*/
/// @internal
/// This is the maximum number of destinations that can be routed
/// to in a complete journey.
#define LISTSTORE_MAX_LISTS ITINERARY_MAX_DESTINATIONS+4

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// @internal
/// This structure describes a list of road segments that together form a
/// continuous section of the route path.  These will be stored either in
/// driving order, or in ID order.
typedef struct TSegmentList {
  INT32       usedCount;    ///< number of valid segments in this list
  INT32       totalCount;   ///< total space available in this list
  TSegmentID* segments;     ///< pointer to the first segment
} TSegmentList;

/// @internal
/// This structure manages a compacted heap of TSegmentLists.  Because the
/// heap is regularly compacted after TSegmentListStore operations, care
/// must be taken to refresh stale addresses.  Also note that the order of
/// lists in the items array is likely to be different to the order they
/// are stored on the heap.
typedef struct TSegmentListStore {
  TSegmentID* memoryBase;   ///< Base of the heap
  INT32 totalSegmentCount;  ///< Total space available on the heap
  TSegmentList item[LISTSTORE_MAX_LISTS + 1]; ///< Lists in the store
} TSegmentListStore;

/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif // RTSEGMENTLIST_H
