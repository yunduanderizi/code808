/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_gsClassFactory_h
#define INCLUDE_GUARD_gsClassFactory_h

#include <geosearch/gsTypes.h>

namespace innov {
namespace geosearch {

//--------------------------------------------------------------------------------
//extern INT32 placeIdxIS = 0;

//--------------------------------------------------------------------------------
query::TStatus
makeActivePOIAddress( IAllocator*         allocator,
                      IPOIEntity*         POIEntity,
                      TPOITypeIndex16     POITypeIndex,
                    //TAreaMatchType      areaMatchType,
                      UINT32              distance,
                      IActivePOIAddress** activePOIAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeActivePOIEntity( IAllocator*        allocator,
                     UINT16             POIMapIndex,
                     TPOINameSetIndex32 POINameSetIndex,
                     TPOISectorIndex16  POISectorIndex,
                     TRoadPlaceIndex16  placeIndex,
                     TPOITypeSetIndex16 POITypeSetIndex,
                     UINT32             POIId,
                     UINT32             POIContentOffset,
                     TPos               POIPosition,
                     IPOIEntity**       activePOIEntity );

//--------------------------------------------------------------------------------
query::TStatus
makeActivePOIResult( IAllocator*      allocator,
                     IStructuredText* resultName,
                     IPOIEntity*      POIEntity,
                     TPOITypeIndex16  POITypeIndex,
                     UINT32           distance,
                     ISearchResult**  activePOIResult );

//--------------------------------------------------------------------------------
query::TStatus
makeActivePOIResultGroup( IAllocator*        allocator,
                          IAllocator*        outputAllocator,
                          const UTF16*       resultGroupKey,
                          IResultGroupCache* resultGroupCache,
                          IResultGroup**     activePOIResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeActivePOIResultGroupFactory( IAllocator*           allocator,
                                 IAllocator*           outputAllocator,
                                 IResultGroupFactory** resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeActivePOISearchEngine( IAllocator*         allocator,
                           IAllocator*         outputAllocator,
                           const TRect*        searchBounds,
                           const TPOITypeId16* POITypes,
                           UINT32              POITypeCount,
                           UINT32              maxDistance,
                           IResultListBuilder* resultListBuilder,
                           IResultGroupCache*  activePOIResultGroupCache,
                           query::IEngine*     tailEngine,
                           query::IEngine**    searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeActivePOISearchParameters( IAllocator*                  allocator,
                               IPlaceFilterCache*           placeFilterCache,
                               const TRect*                 searchBounds,
                               TPOITypeId16                 POITypeId,
                               UINT32                       maxDistance,
                               IActivePOISearchParameters** activePOISearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeActiveUserPOIAddress( IAllocator*             allocator,
                          IUserPOISearch*         userPOISearch,
                          IUserPOISearch::TResult userPOI,
                          UINT32                  distance,
                          IActiveUserPOIAddress** activeUserPOIAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeActiveUserPOIResult( IAllocator*             allocator,
                         IStructuredText*        resultName,
                         IUserPOISearch*         userPOISearch,
                         IUserPOISearch::TResult userPOI,
                         UINT32                  distance,
                         ISearchResult**         activeUserPOIResult );

//--------------------------------------------------------------------------------
query::TStatus
makeActiveUserPOIResultGroup( IAllocator*        allocator,
                              IAllocator*        outputAllocator,
                              const UTF16*       resultGroupKey,
                              IResultGroupCache* resultGroupCache,
                              IResultGroup**     activeUserPOIResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeActiveUserPOIResultGroupFactory( IAllocator*           allocator,
                                     IAllocator*           outputAllocator,
                                     IResultGroupFactory** resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeActiveUserPOISearchEngine( IAllocator*         allocator,
                               const TRect*        searchBounds,
                               const TPOITypeId16* POITypes,
                               UINT32              POITypeCount,
                               UINT32              maxDistance,
                               void*               userPOIContext,
                               IResultListBuilder* resultListBuilder,
                               IResultGroupCache*  activeUserPOIResultGroupCache,
                               query::IEngine*     tailEngine,
                               query::IEngine**    searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeActiveUserPOISearchParameters( IAllocator*                      allocator,
                                   const TRect*                     searchBounds,
                                   TPOITypeId16                     POITypeId,
                                   UINT32                           maxDistance,
                                   void*                            userPOIContext,
                                   IActiveUserPOISearchParameters** activeUserPOISearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeAddressParserCallback( IAllocator*               allocator,
                           IEngineFactoryQueue*      engineFactoryQueue,
                           query::IEngine*           tailEngine,
                           parser::IParserCallback** parserCallback );

//--------------------------------------------------------------------------------
query::TStatus
makeAddressParserEngineFactory( IAllocator*                                 allocator,
                                IAllocator*                                 outputAllocator,
                                IResultListBuilder*                         resultListBuilder,
                                TResultCostMode                             resultCostMode,
                                const UTF16*                                searchString,
                                const TRoadAddressParameters&               roadAddressParameters,
                                const TPlaceAddressParameters&              placeAddressParameters,
                                const TPostalCodeAddressParameters&         postalCodeAddressParameters,
                                const TDetailedPostalCodeAddressParameters& detailedPostalCodeAddressParameters,
                                const TIntersectionAddressParameters&       intersectionAddressParameters,
                                const THouseNumberAddressParameters&        houseNumberAddressParameters,
                                const TPOIAddressParameters&                POIAddressParameters,
                                const TPOIAddressGroupByPOITypeParameters&  POIAddressGroupByPOITypeParameters,
                                UINT16                                      roadMapIndex,
                                UINT16                                      pocodeMapIndex,
                                UINT16                                      POIMapIndex,
                                const TTuningParameters*                    tuningParameters,
                                IEngineFactory**                            addressParserEngineFactory );
//--------------------------------------------------------------------------------
//for TW/CN first-phonetic search
query::TStatus
makeFirstPhoneticAddressParserEngineFactory( IAllocator*                                 allocator,
                                           IAllocator*                                 outputAllocator,
                                           IResultListBuilder*                         resultListBuilder,
                                           TResultCostMode                             resultCostMode,
                                           const UTF16*                                searchString,
                                           const TRoadAddressParameters&               roadAddressParameters,
                                           const TPlaceAddressParameters&              placeAddressParameters,
                                           const TPostalCodeAddressParameters&         postalCodeAddressParameters,
                                           const TDetailedPostalCodeAddressParameters& detailedPostalCodeAddressParameters,
                                           const TIntersectionAddressParameters&       intersectionAddressParameters,
                                           const THouseNumberAddressParameters&        houseNumberAddressParameters,
                                           const TPOIAddressParameters&                POIAddressParameters,
                                           const TPOIAddressGroupByPOITypeParameters&  POIAddressGroupByPOITypeParameters,
                                           UINT16                                      roadMapIndex,
                                           UINT16                                      pocodeMapIndex,
                                           UINT16                                      POIMapIndex,
                                           const TTuningParameters*                    tuningParameters,
                                           IEngineFactory**                            addressParserEngineFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeAddressSearchParameters( IAllocator*                            allocator,
                             IAddressSearchParameters::TAddressType addressType,
                             const UTF16*                           searchString,
                             IPlaceFilterCache*                     placeFilterCache,
                             TGeosearchPlaceColumnMask              placeSearchColumns,
                             TPOITypeId16                           POITypeId,
                             const TTuningParameters*               tuningParameters,
                             IAddressSearchParameters**             addressSearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeAggregator( IAllocator*       allocator,
                IAggregatorInit** aggregatorInit );

//--------------------------------------------------------------------------------
query::TStatus
makeCountryStateEntityCache( IAllocator*         allocator,
                             IAllocator*         outputAllocator,
                             ISearchLocation*    searchLocation,
                             IPlaceEntityCache** countryStateEntityCache );

//--------------------------------------------------------------------------------
query::TStatus
makeCountryStateSearchEngine( IAllocator*               allocator,
                              UINT16                    roadMapIndex,
                              IResultListBuilder*       resultListBuilder,
                              IResultGroupCache*        placeResultGroupCache,
                              const TTuningParameters*  tuningParameters,
                              TGeosearchPlaceColumnMask placeSearchColumns,
                              query::IEngine*           tailEngine,
                              query::IEngine**          countryStateSearchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeCountryStateSearchEngineFactory( IAllocator*               allocator,
                                     UINT16                    roadMapIndex,
                                     IResultListBuilder*       resultListBuilder,
                                     IResultGroupCache*        placeResultGroupCache,
                                     const TTuningParameters*  tuningParameters,
                                     TGeosearchPlaceColumnMask placeSearchColumns,
                                     IEngineFactory**          countryStateSearchEngineFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeDetailedPostalCodeEntity( IAllocator*                 allocator,
                              const TDetailedPostalCode*  detailedPostalCodes,
                              UINT32                      detailedPostalCodeCount,
                              IDetailedPostalCodeEntity** detailedPostalCodeEntity );

//--------------------------------------------------------------------------------
query::TStatus
makeDetailedPostalCodeResult( IAllocator*                allocator,
                              IStructuredText*           resultName,
                              IDetailedPostalCodeEntity* detailedPostalCodeEntity,
                              ISearchResult**            detailedPostalCodeResult );

query::TStatus
makeDetailedPostalCodeResult( IAllocator*                allocator,
                              IStructuredText*           resultName,
                              IDetailedPostalCodeEntity* detailedPostalCodeEntity,
                              UINT32                     locationSegmentPtr,
                              UINT32                     locationSegmentRoadSet,
                              UINT16                     locationSegmentPlace,
                              UINT16                     locationSegmentProportion,
                              TPos                       locationPosition,
                              ISearchResult**            detailedPostalCodeResult );

//--------------------------------------------------------------------------------
query::TStatus
makeEmptyDetailedPostalCodeUnicodeSeq( IAllocator*             allocator,
                                       TGenericMapInfo        *pocodeMapHeader,
                                       IPostalCodeUnicodeSeq** emptyDetailedPostalCodeUnicodeSeq );
//NOTE this function is implemented in
//     <geosearch/gsDetailedPostalCodeUnicodeSeq.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeEmptyPostalCodeUnicodeSeq( IAllocator*             allocator,
                               TGenericMapInfo*        roadMapInfo,
                               IPostalCodeUnicodeSeq** emptyPostalCodeUnicodeSeq );
//NOTE this function is implemented in
//     <geosearch/gsPostalCodeUnicodeSeq.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeEmptyPostalCodeUnicodeSeqQuery( IAllocator*                  allocator,
                                    const TPocodeDataSource*     pocodeSource,
                                    IPostalCodeUnicodeSeqQuery** emptyPostalCodeUnicodeSeqQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeEmptySpellingCharacterSeq( IAllocator*             allocator,
                               INameDataSource*        tokenPartDataSource,
                               ISpellingCharacterSeq** emptySpellingCharacterSeq );
//NOTE this function is implemented in
//     <geosearch/gsSpellingCharacterSeq.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeEmptySpellingCharacterSeqQuery( IAllocator*                  allocator,
                                    INameDataSource*             tokenPartDataSource,
                                    ISpellingCharacterSeqQuery** emptySpellingCharacterSeqQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeEmptyTokenPartSeq( IAllocator*      allocator,
                       INameDataSource* tokenDataSource,
                       BOOL8            isRoute,
                       BOOL8            exactMatch,
                       ITokenPartSeq**  emptyTokenPartSeq );
//NOTE this function is implemented in
//     <geosearch/gsTokenPartSeq.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeEmptyTokenPartSeqQuery( IAllocator*          allocator,
                            INameDataSource*     tokenDataSource,
                            BOOL8                isRoute,
                            BOOL8                exactMatch,
                            ITokenPartSeqQuery** emptyTokenPartSeqQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeEmptyTokenSeqAttribute( IAllocator*          allocator,
                            INameDataSource*     tokenDataSource,
                            ITokenSeqAttribute** tokenSeqAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeEngineFactoryQueue( IAllocator*           allocator,
                        IEngineFactoryQueue** engineFactoryQueue );

//--------------------------------------------------------------------------------
query::TStatus
makeEngineFactoryQueueEngine( IAllocator*          allocator,
                              IEngineFactoryQueue* engineFactoryQueue,
                              TQueryProgress*      queryProgress,
                              query::IEngine*      tailEngine,
                              query::IEngine**     engineFactoryQueueEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeExactPlaceFilter( IAllocator*        allocator,
                      TGenericMapInfo*   roadMapInfo,
                      const TLocalPlace* localPlaces,
                      UINT32             localPlaceCount,
                      ISearchFilter**    exactPlaceFilter,
                      UINT32*            outPassCount = NULL );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeExistsQueryTailEngine( IAllocator*      allocator,
                           query::IEngine** existsQueryTailEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeExtendedPostalCodeUnicodeSeqQuery( IAllocator*                  allocator,
                                       IPostalCodeUnicodeSeqQuery*  postalCodeUnicodeSeqQuery,
                                       IUnicodeQuery*               unicodeQuery,
                                       TNameCost8                   matchCost,
                                       IPostalCodeUnicodeSeqQuery** extendedPostalCodeUnicodeSeqQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeExtendedSpellingCharacterSeqQuery( IAllocator*                  allocator,
                                       ISpellingCharacterSeqQuery*  spellingCharacterSeqQuery,
                                       IUnicodeQuery*               spellingCharacterQuery,
                                       ISpellingCharacterSeqQuery** extendedSpellingCharacterSeqQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeExtendedTokenPartSeqQuery( IAllocator*          allocator,
                               ITokenPartSeqQuery*  tokenPartSeqQuery,
                               ITokenPartQuery*     tokenPartQuery,
                               ITokenPartSeqQuery** extendedTokenPartSeqQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeExtendedTokenSeqAttributeWithEndSeparator( IAllocator*          allocator,
                                               INameDataSource*     tokenDataSource,
                                               ITokenSeqAttribute*  headTokenSeqAttribute,
                                               BOOL8                allowTokenReorder,
                                               ITokenSeqAttribute** tokenSeqAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeExtendedTokenSeqAttributeWithEndToken( IAllocator*          allocator,
                                           INameDataSource*     tokenDataSource,
                                           ITokenSeqAttribute*  headTokenSeqAttribute,
                                           ITokenPartSeqQuery*  tokenPartSeqQuery,
                                           BOOL8                allowPartialTokens,
                                           ITokenSeqAttribute** tokenSeqAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeFuzzyPlaceFilter( IAllocator*        allocator,
                      TGenericMapInfo*   roadMapInfo,
                      const TLocalPlace* localPlaces,
                      UINT32             localPlaceCount,
                      const TBitset*     sectorFilterBitset,
                      ISearchFilter**    fuzzyPlaceFilter,
                      UINT32*            outPassCount = NULL );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberAddress( IAllocator*               allocator,
                        IHouseNumberEntity*       houseNumberEntity,
                        TAreaMatchType8           areaMatchType,
                        BOOL8                     flagMixedRange,
                        UINT32                    startNumber,
                        UINT32                    endNumber,
                        THouseNumberAddressFormat houseNumberAddressFormat,
                        UINT32                    houseNumberFilter,
                        TRoadRoadNameIndex32      nameIndex,
                        UINT32                    matchedTokenPositions,
                        const TTokenHighlight*    matchedTokens,
                        const TSearchArea*        searchArea,
                        const TPlaceQualifier*    placeQualifier,
                        IHouseNumberAddress**     houseNumberAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberAddressAttribute( IAllocator*               allocator,
                                 IAllocator*               outputAllocator,
                                 TResultCostMode           resultCostMode,
                                 THouseNumberAddressFormat houseNumberAddressFormat,
                                 IHouseNumberAttribute*    houseNumberAttribute,
                                 INameDataSource*          roadNameOrRouteNameDataSource,
                                 INameAttribute*           roadNameOrRouteNameAttribute,
                                 IPlaceQualifierAttribute* placeQualifierAttribute,
                                 IPlaceFilter*             placeFilter,
                                 IResultListBuilder*       resultListBuilder,
                                 IResultGroupCache*        houseNumberResultGroupCache,
                                 const TTuningParameters*  tuningParameters,
                                 IEngineFactoryQueue*      engineFactoryQueue,
                                 query::IObject**          houseNumberAddressAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberAddressAttributeFactory( IAllocator*              allocator,
                                        IAllocator*              outputAllocator,
                                        TResultCostMode          resultCostMode,
                                        INameDataSource*         roadNameOrRouteNameDataSource,
                                        IPlaceFilter*            placeFilter,
                                        IResultListBuilder*      resultListBuilder,
                                        IResultGroupCache*       houseNumberResultGroupCache,
                                        const TTuningParameters* tuningParameters,
                                        IEngineFactoryQueue*     engineFactoryQueue,
                                        query::IObject*          outerUnknown,
                                        query::IObject**         houseNumberAddressAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberAttribute( IAllocator*             allocator,
                          UINT32                  houseNumberFilter,
                          BOOL8                   flagAllowPartialHouseNumber,
                          IHouseNumberAttribute** houseNumberAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberAttributeFactory( IAllocator*      allocator,
                                 query::IObject*  outerUnknown,
                                 query::IObject** houseNumberAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberEntity( IAllocator*           allocator,
                       UINT32                houseNumber,
                       UINT16                roadMapIndex,
                       TRoadRoadSetIndex32   roadSetIndex,
                       TRoadPlacePairIndex16 placePairIndex,
                       TRoadSectorIndex16    sectorIndex,
                       UINT16                segmentIndex,
                       UINT16                proportionAlongSegment,
                       TRoadSide             segmentSide,
                       IHouseNumberEntity**  houseNumberEntity );

query::TStatus
makeHouseNumberEntity( IAllocator*           allocator,
                       UINT32                houseNumber,
                       UINT16                roadMapIndex,
                       TRoadRoadSetIndex32   roadSetIndex,
                       TRoadPlacePairIndex16 placePairIndex,
                       TRoadSectorIndex16    sectorIndex,
                       UINT16                segmentIndex,
                       UINT16                proportionAlongSegment,
                       TRoadSide             segmentSide,
                       UINT32                segmentPtr,
                       TPos                  position,
                       INT16                 segmentHeading,
                       IHouseNumberEntity**  houseNumberEntity );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberParserCallback( IAllocator*               allocator,
                               IAllocator*               outputAllocator,
                               UINT16                    roadMapIndex,
                               IPlaceFilter*             placeFilter,
                               const TRoadRoadIndex32*   localRoads,
                               UINT32                    localRoadCount,
                               IResultListBuilder*       resultListBuilder,
                               IResultGroupCache*        houseNumberResultGroupCache,
                               const TTuningParameters*  tuningParameters,
                               query::IEngine*           tailEngine,
                               parser::IParserCallback** parserCallback );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberParserEngineFactory( IAllocator*              allocator,
                                    IAllocator*              outputAllocator,
                                    IResultListBuilder*      resultListBuilder,
                                    const UTF16*             searchString,
                                    IResultGroupCache*       houseNumberResultGroupCache,
                                    UINT32                   placeFilterCacheCount,
                                    IPlaceFilterCache*const* placeFilterCaches,
                                    const UINT32*            baseRoadCounts,
                                    IRoadEntity*const*const* baseRoads,
                                    UINT16                   roadMapIndex,
                                    const TTuningParameters* tuningParameters,
                                    IEngineFactory**         houseNumberParserEngineFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberResult( IAllocator*          allocator,
                       IStructuredText*     resultName,
                       IHouseNumberEntity*  houseNumberEntity,
                       BOOL8                flagFuzzyMatch,
                       BOOL8                flagMixedRange,
                       UINT32               startNumber,
                       UINT32               endNumber,
                       TRoadRoadNameIndex32 roadNameIndex,
                       TRoadRoadNameIndex32 altRoadNameIndex,
                       const TSearchArea*   searchArea,
                       ISearchResult**      houseNumberResult );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberResultGroup( IAllocator*               allocator,
                            IAllocator*               outputAllocator,
                             const TTuningParameters* tuningParameters,
                            IPlaceEntityCache*        placeEntityCache,
                            const UTF16*              resultGroupKey,
                            IResultGroupCache*        resultGroupCache,
                            IResultGroup**            houseNumberResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberResultGroupFactory( IAllocator*              allocator,
                                   IAllocator*              outputAllocator,
                                   const TTuningParameters* tuningParameters,
                                   IPlaceEntityCache*       placeEntityCache,
                                   IResultGroupFactory**    resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberSearchEngine( IAllocator*                allocator,
                             IAllocator*                outputAllocator,
                             UINT16                     roadMapIndex,
                             TResultCostMode            resultCostMode,
                             const TName*               roadName,
                             const TRoadRoadSetIndex32* roadSets,
                             UINT32                     roadSetCount,
                             THouseNumberAddressFormat  houseNumberAddressFormat,
                             UINT32                     houseNumberFilter,
                             BOOL8                      flagAllowPartialHouseNumber,
                             IPlaceQualifierAttribute*  placeQualifierAttribute,
                             IPlaceFilter*              placeFilter,
                             IResultListBuilder*        resultListBuilder,
                             IResultGroupCache*         houseNumberResultGroupCache,
                             const TTuningParameters*   tuningParameters,
                             query::IEngine*            tailEngine,
                             BOOL8*                     flagResultTruncated,
                             query::IEngine**           houseNumberSearchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeHouseNumberSearchParameters( IAllocator*                    allocator,
                                 const UTF16*                   searchString,
                                 IPlaceFilterCache*             placeFilterCache,
                                 IRoadEntity*                   baseRoad,
                                 const TTuningParameters*       tuningParameters,
                                 IHouseNumberSearchParameters** houseNumberSearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectingRoadAddressAttribute( IAllocator*                                   allocator,
                                      IIntersectingRoadNameIntersectionEntityTable* intRoadNameIntEntityTable,
                                      TResultCostMode                               resultCostMode,
                                      INameDataSource*                              roadNameOrRouteNameDataSource,
                                      INameAttribute*                               roadNameOrRouteNameAttribute,
                                      IPlaceFilter*                                 placeFilter,
                                      IResultListBuilder*                           resultListBuilder,
                                      IResultGroupCache*                            intersectionResultGroupCache,
                                      TRoadRoadNameIndex32                          baseRoadNameIndex,
                                      const TTuningParameters*                      tuningParameters,
                                      IEngineFactoryQueue*                          engineFactoryQueue,
                                      query::IObject**                              intersectingRoadAddressAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectingRoadAddressAttributeFactory( IAllocator*                                   allocator,
                                             IIntersectingRoadNameIntersectionEntityTable* intRoadNameIntEntityTable,
                                             TResultCostMode                               resultCostMode,
                                             INameDataSource*                              roadNameOrRouteNameDataSource,
                                             IPlaceFilter*                                 placeFilter,
                                             IResultListBuilder*                           resultListBuilder,
                                             IResultGroupCache*                            intersectionResultGroupCache,
                                             TRoadRoadNameIndex32                          baseRoadNameIndex,
                                             const TTuningParameters*                      tuningParameters,
                                             IEngineFactoryQueue*                          engineFactoryQueue,
                                             query::IObject*                               outerUnknown,
                                             query::IObject**                              intersectingRoadAddressAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectingRoadAddressParserEngineFactory( IAllocator*              allocator,
                                                IAllocator*              outputAllocator,
                                                IResultListBuilder*      resultListBuilder,
                                                const UTF16*             searchString,
                                                IResultGroupCache*       intersectionResultGroupCache,
                                                IPlaceFilterCache*       placeFilterCache,
                                                IRoadEntity*             baseRoad,
                                                UINT16                   roadMapIndex,
                                                const TTuningParameters* tuningParameters,
                                                IEngineFactory**         intersectingRoadAddressParserEngineFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectingRoadNameIntersectionEntityTable( IAllocator*                                    allocator,
                                                 IAllocator*                                    outputAllocator,
                                                 IPlaceFilter*                                  placeFilter,
                                                 UINT16                                         roadMapIndex,
                                                 const TRoadRoadIndex32*                        localRoads,
                                                 UINT32                                         localRoadCount,
                                                 IIntersectingRoadNameIntersectionEntityTable** intRoadNameIntEntityTable );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectingRoadSearchParameters( IAllocator*                         allocator,
                                      const UTF16*                        searchString,
                                      IPlaceFilterCache*                  placeFilterCache,
                                      IRoadEntity*                        baseRoad,
                                      const TTuningParameters*            tuningParameters,
                                      IIntersectingRoadSearchParameters** intersectingRoadSearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectionAddress( IAllocator*            allocator,
                         UINT16                 roadMapIndex,
                         TAreaMatchType         areaMatchType,
                         TRoadRoadNameIndex32   nameIndex1,
                         UINT32                 matchedTokenPositions1,
                         const TTokenHighlight* matchedTokens1,
                         TRoadRoadNameIndex32   nameIndex2,
                         UINT32                 matchedTokenPositions2,
                         const TTokenHighlight* matchedTokens2,
                         const TSearchArea*     searchArea,
                         IIntersectionEntity*   intersectionEntity,
                         IIntersectionAddress** intersectionAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectionAddressAttribute( IAllocator*               allocator,
                                  IAllocator*               outputAllocator,
                                  INameDataSource*          roadNameOrRouteNameDataSource,
                                  INameAttribute*           roadNameOrRouteNameAttribute1,
                                  INameAttribute*           roadNameOrRouteNameAttribute2,
                                  IPlaceQualifierAttribute* placeQualifierAttribute,
                                  IPlaceFilter*             placeFilter,
                                  IResultListBuilder*       resultListBuilder,
                                  IResultGroupCache*        intersectionResultGroupCache,
                                  const TTuningParameters*  tuningParameters,
                                  IEngineFactoryQueue*      engineFactoryQueue,
                                  query::IObject**          intersectionAddressAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectionAddressAttributeFactory( IAllocator*              allocator,
                                         IAllocator*              outputAllocator,
                                         TResultCostMode          resultCostMode,
                                         INameDataSource*         roadNameOrRouteNameDataSource,
                                         IPlaceFilter*            placeFilter,
                                         IResultListBuilder*      resultListBuilder,
                                         IResultGroupCache*       intersectionResultGroupCache,
                                         const TTuningParameters* tuningParameters,
                                         IEngineFactoryQueue*     engineFactoryQueue,
                                         query::IObject*          outerUnknown,
                                         query::IObject**         intersectionAddressAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectionEntity( IAllocator*                                    allocator,
                        UINT32                                         nodePtr,
                        UINT16                                         roadMapIndex,
                        UINT8                                          segCount,
                        const IIntersectionEntity::TSegmentAttributes* segAttributes,
                        UINT8                                          instanceIndex,
                        IIntersectionEntity**                          intersectionEntity );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectionResult( IAllocator*          allocator,
                        IStructuredText*     resultName,
                        IIntersectionEntity* intersectionEntity,
                        BOOL8                flagFuzzyMatch,
                        TRoadRoadNameIndex32 roadNameIndex1,
                        TRoadRoadNameIndex32 roadNameIndex2,
                        ISearchResult**      intersectionResult );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectionResultGroup( IAllocator*              allocator,
                             IAllocator*              outputAllocator,
                             const TTuningParameters* tuningParameters,
                             IPlaceEntityCache*       placeEntityCache,
                             const UTF16*             resultGroupKey,
                             IResultGroupCache*       resultGroupCache,
                             IResultGroup**           intersectionResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeIntersectionResultGroupFactory( IAllocator*              allocator,
                                    IAllocator*              outputAllocator,
                                    const TTuningParameters* tuningParameters,
                                    IPlaceEntityCache*       placeEntityCache,
                                    IResultGroupFactory**    resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeJourneySearchLocation( IAllocator*      allocator,
                           const TJourney*  journey,
                           TMapLocation*    mapLocation,
                           UINT32           searchSize,
                           UINT32           maxDetour,
                           query::IObject** searchLocation );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongAddress( IAllocator*       allocator,
                    TPos              latLongPosition,
                    ILatLongAddress** latLongAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongAddressAttribute( IAllocator*          allocator,
                             TPos                 latLongPosition,
                             IResultListBuilder*  resultListBuilder,
                             IResultGroupCache*   latLongResultGroupCache,
                             IEngineFactoryQueue* engineFactoryQueue,
                             query::IObject**     latLongAddressAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongAddressAttributeFactory( IAllocator*          allocator,
                                    IResultListBuilder*  resultListBuilder,
                                    IResultGroupCache*   latLongResultGroupCache,
                                    IEngineFactoryQueue* engineFactoryQueue,
                                    query::IObject*      outerUnknown,
                                    query::IObject**     latLongAddressAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongAddressParserEngineFactory( IAllocator*         allocator,
                                       IResultListBuilder* resultListBuilder,
                                       const UTF16*        searchString,
                                       IResultGroupCache*  latLongResultGroupCache,
                                       IEngineFactory**    latLongAddressParserEngineFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongDecimalAttribute( IAllocator*                allocator,
                             BOOL8                      flagNegative,
                             UINT8                      degree,
                             ILatLongFractionAttribute* degreeFractionAttribute,
                             ILatLongDecimalAttribute** latLongDecimalAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongDegreeAttribute( IAllocator*               allocator,
                            UINT8                     degree,
                            ILatLongDegreeAttribute** degreeAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongDMSAttribute( IAllocator*              allocator,
                         ILatLongDegreeAttribute* degreeAttribute,
                         ILatLongMinuteAttribute* minuteAttribute,
                         ILatLongSecondAttribute* secondAttribute,
                         ILatLongDMSAttribute**   latLongDMSAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongFractionAttribute( IAllocator*                 allocator,
                              UINT32                      numerator,
                              UINT32                      denominator,
                              ILatLongFractionAttribute** fractionAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongMinuteAttribute( IAllocator*               allocator,
                            UINT8                     minute,
                            ILatLongMinuteAttribute** minuteAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongResult( IAllocator*      allocator,
                   IStructuredText* resultName,
                   TPos             latLongPosition,
                   ISearchResult**  latLongResult );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongResultGroup( IAllocator*        allocator,
                        IAllocator*        outputAllocator,
                        const UTF16*       resultGroupKey,
                        IResultGroupCache* resultGroupCache,
                        IResultGroup**     latLongResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongResultGroupFactory( IAllocator*           allocator,
                               IAllocator*           outputAllocator,
                               IResultGroupFactory** resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongSearchParameters( IAllocator*                allocator,
                             const UTF16*               searchString,
                             ILatLongSearchParameters** latLongSearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeLatLongSecondAttribute( IAllocator*                allocator,
                            UINT8                      second,
                            ILatLongFractionAttribute* secondFractionAttribute,
                            ILatLongSecondAttribute**  secondAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeMergedAddressAttribute( IAllocator*           allocator,
                            query::IObject*const* addressAttributes,
                            UINT32                addressAttributeCount,
                            query::IObject**      mergedAddressAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeMergedAddressAttributeFactory( IAllocator*      allocator,
                                   query::IObject*  outerUnknown,
                                   query::IObject** mergedAddressAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeMergedRoadNameOrRouteNameAttribute( IAllocator*           allocator,
                                        INameDataSource*      nameDataSource,
                                        INameAttribute*const* nameAttributes,
                                        UINT32                nameAttributeCount,
                                        INameAttribute**      mergedNameAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeMergedRoadNameOrRouteNameAttributeFactory( IAllocator*      allocator,
                                               INameDataSource* nameDataSource,
                                               query::IObject*  outerUnknown,
                                               query::IObject** mergedRoadNameOrRouteNameAttributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeMergedTokenSeqAttribute( IAllocator*               allocator,
                             INameDataSource*          tokenDataSource,
                             ITokenSeqAttribute*const* tokenSeqAttributes,
                             UINT32                    tokenSeqAttributeCount,
                             ITokenSeqAttribute**      mergedTokenSeqAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeNameDataSource( IAllocator*          allocator,
                    TTokenDataType       nameType,
                    TGenericMapInfo*     mapInfo,
                    INameDataSource**    nameDataSource );

//--------------------------------------------------------------------------------
query::TStatus
makeNameRangeAttribute( IAllocator*      allocator,
                        TNameIndex32     firstNameIndex,
                        TNameIndex32     lastNameIndex,
                        ISearchFilter*   nameFilter,
                        INameAttribute** nameAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeNameRangeQuery( IAllocator*    allocator,
                    TNameIndex32   firstNameIndex,
                    TNameIndex32   lastNameIndex,
                    ISearchFilter* nameFilter,
                    INameQuery**   nameQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeNameTableScanTask( IAllocator*          allocator,
                       INameDataSource*     nameDataSource,
                       ISearchFilter*       nameFilter,
                       IEngineFactoryQueue* engineFactoryQueue,
                       INameTableScanTask** nameTableScanTask );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOIAddress( IAllocator*          allocator,
                       IPOIEntity*          POIEntity,
                       TPOINameIndex32      POINameIndex,
                       TPOITypeIndex16      POITypeIndex,
                     //TAreaMatchType       areaMatchType,
                       UINT32               detour,
                       UINT32               distance,
                       UINT32               ETA,
                       INearestPOIAddress** nearestPOIAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOIEntity( IAllocator*        allocator,
                      UINT16             POIMapIndex,
                      TPOINameSetIndex32 POINameSetIndex,
                      TPOISectorIndex16  POISectorIndex,
                      TRoadPlaceIndex16  placeIndex,
                      TPOITypeSetIndex16 POITypeSetIndex,
                      UINT32             POIId,
                      UINT32             POIContentOffset,
                      TPos               POIPosition,
                      IPOIEntity**       nearestPOIEntity );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOIEntityFactory( IAllocator*                allocator, 
                             IAllocator*                outputAllocator,
                             TVehicleType               vehicleType,
                             UINT32                     maxCost,
                             INearestPOIEntityFactory** nearestPOIEntityFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOINameAttributeFactory( IAllocator*      allocator,
                                    INameDataSource* POINameDataSource,
                                    ISearchFilter*   POINameFilter,
                                    query::IObject*  outerUnknown,
                                    query::IObject** nearestPOINameAttributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOINameMatchEngine( IAllocator*               allocator,
                               const UTF16*              searchString,
                               INearestPOINameMatchList* nearestPOINameMatchList,
                               query::IEngine*           tailEngine,
                               query::IEngine**          searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOINameMatchList( IAllocator*                allocator,
                             INearestPOINameMatchList** nearestPOINameMatchList );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOINameParserInput( IAllocator*         allocator,
                               INameDataSource*    POINameDataSource,
                               INameTableScanTask* POINameTableScanTask,
                               ISearchFilter*      POINameFilter,
                               query::IObject**    parserInput );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOIResult( IAllocator*      allocator,
                      IStructuredText* resultName,
                      IPOIEntity*      POIEntity,
                      TPOINameIndex32  POINameIndex,
                      TPOITypeIndex16  POITypeIndex,
                      UINT32           detour,
                      UINT32           distance,
                      UINT32           ETA,
                      ISearchResult**  nearestPOIResult );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOIResultGroup( IAllocator*               allocator,
                           IAllocator*               outputAllocator,
                           const TTuningParameters*  tuningParameters,
                           IPlaceEntityCache*        placeEntityCache,
                           INearestPOINameMatchList* nearestPOINameMatchList,
                           const UTF16*              resultGroupKey,
                           IResultGroupCache*        resultGroupCache,
                           IResultGroup**            nearestPOIResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOIResultGroupFactory( IAllocator*               allocator,
                                  IAllocator*               outputAllocator,
                                  const TTuningParameters*  tuningParameters,
                                  IPlaceEntityCache*        placeEntityCache,
                                  INearestPOINameMatchList* nearestPOINameMatchList,
                                  IResultGroupFactory**     resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOINameMatchFilterCache( IAllocator*                       allocator, 
                                    INearestPOINameMatchFilterCache** nearestPOINameMatchFilterCache );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOISearchEngine( IAllocator*               allocator,
                            IAllocator*               outputAllocator,
                            ISearchLocation*          searchLocation,
                            const TRect*              searchBounds,
                            const UTF16*              searchString,
                            const TPOITypeId16*       POITypes,
                            UINT32                    POITypeCount,
                            UINT32                    maxDistance,
                            const TJourney*           currentJourney,
                            UINT32                    maxDetour,
                            IResultListBuilder*       resultListBuilder,
                            IResultGroupCache*        nearestPOIResultGroupCache,
                            INearestPOINameMatchList* nearestPOINameMatchList,
                            TQueryProgress*           queryProgress,
                            query::IEngine*           tailEngine,
                            query::IEngine**          nearestPOISearchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOISearchParameters( IAllocator*                                            allocator,
                                const UTF16*                                           searchString,
                                IPlaceFilterCache*                                     placeFilterCache,
                                const TRect*                                           searchBounds,
                                TPOITypeId16                                           POITypeId,
                                UINT32                                                 maxDistance,
                                const INearestPOISearchParameters::TJourneyParameters* journeyParameters,
                                const TTuningParameters*                               tuningParameters,
                                INearestPOISearchParameters**                          nearestPOISearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOISectorScanEngine( IAllocator*                      allocator,
                                const TPOITypeId16*              POITypes,
                                UINT32                           POITypeCount,
                                ISearchLocation*                 searchLocation,
                                const TRect*                     searchBounds,
                                const UTF16*                     searchString,
                                INearestPOISectorScanList*       nearestPOISectorScanList,
                                INearestPOINameMatchFilterCache* nearestPOINameMatchFilterCache,
                                INearestPOIEntityFactory*        nearestPOIEntityFactory,
                                query::IEngine*                  tailEngine,
                                query::IEngine**                 nearestPOISectorScanEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestPOISectorScanList( IAllocator*                 allocator, 
                              INearestPOISectorScanList** nearestPOISectorScanList );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestUserPOIAddress( IAllocator*              allocator,
                           IUserPOISearch*          userPOISearch,
                           IUserPOISearch::TResult  userPOI,
                           UINT32                   matchLength,
                           UINT32                   distance,
                           UINT32                   ETA,
                           INearestUserPOIAddress** nearestUserPOIAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestUserPOIResult( IAllocator*             allocator,
                          IStructuredText*        resultName,
                          IUserPOISearch*         userPOISearch,
                          IUserPOISearch::TResult userPOI,
                          UINT32                  distance,
                          UINT32                  ETA,
                          UINT32                  segmentId,
                          TRoadPlaceIndex16       segmentPlaceIndex,
                          UINT16                  segmentProportion,
                          TRoadSide               segmentSide,
                          TPos                    segmentPosition,
                          ISearchResult**         nearestUserPOIResult );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestUserPOIResultGroup( IAllocator*              allocator,
                               IAllocator*              outputAllocator,
                               const TTuningParameters* tuningParameters,
                               IPlaceEntityCache*       placeEntityCache,
                               const UTF16*             resultGroupKey,
                               IResultGroupCache*       resultGroupCache,
                               IResultGroup**           nearestUserPOIResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestUserPOISearchEngine( IAllocator*         allocator,
                                TVehicleType        vehicleType,
                                const TRect*        searchBounds,
                                const UTF16*        searchString,
                                const TPOITypeId16* POITypes,
                                UINT32              POITypeCount,
                                UINT32              maxDistance,
                                void*               userPOIContext,
                                IResultListBuilder* resultListBuilder,
                                IResultGroupCache*  nearestUserPOIResultGroupCache,
                                query::IEngine*     tailEngine,
                                query::IEngine**    searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestUserPOISearchParameters( IAllocator*                       allocator,
                                    const UTF16*                      searchString,
                                    const TRect*                      searchBounds,
                                    TPOITypeId16                      POITypeId,
                                    UINT32                            maxDistance,
                                    void*                             userPOIContext,
                                    IPlaceFilterCache*                placeFilterCache,
                                    const TTuningParameters*          tuningParameters,
                                    INearestUserPOISearchParameters** nearestUserPOISearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeNearestUserPOIResultGroupFactory( IAllocator*              allocator,
                                      IAllocator*              outputAllocator,
                                      const TTuningParameters* tuningParameters,
                                      IPlaceEntityCache*       placeEntityCache,
                                      IResultGroupFactory**    resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeP1PlaceFilter( IAllocator*        allocator,
                   TGenericMapInfo*   roadMapInfo,
                   const TLocalPlace* localPlaces,
                   UINT32             localPlaceCount,
                   ISearchFilter**    P1PlaceFilter,
                   UINT32*            outPassCount = NULL );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makePlaceAddress( IAllocator*               allocator,
                  UINT16                    roadMapIndex,
                  TRoadPlaceNameIndex16     placeNameIndex,
                  UINT32                    matchedTokenPositions,
                  const TTokenHighlight*    matchedTokens,
                  TGeosearchPlaceColumnMask placeColumns,
                  UINT16                    placeNamePlaceRecordIndex,
                  const TSearchArea*        searchArea,
                  TNumericISOCountryCode    numericISOCountryCode,
                  IPlaceAddress**           placeAddress );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceAddressAttribute( IAllocator*               allocator,
                           TResultCostMode           resultCostMode,
                           INameDataSource*          placeNameDataSource,
                           INameAttribute*           placeNameAttribute,
                           IPlaceFilter*             placeFilter,
                           IResultListBuilder*       resultListBuilder,
                           IResultGroupCache*        placeResultGroupCache,
                           const TTuningParameters*  tuningParameters,
                           TGeosearchPlaceColumnMask placeSearchColumns,
                           IEngineFactoryQueue*      engineFactoryQueue,
                           query::IObject**          placeAddressAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceAddressAttributeFactory( IAllocator*               allocator,
                                  TResultCostMode           resultCostMode,
                                  INameDataSource*          placeNameDataSource,
                                  IPlaceFilter*             placeFilter,
                                  IResultListBuilder*       resultListBuilder,
                                  IResultGroupCache*        placeResultGroupCache,
                                  const TTuningParameters*  tuningParameters,
                                  TGeosearchPlaceColumnMask placeSearchColumns,
                                  IEngineFactoryQueue*      engineFactoryQueue,
                                  query::IObject*           outerUnknown,
                                  query::IObject**          placeAddressAtributeFactory );

//--------------------------------------------------------------------------------
//for TW/CN first-phonetic search
query::TStatus
makeFirstLetterPlaceAddressAttribute( IAllocator*               allocator,
                                      TResultCostMode           resultCostMode,
                                      INameDataSource*          placeNameDataSource,
                                      INameAttribute*           placeNameAttribute,
                                      IPlaceFilter*             placeFilter,
                                      IResultListBuilder*       resultListBuilder,
                                      IResultGroupCache*        placeResultGroupCache,
                                      const TTuningParameters*  tuningParameters,
                                      TGeosearchPlaceColumnMask placeSearchColumns,
                                      IEngineFactoryQueue*      engineFactoryQueue,
                                      query::IObject**          placeAddressAttribute );

query::TStatus
makeFirstLetterPlaceAddressAttributeFactory( IAllocator*               allocator,
                                             TResultCostMode           resultCostMode,
                                             INameDataSource*          placeNameDataSource,
                                             IPlaceFilter*             placeFilter,
                                             IResultListBuilder*       resultListBuilder,
                                             IResultGroupCache*        placeResultGroupCache,
                                             const TTuningParameters*  tuningParameters,
                                             TGeosearchPlaceColumnMask placeSearchColumns,
                                             IEngineFactoryQueue*      engineFactoryQueue,
                                             query::IObject*           outerUnknown,
                                             query::IObject**          placeAddressAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceEntity( IAllocator*               allocator,
                 const TGlobalPlace*       globalPlaces,
                 UINT32                    globalPlaceCount,
                 TGeosearchPlaceColumnMask placeColumns,
                 TGeosearchPlaceColumnMask placeMergeColumns,
                 const TRect*              placeBounds,
                 BOOL8                     flagNoRoads,
                 const UINT16*             placeNamePlaces,
                 IPlaceEntity**            placeEntity );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceEntityCache( IAllocator*              allocator,
                      IAllocator*              outputAllocator,
                      const TTuningParameters* tuningParameters,
                      ISearchLocation*         searchLocation,
                      IPlaceEntityCache**      placeEntityCache );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceEntityFactoryEngine( IAllocator*                  allocator,
                              IAllocator*                  outputAllocator,
                              const TTuningParameters*     tuningParameters,
                              const UTF16*                 placeIdentityName,
                              ISearchLocation*             searchLocation,
                              IPlaceEntityFactoryCallback* factoryCallback,
                              query::IEngine**             factoryEngine );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceFilter( IAllocator*        allocator,
                 UINT16             roadMapIndex,
                 BOOL8              flagLocalSearch,
                 BOOL8              flagUseNameFilters,
                 ISearchFilter*     exactPlaceFilter,
                 ISearchFilter*     P1PlaceFilter,
                 ISearchFilter*     fuzzyPlaceFilter,
                 ISearchFilter*     sectorFilter,
                 ISearchFilter*     roadFilter,
                 const TSearchArea* searchArea,
                 IPlaceFilter**     placeFilter );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceQualifierAttribute( IAllocator*                allocator,
                             TResultCostMode            resultCostMode,
                             INameDataSource*           placeNameDataSource,
                             INameAttribute*            placeNameAttribute,
                             IPlaceFilter*              placeFilter,
                             const TTuningParameters*   tuningParameters,
                             IEngineFactoryQueue*       engineFactoryQueue,
                             IPlaceQualifierAttribute** placeQualifierAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceQualifierAttributeFactory( IAllocator*              allocator,
                                    TResultCostMode          resultCostMode,
                                    INameDataSource*         placeNameDataSource,
                                    IPlaceFilter*            placeFilter,
                                    const TTuningParameters* tuningParameters,
                                    IEngineFactoryQueue*     engineFactoryQueue,
                                    query::IObject*          outerUnknown,
                                    query::IObject**         placeQualifierAttributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceResult( IAllocator*            allocator,
                 IStructuredText*       resultName,
                 IPlaceEntity*          placeEntity,
               //const TBitset*         globalPlaceFilterBitset,
                 UINT16                 roadMapIndex,
                 TRoadPlaceNameIndex16  placeNameIndex,
                 TNumericISOCountryCode numericISOCountryCode,
                 ISearchResult**        placeResult );

query::TStatus
makePlaceResult( IAllocator*            allocator,
                 IStructuredText*       resultName,
                 IPlaceEntity*          placeEntity,
               //const TBitset*         globalPlaceFilterBitset,
                 UINT16                 roadMapIndex,
                 TRoadPlaceNameIndex16  placeNameIndex,
                 TNumericISOCountryCode numericISOCountryCode,
                 UINT32                 locationSegmentPtr,
                 UINT32                 locationSegmentRoadSet,
                 UINT16                 locationSegmentPlace,
                 UINT16                 locationSegmentProportion,
                 TPos                   locationPosition,
                 ISearchResult**        placeResult );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceResultGroup( IAllocator*              allocator,
                      IAllocator*              outputAllocator,
                      const TTuningParameters* tuningParameters,
                      IPlaceEntityCache*       placeEntityCache,
                      const UTF16*             resultGroupKey,
                      IResultGroupCache*       resultGroupCache,
                      IResultGroup**           placeResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makePlaceResultGroupFactory( IAllocator*              allocator,
                             IAllocator*              outputAllocator,
                             const TTuningParameters* tuningParameters,
                             IPlaceEntityCache*       placeEntityCache,
                             IResultGroupFactory**    resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePOIAddress( IAllocator*            allocator,
                IPOIEntity*            POIEntity,
                UINT32                 nameIndex,
                UINT32                 matchedTokenPositions,
                const TTokenHighlight* matchedTokens,
                //TODO remove POITypeIndex?
                TPOITypeIndex16        POITypeIndex,
                TAreaMatchType         areaMatchType,
                const TSearchArea*     searchArea,
                const TPlaceQualifier* placeQualifier,
                IPOIAddress**          POIAddress );

//--------------------------------------------------------------------------------
query::TStatus
makePOIAddressAttribute( IAllocator*               allocator,
                         IAllocator*               outputAllocator,
                         TResultCostMode           resultCostMode,
                         INameDataSource*          POINameDataSource,
                         INameAttribute*           POINameAttribute,
                         IPlaceQualifierAttribute* placeQualifierAttribute,
                         IPOIPlaceFilter*          POIPlaceFilter,
                         IResultListBuilder*       resultListBuilder,
                         IResultGroupCache*        POIResultGroupCache,
                         const TTuningParameters*  tuningParameters,
                         const TPOITypeId16*       POITypes,
                         UINT32                    POITypeCount,
                         IEngineFactoryQueue*      engineFactoryQueue,
                         query::IObject**          POIAddressAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makePOIAddressAttributeFactory( IAllocator*              allocator,
                                IAllocator*              outputAllocator,
                                TResultCostMode          resultCostMode,
                                INameDataSource*         POINameDataSource,
                                IPOIPlaceFilter*         POIPlaceFilter,
                                IResultListBuilder*      resultListBuilder,
                                IResultGroupCache*       POIResultGroupCache,
                                const TTuningParameters* tuningParameters,
                                const TPOITypeId16*      POITypes,
                                UINT32                   POITypeCount,
                                IEngineFactoryQueue*     engineFactoryQueue,
                                query::IObject*          outerUnknown,
                                query::IObject**         POIAddressAtributeFactory );

//--------------------------------------------------------------------------------
//for TW/CN first-phonetic search
query::TStatus
makeFirstPhoneticPOIAddressAttribute( IAllocator*               allocator,
                                    IAllocator*               outputAllocator,
                                    TResultCostMode           resultCostMode,
                                    INameDataSource*          POINameDataSource,
                                    INameAttribute*           POINameAttribute,
                                    IPlaceQualifierAttribute* placeQualifierAttribute,
                                    IPOIPlaceFilter*          POIPlaceFilter,
                                    IResultListBuilder*       resultListBuilder,
                                    IResultGroupCache*        POIResultGroupCache,
                                    const TTuningParameters*  tuningParameters,
                                    const TPOITypeId16*       POITypes,
                                    UINT32                    POITypeCount,
                                    IEngineFactoryQueue*      engineFactoryQueue,
                                    query::IObject**          POIAddressAttribute );

query::TStatus
makeFirstPhoneticPOIAddressAttributeFactory( IAllocator*              allocator,
                                           IAllocator*              outputAllocator,
                                           TResultCostMode          resultCostMode,
                                           INameDataSource*         POINameDataSource,
                                           IPOIPlaceFilter*         POIPlaceFilter,
                                           IResultListBuilder*      resultListBuilder,
                                           IResultGroupCache*       POIResultGroupCache,
                                           const TTuningParameters* tuningParameters,
                                           const TPOITypeId16*      POITypes,
                                           UINT32                   POITypeCount,
                                           IEngineFactoryQueue*     engineFactoryQueue,
                                           query::IObject*          outerUnknown,
                                           query::IObject**         POIAddressAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePOIAddressGroupByPOITypeAttribute( IAllocator*               allocator,
                                       TResultCostMode           resultCostMode,
                                       INameDataSource*          POINameDataSource,
                                       INameAttribute*           POINameAttribute,
                                       IPlaceQualifierAttribute* placeQualifierAttribute,
                                       IPOIPlaceFilter*          POIPlaceFilter,
                                       IResultListBuilder*       resultListBuilder,
                                       IResultGroupCache*        POITypeResultGroupCache,
                                       const TTuningParameters*  tuningParameters,
                                       IEngineFactoryQueue*      engineFactoryQueue,
                                       query::IObject**          POIAddressGroupByPOITypeAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makePOIAddressGroupByPOITypeAttributeFactory( IAllocator*              allocator,
                                              TResultCostMode          resultCostMode,
                                              INameDataSource*         POINameDataSource,
                                              IPOIPlaceFilter*         POIPlaceFilter,
                                              IResultListBuilder*      resultListBuilder,
                                              IResultGroupCache*       POITypeResultGroupCache,
                                              const TTuningParameters* tuningParameters,
                                              IEngineFactoryQueue*     engineFactoryQueue,
                                              query::IObject*          outerUnknown,
                                              query::IObject**         POIAddressGroupByPOITypeAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePOIEntity( IAllocator*        allocator,
               UINT16             POIMapIndex,
               TPOISectorIndex16  POISectorIndex,
               TRoadPlaceIndex16  placeIndex,
               TPOITypeSetIndex16 POITypeSetIndex,
               TPOINameIndex32    POINameIndex,
               UINT16             POIInstanceIndex,
               IPOIEntity**       POIEntity );

query::TStatus
makePOIEntity( IAllocator*        allocator,
               UINT16             POIMapIndex,
               TPOISectorIndex16  POISectorIndex,
               TRoadPlaceIndex16  placeIndex,
               TPOITypeSetIndex16 POITypeSetIndex,
               UINT32             POIId,
               UINT32             POIContentOffset,
               TPos               POIPosition,
               TPOINameSetIndex32 POINameSetIndex,
               IPOIEntity**       POIEntity );

//--------------------------------------------------------------------------------
query::TStatus
makePOIEnumeratePOITypeSearchEngine( IAllocator*         allocator,
                                     UINT16              POIMapIndex,
                                     TPOITypeSetIndex16  POITypeSetIndex,
                                     IResultListBuilder* resultListBuilder,
                                     IResultGroupCache*  POITypeResultGroupCache,
                                     query::IEngine*     tailEngine,
                                     query::IEngine**    searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makePOIEnumeratePOITypeSearchParameters( IAllocator*                            allocator,
                                         UINT16                                 POIMapIndex,
                                         TPOITypeSetIndex16                     POITypeSetIndex,
                                         IPOIEnumeratePOITypeSearchParameters** POIEnumeratePOITypeSearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makePOINameFilter( IAllocator*      allocator,
                   TGenericMapInfo* POIMapInfo,
                   const TBitset*   placeFilterBitset,
                   const TBitset*   POISectorFilterBitset,
                   ISearchFilter**  POINameFilter );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makePOIPlaceFilter( IAllocator*        allocator,
                    UINT16             POIMapIndex,
                    BOOL8              flagLocalSearch,
                    BOOL8              flagUseNameFilters,
                    ISearchFilter*     exactPlaceFilter,
                    ISearchFilter*     P1PlaceFilter,
                    ISearchFilter*     fuzzyPlaceFilter,
                    ISearchFilter*     POISectorFilter,
                    const TSearchArea* searchArea,
                    IPOIPlaceFilter**  POIPlaceFilter );

//--------------------------------------------------------------------------------
query::TStatus
makePOIResult( IAllocator*      allocator,
               IStructuredText* resultName,
               IPOIEntity*      POIEntity,
               BOOL8            flagFuzzyMatch,
               TPOINameIndex32  POINameIndex,
               TPOITypeIndex16  POITypeIndex,
               ISearchResult**  POIResult );

//--------------------------------------------------------------------------------
query::TStatus
makePOIResultGroup( IAllocator*              allocator,
                    IAllocator*              outputAllocator,
                    const TTuningParameters* tuningParameters,
                    IPlaceEntityCache*       placeEntityCache,
                    const UTF16*             resultGroupKey,
                    IResultGroupCache*       resultGroupCache,
                    IResultGroup**           POIResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makePOIResultGroupFactory( IAllocator*              allocator,
                           IAllocator*              outputAllocator,
                           const TTuningParameters* tuningParameters,
                           IPlaceEntityCache*       placeEntityCache,
                           IResultGroupFactory**    resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePOISectorFilter( IAllocator*      allocator,
                     TGenericMapInfo* POIMapInfo,
                     const TRect*     searchBounds,
                     ISearchFilter**  POISectorFilter,
                     UINT32*          outPassCount = NULL );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makePOISectorFilterForCityMap( IAllocator*        allocator,
                               TGenericMapInfo*   roadMapInfo,
                               TGenericMapInfo*   POIMapInfo,
                               const TRect*       searchBounds,
                               ISearchFilter**    POISectorFilter,
                               const TLocalPlace* localPlaces,
                               UINT32             localPlaceCount,
                               UINT32*            outPassCount = NULL );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeAddress( IAllocator*            allocator,
                    UINT16                 POITypeMapIndex,
                    TPoiTypeNameIndex16    nameIndex,
                    UINT32                 matchedTokenPositions,
                    const TTokenHighlight* matchedTokens,
                    UINT16                 POIMapIndex,
                    TPOITypeIndex16        POITypeIndex,
                    TGeosearchResultType   searchStringMatchType,
                    IPOITypeAddress**      POITypeAddress );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeNameAttributeFactory( IAllocator*      allocator,
                                 TResultCostMode  resultCostMode,
                                 INameDataSource* POITypeNameDataSource,
                                 query::IObject*  outerUnknown,
                                 query::IObject** POITypeNameAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeNameMatchCache( IAllocator*              allocator,
                           IPOITypeNameMatchCache** POITypeNameMatchCache );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeNameParserCallback( IAllocator*               allocator,
                               IEngineFactoryQueue*      engineFactoryQueue,
                               TResultCostMode           resultCostMode,
                               UINT16                    POITypeMapIndex,
                               IPOITypeNameMatchCache*   POITypeNameMatchCache,
                               query::IEngine*           tailEngine,
                               parser::IParserCallback** parserCallback );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeNameParserEngineFactory( IAllocator*             allocator,
                                    TResultCostMode         resultCostMode,
                                    const UTF16*            searchString,
                                    UINT16                  POITypeMapIndex,
                                    IPOITypeNameMatchCache* POITypeNameMatchCache,
                                    IEngineFactory**        POITypeNameParserEngineFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeResult( IAllocator*          allocator,
                   IStructuredText*     resultName,
                 //IPOITypeEntity*      POITypeEntity,
                   UINT16               POIMapIndex,
                   TPOITypeIndex16      POITypeIndex,
                   TGeosearchResultType searchStringMatchType,
                   ISearchResult**      POITypeResult );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeResultGroup( IAllocator*        allocator,
                        IAllocator*        outputAllocator,
                        const UTF16*       resultGroupKey,
                        IResultGroupCache* resultGroupCache,
                        IResultGroup**     POITypeResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeResultGroupFactory( IAllocator*           allocator,
                               IAllocator*           outputAllocator,
                               IResultGroupFactory** resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeSearchEngine( IAllocator*              allocator,
                         UINT16                   POIMapIndex,
                         IPOIPlaceFilter*         POIPlaceFilter,
                         IResultListBuilder*      resultListBuilder,
                         IResultGroupCache*       POITypeResultGroupCache,
                         IPOITypeNameMatchCache*  POITypeNameMatchCache,
                         const TTuningParameters* tuningParameters,
                         query::IEngine*          tailEngine,
                         query::IEngine**         searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeSearchEngineFactory( IAllocator*              allocator,
                                UINT16                   POIMapIndex,
                                IPlaceFilterCache*       placeFilterCache,
                                IResultListBuilder*      resultListBuilder,
                                IResultGroupCache*       POITypeResultGroupCache,
                                IPOITypeNameMatchCache*  POITypeNameMatchCache,
                                const TTuningParameters* tuningParameters,
                                IEngineFactory**         POITypeSearchEngineFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePOITypeSearchParameters( IAllocator*                allocator,
                             TGeosearchResultType       searchType,
                             const UTF16*               searchString,
                             IPlaceFilterCache*         placeFilterCache,
                             const TTuningParameters*   tuningParameters,
                             IPOITypeSearchParameters** POITypeSearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeAddress( IAllocator*          allocator,
                       BOOL8                flagDetailed,
                       UINT16               mapIndex,
                       TPostalCodeIndex32   postalCodeIndex,
                       UINT8                matchLength,
                       const TSearchArea*   searchArea,
                       IPostalCodeAddress** postalCodeAddress );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeAddressAttribute( IAllocator*              allocator,
                                INT32                    roadMapIndex,
                                INT32                    pocodeMapIndex,
                                TResultCostMode          resultCostMode,
                                IPostalCodeQuery*        postalCodeQuery,
                                IPlaceFilter*            placeFilter,
                                IResultListBuilder*      resultListBuilder,
                                IResultGroupCache*       postalCodeResultGroupCache,
                                const TTuningParameters* tuningParameters,
                                IEngineFactoryQueue*     engineFactoryQueue,
                                query::IObject**         postalCodeAddressAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeAddressAttributeFactory( IAllocator*              allocator,
                                       TResultCostMode          resultCostMode,
                                       INT32                    roadMapIndex,
                                       INT32                    pocodeMapIndex,
                                       IPlaceFilter*            placeFilter,
                                       IResultListBuilder*      resultListBuilder,
                                       IResultGroupCache*       postalCodeResultGroupCache,
                                       const TTuningParameters* tuningParameters,
                                       IEngineFactoryQueue*     engineFactoryQueue,
                                       query::IObject*          outerUnknown,
                                       query::IObject**         postalCodeAddressAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeEntity( IAllocator*         allocator,
                      const TGlobalPlace* globalPlaces,
                      UINT32              globalPlaceCount,
                      const TRect*        placeBounds,
                      BOOL8               flagNoRoads,
                      IPostalCodeEntity** postalCodeEntity );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeQuery( IAllocator*                 allocator,
                     BOOL8                       selectFullMatch,
                     BOOL8                       selectPartialMatch,
                     IPostalCodeUnicodeSeqQuery* postalCodeUnicodeSeqQuery,
                     IPostalCodeQuery**          postalCodeQuery );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeQueryFactory( IAllocator*      allocator,
                            query::IObject*  outerUnknown,
                            query::IObject** postalCodeQueryFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeResult( IAllocator*        allocator,
                      IStructuredText*   resultName,
                      IPostalCodeEntity* postalCodeEntity,
                      ISearchResult**    postalCodeResult );

query::TStatus
makePostalCodeResult( IAllocator*        allocator,
                      IStructuredText*   resultName,
                      IPostalCodeEntity* postalCodeEntity,
                      UINT32             locationSegmentPtr,
                      UINT32             locationSegmentRoadSet,
                      UINT16             locationSegmentPlace,
                      UINT16             locationSegmentProportion,
                      TPos               locationPosition,
                      ISearchResult**    postalCodeResult );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeResultGroup( IAllocator*              allocator,
                           IAllocator*              outputAllocator,
                           const TTuningParameters* tuningParameters,
                           IPlaceEntityCache*       placeEntityCache,
                           const UTF16*             resultGroupKey,
                           IResultGroupCache*       resultGroupCache,
                           IResultGroup**           postalCodeResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeResultGroupFactory( IAllocator*              allocator,
                                  IAllocator*              outputAllocator,
                                  const TTuningParameters* tuningParameters,
                                  IPlaceEntityCache*       placeEntityCache,
                                  IResultGroupFactory**    resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makePostalCodeUnicodeSeqQueryFactory( IAllocator*      allocator,
                                      BOOL8            flagDetailed,
                                      UINT16           mapIndex,
                                      query::IObject*  outerUnknown,
                                      query::IObject** postalCodeUnicodeSeqQueryFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeResultCostModePassEngine( IAllocator*          allocator,
                              IEngineFactoryQueue* exactMatchEngineFactoryQueue,
                              IEngineFactoryQueue* nonExactMatchEngineFactoryQueue,
                              IResultListBuilder*  resultListBuilder,
                              TQueryProgress*      queryProgress,
                              UINT32               tailProgressNumerator,
                              query::IEngine*      tailEngine,
                              query::IEngine**     resultCostModePassEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeResultGroupCache( IAllocator*          allocator,
                      IResultGroupFactory* resultGroupFactory,
                      IResultGroupCache**  resultGroupCache );

//--------------------------------------------------------------------------------
query::TStatus
makeResultList( IAllocator*          allocator,
                ISearchResult*const* results,
                UINT32               resultCount,
                IResultList**        resultList );

//--------------------------------------------------------------------------------
query::TStatus
makeResultListBuilder( IAllocator*          allocator,
                       IAllocator*          outputAllocator,
                       UINT32               maxOutputResultCount,
                       IResultListBuilder** resultListBuilder,
                       query::IEngine**     queryTailEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeResultNameFactoryFactoryEngine( IAllocator*                             allocator,
                                    IAllocator*                             outputAllocator,
                                    const TTuningParameters*                tuningParameters,
                                    IPlaceEntityCache*                      placeEntityCache,
                                    UINT32                                  entityCount,
                                    IResultNameFactoryEntityStrategy*const* entityStrategies,
                                    UINT32                                  significantResultCount,
                                    IResultNameFactoryFactoryCallback*      factoryCallback,
                                    query::IEngine**                        factoryEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeRoadAddress( IAllocator*                    allocator,
                 UINT16                         roadMapIndex,
                 TRoadRoadIndex32               roadIndex,
                 TRoadRoadNameIndex32           nameIndex,
                 UINT32                         matchedTokenPositions,
                 const TTokenHighlight*         matchedTokens,
                 TRoadSectorPlacePairSetIndex32 SPPSetIndex,
                 UINT32                         SPPSetSPPCount,
                 const TAreaMatchTypePair*      SPPSetAreaMatchTypes,
                 const TRect*                   glueBounds,
                 TAreaMatchType                 areaMatchType,
                 const TSearchArea*             searchArea,
                 const TPlaceQualifier*         placeQualifier,
                 IRoadAddress**                 roadAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeRoadAddressAttribute( IAllocator*               allocator,
                          TResultCostMode           resultCostMode,
                          INameDataSource*          roadNameOrRouteNameDataSource,
                          INameAttribute*           roadNameOrRouteNameAttribute,
                          IPlaceQualifierAttribute* placeQualifierAttribute,
                          IPlaceFilter*             placeFilter,
                          IResultListBuilder*       resultListBuilder,
                          IResultGroupCache*        roadResultGroupCache,
                          const TTuningParameters*  tuningParameters,
                          IEngineFactoryQueue*      engineFactoryQueue,
                          query::IObject**          roadAddressAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeRoadAddressAttributeFactory( IAllocator*              allocator,
                                 TResultCostMode          resultCostMode,
                                 INameDataSource*         roadNameOrRouteNameDataSource,
                                 IPlaceFilter*            placeFilter,
                                 IResultListBuilder*      resultListBuilder,
                                 IResultGroupCache*       roadResultGroupCache,
                                 const TTuningParameters* tuningParameters,
                                 IEngineFactoryQueue*     engineFactoryQueue,
                                 query::IObject*          outerUnknown,
                                 query::IObject**         roadAddressAtributeFactory );

//--------------------------------------------------------------------------------
//for TW/CN first-phonetic search
query::TStatus
makeFirstLetterRoadAddressAttribute( IAllocator*               allocator,
                                     TResultCostMode           resultCostMode,
                                     INameDataSource*          roadNameOrRouteNameDataSource,
                                     INameAttribute*           roadNameOrRouteNameAttribute,
                                     IPlaceQualifierAttribute* placeQualifierAttribute,
                                     IPlaceFilter*             placeFilter,
                                     IResultListBuilder*       resultListBuilder,
                                     IResultGroupCache*        roadResultGroupCache,
                                     const TTuningParameters*  tuningParameters,
                                     IEngineFactoryQueue*      engineFactoryQueue,
                                     query::IObject**          roadAddressAttribute );

query::TStatus
makeFirstLetterRoadAddressAttributeFactory( IAllocator*              allocator,
                                            TResultCostMode          resultCostMode,
                                            INameDataSource*         roadNameOrRouteNameDataSource,
                                            IPlaceFilter*            placeFilter,
                                            IResultListBuilder*      resultListBuilder,
                                            IResultGroupCache*       roadResultGroupCache,
                                            const TTuningParameters* tuningParameters,
                                            IEngineFactoryQueue*     engineFactoryQueue,
                                            query::IObject*          outerUnknown,
                                            query::IObject**         roadAddressAtributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeRoadEntity( IAllocator*                    allocator,
                const IRoadEntity::TLocalRoad* localRoads,
                UINT32                         localRoadCount,
                const TRect*                   roadBounds,
                IRoadEntity**                  roadEntity );

//--------------------------------------------------------------------------------
query::TStatus
makeRoadEntityFactoryEngine( IAllocator*                    allocator,
                             IAllocator*                    outputAllocator,
                             const TTuningParameters*       tuningParameters,
                             const UTF16*                   roadIdentityName,
                             const IRoadEntity::TLocalRoad* localRoads,
                             UINT32                         localRoadCount,
                             IRoadEntityFactoryCallback*    factoryCallback,
                             query::IEngine**               factoryEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeRoadFilter( IAllocator*             allocator,
                TGenericMapInfo*        roadMapInfo,
                const TRoadRoadIndex32* roads,
                UINT32                  roadCount,
                ISearchFilter**         roadFilter );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeRoadNameOrRouteNameFilter( IAllocator*      allocator,
                               TGenericMapInfo* roadMapInfo,
                               const TBitset*   placeFilterBitset,
                               const TBitset*   sectorFilterBitset,
                               ISearchFilter**  roadNameOrRouteNameFilter );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

query::TStatus
makeRoadNameOrRouteNameFilter( IAllocator*      allocator,
                               TGenericMapInfo* roadMapInfo,
                               const TBitset*   roadFilterBitset,
                               ISearchFilter**  roadNameOrRouteNameFilter );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeRoadResult( IAllocator*              allocator,
                IStructuredText*         resultName,
                IRoadEntity*             roadEntity,
                IRoadEntity*             alternateRoadEntity,
                BOOL8                    flagFuzzyMatch,
                const TBitset*           localRoadFilterBitset,
                UINT32                   exactPlaceCount,
                UINT32                   nonP1PlaceCount,
                UINT32                   fuzzyPlaceCount,
                const TGlobalPlaceLink*  places,
                UINT32                   sectorCount,
                const TGlobalSectorLink* sectors,
                const TSearchArea*       searchArea,
                const TTuningParameters* tuningParameters,
                ISearchResult**          roadResult );

query::TStatus
makeRoadResult( IAllocator*              allocator,
                IStructuredText*         resultName,
                IRoadEntity*             roadEntity,
                IRoadEntity*             alternateRoadEntity,
                BOOL8                    flagFuzzyMatch,
                const TBitset*           localRoadFilterBitset,
                UINT32                   exactPlaceCount,
                UINT32                   nonP1PlaceCount,
                UINT32                   fuzzyPlaceCount,
                const TGlobalPlaceLink*  places,
                UINT32                   sectorCount,
                const TGlobalSectorLink* sectors,
                const TSearchArea*       searchArea,
                const TTuningParameters* tuningParameters,
                UINT32                   locationSegmentPtr,
                UINT16                   locationSegmentPlace,
                INT16                    locationSegmentHeading,
                UINT16                   locationSegmentProportion,
                TRoadSide                locationSegmentSide,
                TPos                     locationPosition,
                ISearchResult**          roadResult );

//--------------------------------------------------------------------------------
query::TStatus
makeRoadResultGroup( IAllocator*              allocator,
                     IAllocator*              outputAllocator,
                     const TTuningParameters* tuningParameters,
                     IPlaceEntityCache*       placeEntityCache,
                     const UTF16*             resultGroupKey,
                     IResultGroupCache*       resultGroupCache,
                     IResultGroup**           roadResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeRoadResultGroupFactory( IAllocator*              allocator,
                            IAllocator*              outputAllocator,
                            const TTuningParameters* tuningParameters,
                            IPlaceEntityCache*       placeEntityCache,
                            IResultGroupFactory**    resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeSearchFilter( IAllocator*     allocator,
                  const TBitset*  filterBitset,
                  ISearchFilter** searchFilter );

//--------------------------------------------------------------------------------
query::TStatus
makeSearchLocation( IAllocator*              allocator,
                    const TTuningParameters* tuningParameters,
                    const TGlobalPlace*      globalPlaces,
                    UINT32                   globalPlaceCount,
                    BOOL8                    flagLocalSearch,
                    const TRect*             searchBounds,
                    IRoadEntity*             baseRoad,
                    IRoadEntity*const*       roadEntities,
                    UINT32                   roadEntityCount,
                    const TSearchArea*       searchArea,
                    query::IObject*          outerUnknown,
                    query::IObject**         searchLocation );

//--------------------------------------------------------------------------------
query::TStatus
makeSearchQuery( IAllocator*           allocator,
                 IAllocator*           outputAllocator,
                 query::IObject*const* searchParameters,
                 UINT32                searchParametersCount,
                 UINT32                maxResultCount,
                 ISearchQuery**        searchQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeSectorFilter( IAllocator*      allocator,
                  TGenericMapInfo* roadMapInfo,
                  const TRect*     searchBounds,
                  ISearchFilter**  sectorFilter,
                  UINT32*          outPassCount = NULL );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeSegmentLocationSearchEngine( IAllocator*                     allocator,
                                 const TPos*                     searchCentroid,
                                 const TRect                     searchBounds[],
                                 UINT16                          searchBoundsCount,
                                 ISegmentLocationSearchCallback* searchCallback,
                                 query::IEngine**                searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeSpellingCharacterQueryFactory( IAllocator*      allocator,
                                   query::IObject*  outerUnknown,
                                   query::IObject** spellingCharacterQueryFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeSpellingCharacterSeqQueryFactory( IAllocator*      allocator,
                                      INameDataSource* nameDataSource,
                                      query::IObject*  outerUnknown,
                                      query::IObject** spellingCharacterSeqQueryFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeSpellingTokenPartQuery( IAllocator*                 allocator,
                            BOOL8                       selectFullMatch,
                            BOOL8                       selectPartialMatch,
                            ISpellingCharacterSeqQuery* spellingCharacterSeqQuery,
                            ITokenPartQuery**           tokenPartQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeStructuredText( IAllocator*           allocator,
                    IStructuredTextInit** structuredTextInit );

query::TStatus
makeStructuredText( IAllocator*       allocator,
                    const UTF16*      string,
                    IStructuredText** structuredText );

//--------------------------------------------------------------------------------
query::TStatus
makeTokenPartQueryFactory( IAllocator*      allocator,
                           query::IObject*  outerUnknown,
                           query::IObject** tokenPartQueryFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeTokenPartSeqQueryFactory( IAllocator*      allocator,
                              TResultCostMode  resultCostMode,
                              INameDataSource* nameDataSource,
                              BOOL8            flagRoute,
                              query::IObject*  outerUnknown,
                              query::IObject** tokenPartSeqQueryFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeTokenSeqAttributeFactory( IAllocator*      allocator,
                              INameDataSource* nameDataSource,
                              query::IObject*  outerUnknown,
                              query::IObject** tokenSeqAttributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeTokenSeqNameAttribute( IAllocator*         allocator,
                           ITokenSeqAttribute* tokenSeqAttribute,
                           INameDataSource*    nameDataSource,
                           ISearchFilter*      nameFilter,
                           BOOL8               exactMatch,
                           INameTableScanTask* nameTableScanTask,
                           INameAttribute**    nameAttribute );

//--------------------------------------------------------------------------------
query::TStatus
makeTokenSeqNameAttributeFactory( IAllocator*         allocator,
                                  TResultCostMode     resultCostMode,
                                  INameDataSource*    nameDataSource,
                                  INameTableScanTask* nameTableScanTask,
                                  ISearchFilter*      nameFilter,
                                  query::IObject*     outerUnknown,
                                  query::IObject**    tokenSeqNameAttributeFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeTokenSet( IAllocator*         allocator,
              INameDataSource*    tokenDataSource,
              ITokenPartSeqQuery* tokenPartSeqQuery,
              BOOL8               allowPartialTokens,
              ITokenSet**         tokenSet );

//--------------------------------------------------------------------------------
query::TStatus
makeUnicodeQuery( IAllocator*        allocator,
                  parser::ITerminal* terminal,
                  IUnicodeQuery**    unicodeQuery );

//--------------------------------------------------------------------------------
query::TStatus
makeUnionFilter( IAllocator*          allocator,
                 ISearchFilter*const* filters,
                 UINT32               filterCount,
                 ISearchFilter**      unionFilter );
//NOTE this function is implemented in
//     <geosearch/gsSearchFilter.cpp>

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOIAddress( IAllocator*             allocator,
                    IUserPOISearch*         userPOISearch,
                    IUserPOISearch::TResult userPOI,
                    UINT32                  matchLength,
                    IUserPOIAddress**       userPOIAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOIEnumerateUserPOITypeSearchEngine( IAllocator*             allocator,
                                             IUserPOISearch*         userPOISearch,
                                             IUserPOISearch::TResult userPOI,
                                             IResultListBuilder*     resultListBuilder,
                                             IResultGroupCache*      userPOITypeResultGroupCache,
                                             query::IEngine*         tailEngine,
                                             query::IEngine**        searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOIEnumerateUserPOITypeSearchParameters( IAllocator*                                    allocator,
                                                 IUserPOISearch*                                userPOISearch,
                                                 IUserPOISearch::TResult                        userPOI,
                                                 IUserPOIEnumerateUserPOITypeSearchParameters** userPOIEnumerateUserPOITypeSearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOIResult( IAllocator*             allocator,
                   IStructuredText*        resultName,
                   IUserPOISearch*         userPOISearch,
                   IUserPOISearch::TResult userPOI,
                   UINT32                  segmentId,
                   TRoadPlaceIndex16       segmentPlaceIndex,
                   UINT16                  segmentProportion,
                   TRoadSide               segmentSide,
                   TPos                    segmentPosition,
                   ISearchResult**         userPOIResult );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOIResultGroup( IAllocator*              allocator,
                        IAllocator*              outputAllocator,
                        const TTuningParameters* tuningParameters,
                        IPlaceEntityCache*       placeEntityCache,
                        const UTF16*             resultGroupKey,
                        IResultGroupCache*       resultGroupCache,
                        IResultGroup**           userPOIResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOIResultGroupFactory( IAllocator*              allocator,
                               IAllocator*              outputAllocator,
                               const TTuningParameters* tuningParameters,
                               IPlaceEntityCache*       placeEntityCache,
                               IResultGroupFactory**    resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOISearchEngine( IAllocator*         allocator,
                         const TRect*        searchBounds,
                         const UTF16*        searchString,
                         const TPOITypeId16* POITypes,
                         UINT32              POITypeCount,
                         void*               userPOIContext,
                         IResultListBuilder* resultListBuilder,
                         IResultGroupCache*  userPOIResultGroupCache,
                         query::IEngine*     tailEngine,
                         query::IEngine**    searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOISearchEngineFactory( IAllocator*         allocator,
                                const TRect*        searchBounds,
                                const UTF16*        searchString,
                                const TPOITypeId16* POITypes,
                                UINT32              POITypeCount,
                                void*               userPOIContext,
                                IResultListBuilder* resultListBuilder,
                                IResultGroupCache*  userPOIResultGroupCache,
                                IEngineFactory**    userPOISearchEngineFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOISearchParameters( IAllocator*                allocator,
                             const UTF16*               searchString,
                             const TRect*               searchBounds,
                             TPOITypeId16               POITypeId,
                             void*                      userPOIContext,
                             IPlaceFilterCache*         placeFilterCache,
                             const TTuningParameters*   tuningParameters,
                             IUserPOISearchParameters** userPOISearchParameters );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOITypeAddress( IAllocator*             allocator,
                        IUserPOISearch*         userPOISearch,
                        IUserPOISearch::TResult userPOIType,
                        UINT32                  matchLength,
                        TGeosearchResultType    searchStringMatchType,
                        IUserPOITypeAddress**   userPOITypeAddress );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOITypeResult( IAllocator*             allocator,
                       IStructuredText*        resultName,
                       IUserPOISearch*         userPOISearch,
                       IUserPOISearch::TResult userPOIType,
                       TGeosearchResultType    searchStringMatchType,
                       ISearchResult**         userPOITypeResult );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOITypeResultGroup( IAllocator*        allocator,
                            IAllocator*        outputAllocator,
                            const UTF16*       resultGroupKey,
                            IResultGroupCache* resultGroupCache,
                            IResultGroup**     userPOITypeResultGroup );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOITypeResultGroupFactory( IAllocator*           allocator,
                                   IAllocator*           outputAllocator,
                                   IResultGroupFactory** resultGroupFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOITypeSearchEngine( IAllocator*          allocator,
                             const TRect*         searchBounds,
                             const UTF16*         searchString,
                             TGeosearchResultType searchStringMatchType,
                             void*                userPOIContext,
                             IResultListBuilder*  resultListBuilder,
                             IResultGroupCache*   userPOITypeResultGroupCache,
                             query::IEngine*      tailEngine,
                             query::IEngine**     searchEngine );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOITypeSearchEngineFactory( IAllocator*          allocator,
                                    const TRect*         searchBounds,
                                    const UTF16*         searchString,
                                    TGeosearchResultType searchStringMatchType,
                                    void*                userPOIContext,
                                    IResultListBuilder*  resultListBuilder,
                                    IResultGroupCache*   userPOITypeResultGroupCache,
                                    IEngineFactory**     userPOITypeSearchEngineFactory );

//--------------------------------------------------------------------------------
query::TStatus
makeUserPOITypeSearchParameters( IAllocator*                    allocator,
                                 TGeosearchResultType           searchType,
                                 const UTF16*                   searchString,
                                 const TRect*                   searchBounds,
                                 void*                          userPOIContext,
                                 IUserPOITypeSearchParameters** userPOITypeSearchParameters );

//--------------------------------------------------------------------------------
} //namespace geosearch
} //namespace innov

#endif //INCLUDE_GUARD_gsClassFactory_h
