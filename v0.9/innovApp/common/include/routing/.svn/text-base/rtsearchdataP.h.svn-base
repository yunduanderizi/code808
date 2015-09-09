#ifndef RTSEARCHDATAP_H
#define RTSEARCHDATAP_H



#include <logging/enlog.h>
#include <logging/sdebugP.h>
#include <routing/rtmapdumpP.h>
#include <routing/rtsearchnodeP.h>



/*----------------------------------------------------------------*/
/* AVL TREE INTERFACE                                             */
/*----------------------------------------------------------------*/

// The following macros/defines must be set up before #including cavl_if.h
// to declare the interface to the avl tree structure.
#define AVL_UNIQUE(x) searchNodeTree_ ## x
#undef  AVL_PRIVATE
#define AVL_PRIVATE_MODIFIERS
#define AVL_HANDLE    TSearchNode *
#define AVL_KEY       TSearchNodeKey

// AVL_MAX_DEPTH has to be chosen according to the maximum number of nodes
// in the tree.
//
// The web page http://www.geocities.com/wkaras/gen_c/cavl_tree.html gives
// a table and explains how to chose AVL_MAX_DEPTH.
//
// The maximum number of nodes is typically about 100000 so AVL_MAX_DEPTH
// should be at least 23 according to the above web page..  In practice,
// since we don't use the iter() function of the avl tree, it costs nothing
// to increase AVL_MAX_DEPTH up to 32 so let's chose 32 (beyond 32, avl tree
// lib uses a slightly different code to record branches).  Value of 32 can
// accomodate up to 9,227,464 nodes which is more than enough.
#define AVL_MAX_DEPTH 32
#undef  AVL_INSIDE_STRUCT
#undef  AVL_BUILD_ITER_TYPE
#undef  AVL_SIZE

#include <containers/cavl_if.h>

typedef searchNodeTree_tree  TSearchNodeTree;


/*----------------------------------------------------------------*/
/* CUSTOM/ADDITIONAL AVL TREE FUNCTIONS                           */
/*----------------------------------------------------------------*/

// Find the first node in the avl tree after the node specified
// which has roadType as least as major as `minRoadType'.
//
// @param  tree The searchNodeTree_tree to search (not modified)
// @param  after Only consider nodes after this node.
//               Specify NULL to consider the entire tree
// @param  minRoadType The minimum road type to consider
// @return The identified node or NULL if none matched
TSearchNode* searchNodeTree_findNextMajorNode( 
  TSearchNodeTree *pThis, 
  TSearchNode     *after,
  TRoadType       minRoadType );


// Count the number of nodes in an searchNodeTree_tree with roadType as
// least as major as `minRoadType'.
//
// @param  pThis The searchNodeTree_tree to search (not modified)
// @param  minRoadType The minimum road type to count
// @return number of nodes in `tree' with backsegments that have the
//           required or a more major road type
INT32 searchNodeTree_countMajorNodes( 
  TSearchNodeTree *pThis, 
  TRoadType       minRoadType );


// Find the first node in the avl tree after the node specified.
//
// @param  tree The searchNodeTree_tree to search (not modified)
// @param  after Only consider nodes after this node.
//               Specify NULL to consider the entire tree
// @return The identified node or NULL if none matched
TSearchNode* searchNodeTree_findNextNode( 
  TSearchNodeTree *pThis, 
  TSearchNode     *after );


// Dump a search tree to a shapefile.
//
// @param pThis  The search tree to dump
// @param pOther The other search tree required for the dump. This 
//   will always be the open list when dumping the closed list.
// @param name The file name to use.  This should not include and
//   extension, and the global name prefix will be prepended.
// @param dumpstyle The style to use
//   - MAPDUMP_STYLE_LINES dumps back segment with all shape points
//   - MAPDUMP_STYLE_NODES gives just the node position 
// @param closedList TRUE if we are dumping the closed list AND we 
//  have implicit storage enabled. 
void searchNodeTree_dumpShapeFile(
  TSearchNodeTree *pThis,
  TSearchNodeTree *pOther,
  const char      *name,
  TMapdumpStyle   style,
  BOOL8           closedList );



/*----------------------------------------------------------------*/
/* A* SEARCH DATA                                                 */
/*----------------------------------------------------------------*/

/// The number of entries in the cache. This value has been found by
/// experimenting.
#define LC_CACHE_SIZE 20

/// This structure contains one entry in the cache. We store the cost with the
/// node pointer to avoid jumping in the memory to the search node, to gather
/// its cost.
typedef struct TLCCacheEntry {
  TSearchNode *node;
  UINT32      cost;
} TLCCacheEntry;

/// The least-cost cache used to accelerate the search for the cheapest node.
typedef struct TLeastCostCache {
  TLCCacheEntry entries[LC_CACHE_SIZE];
  UINT32        first;
} TLeastCostCache;


/// The complete search data needed for an A* search, consisting of open and 
/// closed list and a least-cost node cache for quickly determining the cheapest
/// open node.
typedef struct TAStarSearchData {
  TSearchNodeTree openList;
  TSearchNodeTree closedList;
  TLeastCostCache lcCache;
} TAStarSearchData;


/*----------------------------------------------------------------*/
/* A* SEARCH DATA FUNCTIONS                                       */
/*----------------------------------------------------------------*/

// Initialises the A* search data.
//
// @param[in,out]  pThis
//   The A* search data to initialise (must not be NULL).
void aStarSearchData_init(
  TAStarSearchData *pThis );


// Adds an open node to the open list. It must NOT be in the list already.
//
// @param[in,out]  pThis
//   The A* search data structure containing the open list the node shall be
//   inserted.
// @param[in]      node
//   The node to add to the cache (must not be NULL).
void aStarSearchData_addNewOpenNode(
  TAStarSearchData *pThis,
  TSearchNode      *node );


// Moves a node from the open to the closed list. It must be in the open.
// 
// @param[in,out]  pThis,
//   The search data the node belongs to.
// @param[in]      node
//   The search node to move (must not be NULL).
void aStarSearchData_moveNodeToClosedList(
  TAStarSearchData *pThis,
  TSearchNode      *node );


// Moves a node from the closed list to the open list. It must NOT be in the
// open list already.
//
// @param[in,out]  pThis,
//   The search data the node belongs to.
// @param[in]      node
//   The search node to move (must not be NULL).
void aStarSearchData_moveNodeToOpenList(
  TAStarSearchData *pThis,
  TSearchNode      *node );


// Removes a node from the closed list.
//
// @param[in,out]  pThis,
//   The search data the node shall be removed from.
// @param[in]      node
//   The search node to remove.
void aStarSearchData_removeNodeFromClosedList(
  TAStarSearchData *pThis,
  TSearchNode      *node );


// If a node in the open list will get a lower cost, we have to update it
// in the least-cost cache, too. For this please call this function instead
// of just changing the estimatedTotalCost of the search node. The total
// cost will also be set by this function.
// The cost of the node must NOT be already changed.
//
// @param[in,out]  pThis
//    The A* search data the node belongs to.
// @param[in,out]  node
//   The node of which the cost will be reduced (must not be NULL).
// @param[in]      newCost
//   The new cost the search node will have after the function call.
void aStarSearchData_reduceOpenNodeCost(
  TAStarSearchData *pThis,
  TSearchNode      *node,
  UINT32           newCost );


// Returns the cheapest node of the open list.
TSearchNode* aStarSearchData_getCheapestOpenNode( 
  TAStarSearchData *pThis );



#endif  // ifndef RTSEARCHDATAP_H
