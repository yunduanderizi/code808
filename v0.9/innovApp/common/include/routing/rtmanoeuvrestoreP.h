#ifndef RTMANOEuVRESTOREP_H
#define RTMANOEVURESTOREP_H

#include <innovEngine_mapdata.h>
#include <mapdata/enroadP.h>
#include <routing/rtsearchnodeP.h>
#include <routing/rtpathlistP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* DEFINES */
/*----------------------------------------------------------------*/
/// Maximum number of partially expanded manoeuvres handled.  15 is heaps,
/// I would be surprised to see more than 4 slots used
#define RT_MAX_MANOEUVRES_IN_PROGRESS 15


/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// Type representing a single manoeuvre that is partially expanded in one
/// of the the searches
typedef struct TPartExpManoeuvre {
  TPathList             searchDirection;
  TSearchNodeKey        searchNodeKey;
  BOOL8                 isForced;
  TRoadTurnRestriction  manoeuvre;
  UINT32                nextManoeuvreSegIndex;
} TPartExpManoeuvre;


/// Type representing manoeuvres that are partially expanded in the open
/// lists of the searches
typedef struct TPartExpManoeuvres {
  UINT32            beginIndex;  //< the first valid element
  UINT32            endIndex;    //< one past the last valid element
  TPartExpManoeuvre pem[RT_MAX_MANOEUVRES_IN_PROGRESS + 1];
} TPartExpManoeuvres;



/*----------------------------------------------------------------*/
/* MORE INCLUDES */
/*----------------------------------------------------------------*/
#include <routing/rtpathcalc.h>



/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// Debug function - dumps the state of the PEM queue if the rtManuvQueue
// debug flag is set
void partExpManoeuvres_dumpState(
  const TPartExpManoeuvres* pThis );


// Clear the contents of the PEM queue before routing or when
// transitioning to layer 1
void partExpManoeuvres_clear(
  TPartExpManoeuvres* pThis );

/// Removes an element from the PEM queue
/// @param pThis The PEM queue to remove from
/// @param searchDirection The direction of the current search 
///   (ORIGIN or DESTINATION)
/// @param searchNodeKey The key of the search node of the element to remove
void partExpManoeuvres_remove(
  TPartExpManoeuvres* pThis, 
  TPathList searchDirection,
  TSearchNodeKey searchNodeKey
  );

// Retrieve the number of elements in the PEM queue
//
// @return Number of manoeuvres in the PEM queue, 0 if it is empty
UINT32 partExpManoeuvres_getCount( 
  const TPartExpManoeuvres* pThis );


// Delete (pop) the front element from the PEM queue
//
// @return FALSE if the PEM queue is empty
BOOL8 partExpManoeuvres_popFront( 
  TPartExpManoeuvres* pThis );


// Add a new partially expanded manoeuvre to the end of the manoeuvre
// store
//
// @param pThis The PEM queue to add the manoeuvre to
// @param searchDirection The relevant search, ORIGIN or DESINTATION
// @param searchNodeKey Key of search node to expand next in manoeuvre
// @param isForced Should this manoeuvre be followed exclusively?
// @param nextSegIndex Index into manoeuvre of next segment to expand
// @param manoeuvre The manoeuvre itself
// @return FALSE if the queue if full, TRUE otherwise
BOOL8 partExpManoeuvres_pushBack( 
  TPartExpManoeuvres* pThis, 
  TPathList searchDirection,
  TSearchNodeKey searchNodeKey,
  BOOL8 isForced,
  UINT32 nextSegIndex,
  const TRoadTurnRestriction* manoeuvre );


// Get the search direction of the front element in the manoeuvre store
//
// @param[out] searchDirection ORIGIN or DESINTATION
// @return FALSE if the PEM queue is empty
BOOL8 partExpManoeuvres_getFrontSearchDirection( 
  const TPartExpManoeuvres* pThis,
  TPathList* searchDirection );


// Get the search node key (presumably in the open or closed list of the
// searchDirection search) that needs to be expanded to progress the
// application of the front manoeuvre.
//
// @param[out] searchNodeKey Key of next search node in front manoeuvre 
// @return FALSE if the PEM queue is empty
BOOL8 partExpManoeuvres_getFrontSearchNodeKey( 
  const TPartExpManoeuvres* pThis,
  TSearchNodeKey* searchNodeKey );


// Does the front manoeuvre in the manoeuvre store need to be followed at
// the exclusion of other paths?  Or can other paths be considered?
//
// @param[out] isForced Does this manoeuvre <b>need</b> to be followed?
// @return FALSE if the PEM queue is empty
BOOL8 partExpManoeuvres_getFrontIsForced( 
  const TPartExpManoeuvres* pThis,
  BOOL8* isForced );


// Retrieve the manoeuvre of the front element in the manoeuvre store
//
// @param[out] manoeuvre Pointer to front manoeuvre in the PEM queue
// @return FALSE if the PEM queue is empty
BOOL8 partExpManoeuvres_getFrontManoeuvre( 
  TPartExpManoeuvres* pThis,
  const TRoadTurnRestriction** manoeuvre );


// Retrieve the id of the next segment that will be expanded from
// searchNode as part of this manoeuvre
//
// @param[out] segId Id of next segment in manoeuvre to expand
// @return FALSE if the PEM queue is empty
BOOL8 partExpManoeuvres_getFrontNextSegId( 
  const TPartExpManoeuvres* pThis,
  TSegmentID* segId );


// Retrieve the index (in the manoeuvre) of the next segment that will be
// expanded from search node as part of this manoeuvre
//
// @param[out] segIndex Index of next segment in front manoeuvre
// @return FALSE if the PEM queue is empty
BOOL8 partExpManoeuvres_getFrontNextSegIndex( 
  const TPartExpManoeuvres* pThis,
  UINT32* segIndex );


#ifdef __cplusplus
}
#endif

#endif // RTMANOEUVRESTOREP_H
