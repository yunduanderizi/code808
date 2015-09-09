#ifndef TRSECTORMULTISETP_H
#define TRSECTORMULTISETP_H

#include <platform.h>
#include <mapdata/enroadP.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TSectorMultisetElement {
  UINT32 sectorHeaderVa;
  UINT8 multiplicity;
} TSectorMultisetElement;

// TODO: This is really too many for the simple linear search I'm
// using.  We should look at either adding a supersector layer above
// this or using a binary tree to store this.
#define SECTOR_MULTISET_MAX_ELEMENTS 2000

typedef struct TSectorMultiset {
  TSectorMultisetElement elements[SECTOR_MULTISET_MAX_ELEMENTS];
  UINT32 numElements;
} TSectorMultiset;

void sectorMultiset_init(TSectorMultiset *multiset);
void sectorMultiset_addSector(TSectorMultiset *multiset,
                              const TRoadSectorHeader *sectorHeader);
void sectorMultiset_removeSector(TSectorMultiset *multiset,
                                 const TRoadSectorHeader *sectorHeader);
UINT32 sectorMultiset_containsSector(const TSectorMultiset *multiset,
                                     const TRoadSectorHeader *sectorHeader);
UINT32 sectorMultiset_containsSectorVa(const TSectorMultiset *multiset,
                                       const UINT32 sectorHeaderVa);

#ifdef __cplusplus
}
#endif

#endif
