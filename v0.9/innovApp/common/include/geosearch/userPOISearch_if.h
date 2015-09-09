/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_userPOISearch_if_h
#define INCLUDE_GUARD_userPOISearch_if_h

#include <geosearch/geosearch.h>

//--------------------------------------------------------------------------------
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED

typedef INT32 HRESULT;

#endif

//--------------------------------------------------------------------------------
#ifndef GUID_DEFINED
#define GUID_DEFINED

typedef struct _GUID {
  UINT32 Data1;
  UINT16 Data2;
  UINT16 Data3;
  UINT8  Data4[8];
} GUID;

#endif

//--------------------------------------------------------------------------------
typedef GUID IID;

//--------------------------------------------------------------------------------
#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED

#define REFIID const IID &

#endif

//--------------------------------------------------------------------------------
namespace innov {
namespace geosearch {

//--------------------------------------------------------------------------------
// NOTE: This interface cannot inherit from IObject or use the same smart
// pointers as the app does.  These things can't really happen until the
// app and innovEngine sort out a suitable build and inheritance
// architecture. (And this isn't going to happen for Tui.)

///-----------------------------------------------------------------------------
/// @brief Callback interface for integrating external POI data into a
/// innovEngine geosearch
///
/// The set of data access functions expected by innovEngine to access external
/// (non-DMO) POI data.  Application code may register an object
/// implementing this interface with innovEngine geosearch.  If this is done,
/// innovEngine will call these functions at appropriate times to integrate this
/// data into POI geosearches.
///
/// Notes: =====
/// 1. Mapcore blocks on these function calls - they are not chunked like the
///    rest of geosearch or the DAL layer used to access DMO produced APIs.
///
/// 2. Context is handled in this API with a void* context handle.  This
///    will be application supplied via the search parameters.  It is passed
///    to all the "getIterator" functions and could be used in many ways.  e.g.:
///      - To know which types should be returned (e.g. NavPix only)
///      - To store information about the search that will help with result
///        display
///      - To link a result to a search
///
/// 3. Mapcore regards all data accessed through this interface as part of
///    a "userdata" POI category.  Thus, searches can be restricted to this
///    DAL by specifying the "userdata" category id as the category in
///    TGeocodePOISearchParameters when initiating the search
///
/// 4. Results returned by innovEngine geosearch that were retrieve from this
///    interface will have distinct types GEOSEARCH_RESULT_TYPE_USER_POI_TYPE
///    and GEOSEARCH_RESULT_TYPE_USER_POI.
///      - the name will be the name supplied to innovEngine by resultGetName()
///         - the same name that will be used for sorting
///      - the distance will be valid for "nearest" searches
///    <b>IMPORTANT:</b> None of the remaining result-data access APIs
///    will work on these type - e.g. locating the result or getting the
///    simple name, formatted address, highlight range, etc.  The
///    application is expected to provide these smarts.  What will be
///    provided is
///      UINTP geosearchResult_getUserPOI( TGeosearchResultHandle resultHandle );
///    which will allow the app access to it's result class for the result
///
/// 5. Localisation and canonical names are not handled in this API.  The
///    application should know what locale is currently in use and should
///    use that knowledge to respond correctly to these API calls.
///
/// 6. Mapcore will call release on TIterators and on TResults that will
///    not be returned in the final result list.  It is the responsibility
///    of the application to clean up TResults corresponding to results in
///    the result list.  TResults and TIterators not released by innovEngine
///    must remain valid for the duration of the geosearch.
///
/// 7. This API is deliberately flat (rather than defining additional
///    interfaces for Types and Results) - nasty, but simple
///
class IUserPOISearch
{
public:
  //----------------------------------
  // *** Common interface methods ***
  //----------------------------------

  virtual HRESULT
  QueryInterface( REFIID iid,
                  void** object ) = 0;

  virtual UINT32
  AddRef() = 0;

  virtual UINT32
  Release() = 0;

public:
  //----------------------------
  // *** Interface typedefs ***
  //----------------------------

  // An iterator type
  typedef const void* TIterator;

  // A result type
  typedef const void* TResult;

public:
  //------------------------------------
  // *** Iterator factory functions ***
  //------------------------------------

  //--------------------
  // Search for POI types

  /// Get a POI type iterator
  ///
  /// @param[in]  context            Search context handle (from search parameters)
  /// @param[in]  searchBounds       An optional search bounds (lat/long box) --
  ///                                If not NULL return only results in these bounds
  /// @param[in]  searchString       Return only results that match this
  /// @param[in]  findPOIGroupByType If FALSE, search for POI types directly.
  ///                                If TRUE, search for POI group by POI type.
  /// @return
  ///   - An iterator pointing the first POI type
  ///   - NULL if there are no POI type available via this DAL
  TIterator
  getPOITypeIterator( void*        context,
                      const TRect* searchBounds,
                      const UTF16* searchString,
                      BOOL8        findPOIGroupByType );

  //--------------------
  // Search for individual POIs - by name or by location

  /// Get an iterator for a set of individual POI results
  ///
  /// @param[in]  context      Search context handle (from search parameters)
  /// @param[in]  POITypes     An optional array of POI type identifies to filter
  ///                          the POI search.
  /// @param[in]  POITypeCount The number of elements in the POI type array.
  ///                          Specify zero for an unfiltered search.
  /// @param[in]  searchBounds An optional search bounds (lat/long box) --
  ///                          If not NULL return only results in these bounds
  /// @param[in]  searchString Return only results that match this
  /// @param[out] sortedByName Indicates whether or not the application is able
  ///                          to provide a sorted iterator.  This may allow
  ///                          innovEngine to stop iterating early because it can
  ///                          "know" that the results retrieved so far are the
  ///                          "first" results.
  /// @return
  ///   - An iterator pointing the first result
  ///   - NULL if there are no matching results
  ///
  /// @note The @a sortedByName optimisation may be necessary for unbounded
  /// searches that could return a large number of results.  e.g.:
  ///  - Searching for matches to the empty string "" or to a single letter
  ///  - Searching very large data sets
  TIterator
  getPOIIterator( void*                      context,
                  const TGeosearchPOITypeId* POITypes,
                  UINT32                     POITypeCount,
                  const TRect*               searchBounds,
                  const UTF16*               searchString,
                  BOOL8*                     sortedByName );

public:
  //-----------------------------------------
  // *** Iterator manipulation functions ***
  //-----------------------------------------

  //--------------------
  /// Retrieve the result currently pointed to by the iterator
  /// This should return a "copy" of the result
  TResult
  iteratorGetResult( TIterator it );

  //--------------------
  /// Get the next iterator given a current iterator
  ///
  /// Mapcore will never "dereference" the old value of an iterator after
  /// incrementing it
  ///
  /// The TIterator returned may be a new object, or the original object
  /// passed back out with a state change.
  ///
  /// An old iterator object must be destroyed by this function.
  ///
  /// @param[in] it          An iterator returned from one of the "getFirst" methods
  /// @return
  ///   - A handle to the next object
  ///   - NULL if there are no further objects to return
  TIterator
  iteratorNext( TIterator it );

  //--------------------
  /// Method via which innovEngine can indicate that it is no longer interested
  /// in the iterator
  void
  iteratorDestroy( TIterator it );

public:
  //---------------------------------------
  // *** Result manipulation functions ***
  //---------------------------------------

  //--------------------
  /// Add to the reference count of the result object.
  ///
  /// This may be a no-op if the object will have a lifetime at least as long
  /// as the encompassing geosearch call
  UINT32
  resultAddRef( TResult result );

  //--------------------
  /// Decrement the reference of the result object
  ///
  /// Can be used in conjunction with resultAddRef() to determine when
  /// innovEngine has lost interest in a result
  UINT32
  resultRelease( TResult result );

  //--------------------
  /// Get the name of a result
  /// Mapcore will sort these by name - names should be in the correct locale
  ///
  /// @param[in]  result     The result object.
  /// @param[out] nameBuffer A pointer to a buffer to copy the name string into.
  /// @param[in]  maxLength  The maximum length of the output buffer.
  void
  resultGetName( TResult result,
                 UTF16*  nameBuffer,
                 UINT32  maxLength );

  //--------------------
  /// Get the position of a result (lat/long) - used for "nearest" searching
  ///
  /// Not valid for POI Type results
  TPos
  resultGetPosition( TResult result );

  //--------------------
  /// Get the type id of a result
  TGeosearchPOITypeId
  resultGetTypeId( TResult result );

  //--------------------
  /// Retrieve a type iterator for the types of the specified poi result
  /// This should return a "copy" of the type iterator
  TIterator
  resultGetTypeIterator( TResult result );

  //--------------------
  // === Future functionality ===
  // Retrieve information about the location of the result - e.g. areas,
  // roads with which the result is associated
  // TGeosearchLocation resultGetLocality( TResult result );

private:
  //--------------------------------
  // *** Implementation methods ***
  //--------------------------------

  virtual TIterator
  doGetPOITypeIterator( void*        context,
                        const TRect* searchBounds,
                        const UTF16* searchString,
                        BOOL8        findPOIGroupByType ) = 0;

  virtual TIterator
  doGetPOIIterator( void*                      context,
                    const TGeosearchPOITypeId* POITypes,
                    UINT32                     POITypeCount,
                    const TRect*               searchBounds,
                    const UTF16*               searchString,
                    BOOL8&                     sortedByName ) = 0;

  virtual TResult
  doIteratorGetResult( TIterator it ) = 0;

  virtual TIterator
  doIteratorNext( TIterator it ) = 0;

  virtual void
  doIteratorDestroy( TIterator it ) = 0;

  virtual UINT32
  doResultAddRef( TResult result ) = 0;

  virtual UINT32
  doResultRelease( TResult result ) = 0;

  virtual void
  doResultGetName( TResult result,
                   UTF16   nameBuffer[],
                   UINT32  maxLength ) = 0;

  virtual TPos
  doResultGetPosition( TResult result ) = 0;

  virtual TGeosearchPOITypeId
  doResultGetTypeId( TResult result ) = 0;

  virtual TIterator
  doResultGetTypeIterator( TResult result ) = 0;
};

//--------------------------------------------------------------------------------
/// Set the global user POI search object to be used for POI searching.
///
/// @param[in] userPOISearch A pointer to an object that implements the
///                          user POI search interface.
///                          Specify NULL to detach any previous stored
///                          interface.
void setUserPOISearch( IUserPOISearch* userPOISearch );

//--------------------------------------------------------------------------------
/// Get the global user POI search object to be used for POI searching.
///
/// @param[out] userPOISearch A pointer to the current configured object that
///                           implements the user POI search interface,
///                           or NULL if there is no current configured object.
void getUserPOISearch( IUserPOISearch** userPOISearch );

//--------------------------------------------------------------------------------
} //namespace geosearch
} //namespace innov



#endif //INCLUDE_GUARD_userPOISearch_if_h
