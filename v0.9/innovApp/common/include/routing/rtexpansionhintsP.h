#ifndef RTEXPANSIONHINTSP_H
#define RTEXPANSIONHINTSP_H

#include <mapdata/enroad.h>
#include <routing/rtpathlistP.h>
#include <routing/rtsearchnodeP.h>

#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------*/
/* DEFINES */
/*----------------------------------------------------------------*/
/// Maximum number of hints to allow.  15 should be heaps
#define RT_MAX_EXPANSION_HINTS 15



/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// Type representing a single expansion hint
typedef struct TExpansionHint {
  TPathList       searchDirection;
  TSearchNodeKey  searchNodeKey;
} TExpansionHint;


/// Type representing a queue of expansion hints
typedef struct TExpansionHints {
  UINT32          beginIndex;  //< the first valid element
  UINT32          endIndex;    //< one past the last valid element
  BOOL8           locked;      //< queue is locked => new hints cannot be added
  TExpansionHint  hint[RT_MAX_EXPANSION_HINTS + 1];
} TExpansionHints;



/*----------------------------------------------------------------*/
/* MORE INCLUDES */
/*----------------------------------------------------------------*/
#include <routing/rtpathcalc.h>



/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// check if the hint queue is empty
//
// @return TRUE if empty, FALSE otherwise
BOOL8 expansionHints_isEmpty(
  const TExpansionHints* pThis );


// Debug function - dumps the state of the expansion hint queue if the
// rtExpansionHints debug flag is set
void expansionHints_dumpState(
  const TExpansionHints* pThis );


// Clear the contents of the expansion hints queue - used when we want to
// invalidate all current hints
void expansionHints_clear(
  TExpansionHints* pThis );


// Retrieve the number of elements in the expansion hint queue
//
// @return Number of hints in the hint queue, 0 if it is empty
UINT32 expansionHints_getCount( 
  const TExpansionHints* pThis );


// Delete (pop) the front element from the hint queue
//
// @return FALSE if the hint queue is empty
BOOL8 expansionHints_popFront( 
  TExpansionHints* pThis );

/// Check if a given searchnode is already in the hint queue
///
/// @param pThis The hint queue to check
/// @param searchDirection The relevant search, ORIGIN or DESINTATION
/// @param searchNodeKey Key of the search node to check
/// @return TRUE if the node is already on the given hint queue
BOOL8 expansionHints_exists(
  TExpansionHints* pThis, 
  TPathList searchDirection,
  TSearchNodeKey searchNodeKey);

// Add a new hint to the end of the hint queue
//
// @param pThis The hint queue to add the hint to
// @param searchDirection The relevant search, ORIGIN or DESINTATION
// @param searchNodeKey Key of the search node in the hint
// @return FALSE if the queue if full or locked, TRUE otherwise
BOOL8 expansionHints_pushBack( 
  TExpansionHints* pThis, 
  TPathList searchDirection,
  TSearchNodeKey searchNodeKey );


// Get the search direction of the front element in the hint queue
//
// @param[out] searchDirection ORIGIN or DESINTATION
// @return FALSE if the hint queue is empty
BOOL8 expansionHints_getFrontSearchDirection( 
  const TExpansionHints* pThis,
  TPathList* searchDirection );


// Get the search node (presumably in the open list of the searchDirection
// search) that needs to be expanded to take this hint.
//
// @param[out] searchNodeKey Key of front hint search node
// @return FALSE if the hint queue is empty
BOOL8 expansionHints_getFrontSearchNodeKey( 
  const TExpansionHints* pThis,
  TSearchNodeKey* searchNodeKey );


// Lock the expansion hint queue to prevent new hints being added
void expansionHints_lockQueue(
  TExpansionHints* pThis );


// Unlock the expansion hint queue so that new hints can be added
void expansionHints_unlockQueue(
  TExpansionHints* pThis );


// Is the hint queue locked?
//
// @return FALSE if the hint queue is not locked, TRUE if it is locked
BOOL8 expansionHints_isLocked(
  TExpansionHints* pThis );


#ifdef __cplusplus
}
#endif

#endif // RTEXPANSIONHINTSP_H
