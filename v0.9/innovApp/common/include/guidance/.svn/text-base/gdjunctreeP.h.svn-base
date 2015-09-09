#ifndef GDAVLTREEP_H
#define GDAVLTREEP_H

#include <containers/gelist.h>
#include <containers/gerbtree.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @internal
/// AVL nodes must follow this format. Actual nodes will contain more data
/// after the key.
typedef struct {
  TRBTreeNode rbnode;

  /// a TSegmentID or TNodeID.
  UINT32 ID;
} TGuideJunctionTreeNode;


typedef struct {
  // Current number of nodes in the tree.
  UINT32 n;

  // Memory to store an array of nodes.
  UINT8 *nodes;

  // Maximum number of nodes in the tree;
  UINT32 nMax;

  // The size of each node in bytes;
  UINT32 nodeSize;

  // We maintain a list of free node slots to recover freed memory.
  TList freeNodeList;

  /// The tree itself
  TRBTree tree;

} TGuideJunctionTree;


extern BOOL8 gdtree_create(TGuideJunctionTree *pThis,
                           UINT32 nodeSize,
                           UINT32 maxNodes);

extern void gdtree_clear(TGuideJunctionTree *pThis);

extern void gdtree_destroy(TGuideJunctionTree *pThis);

extern void *gdtree_insert(TGuideJunctionTree *pThis,
                           void *node,
                           BOOL8 *alreadyExists);

extern void *gdtree_delete(TGuideJunctionTree *pThis, UINT32 key);

extern void *gdtree_find(TGuideJunctionTree *pThis,
                         UINT32 key);

extern UINT32 gdtree_numNodes(const TGuideJunctionTree *pThis);

extern void *gdtree_startIterLeast(const TGuideJunctionTree *pThis);

extern void *gdtree_incrementIter(const TGuideJunctionTree *pThis, void *node);

#ifdef __cplusplus
}
#endif

#endif /* GDAVLTREEP_H */
