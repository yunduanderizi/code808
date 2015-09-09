/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_containers_gerbtree_h
#define INCLUDE_GUARD_containers_gerbtree_h

/**
@file

@brief This module defines an implementation of a red-black tree.
*/

/* Include the following header for platform type definitions. */
#include <platform_types.h>


/**
@internal
This flag selects an implementation with the node colour and parent node pointer
packed into a single 32 bit word.

@note
The selected implementation requires that node pointers are aligned at even addresses.
*/
#define RBTREE_USE_PARENT_COLOUR_COMBO


#ifdef __cplusplus
extern "C" {
#endif


/* The tree type. */
typedef struct TRBTree TRBTree;

/* The tree node type. */
typedef struct TRBTreeNode TRBTreeNode;

/* The tree node colour type. */
typedef UINT8 TRBTreeNodeColour;


/**
@brief The type that represents the result of a compare.
*/
typedef INT32 TRBTreeCompareResult;

/**
@brief The result type returned from the #rbtree_find function.
*/
typedef INT32 TRBTreeFindResult;

/**
@brief The type that represents the side of a node to insert another node
into a tree.
*/
typedef INT32 TRBTreeInsertSide;


/**
@brief
This is the function type that is used by #rbtree_initCopy to copy individual
nodes of a red-black tree.
  
@param[in] node Points to the node to be copied.

@param[in] nodeCopyContext Points to user supplied data that is required by
the node copy function.
  
@return A pointer to a created node or NULL if the copy failed.
The created node will contain a copy of the contents of the user defined node structure.

@note
The tree node links will be set by the rbtree_initCopy function.
*/
typedef TRBTreeNode*
TRBTreeNodeCopy (TRBTreeNode* node,
                 void* nodeCopyContext);


/**
@brief
This is the function type that is used by #rbtree_destroy to destroy individual
nodes of a red-black tree.
  
@param[in] node Points to the node to be compared against the search key.

@param[in] nodeDestroyContext Points to user supplied data that is required by
the node destroy function.
*/
typedef void
TRBTreeNodeDestroy (TRBTreeNode* node,
                    void* nodeDestroyContext);


/**
@brief
This is the function type that is used by #rbtree_find and #rbtree_findRange
for locating nodes (or ranges of nodes) which match specified search keys,
or for finding where to insert a new node.

@param[in] key Points to the data that specifies a search key and possible additional
context data required to compare a node value against the search key.
  
@param[in] node Points to the node to be compared against the search key.
  
@return negative if the search key has a value less than the key of the specified node,
positive if the search key has a value greater than the key of the specified node,
and zero if the search key has a value equal to the key of the specified node.
*/
typedef TRBTreeCompareResult
TRBTreeKeyCompare (void* key,
                   const TRBTreeNode* node);


/**
@brief The red-black tree structure.

@note
A red-black tree structure with has all bits set to zero is equivalent
to an initialized empty tree.
@n This allows static declarations of red-black trees without requiring
explicit initialization.
*/
struct TRBTree {
  /** @internal The root node of the tree (NULL if none). */
  TRBTreeNode* root;
};


/**
@brief The red-black tree node structure.

User defined node structures must include this node structure at the beginning
of the structure.
@n The user structure must be cast to a #TRBTreeNode whenever it is passed to any
@a rbtree_* functions.
*/
struct TRBTreeNode {
  /**
  @internal
  @brief Points to the node on the left branch of this node (NULL if none).
  */
  TRBTreeNode* left;
  /**
  @internal
  @brief Points to the node on the right branch of this node (NULL if none).
  */
  TRBTreeNode* right;
#if defined(RBTREE_USE_PARENT_COLOUR_COMBO)
  /**
  @internal
  @brief Contains both a pointer to the parent node of this node (NULL if none),
  and also the colour of this node as required by the red-black tree algorithm.
  */
  UINTP parentColourCombo;
#else
  /**
  @internal
  @brief Points to the parent node of this node (NULL if none).
  */
  TRBTreeNode* parent;
  /**
  @internal
  @brief Stores the colour of this node as required by the red-black tree algorithm.
  */
  TRBTreeNodeColour colour;
#endif
};


/**
@brief Initialize a red-black tree.  The tree is initially empty.

@param[out] tree Points to the #TRBTree which is being initialized.

@note
This function is implemented as a macro.
*/
void rbtree_init (TRBTree* tree);

#define rbtree_init(tree) ((void)((tree)->root = NULL))


/**
@brief Initialize a red-black tree by moving the contents of another
red-black tree into the initialized tree.

This function defines a safe transfer of contents between two trees.

@param[out] tree Points to the #TRBTree which is being initialized.

@param[in,out] sourceTree Points to the #TRBTree which will have its
contents moved into the initialized tree.

@note
The sourceTree will be empty after this function exits.

@note
This function is implemented as a macro.
@n There must not be any side effects in the @a sourceTree argument.
*/
void rbtree_initMove (TRBTree* tree,
                      TRBTree* sourceTree);

#define rbtree_initMove(tree, sourceTree) ((void)((tree)->root = (sourceTree)->root,\
                                                  (sourceTree)->root = NULL))


/**
@brief Initialize a red-black tree with a (deep) copy of another red-black tree.

The reason a copy function is defined as an initialization function is
because the function does not need to balance the new tree.
The balancing structure from the source tree can be exactly duplicated.

@param[out] tree Points to the #TRBTree which is being initialized.

@param[in] sourceTree Points to the #TRBTree which will be copied into the
initialized tree.

@param[in] nodeCopy Points to a function that allocates a new tree node
and copies the logical contents into that new node.

@param[in] nodeCopyContext Points to user supplied data that is required by
the @a nodeCopy function.

@return TRUE if the copy was successful, FALSE if the copy failed.

@note
If the copy fails, then the caller must call the #rbtree_destroy function
to destroy the (partially copied) tree.
@n No other operations are valid on the tree.

@note
The tree node links are copied by the #rbtree_initCopy function.
*/
BOOL8 rbtree_initCopy (TRBTree* tree,
                       const TRBTree* sourceTree,
                       TRBTreeNodeCopy* nodeCopy,
                       const void* nodeCopyContext);


/**
@brief Destroy a red-black tree.

@param[in,out] tree Points to the #TRBTree which is being destroyed.

@param[in] nodeDestroy Points to a function that destroys a tree node.

@param[in] nodeDestroyContext Points to user supplied data that is required by
the @a nodeDestroy function.

@note
The rbtree_destroy function may be called on an already destroyed tree.
*/
void rbtree_destroy (TRBTree* tree,
                     TRBTreeNodeDestroy* nodeDestroy,
                     const void* nodeDestroyContext);


/**
@brief Remove all nodes from a red-black tree.

@param[in,out] tree Points to the #TRBTree which is being cleared.

@param[in] nodeDestroy Points to a function that destroys a tree node.

@param[in] nodeDestroyContext Points to user supplied data that is required by
the @a nodeDestroy function.
*/
void rbtree_clear (TRBTree* tree,
                   TRBTreeNodeDestroy* nodeDestroy,
                   const void* nodeDestroyContext);


/**
@brief Swap the contents of two red-black trees.

This function defines a safe transfer of contents between two trees.

@param[in,out] tree1 Points to the first #TRBTree which is being swapped
with another.

@param[in,out] tree2 Points to the second #TRBTree which is being swapped
with another.
*/
void rbtree_swap (TRBTree* tree1,
                  TRBTree* tree2);


/**
@brief Find a node that matches a specified search key.

@param[in] tree Points to the #TRBTree which is being searched.

@param[in] key Points to the data that specifies a search key and possible additional
context data required to compare a node value against the search key.
  
@param[in] keyCompare Points to a function that compares a tree node against a
specified search key.
  
@param[out] foundNode Points to the topmost node with a key that is closest to the
specified search key.

@return Zero if @a foundNode has a key equal to the specified search key,
nonzero if @a foundNode has a key different from the specified search key.

@note
When the return value is nonzero, both @a foundNode and the return value can be
specified as arguments to the rbtree_insert function.
@n The @a foundNode output is specified to #rbtree_insert as the @a insertAtNode argument.
The return value is specified to #rbtree_insert as the @a insertSide argument.
*/
TRBTreeFindResult rbtree_find (const TRBTree* tree,
                               const void* key,
                               TRBTreeKeyCompare* keyCompare,
                               TRBTreeNode** foundNode);


/**
@brief Find the leftmost node that matches a specified search key.

@param[in] tree Points to the #TRBTree which is being searched.

@param[in] key Points to the data that specifies a search key and possible additional
context data required to compare a node value against the search key.
  
@param[in] keyCompare Points to a function that compares a tree node against a
specified search key.
  
@param[out] foundNode Points to the leftmost node with a key that is closest to the
specified search key.

@return Zero if @a foundNode has a key equal to the specified search key,
nonzero if @a foundNode has a key different from the specified search key.

@note
When the return value is nonzero, both @a foundNode and the return value can be
specified as arguments to the rbtree_insert function.
@n The @a foundNode output is specified to #rbtree_insert as the @a insertAtNode argument.
The return value is specified to #rbtree_insert as the @a insertSide argument.
*/
TRBTreeFindResult rbtree_findFirst (const TRBTree* tree,
                                    const void* key,
                                    TRBTreeKeyCompare* keyCompare,
                                    TRBTreeNode** foundNode);


/**
@brief Find the rightmost node that matches a specified search key.

@param[in] tree Points to the #TRBTree which is being searched.

@param[in] key Points to the data that specifies a search key and possible additional
context data required to compare a node value against the search key.
  
@param[in] keyCompare Points to a function that compares a tree node against a
specified search key.
  
@param[out] foundNode Points to the rightmost node with a key that is closest to the
specified search key.

@return Zero if @a foundNode has a key equal to the specified search key,
nonzero if @a foundNode has a key different from the specified search key.

@note
When the return value is nonzero, both @a foundNode and the return value can be
specified as arguments to the rbtree_insert function.
@n The @a foundNode output is specified to #rbtree_insert as the @a insertAtNode argument.
The return value is specified to #rbtree_insert as the @a insertSide argument.
*/
TRBTreeFindResult rbtree_findLast (const TRBTree* tree,
                                   const void* key,
                                   TRBTreeKeyCompare* keyCompare,
                                   TRBTreeNode** foundNode);


/**
@brief Find a half-open range of nodes that match a specified search key.

@note
This function was provided in the case that the user chooses to store multiple
nodes with the same key in the tree.

@param[in] tree Points to the #TRBTree which is being searched.

@param[in] key Points to the data that specifies a search key and possible additional
context data required to compare a node value against the search key.
  
@param[in] keyCompare Points to a function that compares a tree node against a
specified search key.
  
@param[out] lowerBound Points to the leftmost node in the tree with a key that matches
to the specified search key.

@param[out] upperBound Points to the node that follows the rightmost node in the tree
with a key that matches the specified search key.

@note
The user can iterate through the nodes in the range as follows:
@code
  currentNode = lowerBound;
  while (currentNode != upperBound) {
    ...
    currentNode = rbtree_nextNode (currentNode);
  }
@endcode
*/
void rbtree_findRange (const TRBTree* tree,
                       const void* key,
                       TRBTreeKeyCompare* keyCompare,
                       TRBTreeNode** lowerBound,
                       TRBTreeNode** upperBound);


/**
@brief Insert a new node into a red-black tree.

@param[in] tree Points to the #TRBTree which a node is being inserted into.

@param[in] insertNode Points to the new node to be inserted.
Its data should be initialized.

@param[in] insertAtNode Points to the node where the new node should be inserted nearby.

@param[in] insertSide Specifies which side of the @a foundNode to insert the new node.
Specify negative to insert on the left, and positive to insert on the right.

@note
The @a insertAtNode argument should be the foundNode output from the #rbtree_find function.
@n The @a insertSide argument should be the return value from the #rbtree_find function.

@note
The tree node links of the inserted node are set by the #rbtree_insert function.
*/
void rbtree_insert (TRBTree* tree,
                    TRBTreeNode* insertNode,
                    TRBTreeNode* insertAtNode,
                    TRBTreeInsertSide insertSide);


/**
@brief Remove the specified node from the red-black tree.

@param[in] tree Points to the #TRBTree which a node is being inserted into.

@param[in] removeNode Points to node to be removed.

@note
The caller must explicitly destroy the node after it is removed.
*/
void rbtree_remove (TRBTree* tree,
                    TRBTreeNode* removeNode);


/**
@brief Determine if a red-black tree is empty.

@param[in] tree Points to the #TRBTree which is being checked.

@return TRUE if the tree is empty, FALSE if the tree is not empty.

@note
This function is implemented as a macro.
*/
BOOL8 rbtree_isEmpty (const TRBTree* tree);

#define rbtree_isEmpty(tree) ((tree)->root == NULL)


/**
@brief Find the leftmost node in the tree.

@param[in] tree Points to the #TRBTree which is being searched.

@return A pointer to the leftmost node in the tree.

@note
The user can iterate through the nodes of the tree as follows:
@code
  beginNode = rbtree_begin (tree);
  endNode   = rbtree_end   (tree);
  
  currentNode = beginNode;
  while (currentNode != endNode) {
    ...
    currentNode = rbtree_nextNode (currentNode);
  }
@endcode
*/
TRBTreeNode* rbtree_begin (const TRBTree* tree);


/**
@brief Find the node that follows the rightmost node in the tree.

@param[in] tree Points to the #TRBTree which is being searched.

@return A pointer to the node that follows the rightmost node in the tree.

@note
The user can iterate through the nodes of the tree as follows:
@code
  beginNode = rbtree_begin (tree);
  endNode   = rbtree_end   (tree);
  
  currentNode = beginNode;
  while (currentNode != endNode) {
    ...
    currentNode = rbtree_nextNode (currentNode);
  }
@endcode

@note
This function is implemented as a macro.
*/
TRBTreeNode* rbtree_end (const TRBTree* tree);

#define rbtree_end(tree) (NULL)


/**
@brief Find the node that follows the specified node in the tree.

@param[in] currentNode Points to a #TRBTreeNode which is the current node
of a range being iterated over.

@return The successor node of the currentNode.

@note
The behaviour is undefined if this function is called with @a currentNode
equivalent to either the @a upperBound returned from #rbtree_findRange
or the node returned from #rbtree_end.

@note
The user can iterate through the nodes of the tree as follows:
@code
  beginNode = rbtree_begin (tree);
  endNode   = rbtree_end   (tree);
  
  currentNode = beginNode;
  while (currentNode != endNode) {
    ...
    currentNode = rbtree_nextNode (currentNode);
  }
@endcode
*/
TRBTreeNode* rbtree_nextNode (const TRBTreeNode* currentNode);


/**
@brief Find the leftmost node in the tree.

@param[in] tree Points to the #TRBTree which is being searched.

@return A pointer to the leftmost node in the tree,
or NULL if the tree is empty.

@note
This function is implemented as a macro.
*/
TRBTreeNode* rbtree_first (const TRBTree* tree);

#define rbtree_first(tree) (rbtree_begin(tree))


/**
@brief Find the rightmost node in the tree.

@param[in] tree Points to the #TRBTree which is being searched.

@return A pointer to the rightmost node in the tree,
or NULL if the tree is empty.
*/
TRBTreeNode* rbtree_last (const TRBTree* tree);


/**
@brief Get the root node of the tree.

@param[in] tree Points to the #TRBTree which is being searched.

@return A pointer to the root node of the tree,
or NULL if the tree is empty.

@note
This function is implemented as a macro.
*/
TRBTreeNode* rbtree_root (const TRBTree* tree);

#define rbtree_root(tree) ((tree)->root)


#ifdef __cplusplus
} //extern "C"
#endif

#endif /*INCLUDE_GUARD_containers_gerbtree_h*/
