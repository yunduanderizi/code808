/*****************************************************************************
 *  (c) 2014 innov China limited. all rights reserved.
 *
 *  module name: gelist.c
 *
 *  author(s): matt broadbent <mbroadbent@21com.com>
 *
 *  description:
 *    this module contains functions for implementation of simple linked
 *    lists
 *****************************************************************************/

#ifndef GELIST_H
#define GELIST_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Containers
/// @ingroup Utilities
/// @{

/// @addtogroup List
/// Provides functions to create and maintain linked lists.  Objects
/// maintained in lists are cast to the type TListObject must have 
/// first elements similar to those in the TListObject structure.
/// @ingroup Containers
/// @{

/// Although objects of any size can be maintained in the list, they are
/// cast to the type TListObject when passed to list_ functions.  The user
/// structure must have first elements similar to those in the TListObject
/// structure.
typedef struct TListObject {
  /// Points to the next object in the list (or NULL if there is no next object).
  struct TListObject *next;
} TListObject;

/// Contains working data required to describe the state of a list of 
/// objects.  There are no user accessible portions in this structure.  
/// The list_first and list_last functions should be used to read the 
/// values of the first and last objects.
typedef struct TList {
  /// Points to the first object in the list (or NULL if there is no first object).
  TListObject *first;
  /// Points to the last object in the list (or NULL if there is no last object).
  TListObject *last;
} TList;

/// Add a new object to the end of the list - it then becomes the last 
/// object in the list.  The object to be added must not be already 
/// contained in the list.
///
/// @param list Points to the list to which the object should be added.
///
/// @param object Points to the object to be added to the list.
///
/// @par
/// THE OBJECT TO BE ADDED IS CAST TO THE TYPE TListObject AND MUST
/// HAVE SIMILAR ELEMENTS TO THE TList OBJECT AT THE BEGINNING OF ITS 
/// STRUCTURE.
extern MAPCORE_API void list_add( TList *list,
                                  TListObject *object );

/// Initialise a TList structure.  After it has been initialised, 
/// objects can be added and deleted.
///
/// @param list Points to the list to be initialised.
extern MAPCORE_API void list_create( TList *list );

/// Remove an existing object from the list. The object must already be 
/// present in the list.
///
/// @param list The list from which toi object is to be removed.
///
/// @param object Points to the object being removed from the list.
///
/// @par
/// THE OBJECT TO BE REMOVED IS CAST TO THE TYPE TListObject AND MUST 
/// HAVE SIMILAR ELEMENTS TO THE TList OBJECT AT THE BEGINNING OF ITS 
/// STRUCTURE.
extern MAPCORE_API void list_delete( TList *list,
                                     TListObject *object );

/// Inserts an object in the list.
///
/// @param list This is a pointer to the list in which the object will 
/// be inserted.
///
/// @param insertAfter The object is inserted after the insertAfter object.
/// If insertAfter is NULL then the new object is inserted at the 
/// BEGINNING of the list - it then becomes the first object in the list.
///
/// @param object Points to the object to be inserted.
///
/// @par
/// THE OBJECT TO BE INSERTED IS CAST TO THE TYPE TListObject AND MUST 
/// HAVE SIMILAR ELEMENTS TO THE TList OBJECT AT THE BEGINNING OF ITS 
/// STRUCTURE.
extern MAPCORE_API void list_insert( TList *list,
                                     TListObject *insertAfter,
                                     TListObject *object );

/// Returns the first item in the linked list (or NULL if none exists).
///
/// @param x Points to the list.
///
/// @return a pointer to the first object in the list.
#define list_first(/* TList* */ x) (x)->first

/// Returns the last item in the linked list (or NULL if none exists).
///
/// @param x Points to the list.
///
/// @return a pointer to the last object in the list.
#define list_last(/* TList* */ x) (x)->last

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif /* GELIST_H */
