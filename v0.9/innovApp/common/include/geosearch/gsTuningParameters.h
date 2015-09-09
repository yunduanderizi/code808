/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_gsTuningParameters_h
#define INCLUDE_GUARD_gsTuningParameters_h

#include <geosearch/geosearchP.h>

namespace innov {
namespace geosearch {

//--------------------------------------------------------------------------------
//static const unsigned int TUNING_PARAMS_MAX_PLACE_COLUMN_MASKS = 4;
//
//static const unsigned int TUNING_PARAMS_MAX_ROAD_PREFERRED_DIFF_COLUMNS = 2;

//--------------------------------------------------------------------------------
//struct TNameFormatParameters {
//
//  /** This flag indicates whether or not to use abbreviations
//      in place names that appear anywhere within result
//      display names. */
//  BOOL8  abbreviatePlaceName;
//
//  /** This flag indicates whether or not to use abbreviations
//      in road names that appear anywhere within result
//      display names. */
//  BOOL8  abbreviateRoadName;
//
//  /** This flag indicates whether or not to use abbreviations
//      in POI names that appear anywhere within result
//      display names. */
//  BOOL8  abbreviatePOIName;
//};

//--------------------------------------------------------------------------------
struct TPlaceParameters {

  /** This points to an array of place column masks that define
      each level of the place hierarchy, from country downwards. */
  const TGeosearchPlaceColumnMask*  placeMergeColumns;

  /** This stores the number of levels in the place hierarchy. */
  UINT8                             placeMergeColumnsCount;

  /** This flag indicates if the place records have priority
      names P1 to P5 in the place columns, otherwise the place
      records have a true place hierarchy in the place columns. */
  BOOL8                             flagPlaceHasPriorityNames;

  /** This mask defines all the available columns to
      search in for places.
      The state column will be implicitly excluded for
      place searches. */
//TGeosearchPlaceColumnMask         placeAllowedSearchColumns;

  /** This mask defines which columns must have the
      same value (including null) in order for two place
      results to be mergeable. */
//TGeosearchPlaceColumnMask         placeMergeIfEqualColumns;    //TODO: change this to placeNextOrderColumn (not a mask)?  place merging will check all columns above directly (no cross-column comparisons).

  /** This flag indicates whether two place results
      which are candidates for merging (all criteria other
      than bounding boxes has found the results to be
      mergeable) should require the bounding boxes to overlap
      in order to merge.
      If this flag is not set, then such candidate results
      are always merged. */
//BOOL8                             placeRequiresBBoxOverlapToMerge;

  /** This mask indicates which columns should be considered
      for differentiating place results (not state results)
      by choosing the rightmost column in the mask that has
      a name (different from the place result name?), and
      which encompasses the place result.
      This may require a scan of place rows to determine
      which place encompasses the place result. */
//TGeosearchPlaceColumnMask         placeAllowedDiffColumns;
};

//--------------------------------------------------------------------------------
struct TPlaceSearchLocationParameters {

  /** This flag indicates whether or not a search within a
      place result should be a fuzzy (local) search. */
  BOOL8  flagFuzzySearch;
};

//--------------------------------------------------------------------------------
struct TPostalCodeSearchLocationParameters {

  /** This flag indicates whether or not a search within a
      postal code result should be a fuzzy (local) search. */
  BOOL8  flagFuzzySearch;
};

//--------------------------------------------------------------------------------
//struct TRoadSearchParameters {
//
//  /** This flag indicates whether or not a (base) road search
//      within a place result should be a fuzzy (local) search. */
//  BOOL8  placeFuzzyRoadSearch;
//
//  /** This flag indicates whether or not a (base) road search
//      within a postal code result should be a fuzzy (local)
//      search. */
//  BOOL8  postcodeFuzzyRoadSearch;
//};

//--------------------------------------------------------------------------------
//struct TPOISearchParameters {
//
//  /** This flag indicates whether or not a POI search within
//      a place result should be a fuzzy (local) search. */
//  BOOL8  placeFuzzyPOISearch;
//
//  /** This flag indicates whether or not a POI search within
//      a postal code result should be a fuzzy (local) search. */
//  BOOL8  postcodeFuzzyPOISearch;
//};

//--------------------------------------------------------------------------------
//struct THouseNumberSearchParameters {
//
//  /** This flag indicates whether or not a house number search
//      on a base road which has been found within a place search
//      should locate base road segments using a fuzzy (local)
//      search. */
//  BOOL8  placeFuzzyHouseNumberSearch;
//
//  /** This flag indicates whether or not a house number search
//      on a base road which has been found within a postal code
//      search should locate base road segments using a fuzzy
//      (local) search. */
//  BOOL8  postcodeFuzzyHouseNumberSearch;
//};

//--------------------------------------------------------------------------------
//struct TIntersectionSearchParameters {
//
//  /** This flag indicates whether or not an intersecting road
//      search on a base road which has been found within a place
//      search should locate base road segments using a fuzzy
//      (local) search. */
//  BOOL8  placeFuzzyIntersectionSearch;
//
//  /** This flag indicates whether or not an intersecting road
//      search on a base road which has been found within a postal
//      code search should locate base road segments using a fuzzy
//      (local) search. */
//  BOOL8  postcodeFuzzyIntersectionSearch;
//};

//--------------------------------------------------------------------------------
//struct TRoadResultParameters {
//
//  /** This set of masks is used to determine whether two road
//      results are mergable.  All names found in the set columns
//      of the mask in one name must be found in the same set of
//      columns in the other name.  However, the names may appear
//      in different columns in the two names.  Also, the names
//      may appear more than once in a particular set of columns,
//      and a different number of occurrences in each result. */
//  TGeosearchPlaceColumnMask  roadMergeIfEqualSets [TUNING_PARAMS_MAX_PLACE_COLUMN_MASKS];
//
//  /** These columns are checked first when selecting a place name
//      to differentiate road results. */
//  TGeosearchPlaceColumn      roadPreferredDiffColumn [TUNING_PARAMS_MAX_ROAD_PREFERRED_DIFF_COLUMNS];
//
//  /** This set of columns are checked after the preferred diff
//      columns have been checked, if no preferred name was found. */
//  TGeosearchPlaceColumnMask  roadAllowedDiffColumns;
//
//  /** This flag indicates whether or not to append the search area
//      filter name onto all exact-area matched road results that
//      are found by a search within an area. */
//  BOOL8                      roadApplyAreaFilterDiff;
//
//  /** This flag indicates whether or not to append the search area
//      postal code name onto all exact-area matched road results
//      that are found by a search within a postal code. */
//  BOOL8                      roadApplyPostcodeDiff;
//
//  /** This flag indicates whether or not to append the postal code
//      place adornment name after the postal code name in road results. */
//  BOOL8                      roadPostcodeWithAdornment;
//
//  /** This set of masks is used to determine whether two route
//      results are mergable.  All names found in the set columns
//      of the mask in one name must be found in the same set of
//      columns in the other name.  However, the names may appear
//      in different columns in the two names.  Also, the names
//      may appear more than once in a particular set of columns,
//      and a different number of occurrences in each result. */
//  TGeosearchPlaceColumnMask  routeMergeIfEqualSets [TUNING_PARAMS_MAX_PLACE_COLUMN_MASKS];
//
//  /** These columns are checked first when selecting a place name
//      to differentiate route results. */
//  TGeosearchPlaceColumn      routePreferredDiffColumn [TUNING_PARAMS_MAX_ROAD_PREFERRED_DIFF_COLUMNS];
//
//  /** This set of columns are checked after the preferred diff
//      columns have been checked, if no preferred name was found. */
//  TGeosearchPlaceColumnMask  routeAllowedDiffColumns;
//};

//--------------------------------------------------------------------------------
//struct THouseNumberResultParameters {
//
//  /** This flag indicates whether or not to append the base road name
//      onto house number search results. */
//  BOOL8  houseNumberApplyRoadNameDiff;
//};

//--------------------------------------------------------------------------------
//struct TIntersectionResultParameters {
//
//  /** This flag indicates whether or not to append the base road name
//      onto intersection search results. */
//  BOOL8   intersectionApplyRoadNameDiff;
//
//  /** This flag indicates whether or not an intersection search
//      requires junction bounding boxes to intersect in order to merge
//      results. */
//  BOOL8   intersectionRequiresBBoxOverlapToMerge;    //TODO: use this!
//
//  /** This value indicates the half size of the junction bounding box
//      of intersection searches. */
//  UINT16  intersectionNodeBBoxHalfSize;              //TODO: use this!   //TODO: should this be scaled?
//};

//--------------------------------------------------------------------------------
//struct TFindLocationParameters {
//
//  /** The value indicates the extension half size to apply in the
//      horizontal direction when calculating bounding boxes around
//      road result segments. */
//  UINT16  roadBoundsOuterExtendHalfSizeX;
//
//  /** This value indicates the extension half size to apply in the
//      vertical direction when calculating bounding boxes around
//      road result segments. */
//  UINT16  roadBoundsOuterExtendHalfSizeY;
//};

//--------------------------------------------------------------------------------
struct TTuningParameters {

  /** This points to a structure with parameters to configure
      the formatting of names within result display names. */
//const TNameFormatParameters*               nameFormatParameters;

  /** This points to a structure with parameters to configure
      interactions with place records and place entities. */
  const TPlaceParameters*                    placeParameters;

  /** This points to a structure with parameters to configure
      features of a search within a place result. */
  const TPlaceSearchLocationParameters*      placeSearchLocationParameters;

  /** This points to a structure with parameters to configure
      features of a search within a postal code result. */
  const TPostalCodeSearchLocationParameters* postalCodeSearchLocationParameters;

  /** This points to a structure with parameters to configure
      features of a (base) road search. */
//const TRoadSearchParameters*               roadSearchParameters;

  /** This points to a structure with parameters to configure
      features of a POI search. */
//const TPOISearchParameters*                POISearchParameters;

  /** This points to a structure with parameters to configure
      features of a house number search on a base road. */
//const THouseNumberSearchParameters*        houseNumberSearchParameters;

  /** This points to a structure with parameters to configure
      features of an intersection search on a base road. */
//const TIntersectionSearchParameters*       intersectionSearchParameters;

  /** This points to a structure with parameters to configure
      road (or route) results. */
//const TRoadResultParameters*               roadResultParameters;

  /** This points to a structure with parameters to configure
      house number results. */
//const THouseNumberResultParameters*        houseNumberResultParameters;

  /** This points to a structure with parameters to configure
      intersection results. */
//const TIntersectionResultParameters*       intersectionResultParameters;

  /** This points to a structure with parameters to configure
      alogorithms that find locations. */
//const TFindLocationParameters*             findLocationParameters;
};

//--------------------------------------------------------------------------------
extern BOOL8 g_flagResetDefaultTuningParameters;

//--------------------------------------------------------------------------------
const TTuningParameters* getTuningParameters( TNumericISOCountryCode numericISOCountryCode
                                                                     = (TNumericISOCountryCode)0 );

//--------------------------------------------------------------------------------
} //namespace geosearch
} //namespace innov

#endif //INCLUDE_GUARD_gsTuningParameters_h
