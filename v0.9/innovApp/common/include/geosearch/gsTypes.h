/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_gsTypes_h
#define INCLUDE_GUARD_gsTypes_h

#include <geosearch/geosearchP.h>
#include <geosearch/gsTuningParameters.h>
#include <geosearch/userPOISearch_if.h>
#include <language/utf16.h>
#include <mapdata/enpocodeP.h>
#include <mapdata/enpoiP.h>
#include <mapdata/enpoitypeP.h>
#include <mapdata/enroadP.h>
#include <mapdata/entokenP.h>
#include <memory/allocator.h>
#include <parser/parserP.h>
#include <query/fwd.h>
#include <query/object.h>
#include <routing/rtjourney.h>
#include <routing/rtpathcalcP.h>
#include <stddef.h>

namespace innov {
namespace geosearch {

//--------------------------------------------------------------------------------
extern const UTF16      LAT_LONG_RESULT_SORT_KEY_TAG[];
extern const UTF16      POI_TYPE_RESULT_SORT_KEY_TAG[];
extern const UTF16 USER_POI_TYPE_RESULT_SORT_KEY_TAG[];
extern const UTF16          ROAD_RESULT_SORT_KEY_TAG[];
extern const UTF16           POI_RESULT_SORT_KEY_TAG[];
extern const UTF16      USER_POI_RESULT_SORT_KEY_TAG[];
extern const UTF16  HOUSE_NUMBER_RESULT_SORT_KEY_TAG[];
extern const UTF16  INTERSECTION_RESULT_SORT_KEY_TAG[];
extern const UTF16         PLACE_RESULT_SORT_KEY_TAG[];
extern const UTF16   POSTAL_CODE_RESULT_SORT_KEY_TAG[];

//--------------------------------------------------------------------------------
extern const UTF16      NEAREST_POI_RESULT_SORT_KEY_TAG[];
extern const UTF16 NEAREST_USER_POI_RESULT_SORT_KEY_TAG[];
extern const UTF16       ACTIVE_POI_RESULT_SORT_KEY_TAG[];
extern const UTF16  ACTIVE_USER_POI_RESULT_SORT_KEY_TAG[];

//--------------------------------------------------------------------------------
typedef UINT32 TMapFilter [((MAPINFO_MAX_ROAD_MAPS) + 31) >> 5];

//--------------------------------------------------------------------------------
static const UINT32 POI_RESULT_HOUSE_NUMBER_THRESHOLD_SCALE = 2;

//--------------------------------------------------------------------------------
struct TPocodeDataSource {
  BOOL8 detailed;
  union {
    TGenericMapInfo*   roadMapInfo;
    TGenericMapInfo* pocodeMapInfo;
  };
};

//--------------------------------------------------------------------------------
struct TKeywordCode {
  UTF16   keyword;
  UINT32  maskHigh;
  UINT32  masklow;
};

//--------------------------------------------------------------------------------
struct TDetailKeyword {
  UINT8        count;
  TKeywordCode *keywordAndMask;
};

//--------------------------------------------------------------------------------
struct TKeywords {
  IAllocator*      allocator;
  UINT8            count;
  TDetailKeyword*  keywordDetail;
};

//--------------------------------------------------------------------------------
typedef UINT32
THouseNumber32;

static const THouseNumber32
HOUSE_NUMBER_MAX = 999999;

//--------------------------------------------------------------------------------
typedef UINT8
TNameCost8;

static const TNameCost8
NAME_COST_MAX = 0xFF;

static const TNameCost8
NAME_COST_CASE_CONVERSION = 0x00;    // NOTE we do not have a cost for converting case.

static const TNameCost8
NAME_COST_ACCENT_CONVERSION = 0x00;  // NOTE we do not have a cost for converting accents..

static const TNameCost8
NAME_COST_ACCENT_MISMATCH = 0x01;    // .. but we do have a cost for mismatching accents.

static const TNameCost8
NAME_COST_SEPARATOR_MISMATCH = 0x01;

static const TNameCost8
NAME_COST_IN_ORDER_NO_GAPS_MATCH_PRIMARY_TOKEN = 0x00;

static const TNameCost8
NAME_COST_IN_ORDER_NO_GAPS_NO_MATCH_PRIMARY_TOKEN = 0x08;

static const TNameCost8
NAME_COST_IN_ORDER_WITH_GAPS = /*0x10*/0x50;

static const TNameCost8
NAME_COST_OUT_OF_ORDER = 0xFF/*0x18*/; //@TPE

static const TNameCost8
NAME_COST_ALT_TOKEN_0 = 0x00;

static const TNameCost8
NAME_COST_ALT_TOKEN_1 = 0x04;

static const TNameCost8
NAME_COST_ALT_TOKEN_2 = 0x20;

static const TNameCost8
NAME_COST_ALT_TOKEN_3 = 0x40;

inline TNameCost8
addNameCosts( TNameCost8 nameCost1,
              TNameCost8 nameCost2 )
{
  UINT16 newNameCost = (nameCost1 + nameCost2);
  if ( newNameCost > NAME_COST_MAX ) {
    newNameCost = NAME_COST_MAX;
  }
  return (TNameCost8)newNameCost;
}

//--------------------------------------------------------------------------------
typedef UINT32
TResultCost32;

static const TResultCost32
RESULT_COST_NON_P1_AREA = 0x00000020;

static const TResultCost32
RESULT_COST_FUZZY_AREA = 0x80000000;

inline TResultCost32
addResultCosts( TResultCost32 resultCost1,
                TResultCost32 resultCost2 )
{
  return (resultCost1 + resultCost2);
}

//--------------------------------------------------------------------------------
typedef UINT32
TTokenPartIndex32;
//TODO we can reduce this size down to 16 bits.
//     currently the map data only uses 8 bits.

struct TTokenPart {
  TTokenPartIndex32 tokenPartIndex;
  TNameCost8        nameCost;
  BOOL8             flagPartialMatch;
  UINT8             fullLength;
  UINT8             matchLength;
};

//--------------------------------------------------------------------------------
typedef UINT32
TTokenIndex32;
//TODO is it possible to reduce this down to 16 bits?
//     - probably not!

struct TToken {
  TTokenIndex32 tokenIndex;
  TTokenIndex32 matchedTokenIndex;
  TNameCost8    nameCost;
  BOOL8         flagPartialMatch;
  BOOL8         flagAlternateMatch; //TODO We can calculate this by comparing tokenIndex and matchedTokenIndex.
  UINT8         matchLength;
};

//--------------------------------------------------------------------------------
typedef UINT32
TNameIndex32;

struct TName {
  TNameIndex32      nameIndex;
  TNameCost8        nameCost;
  UINT32            matchedTokenPositions;
  TTokenHighlight   matchedTokens[TOKEN_MAX_HIGHLIGHTS];
                    //NOTE each low 8 bits is matchLength,
                    //     each high 24 bits is tokenIndex.
};

//--------------------------------------------------------------------------------
typedef UINT32
TPostalCodeIndex32;

struct TPostalCode {
  TPostalCodeIndex32 postalCodeIndex;
  BOOL8              flagDetailed;
  UINT8              matchLength;
  TNameCost8         nameCost;
};

//--------------------------------------------------------------------------------
struct TDetailedPostalCode {
  UINT16 pocodeMapIndex;
  UINT16 mainPocodeIndex;
  UINT16 subPocodeIndex;
};

//--------------------------------------------------------------------------------
enum TCoverageType {
  COVERAGE_TYPE_IN,
  COVERAGE_TYPE_THRU,
  COVERAGE_TYPE_NEAR
};

typedef UINT8
TCoverageType8;

//--------------------------------------------------------------------------------
enum TSearchAreaType {
  SEARCH_AREA_TYPE_PLACE,
  SEARCH_AREA_TYPE_POSTAL_CODE,
  SEARCH_AREA_TYPE_DETAILED_POSTAL_CODE,
  SEARCH_AREA_TYPE_ANONYMOUS_BOUNDS
};

typedef UINT8
TSearchAreaType8;

//--------------------------------------------------------------------------------
struct TSearchAreaPlace {
  UINT16                    roadMapIndex;
  TRoadPlaceNameIndex16     placeNameIndex;
  TGeosearchPlaceColumnMask placeColumns;
  TGeosearchPlaceColumnMask placeMergeColumns;
};

//--------------------------------------------------------------------------------
struct TSearchAreaPostalCode {
  UINT16                 roadMapIndex;
  TRoadPostalCodeIndex16 postalCodeIndex;
};

//--------------------------------------------------------------------------------
struct TSearchArea {
  TSearchAreaType8        type;
  union {
    TSearchAreaPlace      place;
    TSearchAreaPostalCode postalCode;
    TDetailedPostalCode   detailedPostalCode;
    TRect                 anonymousBounds;
  };
};

//--------------------------------------------------------------------------------
struct TSearchAreaMatch {
  TSearchArea    searchArea;
  TCoverageType8 coverageType;
};

//--------------------------------------------------------------------------------
struct TPlaceQualifier {
  TName                     placeName;
  TGeosearchPlaceColumnMask placeColumns;
  TGeosearchPlaceColumnMask placeMergeColumns;
  TCoverageType8            coverageType;
};

//--------------------------------------------------------------------------------
struct TGlobalPlaceQualifier {
  UINT16          roadMapIndex;
  TPlaceQualifier localPlaceQualifier;
};

//--------------------------------------------------------------------------------
enum TAreaMatchType {
  AREA_MATCH_TYPE_EXACT,
  AREA_MATCH_TYPE_NON_P1,
  AREA_MATCH_TYPE_FUZZY,
  AREA_MATCH_TYPE_MAX
};

typedef UINT8
TAreaMatchType8;

//--------------------------------------------------------------------------------
struct TAreaMatchTypePair {
  TAreaMatchType8 leftAreaMatchType;
  TAreaMatchType8 rightAreaMatchType;
};

//--------------------------------------------------------------------------------
enum TResultCostMode {
  RESULT_COST_MODE_EXACT,
  RESULT_COST_MODE_NON_EXACT,
  RESULT_COST_MODE_ANY
};

typedef UINT8
TResultCostMode8;

//--------------------------------------------------------------------------------
struct TLocalPlace {
  TRoadPlace                place;
  TGeosearchPlaceColumnMask placeColumns;
  TGeosearchPlaceColumnMask parentPlaceColumns;
};

struct TGlobalPlace {
  UINT16      roadMapIndex;
  TLocalPlace localPlace;
};

//--------------------------------------------------------------------------------
struct TGlobalPlaceLink {
  UINT16            roadMapIndex;
  TRoadPlaceIndex16 placeIndex;
};

//--------------------------------------------------------------------------------
struct TGlobalSectorLink {
  UINT16             roadMapIndex;
  TRoadSectorIndex16 sectorIndex;
};

//--------------------------------------------------------------------------------
struct TQueryProgress {
  UINT32 numerator;
  UINT32 denominator;
};
//--------------------------------------------------------------------------------
struct TTempSegmentRecord{
  UINT16 count;
  TAreaMatchType areaMatchType[200];
  TRoadSegment segment[200];
};
//--------------------------------------------------------------------------------
class IStructuredText : public query::IObject {

public:
  const UTF16*
  getString();

  UINT32
  getRangeCount();

  const TGeosearchTextRange*
  getRanges();

private:
  virtual const UTF16*
  doGetString() = 0;

  virtual UINT32
  doGetRangeCount() = 0;

  virtual const TGeosearchTextRange*
  doGetRanges() = 0;
};

//--------------------------------------------------------------------------------
class IStructuredTextInit : public IStructuredText {

public:
  //NOTE this is a helper function only,
  //     so no corresponding method.
  query::TStatus
  appendString( const UTF16*       string,
                TGeosearchTextType textType = GEOSEARCH_TEXT_TYPE_INVALID,
                UINT32             highlightLength = 0,
                UINT8              highlightType = 0 );

  //NOTE this is a helper function only,
  //     so no corresponding method.
  query::TStatus
  appendName( const UTF16 (&nameString)[GEOSEARCH_MAX_NAME_SIZE],
              const UINT8 (&nameHighlightTypes)[TOKEN_MAX_HIGHLIGHTS],
              const UINT8 (&nameHighlightStarts)[TOKEN_MAX_HIGHLIGHTS],
              const UINT8 (&nameHighlightEnds)[TOKEN_MAX_HIGHLIGHTS],
              TGeosearchTextType textType = GEOSEARCH_TEXT_TYPE_SIMPLE_NAME );

  query::TStatus
  appendText( const UTF16* text,
              UINT32       textLength );

  query::TStatus
  beginRange( TGeosearchTextType textType );

  query::TStatus
  endRange();

private:
  virtual query::TStatus
  doAppendText( const UTF16* text,
                UINT32       textLength ) = 0;

  virtual query::TStatus
  doBeginRange( TGeosearchTextType textType ) = 0;

  virtual query::TStatus
  doEndRange() = 0;
};

//--------------------------------------------------------------------------------
// Forward declaration.
class ISearchResult;

//--------------------------------------------------------------------------------
class IResultList : public query::IObject {

public:
  void
  getResults( ISearchResult*const** results,
              UINT32*               resultCount );

private:
  virtual void
  doGetResults( ISearchResult*const*& results,
                UINT32&               resultCount ) = 0;
};

//--------------------------------------------------------------------------------
class ISearchQuery : public query::IObject {

public:
  query::TStatus
  executeQuery();

  UINT8
  getQueryProgress();

  query::TStatus
  getQueryStatus();

  query::TStatus
  getResultList( IResultList** resultList );

  UINT32//@TPE
  getMiddleResultCount();

  BOOL8//@TPE
  getMiddleResult(int index, UINT32 &POINameIndex, UINT16 &mapIdx, TResultCost32 &resultCost, UINT16 &typeSetIndex, UINT16 &sectorID);

private:
  virtual query::TStatus
  doExecuteQuery() = 0;

  virtual UINT8
  doGetQueryProgress() = 0;

  virtual query::TStatus
  doGetQueryStatus() = 0;

  virtual query::TStatus
  doGetResultList( IResultList*& resultList ) = 0;

  virtual UINT32//@TPE
  doGetMiddleResultCount() = 0;

  virtual BOOL8//@TPE
  doGetMiddleResult(int index, UINT32 &POINameIndex, UINT16 &mapIdx, TResultCost32 &resultCost, UINT16 &typeSetIndex, UINT16 &sectorID) = 0;
};

//--------------------------------------------------------------------------------
class ISearchResult : public query::IObject {

public:
  TGeosearchResultType
  getResultType();

  IStructuredText*
  getResultName();

  query::TStatus
  getLocationName( IStructuredText** locationNameText );

  TNumericISOCountryCode
  getNumericISOCountryCode();

  UINT32
  getHouseNumber();

  BOOL8
  getHouseNumberRangeType( BOOL8* flagMixed );

  TGeosearchResultType
  getSearchStringMatchType();

  UINT32
  getDistance();

  UINT32
  getDetour();

  UINT32 
  getETA();  

  BOOL8
  getTelephoneNumber( UTF16* telephoneNumber );

  BOOL8
  getSpeedLimit( UTF16* speedLimit );

  BOOL8
  getDisplayIconNames( char* iconNames );

  TGeosearchPOITypeId
  getPOITypeId();

  TGeosearchPOITypeId
  getBrandTypeId();

  UINTP
  getUserPOI();

  BOOL8
  getFuzzyMatchFlag();

  BOOL8
  getMapLocation( TMapLocation* mapLocation );

private:
  virtual TGeosearchResultType
  doGetResultType() = 0;

  virtual IStructuredText*
  doGetResultName() = 0;

  virtual query::TStatus
  doGetLocationName( IStructuredText*& locationNameText ) = 0;

  virtual TNumericISOCountryCode
  doGetNumericISOCountryCode() = 0;

  virtual UINT32
  doGetHouseNumber() = 0;

  virtual BOOL8
  doGetHouseNumberRangeType( BOOL8& flagMixed ) = 0;

  virtual TGeosearchResultType
  doGetSearchStringMatchType() = 0;

  virtual UINT32
  doGetDistance() = 0;

  virtual UINT32
  doGetDetour() = 0;

  virtual UINT32
  doGetETA() = 0;  

  virtual BOOL8
  doGetTelephoneNumber( UTF16* telephoneNumber ) = 0;

  virtual BOOL8
  doGetSpeedLimit( UTF16* speedLimit ) = 0;

  virtual BOOL8
  doGetDisplayIconNames( char* iconNames ) = 0;

  virtual TGeosearchPOITypeId
  doGetPOITypeId() = 0;

  virtual TGeosearchPOITypeId
  doGetBrandTypeId() = 0;

  virtual UINTP
  doGetUserPOI() = 0;

  virtual BOOL8
  doGetFuzzyMatchFlag() = 0;

  virtual BOOL8
  doGetMapLocation( TMapLocation& mapLocation ) = 0;
};

//--------------------------------------------------------------------------------
class IResultCountLowBoundMutator : public query::IObject {

public:
  query::TStatus
  setResultCountLowBound( const UTF16* resultSortKey,
                          UINT32       resultCountLowBound );

private:
  virtual query::TStatus
  doSetResultCountLowBound( const UTF16* resultSortKey,
                            UINT32       resultCountLowBound ) = 0;
};

//--------------------------------------------------------------------------------
class IFinalisedResultGroup : public query::IObject {

public:
  query::TStatus
  getResultCount( const UTF16* resultSortKey,
                  UINT32*      resultCount );

  query::TStatus
  getResults( const UTF16*    resultSortKey,
              UINT32          maxResultCount,
              UINT32*         resultCount,
              ISearchResult** results );

private:
  virtual query::TStatus
  doGetResultCount( const UTF16* resultSortKey,
                    UINT32&      resultCount ) = 0;

  virtual query::TStatus
  doGetResults( const UTF16*   resultSortKey,
                UINT32         maxResultCount,
                UINT32&        resultCount,
                ISearchResult* results[] ) = 0;
};

//--------------------------------------------------------------------------------
class IResultGroupFinaliserCallback : public query::IObject {

public:
  query::TStatus
  finaliseDone( IFinalisedResultGroup* finalisedResultGroup,
                query::IEngine**       engine );

private:
  virtual query::TStatus
  onFinaliseDone( IFinalisedResultGroup* finalisedResultGroup,
                  query::IEngine*&       engine ) = 0;
};

//--------------------------------------------------------------------------------
class IResultGroup : public query::IObject {

public:
  const UTF16*
  getResultGroupKey();

  query::TStatus
  attachResultSortKey( const UTF16* resultSortKey );

  void
  detachResultSortKey( const UTF16* resultSortKey );

  query::TStatus
  insertResult( query::IObject*              result,
                const UTF16*                 resultSortKey,
                IResultCountLowBoundMutator* resultCountLowBoundMutator );

  query::TStatus
  makeFinaliserEngine( UINT32                         significantResultCount,
                       IResultGroupFinaliserCallback* finaliserCallback,
                       query::IEngine**               finaliserEngine );

private:
  virtual const UTF16*
  doGetResultGroupKey() = 0;

  virtual query::TStatus
  doAttachResultSortKey( const UTF16* resultSortKey ) = 0;

  virtual void
  doDetachResultSortKey( const UTF16* resultSortKey ) = 0;

  virtual query::TStatus
  doInsertResult( query::IObject*              result,
                  const UTF16*                 resultSortKey,
                  IResultCountLowBoundMutator* resultCountLowBoundMutator ) = 0;

  virtual query::TStatus
  doMakeFinaliserEngine( UINT32                         significantResultCount,
                         IResultGroupFinaliserCallback* finaliserCallback,
                         query::IEngine*&               finaliserEngine ) = 0;
};

//--------------------------------------------------------------------------------
class IResultGroupCache : public query::IObject {

public:
  query::TStatus
  getResultGroup( const UTF16*   resultGroupKey,
                  BOOL8          flagNoCreate,
                  IResultGroup** resultGroup );

  void
  removeResultGroup( const UTF16* resultGroupKey );

private:
  virtual query::TStatus
  doGetResultGroup( const UTF16*   resultGroupKey,
                    BOOL8          flagNoCreate,
                    IResultGroup*& resultGroup ) = 0;

  virtual void
  doRemoveResultGroup( const UTF16* resultGroupKey ) = 0;
};

//--------------------------------------------------------------------------------
class IResultGroupFactory : public query::IObject {

public:
  query::TStatus
  makeResultGroup( const UTF16*       resultGroupKey,
                   IResultGroupCache* resultGroupCache,
                   IResultGroup**     resultGroup );

private:
  virtual query::TStatus
  doMakeResultGroup( const UTF16*       resultGroupKey,
                     IResultGroupCache* resultGroupCache,
                     IResultGroup*&     resultGroup ) = 0;
};
//--------------------------------------------------------------------------------
class IPOINameInstanceCursor : public query::IObject {

public:
  typedef ::TPOINameInstanceResult
  TPOIInstance;

public:
  query::TStatus
  getPOIInstance( TPOIInstance* POIInstance );

private:
  virtual query::TStatus
  doGetPOIInstance( TPOIInstance& POIInstance ) = 0;
};
//--------------------------------------------------------------------------------
class IResultListBuilder : public query::IObject {

public:
  query::TStatus
  insertResult( query::IObject*    result,
                const UTF16*       resultSortKey,
                IResultGroupCache* resultGroupCache,
                const UTF16*       resultGroupKey );

  BOOL8
  getFlagListFull();

  query::TStatus
  getResultList( IResultList** resultList );

  query::TStatus//@TPE
  insertMiddleResult( UINT32 POINameIndex, 
                      UINT16 mapIdx, 
                      TResultCost32 resultCost, 
                      UINT16 typeSetIndex, 
                      UINT16 sectorID );

  UINT32//@TPE
  getMiddleCount();

  BOOL8//@TPE
  getMiddle(int index, UINT32 &POINameIndex, UINT16 &mapIdx, TResultCost32 &resultCost, UINT16 &typeSetIndex, UINT16 &sectorID);

private:
  virtual query::TStatus
  doInsertResult( query::IObject*    result,
                  const UTF16*       resultSortKey,
                  IResultGroupCache* resultGroupCache,
                  const UTF16*       resultGroupKey ) = 0;

  virtual BOOL8
  doGetFlagListFull() = 0;

  virtual query::TStatus
  doGetResultList( IResultList*& resultList ) = 0;

  virtual query::TStatus//@TPE
  doInsertMiddleResult( UINT32 POINameIndex, 
                        UINT16 mapIdx, 
                        TResultCost32 resultCost, 
                        UINT16 typeSetIndex, 
                        UINT16 sectorID ) = 0;

  virtual UINT32//@TPE
  doGetMiddleCount() = 0;

  virtual BOOL8//@TPE
  doGetMiddle(int index, UINT32 &POINameIndex, UINT16 &mapIdx, TResultCost32 &resultCost, UINT16 &typeSetIndex, UINT16 &sectorID) = 0;;
};

//--------------------------------------------------------------------------------
class IResultNameFactory : public query::IObject {

public:
  query::TStatus
  makeResultName( const TSearchAreaMatch*      searchAreaMatch,
                  const TGlobalPlaceQualifier* placeQualifier,
                  IStructuredText**            resultName );

private:
  virtual query::TStatus
  doMakeResultName( const TSearchAreaMatch*      searchAreaMatch,
                    const TGlobalPlaceQualifier* placeQualifier,
                    IStructuredText*&            resultName ) = 0;
};

//--------------------------------------------------------------------------------
class IResultNameFactoryEntityStrategy : public query::IObject {

public:
  const TGlobalPlace*
  getGlobalPlaces( UINT32* globalPlaceCount );

  BOOL8
  getEntityAdornmentPlace( TRoadPlaceNameIndex16* adornmentNameIndex,
                           UINT16*                adornmentRoadMapIndex,
                           TRect*                 entityBounds );

  BOOL8
  getEntityDiffRoadName( UTF16* identityRoadName,
                         UTF16* displayRoadName );

  query::TStatus
  getAppendedEntityName( IStructuredTextInit* resultNameInit );

private:
  virtual const TGlobalPlace*
  doGetGlobalPlaces( UINT32& globalPlaceCount ) = 0;

  virtual BOOL8
  doGetEntityAdornmentPlace( TRoadPlaceNameIndex16& adornmentNameIndex,
                             UINT16&                adornmentRoadMapIndex,
                             TRect&                 entityBounds ) = 0;

  virtual BOOL8
  doGetEntityDiffRoadName( UTF16* identityRoadName,
                           UTF16* displayRoadName ) = 0;

  virtual query::TStatus
  doGetAppendedEntityName( IStructuredTextInit* resultNameInit ) = 0;
};

//--------------------------------------------------------------------------------
class IResultNameFactoryFactoryCallback : public query::IObject {

public:
  query::TStatus
  resultNameFactoryMakeDone( UINT32                    resultNameFactoryCount,
                             IResultNameFactory*const* resultNameFactories,
                             const UINT32*             resultNameFactoryEntityIndices,
                             query::IEngine**          engine );

private:
  virtual query::TStatus
  onResultNameFactoryMakeDone( UINT32                    resultNameFactoryCount,
                               IResultNameFactory*const* resultNameFactories,
                               const UINT32*             resultNameFactoryEntityIndices,
                               query::IEngine*&          engine ) = 0;
};

//--------------------------------------------------------------------------------
class INameDataSource : public query::IObject {

public:
  const TTokenDataSource*
  getDataSource();

private:
  virtual const TTokenDataSource*
  doGetDataSource() = 0;
};

//--------------------------------------------------------------------------------
class ISearchFilter : public query::IObject {

public:
  const TBitset*
  getFilterBitset();

private:
  virtual const TBitset*
  doGetFilterBitset() = 0;
};

//--------------------------------------------------------------------------------
class IPlaceFilter : public query::IObject {

public:
  TAreaMatchType
  getAreaMatchType( TRoadSectorIndex16 sectorIndex,
                    TRoadPlaceIndex16  placeIndex );

  BOOL8
  getPlaceMatchFlag( TRoadPlaceIndex16 placeIndex );

  BOOL8
  getSectorMatchFlag( TRoadSectorIndex16 sectorIndex );

  BOOL8
  getPlaceBoundsMatchFlag( TRoadPlaceIndex16 placeIndex,
                           const TRect&      placeBounds );

  query::TStatus
  getRoadNameOrRouteNameFilter( ISearchFilter** nameFilter );

  query::TStatus
  getRoadFilter( ISearchFilter** roadFilter );

  const TSearchArea*
  getSearchArea();

private:
  virtual TAreaMatchType
  doGetAreaMatchType( TRoadSectorIndex16 sectorIndex,
                      TRoadPlaceIndex16  placeIndex ) = 0;

  virtual BOOL8
  doGetPlaceMatchFlag( TRoadPlaceIndex16 placeIndex ) = 0;

  virtual BOOL8
  doGetSectorMatchFlag( TRoadSectorIndex16 sectorIndex ) = 0;

  virtual BOOL8
  doGetPlaceBoundsMatchFlag( TRoadPlaceIndex16 placeIndex,
                             const TRect&      placeBounds ) = 0;

  virtual query::TStatus
  doGetRoadNameOrRouteNameFilter( ISearchFilter*& nameFilter ) = 0;

  virtual query::TStatus
  doGetRoadFilter( ISearchFilter*& roadFilter ) = 0;

  virtual const TSearchArea*
  doGetSearchArea() = 0;
};

//--------------------------------------------------------------------------------
class IPOIPlaceFilter : public query::IObject {

public:
  TAreaMatchType
  getAreaMatchType( TPOISectorIndex16 POISectorIndex,
                    TRoadPlaceIndex16 placeIndex );

  BOOL8
  getSectorMatchFlag( TPOISectorIndex16 POISectorIndex );

  BOOL8
  getFlagPartialPlaceFilter();

  query::TStatus
  getPOINameFilter( ISearchFilter** nameFilter );

  query::TStatus
  getPOINameInstanceCursor( UINT32                   POINameIndex,
                            IPOINameInstanceCursor** POINameInstanceCursor );

  const TSearchArea*
  getSearchArea();

private:
  virtual TAreaMatchType
  doGetAreaMatchType( TPOISectorIndex16 POISectorIndex,
                      TRoadPlaceIndex16 placeIndex ) = 0;

  virtual BOOL8
  doGetSectorMatchFlag( TPOISectorIndex16 POISectorIndex ) = 0;

  virtual BOOL8
  doGetFlagPartialPlaceFilter() = 0;

  virtual query::TStatus
  doGetPOINameFilter( ISearchFilter*& nameFilter ) = 0;

  virtual query::TStatus
  doGetPOINameInstanceCursor( UINT32                   POINameIndex,
                              IPOINameInstanceCursor*& POINameInstanceCursor ) = 0;

  virtual const TSearchArea*
  doGetSearchArea() = 0;
};

//--------------------------------------------------------------------------------
class IPostalCodeEntity : public query::IObject {

public:
  const TGlobalPlace*
  getGlobalPlaces( UINT32* globalPlaceCount );

  const TRect*
  getPlaceBounds();

  BOOL8
  getFlagNoRoads();

private:
  virtual const TGlobalPlace*
  doGetGlobalPlaces( UINT32& globalPlaceCount ) = 0;

  virtual const TRect*
  doGetPlaceBounds() = 0;

  virtual BOOL8
  doGetFlagNoRoads() = 0;
};

//--------------------------------------------------------------------------------
class IDetailedPostalCodeEntity : public query::IObject {

public:
  const TDetailedPostalCode*
  getDetailedPostalCodes( UINT32* detailedPostalCodeCount );

private:
  virtual const TDetailedPostalCode*
  doGetDetailedPostalCodes( UINT32& detailedPostalCodeCount ) = 0;
};

//--------------------------------------------------------------------------------
class IPlaceEntity : public query::IObject {

public:
  const TGlobalPlace*
  getGlobalPlaces( UINT32* globalPlaceCount );

  TGeosearchPlaceColumnMask
  getPlaceColumns();

  TGeosearchPlaceColumnMask
  getPlaceMergeColumns(); //TODO this function is not being used.

  const TRect*
  getPlaceBounds();

  BOOL8
  getFlagNoRoads();

  BOOL8
  getPlaceNamePlaceMatchFlag( UINT16 roadMapIndex,
                              UINT16 placeNamePlaceRecordIndex );

private:
  virtual const TGlobalPlace*
  doGetGlobalPlaces( UINT32& globalPlaceCount ) = 0;

  virtual TGeosearchPlaceColumnMask
  doGetPlaceColumns() = 0;

  virtual TGeosearchPlaceColumnMask
  doGetPlaceMergeColumns() = 0;

  virtual const TRect*
  doGetPlaceBounds() = 0;

  virtual BOOL8
  doGetFlagNoRoads() = 0;

  virtual BOOL8
  doGetPlaceNamePlaceMatchFlag( UINT16 roadMapIndex,
                                UINT16 placeNamePlaceRecordIndex ) = 0;
};

//--------------------------------------------------------------------------------
class IPlaceEntityFactoryCallback : public query::IObject {

public:
  query::TStatus
  placeEntityMakeDone( IPlaceEntity*const* placeEntities,
                       UINT32              placeEntityCount,
                       query::IEngine**    engine );

private:
  virtual query::TStatus
  onPlaceEntityMakeDone( IPlaceEntity*const* placeEntities,
                         UINT32              placeEntityCount,
                         query::IEngine*&    engine ) = 0;
};

//--------------------------------------------------------------------------------
class IPlaceEntityCache : public query::IObject {

public:
  query::TStatus
  getPlaceEntities( const UTF16*                 placeIdentityName,
                    IPlaceEntity*const**         placeEntities,
                    UINT32*                      placeEntityCount,
                    IPlaceEntityFactoryCallback* factoryCallback,
                    query::IEngine**             factoryEngine );

private:
  virtual query::TStatus
  doGetPlaceEntities( const UTF16*                 placeIdentityName,
                      IPlaceEntity*const*&         placeEntities,
                      UINT32&                      placeEntityCount,
                      IPlaceEntityFactoryCallback* factoryCallback,
                      query::IEngine*&             factoryEngine ) = 0;
};

//--------------------------------------------------------------------------------
class IRoadEntity : public query::IObject {

public:
  struct TLocalRoad {
    UINT16           roadMapIndex;
    TRoadRoadIndex32 roadIndex;
  };

public:
  const TLocalRoad*
  getLocalRoads( UINT32* localRoadCount );

  const TRect*
  getRoadBounds();

private:
  virtual const TLocalRoad*
  doGetLocalRoads( UINT32& localRoadCount ) = 0;

  virtual const TRect*
  doGetRoadBounds() = 0;
};

//--------------------------------------------------------------------------------
class IRoadEntityFactoryCallback : public query::IObject {

public:
  query::TStatus
  roadEntityMakeDone( IRoadEntity*const* roadEntities,
                      UINT32             roadEntityCount,
                      query::IEngine**   engine );

private:
  virtual query::TStatus
  onRoadEntityMakeDone( IRoadEntity*const* roadEntities,
                        UINT32             roadEntityCount,
                        query::IEngine*&   engine ) = 0;
};

//--------------------------------------------------------------------------------
class IHouseNumberEntity : public query::IObject {

public:
  UINT32
  getHouseNumber();

  UINT16
  getRoadMapIndex();

  UINT16
  getPlaceIndex();

  TRoadSide
  getSegmentSide();

  UINT16
  getProportionAlongSegment();

  UINT32
  getSegmentPtr();

  UINT32
  unpackSegment( TRoadSegment* segment );

  BOOL8
  getPosition( TPos*  position,
               INT16* segmentHeading );

private:
  virtual UINT32
  doGetHouseNumber() = 0;

  virtual UINT16
  doGetRoadMapIndex() = 0;

  virtual UINT16
  doGetPlaceIndex() = 0;

  virtual TRoadSide
  doGetSegmentSide() = 0;

  virtual UINT16
  doGetProportionAlongSegment() = 0;

  virtual UINT32
  doGetSegmentPtr() = 0;

  virtual UINT32
  doUnpackSegment( TRoadSegment& segment ) = 0;

  virtual BOOL8
  doGetPosition( TPos&  position,
                 INT16& segmentHeading ) = 0;
};

//--------------------------------------------------------------------------------
class IIntersectionEntity : public query::IObject {

public:
  struct TSegmentAttributes {
    TRoadSectorIndex16    sectorIndex;
    TRoadPlacePairIndex16 placePairIndex;
    TRoadRoadSetIndex32   roadSetIndex;
  };

public:
  UINT16
  getRoadMapIndex();

  const TSegmentAttributes*
  getSegmentAttributes( UINT32* segmentAttributeCount );

  BOOL8
  getPosition( TPos* position );

private:
  virtual UINT16
  doGetRoadMapIndex() = 0;

  virtual const TSegmentAttributes*
  doGetSegmentAttributes( UINT32& segmentAttributeCount ) = 0;

  virtual BOOL8
  doGetPosition( TPos& position ) = 0;
};

//--------------------------------------------------------------------------------
class IIntersectingRoadNameIntersectionEntityCursor : public query::IObject {

public:
  query::TStatus
  getIntersectionEntity( TAreaMatchType*       areaMatchType,
                         IIntersectionEntity** intersectionEntity );

private:
  virtual query::TStatus
  doGetIntersectionEntity( TAreaMatchType&       areaMatchType,
                           IIntersectionEntity*& intersectionEntity ) = 0;
};

//--------------------------------------------------------------------------------
class IIntersectingRoadNameIntersectionEntityTable : public query::IObject {

public:
  query::TStatus
  getIntersectingRoadNameFilter( ISearchFilter** nameFilter );

  query::TStatus
  getIntersectingRoadNameIntersectionEntityCursor(
    TNameIndex32                                    nameIndex,
    IIntersectingRoadNameIntersectionEntityCursor** intersectionEntityCursor );

private:
  virtual query::TStatus
  doGetIntersectingRoadNameFilter( ISearchFilter*& nameFilter ) = 0;

  virtual query::TStatus
  doGetIntersectingRoadNameIntersectionEntityCursor(
    TNameIndex32                                    nameIndex,
    IIntersectingRoadNameIntersectionEntityCursor*& intersectionEntityCursor ) = 0;
};

//--------------------------------------------------------------------------------
class IPOIEntity : public query::IObject {

public:
  UINT16
  getPOIMapIndex();

  TPOISectorIndex16
  getPOISectorIndex();

  TRoadPlaceIndex16
  getPlaceIndex();

  TPOITypeSetIndex16
  getPOITypeSetIndex();

  // The following methods may be dependent
  // on the POI being successfully located.
  // The locate algorithm may run late, so
  // null return values will be returned in
  // the event that the locate algorithm
  // fails to find the POI.

  UINT32
  getPOIId();

  UINT32
  getPOIContentOffset();

  TPos
  getPOIPosition();

  TPOINameSetIndex32
  getPOINameSetIndex();

private:
  virtual UINT16
  doGetPOIMapIndex() = 0;

  virtual TPOISectorIndex16
  doGetPOISectorIndex() = 0;

  virtual TRoadPlaceIndex16
  doGetPlaceIndex() = 0;

  virtual TPOITypeSetIndex16
  doGetPOITypeSetIndex() = 0;

  virtual UINT32
  doGetPOIId() = 0;

  virtual UINT32
  doGetPOIContentOffset() = 0;

  virtual TPos
  doGetPOIPosition() = 0;

  virtual TPOINameSetIndex32
  doGetPOINameSetIndex() = 0;
};

//--------------------------------------------------------------------------------
class IPlaceFilterCache : public query::IObject {

public:
  TBitset
  getRoadMapFilterBitset();

  query::TStatus
  getPlaceFilter( UINT16         roadMapIndex,
                  IPlaceFilter** placeFilter );

  query::TStatus
  getPOIPlaceFilter( UINT16            POIMapIndex,
                     IPOIPlaceFilter** POIPlaceFilter );

  void
  flushCache();

private:
  virtual TBitset
  doGetRoadMapFilterBitset() = 0;

  virtual query::TStatus
  doGetPlaceFilter( UINT16         roadMapIndex,
                    IPlaceFilter*& placeFilter ) = 0;

  virtual query::TStatus
  doGetPOIPlaceFilter( UINT16            POIMapIndex,
                       IPOIPlaceFilter*& POIPlaceFilter ) = 0;

  virtual void
  doFlushCache() = 0;
};

//--------------------------------------------------------------------------------
struct TRoadAddressParameters {
  IResultGroupCache*       resultGroupCache;
  UINT32                   placeFilterCacheCount;
  IPlaceFilterCache*const* placeFilterCaches;
};

//--------------------------------------------------------------------------------
struct TPlaceAddressParameters {
  IResultGroupCache*               resultGroupCache;
  UINT32                           placeFilterCacheCount;
  IPlaceFilterCache*const*         placeFilterCaches;
  const TGeosearchPlaceColumnMask* placeSearchColumns;
};

//--------------------------------------------------------------------------------
struct TPostalCodeAddressParameters {
  IResultGroupCache*       resultGroupCache;
  UINT32                   placeFilterCacheCount;
  IPlaceFilterCache*const* placeFilterCaches;
};

//--------------------------------------------------------------------------------
struct TDetailedPostalCodeAddressParameters {
  IResultGroupCache*       resultGroupCache;
  UINT32                   placeFilterCacheCount;
  IPlaceFilterCache*const* placeFilterCaches;
};

//--------------------------------------------------------------------------------
struct TIntersectionAddressParameters {
  IResultGroupCache*       resultGroupCache;
  UINT32                   placeFilterCacheCount;
  IPlaceFilterCache*const* placeFilterCaches;
};

//--------------------------------------------------------------------------------
struct THouseNumberAddressParameters {
  IResultGroupCache*       resultGroupCache;
  UINT32                   placeFilterCacheCount;
  IPlaceFilterCache*const* placeFilterCaches;
};

//--------------------------------------------------------------------------------
struct TPOIAddressParameters {
  IResultGroupCache*        resultGroupCache;
  UINT32                    placeFilterCacheCount;
  IPlaceFilterCache*const*  placeFilterCaches;
  const UINT32*             POITypeCounts;
  const TPOITypeId16*const* POITypes;
};

//--------------------------------------------------------------------------------
struct TPOIAddressGroupByPOITypeParameters {
  IResultGroupCache*       resultGroupCache;
  UINT32                   placeFilterCacheCount;
  IPlaceFilterCache*const* placeFilterCaches;
};

//--------------------------------------------------------------------------------
class ISearchLocation : public IPlaceFilterCache {//TODO fix this.

public:
  const TTuningParameters*
  getTuningParameters();

  TRect
  getSearchBounds();

private:
  virtual const TTuningParameters*
  doGetTuningParameters() = 0;

  virtual TRect
  doGetSearchBounds() = 0;
};

extern const GUID
IID_IFindCountrySearchLocation;

extern const GUID
IID_IFindStateSearchLocation;

extern const GUID
IID_IFindPlaceSearchLocation;

extern const GUID
IID_IFindPostalCodeSearchLocation;

extern const GUID
IID_IFindDetailedPostalCodeSearchLocation;

extern const GUID
IID_IFindRoadSearchLocation;

extern const GUID
IID_IFindPOITypeSearchLocation;

extern const GUID
IID_IFindPOISearchLocation;

extern const GUID
IID_IFindActivePOISearchLocation;

//--------------------------------------------------------------------------------
class IFindIntersectionSearchLocation : public ISearchLocation {

public:
  IRoadEntity*
  getBaseRoad();

private:
  virtual IRoadEntity*
  doGetBaseRoad() = 0;
};

extern const GUID
IID_IFindIntersectionSearchLocation;

//--------------------------------------------------------------------------------
class IFindHouseNumberSearchLocation : public ISearchLocation {

public:
  IRoadEntity*
  getBaseRoad();

private:
  virtual IRoadEntity*
  doGetBaseRoad() = 0;
};

extern const GUID
IID_IFindHouseNumberSearchLocation;

//--------------------------------------------------------------------------------
class IFindNearestPOISearchLocation : public ISearchLocation {

public:
  //TODO fix this?
  struct TJourneySearchLocation {
    const TJourney* journey;
    TMapLocation*   mapLocation;
    UINT32          searchSize;
    UINT32          maxDetour;
  };

public:
  const TJourneySearchLocation*
  getJourneySearchLocation();

private:
  virtual const TJourneySearchLocation*
  doGetJourneySearchLocation() = 0;
};

extern const GUID
IID_IFindNearestPOISearchLocation;

//--------------------------------------------------------------------------------
class IFindPOITypesInPOITypeSetSearchLocation : public query::IObject {

public:
  UINT16
  getPOIMapIndex();

  TPOITypeSetIndex16
  getPOITypeSetIndex();

private:
  virtual UINT16
  doGetPOIMapIndex() = 0;

  virtual TPOITypeSetIndex16
  doGetPOITypeSetIndex() = 0;
};

extern const GUID
IID_IFindPOITypesInPOITypeSetSearchLocation;

//--------------------------------------------------------------------------------
class IFindUserPOITypesForUserPOISearchLocation : public query::IObject {

public:
  IUserPOISearch*
  getUserPOISearch();

  IUserPOISearch::TResult
  getUserPOI();

private:
  virtual IUserPOISearch*
  doGetUserPOISearch() = 0;

  virtual IUserPOISearch::TResult
  doGetUserPOI_() = 0;  //TODO fix hack!
};

extern const GUID
IID_IFindUserPOITypesForUserPOISearchLocation;

//--------------------------------------------------------------------------------
class IAddressSearchParameters : public query::IObject {

public:
  enum TAddressType {
    ROAD_ADDRESS,
    PLACE_ADDRESS,
    POSTAL_CODE_ADDRESS,
    DETAILED_POSTAL_CODE_ADDRESS,
    INTERSECTION_ADDRESS,
    HOUSE_NUMBER_ADDRESS,
    POI_ADDRESS,
    //FIRST_LETTER_PLACE_ADDRESS,//for TW/CN first-phonetic search
    //FIRST_LETTER_ROAD_ADDRESS,//for TW/CN first-phonetic search
    FIRST_PHONETIC_POI_ADDRESS,//for first-phonetic search
    ADDRESS_TYPES
  };

public:
  TAddressType
  getAddressType();

  const UTF16*
  getSearchString();

  IPlaceFilterCache*
  getPlaceFilterCache();

  TGeosearchPlaceColumnMask
  getPlaceSearchColumns();

  TPOITypeId16
  getPOITypeId();

  const TTuningParameters*
  getTuningParameters();

private:
  virtual TAddressType
  doGetAddressType() = 0;

  virtual const UTF16*
  doGetSearchString() = 0;

  virtual IPlaceFilterCache*
  doGetPlaceFilterCache() = 0;

  virtual TGeosearchPlaceColumnMask
  doGetPlaceSearchColumns() = 0;

  virtual TPOITypeId16
  doGetPOITypeId() = 0;

  virtual const TTuningParameters*
  doGetTuningParameters() = 0;
};

extern const GUID
IID_IAddressSearchParameters;

//--------------------------------------------------------------------------------
class IIntersectingRoadSearchParameters : public query::IObject {

public:
  const UTF16*
  getSearchString();

  IPlaceFilterCache*
  getPlaceFilterCache();

  IRoadEntity*
  getBaseRoad();

  const TTuningParameters*
  getTuningParameters();

private:
  virtual const UTF16*
  doGetSearchString() = 0;

  virtual IPlaceFilterCache*
  doGetPlaceFilterCache() = 0;

  virtual IRoadEntity*
  doGetBaseRoad() = 0;

  virtual const TTuningParameters*
  doGetTuningParameters() = 0;
};

extern const GUID
IID_IIntersectingRoadSearchParameters;

//--------------------------------------------------------------------------------
class IHouseNumberSearchParameters : public query::IObject {

public:
  const UTF16*
  getSearchString();

  IPlaceFilterCache*
  getPlaceFilterCache();

  IRoadEntity*
  getBaseRoad();

  const TTuningParameters*
  getTuningParameters();

private:
  virtual const UTF16*
  doGetSearchString() = 0;

  virtual IPlaceFilterCache*
  doGetPlaceFilterCache() = 0;

  virtual IRoadEntity*
  doGetBaseRoad() = 0;

  virtual const TTuningParameters*
  doGetTuningParameters() = 0;
};

extern const GUID
IID_IHouseNumberSearchParameters;

//--------------------------------------------------------------------------------
class IPOITypeSearchParameters : public query::IObject {

public:
  TGeosearchResultType
  getSearchType();

  const UTF16*
  getSearchString();

  IPlaceFilterCache*
  getPlaceFilterCache();

  const TTuningParameters*
  getTuningParameters();

private:
  virtual TGeosearchResultType
  doGetSearchType() = 0;

  virtual const UTF16*
  doGetSearchString() = 0;

  virtual IPlaceFilterCache*
  doGetPlaceFilterCache() = 0;

  virtual const TTuningParameters*
  doGetTuningParameters() = 0;
};

extern const GUID
IID_IPOITypeSearchParameters;

//--------------------------------------------------------------------------------
class IPOIEnumeratePOITypeSearchParameters : public query::IObject {

public:
  UINT16
  getPOIMapIndex();

  TPOITypeSetIndex16
  getPOITypeSetIndex();

private:
  virtual UINT16
  doGetPOIMapIndex() = 0;

  virtual TPOITypeSetIndex16
  doGetPOITypeSetIndex() = 0;
};

extern const GUID
IID_IPOIEnumeratePOITypeSearchParameters;

//--------------------------------------------------------------------------------
class IUserPOIEnumerateUserPOITypeSearchParameters : public query::IObject {

public:
  IUserPOISearch*
  getUserPOISearch();

  IUserPOISearch::TResult
  getUserPOI();

private:
  virtual IUserPOISearch*
  doGetUserPOISearch() = 0;

  virtual IUserPOISearch::TResult
  doGetUserPOI() = 0;
};

extern const GUID
IID_IUserPOIEnumerateUserPOITypeSearchParameters;

//--------------------------------------------------------------------------------
class IUserPOITypeSearchParameters : public query::IObject {

public:
  TGeosearchResultType
  getSearchType();

  const UTF16*
  getSearchString();

  TRect
  getSearchBounds();

  void*
  getUserPOIContext();

private:
  virtual TGeosearchResultType
  doGetSearchType() = 0;

  virtual const UTF16*
  doGetSearchString() = 0;

  virtual TRect
  doGetSearchBounds() = 0;

  virtual void*
  doGetUserPOIContext() = 0;
};

extern const GUID
IID_IUserPOITypeSearchParameters;

//--------------------------------------------------------------------------------
class IUserPOISearchParameters : public query::IObject {

public:
  const UTF16*
  getSearchString();

  TRect
  getSearchBounds();

  TPOITypeId16
  getPOITypeId();

  void*
  getUserPOIContext();

  //TODO this is currently only required in order to be
  //     passed into the place entity cache.
  IPlaceFilterCache*
  getPlaceFilterCache();

  const TTuningParameters*
  getTuningParameters();

private:
  virtual const UTF16*
  doGetSearchString() = 0;

  virtual TRect
  doGetSearchBounds() = 0;

  virtual TPOITypeId16
  doGetPOITypeId() = 0;

  virtual void*
  doGetUserPOIContext() = 0;

  //TODO this is currently only required in order to be
  //     passed into the place entity cache.
  virtual IPlaceFilterCache*
  doGetPlaceFilterCache() = 0;

  virtual const TTuningParameters*
  doGetTuningParameters() = 0;
};

extern const GUID
IID_IUserPOISearchParameters;

//--------------------------------------------------------------------------------
class INearestPOISearchParameters : public query::IObject {

public:
  struct TJourneyParameters {
    const TJourney* journey;
    TMapLocation*   mapLocation;
    UINT32          searchSize;
    UINT32          maxDetour;
  };

public:
  const UTF16*
  getSearchString();

  IPlaceFilterCache*
  getPlaceFilterCache();

  TRect
  getSearchBounds();

  TPOITypeId16
  getPOITypeId();

  UINT32
  getMaxDistance();

  const TJourneyParameters*
  getJourneyParameters();

  const TTuningParameters*
  getTuningParameters();

private:
  virtual const UTF16*
  doGetSearchString() = 0;

  virtual IPlaceFilterCache*
  doGetPlaceFilterCache() = 0;

  virtual TRect
  doGetSearchBounds() = 0;

  virtual TPOITypeId16
  doGetPOITypeId() = 0;

  virtual UINT32
  doGetMaxDistance() = 0;

  virtual const TJourneyParameters*
  doGetJourneyParameters() = 0;

  virtual const TTuningParameters*
  doGetTuningParameters() = 0;
};

extern const GUID
IID_INearestPOISearchParameters;

//--------------------------------------------------------------------------------
class INearestUserPOISearchParameters : public query::IObject {

public:
  const UTF16*
  getSearchString();

  TRect
  getSearchBounds();

  TPOITypeId16
  getPOITypeId();

  UINT32
  getMaxDistance();

  void*
  getUserPOIContext();

  //TODO this is currently only required in order to be
  //     passed into the place entity cache.
  IPlaceFilterCache*
  getPlaceFilterCache();

  const TTuningParameters*
  getTuningParameters();

private:
  virtual const UTF16*
  doGetSearchString() = 0;

  virtual TRect
  doGetSearchBounds() = 0;

  virtual TPOITypeId16
  doGetPOITypeId() = 0;

  virtual UINT32
  doGetMaxDistance() = 0;

  virtual void*
  doGetUserPOIContext() = 0;

  //TODO this is currently only required in order to be
  //     passed into the place entity cache.
  virtual IPlaceFilterCache*
  doGetPlaceFilterCache() = 0;

  virtual const TTuningParameters*
  doGetTuningParameters() = 0;
};

extern const GUID
IID_INearestUserPOISearchParameters;

//--------------------------------------------------------------------------------
class IActivePOISearchParameters : public query::IObject {

public:
  IPlaceFilterCache*
  getPlaceFilterCache();

  TRect
  getSearchBounds();

  TPOITypeId16
  getPOITypeId();

  UINT32
  getMaxDistance();

private:
  virtual IPlaceFilterCache*
  doGetPlaceFilterCache() = 0;

  virtual TRect
  doGetSearchBounds() = 0;

  virtual TPOITypeId16
  doGetPOITypeId() = 0;

  virtual UINT32
  doGetMaxDistance() = 0;
};

extern const GUID
IID_IActivePOISearchParameters;

//--------------------------------------------------------------------------------
class IActiveUserPOISearchParameters : public query::IObject {

public:
  TRect
  getSearchBounds();

  TPOITypeId16
  getPOITypeId();

  UINT32
  getMaxDistance();

  void*
  getUserPOIContext();

private:
  virtual TRect
  doGetSearchBounds() = 0;

  virtual TPOITypeId16
  doGetPOITypeId() = 0;

  virtual UINT32
  doGetMaxDistance() = 0;

  virtual void*
  doGetUserPOIContext() = 0;
};

extern const GUID
IID_IActiveUserPOISearchParameters;

//--------------------------------------------------------------------------------
class ILatLongSearchParameters : public query::IObject {

public:
  const UTF16*
  getSearchString();

private:
  virtual const UTF16*
  doGetSearchString() = 0;
};

extern const GUID
IID_ILatLongSearchParameters;

//--------------------------------------------------------------------------------
class IEngineFactory : public query::IObject {

public:
  query::TStatus
  makeEngine( query::IEngine*  tailEngine,
              query::IEngine** engine );

private:
  virtual query::TStatus
  doMakeEngine( query::IEngine*  tailEngine,
                query::IEngine*& engine ) = 0;
};

//--------------------------------------------------------------------------------
class IEngineFactoryQueue : public query::IObject {

public:
  query::TStatus
  addEngineFactory( IEngineFactory* engineFactory );

  void
  removeEngineFactory( IEngineFactory* engineFactory );

  query::TStatus
  popFrontEngineFactory( IEngineFactory** engineFactory );

private:
  virtual query::TStatus
  doAddEngineFactory( IEngineFactory* engineFactory ) = 0;

  virtual void
  doRemoveEngineFactory( IEngineFactory* engineFactory ) = 0;

  virtual query::TStatus
  doPopFrontEngineFactory( IEngineFactory*& engineFactory ) = 0;
};

//--------------------------------------------------------------------------------
class ITokenSeqNameWorkspace : public query::IObject {

public:
  query::TStatus
  getKeywords( const TTokenDataSource *tokenSource,
               TKeywords              *keywords );
  query::TStatus
  beginName( TNameIndex32 nameIndex );

  query::TStatus
  checkToken( const TTokenCheckData& tokenData );

  query::TStatus
  endName();

private:
  virtual query::TStatus
  doGetKeywords( const TTokenDataSource *tokenSource,
                 TKeywords              *keywords ) = 0;

  virtual query::TStatus
  doBeginName( TNameIndex32 nameIndex ) = 0;

  virtual query::TStatus
  doCheckToken( const TTokenCheckData& tokenData ) = 0;

  virtual query::TStatus
  doEndName() = 0;
};

//--------------------------------------------------------------------------------
class INameTableScanTask : public query::IObject {

public:
  query::TStatus
  addTokenSeqNameWorkspace( ITokenSeqNameWorkspace* tokenSeqNameWorkspace );

  void
  removeTokenSeqNameWorkspace( ITokenSeqNameWorkspace* tokenSeqNameWorkspace );

private:
  virtual query::TStatus
  doAddTokenSeqNameWorkspace( ITokenSeqNameWorkspace* tokenSeqNameWorkspace ) = 0;

  virtual void
  doRemoveTokenSeqNameWorkspace( ITokenSeqNameWorkspace* tokenSeqNameWorkspace ) = 0;
};

//--------------------------------------------------------------------------------
class IPlaceQualifierCursor : public query::IObject {

public:
  query::TStatus
  getPlaceQualifier( TPlaceQualifier* placeQualifier );

private:
  virtual query::TStatus
  doGetPlaceQualifier( TPlaceQualifier& placeQualifier ) = 0;
};

//--------------------------------------------------------------------------------
class IPlaceQualifierAttribute : public query::IObject {

public:
  struct TSectorPlace {
    TRoadSectorIndex16 sectorIndex;
    TRoadPlaceIndex16  placeIndex;
  };

public:
  query::TStatus
  getMatchedPlaceQualifiers( const TSectorPlace*     sectorPlaces,
                             UINT32                  sectorPlaceCount,
                             TAreaMatchType*         areaMatchType,
                             IPlaceQualifierCursor** placeQualifierCursor );

private:
  virtual query::TStatus
  doGetMatchedPlaceQualifiers( const TSectorPlace      sectorPlaces[],
                               UINT32                  sectorPlaceCount,
                               TAreaMatchType&         areaMatchType,
                               IPlaceQualifierCursor*& placeQualifierCursor ) = 0;
};

//--------------------------------------------------------------------------------
class IHouseNumberAttribute : public query::IObject {

public:
  UINT32
  getHouseNumberFilter();

  BOOL8
  getFlagAllowPartialHouseNumber();

private:
  virtual UINT32
  doGetHouseNumberFilter() = 0;

  virtual BOOL8
  doGetFlagAllowPartialHouseNumber() = 0;
};

//--------------------------------------------------------------------------------
class INameCursor : public query::IObject {

public:
  query::TStatus
  getValue( TName* name );

private:
  virtual query::TStatus
  doGetValue( TName& name ) = 0;
};

//--------------------------------------------------------------------------------
class INameQuery : public query::IObject {

public:
  query::TStatus
  getCursor( INameCursor** cursor );

private:
  virtual query::TStatus
  doGetCursor( INameCursor*& cursor ) = 0;
};

//--------------------------------------------------------------------------------
class INameAttribute : public query::IObject {

public:
  //TODO it might be better to directly get the cursor from the attribute?
  query::TStatus
  getNameQuery( INameQuery** nameQuery );

private:
  virtual query::TStatus
  doGetNameQuery( INameQuery*& nameQuery ) = 0;
};

//--------------------------------------------------------------------------------
class IPostalCodeCursor : public query::IObject {

public:
  query::TStatus
  getValue( TPostalCode* postalCode );

private:
  virtual query::TStatus
  doGetValue( TPostalCode& postalCode ) = 0;
};

//--------------------------------------------------------------------------------
class IPostalCodeQuery : public query::IObject {

public:
  query::TStatus
  getCursor( IPostalCodeCursor** cursor );

private:
  virtual query::TStatus
  doGetCursor( IPostalCodeCursor*& cursor ) = 0;
};

//--------------------------------------------------------------------------------
class IPostalCodeUnicodeSeq : public query::IObject {

public:
  query::TStatus
  getExtendedSequence( UTF16                   unicodeCharacter,
                       TNameCost8              matchCost,
                       IPostalCodeUnicodeSeq** extendedPostalCodeUnicodeSeq );

  query::TStatus
  getPostalCodeCursor( BOOL8               selectFullMatch,
                       BOOL8               selectPartialMatch,
                       IPostalCodeCursor** postalCodeCursor );

private:
  virtual query::TStatus
  doGetExtendedSequence( UTF16                   unicodeCharacter,
                         TNameCost8              matchCost,
                         IPostalCodeUnicodeSeq*& extendedPostalCodeUnicodeSeq ) = 0;

  virtual query::TStatus
  doGetPostalCodeCursor( BOOL8               selectFullMatch,
                         BOOL8               selectPartialMatch,
                         IPostalCodeCursor*& postalCodeCursor ) = 0;
};

//--------------------------------------------------------------------------------
class IPostalCodeUnicodeSeqCursor : public query::IObject {

public:
  query::TStatus
  getValue( IPostalCodeUnicodeSeq** postalCodeUnicodeSeq );

private:
  virtual query::TStatus
  doGetValue( IPostalCodeUnicodeSeq*& postalCodeUnicodeSeq ) = 0;
};

//--------------------------------------------------------------------------------
class IPostalCodeUnicodeSeqQuery : public query::IObject {

public:
  query::TStatus
  getCursor( IPostalCodeUnicodeSeqCursor** cursor );

private:
  virtual query::TStatus
  doGetCursor( IPostalCodeUnicodeSeqCursor*& cursor ) = 0;
};

//--------------------------------------------------------------------------------
struct TKeywordTokenListNode {
  UINT8                  count;
  UINT32*                tokens;
  TKeywordTokenListNode* next;
};

//--------------------------------------------------------------------------------
class ITokenSet : public query::IObject {

public:
  query::TStatus
  getTokenBitset( const TBitset** tokenBitset );

  query::TStatus
  getKeywordToken( TKeywordTokenListNode** keywordTokenListNode );

  query::TStatus
  getToken( TTokenIndex32 tokenIndex,
            TToken*       token );

private:
  virtual query::TStatus
  doGetTokenBitset( const TBitset*& tokenBitset ) = 0;

  virtual query::TStatus
  doGetKeywordToken( TKeywordTokenListNode*& keywordTokenNode ) = 0;

  virtual query::TStatus
  doGetToken( TTokenIndex32 tokenIndex,
              TToken&       token ) = 0;
};

//--------------------------------------------------------------------------------
class ITokenPartSeq : public query::IObject {

public:
  query::TStatus
  getExtendedSequence( const TTokenPart* tokenPart,
                       ITokenPartSeq**   extendedTokenPartSeq );

  query::TStatus
  getRangeStatus( BOOL8 allowPartialTokens );
 
  query::TStatus
  getTokenBitset( IAllocator* allocator,
                  TBitset*    tokenBitset,
                  BOOL8       allowPartialTokens,
                  TKeywordTokenListNode* listNode);

  query::TStatus
  getToken( TTokenIndex32 tokenIndex,
            BOOL8         allowPartialToken,
            TToken*       token );

private:
  virtual query::TStatus
  doGetExtendedSequence( const TTokenPart* tokenPart, 
                         ITokenPartSeq*&   extendedTokenPartSeq ) = 0;

  virtual query::TStatus
  doGetRangeStatus( BOOL8 allowPartialTokens ) = 0;

  virtual query::TStatus
  doGetTokenBitset( IAllocator* allocator,
                    TBitset*    tokenBitset,
                    BOOL8       allowPartialTokens,
                    TKeywordTokenListNode* listNode) = 0;

  virtual query::TStatus
  doGetToken( TTokenIndex32 tokenIndex,
              BOOL8         allowPartialToken,
              TToken&       token ) = 0;
};

//--------------------------------------------------------------------------------
class ITokenPartSeqCursor : public query::IObject {

public:
  query::TStatus
  getValue( ITokenPartSeq** tokenPartSeq );

private:
  virtual query::TStatus
  doGetValue( ITokenPartSeq*& tokenPartSeq ) = 0;
};

//--------------------------------------------------------------------------------
class ITokenPartSeqQuery : public query::IObject {

public:
  query::TStatus
  getCursor( ITokenPartSeqCursor** cursor );

private:
  virtual query::TStatus
  doGetCursor( ITokenPartSeqCursor*& cursor ) = 0;
};

//--------------------------------------------------------------------------------
struct TTokenSeqNode {
  TTokenSeqNode* nextSeqNode;
  ITokenSet*     tokenSet;
  const TBitset* tokenBitset;
  BOOL8          allowFollowTokenReorder;
  UINT8          tailTokenSeqLength;

  union {
    // 'Exact match' temporary data.
    struct {
      TTokenSeqNode* nextMarkerNode;
    } exactMatch;

    // 'Any match' temporary data.
    struct {
      UINT32 matchedSeqTokens;
    } anyMatch;
  };
};

//--------------------------------------------------------------------------------
struct TTokenSeq {
  TTokenSeqNode* firstSeqNode;

  union {
    // 'Exact match' temporary data.
    struct {
      TTokenSeqNode* firstMarkerNode;
    } exactMatch;

    // 'Any match' temporary data.
    struct {
      UINT32 unmatchedCount;
    } anyMatch;
  };
};

//--------------------------------------------------------------------------------
struct TTokenSeqListNode {
  TTokenSeqListNode* nextListNode;
  TTokenSeq          tokenSeq;
};

//--------------------------------------------------------------------------------
struct TTokenSeqList {
  IAllocator*        allocator;
  TTokenSeqListNode* firstListNode;
};

//--------------------------------------------------------------------------------
struct TKeywordTokenList {
  IAllocator*            allocator;
  TKeywordTokenListNode* firstListNode;
};

//--------------------------------------------------------------------------------
void destroyTokenSeqList( TTokenSeqList& tokenSeqList );

//--------------------------------------------------------------------------------
void destroyKeywordTokenList( TKeywordTokenList& keywordTokenList );

//--------------------------------------------------------------------------------
void destroyKeywordTokenListNode( IAllocator*            allocator,
                                  TKeywordTokenListNode& keywordTokenListNode );

//--------------------------------------------------------------------------------
class ITokenSeqAttribute : public query::IObject {

public:
  query::TStatus
  getExtendedSequenceWithEndToken( ITokenPartSeqQuery*  tokenPartSeqQuery,
                                   BOOL8                allowPartialTokens,
                                   ITokenSeqAttribute** extendedTokenSeqAttribute );

  query::TStatus
  getExtendedSequenceWithEndSeparator( BOOL8                allowTokenReorder,
                                       ITokenSeqAttribute** extendedTokenSeqAttribute );

  // Add to an existing token bitset all tokens that
  // can be matched by the attribute (its token and
  // any leading tokens through all possible paths).
  query::TStatus
  getUnionTokenBitset( TBitset* tokenBitset );

  // Get a list of all possible input token sequences.
  query::TStatus
  getTokenSeqList( TTokenSeqList& tokenSeqList,
                   BOOL8          allowFollowTokenReorder );

  query::TStatus
  getKeywordTokenList( TKeywordTokenList& keywordTokenList);

private:
  virtual query::TStatus
  doGetExtendedSequenceWithEndToken( ITokenPartSeqQuery*  tokenPartSeqQuery,
                                     BOOL8                allowPartialTokens,
                                     ITokenSeqAttribute*& extendedTokenSeqAttribute ) = 0;

  virtual query::TStatus
  doGetExtendedSequenceWithEndSeparator( BOOL8                allowTokenReorder,
                                         ITokenSeqAttribute*& extendedTokenSeqAttribute ) = 0;

  virtual query::TStatus
  doGetUnionTokenBitset( TBitset* tokenBitset ) = 0;

  virtual query::TStatus
  doGetTokenSeqList( TTokenSeqList& tokenSeqList,
                     BOOL8          allowFollowTokenReorder ) = 0;

  virtual query::TStatus
  doGetKeywordTokenList( TKeywordTokenList& keywordTokenList ) = 0;
};

//--------------------------------------------------------------------------------
class ITokenPartCursor : public query::IObject {

public:
  query::TStatus
  getValue( TTokenPart* tokenPart );

private:
  virtual query::TStatus
  doGetValue( TTokenPart& tokenPart ) = 0;
};

//--------------------------------------------------------------------------------
class ITokenPartQuery : public query::IObject {

public:
  query::TStatus
  getCursor( ITokenPartCursor** cursor );

private:
  virtual query::TStatus
  doGetCursor( ITokenPartCursor*& cursor ) = 0;
};

//--------------------------------------------------------------------------------
class ISpellingCharacterSeq : public query::IObject {

public:
  query::TStatus
  getExtendedSequence( UTF16                   unicodeCharacter,
                       ISpellingCharacterSeq** extendedSpellingCharacterSeq );

  query::TStatus
  getTokenPartCursor( BOOL8              selectFullMatch,
                      BOOL8              selectPartialMatch,
                      ITokenPartCursor** tokenPartCursor );

private:
  virtual query::TStatus
  doGetExtendedSequence( UTF16                   unicodeCharacter,
                         ISpellingCharacterSeq*& extendedSpellingCharacterSeq ) = 0;

  virtual query::TStatus
  doGetTokenPartCursor( BOOL8              selectFullMatch,
                        BOOL8              selectPartialMatch,
                        ITokenPartCursor*& tokenPartCursor ) = 0;
};

//--------------------------------------------------------------------------------
class ISpellingCharacterSeqCursor : public query::IObject {

public:
  query::TStatus
  getValue( ISpellingCharacterSeq** spellingCharacterSeq );

private:
  virtual query::TStatus
  doGetValue( ISpellingCharacterSeq*& spellingCharacterSeq ) = 0;
};

//--------------------------------------------------------------------------------
class ISpellingCharacterSeqQuery : public query::IObject {

public:
  query::TStatus
  getCursor( ISpellingCharacterSeqCursor** cursor );

private:
  virtual query::TStatus
  doGetCursor( ISpellingCharacterSeqCursor*& cursor ) = 0;
};

//--------------------------------------------------------------------------------
class IUnicodeCursor : public query::IObject {

public:
  query::TStatus
  getValue( UTF16* unicodeCharacter );

private:
  virtual query::TStatus
  doGetValue( UTF16& unicodeCharacter ) = 0;
};

//--------------------------------------------------------------------------------
class IUnicodeQuery : public query::IObject {

public:
  query::TStatus
  getCursor( IUnicodeCursor** cursor );

private:
  virtual query::TStatus
  doGetCursor( IUnicodeCursor*& cursor ) = 0;
};

//--------------------------------------------------------------------------------
class ILatLongFractionAttribute : public query::IObject {

public:
  UINT32
  getNumerator();

  UINT32
  getDenominator();

private:
  virtual UINT32
  doGetNumerator() = 0;

  virtual UINT32
  doGetDenominator() = 0;
};

//--------------------------------------------------------------------------------
class ILatLongDegreeAttribute : public query::IObject {

public:
  UINT8
  getDegree();

private:
  virtual UINT8
  doGetDegree() = 0;
};

//--------------------------------------------------------------------------------
class ILatLongMinuteAttribute : public query::IObject {

public:
  UINT8
  getMinute();

private:
  virtual UINT8
  doGetMinute() = 0;
};

//--------------------------------------------------------------------------------
class ILatLongSecondAttribute : public query::IObject {

public:
  UINT8
  getSecond();

  ILatLongFractionAttribute*
  getSecondFractionAttribute();

private:
  virtual UINT8
  doGetSecond() = 0;

  virtual ILatLongFractionAttribute*
  doGetSecondFractionAttribute() = 0;
};

//--------------------------------------------------------------------------------
class ILatLongDMSAttribute : public query::IObject {

public:
  ILatLongDegreeAttribute*
  getDegreeAttribute();

  ILatLongMinuteAttribute*
  getMinuteAttribute();

  ILatLongSecondAttribute*
  getSecondAttribute();

private:
  virtual ILatLongDegreeAttribute*
  doGetDegreeAttribute() = 0;

  virtual ILatLongMinuteAttribute*
  doGetMinuteAttribute() = 0;

  virtual ILatLongSecondAttribute*
  doGetSecondAttribute() = 0;
};

//--------------------------------------------------------------------------------
class ILatLongDecimalAttribute : public query::IObject {

public:
  BOOL8
  getNegativeFlag();

  UINT8
  getDegree();

  ILatLongFractionAttribute*
  getDegreeFractionAttribute();

private:
  virtual BOOL8
  doGetNegativeFlag() = 0;

  virtual UINT8
  doGetDegree() = 0;

  virtual ILatLongFractionAttribute*
  doGetDegreeFractionAttribute() = 0;
};

//--------------------------------------------------------------------------------
enum THouseNumberAddressFormat {
  HOUSE_NUMBER_ADDRESS_FORMAT_NUMBER_AT_FRONT_SPACE_SEPARATOR,
  HOUSE_NUMBER_ADDRESS_FORMAT_NUMBER_AT_FRONT_COMMA_SEPARATOR,
  HOUSE_NUMBER_ADDRESS_FORMAT_NUMBER_AT_BACK_SPACE_SEPARATOR,
  HOUSE_NUMBER_ADDRESS_FORMAT_NUMBER_AT_BACK_COMMA_SEPARATOR
};

typedef UINT8
THouseNumberAddressFormat8;

//--------------------------------------------------------------------------------
class IMergedAddressAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeMergedAddressAttribute( query::IObject*const* addressAttributes,
                              UINT32                addressAttributeCount,
                              query::IObject**      mergedAddressAttribute );

private:
  virtual query::TStatus
  doMakeMergedAddressAttribute( query::IObject*const addressAttributes[],
                                UINT32               addressAttributeCount,
                                query::IObject*&     mergedAddressAttribute ) = 0;
};

extern const GUID
IID_IMergedAddressAttributeFactory;

//--------------------------------------------------------------------------------
class ILatLongAddressAttributeFactory : public query::IObject {

public:
  enum TCardinal {
    CARDINAL_NORTH,
    CARDINAL_SOUTH,
    CARDINAL_EAST,
    CARDINAL_WEST,
    CARDINAL_MAX
  };

public:
  query::TStatus
  makeLatLongAddressAttribute( TPos             latLongPosition,
                               query::IObject** latLongAddressAttribute );

  query::TStatus
  makeLatLongDecimalAttribute( BOOL8                      flagNegative,
                               UINT8                      degree,
                               ILatLongFractionAttribute* degreeFractionAttribute,
                               ILatLongDecimalAttribute** latLongDecimalAttribute );

  query::TStatus
  makeLatLongDMSAttribute( ILatLongDegreeAttribute* degreeAttribute,
                           ILatLongMinuteAttribute* minuteAttribute,
                           ILatLongSecondAttribute* secondAttribute,
                           ILatLongDMSAttribute**   latLongDMSAttribute );

  query::TStatus
  makeLatLongDegreeAttribute( UINT8                     degree,
                              ILatLongDegreeAttribute** degreeAttribute );

  query::TStatus
  makeLatLongMinuteAttribute( UINT8                     minute,
                              ILatLongMinuteAttribute** minuteAttribute );

  query::TStatus
  makeLatLongSecondAttribute( UINT8                      second,
                              ILatLongFractionAttribute* secondFractionAttribute,
                              ILatLongSecondAttribute**  secondAttribute );

  query::TStatus
  makeLatLongFractionAttribute( UINT32                      numerator,
                                UINT32                      denominator,
                                ILatLongFractionAttribute** fractionAttribute );

  BOOL8
  getDecimalPointMatchFlag( UTF16 unicodeChar );

  BOOL8
  getCardinalMatchFlag( TCardinal cardinal,
                        UTF16     unicodeChar );

private:
  virtual query::TStatus
  doMakeLatLongAddressAttribute( TPos             latLongPosition,
                                 query::IObject*& latLongAddressAttribute ) = 0;

  virtual query::TStatus
  doMakeLatLongDecimalAttribute( BOOL8                      flagNegative,
                                 UINT8                      degree,
                                 ILatLongFractionAttribute* degreeFractionAttribute,
                                 ILatLongDecimalAttribute*& latLongDecimalAttribute ) = 0;

  virtual query::TStatus
  doMakeLatLongDMSAttribute( ILatLongDegreeAttribute* degreeAttribute,
                             ILatLongMinuteAttribute* minuteAttribute,
                             ILatLongSecondAttribute* secondAttribute,
                             ILatLongDMSAttribute*&   latLongDMSAttribute ) = 0;

  virtual query::TStatus
  doMakeLatLongDegreeAttribute( UINT8                     degree,
                                ILatLongDegreeAttribute*& degreeAttribute ) = 0;

  virtual query::TStatus
  doMakeLatLongMinuteAttribute( UINT8                     minute,
                                ILatLongMinuteAttribute*& minuteAttribute ) = 0;

  virtual query::TStatus
  doMakeLatLongSecondAttribute( UINT8                      second,
                                ILatLongFractionAttribute* secondFractionAttribute,
                                ILatLongSecondAttribute*&  secondAttribute ) = 0;

  virtual query::TStatus
  doMakeLatLongFractionAttribute( UINT32                      numerator,
                                  UINT32                      denominator,
                                  ILatLongFractionAttribute*& fractionAttribute ) = 0;

  virtual BOOL8
  doGetDecimalPointMatchFlag( UTF16 unicodeChar ) = 0;

  virtual BOOL8
  doGetCardinalMatchFlag( TCardinal cardinal,
                          UTF16     unicodeChar ) = 0;
};

extern const GUID
IID_ILatLongAddressAttributeFactory;

//--------------------------------------------------------------------------------
class IRoadAddressAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeRoadAddressAttribute( INameAttribute*           roadNameOrRouteNameAttribute,
                            IPlaceQualifierAttribute* placeQualifierAttribute,
                            query::IObject**          roadAddressAttribute );

  query::TStatus
  makeDefaultRoadAddressAttribute( query::IObject** roadAddressAttribute );

private:
  virtual query::TStatus
  doMakeRoadAddressAttribute( INameAttribute*           roadNameOrRouteNameAttribute,
                              IPlaceQualifierAttribute* placeQualifierAttribute,
                              query::IObject*&          roadAddressAttribute ) = 0;

  virtual query::TStatus
  doMakeDefaultRoadAddressAttribute( query::IObject*& roadAddressAttribute ) = 0;
};

extern const GUID
IID_IRoadAddressAttributeFactory;

//--------------------------------------------------------------------------------
class IPlaceAddressAttributeFactory : public query::IObject {

public:
  query::TStatus
  makePlaceAddressAttribute( INameAttribute*           placeNameAttribute,
                             IPlaceQualifierAttribute* placeQualifierAttribute,
                             query::IObject**          placeAddressAttribute );

  query::TStatus
  makeDefaultPlaceAddressAttribute( query::IObject** placeAddressAttribute );

private:
  virtual query::TStatus
  doMakePlaceAddressAttribute( INameAttribute*           placeNameAttribute,
                               IPlaceQualifierAttribute* placeQualifierAttribute,
                               query::IObject*&          placeAddressAttribute ) = 0;

  virtual query::TStatus
  doMakeDefaultPlaceAddressAttribute( query::IObject*& placeAddressAttribute ) = 0;
};

extern const GUID
IID_IPlaceAddressAttributeFactory;

//--------------------------------------------------------------------------------
class IPOIAddressAttributeFactory : public query::IObject {

public:
  query::TStatus
  makePOIAddressAttribute( INameAttribute*           POINameAttribute,
                           IPlaceQualifierAttribute* placeQualifierAttribute,
                           query::IObject**          POIAddressAttribute );

  query::TStatus
  makeDefaultPOIAddressAttribute( query::IObject** POIAddressAttribute );

private:
  virtual query::TStatus
  doMakePOIAddressAttribute( INameAttribute*           POINameAttribute,
                             IPlaceQualifierAttribute* placeQualifierAttribute,
                             query::IObject*&          POIAddressAttribute ) = 0;

  virtual query::TStatus
  doMakeDefaultPOIAddressAttribute( query::IObject*& POIAddressAttribute ) = 0;
};

extern const GUID
IID_IPOIAddressAttributeFactory;

//--------------------------------------------------------------------------------
class IPOIAddressGroupByPOITypeAttributeFactory : public query::IObject {

public:
  query::TStatus
  makePOIAddressGroupByPOITypeAttribute( INameAttribute*           POINameAttribute,
                                         IPlaceQualifierAttribute* placeQualifierAttribute,
                                         query::IObject**          POIAddressGroupByPOITypeAttribute );

private:
  virtual query::TStatus
  doMakePOIAddressGroupByPOITypeAttribute( INameAttribute*           POINameAttribute,
                                           IPlaceQualifierAttribute* placeQualifierAttribute,
                                           query::IObject*&          POIAddressGroupByPOITypeAttribute ) = 0;
};

extern const GUID
IID_IPOIAddressGroupByPOITypeAttributeFactory;

//--------------------------------------------------------------------------------
class IIntersectionAddressAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeIntersectionAddressAttribute( INameAttribute*           roadNameOrRouteNameAttribute1,
                                    INameAttribute*           roadNameOrRouteNameAttribute2,
                                    IPlaceQualifierAttribute* placeQualifierAttribute,
                                    query::IObject**          intersectionAddressAttribute );

private:
  virtual query::TStatus
  doMakeIntersectionAddressAttribute( INameAttribute*           roadNameOrRouteNameAttribute1,
                                      INameAttribute*           roadNameOrRouteNameAttribute2,
                                      IPlaceQualifierAttribute* placeQualifierAttribute,
                                      query::IObject*&          intersectionAddressAttribute ) = 0;
};

extern const GUID
IID_IIntersectionAddressAttributeFactory;

//--------------------------------------------------------------------------------
class IHouseNumberAddressAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeHouseNumberAddressAttribute( THouseNumberAddressFormat houseNumberAddressFormat,
                                   IHouseNumberAttribute*    houseNumberAttribute,
                                   INameAttribute*           roadNameOrRouteNameAttribute,
                                   IPlaceQualifierAttribute* placeQualifierAttribute,
                                   query::IObject**          houseNumberAddressAttribute );

private:
  virtual query::TStatus
  doMakeHouseNumberAddressAttribute( THouseNumberAddressFormat houseNumberAddressFormat,
                                     IHouseNumberAttribute*    houseNumberAttribute,
                                     INameAttribute*           roadNameOrRouteNameAttribute,
                                     IPlaceQualifierAttribute* placeQualifierAttribute,
                                     query::IObject*&          houseNumberAddressAttribute ) = 0;
};

extern const GUID
IID_IHouseNumberAddressAttributeFactory;

//--------------------------------------------------------------------------------
class IHouseNumberAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeHouseNumberAttribute( UINT32                  houseNumberFilter,
                            BOOL8                   flagAllowPartialHouseNumber,
                            IHouseNumberAttribute** houseNumberAttribute );

private:
  virtual query::TStatus
  doMakeHouseNumberAttribute( UINT32                  houseNumberFilter,
                              BOOL8                   flagAllowPartialHouseNumber,
                              IHouseNumberAttribute*& houseNumberAttribute ) = 0;
};

extern const GUID
IID_IHouseNumberAttributeFactory;

//--------------------------------------------------------------------------------
class IPostalCodeAddressAttributeFactory : public query::IObject {

public:
  query::TStatus
  makePostalCodeAddressAttribute( IPostalCodeQuery* postalCodeQuery,
                                  query::IObject**  postalCodeAddressAttribute );

  query::TStatus
  makeDefaultPostalCodeAddressAttribute( query::IObject** postalCodeAddressAttribute );

private:
  virtual query::TStatus
  doMakePostalCodeAddressAttribute( IPostalCodeQuery* postalCodeQuery,
                                    query::IObject*&  postalCodeAddressAttribute ) = 0;

  virtual query::TStatus
  doMakeDefaultPostalCodeAddressAttribute( query::IObject*& postalCodeAddressAttribute ) = 0;
};

extern const GUID
IID_IPostalCodeAddressAttributeFactory;

extern const GUID
IID_IDetailedPostalCodeAddressAttributeFactory;

//--------------------------------------------------------------------------------
class IPostalCodeQueryFactory : public query::IObject {

public:
  query::TStatus
  makePostalCodeQuery( IPostalCodeUnicodeSeqQuery* postalCodeUnicodeSeqQuery,
                       IPostalCodeQuery**          postalCodeQuery );

  query::TStatus
  makeMergedPostalCodeQuery( IPostalCodeQuery*const* postalCodeQueries,
                             UINT32                  postalCodeQueryCount,
                             IPostalCodeQuery**      mergedPostalCodeQuery );

private:
  virtual query::TStatus
  doMakePostalCodeQuery( IPostalCodeUnicodeSeqQuery* postalCodeUnicodeSeqQuery,
                         IPostalCodeQuery*&          postalCodeQuery ) = 0;

  virtual query::TStatus
  doMakeMergedPostalCodeQuery( IPostalCodeQuery*const* postalCodeQueries,
                               UINT32                  postalCodeQueryCount,
                               IPostalCodeQuery*&      mergedPostalCodeQuery ) = 0;
};

extern const GUID
IID_IPostalCodeQueryFactory;

extern const GUID
IID_IDetailedPostalCodeQueryFactory;

//--------------------------------------------------------------------------------
class IPostalCodeUnicodeSeqQueryFactory : public query::IObject {

public:
  query::TStatus
  makeEmptyMainPostalCodeUnicodeSeqQuery( IPostalCodeUnicodeSeqQuery** emptyMainPostalCodeUnicodeSeqQuery );

  query::TStatus
  makeEmptySubPostalCodeUnicodeSeqQuery( IPostalCodeUnicodeSeqQuery*  fullMainPostalCodeUnicodeSeqQuery,
                                         parser::ITerminal*           unicodeTerminal,
                                         BOOL8*                       unicodeTerminalIsSeparator, 
                                         IPostalCodeUnicodeSeqQuery** emptySubPostalCodeUnicodeSeqQuery );

  query::TStatus
  makeExtendedPostalCodeUnicodeSeqQuery( IPostalCodeUnicodeSeqQuery*  postalCodeUnicodeSeqQuery,
                                         parser::ITerminal*           unicodeTerminal,
                                         IPostalCodeUnicodeSeqQuery** extendedPostalCodeUnicodeSeqQuery );

private:
  virtual query::TStatus
  doMakeEmptyMainPostalCodeUnicodeSeqQuery( IPostalCodeUnicodeSeqQuery*& emptyMainPostalCodeUnicodeSeqQuery ) = 0;

  virtual query::TStatus
  doMakeEmptySubPostalCodeUnicodeSeqQuery( IPostalCodeUnicodeSeqQuery*  fullMainPostalCodeUnicodeSeqQuery,
                                           parser::ITerminal*           unicodeTerminal,
                                           BOOL8&                       unicodeTerminalIsSeparator, 
                                           IPostalCodeUnicodeSeqQuery*& emptySubPostalCodeUnicodeSeqQuery ) = 0;

  virtual query::TStatus
  doMakeExtendedPostalCodeUnicodeSeqQuery( IPostalCodeUnicodeSeqQuery*  postalCodeUnicodeSeqQuery,
                                           parser::ITerminal*           unicodeTerminal,
                                           IPostalCodeUnicodeSeqQuery*& extendedPostalCodeUnicodeSeqQuery ) = 0;
};

extern const GUID
IID_IPostalCodeUnicodeSeqQueryFactory;

extern const GUID
IID_IDetailedPostalCodeUnicodeSeqQueryFactory;

//--------------------------------------------------------------------------------
class INearestPOINameAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeDefaultNearestPOINameAttribute( INameAttribute** nearestPOINameAttribute );

private:
  virtual query::TStatus
  doMakeDefaultNearestPOINameAttribute( INameAttribute*& nearestPOINameAttribute ) = 0;
};

extern const GUID
IID_INearestPOINameAttributeFactory;

//--------------------------------------------------------------------------------
class IPOITypeNameAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeDefaultPOITypeNameAttribute( INameAttribute** POITypeNameAttribute );

private:
  virtual query::TStatus
  doMakeDefaultPOITypeNameAttribute( INameAttribute*& POITypeNameAttribute ) = 0;
};

extern const GUID
IID_IPOITypeNameAttributeFactory;

//--------------------------------------------------------------------------------
class IMergedRoadNameOrRouteNameAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeMergedRoadNameOrRouteNameAttribute( INameAttribute*const* nameAttributes,
                                          UINT32                nameAttributeCount,
                                          INameAttribute**      mergedNameAttribute );

private:
  virtual query::TStatus
  doMakeMergedRoadNameOrRouteNameAttribute( INameAttribute*const nameAttributes[],
                                            UINT32               nameAttributeCount,
                                            INameAttribute*&     mergedNameAttribute ) = 0;
};

extern const GUID
IID_IMergedRoadNameOrRouteNameAttributeFactory;

//--------------------------------------------------------------------------------
class IPlaceQualifierAttributeFactory : public query::IObject {

public:
  query::TStatus
  makePlaceQualifierAttribute( INameAttribute*            placeNameAttribute,
                               IPlaceQualifierAttribute** placeQualifierAttribute );

private:
  virtual query::TStatus
  doMakePlaceQualifierAttribute( INameAttribute*            placeNameAttribute,
                                 IPlaceQualifierAttribute*& placeQualifierAttribute ) = 0;
};

extern const GUID
IID_IPlaceQualifierAttributeFactory;

//--------------------------------------------------------------------------------
class ITokenSeqNameAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeTokenSeqNameAttribute( ITokenSeqAttribute* tokenSeqAttribute,
                             INameAttribute**    nameAttribute );

  query::TStatus
  makeMergedTokenSeqNameAttribute( INameAttribute*const* nameAttributes,
                                   UINT32                nameAttributeCount,
                                   INameAttribute**      mergedNameAttribute );

private:
  virtual query::TStatus
  doMakeTokenSeqNameAttribute( ITokenSeqAttribute* tokenSeqAttribute,
                               INameAttribute*&    nameAttribute ) = 0;

  virtual query::TStatus
  doMakeMergedTokenSeqNameAttribute( INameAttribute*const nameAttributes[],
                                     UINT32               nameAttributeCount,
                                     INameAttribute*&     mergedNameAttribute ) = 0;
};

extern const GUID
IID_IRoadTokenSeqNameAttributeFactory;

extern const GUID
IID_IRouteTokenSeqNameAttributeFactory;

extern const GUID
IID_IPlaceTokenSeqNameAttributeFactory;

extern const GUID
IID_IPOITokenSeqNameAttributeFactory;

extern const GUID
IID_IPOITypeTokenSeqNameAttributeFactory;

//--------------------------------------------------------------------------------
class ITokenSeqAttributeFactory : public query::IObject {

public:
  query::TStatus
  makeEmptyTokenSeqAttribute( ITokenSeqAttribute** emptyTokenSeqAttribute );

  query::TStatus
  makeMergedTokenSeqAttribute( ITokenSeqAttribute*const* tokenSeqAttributes,
                               UINT32                    tokenSeqAttributeCount,
                               ITokenSeqAttribute**      mergedTokenSeqAttribute );

private:
  virtual query::TStatus
  doMakeEmptyTokenSeqAttribute( ITokenSeqAttribute*& emptyTokenSeqAttribute ) = 0;

  virtual query::TStatus
  doMakeMergedTokenSeqAttribute( ITokenSeqAttribute*const tokenSeqAttributes[],
                                 UINT32                   tokenSeqAttributeCount,
                                 ITokenSeqAttribute*&     mergedTokenSeqAttribute ) = 0;
};

extern const GUID
IID_IRoadTokenSeqAttributeFactory;

extern const GUID
IID_IRouteTokenSeqAttributeFactory;

extern const GUID
IID_IPlaceTokenSeqAttributeFactory;

extern const GUID
IID_IPOITokenSeqAttributeFactory;

extern const GUID
IID_IPOITypeTokenSeqAttributeFactory;

//--------------------------------------------------------------------------------
class ITokenPartSeqQueryFactory : public query::IObject {

public:
  query::TStatus
  makeEmptyTokenPartSeqQuery( ITokenPartSeqQuery** emptyTokenPartSeqQuery );

  query::TStatus
  makeExtendedTokenPartSeqQuery( ITokenPartSeqQuery*  tokenPartSeqQuery,
                                 ITokenPartQuery*     tokenPartQuery,
                                 ITokenPartSeqQuery** extendedTokenPartSeqQuery );

  query::TStatus
  makeMergedTokenPartSeqQuery( ITokenPartSeqQuery*const* tokenPartSeqQueries,
                               UINT32                    tokenPartSeqQueryCount,
                               ITokenPartSeqQuery**      mergedTokenPartSeqQuery );

private:
  virtual query::TStatus
  doMakeEmptyTokenPartSeqQuery( ITokenPartSeqQuery*& emptyTokenPartSeqQuery ) = 0;

  virtual query::TStatus
  doMakeExtendedTokenPartSeqQuery( ITokenPartSeqQuery*  tokenPartSeqQuery,
                                   ITokenPartQuery*     tokenPartQuery,
                                   ITokenPartSeqQuery*& extendedTokenPartSeqQuery ) = 0;

  virtual query::TStatus
  doMakeMergedTokenPartSeqQuery( ITokenPartSeqQuery*const tokenPartSeqQueries[],
                                 UINT32                   tokenPartSeqQueryCount,
                                 ITokenPartSeqQuery*&     mergedTokenPartSeqQuery ) = 0;
};

extern const GUID
IID_IRoadTokenPartSeqQueryFactory;

extern const GUID
IID_IRouteTokenPartSeqQueryFactory;

extern const GUID
IID_IPlaceTokenPartSeqQueryFactory;

extern const GUID
IID_IPOITokenPartSeqQueryFactory;

extern const GUID
IID_IPOITypeTokenPartSeqQueryFactory;

//--------------------------------------------------------------------------------
class ITokenPartQueryFactory : public query::IObject {

public:
  query::TStatus
  makeSpellingTokenPartQuery( BOOL8                       allowPartialTokenPart,
                              ISpellingCharacterSeqQuery* spellingCharacterSeqQuery,
                              ITokenPartQuery**           tokenPartQuery );

  query::TStatus
  makeMergedTokenPartQuery( ITokenPartQuery*const* tokenPartQueries,
                            UINT32                 tokenPartQueryCount,
                            ITokenPartQuery**      mergedTokenPartQuery );

private:
  virtual query::TStatus
  doMakeSpellingTokenPartQuery( BOOL8                       allowPartialTokenPart,
                                ISpellingCharacterSeqQuery* spellingCharacterSeqQuery,
                                ITokenPartQuery*&           tokenPartQuery ) = 0;

  virtual query::TStatus
  doMakeMergedTokenPartQuery( ITokenPartQuery*const tokenPartQueries[],
                              UINT32                tokenPartQueryCount,
                              ITokenPartQuery*&     mergedTokenPartQuery ) = 0;
};

extern const GUID
IID_ITokenPartQueryFactory;

extern const GUID
IID_IPOITokenPartQueryFactory;

extern const GUID
IID_IPOITypeTokenPartQueryFactory;

//--------------------------------------------------------------------------------
class ISpellingCharacterSeqQueryFactory : public query::IObject {

public:
  query::TStatus
  makeEmptySpellingCharacterSeqQuery( ISpellingCharacterSeqQuery** emptySpellingCharacterSeqQuery );

  query::TStatus
  makeExtendedSpellingCharacterSeqQuery( ISpellingCharacterSeqQuery*  spellingCharacterSeqQuery,
                                         IUnicodeQuery*               spellingCharacterQuery,
                                         ISpellingCharacterSeqQuery** extendedSpellingCharacterSeqQuery );

  query::TStatus
  makeMergedSpellingCharacterSeqQuery( ISpellingCharacterSeqQuery*const* spellingCharacterSeqQueries,
                                       UINT32                            spellingCharacterSeqQueryCount,
                                       ISpellingCharacterSeqQuery**      mergedSpellingCharacterSeqQuery );

private:
  virtual query::TStatus
  doMakeEmptySpellingCharacterSeqQuery( ISpellingCharacterSeqQuery*& emptySpellingCharacterSeqQuery ) = 0;

  virtual query::TStatus
  doMakeExtendedSpellingCharacterSeqQuery( ISpellingCharacterSeqQuery*  spellingCharacterSeqQuery,
                                           IUnicodeQuery*               spellingCharacterQuery,
                                           ISpellingCharacterSeqQuery*& extendedSpellingCharacterSeqQuery ) = 0;

  virtual query::TStatus
  doMakeMergedSpellingCharacterSeqQuery( ISpellingCharacterSeqQuery*const spellingCharacterSeqQueries[],
                                         UINT32                           spellingCharacterSeqQueryCount,
                                         ISpellingCharacterSeqQuery*&     mergedSpellingCharacterSeqQuery ) = 0;
};

extern const GUID
IID_ISpellingCharacterSeqQueryFactory;

extern const GUID
IID_IPOISpellingCharacterSeqQueryFactory;

extern const GUID
IID_IPOITypeSpellingCharacterSeqQueryFactory;

//--------------------------------------------------------------------------------
class ISpellingCharacterQueryFactory : public query::IObject {

public:
  query::TStatus
  makeSpellingCharacterQuery( parser::ITerminal* unicodeTerminal,
                              IUnicodeQuery**    spellingCharacterQuery );

private:
  virtual query::TStatus
  doMakeSpellingCharacterQuery( parser::ITerminal* unicodeTerminal,
                                IUnicodeQuery*&    spellingCharacterQuery ) = 0;
};

extern const GUID
IID_ISpellingCharacterQueryFactory;

//--------------------------------------------------------------------------------
class IPlaceAddress : public query::IObject {

public:
  UINT16
  getRoadMapIndex();

  TRoadPlaceNameIndex16
  getPlaceNameIndex();

  const TTokenHighlight*
  getMatchedPlaceNameTokens( UINT32* matchedTokenPositions );

  TGeosearchPlaceColumnMask
  getPlaceColumns();

  UINT16
  getPlaceNamePlaceRecordIndex();

  const TSearchArea*
  getSearchArea();

  TNumericISOCountryCode
  getNumericISOCountryCode();

private:
  virtual UINT16
  doGetRoadMapIndex() = 0;

  virtual TRoadPlaceNameIndex16
  doGetPlaceNameIndex() = 0;

  virtual const TTokenHighlight*
  doGetMatchedPlaceNameTokens( UINT32& matchedTokenPositions ) = 0;

  virtual TGeosearchPlaceColumnMask
  doGetPlaceColumns() = 0;

  virtual UINT16
  doGetPlaceNamePlaceRecordIndex() = 0;

  virtual const TSearchArea*
  doGetSearchArea() = 0;

  virtual TNumericISOCountryCode
  doGetNumericISOCountryCode() = 0;
};

extern const GUID
IID_IPlaceAddress;

//--------------------------------------------------------------------------------
class IPostalCodeAddress : public query::IObject {

public:
  BOOL8
  getFlagDetailed();

  UINT16
  getMapIndex();

  TPostalCodeIndex32
  getPostalCodeIndex();

  UINT8
  getMatchLength();

  const TSearchArea*
  getSearchArea();

private:
  virtual BOOL8
  doGetFlagDetailed() = 0;

  virtual UINT16
  doGetMapIndex() = 0;

  virtual TPostalCodeIndex32
  doGetPostalCodeIndex() = 0;

  virtual UINT8
  doGetMatchLength() = 0;

  virtual const TSearchArea*
  doGetSearchArea() = 0;
};

extern const GUID
IID_IPostalCodeAddress;

//--------------------------------------------------------------------------------
class IRoadAddress : public query::IObject {

public:
  UINT16
  getRoadMapIndex();

  TRoadRoadIndex32
  getRoadIndex();

  TRoadRoadNameIndex32
  getRoadNameIndex();

  const TTokenHighlight*
  getMatchedRoadNameTokens( UINT32* matchedTokenPositions );

  TRoadSectorPlacePairSetIndex32
  getSectorPlacePairSetIndex();

  const TAreaMatchTypePair*
  getSectorPlacePairSetAreaMatchTypes( UINT32* sectorPlacePairCount );

  const TRect*
  getGlueBounds();

  const TSearchArea*
  getSearchArea();

  const TPlaceQualifier*
  getPlaceQualifier();

  TAreaMatchType
  getAreaMatchType();

private:
  virtual UINT16
  doGetRoadMapIndex() = 0;

  virtual TRoadRoadIndex32
  doGetRoadIndex() = 0;

  virtual TRoadRoadNameIndex32
  doGetRoadNameIndex() = 0;

  virtual const TTokenHighlight*
  doGetMatchedRoadNameTokens( UINT32& matchedTokenPositions ) = 0;

  virtual TRoadSectorPlacePairSetIndex32
  doGetSectorPlacePairSetIndex() = 0;

  virtual const TAreaMatchTypePair*
  doGetSectorPlacePairSetAreaMatchTypes( UINT32& sectorPlacePairCount ) = 0;

  virtual const TRect*
  doGetGlueBounds() = 0;

  virtual const TSearchArea*
  doGetSearchArea() = 0;

  virtual const TPlaceQualifier*
  doGetPlaceQualifier() = 0;

  virtual TAreaMatchType
  doGetAreaMatchType() = 0;
};

extern const GUID
IID_IRoadAddress;

//--------------------------------------------------------------------------------
class IHouseNumberAddress : public query::IObject {

public:
  IHouseNumberEntity*
  getHouseNumberEntity();

  TAreaMatchType
  getAreaMatchType();

  BOOL8
  getFlagMixedRange();

  UINT32
  getStartNumber();

  UINT32
  getEndNumber();

  THouseNumberAddressFormat
  getHouseNumberAddressFormat();

  UINT32
  getHouseNumberFilter();

  TRoadRoadNameIndex32
  getRoadNameIndex();

  const TTokenHighlight*
  getMatchedRoadNameTokens( UINT32* matchedTokenPositions );

  const TSearchArea*
  getSearchArea();

  const TPlaceQualifier*
  getPlaceQualifier();

private:
  virtual IHouseNumberEntity*
  doGetHouseNumberEntity() = 0;

  virtual TAreaMatchType
  doGetAreaMatchType() = 0;

  virtual BOOL8
  doGetFlagMixedRange() = 0;

  virtual UINT32
  doGetStartNumber() = 0;

  virtual UINT32
  doGetEndNumber() = 0;

  virtual THouseNumberAddressFormat
  doGetHouseNumberAddressFormat() = 0;

  virtual UINT32
  doGetHouseNumberFilter() = 0;

  virtual TRoadRoadNameIndex32
  doGetRoadNameIndex() = 0;

  virtual const TTokenHighlight*
  doGetMatchedRoadNameTokens( UINT32& matchedTokenPositions ) = 0;

  virtual const TSearchArea*
  doGetSearchArea() = 0;

  virtual const TPlaceQualifier*
  doGetPlaceQualifier() = 0;
};

extern const GUID
IID_IHouseNumberAddress;

//--------------------------------------------------------------------------------
class IIntersectionAddress : public query::IObject {

public:
  UINT16
  getRoadMapIndex();

  TAreaMatchType
  getAreaMatchType();

  TRoadRoadNameIndex32
  getFirstRoadNameIndex();

  const TTokenHighlight*
  getFirstMatchedRoadNameTokens( UINT32* matchedTokenPositions );

  TRoadRoadNameIndex32
  getSecondRoadNameIndex();

  const TTokenHighlight*
  getSecondMatchedRoadNameTokens( UINT32* matchedTokenPositions );

  const TSearchArea*
  getSearchArea();

  IIntersectionEntity*
  getIntersectionEntity();

private:
  virtual UINT16
  doGetRoadMapIndex() = 0;

  virtual TAreaMatchType
  doGetAreaMatchType() = 0;

  virtual TRoadRoadNameIndex32
  doGetFirstRoadNameIndex() = 0;

  virtual const TTokenHighlight*
  doGetFirstMatchedRoadNameTokens( UINT32& matchedTokenPositions ) = 0;

  virtual TRoadRoadNameIndex32
  doGetSecondRoadNameIndex() = 0;

  virtual const TTokenHighlight*
  doGetSecondMatchedRoadNameTokens( UINT32& matchedTokenPositions ) = 0;

  virtual const TSearchArea*
  doGetSearchArea() = 0;

  virtual IIntersectionEntity*
  doGetIntersectionEntity() = 0;
};

extern const GUID
IID_IIntersectionAddress;

//--------------------------------------------------------------------------------
class IPOITypeAddress : public query::IObject {

public:
  UINT16
  getPOITypeMapIndex();

  TPoiTypeNameIndex16
  getPOITypeNameIndex();

  const TTokenHighlight*
  getMatchedPOITypeNameTokens( UINT32* matchedTokenPositions );

  UINT16
  getPOIMapIndex();

  TPOITypeIndex16
  getPOITypeIndex();

  TGeosearchResultType
  getSearchStringMatchType();

private:
  virtual UINT16
  doGetPOITypeMapIndex() = 0;

  virtual TPoiTypeNameIndex16
  doGetPOITypeNameIndex() = 0;

  virtual const TTokenHighlight*
  doGetMatchedPOITypeNameTokens( UINT32& matchedTokenPositions ) = 0;

  virtual UINT16
  doGetPOIMapIndex() = 0;

  virtual TPOITypeIndex16
  doGetPOITypeIndex() = 0;

  virtual TGeosearchResultType
  doGetSearchStringMatchType() = 0;
};

extern const GUID
IID_IPOITypeAddress;

//--------------------------------------------------------------------------------
class IUserPOITypeAddress : public query::IObject {

public:
  IUserPOISearch*
  getUserPOISearch();

  IUserPOISearch::TResult
  getUserPOIType();

  UINT32
  getMatchLength();

  TGeosearchResultType
  getSearchStringMatchType();

private:
  virtual IUserPOISearch*
  doGetUserPOISearch() = 0;

  virtual IUserPOISearch::TResult
  doGetUserPOIType() = 0;

  virtual UINT32
  doGetMatchLength() = 0;

  virtual TGeosearchResultType
  doGetSearchStringMatchType() = 0;
};

extern const GUID
IID_IUserPOITypeAddress;

//--------------------------------------------------------------------------------
class IPOIAddress : public query::IObject {

public:
  IPOIEntity*
  getPOIEntity();

  UINT32
  getPOINameIndex();

  const TTokenHighlight*
  getMatchedPOINameTokens( UINT32* matchedTokenPositions );

  //TODO remove getPOITypeIndex()?
  TPOITypeIndex16
  getPOITypeIndex();

  const TSearchArea*
  getSearchArea();

  const TPlaceQualifier*
  getPlaceQualifier();

  TAreaMatchType
  getAreaMatchType();

private:
  virtual IPOIEntity*
  doGetPOIEntity() = 0;

  virtual UINT32
  doGetPOINameIndex() = 0;

  virtual const TTokenHighlight*
  doGetMatchedPOINameTokens( UINT32& matchedTokenPositions ) = 0;

  //TODO remove doGetPOITypeIndex()?
  virtual TPOITypeIndex16
  doGetPOITypeIndex() = 0;

  virtual const TSearchArea*
  doGetSearchArea() = 0;

  virtual const TPlaceQualifier*
  doGetPlaceQualifier() = 0;

  virtual TAreaMatchType
  doGetAreaMatchType() = 0;
};

extern const GUID
IID_IPOIAddress;

//--------------------------------------------------------------------------------
class IUserPOIAddress : public query::IObject {

public:
  IUserPOISearch*
  getUserPOISearch();

  IUserPOISearch::TResult
  getUserPOI();

  UINT32
  getMatchLength();

private:
  virtual IUserPOISearch*
  doGetUserPOISearch() = 0;

  virtual IUserPOISearch::TResult
  doGetUserPOI() = 0;

  virtual UINT32
  doGetMatchLength() = 0;
};

extern const GUID
IID_IUserPOIAddress;

//--------------------------------------------------------------------------------
class INearestPOIAddress : public query::IObject {

public:
  IPOIEntity*
  getPOIEntity();

  TPOINameIndex32
  getPOINameIndex();

  TPOITypeIndex16
  getPOITypeIndex();

  //TAreaMatchType
  //getAreaMatchType();

  UINT32
  getDetour();

  UINT32
  getDistance();

  UINT32
  getETA();

private:
  virtual IPOIEntity*
  doGetPOIEntity() = 0;

  virtual TPOINameIndex32
  doGetPOINameIndex() = 0;

  virtual TPOITypeIndex16
  doGetPOITypeIndex() = 0;

  //virtual TAreaMatchType
  //doGetAreaMatchType() = 0;

  virtual UINT32
  doGetDetour() = 0;

  virtual UINT32
  doGetDistance() = 0;

  virtual UINT32
  doGetETA() = 0;
};

extern const GUID
IID_INearestPOIAddress;

//--------------------------------------------------------------------------------
class INearestUserPOIAddress : public query::IObject {

public:
  IUserPOISearch*
  getUserPOISearch();

  IUserPOISearch::TResult
  getUserPOI();

  UINT32
  getMatchLength();

  UINT32
  getDistance();

  UINT32
  getETA();

private:
  virtual IUserPOISearch*
  doGetUserPOISearch() = 0;

  virtual IUserPOISearch::TResult
  doGetUserPOI() = 0;

  virtual UINT32
  doGetMatchLength() = 0;

  virtual UINT32
  doGetDistance() = 0;

  virtual UINT32
  doGetETA() = 0;
};

extern const GUID
IID_INearestUserPOIAddress;

//--------------------------------------------------------------------------------
class IActivePOIAddress : public query::IObject {

public:
  IPOIEntity*
  getPOIEntity();

  TPOITypeIndex16
  getPOITypeIndex();

  //TAreaMatchType
  //getAreaMatchType();

  UINT32
  getDistance();

private:
  virtual IPOIEntity*
  doGetPOIEntity() = 0;

  virtual TPOITypeIndex16
  doGetPOITypeIndex() = 0;

  //virtual TAreaMatchType
  //doGetAreaMatchType() = 0;

  virtual UINT32
  doGetDistance() = 0;
};

extern const GUID
IID_IActivePOIAddress;

//--------------------------------------------------------------------------------
class IActiveUserPOIAddress : public query::IObject {

public:
  IUserPOISearch*
  getUserPOISearch();

  IUserPOISearch::TResult
  getUserPOI();

  UINT32
  getDistance();

private:
  virtual IUserPOISearch*
  doGetUserPOISearch() = 0;

  virtual IUserPOISearch::TResult
  doGetUserPOI() = 0;

  virtual UINT32
  doGetDistance() = 0;
};

extern const GUID
IID_IActiveUserPOIAddress;

//--------------------------------------------------------------------------------
class ILatLongAddress : public query::IObject {

public:
  TPos
  getLatLongPosition();

private:
  virtual TPos
  doGetLatLongPosition() = 0;
};

extern const GUID
IID_ILatLongAddress;

//--------------------------------------------------------------------------------
class IPOITypeNameMatchCache : public query::IObject {

public:
  struct TPOITypeNameMatch {
    UINT16 POITypeMapIndex;
    TName  POITypeName;
  };

public:
  query::TStatus
  addMatchedPOITypeName( const TPOITypeNameMatch& POITypeNameMatch );

  UINT32
  initMatchedPOITypeFilter( UINT16   POIMapIndex,
                            TBitset* POITypeFilterBitset );

  BOOL8
  getMatchedPOITypeNamesForPOITypeId( TPOITypeId16              POITypeId,
                                      const TPOITypeNameMatch** POITypeNameMatches,
                                      UINT32*                   POITypeNameMatchCount );

  void
  removeMatchedPOITypeNamesForPOITypeId( TPOITypeId16 POITypeId );

private:
  virtual query::TStatus
  doAddMatchedPOITypeName( const TPOITypeNameMatch& POITypeNameMatch ) = 0;

  virtual UINT32
  doInitMatchedPOITypeFilter( UINT16   POIMapIndex,
                              TBitset* POITypeFilterBitset ) = 0;

  virtual BOOL8
  doGetMatchedPOITypeNamesForPOITypeId( TPOITypeId16              POITypeId,
                                        const TPOITypeNameMatch*& POITypeNameMatches,
                                        UINT32&                   POITypeNameMatchCount ) = 0;

  virtual void
  doRemoveMatchedPOITypeNamesForPOITypeId( TPOITypeId16 POITypeId ) = 0;
};

//--------------------------------------------------------------------------------
class INearestPOINameMatchFilterCache : public query::IObject {

public:
  query::TStatus
  setPOINameMatchFilter( UINT16         POIMapIndex,
                         const TBitset* POINameMatchFilterBitset );

  query::TStatus
  getPOINameMatchFilter( UINT16          POIMapIndex,
                         const TBitset** POINameMatchFilterBitset );

private:
  virtual query::TStatus
  doSetPOINameMatchFilter( UINT16         POIMapIndex,
                           const TBitset* POINameMatchFilterBitset ) = 0;

  virtual query::TStatus
  doGetPOINameMatchFilter( UINT16          POIMapIndex,
                           const TBitset*& POINameMatchFilterBitset ) = 0;
};

//--------------------------------------------------------------------------------
class INearestPOIEntityFactory : public query::IObject {

public:
  struct TNearestPOIEntityMatch {
    UINT16             POIMapIndex;
    TPOITypeSetIndex16 POITypeSetIndex;
    TPOINameSetIndex32 POINameSetIndex;
    TRoadPlaceIndex16  placeIndex;
    UINT16             entrySegmentProportion;
    UINT32             entrySegmentId;
    UINT32             POIId;
    UINT32             POIContentOffset;
    UINT32             matchedNameFilter; //identifies which of the name set names matched (max 32 names).
  };

public:
  query::TStatus
  addCandidateNearestPOIEntityMatch( const TNearestPOIEntityMatch& nearestPOIEntityMatch );

  query::TStatus
  getFinalisedNearestPOIEntityMatch( IPOIEntity** nearestPOIEntity,
                                     UINT32*      matchedNameFilter,
                                     UINT32*      detour,
                                     UINT32*      distance,
                                     UINT32*      ETA );

  void
  updateSegmentCosts( TSearchNode*                      searchNode,
                      UINT32                            segmentId,
                      UINT32                            travelProp,
                      BOOL8                             fromFirstNode,
                      const TPathCalculatorExploreCost* searchCost,
                      const TPathCalculatorExploreCost* segmentCost );

private:
  virtual query::TStatus
  doAddCandidateNearestPOIEntityMatch( const TNearestPOIEntityMatch& nearestPOIEntityMatch ) = 0;

  virtual query::TStatus
  doGetFinalisedNearestPOIEntityMatch( IPOIEntity*& nearestPOIEntity,
                                       UINT32&      matchedNameFilter,
                                       UINT32&      detour,
                                       UINT32&      distance,
                                       UINT32&      ETA ) = 0;

  virtual void
  doUpdateSegmentCosts( TSearchNode*                      searchNode,
                        UINT32                            segmentId,
                        UINT32                            travelProp,
                        BOOL8                             fromFirstNode,
                        const TPathCalculatorExploreCost* searchCost,
                        const TPathCalculatorExploreCost* segmentCost ) = 0;
};

//--------------------------------------------------------------------------------
class INearestPOISectorScanList : public query::IObject {

public:
  query::TStatus
  addSegment( TRoadSegment* segment );

  void
  beginPOISectorScanIterator();

  query::TStatus
  getNextPOISectorScanFilter( UINT16*         POIMapIndex,
                              const TBitset** sectorScanFilterBitset );

  void
  endPOISectorScanIterator();

private:
  virtual query::TStatus
  doAddSegment( TRoadSegment* segment ) = 0;

  virtual void
  doBeginPOISectorScanIterator() = 0;

  virtual query::TStatus
  doGetNextPOISectorScanFilter( UINT16&         POIMapIndex,
                                const TBitset*& sectorScanFilterBitset ) = 0;

  virtual void
  doEndPOISectorScanIterator() = 0;
};

//--------------------------------------------------------------------------------
class INearestPOINameMatchList : public query::IObject {

public:
  query::TStatus
  addMatchedPOIName( TGenericMapInfo* POIMapInfo,
                     TPOINameIndex32  POINameIndex );

  query::TStatus
  addMatchedPOINameTokenData( TGenericMapInfo*       POIMapInfo,
                              TPOINameIndex32        POINameIndex,
                              UINT32                 matchedTokenPositions,
                              const TTokenHighlight* matchedTokens );

  query::TStatus
  getMatchedPOINameTokenData( TGenericMapInfo* POIMapInfo,
                              TPOINameIndex32  POINameIndex,
                              UINT32*          matchedTokenPositions,
                              TTokenHighlight* matchedTokens );

  query::TStatus
  initMatchedPOINameBitsetIterator();

  query::TStatus
  getNextMatchedPOINameBitset( TGenericMapInfo** POIMapInfo,
                               const TBitset**   matchedPOINameBitset );

private:
  virtual query::TStatus
  doAddMatchedPOIName( TGenericMapInfo* POIMapInfo,
                       TPOINameIndex32  POINameIndex ) = 0;

  virtual query::TStatus
  doAddMatchedPOINameTokenData( TGenericMapInfo*       POIMapInfo, 
                                TPOINameIndex32        POINameIndex, 
                                UINT32                 matchedTokenPositions,
                                const TTokenHighlight* matchedTokens ) = 0;

  virtual query::TStatus
  doGetMatchedPOINameTokenData( TGenericMapInfo* POIMapInfo,
                                TPOINameIndex32  POINameIndex, 
                                UINT32*          matchedTokenPositions,
                                TTokenHighlight* matchedTokens ) = 0;

  virtual query::TStatus
  doInitMatchedPOINameBitsetIterator() = 0;

  virtual query::TStatus
  doGetNextMatchedPOINameBitset( TGenericMapInfo** POIMapInfo,
                                 const TBitset**   matchedPOINameBitset ) = 0;
};

//--------------------------------------------------------------------------------
class ISegmentLocationSearchCallback : public query::IObject {

public:
  struct TVisitSegmentData {
    UINT16             roadMapIndex;
    UINT16             sectorIndex;
    TGenericMapInfo*   roadMapInfo;
    TRoadSectorHeader* sector;
    UINT32             segmentPtr;
    TRoadSegment       segment;
    TRect              searchBounds;      //TODO should this be here?
    TPos               searchCentroid;    //TODO should this be here?
  };

public:
  BOOL8
  getSectorMatchFlag( UINT16 roadMapIndex,
                      UINT16 sectorIndex );

  UINT64
  getBestDistanceSquared();

  query::TStatus
  visitSegment( const TVisitSegmentData& visitSegmentData );

private:
  virtual BOOL8
  doGetSectorMatchFlag( UINT16 roadMapIndex,
                        UINT16 sectorIndex ) = 0;

  virtual UINT64
  doGetBestDistanceSquared() = 0;

  virtual query::TStatus
  doVisitSegment( const TVisitSegmentData& visitSegmentData ) = 0;
};

//--------------------------------------------------------------------------------
class IAggregatorInit : public query::IObject {

public:
  query::TStatus
  addAggregatedInterface( REFIID          iid,
                          query::IObject* object );

private:
  virtual query::TStatus
  doAddAggregatedInterface( REFIID          iid,
                            query::IObject* object ) = 0;
};

class CGlobalforGS{
public:
  static int placeIdxIS;
};

//--------------------------------------------------------------------------------
} //namespace geosearch
} //namespace innov

#endif //INCLUDE_GUARD_gsTypes_h
