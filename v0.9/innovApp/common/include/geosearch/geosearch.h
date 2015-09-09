/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_geosearch_h
#define INCLUDE_GUARD_geosearch_h

#include <display/dipoitypelist.h>
#include <location/locmaplocation.h>
#include <language/lacodepage.h>
#include <mapdata/enpoi.h>
#include <mapdata/enroad.h>
#include <mapdata/entoken.h>
#include <math/gerect.h>
#include <routing/rtjourney.h>

#if defined(__cplusplus)
extern "C" {
#endif

//--------------------------------------------------------------------------------
/// @brief The maximum length of geosearch name strings.
#define GEOSEARCH_MAX_NAME_SIZE (72)

//--------------------------------------------------------------------------------
/// @internal
///
/// This type is a generic handle type.
///
/// This type is to be declared as a field of union types that
/// represent different types of specific handle types.
///
/// The union type that represents a specific handle type must
/// declare nested instances of this generic handle type with
/// predefined names for each level of the class hierarchy of
/// the objects they represent.
///
/// Each API function for a specific handle type will actually
/// be implemented as a macro that forwards to an equivalently
/// named macro where all specific handle type input arguments
/// are converted to generic handle types.
///
/// The strong typing is enforced by requiring the correctly
/// named generic handle type contained within the specific
/// handle type.
///
typedef struct TGeosearchHandle {
  /// @internal
  /// @brief The current engine initialisation count, which is
  ///        used to prevent use of objects which were allocated
  ///        within a map engine session that is no longer valid.
  INT32  _engineInitCount;

  /// @internal
  /// @brief A pointer to any type of COM interface.
  void*  _object;

#if defined(__cplusplus)
  /// @brief The default constructor.
  TGeosearchHandle()
    : _engineInitCount(0),
      _object(0) {}
#endif

} TGeosearchHandle;

//--------------------------------------------------------------------------------
/// @internal
extern TGeosearchHandle _geosearchNullHandle;

//--------------------------------------------------------------------------------
/// @brief This type is a handle to an object.
///
#if defined(__cplusplus)
struct TGeosearchObjectHandle {
  /// @internal
  TGeosearchHandle  _genericHandle;
};
#else
typedef union TGeosearchObjectHandle {
  /// @internal
  TGeosearchHandle  _genericObjectHandle;

} TGeosearchObjectHandle;
#endif

//--------------------------------------------------------------------------------
/// @brief This type is a handle to an object that represents
///        a string of text, possibly with information about
///        the structure of the text.
///
#if defined(__cplusplus)
struct TGeosearchTextHandle : public TGeosearchObjectHandle {
};
#else
typedef union TGeosearchTextHandle {
  /// @internal
  TGeosearchHandle  _genericObjectHandle;

  /// @internal
  TGeosearchHandle  _genericTextHandle;

} TGeosearchTextHandle;
#endif

//--------------------------------------------------------------------------------
/// @enum TGeosearchTextType
///
typedef enum TGeosearchTextType {

  /// @brief An invalid text type.
  GEOSEARCH_TEXT_TYPE_INVALID,

  /// @brief Indicates that a range of text in a string
  /// represents a strong highlight.
  GEOSEARCH_TEXT_TYPE_STRONG_HIGHLIGHT,

  /// @brief Indicates that a range of text in a string
  /// represents a weak match highlight.
  GEOSEARCH_TEXT_TYPE_WEAK_HIGHLIGHT,

  /// @brief Indicates that a range of text in a string
  /// represents a simple name.
  GEOSEARCH_TEXT_TYPE_SIMPLE_NAME,

  /// @brief Indicates that a range of text in a string
  /// represents a complex name.
  ///
  /// Such a name consists of sub-ranges that make up the
  /// structure of the name.
  GEOSEARCH_TEXT_TYPE_COMPLEX_NAME,

  /// @brief Indicates that a range of text in a string
  /// represents a list of names.
  ///
  /// Such a name consists of sub-ranges that represent the
  /// elements of the name list.
  ///
  /// The punctuation that separates elements of the name list
  /// is contained within this range, not the sub-ranges.
  ///
  /// This range type is used to list alternate place names
  /// in a place specifier, for example, "PLACE1/PLACE2".
  GEOSEARCH_TEXT_TYPE_NAME_LIST,

  /// @brief Indicates that a range of text in a string
  /// represents a place adornment, including the punctuation
  /// that surrounds and separates the place adornment from
  /// its base place name.
  ///
  /// The base place name is represented by a range preceeding
  /// this range at the same nesting level.
  ///
  /// A single sub-range must be contained within this range
  /// which represents the place adornment name.
  ///
  /// The place adornment format string is effectively " (%s)"
  /// where %s represents the sub-range.
  GEOSEARCH_TEXT_TYPE_PLACE_ADORNMENT,

  /// @brief Indicates that a range of text in a string
  /// represents a place specifier, including the punctuation
  /// that surrounds and separates the place separator from
  /// its base name.
  ///
  /// The base name is represented by a range preceeding this
  /// range at the same nesting level.
  ///
  /// A single sub-range must be contained within this range
  /// which represents the place specifier name, which may be
  /// a complex name.
  ///
  /// The place specifier format string is effectively ", %s"
  /// where %s represents the sub-range.
  GEOSEARCH_TEXT_TYPE_PLACE_SPECIFIER,

  /// @brief Indicates that a range of text in a string
  /// represents an address located at a house number or
  /// house number range on a road.
  ///
  /// Exactly two sub-ranges must be contained within this
  /// range, one being the house number or house number range,
  /// the other being the road name.
  ///
  /// It is unspecified which of the two sub-ranges represents
  /// which of the two sub-components of this feature.
  ///
  /// The address range format string is effectively either
  /// "%s %s" or "%s, %s" where one of the sub-ranges
  /// represents the house number or house number range,
  /// and the other represents the road name.
  GEOSEARCH_TEXT_TYPE_ROAD_ADDRESS,

  /// @brief Indicates that a range of text in a string
  /// represents a house number.
  GEOSEARCH_TEXT_TYPE_HOUSE_NUMBER,

  /// @brief Indicates that a range of text in a string
  /// represents a house number range.
  ///
  /// Exactly two sub-ranges must be contained within this
  /// range, for each of the start and end numbers of the
  /// house number range.
  ///
  /// The house number range format string is effectively
  /// "%d - %d" where each %d represent sub-ranges for both
  /// the start and end numbers, respectively.
  GEOSEARCH_TEXT_TYPE_HOUSE_NUMBER_RANGE,

  /// @brief Indicates that a range of text in a string
  /// represents an intersection.
  ///
  /// Exactly two sub-ranges must be contained within this
  /// range, one for each of two road names that identify the
  /// intersection.
  ///
  /// The intersection range format string is effectively
  /// "%s, %s" where each of the sub-ranges represents a road
  /// name.
  GEOSEARCH_TEXT_TYPE_INTERSECTION,

  /// @brief Indicates that a range of text in a string
  /// represents a lat-long position.
  ///
  /// Exactly two sub-ranges must be contained within this
  /// range, one for each of the latitude and longitude values
  /// that identify the position.
  ///
  /// The intersection range format string is effectively
  /// "%s, %s" where each of the sub-ranges represents a latitude
  /// or longitude value.
  GEOSEARCH_TEXT_TYPE_LAT_LONG_ADDRESS,

  /// @brief Indicates that a range of text in a string
  /// represents a latitude in degree/minute/second format.
  GEOSEARCH_TEXT_TYPE_LATITUDE_DMS,

  /// @brief Indicates that a range of text in a string
  /// represents a longitude in degree/minute/second format.
  GEOSEARCH_TEXT_TYPE_LONGITUDE_DMS,

  /// @brief Indicates that a range of text in a string
  /// represents a place qualifier, including the punctuation
  /// and prepositional phrase that surrounds and separates
  /// the place qualifier from its base name.
  ///
  /// The base name is represented by a range preceeding
  /// this range at the same nesting level.
  ///
  /// A single sub-range must be contained within this
  /// range which represents the place qualifier name,
  /// which may be a complex name.
  ///
  /// The place qualifier format string is effectively
  /// "; in %s" where %s represents the sub-range.
  GEOSEARCH_TEXT_TYPE_IN_PLACE_QUALIFIER,

  /// @brief Indicates that a range of text in a string
  /// represents a place qualifier, including the punctuation
  /// and prepositional phrase that surrounds and separates
  /// the place qualifier from its base name.
  ///
  /// The base name is represented by a range preceeding
  /// this range at the same nesting level.
  ///
  /// A single sub-range must be contained within this
  /// range which represents the place qualifier name,
  /// which may be a complex name.
  ///
  /// The place qualifier format string is effectively
  /// "; thru %s" where %s represents the sub-range.
  GEOSEARCH_TEXT_TYPE_THRU_PLACE_QUALIFIER,

  /// @brief Indicates that a range of text in a string
  /// represents a place qualifier, including the punctuation
  /// and prepositional phrase that surrounds and separates
  /// the place qualifier from its base name.
  ///
  /// The base name is represented by a range preceeding
  /// this range at the same nesting level.
  ///
  /// A single sub-range must be contained within this
  /// range which represents the place qualifier name,
  /// which may be a complex name.
  ///
  /// The place qualifier format string is effectively
  /// "; near %s" where %s represents the sub-range.
  GEOSEARCH_TEXT_TYPE_NEAR_PLACE_QUALIFIER,

  /// @brief An invalid text type.
  GEOSEARCH_TEXT_TYPE_MAX

} TGeosearchTextType;

//--------------------------------------------------------------------------------
/// @brief This type is a structure to hold information about
///        a range of text.
///
typedef struct TGeosearchTextRange {

  /// @brief The type of the range of text.
  TGeosearchTextType type;

  /// @brief The nesting level of the range.
  UINT32             level;

  /// @brief The start position of the range.
  UINT32             startPos;

  /// @brief The end position (one past the last) of the range.
  UINT32             endPos;

} TGeosearchTextRange;

typedef struct POIInfo {
  UINT32 ID;
  UINT16 typeID;
  UTF16 szName[72];
  TPos pos;
  char iconName[72];
  UTF16 typeName[72];
  UTF16 phoneNumber[72];
  UTF16 address[1024];
} POIInfo;

typedef struct MiddlePOIResult {
  UINT16 sectorID;
  UINT16 mapIdx;
  UINT16 typeSetIndex;
  UINT32 resultCost;
  UINT32 nameIdx;
} MiddlePOIResult;

//--------------------------------------------------------------------------------
/// @brief This type is a handle to an object that represents
///        a search result.
///
#if defined(__cplusplus)
struct TGeosearchResultHandle : public TGeosearchObjectHandle {
};
#else
typedef union TGeosearchResultHandle {
  /// @internal
  TGeosearchHandle  _genericObjectHandle;

  /// @internal
  TGeosearchHandle  _genericResultHandle;

} TGeosearchResultHandle;
#endif

//--------------------------------------------------------------------------------
/// @enum TGeosearchResultType
typedef enum TGeosearchResultType {

  /// @brief invalid result type.
  GEOSEARCH_RESULT_TYPE_INVALID,

  /// @brief country name.
  GEOSEARCH_RESULT_TYPE_COUNTRY,

  /// @brief state name.
  GEOSEARCH_RESULT_TYPE_STATE,

  /// @brief place name.
  GEOSEARCH_RESULT_TYPE_PLACE,

  /// @brief postal code.
  GEOSEARCH_RESULT_TYPE_POSTAL_CODE,

  /// @brief UK postal code.
  GEOSEARCH_RESULT_TYPE_DETAILED_POSTAL_CODE,

  /// @brief road name.
  GEOSEARCH_RESULT_TYPE_ROAD,

  /// @brief road intersection.
  GEOSEARCH_RESULT_TYPE_INTERSECTION,

  /// @brief specific segment and house number.
  GEOSEARCH_RESULT_TYPE_HOUSE_NUMBER,

  /// @brief POI type result.
  GEOSEARCH_RESULT_TYPE_POI_TYPE,

  /// @brief user POI type result.
  GEOSEARCH_RESULT_TYPE_USER_POI_TYPE,

  /// @brief POI name.
  GEOSEARCH_RESULT_TYPE_POI,

  /// @brief user POI name.
  GEOSEARCH_RESULT_TYPE_USER_POI,

  /// @brief POI name, sorted by distance from search centroid.
  GEOSEARCH_RESULT_TYPE_NEAREST_POI,

  /// @brief user POI name, sorted by distance from search centroid.
  GEOSEARCH_RESULT_TYPE_NEAREST_USER_POI,

  /// @brief active POI, sorted by distance from search centroid.
  GEOSEARCH_RESULT_TYPE_ACTIVE_POI,

  /// @brief active user POI, sorted by distance from search centroid.
  GEOSEARCH_RESULT_TYPE_ACTIVE_USER_POI,

  /// @brief lat/long position.
  GEOSEARCH_RESULT_TYPE_LAT_LONG,

  /// @brief first-phonetic POI name.
  GEOSEARCH_RESULT_TYPE_FIRST_PHONETIC_POI,//for first-phonetic search

  /// @brief invalid result type.
  /// @internal
  GEOSEARCH_RESULT_TYPE_PLACE_SUB,		// 0011
  GEOSEARCH_RESULT_TYPE_MAX

} TGeosearchResultType;

//--------------------------------------------------------------------------------
/// @brief This type is a handle to an object that represents
///        search parameters.
///
#if defined(__cplusplus)
struct TGeosearchParametersHandle : public TGeosearchObjectHandle {
};
#else
typedef union TGeosearchParametersHandle {
  /// @internal
  TGeosearchHandle  _genericObjectHandle;

  /// @internal
  TGeosearchHandle  _genericParametersHandle;

} TGeosearchParametersHandle;
#endif

//--------------------------------------------------------------------------------
/// @brief This type is a handle to an object that represents
///        a search query.
///
#if defined(__cplusplus)
struct TGeosearchQueryHandle : public TGeosearchObjectHandle {
};
#else
typedef union TGeosearchQueryHandle {
  /// @internal
  TGeosearchHandle  _genericObjectHandle;

  /// @internal
  TGeosearchHandle  _genericQueryHandle;

} TGeosearchQueryHandle;
#endif

//--------------------------------------------------------------------------------
/// @brief This type holds a globally unique POI type id that
///        uniquely identifies its POI type.
///
typedef TPOITypeId16 TGeosearchPOITypeId;

//--------------------------------------------------------------------------------
/// @brief Get an invalid object handle.
///
/// @return  An invalid object handle.
///
#if defined(__cplusplus)
#define geosearch_getInvalidObjectHandle()\
        TGeosearchObjectHandle()
#else
#define geosearch_getInvalidObjectHandle()\
        (*(TGeosearchObjectHandle*)&_geosearchNullHandle)
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchObject_isValidHandle( TGeosearchHandle genericObjectHandle );
///
/// @brief Determine whether or not a handle is valid.
///
/// @param[in]  objectHandle  The handle to a specific type of object.
///
/// @return  A flag that indicates whether or not the handle is valid.
///
#if defined(__cplusplus)
inline BOOL8 geosearchObject_isValidHandle( TGeosearchObjectHandle objectHandle )
{
  return _geosearchObject_isValidHandle( objectHandle._genericHandle );
}
#else
#define geosearchObject_isValidHandle( objectHandle )\
       _geosearchObject_isValidHandle( (objectHandle)._genericObjectHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
void _geosearchObject_addRef( TGeosearchHandle genericObjectHandle );
///
/// @brief Increase the reference count of an object.
///
/// @param[in]  objectHandle  The handle to a specific type of object.
///
#if defined(__cplusplus)
inline void geosearchObject_addRef( TGeosearchObjectHandle objectHandle )
{
  _geosearchObject_addRef( objectHandle._genericHandle );
}
#else
#define geosearchObject_addRef( objectHandle )\
       _geosearchObject_addRef( (objectHandle)._genericObjectHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
void _geosearchObject_release( TGeosearchHandle genericObjectHandle );
///
/// @brief Decrease the reference count of a generic object, and
///        destroy the object if the reference count reaches zero.
///
/// @param[in]  objectHandle  The handle to a specific type of object.
///
#if defined(__cplusplus)
inline void geosearchObject_release( TGeosearchObjectHandle objectHandle )
{
  _geosearchObject_release( objectHandle._genericHandle );
}
#else
#define geosearchObject_release( objectHandle )\
       _geosearchObject_release( (objectHandle)._genericObjectHandle )
#endif

//--------------------------------------------------------------------------------
/// @brief Get an invalid text object handle.
///
/// @return  An invalid text object handle.
///
#if defined(__cplusplus)
#define geosearch_getInvalidTextHandle()\
        TGeosearchTextHandle()
#else
#define geosearch_getInvalidTextHandle()\
        (*(TGeosearchTextHandle*)&_geosearchNullHandle)
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT32 _geosearchText_getStringLength( TGeosearchHandle genericTextHandle );
///
/// @brief Get the length of the string stored in the text object.
///
/// @param[in]  textHandle        A handle to a text object.
///
/// @return  The length of the result name string.
///
#if defined(__cplusplus)
inline UINT32 geosearchText_getStringLength( TGeosearchTextHandle textHandle )
{
  return _geosearchText_getStringLength( textHandle._genericHandle );
}
#else
#define geosearchText_getStringLength( textHandle )\
       _geosearchText_getStringLength( (textHandle)._genericTextHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT32 _geosearchText_getString( TGeosearchHandle genericTextHandle,
                                 UINT32           stringBufferSize,
                                 UTF16            stringBuffer[] );
///
/// @brief Get a copy of the string stored in a text object.
///
/// @param[in]  textHandle        A handle to a text object.
///
/// @param[in]  stringBufferSize  The size of the output string buffer.
///
/// @param[out] stringBuffer      The output string buffer.
///
/// @return  The length of the output result name string.
///
#if defined(__cplusplus)
inline UINT32 geosearchText_getString( TGeosearchTextHandle textHandle,
                                       UINT32               stringBufferSize,
                                       UTF16                stringBuffer[] )
{
  return _geosearchText_getString( textHandle._genericHandle,
                                   stringBufferSize,
                                   stringBuffer );
}
#else
#define geosearchText_getString( textHandle,                     \
                                 stringBufferSize,               \
                                 stringBuffer )                  \
       _geosearchText_getString( (textHandle)._genericTextHandle,\
                                 stringBufferSize,               \
                                 stringBuffer )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT32 _geosearchText_getRangeCount( TGeosearchHandle genericTextHandle );
///
/// @brief Get the number of ranges that describe the structure of
///        the text object.
///
/// @param[in]  textHandle        A handle to a text object.
///
/// @return  The number of text ranges.
///
#if defined(__cplusplus)
inline UINT32 geosearchText_getRangeCount( TGeosearchTextHandle textHandle )
{
  return _geosearchText_getRangeCount( textHandle._genericHandle );
}
#else
#define geosearchText_getRangeCount( textHandle )\
       _geosearchText_getRangeCount( (textHandle)._genericTextHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT32 _geosearchText_getRanges( TGeosearchHandle    genericTextHandle,
                                 UINT32              maxRangeCount,
                                 TGeosearchTextRange ranges[] );
///
/// @brief Get a copy of the ranges that describe the structure of
///        the text object.
///
/// @param[in]  textHandle        A handle to a text object.
///
/// @param[in]  maxRangeCount     The size of the output range buffer.
///
/// @param[out] ranges            An array to store the output ranges.
///                          
/// @return  The number of output result name ranges.
///
#if defined(__cplusplus)
inline UINT32 geosearchText_getRanges( TGeosearchTextHandle textHandle,
                                       UINT32               maxRangeCount,
                                       TGeosearchTextRange  ranges[] )
{
  return _geosearchText_getRanges( textHandle._genericHandle,
                                   maxRangeCount,
                                   ranges );
}
#else
#define geosearchText_getRanges( textHandle,                     \
                                 maxRangeCount,                  \
                                 ranges )                        \
       _geosearchText_getRanges( (textHandle)._genericTextHandle,\
                                 maxRangeCount,                  \
                                 ranges )
#endif

//--------------------------------------------------------------------------------
/// @brief Initialise a rectangle with world coordinate search bounds
///        calculated from the specified centroid and size.
///
/// @param[in]  searchBounds    The search bounds to be initialised.
///
/// @param[in]  searchCentroid  The centre position of the search bounds.
///
/// @param[in]  searchSize      The half size in metres of the search
///                             bounds.
void geosearch_initSearchBounds( TRect*      searchBounds,
                                 const TPos* searchCentroid,
                                 UINT32      searchSize );

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearch_makeWorldSearchLocation( const TRect*      searchBounds,
                                          TGeosearchHandle* genericSearchLocationHandle );
///
/// @brief Initialise a search location object to be used to create
///        searches in all loaded maps, possibly within a specified
///        search bounds.
///
/// @param[in]  searchBounds          A bounding box in map coordinates to
///                                   be used to filter results for a local
///                                   (or fuzzy) type search, or NULL if a
///                                   global search is to be done.
///
/// @param[out] searchLocationHandle  A handle to a search location object.
///
/// @return  A flag which indicates if a search location object
///          has been successfully created.
///
#if defined(__cplusplus)
inline BOOL8 geosearch_makeWorldSearchLocation( const TRect*            searchBounds,
                                                TGeosearchObjectHandle* searchLocationHandle )
{
  return _geosearch_makeWorldSearchLocation( searchBounds,
                                             &searchLocationHandle->_genericHandle );
}
#else
#define geosearch_makeWorldSearchLocation( searchBounds,         \
                                           searchLocationHandle )\
       _geosearch_makeWorldSearchLocation( searchBounds,         \
                                           &(searchLocationHandle)->_genericObjectHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearch_makeJourneySearchLocation( const TJourney*   journey,
                                            TMapLocation*     mapLocation,
                                            UINT32            searchSize,
                                            UINT32            maxDetour,
                                            TGeosearchHandle* genericSearchLocationHandle );
///
/// @brief Initialise a search location object to be used to create
///        a search along a journey from a specified start position.
///
/// @param[in]  journey               A pointer to the journey to search along.
///
/// @param[in]  mapLocation           A pointer to a map location structure
///                                   which represents the start position to
///                                   search from.
///
/// @param[in]  searchSize            The half size (in metres) of the search
///                                   bounds to be calculated around the start
///                                   position.
///
/// @param[in]  maxDetour             The maximum allowed detour (in metres)
///                                   to search along roads off the specified
///                                   journey.
///
/// @param[out] searchLocationHandle  A handle to a search location object.
///
/// @return  A flag which indicates if a search location object
///          has been successfully created.
///
/// @note  The journey structure must remain valid throughout the whole
///        nearest POI search.
///
#if defined(__cplusplus)
inline BOOL8 geosearch_makeJourneySearchLocation( const TJourney*         journey,
                                                  TMapLocation*           mapLocation,
                                                  UINT32                  searchSize,
                                                  UINT32                  maxDetour,
                                                  TGeosearchObjectHandle* searchLocationHandle )
{
  return _geosearch_makeJourneySearchLocation( journey,
                                               mapLocation,
                                               searchSize,
                                               maxDetour,
                                               &searchLocationHandle->_genericHandle );
}
#else
#define geosearch_makeJourneySearchLocation( journey,              \
                                             mapLocation,          \
                                             searchSize,           \
                                             maxDetour,            \
                                             searchLocationHandle )\
       _geosearch_makeJourneySearchLocation( journey,              \
                                             mapLocation,          \
                                             searchSize,           \
                                             maxDetour,            \
                                             &(searchLocationHandle)->_genericObjectHandle )
#endif

//--------------------------------------------------------------------------------
/// @brief Get an invalid parameters object handle.
///
/// @return  An invalid parameters object handle.
///
#if defined(__cplusplus)
#define geosearch_getInvalidParametersHandle()\
        TGeosearchParametersHandle()
#else
#define geosearch_getInvalidParametersHandle()\
        (*(TGeosearchParametersHandle*)&_geosearchNullHandle)
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearch_makeSearchParameters( TGeosearchResultType resultType,
                                       const UTF16*         searchString,
                                       TGeosearchHandle     genericSearchLocationHandle,
                                       TGeosearchHandle*    genericParametersHandle );
///
/// @brief Create search parameters for a search within the specified search
///        location for results of the specified result type that have names
///        that match the specified search string.
///
/// @param[in]  resultType              The type of result to search for.
///
/// @param[in]  searchString            A pointer to a search string filter.
///
/// @param[in]  searchLocationHandle    A handle to a search location object.
///
/// @param[out] searchParametersHandle  A handle to the search parameters
///                                     object which has been created.
///
/// @return  A flag which indicates if a search parameters object has been
///          successfully created.
///
/// @note  A search parameters object will not be created for a type of search
///        which is not available in a particular type of search location
///        (for example, no place searching in road result search locations).
///
/// @note  For an intersection search, the search location may be a base road
///        (in which case the search input is to match an intersecting road name)
///        or a place location (in which case the search input must specify two
///        road names at the intersection).
///
/// @note  For a house number search, the search location may be a base road or
///        a detailed postal code (in which case the search input is a number only),
///        or a place location (in which case the search input must specify a
///        number and a road name).
/// 
#if defined(__cplusplus)
inline BOOL8 geosearch_makeSearchParameters( TGeosearchResultType        resultType,
                                             const UTF16*                searchString,
                                             TGeosearchObjectHandle      searchLocationHandle,
                                             TGeosearchParametersHandle* searchParametersHandle )
{
  return _geosearch_makeSearchParameters( resultType,
                                          searchString,
                                          searchLocationHandle._genericHandle,
                                          &searchParametersHandle->_genericHandle );
}
#else
#define geosearch_makeSearchParameters( resultType,                                 \
                                        searchString,                               \
                                        searchLocationHandle,                       \
                                        searchParametersHandle )                    \
       _geosearch_makeSearchParameters( resultType,                                 \
                                        searchString,                               \
                                        (searchLocationHandle)._genericObjectHandle,\
                                        &(searchParametersHandle)->_genericParametersHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearch_makePOISearchParameters( TGeosearchResultType resultType,
                                          const UTF16*         searchString,
                                          TGeosearchHandle     genericSearchLocationHandle,
                                          TGeosearchPOITypeId  POITypeId,
                                          UINT32               maxDistance,
                                          void*                userPOIContext,
                                          TGeosearchHandle*    genericParametersHandle );
///
/// @brief Create search parameters for a search within the specified search
///        location for POI results that have names that match the specified
///        search string and have POI types that match any of the specified
///        POI types.
///
/// @param[in]  resultType              The type of POI result to search for.
///
/// @param[in]  searchString            A pointer to a search string filter.
///
/// @param[in]  searchLocationHandle    A handle to a search location object.
///
/// @param[in]  POITypeId               An POI type id to filter the search.
///                                     Specify zero if not required or not
///                                     relevant.
///
/// @param[in]  maxDistance             The maximum drivable distance (in metres)
///                                     for nearest POI searches.  Must be zero
///                                     for non nearest POI searches.
///
/// @param[in]  userPOIContext          An optional pointer to context data
///                                     to be used in user POI searches.
///                                     Must be null for non user POI searches.
///
/// @param[out] searchParametersHandle  A handle to the search parameters
///                                     object which has been created.
///
/// @return  A flag which indicates if a search parameters object has been
///          successfully created.
///
/// @note  If a POI type filter is not required and a user POI context is not
///        required, then the function @a geosearch_makeSearchParameters()
///        can also be used to create the search parameters for a POI search.
///
#if defined(__cplusplus)
inline BOOL8 geosearch_makePOISearchParameters( TGeosearchResultType        resultType,
                                                const UTF16*                searchString,
                                                TGeosearchObjectHandle      searchLocationHandle,
                                                TGeosearchPOITypeId         POITypeId,
                                                UINT32                      maxDistance,
                                                void*                       userPOIContext,
                                                TGeosearchParametersHandle* searchParametersHandle )
{
  return _geosearch_makePOISearchParameters( resultType,
                                             searchString,
                                             searchLocationHandle._genericHandle,
                                             POITypeId,
                                             maxDistance,
                                             userPOIContext,
                                             &searchParametersHandle->_genericHandle );
}
#else
#define geosearch_makePOISearchParameters( resultType,                                 \
                                           searchString,                               \
                                           searchLocationHandle,                       \
                                           POITypeId,                                  \
                                           maxDistance,                                \
                                           userPOIContext,                             \
                                           searchParametersHandle )                    \
       _geosearch_makePOISearchParameters( resultType,                                 \
                                           searchString,                               \
                                           (searchLocationHandle)._genericObjectHandle,\
                                           POITypeId,                                  \
                                           maxDistance,                                \
                                           userPOIContext,                             \
                                           &(searchParametersHandle)->_genericParametersHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearch_makePOITypeSearchParameters( TGeosearchResultType searchType,
                                              const UTF16*         searchString,
                                              TGeosearchHandle     genericSearchLocationHandle,
                                              void*                userPOIContext,
                                              TGeosearchHandle*    genericParametersHandle );
///
/// @brief Create search parameters for a search within the specified search
///        location for POI types where the search string matches instances
///        of the specified search type (one of POI, user POI, POI type, or
///        user POI type).
///
/// @param[in]  searchType              The type of entity to search for.
///
/// @param[in]  searchString            A pointer to a search string filter.
///
/// @param[in]  searchLocationHandle    A handle to a search location object.
///
/// @param[in]  userPOIContext          An optional pointer to context data
///                                     to be used in user POI searches.
///                                     Must be null for non user POI searches.
///
/// @param[out] searchParametersHandle  A handle to the search parameters
///                                     object which has been created.
///
/// @return  A flag which indicates if a search parameters object has been
///          successfully created.
///
/// @note  If a search for POI types is required, then either or the functions
///        @a geosearch_makeSearchParameters() or @a geosearch_makePOISearchParameters()
///        can be used instead.
///
/// @note  If a search for user POI types is required, then the function
///        @a geosearch_makePOISearchParameters() can be used instead.  The
///        function @a geosearch_makeSearchParameters() can also be used if
///        a user POI context is not required.
///
#if defined(__cplusplus)
inline BOOL8 geosearch_makePOITypeSearchParameters( TGeosearchResultType        searchType,
                                                    const UTF16*                searchString,
                                                    TGeosearchObjectHandle      searchLocationHandle,
                                                    void*                       userPOIContext,
                                                    TGeosearchParametersHandle* searchParametersHandle )
{
  return _geosearch_makePOITypeSearchParameters( searchType,
                                                 searchString,
                                                 searchLocationHandle._genericHandle,
                                                 userPOIContext,
                                                 &searchParametersHandle->_genericHandle );
}
#else
#define geosearch_makePOITypeSearchParameters( searchType,                                 \
                                               searchString,                               \
                                               searchLocationHandle,                       \
                                               userPOIContext,                             \
                                               searchParametersHandle )                    \
       _geosearch_makePOITypeSearchParameters( searchType,                                 \
                                               searchString,                               \
                                               (searchLocationHandle)._genericObjectHandle,\
                                               userPOIContext,                             \
                                               &(searchParametersHandle)->_genericParametersHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearch_makeSearchQuery( const TGeosearchHandle genericParameters[],
                                  UINT32                 searchParametersCount,
                                  UINT32                 maxResultCount,
                                  TGeosearchHandle*      genericQueryHandle );
///
/// @brief Create a search query for all types of entities within the
///        search location of the search query factory.
///
/// @param[in]  searchParameters       An array of search parameter objects to
///                                    specify the different types of searches to
///                                    perform in a single search query.
///
/// @param[in]  searchParametersCount  The number of search parameter objects.
///
/// @param[in]  maxResultCount         The maximum number of results to fetch,
///                                    or zero for an 'exists' query.
///
/// @param[out] queryHandle            A handle to the search query object
///                                    which has been created.
///
/// @return  A flag which indicates if a search query object has been
///          successfully created.
///
/// @note  If the specified combination of search parameters cannot be combined
///        into a single search query, then this function will fail. @n
///        We currently allow any type of search to be combined with any other
///        type of search subject to the following restrictions: @n
///             1. Each type of search may be specified at most once in the
///                input search parameters. @n
///             2. The search strings must be the same in all input search
///                parameters. @n
///             3. The search locations must be the same in all input search
///                parameters. @n
///             4. INTERSECTION cannot be combined with anything if there is
///                a base road. @n
///             5. HOUSE NUMBER cannot be combined with anything if there is
///                a base road or roads. @n
///             6. POI TYPE and USER POI TYPE can only be combined with each
///                other. @n
///             7. When POI and USER POI are combined with each other, the POI
///                type name of both input search parameters must be the same. @n
///             8. NEAREST POI and NEAREST USER POI can only be combined with
///                each other, and the POI type name and max distance of both
///                input search parameters must be the same. @n
///
#if defined(__cplusplus)
inline BOOL8 geosearch_makeSearchQuery( const TGeosearchParametersHandle searchParameters[],
                                        UINT32                           searchParametersCount,
                                        UINT32                           maxResultCount,
                                        TGeosearchQueryHandle*           queryHandle )
{
  return _geosearch_makeSearchQuery( &searchParameters->_genericHandle,
                                     searchParametersCount,
                                     maxResultCount,
                                     &queryHandle->_genericHandle );
}
#else
#define geosearch_makeSearchQuery( searchParameters,                             \
                                   searchParametersCount,                        \
                                   maxResultCount,                               \
                                   queryHandle )                                 \
       _geosearch_makeSearchQuery( &(searchParameters)->_genericParametersHandle,\
                                   searchParametersCount,                        \
                                   maxResultCount,                               \
                                   &(queryHandle)->_genericQueryHandle )
#endif

//--------------------------------------------------------------------------------
/// @brief Get an invalid query object handle.
///
/// @return  An invalid query object handle.
///
#if defined(__cplusplus)
#define geosearch_getInvalidQueryHandle()\
        TGeosearchQueryHandle()
#else
#define geosearch_getInvalidQueryHandle()\
        (*(TGeosearchQueryHandle*)&_geosearchNullHandle)
#endif

//--------------------------------------------------------------------------------
/// @internal
INT32 _geosearchQuery_execute( TGeosearchHandle genericQueryHandle );
///
/// @brief Execute a search query.
///
/// @param[in]  queryHandle      A handle to a search query object.
///
/// @return  >0 if the search query has not yet completed execution,
///           0 if the search query has completed successfully,
///          <0 if the search query has stopped due to an error condition.
///
/// @note  This function can be called after the query has completed
///        execution, in which case the last return value will be
///        returned again.
///
#if defined(__cplusplus)
inline INT32 geosearchQuery_execute( TGeosearchQueryHandle queryHandle )
{
  return _geosearchQuery_execute( queryHandle._genericHandle );
}
#else
#define geosearchQuery_execute( queryHandle )\
       _geosearchQuery_execute( (queryHandle)._genericQueryHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT8 _geosearchQuery_getProgress( TGeosearchHandle genericQueryHandle );
///
/// @brief Get the progress of the execution of the query, as a
///        precentage (0 to 100).
///
/// @param[in]  queryHandle      A handle to a search query object.
///
/// @return  The query progress, as a percentage.
///
#if defined(__cplusplus)
inline UINT8 geosearchQuery_getProgress( TGeosearchQueryHandle queryHandle )
{
  return _geosearchQuery_getProgress( queryHandle._genericHandle );
}
#else
#define geosearchQuery_getProgress( queryHandle )\
       _geosearchQuery_getProgress( (queryHandle)._genericQueryHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
INT32 _geosearchQuery_getStatus( TGeosearchHandle genericQueryHandle );
///
/// @brief Get the execution status from a completed query.
///
/// @param[in]  queryHandle      A handle to a search query object.
///
/// @return  zero, if a completed search query was successful
///                (or a result was found for an 'exists' query).
///          non-zero, if a completed search query stopped due to
///                    an error condition (or no results were found
///                    for an 'exists' query).
///
#if defined(__cplusplus)
inline INT32 geosearchQuery_getStatus( TGeosearchQueryHandle queryHandle )
{
  return _geosearchQuery_getStatus( queryHandle._genericHandle );
}
#else
#define geosearchQuery_getStatus( queryHandle )\
       _geosearchQuery_getStatus( (queryHandle)._genericQueryHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT32 _geosearchQuery_getResults( TGeosearchHandle genericQueryHandle,
                                   TGeosearchHandle genericResults[] );
///
/// @brief Get the output results from a completed search query.
///
/// @param[in]  queryHandle      A handle to a search query object.
///
/// @param[out] results          An output array to store found search
///                              result handles into.
///                              The number of elements in the array must
///                              be sufficient to store 'maxResultCount'
///                              result handles, specified when the search
///                              query was created.
///
/// @return  The number of found search results, or zero if the query
///          was stopped due to an error condition.
///
#if defined(__cplusplus)
inline UINT32 geosearchQuery_getResults( TGeosearchQueryHandle  queryHandle,
                                         TGeosearchResultHandle results[] )
{
  return _geosearchQuery_getResults( queryHandle._genericHandle,
                                     &results->_genericHandle );
}
#else
#define geosearchQuery_getResults( queryHandle,                      \
                                   results )                         \
       _geosearchQuery_getResults( (queryHandle)._genericQueryHandle,\
                                   &(results)->_genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearch_findMapLocationResult( TMapLocation*     mapLocation,
                                        TGeosearchHandle* genericResultHandle );
///
/// @brief Reverse geocode a map location into a search result.
///
/// @param[in]  mapLocation      A pointer to a map location structure.
///
/// @param[out] resultHandle     A handle to a search result which represents
///                              the reverse geocoded map location.
///
/// @return  A flag which indicates if the reverse geocode was successful.
///
#if defined(__cplusplus)
inline BOOL8 geosearch_findMapLocationResult( TMapLocation*           mapLocation,
                                              TGeosearchResultHandle* resultHandle )
{
  return _geosearch_findMapLocationResult( mapLocation,
                                           &resultHandle->_genericHandle );
}
#else
#define geosearch_findMapLocationResult( mapLocation,  \
                                         resultHandle )\
       _geosearch_findMapLocationResult( mapLocation,  \
                                         &(resultHandle)->_genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearch_findMapLocationRoadResult( TMapLocation*     mapLocation,
                                            TGeosearchHandle* genericResultHandle );
///
/// @brief Reverse geocode a map location into a road search result.
///
/// @param[in]  mapLocation      A pointer to a map location structure.
///
/// @param[out] resultHandle     A handle to a road search result which represents
///                              the reverse geocoded map location.
///
/// @return  A flag which indicates if the reverse geocode was successful.
///
#if defined(__cplusplus)
inline BOOL8 geosearch_findMapLocationRoadResult( TMapLocation*           mapLocation,
                                                  TGeosearchResultHandle* resultHandle )
{
  return _geosearch_findMapLocationRoadResult( mapLocation,
                                               &resultHandle->_genericHandle );
}
#else
#define geosearch_findMapLocationRoadResult( mapLocation,  \
                                             resultHandle )\
       _geosearch_findMapLocationRoadResult( mapLocation,  \
                                             &(resultHandle)->_genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @brief Get an invalid result object handle.
///
/// @return  An invalid result object handle.
///
#if defined(__cplusplus)
#define geosearch_getInvalidResultHandle()\
        TGeosearchResultHandle()
#else
#define geosearch_getInvalidResultHandle()\
        (*(TGeosearchResultHandle*)&_geosearchNullHandle)
#endif

//--------------------------------------------------------------------------------
/// @internal
TGeosearchResultType _geosearchResult_getType( TGeosearchHandle genericResultHandle );
///
/// @brief Get the type of a search result object.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @return  The type of a search result object.
///
#if defined(__cplusplus)
inline TGeosearchResultType geosearchResult_getType( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getType( resultHandle._genericHandle );
}
#else
#define geosearchResult_getType( resultHandle )\
       _geosearchResult_getType( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchResult_getName( TGeosearchHandle  genericResultHandle,
                                TGeosearchHandle* genericTextHandle );
///
/// @brief Get a text object for the name of the result.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @param[out] nameTextHandle    A handle to a name text object.
///
/// @return  A flag indicating whether or not the name text object
///          was successfully gotten.
///
#if defined(__cplusplus)
inline BOOL8 geosearchResult_getName( TGeosearchResultHandle resultHandle,
                                      TGeosearchTextHandle*  nameTextHandle )
{
  return _geosearchResult_getName( resultHandle._genericHandle,
                                   &nameTextHandle->_genericHandle );
}
#else
#define geosearchResult_getName( resultHandle,                       \
                                 nameTextHandle )                    \
       _geosearchResult_getName( (resultHandle)._genericResultHandle,\
                                 &(nameTextHandle)->_genericTextHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchResult_getLocationName( TGeosearchHandle  genericResultHandle,
                                        TGeosearchHandle* genericTextHandle );
///
/// @brief Get a text object for the name of the location for the result.
///
/// @param[in]  resultHandle     A handle to a search result object.
///
/// @param[out] nameTextHandle   A handle to a name text object.
///
/// @return  A flag indicating whether or not the name text object
///          was successfully gotten.
///
#if defined(__cplusplus)
inline BOOL8 geosearchResult_getLocationName( TGeosearchResultHandle resultHandle,
                                              TGeosearchTextHandle*  nameTextHandle )
{
  return _geosearchResult_getLocationName( resultHandle._genericHandle,
                                           &nameTextHandle->_genericHandle );
}
#else
#define geosearchResult_getLocationName( resultHandle,                       \
                                         nameTextHandle )                    \
       _geosearchResult_getLocationName( (resultHandle)._genericResultHandle,\
                                         &(nameTextHandle)->_genericTextHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchResult_getSearchLocation( TGeosearchHandle  genericResultHandle,
                                          TGeosearchHandle* genericSearchLocationHandle );
///
/// @brief Get a search location object for the specified search result.
///
/// @param[in]  resultHandle          A handle to a search result object.
///
/// @param[out] searchLocationHandle  A handle to a search location object.
///
/// @return  A flag which indicates if a search location object has been
///          successfully gotten.
///
#if defined(__cplusplus)
inline BOOL8 geosearchResult_getSearchLocation( TGeosearchResultHandle  resultHandle,
                                                TGeosearchObjectHandle* searchLocationHandle )
{
  return _geosearchResult_getSearchLocation( resultHandle._genericHandle,
                                             &searchLocationHandle->_genericHandle );
}
#else
#define geosearchResult_getSearchLocation( resultHandle,                       \
                                           searchLocationHandle )              \
       _geosearchResult_getSearchLocation( (resultHandle)._genericResultHandle,\
                                           &(searchLocationHandle)->_genericObjectHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
TNumericISOCountryCode _geosearchResult_getNumericISOCountryCode( TGeosearchHandle genericResultHandle );
///
/// @brief Get the numeric ISO country code for a country result.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @return  The numeric ISO country code of a country result, or zero if not applicable.
///
#if defined(__cplusplus)
inline TNumericISOCountryCode geosearchResult_getNumericISOCountryCode( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getNumericISOCountryCode( resultHandle._genericHandle );
}
#else
#define geosearchResult_getNumericISOCountryCode( resultHandle )\
       _geosearchResult_getNumericISOCountryCode( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT32 _geosearchResult_getHouseNumber( TGeosearchHandle genericResultHandle );
///
/// @brief Get the result house number, if any.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @return  The house number of a result, or zero if not applicable.
///
#if defined(__cplusplus)
inline UINT32 geosearchResult_getHouseNumber( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getHouseNumber( resultHandle._genericHandle );
}
#else
#define geosearchResult_getHouseNumber( resultHandle )\
       _geosearchResult_getHouseNumber( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchResult_getHouseNumberRangeType( TGeosearchHandle genericResultHandle,
                                                BOOL8*           flagMixed );
///
/// @brief Determine whether or not a house number result is a range,
///        and if so, then indicate whether or not the range is mixed.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @param[out] flagMixed         If the result is a range, then this
///                               flag will indicate whether or not
///                               the range is mixed.
///
/// @return  A flag indicating whether or not a house number result
///          is a range of numbers.
///
#if defined(__cplusplus)
inline BOOL8 geosearchResult_getHouseNumberRangeType( TGeosearchResultHandle resultHandle,
                                                      BOOL8*                 flagMixed )
{
  return _geosearchResult_getHouseNumberRangeType( resultHandle._genericHandle,
                                                   flagMixed );
}
#else
#define geosearchResult_getHouseNumberRangeType( resultHandle,                       \
                                                 flagMixed )                         \
       _geosearchResult_getHouseNumberRangeType( (resultHandle)._genericResultHandle,\
                                                 flagMixed )
#endif

//--------------------------------------------------------------------------------
/// @internal
TGeosearchResultType _geosearchResult_getSearchStringMatchType( TGeosearchHandle genericResultHandle );
///
/// @brief Get the type of result that matched the search string,
///        which may be different than the actual result type.
///
/// @param[in]  resultHandle  A handle to a search result object.
///
/// @return  The type of result that matched the search string.
///
#if defined(__cplusplus)
inline TGeosearchResultType geosearchResult_getSearchStringMatchType( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getSearchStringMatchType( resultHandle._genericHandle );
}
#else
#define geosearchResult_getSearchStringMatchType( resultHandle )\
       _geosearchResult_getSearchStringMatchType( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT32 _geosearchResult_getDistance( TGeosearchHandle genericResultHandle );
///
/// @brief Get the result distance (in metres), if any.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @return  The stored distance for a result, or zero if not applicable.
///
#if defined(__cplusplus)
inline UINT32 geosearchResult_getDistance( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getDistance( resultHandle._genericHandle );
}
#else
#define geosearchResult_getDistance( resultHandle )\
       _geosearchResult_getDistance( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT32 _geosearchResult_getDetour( TGeosearchHandle genericResultHandle );
///
/// @brief Get the result detour distance (in metres), if any.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @return  The stored distance for a result, or zero if not applicable.
///
#if defined(__cplusplus)
inline UINT32 geosearchResult_getDetour( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getDetour( resultHandle._genericHandle );
}
#else
#define geosearchResult_getDetour( resultHandle )\
       _geosearchResult_getDetour( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINT32 _geosearchResult_getETA( TGeosearchHandle genericResultHandle );
///
/// @brief Get the result ETA (in seconds), if any.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @return  The stored ETA for a result, or zero if not applicable.
///
#if defined(__cplusplus)
inline UINT32 geosearchResult_getETA( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getETA( resultHandle._genericHandle );
}
#else
#define geosearchResult_getETA( resultHandle )\
       _geosearchResult_getETA( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchResult_getTelephoneNumber( TGeosearchHandle genericResultHandle,
                                           UTF16*           telephoneNumber );
///
/// @brief Get the result telephone number, if any.
///
/// @param[in]   resultHandle     A handle to a search result object.
///
/// @param[out]  telephoneNumber  The telephone number of a result as a formatted
///                               string.  It is assumed that the string buffer
///                               is of size GEOSEARCH_MAX_NAME_SIZE characters.
///
/// @return  A flag indicating whether or not a telephone number is available.
///
#if defined(__cplusplus)
inline BOOL8 geosearchResult_getTelephoneNumber( TGeosearchResultHandle resultHandle,
                                                 UTF16*                 telephoneNumber )
{
  return _geosearchResult_getTelephoneNumber( resultHandle._genericHandle,
                                              telephoneNumber );
}
#else
#define geosearchResult_getTelephoneNumber( resultHandle,                       \
                                            telephoneNumber )                   \
       _geosearchResult_getTelephoneNumber( (resultHandle)._genericResultHandle,\
                                            telephoneNumber )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchResult_getSpeedLimit( TGeosearchHandle genericResultHandle,
                                      UTF16*           speedLimit );
///
/// @brief Get the result speed limit, if any.
///
/// @param[in]   resultHandle     A handle to a search result object.
///
/// @param[out]  speedLimit       The speed limit of a result as a formatted
///                               string.  It is assumed that the string buffer
///                               is of size GEOSEARCH_MAX_NAME_SIZE characters.
///
/// @return  A flag indicating whether or not a speed limit is available.
///
#if defined(__cplusplus)
inline BOOL8 geosearchResult_getSpeedLimit( TGeosearchResultHandle resultHandle,
                                            UTF16*                 speedLimit )
{
  return _geosearchResult_getSpeedLimit( resultHandle._genericHandle,
                                         speedLimit );
}
#else
#define geosearchResult_getSpeedLimit( resultHandle,                       \
                                       speedLimit )                        \
       _geosearchResult_getSpeedLimit( (resultHandle)._genericResultHandle,\
                                       speedLimit )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchResult_getDisplayIconNames( TGeosearchHandle genericResultHandle,
                                            char*            iconNames );
///
/// @brief Get a prioritised list of display icon names separated by newlines,
///        if any, for a geosearch result.
///        Brand icon names will be prefixed with an @ character.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @param[out] iconNames         An output buffer (of size GEOSEARCH_MAX_NAME_SIZE)
///                               to store the the icon name list into.
///
/// @return  A flag indicating whether or not an icon name list is available.
///
#if defined(__cplusplus)
inline BOOL8 geosearchResult_getDisplayIconNames( TGeosearchResultHandle resultHandle,
                                                  char*                  iconNames )
{
  return _geosearchResult_getDisplayIconNames( resultHandle._genericHandle,
                                               iconNames );
}
#else
#define geosearchResult_getDisplayIconNames( resultHandle,                       \
                                             iconNames )                         \
       _geosearchResult_getDisplayIconNames( (resultHandle)._genericResultHandle,\
                                             iconNames )
#endif

//--------------------------------------------------------------------------------
/// @internal
TGeosearchPOITypeId _geosearchResult_getPOITypeId( TGeosearchHandle genericResultHandle );
///
/// @brief Get the POI type id of a POI type or user POI type result.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @param[out] typeName          An output buffer (of size GEOSEARCH_MAX_NAME_SIZE)
///                               to copy the type name into.
///
/// @return  The POI type id.
///
#if defined(__cplusplus)
inline TGeosearchPOITypeId geosearchResult_getPOITypeId( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getPOITypeId( resultHandle._genericHandle );
}
#else
#define geosearchResult_getPOITypeId( resultHandle )\
       _geosearchResult_getPOITypeId( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
TGeosearchPOITypeId _geosearchResult_getBrandTypeId( TGeosearchHandle genericResultHandle );
///
/// @brief Get the brand type id of a POI type result.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @return  The Brand type id.
///
#if defined(__cplusplus)
inline TGeosearchPOITypeId geosearchResult_getBrandTypeId( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getBrandTypeId( resultHandle._genericHandle );
}
#else
#define geosearchResult_getBrandTypeId( resultHandle )\
  _geosearchResult_getBrandTypeId( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
UINTP _geosearchResult_getUserPOI( TGeosearchHandle genericResultHandle );
///
/// @brief Get the result user POI value, if any.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @return  The user POI value, or zero if none.
///
#if defined(__cplusplus)
inline UINTP geosearchResult_getUserPOI( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getUserPOI( resultHandle._genericHandle );
}
#else
#define geosearchResult_getUserPOI( resultHandle )\
       _geosearchResult_getUserPOI( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchResult_getFuzzyMatchFlag( TGeosearchHandle genericResultHandle );
///
/// @brief Get the result fuzzy match flag.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @return  The fuzzy match flag.
///
#if defined(__cplusplus)
inline BOOL8 geosearchResult_getFuzzyMatchFlag( TGeosearchResultHandle resultHandle )
{
  return _geosearchResult_getFuzzyMatchFlag( resultHandle._genericHandle );
}
#else
#define geosearchResult_getFuzzyMatchFlag( resultHandle )\
       _geosearchResult_getFuzzyMatchFlag( (resultHandle)._genericResultHandle )
#endif

//--------------------------------------------------------------------------------
/// @internal
BOOL8 _geosearchResult_getMapLocation( TGeosearchHandle genericResultHandle,
                                       TMapLocation*    mapLocation );
///
/// @brief Get a map location for a result.
///
/// @param[in]  resultHandle      A handle to a search result object.
///
/// @param[out] mapLocation       A pointer to the output map location.
///
/// @return  A flag indicating whether or not a map location is
///          successfully obtained from the result.
///
#if defined(__cplusplus)
inline BOOL8 geosearchResult_getMapLocation( TGeosearchResultHandle resultHandle,
                                             TMapLocation*          mapLocation )
{
  return _geosearchResult_getMapLocation( resultHandle._genericHandle,
                                          mapLocation );
}
#else
#define geosearchResult_getMapLocation( resultHandle,                       \
                                        mapLocation )                       \
       _geosearchResult_getMapLocation( (resultHandle)._genericResultHandle,\
                                        mapLocation )
#endif

//for TAIWAN RDS
void GetPOIInfo(UINT32 poiID, char *mapName, POIInfo *info);
void GetPOIName(UINT32 poiID, char *mapName, POIInfo *info);

//for explore get address and phone number
void GetPOIInfoByIDandMapIdx(UINT32 poiID, UINT16 mapIdx, UTF16 *address, const UINT32 addressMaxlen, UTF16 *phoneNumber, const UINT32 phoneNumberMaxlen);

//for poi search by page //@TPE
UINT32 GetMiddleResultCount(TGeosearchQueryHandle queryHandle);

MiddlePOIResult GetPOIMiddleResultforPOISP(TGeosearchQueryHandle queryHandle, UINT32 index);

void GetResultNameforPOISP(UTF16 *poiName, UINT32 POINameIndex, UINT16 mapIdx);

void GetResultNameforFPS(UTF16 *poiName, UINT32 poiID, UINT16 mapIdx, UINT16 sectorID);

void GetAddressforPOISP(UTF16         *address, 
                        const UINT32   addressMaxlen, 
                        UINT32 poiID,  UINT16 mapIdx);

void GetTelephoneNumberforPOISP(UTF16*       phoneNumber, 
                                const UINT32 phoneNumberMaxlen, 
                                UINT32       poiID, 
                                UINT16       mapIdx);

void GetDisplayIconNamesforPOISP(char*  iconNames, 
                                 UINT32 poiID, 
                                 UINT16 typeSetIndex, 
                                 UINT16 mapIdx);

UINT16 GetPOITypeIdforPOISP(UINT16 typeSetIndex, UINT16 mapIdx);

void GetPosforPOISP(TPos* pos, UINT32 poiID, UINT16 mapIdx);

UINT32 GetPOIID(UINT16 mapIdx, UINT16 sectorID, UINT32 nameIdx);

BOOL8 GetMapLocationforPOISP(TMapLocation* mapLocation, 
                             UINT32        poiID, 
                             UINT16        mapIdx, 
                             UINT32        POINameIndex);

BOOL8 geosearchResult_getRoadOffset(TGeosearchResultHandle resultHandle, 
                                    UINT32 *roadOffset, UINT32 *mapIdx);

//Set the place index before execute intersection search if necessary.
//This API will use the position to find the place index where the position locate in.
//And search results will only contain the road in the special place.
void SetPlaceIdxforISSearch(TPos *pos);
void ResetPlaceIdx(void);
//--------------------------------------------------------------------------------
#if defined(__cplusplus)
} //extern "C"
#endif

#endif /*INCLUDE_GUARD_geosearch_h*/
