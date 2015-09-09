#ifndef GEBSRCH_H
#define GEBSRCH_H

/* Binary search routines that allow ordered lists of objects to be
   rapidly searched for an object corresponding to a key, or allow objects to
   be sorted into an ordered list */

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Containers
/// @ingroup Utilities
/// @{

/// @addtogroup BinarySearch
/// @ingroup Containers
/// Binary search objects that allow ordered lists of objects to be
/// rapidly searched for an object corresponding to a key, or allow 
/// objects to be sorted into an ordered list.
/// @{

/// @typedef TBSearchCompareFunction
/// The form of a user supplied compare function that
///
/// @param key Points to the key to compare the item against.
///
/// @param index The index of the item to compare the key value against.
///
/// @return
/// - < 0 if key is smaller than item corresponding to index,
/// - 0 if key is equal to the item corresponding to index or
/// - > 0 if key is greater then item corresponding to index.
typedef INT32 TBSearchCompareFunction( const void *key,
                                       UINT32 index );

/// @typedef TBSearchSortSourceFunction
/// The form of a user supplied object source function.  If the first 
/// byte of the object returned is zero then the object is considered 
/// invalid and is not inserted into the sorted array.
///
/// @param output A pointer to an area of memory into which the source 
/// function should copy any objects that it returns.  The source function 
/// must maintain its own global variables to keep track of the objects it 
/// is returning.
///
/// @return If the function returns TRUE then the source function may 
/// return more objects.  If the function returns FALSE then no valid 
/// object was returned and no further objects are available.
typedef BOOL8 TBSearchSortSourceFunction( void *output,
                                          void *dataStart,
                                          UINT32 objectCount,
                                          UINT32 *outputCurrentCount,
                                          BOOL8 lastWasDuplicate );

/// @typedef TBSearchOverflowFunction
/// The form of a user supplied overflow function.  This function is 
/// called by the sorting algorithm when inserting an object into the list 
/// causes the object at the end of the list to be lost.
///
/// @param output A pointer to the object that becomes the last in the 
/// list.
typedef void TBSearchOverflowFunction(void *output);

/// @typedef TBSearchInsertNotifyFunction
/// The form of a user supplied overflow function.  This function is 
/// called by the sorting algorithm when a unique item is found.
/// The function is called with the insert position specified.
///
///
/// @param insertIndex The index where the item would be inserted.
/// This insert position may occur after the end of the list.
typedef void TBSearchInsertNotifyFunction(UINT32 insertIndex,
                                          void* output);

/// @def bsearch_calcInsertIndex( returnedIndex )
/// Negative values returned by bsearch_find can be used to determine 
/// the index before which a new object should be inserted.  
/// bsearch_calcInsertIndex can be used to convert the return value into 
/// an index value.
///
/// @param returnedIndex A negative index value returned by bsearch_find.
///
/// @return A positive index value before which the item searched for should
/// be inserted.
#define /* INT32 */ bsearch_calcInsertIndex(/* INT32 */ returnedIndex) (-(returnedIndex) - 1)

/// Searches an array of organised objects for the object corresponding to
/// the key value.
///
/// @par
/// AT PRESENT THE FUNCTION DOES NOT ACCEPT A POINTER TO THE BASE ARRAY AS 
/// PART OF ITS ARGUMENTS AND THUS THE USER SUPPLIED COMPARE FUNCTION MUST 
/// READ A GLOBAL VARIABLE TO DETERMINE THE BASE ARRAY LOCATION.
///
/// @param key The key value to find within the ordered set of objects.
///
/// @param compareFunction A user supplied compare function of the form
/// TBsearchCompareFunction used to compare key values against set object
/// values.
///
/// @param objectCount The number of objects available in the ordered set.
///
/// @return
/// - >= 0 if the object was located.  This value represents the index of an
/// exact match that was found.  Note that if the key is not unique then 
/// there could be objects before this one or after this one that also 
/// match the key.
/// - < 0 if an exact match is NOT FOUND.  This negative value can be used 
/// to determine the index before which a new object corresponding to the 
/// key value should be inserted.  bsearch_calcInsertIndex() can be used to 
/// convert this return value into an index value.  In this way an ordered 
/// list can be built by first searching for objects and then inserting 
/// them at the appropriate place if they do not already exist.
/// - < -((INT32)(objectCount) + 1) implies that no compare function was
/// supplied and therefore the object could not be found within the list.
extern MAPCORE_API INT32 bsearch_find( const void *key,
                                       TBSearchCompareFunction *compareFunction,
                                       UINT32 objectCount );

//////////////////////////////////////////////////////////////////////////
/// Searches for a given object in a *sorted* list of objects. If a range of objects
/// match the given key then either returns the *first* or the *last* item this range.
///
/// @param        keyContext[in] This is an arbitary structure containing the search key 
//                               and any other context data (mapheader, array base etc) 
///                              required to perform a comparison between two given indexes. 
/// 
/// @param   compareFunction[in] a user defined compare function which is used to compare the 
///                              supplied key to a value at a given index.
///
/// @param          getFirst[in] TRUE  if we are searching for the start of the range. 
///                              FALSE if we are searching for the end of the range.
///
/// @param        startIndex[in] The start index of the subrange we want to search
/// 
/// @param          endIndex[in] The end index of the subrange we want to search
/// 
/// @returns index in the array if an object was found. -1 otherwise.
extern MAPCORE_API INT32 bsearch_find_range_boundary( const void               *keyContext, 
                                                      TBSearchCompareFunction  *compareFunction,
                                                      BOOL8                    getFirst,
                                                      UINT32                   startIndex, 
                                                      UINT32                   endIndex );

//////////////////////////////////////////////////////////////////////////
/// Searches an *ordered* list of objects and returns the range of objects that match 
/// a given key. 
///
/// @param        keyContext[in] This is an arbitary structure containing the search key 
//                               and any other context data (mapheader, array base etc) 
///                              required to perform a comparison between two given indexes. 
/// 
/// @param   compareFunction[in] a user defined compare function which is used to compare the 
///                              supplied key to a value at a given index.
/// 
/// @param        startIndex[in] The start index of the subrange we want to search
/// 
/// @param          endIndex[in] The end index of the subrange we want to search
///
/// @param       rangeStart[out] Start of the output range
///
/// @param         rangeEnd[out] End of the output range
/// 
/// @returns TRUE if a matching range was found, FALSE otherwise
extern MAPCORE_API BOOL8 bsearch_find_range( const void *keyContext, 
                                             TBSearchCompareFunction *compareFunction, 
                                             UINT32 startIndex,
                                             UINT32 endIndex,
                                             UINT32 *rangeStart, 
                                             UINT32 *rangeEnd );

//@MRC Start
//////////////////////////////////////////////////////////////////////////
/// Searches an *ordered* list of objects and returns the change position of objects that match 
/// a given key. 
///
/// @param        keyContext[in] This is an arbitary structure containing the search key 
//                               and any other context data (mapheader, array base etc) 
///                              required to perform a comparison between two given indexes. 
/// 
/// @param   compareFunction[in] a user defined compare function which is used to compare the 
///                              supplied key to a value at a given index.
/// 
/// @param        startIndex[in] The start index of the subrange we want to search
/// 
/// @param          endIndex[in] The end index of the subrange we want to search
///
/// @returns >= 0 if a matching change position was found, -1 otherwise
extern MAPCORE_API INT32 bsearch_find_change( const void *keyContext, 
											  TBSearchCompareFunction *compareFunction, 
											  UINT32 startIndex,
											  UINT32 endIndex );
//@MRC End

/// Sorts objects into order as they are supplied by a source function
/// and uses the binary search function to insertion sort a number of
/// objects into the output array.  if the first byte of the object 
/// returned by the source function is zero then the object is considered 
/// invalid and is not inserted into the sorted array.
///
/// @param output Points to a location into which objects from the source 
/// function are sorted.
///
/// @param outputMaxCount The maximum number of objects that can be placed 
/// in the output array.  This space should be of size outputMaxCount * 
/// objectSize.
///
/// @param outputCurrentCount This is the number of objects that have 
/// already been sorted into the output array.
///
/// @param objectSourceFunction Points to a function that returns objects 
/// to be sorted.
///
/// @param objectCompareFunction This user supplied function compares an 
/// object supplied by the object source function with the objects that 
/// have already been sorted to determine where and whether the current 
/// object will be inserted into the output array.
///
/// @param objectOverflowFunction This function is called when the 
/// insertion of an object forces an object to drop of the end of the 
/// array.  Once overflow starts to occur the source function can alter 
/// the range of objects that it supplies to the sorting function.  e.g. if 
/// the overflow value is a string "TREE" then all strings such as "ROSE" 
/// that are 'greater' than "TREE" are no longer supplied by the source 
/// function.
///
/// @param workspaceObject The sorting function requires a workspace of 
/// size objectSize bytes in which to store temporary objects.
/// 
/// @param objectSize This is the size in bytes of the objects being sorted.
///
/// @return an updated value of outputCurrentCount that will be <= 
/// outputMaxCount. */
extern MAPCORE_API UINT32 bsearch_insertionSort( void *output,
                                                 UINT32 outputMaxCount,
                                                 UINT32 outputCurrentCount,
                                                 TBSearchSortSourceFunction *objectSourceFunction,
                                                 TBSearchCompareFunction *objectCompareFunction,
                                                 TBSearchOverflowFunction *objectOverflowFunction,
                                                 TBSearchInsertNotifyFunction* insertNotifyFunction,
                                                 void *workspaceObject,
                                                 UINT32 objectSize );

/// @}
/// @}

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------*/

#endif
