#ifndef ENPOCODEP_H
#define ENPOCODEP_H

/*
 * This module provides a number fo API calls that allow the pocode map data to
 * be accessed in a way that is reasonably independent of the underlying map
 * data format.  Typically the binary map data is unpacked into a C structure
 * where it can be more easily accessed.
 */

#include <mapdata/enpocode.h>
#include <math/gerect.h>
#include <mapdata/enroadP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Sizes of main and sub post code strings (without EOS).
 * These should ideally not be hardcoded in map core as mapdata
 * actually specifies size of strings in global chunk.  However,
 * since detailed post codes are only used in UK where post
 * codes are 4+4 char and for simplicity sake, they are currently
 * hardcoded
 */
#define POCODE_MAIN_MAX_LEN 4
#define POCODE_SUB_MAX_LEN  4//3

#define POCODE_SUB_CHUNK_COUNT          64
#define POCODE_SEGMENT_LINK_CHUNK_COUNT POCODE_SUB_CHUNK_COUNT

/*
 * These values should be consistent with chunk directory structure
 */
typedef enum {
  POCODE_CHUNK_IDX_MAIN,
  POCODE_CHUNK_IDX_SEGMENT_LINK,
  POCODE_CHUNK_IDX_SUB    = POCODE_CHUNK_IDX_SEGMENT_LINK + POCODE_SUB_CHUNK_COUNT
} TPocodeChunkIdx;

/*
 * This flag is set if segment attachment comes from map provider
 * and not set if segment attachment was computed by post code tool
 * chain.
 */
#define POCODE_FLAG_EXACT_SEGMENT_LINK 1


/* ONDISK global data chunk definition - this data applies to a whole map */
typedef struct TBinPocodeGlobals {
  /* global map data - bounds, etc */
  TRect  bounds;      /* the map area is within these bounds */
  UINT8  maxSizeMainStr;   /* size of main post code strings (currently 4) */
  UINT8  maxSizeSubStr;    /* size of sub post code string (currently 4) */
  UTF16  mainSubSeparator; /* sub/main post code separator (currently space) */
  UINT8  resX, resY;       /* resolution bit shift to apply in x or y */
  UINT8  flags;

  /* Following information is used to link to road data */
  UINT32 crcRoadMap;       /* crc of matching road map to use for
                            * | segment links */
} TBinPocodeGlobals;

/* An instance of the following structure hangs off every MapInfo
 * structure corresponding to a pocode map. At present it contains
 * everything from the ONDISK structure, and a pointer to the matching
 * roadMapInfo in addition.
 */
typedef struct TPocodeMapGlobals {
  /* global map data - bounds, etc */
  TRect  bounds;      /* the map area is within these bounds */
  UINT8  maxSizeMainStr;   /* size of main post code strings (currently 4) */
  UINT8  maxSizeSubStr;    /* size of sub post code string (currently 4) */
  UTF16  mainSubSeparator; /* sub/main post code separator (currently space) */
  UINT8  resX, resY;       /* resolution bit shift to apply in x or y */
  UINT8  flags;

  /* Following information is used to link to road data */
  UINT32 crcRoadMap;       /* crc of matching road map to use for
                            * | segment links */
  /* The layout of the fields above this comment is in agreement with
   * the ondisk pocode globals; this layout should not be tampered
   * with. */

  /* The following field(s) are of value to innovEngine only at runtime. */
  struct TGenericMapInfo *roadMapInfo; /* mapInfo of matching road map */
} TPocodeMapGlobals;

/*
 * Fixed size record for in main post code table.
 * This structure matches exactly mapfile content
 * The size of the string (str field) should ideally not be hardcoded
 * but be read from 'global.maxSizeMainPocodeStr'.  However, for simplicity
 * sake, it is currently hard coded here but map data potentially supports
 * different string sizes.
 *
 * The size of this struct is not very critical because there are only
 * a few hundreds main post code records
 */
typedef struct TPocodeMain {
  INT32  xmin, ymin;             /* min position of bounding box */
  UINT16 sizeX, sizeY;           /* size of bounding box (to be shifted
                                  * | by resX, resY) */
  BITFIELD32 subPocodeCount: 13; /* Number of sub post codes */
  BITFIELD32 maskPlace     :  3; /* 6 a0 .. a9 bua significant
                                    5 a0 .. a9 significant?
                                    4 a0 .. a8 significant?
                                    3 a0 .. a7 significant?
                                    2 a0 .. a2 significant?
                                    1 a0 a1 significant
                                    0 a0 significant */
  BITFIELD32 placeId : 16;        /* most specific place that entirely
                                   * | contains this main pocode (link to
                                   * | place table in road map) */
  UINT16 firstSubPocodeIdx;       /* index of sub post code record
                                   * | relative to beginning of its
                                   * | chunk */
  UINT8 chkIdx;                   /* index of sub pocode and
                                   * | segment link chunk */
  //TODO add EOS
  UTF16 str[POCODE_MAIN_MAX_LEN + 1];  /* main post code string (no EOS!) */
} TPocodeMain;

/*
 * Fixed size record in sub post code table.
 *
 * This structure is unpacked and may thus not match exactly
 * the fixed size records in map data.  Because size is critical
 * in map data, it is compacted without spare bytes in map data.
 * However, size is not critical after being unpacked. The string
 * for example has an EOS whereas map data does not.
 */
typedef struct TPocodeSub {
  /*
   * If direct is set, then:
   *   if firstSegOffset is == 0:
   *      sub post code does not link to any road segment (unusual!)
   *   else:
   *      sub post code link to 1 segment only, its direct segment
   *      offset in road map is stored in firstSegOffset (frequent)
   *
   * else:
   *   sub post code links to more than 1 segment and firstSegOffset
   *   field contains offset in pocode map (segmentLink table) where
   *   to  find first segment offset.  The total number of segments
   *   can be found while unpacking segment offsets in segmentLink table.
   */
  BITFIELD32 direct         :  1; /* segment link encoded directly in
                                   * | firstSegOffset */
  BITFIELD32 displayRdNm    :  1; /* can display road name for this sub
                                   * | post code because all name1 or
                                   * | all name2 are identical for
                                   * | all segments (ignoring null names) */
  BITFIELD32 maskPlace      :  3; /* what level of place hierarchy are
                                   * | relevant in placeId */
  BITFIELD32 firstSegOffset : 27; /* offset of first segment id in
                                   * | segment link chunk (when direct = 0)
                                   * | or segment id (when direct = 1) */
  TPos   pos;                     /* XY of the post code, in map units */
  UINT16 placeId;      /* most specific place that entirely
                        * | contains this main pocode (link to
                        * | place table in road map) */

  UTF16 str[POCODE_SUB_MAX_LEN + 1]; /* sub post code string (+ EOS) */
} TPocodeSub;


/*
 * A MapHeader is an interpretation of the chunk directory table which
 * makes available symbolic names to refer to the first few
 * chunks. Macros such as <maptype>_CHUNK_ADDRESS allow us to
 * translate these names in the mapHeader into chunk-directory
 * indices.
 *
 * A detailed post code map is made of 82 chunks
 * organized as described in this structure.
 */

typedef struct {
  /*
   * Table of main post code.
   * This table is fairly small.  It contains fixed size records
   * for each main post code of type TPocodeMain
   */
  struct TChunkFptr main;

  /*
   * Sub post code table.
   * This table can contain a fairly large number of fixed size
   * records of type TPocodeSub.  Since table can be large, it is
   * split into POCODE_SUB_CHUNK_COUNT chunks. All sub post codes
   * of one main post code are guaranteed to be all in same chunk
   */
  struct TChunkFptr sub[POCODE_SUB_CHUNK_COUNT];

  /*
   * POCODE_SEGMENT_LINK_CHUNK_COUNT chunks containing indirect
   * links to road segments (map offsets in road maps).  Each
   * sub post code can have 1 to n links to road segments which
   * belong to that sub post code.  If a sub post code link to
   * only 1 segment, then it is stored directly in sub post
   * code table (sub chunk).  If sub post code contains
   * several segments, then sub post code contains first
   * offset in segmentLink chunks where to find all segments
   * map offsets.
   */
  struct TChunkFptr segmentLink[POCODE_SEGMENT_LINK_CHUNK_COUNT];
} TPocodeMapHeader;

#define POSTCODE_GLOBALS(mapInfo)                       \
  (assert((mapInfo)->mapType == MAPTYPE_POSTCODE),      \
   ((TPocodeMapGlobals *)((mapInfo)->_globals)))

#define POCODE_CHUNK_INDEX(chunkName)                                   \
  ((offsetof(TPocodeMapHeader, chunkName)) / sizeof(TChunkFptr))

#define POCODE_CHUNK_SIZE_MEM(mapInfo, chunkName)               \
  CHUNK_SIZE_MEM((mapInfo), POCODE_CHUNK_INDEX(chunkName))
#define POCODE_CHUNK_IS_EMPTY(mapInfo, chunkName)               \
  CHUNK_IS_EMPTY((mapInfo), POCODE_CHUNK_INDEX(chunkName))

/*
 * This structure is used to iterate through all segment IDs of
 * a given sub post code
 */
typedef struct TPocodeSegmentIterator {
  TGenericMapInfo   *mapInfo;
  const TPocodeMain *mainPocode;   /* main post code */
  const TPocodeSub  *subPocode;    /* sub post code */
  UINT32             segLeft;      /* number of segment ID left
                                    * | to extract */
  UINT32             segOffset;
  UINT32             offset;       /* current segment offset
                                    * | segmentLink chunk */
} TPocodeSegmentIterator;

/*
 * API to access post code map data
 */
UINT32 pocode_getMainRecordCount(TGenericMapInfo *const mapInfo);

void pocode_unpackMain(TGenericMapInfo  *const mapInfo,
                       UINT32                  mainRowIdx,
                       TPocodeMain            *mainPocode);

void pocode_unpackSub(TGenericMapInfo  *const mapInfo,
                      const TPocodeMain      *mainPocode,
                      UINT32                  subIdx,
                      TPocodeSub             *subPocode);

void pocode_unsafeUnpackSub(TGenericMapInfo  *const mapInfo,
                            const TPocodeMain      *mainPocode,
                            UINT32                  subIdx,
                            TPocodeSub             *subPocode);

UINT32 pocode_unsafeUnpackFirstSegmentLink(TGenericMapInfo       *const mapInfo,
                                           const TPocodeMain      *mainPocode,
                                           const TPocodeSub       *subPocode,
                                           TPocodeSegmentIterator *iterator);

UINT32 pocode_unsafeUnpackNextSegmentLink(TPocodeSegmentIterator *iterator);

void pocode_unpackPocodeName(TGenericMapInfo *mapInfo,
                             UINT32           mainIdx,
                             UINT32           subIdx,
                             UTF16           *pocodeName);

void pocode_formatPocodeName(const TPocodeMapGlobals *pocodeGlobals,
                             const TPocodeMain       *mainPocode,
                             const TPocodeSub        *subPocode,
                             UTF16                   *pocodeName);

#ifdef __cplusplus
}
#endif

#endif /* ENPOCODEP_H */
