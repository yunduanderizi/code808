#ifndef ENROADSCANP_H
#define ENROADSCANP_H

#include <mapdata/enroadP.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup MapData
///    @{

/// @internal
///
/// \brief An iterator that loops on a road sector returning nodes.
///
/// The iterator should be created and initialised to point to
/// the first element by nodeIterator_first().
///
/// The iterator should be destroyed by
/// nodeIterator_free().
///
/// Use nodeIterator_next() and optionally
/// nodeIterator_hasNext() to loop through the iterator.
///
///
/// @code
///    TNodeIterator nodeIterator;
///    TRoadNode node;
///
///    nodeIterator_first(&nodeIterator, sector, header);
///    while (nodeIterator_next(&nodeIterator, &node)) {
///      nodeFunc(&node); // do some stuff with the node now we have it
///    } // end of loop over sector
///    assert(!nodeIterator_hasNext(&nodeIterator));
///    nodeIterator_free(&nodeIterator);
///
/// @endcode
typedef struct TNodeIterator {
  UINT32 nodeOffset;
  UINT32 nodeEnd;
  TRoadSectorHeader   *pSector;
  BOOL8 sectorLocked;
} TNodeIterator;

void nodeIterator_first(TNodeIterator *pThis,
                        TRoadSectorHeader   *pSector);

BOOL8 nodeIterator_next(TNodeIterator *pThis,
                        TRoadNode *pNode);

BOOL8 nodeIterator_hasNext(TNodeIterator *pThis);

void nodeIterator_free(TNodeIterator *pThis);

/// @internal
///
/// \brief An iterator that loops on a road sector returning nodes.
///
/// The iterator should be created and initialised to point to
/// the first element by segmentIterator_first().
///
/// The iterator should be destroyed by
/// segmentIterator_free().
///
/// Use segmentIterator_next() and optionally
/// segmentIterator_hasNext() to loop through the iterator.
///
///
/// @code
///    TSegmentIterator segmentIterator;
///    TRoadSegment segment;
///
///    segmentIterator_first(&segmentIterator, sector, header);
///    while (segmentIterator_next(&segmentIterator, &segment)) {
///      segmentFunc(&segment); // do some stuff with the node now we have it
///    } // end of loop over sector
///    assert(!segmentIterator_hasNext(&segmentIterator));
///    segmentIterator_free(&segmentIterator);
///
/// @endcode
typedef struct TSegmentIterator {
  UINT32 segmentOffset;
  UINT32 segmentEnd;
  TRoadSectorHeader   *pSector;
  BOOL8 sectorLocked;
} TSegmentIterator;

void segmentIterator_first(TSegmentIterator *pThis,
                           TRoadSectorHeader   *pSector,
                           TRoadType maxRoadType);
    
BOOL8 segmentIterator_next(TSegmentIterator *pThis,
                           TRoadSegment *pSegment);

BOOL8 segmentIterator_hasNext(TSegmentIterator *pThis);

void segmentIterator_free(TSegmentIterator *pThis);

///
/// @internal
///
/// \brief An iterator that loops on a road super sector returning
/// sectors.
///
/// The iterator should be created and initialised to point to
/// the first element by roadSectorIterator_first().
///
/// The iterator should be destroyed by
/// roadSectorIterator_free().
///
/// Use roadSectorIterator_next() and optionally
/// roadSectorIterator_hasNext() to loop through the iterator.
///
///
/// @code
///    TRoadSectorIterator sectorIterator;
///    TRoadSectorHeader   *sector;
///
///    roadSectorIterator_first(&sectorIterator, superSector, header);
///    while (roadSectorIterator_next(&sectorIterator, &sector)) {
///        sector_func(sector); // do something with the sector
///    }
///    assert(!roadSectorIterator_hasNext(&sectorIterator));
///    roadSectorIterator_free(&sectorIterator);
///
/// @endcode

typedef struct TRoadSectorIterator {
  UINT32 index;
  UINT32 sectorCount;
  UINT32 pSectorVa;
  TRoadSectorHeader *pSectorPa;
 } TRoadSectorIterator;

void roadSectorIterator_first(TRoadSectorIterator *pThis,
                              TRoadSuperSectorHeader *pSuperSector,
                              TGenericMapInfo *const pMapInfo);

BOOL8 roadSectorIterator_next(TRoadSectorIterator *pThis,
                              TRoadSectorHeader** ppSector);

BOOL8 roadSectorIterator_hasNext(TRoadSectorIterator *pThis);

void roadSectorIterator_free(TRoadSectorIterator *pThis);

/// @internal
///
/// \brief An iterator that loops on a map and returns the super sectors
///        for a given map layer.
///
/// The iterator should be created and initialised to point to
/// the first element by roadSuperSectorIterator_first().
///
/// The iterator should be destroyed by
/// roadSuperSectorIterator_free().
///
/// Use roadSuperSectorIterator_next() and optionally
/// roadSuperSectorIterator_hasNext() to loop through the iterator.
///
/// @code
///    TRoadSuperSectorIterator superSectorIterator;
///    TRoadSuperSectorHeader *superSector;
///
///    roadSuperSectorIterator_first(&superSectorIterator, header, 0);
///    while (roadSuperSectorIterator_next(&superSectorIterator, &superSector)) {
///        superSector_func(superSector); // do something with super sector
///    }
///    assert(!roadSuperSectorIterator_hasNext(&superSectorIterator));
///    roadSuperSectorIterator_free(&superSectorIterator);
///
/// @endcode
typedef struct TRoadSuperSectorIterator {
  UINT32 index;
  UINT32 superSectorCount;
  UINT32 superSectorVa;
  TRoadSuperSectorHeader *superSectorPa;
} TRoadSuperSectorIterator;

void roadSuperSectorIterator_first(TRoadSuperSectorIterator *pThis,
                                   TGenericMapInfo *const pMapHeader,
                                   UINT32 mapLayer);

BOOL8 roadSuperSectorIterator_next(TRoadSuperSectorIterator *pThis,
                           TRoadSuperSectorHeader** ppSuperSector);

BOOL8 roadSuperSectorIterator_hasNext(TRoadSuperSectorIterator *pThis);

void roadSuperSectorIterator_free(TRoadSuperSectorIterator *pThis);

/** @param[in] pointer to a Node */
typedef void TGenericNodeFunc(TRoadNode *pNode);
typedef void TGenericSegmentFunc(TRoadSegment *pSegment);

UINT32 road_loopOnAllNodes(TGenericNodeFunc *nodeFunc);
UINT32 road_loopOnAllSegments(TGenericSegmentFunc *segmentFunc);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* ENROADSCANP_H */
