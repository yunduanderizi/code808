//
// Declaration of routing related segment list functions
//
// $Id: rtsegmentlistP.h 4954 2006-07-20 04:18:31Z wongd $
//

/*----------------------------------------------------------------*/
/* MANAGE ROUTE LISTS */
/*----------------------------------------------------------------*/
/// These functions manage the memory which stores lists of road segment
/// pointers associated with the paths between each destination in a
/// journey.
///
/// Each path (TPath) has a two lists of road segment pointers
/// (TSegmentList) in different large arrays.  One list is for driving
/// ordered segments and one for id ordered segments.  There are two
/// segment list stores (TSegmentListStore) to manage these arrays, one for
/// all the driving ordered lists and one for all the id ordered lists.
///
/// There is a special TSegmentList in each store that contains all the
/// store memory not currently allocated.  These are the "free segment
/// lists".  The free lists are used by the routing subsystem to build new
/// lists before assigning them a real index.  (Memory in the lists is used
/// THEN allocated.)
///
/// As each TSegmentList points to a block of road segments pointers in an
/// array.  The order and index of the lists within the journey can changed
/// without physically copying the segments around. Becasue of this, users
/// of these APIs must take care not to use stale TSegmentList data.

#ifndef RTSEGMENTLISTP_H
#define RTSEGMENTLISTP_H

#include <routing/rtsegmentlist.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* DEFINES */
/*----------------------------------------------------------------*/
/// @internal
/// This segment list contains all the free segments in the store
#define LISTSTORE_FREE_LIST LISTSTORE_MAX_LISTS

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// @typedef UINT32 TSegmentListID
/// This structure describes a list of road segments that together form a
/// @todo Document this...
typedef UINT32 TSegmentListID;

/*----------------------------------------------------------------*/
/* SEGMENT LIST FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// Given a list of path ordered segments, generate a list of path indicies
// that is segment id ordered.
//
// So, if the route has 5 segments ...
// pThis   = { 2014, 1983, 3445, 2226, 2014 }
// Then the generated segment id sorted list of indices will be
// outList = { 1, 0, 4, 3, 2 }
//
// Using this, a binary search can determine if a segment is in a path.
// NOTE: Duplicate segment ids have their indicies stored in route order
BOOL8 segmentList_sortSegments( 
  TSegmentList* inList,   /* pThis */
  TSegmentList* outList );

/*----------------------------------------------------------------*/
/* SEGMENT LIST STORE FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// Create an empty segmentlist store, initialising all lists to contain no
// segments, and placing all the free memory into the free-segment list.
//
// @param list The segment list store
// @param memoryBase The memory block which this segment list store
//   controls and which will contain all the sequences of segment id's.
// @param totalSegmentCount The maximum number of segement id's that
//   memoryBase can contain 
void segmentListStore_create( 
  TSegmentListStore *pThis,
  TSegmentID *memoryBase,
  UINT32 totalSegmentCount );

// @brief Copies the contents of one segment list store to another.
//
// The entire contents of the source's segment buffer are copied to
// the target's segment buffer, which must be at least as large as
// the source's segment buffer.  Furthermore, the two segment buffers
// must not overlap.
//
// @param[in]   pThis the source segment list store
// @param[out] target the target segment list store
//
// @return \c TRUE if the source was successfully copied to the
//         target
BOOL8 segmentListStore_clone(const TSegmentListStore *pThis,
                             TSegmentListStore *target);

// Clear (and recover memory from) all segment lists in the store
// All free space is returned to the free segment block
//
// @param pThis Store to clear
void segmentListStore_clear( 
  TSegmentListStore *pThis );

// Get the specifed segment list from the store
//  _________________________________________________________________
// | 3   | 2                 | 0          | 1        | free space    |
// |_____|___________________|____________|__________|_______________|
//
// @param[in]  pThis The store
// @param[in]  id The id of the segment list to retrieve
// @param[out] pSegmentList The returned store item.
// @return     FALSE if `id' is invalid, TRUE otherwise
BOOL8 segmentListStore_getSegmentList( 
  TSegmentListStore* pThis,
  TSegmentListID     id,
  TSegmentList**     pSegmentList );

// Get the free space available in the specified store
//
// @param[in]  pThis The store
// @param[out] pSegmentList The returned store item.
// @return     TRUE always
BOOL8 segmentListStore_getFreeSegmentList( 
  TSegmentListStore* pThis,
  TSegmentList**     pSegmentList );

// Replace a segment list with new data that is at the start of the
// LISTSTORE_FREE_LIST
// 
// Conceptually, list i is deleted, then all the items higher in
// memory are shuffled down, and finally the part of the
// LISTSTORE_FREE_LIST containing the new data becomes list i.
// 
// e.g. Memory layout before replacing block 1 with new segs:
//  _________________________________________________________________
// | 3   | 1      | 2                 | 0          | new segs | free |
// |_____|________|___________________|____________|__________|______|
// 
// e.g. Memory layout after updating block 1 with new segs:
//  _________________________________________________________________
// | 3   | 2                 | 0          | 1        | free          |
// |_____|___________________|____________|__________|_______________|
// 
// @param pThis The list object to alter
// @param id The index of the item in the list to delete and
//   replace with a new segment block. 
void segmentListStore_updateListFromFreeSegmentList( 
  TSegmentListStore *pThis,
  TSegmentListID     id );

#ifdef __cplusplus
}
#endif

#endif // RTSEGMENTLISTP_H
