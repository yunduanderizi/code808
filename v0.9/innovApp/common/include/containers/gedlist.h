#ifndef GEDLIST_H
#define GEDLIST_H

// Provides functions to create and maintain doubly linked lists.   Objects
// maintained in lists are cast to the type TDListObject must have first
// elements similar to those in the TDListObject structure.

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Containers
/// @ingroup Utilities
/// @{

/// @addtogroup DList
/// Provides functions to create and maintain doubly linked lists.  Objects
/// maintained in lists are cast to the type TDListObject must have first
/// elements similar to those in the TDListObject structure.
/// @ingroup Containers
/// @{

/// @typedef struct TDListObject
/// Although objects of any size can be maintained in the list, they are
/// cast to the type TDListObject when passed to dlist_ functions.  The user
/// structure must have first elements similar to those in the TDListObject
/// structure.
typedef struct TDListObject {
    /// Points to the previous object in the list.
    struct TDListObject *previous;
    /// Points to the next object in the list.
    struct TDListObject *next;
} TDListObject;

/// @typedef struct TDList
/// Contains working data required to describe the state of a list of 
/// objects.  There are no user accessible portions in this structure.  
/// The dlist_first and dlist_last functions should be used to read the 
/// values of the first and last objects.
typedef struct TDList {
  TDListObject *first;
  TDListObject *last;
} TDList;

/// Add a new object to the end of the list - it then becomes the last 
/// object in the list.  The object to be added is cast to the type 
/// TDListObject and must have similar elements to the TDList object at 
/// the beginning of its structure.
///
/// @param list Points to the list to which the object is to be added.
///
/// @param object Points to the object to be added which must not be already
/// contained in the list.
extern MAPCORE_API void dlist_add( TDList *list,
                                   TDListObject *object );
  
/// Initialise a TDList structure.  After it has been initialised, objects
/// can be added and deleted.
///
/// @param list Pointer to the list to be initialised.
extern MAPCORE_API void dlist_create( TDList *list );

/// Remove an existing object from the list.  The object to be removed is 
/// cast to the type TDListObject and must have similar elements to the 
/// TDList object at the beginning of its structure.
///
/// @param list Points to the list from which the object is to be deleted.
///
/// @param object Points to the object to be deleted (Which must already 
/// be present in the list).
extern MAPCORE_API void dlist_delete( TDList *list,
                                      TDListObject *object );

/// Inserts an object into the list.
/// 
/// @param list The list into which the object is inserted.
///
/// @param insertAfter The object already in the list after which the new 
/// object is inserted.  If insertAfter is NULL then the new object is 
/// inserted at the beginning of the list - it then becomes the first 
/// object in the list.
/// 
/// @param object Points to the object to be inserted in the list.
extern MAPCORE_API void dlist_insert( TDList *list,
                                      TDListObject *insertAfter,
                                      TDListObject *object );

/// @def dlist_first( x )
/// Get the first item in a list.
///
/// @param x Points to the list.
///
/// @return the first item in the doubly linked list (or NULL if none 
/// exists).
#define /* TDListObject* */ dlist_first(/* TDList* */ x) (x)->first

/// @def dlist_last( x )
/// Get the last item in a list.
///
/// @param x Points to the list.
///
/// @return the last item in the doubly linked list (or NULL if none 
/// exists).
#define /* TDListObject* */ dlist_last(/* TDlist* */ x) (x)->last

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif
