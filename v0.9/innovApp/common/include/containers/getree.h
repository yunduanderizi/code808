#ifndef GETREE_H
#define GETREE_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Containers
/// @ingroup Utilities
/// @{

/// @addtogroup Tree
/// Binary tree routines that allow trees of objects (nodes) to be 
/// created that have a UNIQUE 32 bit key.  A tree of order log2(n) 
/// deep is required to store n randomly distributed objects.  The 
/// tree is not a balanced tree and therefore will perform badly 
/// if the objects encountered contain ordered sequences.
/// GEBTREE.C is a generalised version of the same code that is 
/// used for keys of any size.
/// @ingroup Containers
/// @{

/// @typedef struct TTreeNode
/// Individual nodes in the tree have this format.  Actually the user 
/// specifies their own structure which contains similar fields at the 
/// beginning.  The user structure which may contain a 32 BIT KEY is 
/// cast to a TTree node whenever it is passed to any tree_ functions.
typedef struct TTreeNode {
  /// points to the node on the left branch of this node (NULL if none).
  struct TTreeNode *left;
  /// points to the node on the right branch of this node (NULL if none).
  struct TTreeNode *right;
  /// points to the parent node of this node (NULL if none).
  struct TTreeNode *parent;
  /// Fixed sized UNIQUE 32 bit key for this node.
  UINT32 key;
} TTreeNode;

/// @typedef struct TTree
/// This structure contains working data required to maintain a binary 
/// tree.  The structure can be initialised by calling tree_init.  The 
/// only user accessible portion is 'root'.
typedef struct TTree {
    struct TTreeNode *root;
    /// Points to the root node of the tree (NULL if the tree is empty).
    struct TTreeNode *lastParent;
    UINT8 keySize;
} TTree;

/// This is the type of function that is used by tree_scan to scan 
/// objects in the tree in increasing key order.
///
/// @param node A pointer to successively increasing node values that are
/// presented to the user supplied scanning function.  The scanning function
/// must know the actual form of the node being scanned.
///
/// @return btree_scan will only scan more values if the return value from 
/// this function is TRUE.  A return value of FALSE halts further
/// scanning.
typedef BOOL8 TTreeScanFunc( TTreeNode *node );

/// Add a new node to the tree immediately after using tree_find to find 
/// that key value in the tree.
///
/// @par
/// IF tree_find IS NOT CALLED IMMEDIATELY BEFORE THIS FUNCTION THEN IT WILL
/// NOT OPERATE CORRECTLY.  NODES MUST HAVE UNIQUE KEY VALUES.
///
/// @param tree Points to the TTree to which the node is being added.
///
/// @param newNode Points to the node that is being added into the tree.  
/// The function assumes that newNode already contains the key value.
extern MAPCORE_API void tree_add( TTree *tree,
                                  TTreeNode *newNode );

/// Delete a node in the tree.
///
/// @param tree Points to the TTree from which the node is being deleted
///
/// @param deleteNode Points to the node to delete.  The node must 
/// currently be contained in the tree.
///
/// @return FALSE if the node could not be deleted for some reason.
extern MAPCORE_API void tree_delete( TTree *tree,
                                     TTreeNode *deleteNode );

/// Find a node corresponding to a particular key.
///
/// @par
/// THIS FUNCTION MUST BE CALLED BEFORE ADDING A NEW NODE TO THE TREE.
///
/// @param tree Points to the tree to be searched.
///
/// @param key Pointer to a location containing the 32 bit key.
///
/// @return If the node was found then the function returns a pointer 
/// to it, otherwise the return value is NULL.
extern MAPCORE_API TTreeNode *tree_find( TTree *tree,
                                         void *key );

/// Create an empty tree.
///
/// @param tree Points to the tree to be initialised.
///
/// @param keySize Is the size of an array of bytes that is treated as a 
/// unique key to tree nodes.
extern MAPCORE_API void tree_init( TTree *tree,
                                   UINT8 keySize );

/// Traverse the nodes in a tree in ascending key order calling
/// a user supplied function as each successive node is encountered.
///
/// @param tree Points to the tree to be scanned.
///
/// @param scanFunc Points to a function of the type TTreeScanFunc that is
/// presented with nodes in ascending order.  If the user function returns 
/// FALSE then no further objects are scanned.
extern MAPCORE_API void tree_scan( TTree *tree,
                                   TTreeScanFunc *scanFunc );

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif
