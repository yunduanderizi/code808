#ifndef GEBTREE_H
#define GEBTREE_H

// binary tree routines that allow trees of objects (nodes) to be created that
// have a UNIQUE key of arbitrary size.  A tree of order log2(n) deep is required to
// store n randomly distributed objects.  The tree is not a balanced tree and
// therefore will perform badly if the objects encountered contain ordered
// sequences.
// GETREE.C is a specialised version of the same code that is optimised for
// 32 bit integer keys.

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Containers
/// @ingroup Utilities
/// @{

/// @addtogroup BinaryTree
/// Binary tree routines that allow trees of objects (nodes) to be 
/// created that have a UNIQUE key of arbitrary size.  A tree of 
/// order log2(n) deep is required to store n randomly distributed 
/// objects.  The tree is not a balanced tree and therefore will 
/// perform badly if the objects encountered contain ordered 
/// sequences.  GETREE.C is a specialised version of the same code 
/// that is optimised for 32 bit integer keys.
/// @ingroup Containers
/// @{

/// Individual nodes in the tree have this format.  Actually the user 
/// specifies their own structure which contains similar fields at the 
/// beginning.  The user structure which may contain a key of arbitrary 
/// length is cast to a TBTree node whenever it is passed to any btree_ 
/// functions. */
typedef struct TBTreeNode {
  /// points to the node on the left branch of this node (NULL if none).
  struct TBTreeNode *left;
  /// points to the node on the right branch of this node (NULL if none).
  struct TBTreeNode *right;
  /// points to the parent node of this node (NULL if none).
  struct TBTreeNode *parent;
  /// the key field can actually be any number of bytes corresponding to
  /// the keySize parameter of TBTree.
  UINT8 key;
} TBTreeNode;

/// This structure contains working data required to maintain a binary 
/// tree.  The structure can be initialised by calling btree_init.  The 
/// only user accessible element is 'root'.
typedef struct TBTree {
    /// Points to the root node of the tree (NULL if the tree is empty)
    struct TBTreeNode *root;
    struct TBTreeNode *lastParent;
    UINT8 keySize;
} TBTree;

/// This is the type of function that is used by btree_scan to scan objects
/// in the tree in successive order (i.e. memcmp order).

/// @param node A pointer to successively increasing node values that are
/// presented to the user supplied scanning function.  The scanning function
/// must know the actual form of the node being scanned.
///   
/// @return btree_scan will only scan more values if the return value from 
/// this function is TRUE.  A return value of FALSE halts further 
/// scanning.
typedef BOOL8 TBTreeScanFunc( TBTreeNode *node );

/// Add a new node to the tree immediately after using btree_find to find 
/// that key value in the tree.
///
/// @par
/// IF btree_find IS NOT CALLED IMMEDIATELY BEFORE THIS FUNCTION THEN IT 
/// WILL NOT OPERATE CORRECTLY.  NODES MUST HAVE UNIQUE KEY VALUES.
///
/// @param tree Points to the TBtree to which the node is being added.
///
/// @param newNode Points to the node that is being added into the tree.  
/// The function assumes that newNode already contains the key value.
extern MAPCORE_API void btree_add( TBTree *tree,
                                   TBTreeNode *newNode );

/// Delete a node in the tree.
///
/// @param tree Points to the TBtree from which the node is being deleted
///
/// @param deleteNode Points to the node to delete.  The node must 
/// currently be contained in the tree.
///   
/// @return FALSE if the node could not be deleted for some reason.
extern MAPCORE_API void btree_delete( TBTree *tree,
                                      TBTreeNode *deleteNode );

/// Find a node corresponding to a particular key.
///
/// @par
/// THIS FUNCTION MUST BE CALLED BEFORE ADDING A NEW NODE TO THE TREE.
///
/// @param tree Points to the tree to be searched.
///
/// @param key Pointer to a location containing the key (where the size of 
/// the key is assumed to be consistent with TBTree.keySize).
/// 
/// @return If the node was found then the function returns a pointer to 
/// it, otherwise the return value is NULL.
extern MAPCORE_API TBTreeNode *btree_find( TBTree *tree,
                                           void *key );

/// Create an empty tree.
/// 
/// @param tree Points to the tree to be initialised.
/// 
/// @param keySize Is the size of an array of bytes that is treated as a 
/// unique key to tree nodes.
extern MAPCORE_API void btree_init( TBTree *tree,
                                    UINT8 keySize );

/// Traverse the nodes in a tree in ascending key order calling
/// a user supplied function as each successive node is encountered.
///
/// @param tree Points to the tree to be scanned.
/// 
/// @param scanFunc Points to a function of the type TBTreeScanFunc that is
/// presented with nodes in ascending order.  If the user function returns 
/// FALSE then no further objects are scanned.
extern MAPCORE_API void btree_scan( TBTree *tree,
                                    TBTreeScanFunc *scanFunc );
  
/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif
