#ifndef ENSVMP_H
#define ENSVMP_H

#include <mapdata/ensvm.h>
#include <mapdata/enroadP.h>
#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ensvm: software virtual memory library */


/*
 * Number of primary cached virtual address (has to be a power of 2)
 * Each entry in primary cache takes 8 bytes (2 addresses).
 */
#define SVM_CACHE_1ST_LEVEL_COUNT 8192

extern UINT32 totalSlotCount;
extern UINT32 svm_lockedItemCount;

/*
 * Some floating & locked blocks in map cache correspond to a sector
 * or super sector header stored in ensvm.  When a block is in
 * map cache (locked or floating), its sector header is locked
 * in ensvm.  As a consequence, map cache must not store more
 * blocks than ensvm can store.  Fortunately, this is almost always
 * the case because blocks in map cache are big so map cache cannot
 * store many of them.  However, in some rare case, it is possible
 * that map cache allocate many small blocks (sectors with just one
 * roads in Australia for example). In such case, the map cache can
 * hold several thousands of such small blocks and ensvm may run out
 * of nodes if SVM_CACHE_2ND_LEVEL_COUNT were too small.  One simple
 * way to prevent this situation is to limit the number of blocks in
 * map cache to SVM_CACHE_2ND_LEVEL_COUNT (minus a small margin
 * because a few blocks in ensvm can be locked for other reasons
 * than block in map cache)
 *
 * Additionally, we must not lock too many slots or else svm would
 * perform poorly as only a few slots would be free and svm would
 * thrash.
 */
#define SVM_LOCKED_THRESHOLD ( totalSlotCount -      \
                              (totalSlotCount >> 3) \
                             )

/*
 * Check whether we've reached a threshold for number of locked
 * items in svm.  When this threshold is reached, map cache will
 * free a block before allocating a new one to prevent having too
 * many locked blocks in svm.  If most of the blocks were locked
 * in svm, performance would be degraded as svm would thrash.
 * Note the it is not expected to happen often.  This situation
 * would only happen if lots of small blocks were pulled in
 * map cache which is not a typical situation.
 */
#define SVM_MAX_LOCKED_REACHED() \
  (svm_lockedItemCount >= SVM_LOCKED_THRESHOLD)
#define SVM_HALF_MAX_LOCKED_REACHED() \
  (svm_lockedItemCount >= (SVM_LOCKED_THRESHOLD >> 1))


/* Types that ensvm lib is caching */
typedef enum TSlotType {
  SLOT_TYPE_ROAD_SECTOR_HEADER,
  SLOT_TYPE_ROAD_SUPER_SECTOR_HEADER,

  SLOT_TYPE_GEOM_LOWER_SECTOR_HEADER,
  SLOT_TYPE_GEOM_UPPER_SECTOR_HEADER,

  SLOT_TYPE_POI_SECTOR_HEADER,
  SLOT_TYPE_POI_SUPER_SECTOR_HEADER,

  SLOT_TYPE_COUNT
} TSlotType;

/* Last virtual address + 1 */
extern UINT32 svm_lastVa;

/* Initialize the ensvm lib */
extern BOOL8 svm_init(void);

/*
 * Lock a physical address.  Locking ensures that the physical
 * memory at address pa won't be recycled by the ensvm lib api.
 * Nested locking is allowed, a block of memory can be locked
 * several times and will need to be unlocked as many times as
 * it has been previously locked.
 */
extern void svm_lockPa(void *pa);

/*
 * Unlock a block of memory previously locked by svm_lockPa().
 * Once unlocked, a block of physical memory can be recycled
 * whenever ensvm api is invoked
 */
extern void svm_unlockPa_(void *pa);
#ifdef SIMULATOR
// always use debug on simulator
#define svm_unlockPa(pa) { svm_unlockPa_(pa); pa = NULL; }
#else
// debug too expensive on target
#define svm_unlockPa(pa) svm_unlockPa_(pa)
#endif

/* Transform a physical address into virtual address */
extern UINT32 svm_pa2va(const void *pa);

/*
 * Resolve a virtual address <va> pointing the object of type <type>.
 * It returns a physical address pointing to the object at the
 * specified virtual address <va>.  The physical address is unlocked so
 * it is safe only if there are no further calls to ensvm api.
 *
 * This function is not supposed to be called directly, user should
 * invoke SVM_RESOLVE_*() macros defined below for each kind of
 * lockable objects.
 */
extern void *svm_resolveVa(const UINT32 va, TSlotType type);

// These offsets are not used by non-indirect segments and nodes
#define UNUSED_NODE_OFFSET 0xFFFF
#define UNUSED_SEG_OFFSET 0xFFFF
extern void svm_roadSectorReserveDynamicTables(
 TRoadSectorHeader *pSectorHeader);

extern void svm_dynamicBuffAccessed(TDynamicRoadSectorBuff *dynamicBuff);

#define SVM_RESOLVE_ROAD_SUPER_SECTOR_HEADER(va) (TRoadSuperSectorHeader *) \
  (svm_resolveVa((va), SLOT_TYPE_ROAD_SUPER_SECTOR_HEADER))

#define SVM_RESOLVE_ROAD_SECTOR_HEADER(va) (TRoadSectorHeader *)        \
  (svm_resolveVa((va), SLOT_TYPE_ROAD_SECTOR_HEADER))

#define SVM_RESOLVE_GEOM_UPPER_SECTOR_HEADER(va) (TGeomUpperSectorHeader *) \
  (svm_resolveVa((va), SLOT_TYPE_GEOM_UPPER_SECTOR_HEADER))

#define SVM_RESOLVE_GEOM_LOWER_SECTOR_HEADER(va) (TGeomLowerSectorHeader *) \
  (svm_resolveVa((va), SLOT_TYPE_GEOM_LOWER_SECTOR_HEADER))

#define SVM_RESOLVE_POI_SUPER_SECTOR_HEADER(va) (TPOISuperSectorHeader *) \
  (svm_resolveVa((va), SLOT_TYPE_POI_SUPER_SECTOR_HEADER))

#define SVM_RESOLVE_POI_SECTOR_HEADER(va) (TPOISectorHeader *)          \
  (svm_resolveVa((va), SLOT_TYPE_POI_SECTOR_HEADER))

#ifdef __cplusplus
}
#endif

#endif /* ENSVMP_H */
