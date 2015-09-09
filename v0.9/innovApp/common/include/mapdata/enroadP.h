#ifndef ENROADP_H
#define ENROADP_H

/* This module provides a number fo API calls that allow the map data to be
   accessed in a way that is reasonably independent of the underlying
   map data format.  Typically the binary map data is unpacked into a C
   structure where it can be more easily accessed.  Objects such as road
   segments and road addresses need to be unpacked from the map in the
   right sequence to ensure that all the data they contain is valid. */

#include <containers/gebitset.h>
#include <containers/gerbtree.h>
#include <containers/gedlist.h>
#include <location/locmaplocation.h>
#include <language/lacodepage.h>
#include <language/lainit.h>
#include <language/utf16.h>
#include <mapdata/enbitstreamP.h>
#include <mapdata/enmfileP.h>
#include <mapdata/enphoneP.h>
#include <mapdata/enroad.h>
#include <mapdata/enarchiveP.h>
#include <mapdata/entokenP.h>
#include <guidance/gdcontext.h>
#include <memory/allocator.h>
#include <traffic/trlocalP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/

// the name and postal code data is spread across multiple chunks.  These
// define the maximum number of chunks used for each type of data
#define NAME_TOKEN_CHUNKS               40
#define TOKEN_CHUNKS                    40
#define PLACE_NAME_TOKEN_CHUNKS         8
#define PLACE_TOKEN_CHUNKS              8
#define PLACE_NAME_PLACE_COMBO_CHUNKS   8
#define POCODE_PLACE_COMBO_CHUNKS       4
#define POSTAL_CODE_CHUNKS              8
#define PLACE_NAME_CHUNKS               8
#define SECTOR_PLACE_PAIR_SET_CHUNKS    4
#define ROAD_ENTITY_CHUNKS              20
#define ROAD_SET_CHUNKS                 10
#define ROAD_SET_SPP_SET_CHUNKS         20
#define HOUSE_NUMBER_CHUNKS             50
#define PLACE_NAME_SET_CHUNKS           1
#define SIGNPOST_PHONETIC_CHUNKS        3
#define PLACE_NAME_PHONETIC_CHUNKS      8
#define ROAD_NAME_PHONETIC_CHUNKS       30
//@MIC Start
#define TMC_LOCATION_INDEX_CHUNKS_OLD   6
#define TMC_LOCATION_INDEX_CHUNKS       10
#define TMC_LOCATION_SEGMENTS_CHUNKS_OLD 6
#define TMC_LOCATION_SEGMENTS_CHUNKS    10
//@MIC End

// number of route shield rules
#define ROAD_SHIELD_RULES 16
// route type value used to mark invalid route shield rules
#define ROAD_SHIELD_RULE_INVALID_RT 255


/* Bit flags in global chunk of road map */
#define ROAD_GLOBAL_FLAG_LEFT_HAND_DRIVE 1
#define ROAD_GLOBAL_FLAG_IS_BASEMAP      2
#define ROAD_GLOBAL_FLAG_IS_IMPERIAL     4 /* 1 if unit of speed restr is mile/h
                                            * 0 if unit of speed rest is km/h */
#define ROAD_GLOBAL_FLAG_NO_DC_UTURNS    8 /* 1 if U-Turns illegal by default
                                            * 0 if U-Turns allowed by default */

// the maximum number of segments that can sprout from a node
#define ROAD_NODE_MAX_SEGMENTS 15

// Maximum number of intersection per node
#define ROAD_NODE_MAX_INTERSECTION 4

// The maximum number of turn restrictions per node
#define ROAD_NODE_MAX_RESTRICTIONS 31

// The maximum number of shapepoints per segment
#define ROAD_MAX_SHAPEPOINT_COUNT 15

// The maximum number of segments for a lane path
#define ROAD_MAX_LANE_PATH_SEGMENTS 10

// fudge factor because the stored segment.roadLength is computed
// differently from the shape point length calculation
#define ROAD_DISTANCE_ERROR_MARGIN 20

// The maximum number of lane paths originating from a single segment
// NOTE: increased from 10, example at lat/long 52.52599, 13.41735 (11 lane paths)
// NOTE: This has to match the value stored in the encoder. If this number is
// increased/decreased corresponding changes need to be done in the road map
// encoder to ensure our data contains only expected values.
#define ROAD_MAX_LANE_PATHS 20 //16

// The maximum number of junction view paths originating from a single segment.
// NOTE: This has to match the value stored in the encoder. If this number is
// increased/decreased corresponding changes need to be done in the road map
// encoder to ensure our data contains only expected values.
#define ROAD_MAX_JUNCTION_VIEW_PATHS 16

// The size of the fixed size rows in the lane path table
#define ROAD_LANE_PATH_ROW_SIZE 5
// The size of the fixed size rows in the lane attribute table
#define ROAD_LANE_ATTR_ROW_SIZE 6

// the maximum number of countries described by a map
#define MAX_ISO_CODES_PER_MAP 3 /* This value should be set to an odd
                                   number; else there could be
                                   alignment problems for
                                   TBinRoadGlobals. */
#define INVALID_ISO_CODE      0

// For road_calcMapBoundingBoxbyStateName to check state name in CHN special case
static const UTF16 ChinaState[31][9] = {
	{0x5B89, 0x5FBD, 0x7701, 0},
	{0x5317, 0x4EAC, 0x5E02, 0},
	{0x91CD, 0x5E86, 0x5E02, 0},
	{0x798F, 0x5EFA, 0x7701, 0},
	{0x7518, 0x8083, 0x7701, 0},
	{0x5E7F, 0x4E1C, 0x7701, 0},
	{0x5E7F, 0x897F, 0x58EE, 0x65CF, 0x81EA, 0x6CBB, 0x533A, 0},
	{0x8D35, 0x5DDE, 0x7701, 0},
	{0x6D77, 0x5357, 0x7701, 0},
	{0x6CB3, 0x5317, 0x7701, 0},
	{0x9ED1, 0x9F99, 0x6C5F, 0x7701, 0},
	{0x6CB3, 0x5357, 0x7701, 0},
	{0x6E56, 0x5317, 0x7701, 0},
	{0x6E56, 0x5357, 0x7701, 0},
	{0x6C5F, 0x82CF, 0x7701, 0},
	{0x6C5F, 0x897F, 0x7701, 0},
	{0x5409, 0x6797, 0x7701, 0},
	{0x8FBD, 0x5B81, 0x7701, 0},
	{0x5185, 0x8499, 0x53E4, 0x81EA, 0x6CBB, 0x533A, 0},
	{0x5B81, 0x590F, 0x56DE, 0x65CF, 0x81EA, 0x6CBB, 0x533A, 0},
	{0x9752, 0x6D77, 0x7701, 0},
	{0x9655, 0x897F, 0x7701, 0},
	{0x5C71, 0x4E1C, 0x7701, 0},
	{0x4E0A, 0x6D77, 0x5E02, 0},
	{0x5C71, 0x897F, 0x7701, 0},
	{0x56DB, 0x5DDD, 0x7701, 0},
	{0x5929, 0x6D25, 0x5E02, 0},
	{0x65B0, 0x7586, 0x7EF4, 0x543E, 0x5C14, 0x81EA, 0x6CBB, 0x533A, 0},
	{0x897F, 0x85CF, 0x81EA, 0x6CBB, 0x533A, 0},
	{0x4E91, 0x5357, 0x7701, 0},
	{0x6D59, 0x6C5F, 0x7701, 0}
};

/*----------------------------------------------------------------*/

/* In order to find an address result format we need to figure out what country
   the result originates from.  This gives us a country code which we can match against
   the address format data. */
typedef struct TAddressDisplayCountry {
  UINT32 countryID;
  UTF16  country[ROAD_MAX_NAME_SIZE];
} TAddressDisplayCountry;

/* Once we know the country code we can look up the applicable address display elements
   in this table. */
typedef struct TAddressDisplayFormat {
  UINT32 countryID;
  UINT8 element;
  UINT8 minPlaceRange;
  UINT8 maxPlaceRange;
  BOOL8 newlineSuffix;
  UTF16 prefix[4];
  UTF16 suffix[4];
} TAddressDisplayFormat;

/*----------------------------------------------------------------*/

/* Name type for tokens and names */
typedef enum {
  ROAD_NAME_TYPE_ROAD,
  ROAD_NAME_TYPE_PLACE
}TRoadNameType;

/* Lane Divider types */
typedef enum {
  ROAD_LANE_DIV_NONE,
  ROAD_LANE_DIV_INTER_LONG,             /* Interrupted line with long lines */
  ROAD_LANE_DIV_DOUBLE_SOLID,           /* Double solid line */
  ROAD_LANE_DIV_SINGLE_SOLID,           /* Single solid line */
  ROAD_LANE_DIV_SINGLE_SOLID_AND_INTER, /* Combination of single solid and interrupted line */
  ROAD_LANE_DIV_INTER_AND_SOLID,        /* Combination of an interrupted and solid line */
  ROAD_LANE_DIV_INTER_SHORT             /* Interrupted line with short lines */
} TRoadLaneDivType;

typedef struct TBinTokenHeader {
  UINT8   sizeHeader;
  UINT8   languageCodes[3];
  UINT32  tokPartBlobSz;

  /* The implicit token id used to represent the space */
  UINT16  implicitSepRoadTokId;
  UINT16  implicitSepPlaceTokId;

  /* The number of separator tokens in this file */
  UINT16  roadSepTokenCount;
  UINT16  placeSepTokenCount;

  /* Number of Route tokens and route names */
  UINT16  routeTokenCount;
  UINT16  routeNameCount;

  /* Offsets of the various tables */
  UINT16  offsetRoadMostFrequent;
  UINT16  offsetPlaceMostFrequent;
  UINT16  offsetTokenParts;
  UINT16  offsetExactSpIdx;
  UINT16  offsetPhonemeIdx;
  UINT16  offsetEquivSpell;
  UINT16  offsetRouteDir;
  UINT16  offsetDataBlob;

  /* Number of rows in the above tables */
  UINT16  mostFreqRoadTokenCount;
  UINT16  mostFreqPlaceTokenCount;
  UINT16  tokenPartCount;
  UINT16  equivSpellCount;
  UINT16  altRoadTokenCount;
  UINT16  altPlaceTokenCount;
  UINT16  routeDirCount;

} TBinTokenHeader;

// Road segments are separated into major, interconnect, and minor road networks
// within each sector
enum {
  ROAD_NETWORK_MAJOR = 0,
  ROAD_NETWORK_INTERCONNECT,
  ROAD_NETWORK_MINOR,
  // *** THIS MUST COME LAST
  ROAD_NETWORK_TYPES
};

// The number of separate physical road networks that can be contained
// within a coverage area.  These separate road maps are joined together
// through wormholes
enum {
  ROAD_MAP_LAYER_0,
  ROAD_MAP_LAYER_1,
  ROAD_MAP_LAYER_2,
  ROAD_MAP_LAYER_3,
  // *** THIS MUST COME LAST
  ROAD_MAP_LAYERS
};

// these correspond to the road form field in the road segment data
// structure
typedef enum {
  ROAD_FORM_SEALED = 0,
  ROAD_FORM_UNSEALED,
  ROAD_FORM_FREEWAY,
  ROAD_FORM_FERRY,
  // ** THIS MUST COME LAST
  ROAD_FORMS
} TRoadForm;

#define ROAD_FORM_DEFAULT ROAD_FORM_SEALED

/*
 * fow on top of roadForm.
 * It comes directly from the TA Form of Way
 * The slip road type has been encoded within the
 * form of way to make the encoding more compact.
 *
 * TODO: in the future, these should be
 * combined with roadForm since they
 * are partially mutually exclusive. However
 * this would require possible risky changes
 * to tool chain and map core.
 *
 * We need to make changes to routing which
 * use road format directly to calculate travel costs.
 *
 */
typedef enum {
  ROAD_FOW_UNUSED_0,              /*  0 */
  ROAD_FOW_MOTORWAY,              /*  1 */
  ROAD_FOW_MULTI_CARRIAGEWAY,     /*  2 which is not motorway */
  ROAD_FOW_SINGLE_CARRIAGEWAY,    /*  3 default */
  ROAD_FOW_ROUNDABOUT,            /*  4 */
  ROAD_FOW_TUNNEL,                /*  5 tunnel */
  ROAD_FOW_ETA_PARKING,           /*  6 */
  ROAD_FOW_ETA_GARAGE,            /*  7 building */
  ROAD_FOW_ETA_UNSTRUCTURED,      /*  8 */
  ROAD_FOW_FASTWAY,               /*  9 Fast way*/
  ROAD_FOW_SLIP_PARALLEL,         /* 10 slip type 1 parallel road */
  ROAD_FOW_SERVICE,               /* 11 */
  ROAD_FOW_ENTR_EXIT_CAR_PARK,    /* 12 */
  ROAD_FOW_BRIDGE,                /* 13 Bridge*/
  ROAD_FOW_PEDESTRIAN,            /* 14 */
  ROAD_FOW_WALKWAY,               /* 15 */
  ROAD_FOW_SLOWWAY,               /* 16 Slow way*/
  ROAD_FOW_TRAFFIC_FIGURE,        /* 17 */
  ROAD_FOW_DUMMYLINK,             /* 18 dummy link*/
  ROAD_FOW_OVERPASS,              /* 19 over pass (viaduct)*/
  ROAD_FOW_AUTHORITIES,           /* 20 */
  ROAD_FOW_SLIP_MAJOR_MINOR,      /* 21 slip type 18 major-minor */
  ROAD_FOW_SLIP_MAJOR,            /* 22 slip type 2 grade-separated*/
  ROAD_FOW_SLIP_MINOR,            /* 23 slip type 3 grade level */
  ROAD_FOW_SLIP_UNDEFINED,        /* 24 slip type unknown */
  ROAD_FOW_UNDERPASS,             /* 25 Underpass*/
  ROAD_FOW_EXPRESSWAY,            /* 26 Express way*/
  ROAD_FOW_NOT_APPLICABLE         /* Last value */
} TRoadFow;

#define ROAD_FOW_DEFAULT ROAD_FOW_SINGLE_CARRIAGEWAY
#define FOW_SIZE_DIFF (ROAD_FOW_NOT_APPLICABLE - ROAD_FOW_UNDERPASS) * 2

// these identify extended data chunks that have been added in road formats
// 1.11 and upwards
typedef enum {
  ROAD_EXTENDED_CHUNK_ADDRESS_DISPLAY_COUNTRIES = 0,
  ROAD_EXTENDED_CHUNK_ADDRESS_DISPLAY_FORMATS,

  /* Following extended chunk have been added in 1.13 on upward */
  ROAD_EXTENDED_CHUNK_SIGN_POST_STRING,
  ROAD_EXTENDED_CHUNK_SIGN_POST_STRING_INDEX,
  ROAD_EXTENDED_CHUNK_TIMEZONE,
  ROAD_EXTENDED_CHUNK_INTERSECTION,
  ROAD_EXTENDED_CHUNK_COUNTRY_STATE,

  /* The following extended chunks are used by the mini-SweePea release */
  ROAD_EXTENDED_CHUNK_NAME_TOKENS,
  ROAD_EXTENDED_CHUNK_TOKENS                  =
    ROAD_EXTENDED_CHUNK_NAME_TOKENS + NAME_TOKEN_CHUNKS,
  ROAD_EXTENDED_CHUNK_PLACE_NAME_TOKENS       =
    ROAD_EXTENDED_CHUNK_TOKENS + TOKEN_CHUNKS,
  ROAD_EXTENDED_CHUNK_PLACE_TOKENS            =
    ROAD_EXTENDED_CHUNK_PLACE_NAME_TOKENS + PLACE_NAME_TOKEN_CHUNKS,
  ROAD_EXTENDED_CHUNK_PLACE_NAME_PLACE_COMBO  =
    ROAD_EXTENDED_CHUNK_PLACE_TOKENS + PLACE_TOKEN_CHUNKS,
  ROAD_EXTENDED_CHUNK_POCODE_PLACE_COMBO      =
    ROAD_EXTENDED_CHUNK_PLACE_NAME_PLACE_COMBO + PLACE_NAME_PLACE_COMBO_CHUNKS,
  ROAD_EXTENDED_CHUNK_TOKEN_MISC              =
    ROAD_EXTENDED_CHUNK_POCODE_PLACE_COMBO + POCODE_PLACE_COMBO_CHUNKS,

  ROAD_EXTEDNED_CHUNK_TOKEN_ALT_BITSET_ROAD,
  ROAD_EXTEDNED_CHUNK_TOKEN_ALT_BITSET_PLACE,
  ROAD_EXTENDED_CHUNK_TOKEN_ALT_NAMES,

  ROAD_EXTENDED_CHUNK_PHONETIC_BLOB,
  ROAD_EXTENDED_CHUNK_SIGNPOST_PHONETICS,
  ROAD_EXTENDED_CHUNK_PLACE_NAME_PHONETICS    =
    ROAD_EXTENDED_CHUNK_SIGNPOST_PHONETICS + SIGNPOST_PHONETIC_CHUNKS,
  ROAD_EXTENDED_CHUNK_ROAD_NAME_PHONETICS     =
    ROAD_EXTENDED_CHUNK_PLACE_NAME_PHONETICS + PLACE_NAME_PHONETIC_CHUNKS,

  ROAD_EXTENDED_CHUNK_TMC_LOCATION_TABLE_INDEX =
    ROAD_EXTENDED_CHUNK_ROAD_NAME_PHONETICS + ROAD_NAME_PHONETIC_CHUNKS,
  ROAD_EXTENDED_CHUNK_TMC_LOCATION_INDEX,
  ROAD_EXTENDED_CHUNK_TMC_LOCATION_SEGMENTS   =
    ROAD_EXTENDED_CHUNK_TMC_LOCATION_INDEX + TMC_LOCATION_INDEX_CHUNKS,
 //@MIC Start
 ROAD_EXTENDED_CHUNK_TMC_LOCATION_SEGMENTS_OLD   =
    ROAD_EXTENDED_CHUNK_TMC_LOCATION_INDEX + TMC_LOCATION_INDEX_CHUNKS_OLD,
  // ** THIS MUST COME LAST
  ROAD_EXTENDED_CHUNKS_OLD                 =
    ROAD_EXTENDED_CHUNK_TMC_LOCATION_SEGMENTS_OLD + TMC_LOCATION_SEGMENTS_CHUNKS_OLD,
 //@MIC End
 ROAD_EXTENDED_CHUNKS                        =
    ROAD_EXTENDED_CHUNK_TMC_LOCATION_SEGMENTS + TMC_LOCATION_SEGMENTS_CHUNKS,
} TRoadExtendedChunks;


/*----------------------------------------------------------------*/

// raw turn restriction data as encoded in the map.  It is interpreted as
// a 16 bit pointer into the sector turn restriction data space
typedef UINT8 RAWTURN16[2];

// 24 bit unsigned numbers are used in the mapfile
typedef unsigned char UINT24[3];

/*----------------------------------------------------------------*/

// possible values for type of turn restrictions as in TRoadTurnRestriction.type
typedef enum {
  RESTRICTION_OBLIGATED = 1,
  RESTRICTION_FORBIDDEN = 2,
  RESTRICTION_PREFERRED = 3,
  RESTRICTION_UNDEFINED_BIFURCATION = 4,
  RESTRICTION_MULTILANE_BIFURCATION = 5,
  RESTRICTION_SIMPLE_BIFURCATION = 6,
  RESTRICTION_EXIT_BIFURCATION = 7
} TRoadTurnRestrictionType;

// the maximum number of segments in a turn restriction
#define TURN_RESTRICTION_MAX_SEGMENTS 15

// this structure is filled in by the function road_unpackTurnRestriction
// and it describes the nature of the restriction and an ordered list
// of segments that are involved in the restriction
typedef struct TRoadTurnRestriction {
  // 1 UINT8 OF DATA COPIED DIRECTLY FROM THE MAP
  BITFIELD32 segmentCount:4;  // number of edges in the restriction
  BITFIELD32 restrictionType:4; // type of turn restriction

  // computed
  // maximum of 15 segments in the turn restriction
  UINT32 segments[TURN_RESTRICTION_MAX_SEGMENTS];
} TRoadTurnRestriction;

/*----------------------------------------------------------------*/
// This structure is used when unpacking segment address data.
// It describes the house numbers on each side of the road.
// If the start number and end number for a side are both zero,
// then there are no house numbers for that side of the road.
typedef struct TRoadHouseNumbers {
  UINT32 leftStartNumber;
  UINT32 leftEndNumber;
  UINT32 rightStartNumber;
  UINT32 rightEndNumber;
  BOOL8 leftNumbersMixed;
  BOOL8 rightNumbersMixed;
} TRoadHouseNumbers;


/*----------------------------------------------------------------*/

// super sector name table contains a list of distinct names and places that
// are used within a super sector.  PLACE ACTUALLY INDEXES INTO THE SUPER
// SECTOR PLACE TABLE, WHILE NAME INDEXES INTO THE GLOBAL NAME TABLE.
typedef struct TRoadName {
  BITFIELD32 name:20;
  BITFIELD32 place:12;
} TRoadName;

/*
 * The following structure defines the layout of the road
 * SuperSectorHeader on media.
 */
typedef struct TBinRoadSuperSectorHeader {
  /* The fields in this structure should have a one-to-one
   * correspondence with those in TRoadSuperSectorHeader; the
   * in-memory superSectorHeader is expected to be a superset of this
   * structure.
   */

  // global start and end global map pointers of the data chunks that
  // contain the road map data for this super sector.
  UINT32 globalStartVa;
  UINT32 globalEndVa;

  TPos   origin;         /* x,y lat / long origin */
  INT32  size;           /* super sector size in both lat and long directions */
  UINT16 dataChunkIndex; /* index of the data chunk
                            (i.e. TRoadMapHeader.sectorData[index])
                            containing the super sector data
                            corresponding to this header */
  UINT8  resolution;     /* left shifts to apply to 12 bit node and
                            segment coordinates unpacked by
                            os_pos24(), allows lower data resolutions */
  UINT8  layer;          /* map layer */

  // the super sector refers to the map data contained in all sector
  // chunks between the following limits.  These values index into the
  // TRoadMapHeader.sectors[] and TRoadMapHeader.upperSectors[]
  // arrays.
  UINT16 sectorStartIndex; // index of sector header for first sector
  UINT16 sectorEndIndex;   // index of sector header for last sector
} TBinRoadSuperSectorHeader;

/* The in-memory road SuperSector Header. */
typedef struct TRoadSuperSectorHeader {
  // global start and end global map pointers of the data chunks that
  // contain the road map data for this super sector.
  UINT32 globalStartVa;
  UINT32 globalEndVa;

  TPos   origin;         /* x,y lat / long origin */
  INT32  size;           /* super sector size in both lat and long directions */
  UINT16 dataChunkIndex; /* index of the data chunk
                            (i.e. TRoadMapHeader.sectorData[index])
                            containing the super sector data
                            corresponding to this header */
  UINT8  resolution;     /* left shifts to apply to 12 bit node and
                            segment coordinates unpacked by
                            os_pos24(), allows lower data resolutions */
  UINT8  layer;          /* map layer */

  // the super sector refers to the map data contained in all sector
  // chunks between the following limits.  These values index into the
  // TRoadMapHeader.sectors[] and TRoadMapHeader.upperSectors[]
  // arrays.
  UINT16 sectorStartIndex; // index of sector header for first sector
  UINT16 sectorEndIndex;   // index of sector header for last sector
  /*
   * The fields above this comment should have a one-to-one
   * correspondence with the fields in the ondisk Road Super Sector
   * Header.
   */

  struct TGenericMapInfo *mapInfo;
} TRoadSuperSectorHeader;


// @todo use the ROAD_NODE_MAX_SEGMENTS if we can do it efficiently
#define MAX_DYNAMIC_ATTACHED_SEGS 4

struct TRoadSectorHeader;     /* forward declaration */
typedef struct TDynamicRoadSectorBuff {
  TDListObject              lruDlink;        // For recycling.
  struct TRoadSectorHeader *sectorHeader;    // back pointer to the sector header

  UINT16 *nodeOffsetTable; // table of offsets to nodes
  UINT16 *nodeExtendedInfo;
} TDynamicRoadSectorBuff;

/*----------------------------------------------------------------*/
// We sort the nodes into subsectors.
// The following two defines describe the number of bits
// to describe the number of subsectors in the X and Y directions.
// So, for example if there are 2 bits in the X direction and
// 1 in the y direction we can describe 4 * 2 = 8 subsectors.
// E.g.,
//
// 0 | 1 | 2 | 3
// --+---+---+--
// 4 | 5 | 6 | 7
//
// For now we have four subsectors
//
// 0 | 1
// --+---
// 2 | 3
//
#define BIN_SUBSECTOR_BITS_X   1
#define BIN_SUBSECTOR_BITS_Y   1
#define BIN_SUBSECTOR_COUNT ((1 << BIN_SUBSECTOR_BITS_X) * (1 << BIN_SUBSECTOR_BITS_Y))

// Number of bits used to represent an x or y coordinate within a sector
#define BIN_SECTOR_NODE_POS_WIDTH  12
#define BIN_SECTOR_NODE_POS_RANGE  (1 << BIN_SECTOR_NODE_POS_WIDTH)

// TRoadSectorDataHeader is kept separate from the TBinRoadSector
// to keep the size of the data strutures in svm small.
// If all loaded sectors were road sectors then
// it would not make any difference whether we split
// the header data into two because we have to load
// both of them when we load the sector.
// However, because the svm data structures are a union
// of header types, we waste svm space when we load
// data sectors of different types.

/*
 * TRoadSectorDataHeader is encoded to contain 32-bit offsets to
 * metadata tables within the sector relative to the start of the
 * chunk. On a 32-bit platform, these offsets are conveniently
 * translated into memory addresses after loading a sector's chunk
 * (through roadDynamicLinkSector()); on a 64-bit platform, however,
 * since pointers are 64-bits wide, such a translation is not directly
 * possible. On a 64-bit platform, therefore, for a valid
 * interpretation and use of the offsets in the sector data header,
 * we've made available the following accessor macros which take over
 * the task of translation everytime the metadata tables are
 * accessed. To allow for this magic, TRoadSectorDataHeader is defined
 * appropriately based on the platform's word size.
 */

#if !defined __x86_64__
/* definitions for a 32 bit machine */
typedef struct TRoadSectorDataHeader {
  UINT8 *_nodeIndirect;    /* a lookup table that converts 16 bit node
                              references to 32 bit references */
  UINT8 *_segmentIndirect; /* a lookup table that converts 16 bit
                              segment references to 32 bit
                              references */
  UINT8 *_turnRestrictions;/* a data area that contains the structure
                              of all turn restrictions in this
                              sector */
  UINT8 *_laneInformation; /* a data area that contains the lane
                              information in this sector */
  UINT16 nodeSubsectorEnd[BIN_SUBSECTOR_COUNT]; /* end offsets (from
                                                   the nodes field of
                                                   the
                                                   TRoadSectorHeader)
                                                   of each
                                                   subsector */
} TRoadSectorDataHeader;
#define ROAD_SECTOR_DATA_HEADER_NODE_INDIRECT(sectorHeader)     \
  ((sectorHeader)->sectorDataHeader->_nodeIndirect)
#define ROAD_SECTOR_DATA_HEADER_SEGMENT_INDIRECT(sectorHeader)  \
  ((sectorHeader)->sectorDataHeader->_segmentIndirect)
#define ROAD_SECTOR_DATA_HEADER_TURN_RESTRICTIONS(sectorHeader) \
  ((sectorHeader)->sectorDataHeader->_turnRestrictions)
#define ROAD_SECTOR_DATA_HEADER_LANE_INFORMATION(sectorHeader)  \
  ((sectorHeader)->sectorDataHeader->_laneInformation)

#else //!defined __x86_64__
/* definitions for a 64 bit machine */
typedef struct TRoadSectorDataHeader {
  UINT32 nodeIndirectOffset;     /* a lookup table that converts 16
                                    bit node references to 32 bit
                                    references */
  UINT32 segmentIndirectOffset;  /* a lookup table that converts 16
                                    bit segment references to 32 bit
                                    references */
  UINT32 turnRestrictionsOffset; /* a data area that contains the
                                    structure of all turn restrictions
                                    in this sector */
  UINT32 laneInformationOffset;  /* a data area that contains the lane
                                    information in this sector */
  UINT16 nodeSubsectorEnd[BIN_SUBSECTOR_COUNT]; /* end offsets (from
                                                   the nodes field of
                                                   the
                                                   TRoadSectorHeader)
                                                   of each
                                                   subsector */
} TRoadSectorDataHeader;
#define ROAD_SECTOR_DATA_HEADER_NODE_INDIRECT(sectorHeader)             \
  (UINT8 *)(((sectorHeader)->sectorDataHeader->nodeIndirectOffset) +    \
            ((INTP)(sectorHeader)->sectorDataHeader -                   \
             ((INT32)(sectorHeader)->globalStartVa -                    \
              sectorHeader->mapInfo->mapStartVa)))
#define ROAD_SECTOR_DATA_HEADER_SEGMENT_INDIRECT(sectorHeader)          \
  (UINT8 *)(((sectorHeader)->sectorDataHeader->segmentIndirectOffset) + \
            ((INTP)(sectorHeader)->sectorDataHeader -                   \
             ((INT32)(sectorHeader)->globalStartVa -                    \
              sectorHeader->mapInfo->mapStartVa)))
#define ROAD_SECTOR_DATA_HEADER_TURN_RESTRICTIONS(sectorHeader)         \
  (UINT8 *)(((sectorHeader)->sectorDataHeader->turnRestrictionsOffset) + \
            ((INTP)(sectorHeader)->sectorDataHeader -                   \
             ((INT32)(sectorHeader)->globalStartVa -                    \
              sectorHeader->mapInfo->mapStartVa)))
#define ROAD_SECTOR_DATA_HEADER_LANE_INFORMATION(sectorHeader)          \
  (UINT8 *)(((sectorHeader)->sectorDataHeader->laneInformationOffset) + \
            ((INTP)(sectorHeader)->sectorDataHeader -                   \
             ((INT32)(sectorHeader)->globalStartVa -                    \
              sectorHeader->mapInfo->mapStartVa)))
#endif /* #if !defined __X86_64__ */

/*
 * The following structure defines the layout of the road SectorHeader
 * on media.
 */
typedef struct TBinRoadSectorHeader {
  /* The fields in this structure should have a one-to-one
   * correspondence with those in TRoadSectorHeader; the in-memory
   * superSectorHeader is expected to be a superset of this
   * structure. */

  // global start and end global map pointers of the data chunk that
  // contains the road map data for this sector.
  UINT32 globalStartVa;
  UINT32 globalEndVa;

  TPos   origin; /* position of the middle of the sector */
  UINT16 halfSizeX, halfSizeY; /* half size in X and Y of bounding box
                                * | centered around sector origin (need to
                                * | be left-shifted by resolution).  This
                                * | bounding box is guaranteed to contain
                                * | all segments (nodes & shape points)
                                * | in this sector */
  BITFIELD32   dataChunkIndex : 16;
  BITFIELD32   resolution     :  4;
  BITFIELD32   minRoadType    :  4; /* map display can use this to avoid */
                                    /* | loading sector data if sector */
                                    /* | only contain minor roads */
                                    /* | at high zoom level (filled */
                                    /* | with 0 in map version <= 111) */
  BITFIELD32   layer          :  2;
  BITFIELD32   skipSecondPass :  3; /* not in map data, used by map core */
  BITFIELD32   spare          :  3;

  UINT32 superSectorHeaderOffset; /* virtual address of the super sector
                                   * header containing this sector */
  UINT16 numDynamicNodes;
  UINT16 padding;

  UINT32 nodes;    /* offset within the chunk data for nodes */
  UINT32 segments; /* offset within the chunk data for segments */
  UINT16 segmentLayerEnd[ROAD_TYPES];
} TBinRoadSectorHeader;

/* The in-memory road SectorHeader. */
typedef struct TRoadSectorHeader {
  // global start and end global map pointers of the data chunk that
  // contains the road map data for this sector.
  UINT32 globalStartVa;
  UINT32 globalEndVa;

  TPos   origin; /* position of the middle of the sector */
  UINT16 halfSizeX, halfSizeY; /* half size in X and Y of bounding box
                                * | centered around sector origin (need to
                                * | be left-shifted by resolution).  This
                                * | bounding box is guaranteed to contain
                                * | all segments (nodes & shape points)
                                * | in this sector */
  BITFIELD32   dataChunkIndex : 16;
  BITFIELD32   resolution     :  4;
  BITFIELD32   minRoadType    :  4; /* map display can use this to avoid */
                                    /* | loading sector data if sector */
                                    /* | only contain minor roads */
                                    /* | at high zoom level (filled */
                                    /* | with 0 in map version <= 111) */
  BITFIELD32   layer          :  2;
  BITFIELD32   skipSecondPass :  3; /* not in map data, used by map core */
  BITFIELD32   spare          :  3;

  UINT32 superSectorHeaderVa; /* virtual address of the super sector
                               * header containing this sector */
  UINT16 numDynamicNodes;
  UINT16 padding;

  UINT32 nodes;    /* offset within the chunk data for nodes */
  UINT32 segments; /* offset within the chunk data for segments */
  UINT16 segmentLayerEnd[ROAD_TYPES];
  /*
   * The fields above this comment have a one-to-one correspondence
   * with the fields of the structure TBinRoadSectorHeader.
   */

  struct TGenericMapInfo *mapInfo;
  TDynamicRoadSectorBuff *dynamicBuff;
  TRoadSectorDataHeader  *sectorDataHeader;
  UINT8                  *globalAddrTrans; /* converts global map
                                            * pointer between
                                            * globalStart and
                                            * globalEnd to memory
                                            * address of data */
} TRoadSectorHeader;

/*----------------------------------------------------------------*/
// This is a helper structure used to unpack the first
// few bytes of a TRoadNode. It corresponds to the mapdata format
// NOTE: The flags are assumed to come directly after the
// node position, and the node position is assumed to be 3 bytes.
// Furthermore,flagSegsAttached is assumed to be in the first byte
// of flags.
// See
// -  unsafeUnpackNode()
// -  road_unsafeUnpackNodePosition()
// -  nodeCheckFlagSegsAttached()
typedef struct TRoadNodeTemplate {
  // There are three bytes of position and flag data
  // in the binary data then this info

  // This byte is present if flagSegsAttached
  BITFIELD32 segmentCount         :4; /* number of segment attached to this node */
  BITFIELD32 flagWormhole         :1;
  BITFIELD32 flagConcatenated     :1;
  BITFIELD32 flagSpare            :2;

  // byte present in mapdata if flagRare set
  BITFIELD32 flagRare2                 :1;
  BITFIELD32 flagIntersection          :1;
  BITFIELD32 flagRoundabout            :1;
  BITFIELD32 flagTmc                   :1;
  BITFIELD32 turnRestrictionCountLSB   :2;
  BITFIELD32 flagGlue                  :1;
  BITFIELD32 flagSignPosts             :1;

  // byte present in mapdata if flagRare2 set
  BITFIELD32 flagTrafficLights         :1;
  BITFIELD32 flagTurnRestriction       :1;
  BITFIELD32 turnRestrictionCountMSB   :3;
  BITFIELD32 spare                     :3;
} TRoadNodeTemplate;

// the road_unpackNode routine unpacks node data from the map file into this
// structure
typedef struct TRoadNode {
  /* COMPUTED FIELDS */
  // This field is magically computed from a combination of bytes
  // and the fields segmentCountLSB/MSB and maneuverCountLSB/MSB
  BITFIELD32 segmentCount         :4; /* number of connected segments */
  BITFIELD32 spare0               :4;

  /* This byte is taken directly from the rare byte. Do not reorder */
  BITFIELD32 flagRare2            :1; /* Node has additional byte very rare. Not needed */
  BITFIELD32 flagIntersection     :1; /* Node has 1..n intersection IDs */
  BITFIELD32 flagRoundabout       :1; /* this node is actually a roundabout */
  BITFIELD32 flagTmc              :1; /* node part of TMC location */
  BITFIELD32 spareManeuverLSB     :2; /* not needed */
  BITFIELD32 flagGlue             :1; /* this node is a glue node (affects the size of the record) */
  BITFIELD32 flagSignPosts        :1; /* 1 or more sign posts attached to this node */

  // This field is templated by the rare2 byte. Do not
  // reorder flagTrafficLights or flagTurnRestriction.
  // flagTurnRestriction and the other fields are also altered by other fields.
  BITFIELD32 flagTrafficLights    :1; /* there are traffic lights at this node */
  BITFIELD32 flagTurnRestriction  :1; /* this node is involved in a turn restriction */
  BITFIELD32 flagConcatenated     :1; /* last attached segment not stored explicitly */
  BITFIELD32 flagWormhole         :1; /* this node contains a link to the next level up */
  BITFIELD32 flagSegsAttached     :1;
  BITFIELD32 flagRare             :1;
  BITFIELD32 spare                :2;

  BITFIELD32 turnRestrictionCount :5; /* number of attached turn restrictions */
  BITFIELD32 spare3               :3;

  TPos   pos;                              /* position in map coordinates */
  UINT32 segments[ROAD_NODE_MAX_SEGMENTS]; /* attached segments IDs */
  UINT32 wormholeNode;                     /* points to node in upper level */

  UINT32 sectorHeaderVa;                   /* sector in which this
                                              node was found */
  UINT8  size;                              /* packed record size in bytes */

  /*
   * Several nodes may share the same intersection ID when
   * they are part of the same intersection.  For example,
   * when dual carriage road cross a single carriage road,
   * 2 nodes share the same intersection ID
   *
   *                   |
   *                   |
   *    -------<-------o-------<--------
   *    ------->-------o------->--------
   *                   |
   *                   |
   *
   * A node can belong to multiple intersections.
   */
  UINT8  intersectionCount;  /* node can belong to 0..n intersections */
  UINT32 intersectionIds[ROAD_NODE_MAX_INTERSECTION];

  UINT64 glueNodeID; /* global node ID for this node */
  UINT32 ID;

  UINT32 firstSignPostOffset; /* if (map offset) of first sign post */

  // **** THESE FIELDS ARE NOT CACHE SAFE
  // the sector containing the node in question should be first
  // identified using road_findSector and then locked in place
  // using roadfile_lockSector prior to calling road_unpackNode,
  // then these fields can be read before the sector is unlocked
  //
  // Unsafe fields are suffixed with _u to help remind users of
  // these fields about potential caveat

  /* points directly into sector content data in memory */
  RAWTURN16 *turnRestrictions_u;
} TRoadNode;


/*
* Get the turn restriction field of a node and check that
* turnRestrictions_u field points to a locked memory region
* in map cache (if there are turn restrictions)
*/
#ifdef DEBUG_MAPCACHE_CHECK_LOCK_PTR
#define ROAD_NODE_GET_TURN_RESTRICTIONS(nodePtr)                  \
  (assert(!(nodePtr)->turnRestrictionCount ||                     \
  mapcache_isLockedPtr((nodePtr)->turnRestrictions_u)),   \
  (nodePtr)->turnRestrictions_u)
#else
#define ROAD_NODE_GET_TURN_RESTRICTIONS(nodePtr) ((nodePtr)->turnRestrictions_u)
#endif

/*----------------------------------------------------------------*/

// the road_unpackSegment routine unpacks segments into this structure.
// Segments must be unpacked in order starting at the first segment in each
// layer to extract all information correctly
struct TRoadSegment {
  /*
  * Segment size in binary file only depends on the content of
  * the first 2 bytes attributes.  So computing segment size
  * can be tabulated
  */

  // This byte is always present
  BITFIELD32 flagForwardsRestriction  :1; // can't travel from first to last node
  BITFIELD32 flagBackwardsRestriction :1; // can't travel from last to first node
  BITFIELD32 flagUrbanRoad            :1; // road is in a built up area
  BITFIELD32 flagCurved               :1; // draw curves at intermediate zoom levels
  BITFIELD32 flagConcatenated         :1; // last node = first node of next
  BITFIELD32 flagLastNode             :1; // segment record explicitly contains last node data
  BITFIELD32 flagAddressData          :1; // the segment has house number data
  BITFIELD32 flagRare1                :1; // byte containing rare flags is present

  // This byte is always present
  BITFIELD32 flagRoadSetChange    :1; // road set has changed
  BITFIELD32 flagPlacePairChange  :1; // place pair has changed
  BITFIELD32 flagFOWEncoded       :1; // non-default road form and form of way.
  BITFIELD32 flagCompactShape     :1; // shape points in 8+8 bits rather than 12+12 bits
  BITFIELD32 shapePointCountLSB   :3; // number of shape points
  BITFIELD32 flagLongRoad         :1;

  // This byte is always present when layer < 2
  // The avgSpeedIdx links to the dictionary averageSpeed in
  // TRoadGlobalData so must have have enough bits to describe
  // BIN_ROAD_GLOBAL_NUM_AVG_SPEEDS + 1 values.
  BITFIELD32 roadLenLSB            :4;
  BITFIELD32 avgSpeedIdx           :4; // index into average speed table

  // This byte is only present if flagFOWEncoded is set, otherwise
  // the values are set to defaults.
  //
  // @todo Tidy up roadForm. Is there any reason for ferry to be part
  // of roadForm?
  // @todo Reduce number of bits used by roadFow. Some fow are
  // extremely rare, and there are also unused values.
  BITFIELD32 roadForm  :2; // unsealed, sealed, freeway, ferry (TRoadForm)
  BITFIELD32 roadFow   :5; // the road form of way (TRoadFow)
  BITFIELD32 flagNotPublic :1; // The road is "No Public Access"

  //This byte is present if flagLongRoad is set
  BITFIELD32 roadLengthFieldWidth  :1; // road length is 1 or 2 bytes
  BITFIELD32 roadLenMSB            :4;
  BITFIELD32 shapePointCountMSB    :1;
  BITFIELD32 spare_longRoad        :2;

  // This byte is only present if flagRareFlags is set (all those
  // flags are typically rarely populated)
  BITFIELD32 flagTollRoad         :1; // toll road
  BITFIELD32 flagLaneInfo         :1; // this segment has additional lane information
  BITFIELD32 flagSpdR             :1; // speed restriction not default
  BITFIELD32 flagTmc              :1; // segment involved in TMC location
  BITFIELD32 flagResident         :1; // No Through Traffic. i.e Not for routing.
  BITFIELD32 flagPluralJ          :1; // plural junction
  BITFIELD32 flagDivided          :1; // segment is part of dualcarriageway
                                      // with physical separation
  BITFIELD32 flagRare2            :1;

  /* Second rare bytes, present in map data only if flagRare2 is set */
  BITFIELD32 flagClosedAtStart    :1; // segment closed at start
  BITFIELD32 flagClosedAtEnd      :1; // segment closed at end
  BITFIELD32 flagEnExLn           :1;
  BITFIELD32 flagTzChange         :1; // time zone change (extremely rare)
  BITFIELD32 laneCount            :4; // lane count (0 if not populated)

  // This byte is computed from the average speed dictionary
  // but if the average speed dictionary is full
  // (i.e., if the value in avgSpeedIdx >= BIN_ROAD_GLOBAL_NUM_AVG_SPEEDS)
  // then this byte is present in the binary format as the raw data.
  UINT8 averageSpeed; // in kph

  // Forward and backward speed restrictions (limits)
  // They are usually computed from defaults.
  UINT8 fwdSpdR;
  UINT8 bckSpdR;

  /* Time zone ID (index in time zone table) */
  UINT8 tzId;

  /* COMPUTED FIELDS */
  UINT32 firstNode;       // offset to first node
  UINT16 roadLength;      // road length up to 65km
  UINT32 lastNode;        // offset to last node, or NULL
  UINT32 roadSetIndex;    // points to the road set record
  UINT32 placePairIndex;  // points to the place pair record

  UINT32 sectorHeaderVa;  // sector in which the segment was found
  UINT8  roadType;        // frc (computed from sort order)
  UINT8  shapePointCount; // from shapePointCountMSB/shapePointCountLSB

  /*
  * Vertices are shape points + nodes.  Segments have
  * at least a first node and <shapePointCount> shape points
  * Last node is optional.
  * So vertexCount == shapePointCount + 1  (when no last node)
  *    vertexCount == shapePointCount + 2  (when there is a last node)
  */
  UINT8 vertexCount;
  UINT8 size; // record size
  UINT32 ID;  // (unique) global ID of the segment

  // **** THESE FIELDS ARE NOT CACHE SAFE
  // the sector containing the segment in question should be first
  // identified using road_findSector and then locked in place
  // using roadfile_lockSector prior to calling road_unpackSegment,
  // then these fields can be read before the sector is unlocked
  //
  // Unsafe fields are suffixed with _u to help remind users of
  // these fields about potential caveat

  void *shapePoints_u; // up to 16 packed shape point positions which are packed
  // either 3 bytes per (x, y) or 2 bytes per (x, y)

  // Unpacked shape points including the Start and End node of this segment.
  // These are not unpacked by default and are only unpacked when
  // invoking road_unpackVertices() or by unpacking first and last node and
  // shape points explicitly in this array.
  TPos vertices[ROAD_MAX_SHAPEPOINT_COUNT+2];

  // this boolean is used for keeping track of the shapePoints array
  // it is set to false during init and it set to true once
  // shapePoints have been unpacked. Therefore subsequent calls to
  // road_unpackShapePoints or road_unsafeUnpackShapePoints will not
  // result in recomputation
  BOOL8 shapePointsUnpacked;
};

  //#define ROAD_SEGMENT_ROUNDABOUT(segmentPtr) ((BOOL8)((segmentPtr)->flagRoundabout))
  //#define ROAD_SEGMENT_SLIPROAD(segmentPtr) ((BOOL8)((segmentPtr)->flagSlipRoad))

#define ROAD_SEGMENT_ROUNDABOUT(segmentPtr) ( (BOOL8) \
((segmentPtr)->roadFow == ROAD_FOW_ROUNDABOUT \
|| (segmentPtr)->roadFow == ROAD_FOW_TRAFFIC_FIGURE \
))

#define ROAD_SEGMENT_SLIPROAD(segmentPtr) ( (BOOL8) \
((segmentPtr)->roadFow == ROAD_FOW_SLIP_PARALLEL \
|| (segmentPtr)->roadFow == ROAD_FOW_SLIP_MAJOR_MINOR \
|| (segmentPtr)->roadFow == ROAD_FOW_SLIP_MAJOR \
|| (segmentPtr)->roadFow == ROAD_FOW_SLIP_MINOR \
|| (segmentPtr)->roadFow == ROAD_FOW_SLIP_UNDEFINED \
))

/*
 * Get the shape points field of a node and check that
 * shapePoint_u field points to a locked memory region
 * in map cache (if there are shapePoints)
 */
#ifdef DEBUG_MAPCACHE_CHECK_LOCK_PTR
#define ROAD_SEGMENT_GET_SHAPEPOINTS(segmentPtr)               \
  (assert(!(segmentPtr)->shapePointCount ||                    \
          mapcache_isLockedPtr((segmentPtr)->shapePoints_u)),  \
   (segmentPtr)->shapePoints_u)
#else
#define ROAD_SEGMENT_GET_SHAPEPOINTS(segmentPtr) ((segmentPtr)->shapePoints_u)
#endif

#ifdef DEBUG_MAPCACHE_CHECK_LOCK_PTR
#define ROAD_SEGMENT_GET_ADDRESSES(segmentPtr)               \
  (assert(!(segmentPtr)->flagAddressData ||                  \
          (segmentPtr->leftAddressStartFieldWidth == 0 &&    \
           segmentPtr->rightAddressStartFieldWidth == 0) ||  \
          mapcache_isLockedPtr((segmentPtr)->addresses_u)),  \
   (segmentPtr)->addresses_u)
#else
#define ROAD_SEGMENT_GET_ADDRESSES(segmentPtr) ((segmentPtr)->addresses_u)
#endif


/*----------------------------------------------------------------*/

#define ROAD_SHIELD_MAX_PREFIX_LENGTH 8
#define ROAD_SHIELD_MAX_ICON_LENGTH 7

// This must be identical to TRGBA8 in the tool chain.
typedef struct TRoadRGBA {
  UINT8 r;
  UINT8 g;
  UINT8 b;
  UINT8 a;
} TRoadRGBA;

// This must be identical to TBinRoadColours in the tool chain.
typedef struct TRoadShieldRule {
  BITFIELD32 minRouteType            :8;
  BITFIELD32 maxRouteType            :8;

  BITFIELD32 flagStripPrefixDisplay  :1;
  BITFIELD32 flagStripPrefixGuidance :1;
  BITFIELD32 flagsSpare1             :6;

  BITFIELD32 flagsSpare2             :8;

  UTF16     prefix[ROAD_SHIELD_MAX_PREFIX_LENGTH]; // not necessarily
                                                   // null-terminated
  TRoadRGBA text;
  TRoadRGBA background;
  TRoadRGBA border;
  char      icon[ROAD_SHIELD_MAX_ICON_LENGTH + 1]; // always
                                                   // null-terminated
} TRoadShieldRule;

// Store the most common average speeds in a dictionary.
#define BIN_ROAD_GLOBAL_NUM_AVG_SPEEDS 15

typedef struct TBinRoadGlobals {
  /* global map data - bounds, etc */
  TRect        bounds;   /* the map area is within these bounds */
  UINT8        flags;    /* left/right hand drive? basemap? km/h or m/h? */
  UINT8        spare;

  UINT16       ISOcodes[MAX_ISO_CODES_PER_MAP]; /* for the countries
                                                   described by this map */

  UINT32       sectorHeadersCount     [ROAD_MAP_LAYERS];
  UINT32       sectorHeadersFptr      [ROAD_MAP_LAYERS];
  UINT32       superSectorHeadersCount[ROAD_MAP_LAYERS];
  UINT32       superSectorHeadersFptr [ROAD_MAP_LAYERS];

  UINT32       extChunkStart; /* index of the start of the extended chunks */
  TRoadShieldRule routeShieldRules[ROAD_SHIELD_RULES];
  TRoadSpeed   averageSpeed[BIN_ROAD_GLOBAL_NUM_AVG_SPEEDS];
  // A dictionary of road speed defaults against fow and the urban flag
  TRoadSpeed   spdRDefault[ROAD_FOW_NOT_APPLICABLE + 1][2];

  UINT8        numJunctionViewComponents:3;
  UINT8        archiveIndexWidth:2;   /*  width 0  --> 8  bit archive indices
                                       *        1  --> 12 bit archive indices
                                       *        2  --> 16 bit archive indices */
  UINT16       countOfJunctionViewImageTypes[MAX_JUNCTION_IMAGE_TYPES];
  UINT32       crcArchiveMap; /* 32bit CRC of the matching archive map */
} TBinRoadGlobals;

/* An instance of the following structure hangs off every MapInfo
 * structure corresponding to a road map. */
typedef struct TRoadMapGlobals {
  /* global map data - bounds, etc */
  TRect        bounds;   /* the map area is within these bounds */
  UINT8        flags;    /* left/right hand drive? basemap? km/h or m/h? */
  UINT8        spare;

  UINT16       ISOcodes[MAX_ISO_CODES_PER_MAP]; /* for the countries
                                                   described by this map */

  UINT32       sectorHeadersCount     [ROAD_MAP_LAYERS];
  UINT32       sectorHeadersFptr      [ROAD_MAP_LAYERS];
  UINT32       superSectorHeadersCount[ROAD_MAP_LAYERS];
  UINT32       superSectorHeadersFptr [ROAD_MAP_LAYERS];

  UINT32       extChunkStart; /* Index of the start of the extended chunks */
  TRoadShieldRule routeShieldRules[ROAD_SHIELD_RULES];
  TRoadSpeed   averageSpeed[BIN_ROAD_GLOBAL_NUM_AVG_SPEEDS];
  // A dictionary of road speed defaults against fow and the urban flag
  TRoadSpeed   spdRDefault[ROAD_FOW_NOT_APPLICABLE + 1][2];

  UINT8        numJunctionViewComponents:3;
  UINT8        archiveIndexWidth:2;   /*  width 0  --> 8  bit archive indices
                                       *        1  --> 12 bit archive indices
                                       *        2  --> 16 bit archive indices */
  UINT16       countOfJunctionViewImageTypes[MAX_JUNCTION_IMAGE_TYPES];
  UINT32       crcArchiveMap; /* 32bit CRC of the matching archive map */

  /* the following are initialized at map-load time */
  const TArchiveInfo *archiveInfo;
  UINT8        junctionViewIndexWidths[MAX_JUNCTION_IMAGE_TYPES]; /* in bytes */
  UINT8        junctionViewRowSize;                               /* in bytes */
} TRoadMapGlobals;

/* glue node chunk definition - gives the local file position of each glue node */
typedef struct TRoadGlueNode {
  UINT64 id;
  UINT32 fptr;
  UINT32 spare;
} TRoadGlueNode;

/* Constants to access columns in place table */
#define PLACE_TYPE_A0 6
#define PLACE_TYPE_A1 5
#define PLACE_TYPE_A2 4
#define PLACE_TYPE_A7 3
#define PLACE_TYPE_A8 2
#define PLACE_TYPE_A9 1

/* Header before packed places */
typedef struct TRoadPlaceHeader {
  UINT8  bytesPerPlace;  /* number of bytes per packed place */
  UINT8  mask;           /* indicates which columns have default values */

  /* Default value for place columns */
  UINT16 default_a0, default_a1, default_a2,
         default_a7, default_a8, default_a9,
         default_bua, default_pocode;
} TRoadPlaceHeader;

/* table of distinct road names and  */
typedef STRUCT_PACKED TRoadNamePlaceCombo {
  UINT24 name;
  UINT16 place;
} TRoadNamePlaceCombo;

/* NEW mini-SweePea Structure */
typedef struct TRoadPlaceNamePlace{
  UINT32  nameId;
  UINT32  placeId;
  UINT8   placeColumn;
  BOOL8   hasNoRoads;
  BOOL8   hasBoundingBox;
  TRect   boundingBox;
} TRoadPlaceNamePlace;

typedef struct TRoadPocodePlace{
  UINT32  nameId;
  UINT32  placeId;
  UINT8   placeColumn;
  BOOL8   hasNoRoads;
  BOOL8   hasBoundingBox;
  TRect   boundingBox;
  UINT32  adornmentPlaceName;
} TRoadPocodePlace;

/* A MapHeader is an interpretation of the chunk directory table which
 * makes available symbolic names to refer to the first few
 * chunks. Macros such as <maptype>_CHUNK_ADDRESS allow us to
 * translate these names in the mapHeader into chunk-directory
 * indices. */
typedef struct TRoadMapHeader {
  /* these chunks at the start of the chunk directory */
  TChunkFptr glueNodes;
  TChunkFptr postalCodes[POSTAL_CODE_CHUNKS];
  TChunkFptr postalCodeIndex[POSTAL_CODE_CHUNKS];
  TChunkFptr places;
  TChunkFptr placePair;
  TChunkFptr sectorPlacePair;
  TChunkFptr sectorPlacePairSet[SECTOR_PLACE_PAIR_SET_CHUNKS];
  TChunkFptr roadEntity[ROAD_ENTITY_CHUNKS];
  TChunkFptr glueRoadBounds;
  TChunkFptr roadSetHeader;
  TChunkFptr roadSet[ROAD_SET_CHUNKS];
  TChunkFptr roadSetSPPSet[ROAD_SET_SPP_SET_CHUNKS];
  TChunkFptr roadSetIntersection;
  TChunkFptr houseNumberRange;
  TChunkFptr houseNumber[HOUSE_NUMBER_CHUNKS];
  TChunkFptr placeNameSetHeader;
  TChunkFptr placeNameSet[PLACE_NAME_SET_CHUNKS];
  TChunkFptr junctionImageSet;

  /* an index of other chunks containing sector data */
  TChunkFptr sectorData[1]; /* actually can be more than 1 sector chunk */
  /* .... */
  /* Extended chunks can be present after all sector/super sector data chunks */
} TRoadMapHeader;

#define ROAD_GLOBALS(mapInfo)                           \
  (assert((mapInfo)->mapType == MAPTYPE_ROAD),          \
   ((TRoadMapGlobals *)((mapInfo)->_globals)))

#define ROAD_CHUNK_INDEX(chunkName)                             \
  ((offsetof(TRoadMapHeader, chunkName)) / sizeof(TChunkFptr))

#define ROAD_CHUNK_SIZE_MEM(mapInfo, chunkName)                 \
  CHUNK_SIZE_MEM((mapInfo), ROAD_CHUNK_INDEX(chunkName))
#define ROAD_CHUNK_IS_EMPTY(mapInfo, chunkName)                 \
  CHUNK_IS_EMPTY((mapInfo), ROAD_CHUNK_INDEX(chunkName))

#define COUNTOF_ROAD_SECTOR_HEADERS(mapInfo, layer)     \
  (assert(layer < ROAD_MAP_LAYERS),                     \
   ROAD_GLOBALS(mapInfo)->sectorHeadersCount[layer])
#define COUNTOF_ROAD_SUPER_SECTOR_HEADERS(mapInfo, layer)       \
  (assert(layer < ROAD_MAP_LAYERS),                             \
   ROAD_GLOBALS(mapInfo)->superSectorHeadersCount[layer])
#define FPTROF_ROAD_SECTOR_HEADERS(mapInfo, layer)      \
  (assert(layer < ROAD_MAP_LAYERS),                     \
   ROAD_GLOBALS(mapInfo)->sectorHeadersFptr[layer])
#define FPTROF_ROAD_SUPER_SECTOR_HEADERS(mapInfo, layer)        \
  (assert(layer < ROAD_MAP_LAYERS),                             \
   ROAD_GLOBALS(mapInfo)->superSectorHeadersFptr[layer])

#define ROAD_SECTOR_HEADER_VA(mapInfo, layer, index) (UINT32)           \
  (assert((index) < COUNTOF_ROAD_SECTOR_HEADERS(mapInfo, (layer))),     \
   mapInfo->mapStartVa +                                                \
   FPTROF_ROAD_SECTOR_HEADERS(mapInfo, (layer)) +                       \
   ((index) * sizeof(TBinRoadSectorHeader)))
#define ROAD_SECTOR_HEADER(mapInfo, layer, index) ((TRoadSectorHeader *) \
  SVM_RESOLVE_ROAD_SECTOR_HEADER(ROAD_SECTOR_HEADER_VA(mapInfo, layer, index)))
#define ROAD_SUPER_SECTOR_HEADER_VA(mapInfo, layer, index) (UINT32)     \
  (assert((index)<COUNTOF_ROAD_SUPER_SECTOR_HEADERS(mapInfo, (layer))), \
   mapInfo->mapStartVa +                                                \
   FPTROF_ROAD_SUPER_SECTOR_HEADERS(mapInfo, (layer)) +                 \
   ((index) * sizeof(TBinRoadSuperSectorHeader)))
#define ROAD_SUPER_SECTOR_HEADER(mapInfo, layer, index)         \
  ((TRoadSuperSectorHeader *)                                   \
    SVM_RESOLVE_ROAD_SUPER_SECTOR_HEADER(                       \
      ROAD_SUPER_SECTOR_HEADER_VA(mapInfo, layer, index)))

typedef struct TRoadPlaceNamePlaceIterator{
  UINT32           nameId;
  TGenericMapInfo *mapInfo;
  UINT32           chunkIdx;
  UINT32           lastPlaceId;
  UINT32           rowOffset;
  BOOL8            complete;
} TRoadPlaceNamePlaceIterator;

// this structure is filled with the details of a lane path by
// the function road_unpacklanepath
typedef struct TRoadLanePath {
  BITFIELD32  segmentCount :4;
  BITFIELD32  fromLane     :4;
  BITFIELD32  toLane       :4;
  TSegmentID  segments[ROAD_MAX_LANE_PATH_SEGMENTS];
} TRoadLanePath;

/* This structure is used internally to describe junctionView paths
 * unpacked from road-maps. */
typedef struct TJunctionViewPath {
  UINT16     roadMapIndex;
  UINT16     segmentCount        :4;
  UINT16     numImageSetIndices  :4;
  UINT16     spare               :8;
  TSegmentID segments[ROAD_MAX_JUNCTION_VIEW_SEGMENTS];
  UINT16     imageSetIndices[MAX_JUNCTION_IMAGE_TYPES];
} TJunctionViewPath;

// this structure holds the various attributes for a given lane
typedef struct TRoadLaneAttributes {
  BOOL8             fwdR;     /* Forward restriction */
  BOOL8             bckR;     /* Backwards restriction */
  TRoadLaneDivType  laneDiv;  /* Lane divider type */
  UINT16            laneDir;  /* Lane Direction Mask */
} TRoadLaneAttributes;

typedef struct TRoadLaneSearchKey {
  const UINT8             *tableAddr; /* address of the lane path table  */
  UINT32                   segmentId; /* segment id to search */
  const TRoadSectorHeader *sector;    /* current sector header */
} TRoadLaneSearchKey;
typedef struct TRoadLaneSearchKey TJunctionViewSearchKey;

typedef enum TJunctionImageSetIndexWidth {
  JUNCTION_IMAGE_INDEX_8BIT  = 0,
  JUNCTION_IMAGE_INDEX_12BIT = 1,
  JUNCTION_IMAGE_INDEX_16BIT = 2,
} TJunctionImageSetIndexWidth;


/* The numeric suffix in the typedef names must be the
 * same as in the decl type. */
typedef UINT16 TRoadPostalCodeIndex16;
typedef UINT16 TRoadPlaceIndex16;
typedef UINT16 TRoadPlacePairIndex16;
typedef UINT16 TRoadSectorIndex16;
typedef UINT32 TRoadSectorPlacePairIndex32;
typedef UINT32 TRoadSectorPlacePairSetIndex32;
typedef UINT32 TRoadRoadIndex32;
typedef UINT32 TRoadRoadNameIndex32;
typedef UINT32 TRoadRoadSetIndex32;
typedef UINT16 TRoadHouseNumRoadSetSPPSegIndex16;
typedef UINT16 TRoadPlaceNameIndex16;
typedef UINT16 TRoadPlaceNameSetIndex16;

typedef struct TRoadPlace {
  TRoadPlaceNameSetIndex16 placeNameSets[PLACE_TYPES];
  TRoadPostalCodeIndex16   postalCodeIndex;
} TRoadPlace;

typedef struct TRoadPlacePair {
  TRoadPlaceIndex16  leftPlaceIndex;
  TRoadPlaceIndex16  rightPlaceIndex;
} TRoadPlacePair;

typedef struct TRoadSectorPlacePair {
  TRoadSectorIndex16     sectorIndex;
  TRoadPlacePairIndex16  placePairIndex;
} TRoadSectorPlacePair;

typedef BOOL8 TRoadVisitSectorPlacePairSetItem(
  TRoadSectorPlacePairIndex32  sectorPlacePairIndex,
  void                        *context);

/** @note this structure cannot be moved or copied */
typedef struct TRoadSectorPlacePairSet {
  TRoadSectorPlacePairIndex32   *sectorPlacePairs;
  UINT16                         sectorPlacePairCount;
  union {
    TAllocatorHandle             allocator;
    TRoadSectorPlacePairIndex32  sectorPlacePair;
  }                              _buffer;
} TRoadSectorPlacePairSet;

typedef struct TRoadRoad {
  TRoadRoadNameIndex32            nameIndex;
  TRoadSectorPlacePairSetIndex32  sectorPlacePairSetIndex;
} TRoadRoad;

#define ROAD_ROAD_SET_BUFFER_SIZE 2

/** @note this structure cannot be moved or copied */
typedef struct TRoadRoadSet {
  TRoadRoadIndex32   *roads;
  UINT16              roads1Count;
  UINT16              roads2Count;
  union {
    TAllocatorHandle  allocator;
    TRoadRoadIndex32  roads[ROAD_ROAD_SET_BUFFER_SIZE];
  }                   _buffer;
} TRoadRoadSet;

typedef struct TRoadRoadRangeRoadSetTable {
  UINT32                roadRangeCount;
  UINT16               *roadRangeRoadSetCounts;
  TRoadRoadSetIndex32 **roadRangeRoadSets;
//TRoadRoadSetIndex32  *_roadSetBuffer;   //TODO we could possibly have multiple buffers.
} TRoadRoadRangeRoadSetTable;

typedef struct TRoadSPPRoadSetTable {
  UINT16             *sppRoadSetCounts;
  UINT32            **sppRoadSets;
  UINT32             *_roadSetBuffer;   //TODO we could possibly have multiple buffers.
  UINT8               _initPass;
  UINT16              _initChunkIndex;
  TRoadRoadSetIndex32 _initRoadSetIndex;
} TRoadSPPRoadSetTable;

#define ROAD_ROAD_SET_INTERSECTION_GROUP_MAX (2)

/** @note All fields of this structure with a leading
          underscore are for private use only.
          All other fields must be treated as read only.
          Only the first 'segCount' elements of the
          'sectorPlacePairs' and 'localRoadSets' arrays
          are valid. */
typedef struct TRoadRoadSetIntersectionIterator {
  TGenericMapInfo            *mapInfo;
  const TRoadSPPRoadSetTable *sppRoadSetTable;
  UINT32                      sectorPlacePairs[ROAD_NODE_MAX_SEGMENTS];
  UINT32                      localRoadSets[ROAD_NODE_MAX_SEGMENTS];
  INT32                       intersectionRowId;
  UINT8                       instanceIndex;
  BOOL8                       flagMergeSameRoadPairs;
  UINT8                       segCount;
  UINT8                       _group;
  UINT32                      _rowIndex;
  UINT32                      _sectorPlacePairCount;
  UINT8                       _groupMaxSegCount[ROAD_ROAD_SET_INTERSECTION_GROUP_MAX];
  UINT32                      _groupSegCountRowCount[ROAD_ROAD_SET_INTERSECTION_GROUP_MAX][ROAD_NODE_MAX_SEGMENTS - 1];
  TBitStream                  _bitStream;
} TRoadRoadSetIntersectionIterator;

/** @internal */
typedef struct TRoadHouseNumberRangeLimits {
  UINT32  rangeCount;
  UINT32  maxStartNumber;
  UINT32  maxDelta;
} TRoadHouseNumberRangeLimits;

/** @note this structure cannot be moved or copied */
typedef struct TRoadHouseNumRoadSetSPPIterator {
  TGenericMapInfo             *_mapInfo;
  TRoadHouseNumberRangeLimits  _rangeLimits;
  TRoadSectorPlacePairSet      _sectorPlacePairSet;
  UINT16                       _chunkIndex;
  UINT16                       _sppRecIndex;
  UINT32                       _sppRecPos;
  UINT32                       _roadSetRecEnd;
} TRoadHouseNumRoadSetSPPIterator;

/** @internal */
typedef struct TRoadHouseNumRoadSetSPPSegIterState {
  UINT32  segRecPos;
  UINT32  sppRecEnd;
} TRoadHouseNumRoadSetSPPSegIterState;

typedef struct TRoadHouseNumRoadSetSPPSegIterator {
  TGenericMapInfo                     *_mapInfo;
  TRoadHouseNumberRangeLimits          _rangeLimits;
  UINT16                               _chunkIndex;
  TRoadHouseNumRoadSetSPPSegIterState  _iterState;
} TRoadHouseNumRoadSetSPPSegIterator;

/** @note this structure cannot be moved or copied */
typedef struct TRoadHouseNumSectorSegIterator {
  TGenericMapInfo             *_mapInfo;
  TRoadHouseNumberRangeLimits  _rangeLimits;
//UINT16                       _chunkIndex;
  TRBTree                      _roadSetPlacePairIterators;
  TAllocatorHandle             _allocator;
} TRoadHouseNumSectorSegIterator;

#define ROAD_PLACE_NAME_SET_BUFFER_SIZE (sizeof(UINTP)/sizeof(TRoadPlaceNameIndex16))

/** @note this structure cannot be moved or copied */
typedef struct TRoadPlaceNameSet {
  TRoadPlaceNameIndex16   *placeNames;
  UINT16                   officialNameCount;
  UINT16                   officialExonymCount;
  UINT16                   alternateNameCount;
  union {
    TAllocatorHandle       allocator;
    TRoadPlaceNameIndex16  placeNames[ROAD_PLACE_NAME_SET_BUFFER_SIZE];
  }                        _buffer;
} TRoadPlaceNameSet;

// Structure used to return snapping points on a road segment
typedef struct TRoadSegmentSnapPoint{
  TPos      pos;
  TRoadSide side;
  UINT32    distAlongSeg;
  UINT32    distance;
  BOOL8     orientationMatch;
  INT32     orientation;
} TRoadSegmentSnapPoint;

// BEGIN PRIVATE

/*----------------------------------------------------------------*/

/// Find the map header of a given global file offset.
extern TGenericMapInfo *road_findMap(UINT32 globalMapPtr);

/* in each map file pointers to objects are relative to the start of
 * the file.  The roadfile and mapfile module allows them to be linked
 * into a global range of map pointer addresses.  It does this by
 * making NODE AND SEGMENT POINTERS GLOBAL by allocating each map a
 * range of pointer addresses.  road_findSector finds the sector
 * containing the specified node or segment in the global map
 * space.
 */
extern TRoadSectorHeader *road_findSector(UINT32 globalMapPtr);

/*
 * Find the first and last super sector indexes in order to cover
 * the entire rectangular region <bounds_w> (in world coordinates)
 *
 * Returns TRUE if any super sector overlaps visible area
 * or FALSE if no super sector overlaps visible area.
 * firstIndex and lastIndex are set only when function returns
 * TRUE.
 */
extern BOOL8 road_findSuperSectorRange(UINT32 *firstIndex,
                                       UINT32 *lastIndex,
                                       TGenericMapInfo *const mapInfo,
                                       TRoadMapLayer roadMapLayer,
                                       const TRect *bounds_w);

/* call internal unpacking function to unpack node data at global file
 * offset into TRoadNode structure and find segments in other maps
 * that are glued to this node through global node IDs.  The source
 * sector may not be known, in which case it can be passed as NULL.
 * some fields required may be non cache safe (e.g. turnRestrictions)
 * in which case the sector containing the node in question should be
 * first identified using road_findSector and then locked in place
 * using roadfile_lockSector prior to calling road_unpackSegment, then
 * these fields can be read before the sector is unlocked.  If only
 * cache safe fields are required then the callee can rely on a
 * road_unpackNode internal lock / unlock mechanism
 */
extern void road_unsafeUnpackNode(
  TRoadNode *node           /* output result */,
  TRoadSectorHeader* sector /* locked sector containing source */,
  UINT32 source             /* global map pointer to source node */ );
extern void road_unpackNode(
  TRoadNode *node           /* output result */,
  UINT32 source             /* global map pointer to source node */ );
extern void road_unpackNodeNoGlue(
  TRoadNode *node           /* output result */,
  UINT32 source             /* global map pointer to source node */ );

  /* used by display routine to get first or last node positions,
  for last node source sector may be unknown */
  extern void road_unpackNodePosition(
    TPos              *p      /* result placed here */,
    UINT32             source /* global map pointer to source node */);

// END PRIVATE

/*
 * Same as road_unpackNodePosition() but assume that node is
 * in sector <sector> (no verification) and assumes sector has already
 * been locked.  This function is thus slightly faster than
 * road_unpackNodePosition() when we know for sure in
 * which sector the node is located.  This can for example
 * be used to unpack the position of the first node of a
 * segment which is guaranteed to be inside the segment's sector.
 */
extern void road_unsafeUnpackNodePosition(
  TPos              *p      /* result placed here */,
  TRoadSectorHeader *sector /* source sector containing node */,
  UINT32             source /* global map pointer to source node */);

// BEGIN PRIVATE

/* unpacking function to unpack segment data at global file offset
 * into TRoadSegment structure.  The source sector may not be known,
 * in which case it can be passed as NULL.  some fields required may
 * be non cache safe (e.g. shapePoints and addresses) in which case
 * the sector containing the segment in question should be first
 * identified using road_findSector and then locked in place using
 * roadfile_lockSector prior to calling road_unpackSegment, then the
 * required fields can be read before the sector is unlocked.  If only
 * cache safe fields are required then the callee can rely on a
 * road_unpackSegment internal lock / unlock mechanism.
 */
extern void road_unpackSegment(
  TRoadSegment      *segment /* output result */,
  UINT32             source  /* global map pointer to source segment */);

/*
 * Same as road_unpackSegment() but assume that segment is
 * in sector <sector> (no verification) and assumes sector has already
 * been locked.  This function is thus slightly faster than the
 * safe one road_unpackSegment() when we know for sure in
 * which sector the node is located.
 */
extern void road_unsafeUnpackSegment(
  TRoadSegment      *segment /* output result */,
  TRoadSectorHeader *sector  /* sector containing source if known */,
  UINT32             source  /* global map pointer to source segment */);


/* This function is used at boundary nodes and wormhole nodes to
 * expand a segment where either the first or last node is already
 * known.  It replaces the encoded firstNode or lastNode value in the
 * segment with commonNode where a connection through a wormhole or
 * global node exists.  If commonNode is not found to be connected in
 * any way to the segment that has been expanded then the function
 * returns the first and last node ID's as encoded in the file and
 * returns FALSE.
 */
extern BOOL8 road_unsafeUnpackConsecutiveSegment(
  TRoadSegment      *segment /* output result */,
  TRoadSectorHeader *sector  /* sector containing source if known */,
  UINT32             source  /* global map pointer to source segment */,
  UINT32             commonNode );
extern BOOL8 road_unpackConsecutiveSegment(
  TRoadSegment *segment     /* output result */,
  UINT32 source             /* global map pointer to source segment */,
  UINT32 commonNode );

/* find the road names that apply to the specified road segment by
 * scanning all sectors in the correct order to find the source
 * segment
 */
extern void road_findSegmentName(
  UINT32         segment       /* global map pointer of source segment */,
  BOOL8          identityName, /* flag indicating whether or not to get identity names*/
  UTF16         *name1         /* place to put name1 result */,
  UTF16         *name2         /* place to put name2 result */);

/* find the house number range that applies to the specified road
 * segment within a sector by scanning all segments in the correct
 * order to find the source segment
 */
extern BOOL8 road_findHouseNumbers(TRoadHouseNumbers *houseNumberData,
                                   TRoadSegment      *segmentOut,
                                   UINT32             segmentId);

/* Unpack ALL the details of a specified road segment into a TRoadSegment structure
   by scanning segments in the correct order - sector must be known exactly */
extern BOOL8 road_findSegmentDetails(TRoadSegment *segmentData,
  TRoadSectorHeader *sector, UINT32 segment);

// converts segment.roadSetIndex value into two global name indices that
// correspond to the old name1/name2 values attached to a segment.
extern void road_getRoadSetNames(TGenericMapInfo *const mapInfo,
                                 UINT32           roadSetIndex,
                                 UINT32          *name1Index,
                                 UINT32          *name2Index);

/* find the indices of the road names that apply to the specified road
   segment within a sector by scanning all segments in the correct order to
   find the source segment */
extern BOOL8 road_findSegmentNameIndex(INT32 *name1Index,
                                       INT32 *name2Index,
                                       TRoadSectorHeader *sector,
                                       UINT32 segment);

/* returns the road network to which a particular road type normally belongs
 * returns:
 * 0 = major road network
 * 1 = interconnect network
 * 2 = minor road network
 *
 * In order to extract roads of the given type from the data it is
 * necessary to scan from layer 0 up to and including the layer
 * returned by this function
 */
extern TRoadNetworkType road_calcNetworkType(TRoadType roadType);

extern void road_calcFrcRange(TRoadNetworkType networkType,
                              TRoadType *startRange,
                              TRoadType *endRange);

/* Turn restrictions are tied to a node, and can optionally be
 * unpacked one at a time using this routine.  The source data comes
 * from the TRoadNode.turnRestrictions array of RAWTURN16 pointers.
 * note that this field of the TRoadNode structure is not cache safe
 * and therefore the sector containing the node in question should be
 * first identified using road_findSector and then locked in place
 * using roadfile_lockSector prior to calling road_unpackSegment, then
 * the turnRestrictions can be unpacked by road_unpackTurnRestriction
 * before the sector is unlocked
 */
extern void road_unpackTurnRestriction(
  TRoadTurnRestriction *restriction /* output turn restriction data */,
  TRoadSectorHeader *sector   /* sector containing the turn restriction known */,
  UINT16 source,                     /* source turn restriction */
  const TRoadNode *pNode);           /* unpacked node */


/* Mini-Pea Specific Functions */
#define road_getExtendedChunkIndex(mapInfo, index)                      \
  (((ROAD_GLOBALS(mapInfo)->extChunkStart + (index)) < mapInfo->chunkCount) ? \
   (ROAD_GLOBALS(mapInfo)->extChunkStart + (index)): 0)

#define ROAD_GET_MISC_CHUNK(mapInfo)                                    \
  road_getExtendedChunkIndex((mapInfo), ROAD_EXTENDED_CHUNK_TOKEN_MISC)

#define ROAD_MISC_CHUNK_HEADER(mapInfo, miscChunkIdx)                   \
  ((TBinTokenHeader *)(mapcache_lookup(mapInfo, miscChunkIdx)))

BOOL8 road_isARoute(TGenericMapInfo *const mapInfo,
                    const UINT32     nameId);

//returns the offsets of the 'non route' tokens and names
void road_getNonRouteOffsets(TGenericMapInfo *const mapInfo,
                             UINT32          *tokenOffset,
                             UINT32          *nameOffset);

BOOL8 road_initPlaceNamePlaceIterator(
  TRoadPlaceNamePlaceIterator  *namePlaceIterator,
  UINT32                        placeNameId,
  TGenericMapInfo        *const mapInfo);

BOOL8 road_unsafeUnpackNextPlaceNamePlace(
  TRoadPlaceNamePlaceIterator  *iterator,
  TRoadPlaceNamePlace          *result );

BOOL8 road_unpackPostCodePlace (
  UINT32                  postCodeId,
  TGenericMapInfo  *const mapInfo,
  TRoadPocodePlace*       result);

UINT8 road_unpackDisplayName(
  TGenericMapInfo *const mapInfo,
  TRoadNameType    type,
  UINT32           nameId,
  BOOL8            abbreviate,
  BOOL8            appendDirectional,
  UTF16*           output,
  UINT16*          routeType);

BOOL8 road_initTokenDataSource(TTokenDataSource*      tknData,
                               TGenericMapInfo       *const mapInfo,
                               const TRoadNameType    nameType);

BOOL8
road_initPhoneticDataSource(TPhoneticDataSource*   phoneticSource,
                            TGenericMapInfo       *const mapInfo,
                            TPhoneticDataType      dataType);

///
/// \brief Unpacks a postal code from the global postal code table
///
/// @param            output[out] unpacked string
/// @param          mapHeader[in] map header
/// @param  globalPostalIndex[in] index of string to unpack (from 0 to
///                               to the number of string in table - 1)
///
/// @return length of the name unpacked, or zero if not found
///
extern UINT8
road_unpackGlobalPostalCode(UTF16           *output,
                            TGenericMapInfo *const mapInfo,
                            const UINT32     globalPostalIndex);

extern void road_calculateSectorBounds(
    TRect                   *bbox,
    const TRoadSectorHeader *sector );

extern void road_calculateSuperSectorBounds(
    TRect                        *bbox,
    const TRoadSuperSectorHeader *ssector );


// gets the name of the road map.  This is a succinct descriptor that
// describes the map coverage.  E.g. "Greater Sydney"
extern void road_unpackMapName(char *output, const TGenericMapInfo *const mapInfo);

// gets the two letter descriptor (e.g., "NZ") of the country
// from the map header.
void road_unpackCountryCode(TCountryCode *pCountryCode,
                            const TGenericMapInfo *const mapInfo);

// called once at startup to initialise the road module
extern BOOL8 road_init(void);

// get the number of postal code records in a particular road map
extern UINT32 road_getGlobalPostalCodeCount(TGenericMapInfo *const mapInfo);

// get the number of road name records in a particular road map
extern UINT32 road_getGlobalRoadNameCount(TGenericMapInfo *const mapInfo);

// get the number of place names in a particular road map
extern UINT32 road_getGlobalPlaceNameCount(TGenericMapInfo *const mapInfo);

UINT32 road_getAddressDisplayCountryCount(TGenericMapInfo *const mapInfo);

UINT32 road_getAddressDisplayFormatCount(TGenericMapInfo *const mapInfo);

BOOL8 road_unpackAddressDisplayCountry(TAddressDisplayCountry *output,
                                       TGenericMapInfo *const mapInfo,
                                       const UINT32 record);

BOOL8 road_unpackAddressDisplayFormat(TAddressDisplayFormat *output,
                                      TGenericMapInfo *const mapInfo,
                                      const UINT32 record);

// this type mirrors the TRoadSegment bitfields and road length that are directly
// copied from the map data.
// IF THE SIZE OF THE BITFIELDS CHANGES THEN THIS STRUCTURE MUST BE CHANGED
typedef struct TRoadPackedSegment {
  UINT8  flagsAlwaysPresent[2];
  UINT8  flagsRoadLen;
  UINT8  flagsFOWEncoded;
  UINT8  flagsLongRoad;
  UINT8  flagsRare1;
  UINT8  flagsRare2;
} TRoadPackedSegment;

/*----------------------------------------------------------------*/

// Determine the drive side of the current map.
//
// @param pThis A sector in the map whose handedness should be determined
// @param DRIVE_SIDE_LEFT or DRIVE_SIDE_RIGHT as required
TRoadHandedness roadSectorHeader_getDriveSide(
  const TRoadSectorHeader* pThis );


// Determine if dual carriageway u-turn are allowed in this sector
//
// @param pThis A sector in the map whose dc u-turn rule should be determined
// @return TRUE if dc u-turns are okay, FALSE otherwise
BOOL8 roadSectorHeader_getDualCarriagewayUTurnsAllowed(
  const TRoadSectorHeader* pThis );

TRoadSpeedUnit roadSectorHeader_getRoadSpeedUnit (
  const TRoadSectorHeader *pRoadSectorHeader );

TRoadSpeedUnit road_getRoadSpeedUnit (
  const TRoadSegment *pSegment );

extern TPos* road_getVertices(
  TRoadSegment    *segment /* Already populated segment */
  );

extern void road_getVertex(
  TRoadSegment    *segment, /* Already populated segment */
  UINT32          index,
  TPos            *output
  );

extern void road_unsafeGetVertex(
  TRoadSegment      *segment, /* Already populated segment */
  TRoadSectorHeader *sector,
  UINT32             index, /* index of the shapepoint required */
  TPos              *output
  );

extern TPos* road_unsafeGetVertices(
  TRoadSegment      *segment,
  TRoadSectorHeader *sector
  );

// END PRIVATE

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

///
/// @addtogroup Map
/// @ingroup UnUsed
/// @{
///


/// @addtogroup Road
/// @ingroup Map
/// @{

/// @addtogroup RoadSegment
/// @ingroup Road
/// @{

// BEGIN PRIVATE
extern BOOL8 road_isOnBaseMap(const UINT32 offset);
// END PRIVATE

/// }@
/// }@
/// }@

/*----------------------------------------------------------------*/

/* Maximum string len of a sign post info (without EOS) */
#define SIGN_POST_INFO_STR_LEN_MAX 80/*ROAD_MAX_NAME_SIZE*/

/* no more than x sign post info per sign post */
#define SIGN_POST_INFO_COUNT_MAX 15

/* Maximum number of segments in a sign post path */
#define SIGN_POST_PATH_LEN_MAX 15

// The string ID contains sufficient information
// to generate a string, either normal or phonetic.
typedef struct TRoadStringID {
  TGenericMapInfo *mapInfo;
  UINT32 strIdx;
  TPhoneticDataType dataType; // not POIs for now.
} TRoadStringID;

// Define a null string ID.
#define ROAD_STRING_ID_CLEAR(roadStringID) (((TRoadStringID *)(roadStringID))->mapInfo = NULL)
#define ROAD_STRING_ID_IS_NULL(roadStringID) (((TRoadStringID *)(roadStringID))->mapInfo == NULL)
#define ROAD_STRING_ID_IS_EMPTY(roadStringID) (ROAD_STRING_ID_IS_NULL(roadStringID) || ((TRoadStringID *)(roadStringID))->strIdx == 0)

extern void road_findSegmentRoadStringIDs(UINT32         segment,
                                          TRoadStringID *id1,
                                          TRoadStringID *id2);

/* Where is sign post string located */
typedef enum {
  SIGN_POST_STR_OTHER,
  SIGN_POST_STR_ROAD_NAME,
  SIGN_POST_STR_PLACE
} TRoadSignPostInfoStringWhere;

/* Type of sign post info */
typedef enum {
  SIGN_POST_TYPE_PLACE_NAME,   /* Place Name */
  SIGN_POST_TYPE_ROUTE_NUMBER, /* Route Number */
  SIGN_POST_TYPE_PICTOGRAM,    /* Pictogram */
  SIGN_POST_TYPE_EXIT_NUMBER,  /* Exit Number */
  SIGN_POST_TYPE_STREET_NAME,  /* Street Name */
  SIGN_POST_TYPE_EXIT_NAME,    /* Exit Name */
  SIGN_POST_TYPE_DIRECTIONAL,  /* Route Directional */
  SIGN_POST_TYPE_OTHER_DEST,   /* Other Destination */
  SIGN_POST_TYPE_ROUTE_TYPE,   /* Route number type */
  SIGN_POST_TYPE_DISABLED      /* Disabled Signpost Data,
                                  should not be used */
} TRoadSignPostInfoType;

/*
 * Connection type of sign posts info. For more details, see:
 * - CONTYP field of TA SI table
 * - sign post info section 3.01 of MultiNet_3-4_DataSpec_Final_v1-3.pdf
 */
typedef enum {
  SIGN_POST_CONTYP_UNDEF,
  SIGN_POST_CONTYP_BRANCH,  /* the destination(s) mentioned in sign
                             * | post is reached after the decision
                             * | point (ignoring slot/parallel roads)
                             * | Destination info can only be
                             * | route number or street name */
  SIGN_POST_CONTYP_TOWARDS, /* the destination(s) mentioned in sign
                               | post is not yet reached after the decision
                               | point (ignoring slip/parallel roads) */
  SIGN_POST_CONTYP_EXIT     /* exit information, info type can
                             * | only be exit number or exit name */
} TRoadSignPostInfoConType;

/*
 * Signpost information pictogram code. For more details, see INFOTYP 4H
 * of the TA SI table.
 */
typedef enum {
  SIGN_POST_PICTOGRAM_AIRPORT = 1,
  SIGN_POST_PICTOGRAM_BUS_STATION,
  SIGN_POST_PICTOGRAM_FAIR,
  SIGN_POST_PICTOGRAM_FERRY_CONNECTION,
  SIGN_POST_PICTOGRAM_FIRST_AID_POST,
  SIGN_POST_PICTOGRAM_HARBOUR,
  SIGN_POST_PICTOGRAM_HOSPITAL,
  SIGN_POST_PICTOGRAM_HOTEL_MOTEL,
  SIGN_POST_PICTOGRAM_INDUSTRIAL_AREA,
  SIGN_POST_PICTOGRAM_INFORMATION_CENTRE,
  SIGN_POST_PICTOGRAM_PARKING_FACILITY,
  SIGN_POST_PICTOGRAM_PETROL_STATION,
  SIGN_POST_PICTOGRAM_RAILWAY_STATION,
  SIGN_POST_PICTOGRAM_REST_AREA,
  SIGN_POST_PICTOGRAM_RESTAURANT,
  SIGN_POST_PICTOGRAM_TOILET
} TRoadSignPostPictogramType;

/*
 * Sign post info contains information inside the matrix
 * of sign post infos in a sign post
 */
typedef struct TRoadSignPostInfo {
  /* Type of sign post info */
  TRoadSignPostInfoType    type;      /* type of sign info */
  TRoadSignPostInfoConType conType;   /* connection type */
  BOOL8                    ambiguous; /* ambiguous sign post info
                                       * | (more than 1 way to reach
                                       * | destination) */
  /*
   * Sequence in Destination Set (DESTSEQ in the TA SI table). This
   * represents the order of destination info inside a functional unit
   * of destination information.
   */
  INT8 row;
  /*
   * Destination Set Number (SEQNR in the TA SI table). Destination set
   * represents one functional unit of destination information on a signpost.
   */
  INT8 col;
  /*
   * Sign post string. Also return an identifier and a language code so that we
   * can unpack the phonetics later if we need them.
   */
  UTF16 str[SIGN_POST_INFO_STR_LEN_MAX];
  TRoadStringID roadStringID;
  TLanguageCode langCode;
} TRoadSignPostInfo;

//SI (Signpost Information) Table
//The SI table includes the text and pictogram descriptions. If a signpost has multiple
//textual descriptions (for example, when the same destination is described in two
//languages), or multiple pictograms (for example, Railway Station with Parking Facility
//on the same signpost), then the records are repeated in the SI table for each textual
//description or pictogram, and ordered by a Textual Context Sequential Number
//(SEQNR).
//When a textual description has more than one pictogram or destination, the
//pictograms or destinations are listed as they occur on the signpost with respect to the
//text: the signpost is treated as an information matrix. The SI table includes the
//DESTSEQ (Sequence in the Destination Set) field to determine the line in this matrix.
//If Route Numbers are listed on the signpost in one line, the sequence within the line
//is determined in the RNPART field (Route Number Part).
//Both text and pictograms are stored in the TXTCONT field of the SI table.
//EXAMPLE: “Waterloo Station // 13 // 11” indicates that a sign has, reading left to
//right, the name of the station, then a Railway Station pictogram, then a Parking
//Facility pictogram. “13 // Waterloo Station // 11” indicates that the Railway Station
//pictogram is to the left of the station name on the signpost.
//The SI table also indicates the Type of connection information that the signpost
//represents (CONTYP field).
//The AMBIG field indicates whether the destination information is ambiguous or clear.

/*
 * A sign post is attached to a node and contains a path
 * of segments.
 *
 * A sign posts also contains a matrix of sign post infos.
 */
typedef struct TRoadSignPost {
  /* segment path for this sign post */
  UINT32 segId[SIGN_POST_PATH_LEN_MAX];
  /* number of segments in sign post path */
  UINT8  segCount;
  /* number of sign post info in this sign post */
  UINT8  infoCount;
  /* <infoCount> sign post infos */
  TRoadSignPostInfo info[SIGN_POST_INFO_COUNT_MAX];
} TRoadSignPost;


/**
 * Unpack a sign post for a node and all its sign post infos.
 * Function works as an iterator through all sign post attached
 * to a node.
 *
 * For a given TRoadNode node, user of this API typically calls
 * the function as follows in order to process every sign post
 * of the node:
 *
 *   TRoadSignPost signPost;
 *   UINT32 signPostOffset = node->firstSignPostId;
 *
 *   while (signPostOffset) {
 *     signPostOffset = road_unpackSignPost(&signPost, signPostOffset);
 *     // perform whatever processing is required on <signPost>
 *     // this typically involves looping on all sign post infos
 *     for (i = 0; i < signPost.infoCount; i++) {
 *       // perform whatever processing is required on sign post info
 *       // ...
 *     }
 *   }
 *
 * @param signPost[output] structure to contain unpacked sign post
 * @param source[input]    id (ie map offset) of the sign post info
 *                         source should be node->firstSignPostId
 *                         when unpacking first sign post of an node
 *                         and returned value of road_unpackSignPost()
 *                         for further calls until road_unpackSignPost()
 *                         returns 0 (no more sign posts)
 *
 * @return id (ie offset) of next sign post info in same node
 *         return offset can be used as <source> parameter of
 *         next call to road_unpackSignPost() to unpack all sign
 *         posts until it returns 0 (last sign post info)
 */
extern UINT32 road_unpackSignPost(TRoadSignPost *signPost, UINT32 source);

/*----------------------------------------------------------------*/

/// Intersection Type. INTTYP field of TA IS table.
typedef enum {
  /// No Type (default). The intersection requires one route advisory.
  INTERS_TYPE_NO_TYPE = 0,
  /// Freeway Intersection.
  INTERS_TYPE_FREEWAY,
  /// Complex Street Intersection requiring more than one route advisory.
  INTERS_TYPE_COMPLEX_STREET
} TRoadIntersectionType;

/// Freeway Intersection Type. FWINTTYP field of TA IS table.
typedef enum {
  /// Not Applicable (default)
  FREEWAY_INTERS_TYPE_NOT_APPLICABLE = 0,
  /// Motorway Exit/Access
  FREEWAY_INTERS_TYPE_MOTORWAY_EXIT_ACCESS,
  /// Motorway Interchange
  FREEWAY_INTERS_TYPE_MOTORWAY_INTERCHANGE,
  /// Others
  FREEWAY_INTERS_TYPE_OTHER
} TRoadFreewayIntersectionType;

/// Unpacked road intersection record.
typedef struct TRoadIntersection {
  /// Official Name / Number
  UTF16 name[ROAD_MAX_NAME_SIZE];
  /// Intersection type
  TRoadIntersectionType type;
  /// Freeway intersection type
  TRoadFreewayIntersectionType fwtype;
} TRoadIntersection;

extern BOOL8 road_unpackIntersection(TGenericMapInfo *const mapInfo,
                                     UINT32 source,
                                     TRoadIntersection *intersection);

/*----------------------------------------------------------------*/

#define MAX_TZ_NAME_LEN 31 /* max length of a time zone name string */

/*
 * Time zone record
 */
typedef struct TRoadTimeZone {
  INT16 gmt;                         /* gmt offset in minutes */
  INT16 dls;                         /* daylight saving shift in minutes */

  /*
   * TODO: tzName should be localised (and thus also stored in Unicode)
   * if we want to use it.  But rather than putting localisation in
   * map data, we should put localised strings in localised resource
   * string files.
   *
   * Translation of time zone names can be found for example at:
   *
   * http://carolina.mff.cuni.cz/~trmac/fedora/translate/system-config-date/timezones/de.po
   * http://carolina.mff.cuni.cz/~trmac/fedora/translate/system-config-date/timezones/fr.po
   * etc...
   */
  char tzName[MAX_TZ_NAME_LEN + 1];  /* tz database name */
} TRoadTimeZone;

/*
 * Find the number of time zone records in the time zone table
 */
extern INT32 road_getTimeZoneRecordCount(TGenericMapInfo *const mapInfo);

/*
 * Unpack a time zone record from the time zone table
 */
extern BOOL8 road_unpackTimeZone(TGenericMapInfo *const mapInfo,
                                 UINT32                timeZoneIdx,
                                 TRoadTimeZone        *timeZone);

/*----------------------------------------------------------------*/

typedef struct TRoadCountryStateIterator {
  TGenericMapInfo *mapInfo;
  INT32            chunkIdx;
  UINT32           offset;   /* current offset of iterator in chunk */

  /*
   * These are the data fields to be used by caller of
   * road_initCountryStateIterator() and road_unpackNextCountryState()
   */
  UINT16        a0, a1;   /* country and state name set indices */
  UINT16        ISO_code; /* numerical ISO_3166-1 code of country */
  UINT16        nameIdx;  /* name index of the current country or state */
  BOOL8         official; /* flag indicating if the name is official or exonym */
  TLanguageCode langCode; /* the language code of the current country or state */
  UTF16         displayName[ROAD_MAX_NAME_SIZE];  /* the country or state display name */
  UTF16         identityName[ROAD_MAX_NAME_SIZE]; /* the country or state identity name */

} TRoadCountryStateIterator;

void road_initCountryStateIterator(TRoadCountryStateIterator *countryState,
                                   TGenericMapInfo           *mapInfo);

BOOL8 road_unpackNextCountryState(TRoadCountryStateIterator *countryState);

/*----------------------------------------------------------------*/

UINT32
road_unsafeUnpackRouteDirectional(TGenericMapInfo *const mapInfo,
                                  UINT32           routeDirID,
                                  UINT32           miscChunkIdx,
                                  TTokenRouteDir  *dir,
                                  UTF16           *dirString);

UINT32 road_unpackPhoneticString(
  TRoadStringID roadStringID,
  TLanguageCode languageCode,
  UTF16        *outputBuffer);

UINT32 road_unpackDisplayString(
  TRoadStringID roadStringID,
  BOOL8 abbreviate,
  BOOL8 appendDirectional,
  UTF16 *outputBuffer);

DEPRECATED(
BOOL8 road_getPriorityRoadName(
  TSegmentID segment,
  TRoadStringID *primaryName,
  TRoadStringID *secondaryName);
)

/// @brief Get a route shield colour rule for a particular map.
///
/// @param[in] mapInfo The map the road is in.
/// @param routeType The road's route type.
/// @param[out] rule
///
/// @return \c TRUE if a rule was found
///
BOOL8 road_getRouteShieldColour(const TGenericMapInfo *const mapInfo,
                                UINT16 routeType,
                                TRoadShieldRule *rule);

BOOL8 road_getCountryStateDetails(TGenericMapInfo           *mapInfo,
                                  UINT16                     a0,
                                  UINT16                     a1,
                                  TRoadCountryStateIterator *countryState);

BOOL8 road_getCountryISO(const TSegmentID segID,
                         TNumericISOCountryCode *ISO_code);

void road_unpackLanePaths(
  TRoadLanePath *lanePaths,
  UINT32        *foundCount,
  UINT32         segmentId );

void road_unpackJunctionViewPaths(
  TJunctionViewPath          *junctionViewPaths,
  UINT32                     *foundCount,
  TSegmentID                  segmentId);

void road_unpackLaneAttributes(
  TRoadLaneAttributes *laneAttribute,
  UINT32              segmentId,
  UINT8               laneNumber);

void road_unsafeUnpackLaneAttributes(
  TRoadLaneAttributes *laneAttribute,
  TRoadSectorHeader  *sector,
  UINT32              segmentId,
  UINT8               laneNumber);

void road_unsafeUnpackLanePaths(
  TRoadLanePath     *lanePaths,
  UINT32            *foundCount,
  TRoadSectorHeader *sector,
  UINT32             segmentId );

void road_unsafeUnpackJunctionViewPaths(
  TJunctionViewPath         *junctionViewPaths,
  UINT32                    *foundCount,
  const TRoadSectorHeader   *sector,
  TSegmentID                 segmentId);

/*
 *  This function parses the JunctionImageSet chunk for a roadMap to
 *  look for indices into the JunctionViewPictorial archive-map. It
 *  resolves JunctionImageSet indices into archive-map indices.
 *
 *  @param[in]  mapInfo
 *  @param[in]  dayOrNight
 *      To indicate which of the two possible images to pick out.
 *  @param[in]  numIndices
 *      The number of typeSpecific indices to resolve into archive indices.
 *  @param[in]  typeSpecificIndices
 *      This is an array of ZERO-BASED indices into the corresponding
 *      junction image-set rows. There is at most one row-index for
 *      each image-type category, and indexing into the rows will
 *      fetch the indices into the archive-map.
 *  @param[out] archiveIndices
 *      The array containing the results of the resolution. It
 *      contains one archive-map image index corresponding to each
 *      type-specific row index passed in. The caller is responsible
 *      for managing the memory for this array. It is safe to use the
 *      same array for both typeSpecificIndices and the output
 *      archiveIndices.
 */
void
road_unpackJunctionImageArchiveIndices(
  TGenericMapInfo            *const mapInfo,
  TJunctionImageDayNightType  dayOrNight,
  UINT16                      numIndices,
  const UINT16               *typeSpecificIndices,
  UINT16                     *archiveIndices);

UINT32 road_getPlaceCount(
  TGenericMapInfo *mapInfo);

void road_unpackPlace(
  TRoadPlace        *place,
  TRoadPlaceIndex16  placeIndex,
  TGenericMapInfo   *mapInfo);

UINT32 road_getPlacePairCount(
  TGenericMapInfo *mapInfo);

void road_unpackPlacePair(
  TRoadPlacePair        *placePair,
  TRoadPlacePairIndex16  placePairIndex,
  TGenericMapInfo       *mapInfo);

UINT32 road_getSectorPlacePairCount(
  TGenericMapInfo *mapInfo);

void road_unpackSectorPlacePair(
  TRoadSectorPlacePair        *sectorPlacePair,
  TRoadSectorPlacePairIndex32  sectorPlacePairIndex,
  TGenericMapInfo             *mapInfo);

UINT32 road_calcSPPFilterFromPlaceSectorFilters(
  TBitset         *sectorPlacePairFilterBitset,
  const TBitset   *placeFilterBitset,
  const TBitset   *sectorFilterBitset,
  TGenericMapInfo *mapInfo);

UINT32 road_getSectorPlacePairSetCount(
  TGenericMapInfo *mapInfo);

void road_scanSectorPlacePairSet(
  TRoadSectorPlacePairSetIndex32    sectorPlacePairSetIndex,
  TGenericMapInfo                  *mapInfo,
  TRoadVisitSectorPlacePairSetItem *visitSectorPlacePairSetItem,
  void                             *context);

BOOL8 road_unpackSectorPlacePairSet(
  TRoadSectorPlacePairSet        *sectorPlacePairSet,
  TRoadSectorPlacePairSetIndex32  sectorPlacePairSetIndex,
  TGenericMapInfo                *mapInfo,
  TAllocatorHandle                allocator);

void road_freeUnpackedSectorPlacePairSet(
  TRoadSectorPlacePairSet *sectorPlacePairSet);

UINT32 road_calcSPPSetFilterFromSPPFilter(
  TBitset         *sectorPlacePairSetFilterBitset,
  const TBitset   *sectorPlacePairFilterBitset,
  TGenericMapInfo *mapInfo);

UINT32 road_getRoadCount(
  TGenericMapInfo *mapInfo);

void road_unpackRoad(
  TRoadRoad        *road,
  TRoadRoadIndex32  roadIndex,
  TGenericMapInfo  *mapInfo);

UINT32 road_calcRoadNameFilterFromSPPSetFilter(
  TBitset         *roadNameFilterBitset,
  const TBitset   *sectorPlacePairSetFilterBitset,
  TGenericMapInfo *mapInfo);

UINT32 road_calcRoadFilterFromRoadNameFilter(
  TBitset         *roadFilterBitset,
  const TBitset   *roadNameFilterBitset,
  TGenericMapInfo *mapHeader);

BOOL8 road_unpackGlueRoadBounds(
  TRect            *bounds,
  TRoadRoadIndex32  roadIndex,
  TGenericMapInfo  *mapInfo);

UINT32 road_getRoadSetCount(
  TGenericMapInfo *mapInfo);

BOOL8 road_unpackRoadSet(
  TRoadRoadSet        *roadSet,
  TRoadRoadSetIndex32  roadSetIndex,
  TGenericMapInfo     *mapInfo,
  TAllocatorHandle     allocator);

void road_freeUnpackedRoadSet(
  TRoadRoadSet *roadSet);

void road_getRoadSetPriorityNames(
  TRoadRoadNameIndex32 *roadName1,
  TRoadRoadNameIndex32 *roadName2,
  TGenericMapInfo      *mapInfo,
  TRoadRoadSetIndex32   roadSetIndex);

void road_getRoadSetDisplayNames(
  TRoadRoadNameIndex32 *roadName1,
  TRoadRoadNameIndex32 *roadName2,
  TGenericMapInfo      *mapInfo,
  TRoadRoadSetIndex32   roadSetIndex,
  const TLanguageCode   altLangs[],
  UINT32                altLangCount);

void road_unpackRoadSetDisplayNames(
  UTF16               *roadName1,
  UTF16               *roadName2,
  TGenericMapInfo     *mapInfo,
  TRoadRoadSetIndex32  roadSetIndex,
  const TLanguageCode  altLangs[],
  UINT32               altLangCount,
  BOOL8                abbreviate);

//This struct is short-term solution for Taiwan full address display issue.
//If the long-term solution be define, and this struct will be remove.
void road_unpackRoadSetDisplayNames2(
  UTF16               *roadName1,
  UTF16               *roadName2,
  TGenericMapInfo     *mapInfo,
  TRoadSectorHeader   *sector,
  UINT32               segmentID);

void road_getRoadSetDisplayRoads(
  TRoadRoadIndex32     *roadIndex1,
  TRoadRoadIndex32     *roadIndex2,
  TGenericMapInfo      *mapInfo,
  TRoadRoadSetIndex32   roadSetIndex,
  const TLanguageCode   altLangs[],
  UINT32                altLangCount);

UINT32 road_calcRoadSetFilterFromRoadFilter(
  TBitset*         roadSetFilterBitset,
  const TBitset   *roadFilterBitset,
  TGenericMapInfo *mapInfo);

BOOL8 road_initRoadRangeRoadSetTable(
  TRoadRoadRangeRoadSetTable *roadRangeRoadSetTable,
  TGenericMapInfo            *mapInfo,
  UINT32                      roadRangeCount,
  const TRoadRoadIndex32     *firstRoadIndices,
  const TRoadRoadIndex32     *lastRoadIndices,
  TAllocatorHandle            allocator);

void road_destroyRoadRangeRoadSetTable(
  TRoadRoadRangeRoadSetTable *roadRangeRoadSetTable,
  TAllocatorHandle            allocator);

INT32 road_initSPPRoadSetTable(
  TRoadSPPRoadSetTable *sppRoadSetTable,
  TGenericMapInfo      *mapInfo,
  const TBitset        *sppBitset,
  TAllocatorHandle      allocator,
  BOOL8                 flagStartInit);

void road_destroySPPRoadSetTable(
  TRoadSPPRoadSetTable *sppRoadSetTable,
  TAllocatorHandle      allocator);

BOOL8 road_initRoadSetIntersectionIterator(
  TRoadRoadSetIntersectionIterator *iterator,
  TGenericMapInfo                  *mapInfo,
  const TRoadSPPRoadSetTable       *sppRoadSetTable);

void road_destroyRoadSetIntersectionIterator(
  TRoadRoadSetIntersectionIterator *iterator);

BOOL8 road_unpackNextRoadSetIntersection(
  TRoadRoadSetIntersectionIterator *iterator);

BOOL8 road_initHouseNumRoadSetSPPIterator(
  TRoadHouseNumRoadSetSPPIterator *sppIterator,
  TGenericMapInfo                 *mapInfo,
  TRoadRoadSetIndex32              roadSetIndex,
  TAllocatorHandle                 allocator);

void road_destroyHouseNumRoadSetSPPIterator(
  TRoadHouseNumRoadSetSPPIterator *sppIterator);

void road_lockHouseNumRoadSetSPPIteratorData(
  const TRoadHouseNumRoadSetSPPIterator *sppIterator);

void road_unlockHouseNumRoadSetSPPIteratorData(
  const TRoadHouseNumRoadSetSPPIterator *sppIterator);

BOOL8 road_getHouseNumRoadSetSPPIterSPPIndex(
  const TRoadHouseNumRoadSetSPPIterator *sppIterator,
  TRoadSectorPlacePairIndex32           *sectorPlacePairIndex);

void road_unsafeAdvanceHouseNumRoadSetSPPIterator(
  TRoadHouseNumRoadSetSPPIterator *sppIterator);

void road_unsafeGetHouseNumRoadSetSPPSegIterator(
  const TRoadHouseNumRoadSetSPPIterator *sppIterator,
  TRoadHouseNumRoadSetSPPSegIterator    *segIterator);

BOOL8 road_initHouseNumRoadSetSPPSegIterator(
  TRoadHouseNumRoadSetSPPSegIterator *segIterator,
  TGenericMapInfo                    *mapInfo,
  TRoadRoadSetIndex32                 roadSetIndex,
  TRoadSectorIndex16                  sectorIndex,
  TRoadPlacePairIndex16               placePairIndex,
  TAllocatorHandle                    allocator);

void road_lockHouseNumRoadSetSPPSegIteratorData(
  TRoadHouseNumRoadSetSPPSegIterator *segIterator);

void road_unlockHouseNumRoadSetSPPSegIteratorData(
  TRoadHouseNumRoadSetSPPSegIterator *segIterator);

BOOL8 road_unsafeUnpackNextHouseNumRoadSetSPPSegment(
  TRoadHouseNumRoadSetSPPSegIterator *segIterator,
  TRoadHouseNumbers                  *houseNumbers);

BOOL8 road_initHouseNumSectorSegIterator(
  TRoadHouseNumSectorSegIterator *segIterator,
  TGenericMapInfo                *mapInfo,
  TRoadSectorIndex16              sectorIndex,
  TAllocatorHandle                allocator);

void road_destroyHouseNumSectorSegIterator(
  TRoadHouseNumSectorSegIterator *segIterator);

void road_checkHouseNumSectorSegIterator(
  TRoadHouseNumSectorSegIterator *segIterator);

void road_lockHouseNumSectorSegIteratorData(
  TRoadHouseNumSectorSegIterator *segIterator);

void road_unlockHouseNumSectorSegIteratorData(
  TRoadHouseNumSectorSegIterator *segIterator);

void road_unsafeUnpackNextHouseNumSectorSegment(
  TRoadHouseNumSectorSegIterator    *segIterator,
  TRoadHouseNumbers                 *houseNumbers,
  TRoadHouseNumRoadSetSPPSegIndex16 *segRecIndex,
  TRoadRoadSetIndex32                roadSetIndex,
  TRoadPlacePairIndex16              placePairIndex);

void road_unpackNextHouseNumSectorSegment(
  TRoadHouseNumSectorSegIterator    *segIterator,
  TRoadHouseNumbers                 *houseNumbers,
  TRoadHouseNumRoadSetSPPSegIndex16 *segRecIndex,
  TRoadRoadSetIndex32                roadSetIndex,
  TRoadPlacePairIndex16              placePairIndex);

UINT32 road_getPlaceNameSetCount(
  TGenericMapInfo *mapInfo);

BOOL8 road_unpackPlaceNameSet(
  TRoadPlaceNameSet        *placeNameSet,
  TRoadPlaceNameSetIndex16  placeNameSetIndex,
  TGenericMapInfo          *mapInfo,
  TAllocatorHandle          allocator);

void road_freeUnpackedPlaceNameSet(
  TRoadPlaceNameSet *placeNameSet);

TRoadPlaceNameIndex16 road_getPlaceNameSetIdentityNameIndex(
  TRoadPlaceNameSetIndex16  placeNameSetIndex,
  TGenericMapInfo          *mapInfo);

TRoadPlaceNameIndex16 road_getPlaceNameSetDisplayNameIndex(
  TRoadPlaceNameSetIndex16  placeNameSetIndex,
  TGenericMapInfo          *mapInfo);

void road_unpackCountryPlaceNameSetDisplayName(
  UTF16                    *countryName,
  TRoadPlaceNameSetIndex16  countryNameSetIndex,
  TGenericMapInfo          *mapInfo);

void road_unpackSegmentBounds(
  TRoadSegment *segment,
  TRect        *bounds );

void road_unsafeUnpackSegmentBounds(
  TRoadSegment      *segment,
  TRoadSectorHeader *sector,
  TRect             *bounds );

UINT32 road_getAverageSpeed(
  const TRoadSegment *pSegment,
  TVehicleType        vehicleType );

UINT32 road_getMaximumFreewaySpeed(
  void );

UINT32 road_getMaximumNonFreewaySpeed(
  void );


// TMC ---------------------------------------------------------------

/// Holds un unpacked TMC location table index entry.
typedef struct TRoadTmcLocationTableIndexRow {
  UINT8  cc;                    ///< TMC country code
  UINT16 ltn;                   ///< TMC location table number
  char   tabver[10];            ///< TMC location table version

  /// @internal
  /// the map info of the road map this index entry is for
  TGenericMapInfo *_mapInfo;

  /// @internal
  /// TMC location index chunk containing locations for this table
  UINT8 _chunkIndex;

  /// @internal
  /// count of location index chunk; only valid if locked
  UINT32 _count;

  /// @internal
  /// address of location index chunk if locked, otherwise NULL
  UINT8 *_chunkAddress;
} TRoadTmcLocationTableIndexRow;

UINT32 road_unpackTmcLocationTableIndex(
  TRoadTmcLocationTableIndexRow *rows,
  UINT32                         maxRows,
  TGenericMapInfo               *mapInfo);

BOOL8 road_findTmcLocationTable(
  TRoadTmcLocationTableIndexRow *tableIndexRow,
  UINT8                          cc,
  UINT16                         ltn,
  const char                     tabver[10],
  TGenericMapInfo               *mapInfo);

void road_lockTmcLocationIndex(TRoadTmcLocationTableIndexRow *tableIndexRow);

void road_unlockTmcLocationIndex(TRoadTmcLocationTableIndexRow *tableIndexRow);

/// Holds an unpacked TMC location index entry.
typedef struct TRoadTmcLocationIndexRow {
  UINT16 lcd;                   ///< TMC location code

  /// @internal
  /// offset into the location segments chunk of the segment list for
  /// this location, one for each direction
  UINT32 _offset[2];

  /// @internal
  /// address of the location segments chunk for this location if it
  /// is locked, otherwise NULL
  UINT8 *_chunkAddress;
} TRoadTmcLocationIndexRow;

BOOL8 road_unsafeFindTmcLocation(
  TRoadTmcLocationIndexRow            *locationIndexRow,
  UINT16                               lcd,
  const TRoadTmcLocationTableIndexRow *tableIndexRow);

/// A segment leading to a TMC location.
typedef struct TRoadTmcLocationSegment {
  BITFIELD32 segmentId :31;     ///< segment ID

  /// 1 if segment is in reverse orientation along path
  BITFIELD32 reverse   :1;
} TRoadTmcLocationSegment;

void road_lockTmcLocationSegments(
  const TRoadTmcLocationTableIndexRow *tableIndexRow,
  TRoadTmcLocationIndexRow            *locationIndexRow);

void road_unlockTmcLocationSegments(
  const TRoadTmcLocationTableIndexRow *tableIndexRow,
  TRoadTmcLocationIndexRow            *locationIndexRow);

UINT32 road_unsafeUnpackTmcLocationSegments(
  TRoadTmcLocationSegment             *segments,
  UINT32                               maxSegments,
  const TRoadTmcLocationTableIndexRow *tableIndexRow,
  const TRoadTmcLocationIndexRow      *locationIndexRow,
  TtmcDirection                        direction);

UINT32 road_unpackTmcLocationSegments(
  TRoadTmcLocationSegment             *segments,
  UINT32                               maxSegments,
  const TRoadTmcLocationTableIndexRow *tableIndexRow,
  TRoadTmcLocationIndexRow            *locationIndexRow,
  TtmcDirection                        direction);

UINT32 road_unsafeCalculateDistanceToSegment(
  TRoadSegment          *segment,
  const TPos            *inputPos,
  TRoadSectorHeader     *sector,
  INT32                  hintOrientation,
  UINT32                 filterDistance,
  TRoadSegmentSnapPoint *snapData );

BOOL8 road_findRoadNameRange( TGenericMapInfo *roadMapInfo,
                              const UTF16     *identityName,
                              UINT32          *firstNameId,
                              UINT32          *lastNameId );

BOOL8 road_findRoadNameRange2( TGenericMapInfo *roadMapInfo,
                               const UTF16     *displayName,
                               UINT32          *firstNameId,
                               UINT32          *lastNameId );

BOOL8 road_findPlaceNameRange( TGenericMapInfo *roadMapInfo,
                               const UTF16     *identityName,
                               UINT32          *firstNameId,
                               UINT32          *lastNameId );

BOOL8 road_findPostalCodeRange( TGenericMapInfo *roadMapInfo,
                                const UTF16*     postalCode,
                                UINT32*          firstPostalCodeIndex,
                                UINT32*          lastPostalCodeIndex );

BOOL8 road_findRoadRange( TGenericMapInfo       *mapInfo,
                          TRoadRoadNameIndex32   nameStartIndex,
                          TRoadRoadNameIndex32   nameEndIndex,
                          TRoadRoadIndex32      *firstRoadIndex,
                          TRoadRoadIndex32      *lastRoadIndex );

UINT32 road_calculateHouseNumber( const TRoadHouseNumbers *houseNumbers, 
                                  TRoadSide                sideOfLine,
                                  UINT32                   proportion,
                                  TRoadSide               *foundSideOfLine );

TRoadSide road_getDrivingSide( const TGenericMapInfo *mapInfo );

BOOL8 road_shorterThanSectorResolution(const TRoadSegment *segment);

// scans all road maps and determines a bounding box that match the country name
BOOL8 road_calcMapBoundingBoxbyCountryName(TRect *bbox, TNumericISOCountryCode ISOCountryCode);

// scans all road maps and determines a bounding box that match the country code and state name
BOOL8 road_calcMapBoundingBoxbyStateName(TRect *bbox, TNumericISOCountryCode ISOCountryCode, const UTF16 *stateName);

INT32 getRoadName(const TRoadStringID *pNameID, UTF16 *name, BOOL8 *isARoute);

// -------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* ENROADP_H */
