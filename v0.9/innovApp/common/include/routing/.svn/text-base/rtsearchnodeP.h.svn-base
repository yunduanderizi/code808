//
// Declaration of the A* search node type, the avl tree of search nodes
// used during the A* search and associated functions.
//
// $Id: rtsearchnodeP.h 10913 2008-08-13 12:24:10Z irene.peng $
//

#ifndef RTSEARCHNODEP_H
#define RTSEARCHNODEP_H

#include <mapdata/enroad.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/

/// Search node structure.  Nodes of this type form the search trees for
/// the origin and destination A* searches.  These search trees are stored
/// as AVL trees for quick access via road node id.
typedef struct TSearchNode {

  // these members are needed for the AVL tree:
  struct TSearchNode *left;
  struct TSearchNode *right;


  // the rest of the structure is used for the A* algorithm

  // NOTE: The AVL key is built out of segment ID and direction.
  TSegmentID  backSegment;            ///< ID (offset) of the segment the node (see below)  <---+
                                      ///< was reached from                                     |
                                                                                                // AVL key
  // flags coming from 1 word...                                                                |
  BITFIELD32 direction:1;             ///< the direction we use in that search node         <---+
                                      ///< (0=towards first node, 1=towards last node)           
  INT32      balance:2;               ///< Used to balance nodes in AVL tree (-1, 0, 1)
  BITFIELD32 mapLayer:1;              ///< at the moment there are only layer 0 and 1
  BITFIELD32 intermediate:1;          ///< is this an intermediate node? (hinted)
  BITFIELD32 actualCost:27;           ///< actual cost of all roads/turns from origin

  // these members store some additional information about where we come from
  // and where we go
  TNodeID             node;           ///< ID (offset) of the road node
  struct TSearchNode* back;           ///< prior search node, scan back to orig

  // flags coming from 1 word...
  BITFIELD32  backbackHeading:8;      ///< heading back from back node (only if > 2 segs)
  BITFIELD32  roadType:3;             ///< the road type of the back segment
  BITFIELD32  travelDistance:21;      ///< travel distance

  // flags coming from 1 word...
  BITFIELD32  enteredResidential:1;   ///< TRUE if we have entered a residential area (for exploration)
  BITFIELD32  onRoute:1;              ///< Is the backsegment on the route (for exploration)
  BITFIELD32  uturn:1;                ///< must u-turn to reach node (used at origin)
  BITFIELD32  ferryFlag:1;            ///< TRUE if backSeg is ROAD_FORM_FERRY
  BITFIELD32  estimatedTotalCost:28;  ///< actual cost to here + heuristic to dest

#ifdef MAPDUMP_EXTRA_FIELDS
  UINT32      nodesSearched;
  UINT8       roadFilter;
  UINT32      travelCost;
  UINT32      turnCost;
#endif

} TSearchNode;


typedef UINT64  TSearchNodeKey;


// Obtains the segment ID from a TSearchNodeKey.
// <key> should be of type TSearchNodeKey
// Returns a segment ID of type TSegmentID
#define SEARCH_NODE_KEY_GET_BACK_SEGMENT(key)                     \
  ( (TSegmentID)((key) >> 1) )
// Obtains the direction from a TSearchNodeKey.
// <key> should be of type TSearchNodeKey
// Returns values 0 or 1
#define SEARCH_NODE_KEY_GET_DIRECTION(key)                        \
  ( (BITFIELD32)((key) & 1) )

// Builds a TSearchNodeKey out of a segment ID and a direction.
// <backSegmentID> should be of type TSegmentID
// <direction> should be an integer with a value of 0 or 1 depending,
// if the path directions is to the first or to the last node
// Returns a value of type TSearchNodeKey
#define SEARCH_NODE_KEY_BUILD(backSegmentID, direction)           \
  ( (((TSearchNodeKey)(backSegmentID)) << 1) | (TSearchNodeKey)(direction) )
// Builds a TSearchNodeKey out of a TRoadSegment and a node ID.
// <backSegment> should be of type const TRoadSegment*
// <nodeID> should be of type TNodeID
// Returns a value of type TSearchNodeKey
#define SEARCH_NODE_KEY_BUILD_FROM_NODE_ID(backSegment, nodeID)  \
  SEARCH_NODE_KEY_BUILD((backSegment)->ID, ((backSegment)->lastNode == (nodeID)) ? 1 : 0)

// Obtains the TSearchNodeKey from a TSearchNode.
// <node> should be of type const TSearchNode*
// Returns a value of type TSearchNodeKey
#define SEARCH_NODE_GET_KEY(node)                                 \
  SEARCH_NODE_KEY_BUILD((node)->backSegment, (node)->direction)
// Sets the backSegment and the direction in a TSearchNode to the values
// stored in a TSearchNodeKey.
// <node> should be of type TSearchNode*
#define SEARCH_NODE_SET_KEY(node,key)                             \
  { (node)->backSegment = SEARCH_NODE_KEY_GET_BACK_SEGMENT(key);  \
    (node)->direction = SEARCH_NODE_KEY_GET_DIRECTION(key); }


/// The maximum value that can be stored in the actualCost field of
/// TSearchNode.
#define MAX_SEARCH_NODE_ACTUAL_COST ((1<<27)-1)
#define MAX_SEARCH_NODE_TRAVEL_DIST ((1<<21)-1)


/*----------------------------------------------------------------*/
/* SEARCH NODE MEMORY MANAGEMENT */
/*----------------------------------------------------------------*/
/// Maximum number of chunks of memory from the map cache which can be
/// allocated for search nodes
#define SEARCH_NODES_NUM_CHUNKS (15)


/// This type represents a chunk of memory allocated to hold nodes explored
/// while routing.
typedef struct TSearchNodeMemChunk {
  INT32        allocatedCount;  ///< number of TSearchNodes allocated..
  TSearchNode* addr;            ///< ... at this address
} TSearchNodeMemChunk;


/// Struct holding details about a "searchNode" memory store Search nodes
/// are allocated off the map cache -- this struct allows the nodes for
/// seperate pathCalculators to be managed independantly.
typedef struct TSearchNodeAllocator {
  /// Current chunk to allocate nodes from
  INT32 currentChunk;
  //
  /// Current index in current chunk
  INT32 currentChunkNodeIndex;
  //
  /// Total space (in count of nodes) allocated in the mapcache so far.
  /// This includes space used plus free space already reserved.
  INT32 mapcacheAllocatedCount;
  //
  /// Total space (in count of nodes) used in the mapcache so far.  This
  /// includes only space returned by searchNode_new() and is usually
  /// smaller than the amount of space allocated on the mapcache.
  INT32 usedCount;
  //
  /// Maximum space (in count of nodes) that is allowed to be used
  INT32 maximumAllowed;
  //
  /// set to true when an OOM condition is encountered
  BOOL8 outOfMemory;
  //
  /// Array of chunks of memory allocated for search nodes.
  /// Chunks can have variable number of allocated nodes.
  TSearchNodeMemChunk searchNodes[SEARCH_NODES_NUM_CHUNKS];
  //
} TSearchNodeAllocator;


// Create space for a new search node.  This function is used during
// search expansion to create a new search node structure that can be
// added to one of the search avl trees.
//
// It allocates search node in chunks from map cache.  Memory must be
// released at the end of routing by calling searchNodes_freeAll().
//
// @return a pointer to the newly created node or NULL if no more memory
//         is available.
TSearchNode *searchNode_new( TSearchNodeAllocator* a );


// Free all chunks of mapcache memory that had been allocated by
// searchNode_new().
//
// @return number of chunks of memory freed (not the number of nodes).
INT32 searchNodes_freeAll( TSearchNodeAllocator* a );


// Retrieve the number of nodes used.  This is the number of successful
// calls to searchNode_new() since the last _freeAll() or _init().
INT32 searchNodes_getUsed( TSearchNodeAllocator* a );


// Retrieve the maximum number of search nodes that will be allowed.  This
// can be used to determine how close to out-off-memory the search is
INT32 searchNodes_getMaxAllowed( TSearchNodeAllocator* a );


// Set the maximum number of search nodes that will be allowed.  These
// nodes will be allocated on the mapcache.  Route calculation will return
// "Out of memory" once this many search nodes have been used.
//
// NOTE: Calling this is optional, the innovEngine defaults to 100000 nodes
// NOTE: Each node requires 8 words of space on the mapcache
// NOTE: Mapcore enforces a limit of 600000 nodes even if this is set higher
void searchNodes_setMaxAllowed( TSearchNodeAllocator* a, INT32 numNodes );

// Query if search node memory is exhaused.
BOOL8 searchNodes_getOutOfMemory( TSearchNodeAllocator* a );


// Wipe all record of having allocated nodes without actually unallocating
// any allocated search nodes.  This is only useful at startup or after the
// mapcache has also been completely reset.
BOOL8 searchNodes_init( TSearchNodeAllocator* a );


// Determine the distance (metres) of a node in the search tree from the
// search origin.  This is used to determine if high turn costs should be
// used or not.  As such we are only interested in short distances.
//
// NOTE: If the distance to the origin is greater than maxDist then
// UINT32_MAX is returned.
// NOTE: If pThis is more than RT_MAX_HIGHTURNCOSTSEGMENTS segments from the
// search origin then UINT32_MAX is returned.
// NOTE: The partial segment at the start of the route is not counted
//
// @param[in]  pThis
//   The node whose dist from the origin should be determined
// @param[in]  maxDistance
//   The maximum distance to accumulate before giving up
// @param[in]  maxSegments
//   The maximum number of segments to accumulate before giving up
// @return 
//   The calculated distance or UINT32_MAX if it is too far
UINT32 searchNode_getDistFromSearchOrigin(
  const TSearchNode* pThis,
  UINT32 maxDistance,
  UINT32 maxSegments );


#ifdef __cplusplus
}
#endif

#endif // RTSEARCHNODEP_H
