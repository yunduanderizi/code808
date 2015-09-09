#ifndef TRLOCTABLEP_H
#define TRLOCTABLEP_H

#ifndef TRAFFIC_ENCODER
#include <language/utf16.h>
#include <innovEngine_types.h>
#else
#include <common/types.h>
#include <common/utf16.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
  This file is used both in the traffic encoder and in mapcore.  The
  two environments have different ideas about what the boolean type
  should be called.
*/

#ifndef TRAFFIC_ENCODER
#define BOOL BOOL8
#endif

//////////////////////////////////////////////////////////////////////
// Location categories, types and subtypes

typedef enum TtmcLocationCategory {
  TMC_LOCATION_CATEGORY_AREA,
  TMC_LOCATION_CATEGORY_LINEAR,
  TMC_LOCATION_CATEGORY_POINT
} TtmcLocationCategory;

typedef UINT16 TtmcLocationTypeCode;

TtmcLocationCategory tmcLocationTypeCode_category(TtmcLocationTypeCode code);
UINT8 tmcLocationTypeCode_type(TtmcLocationTypeCode code);
UINT8 tmcLocationTypeCode_subtype(TtmcLocationTypeCode code);
TtmcLocationTypeCode tmcLocationTypeCode_pack(TtmcLocationCategory category,
                                              UINT8 type,
                                              UINT8 subtype);

//////////////////////////////////////////////////////////////////////
// Directions

typedef enum {
  TMC_LOCATION_DIRECTION_NONE,
  TMC_LOCATION_DIRECTION_NORTH,
  TMC_LOCATION_DIRECTION_SOUTH,
  TMC_LOCATION_DIRECTION_WEST,
  TMC_LOCATION_DIRECTION_EAST,
  TMC_LOCATION_DIRECTION_CLOCKWISE,
  TMC_LOCATION_DIRECTION_ANTICLOCKWISE
} TtmcDirectionName;

TtmcDirectionName tmcLocationDirection_reverse(TtmcDirectionName dir);

//////////////////////////////////////////////////////////////////////
// Locations

#define TRAFFIC_FILE_NUM_LAYERS 2

typedef UINT16 TtmcLocationCode;

/**
 * \brief the value used to indicate that there is no such location
 *
 * This is used as the value of \c posOffset or \c negOffset when a
 * location is the last or first in its chain, respectively, and for
 * \c areaRef and \c linearRef when a location doesn't have an area or
 * linear reference, respectively.
 */
#define TRAFFIC_FILE_NO_LOCATION 0xFFFF

typedef struct TtmcLocationInfo {
  TtmcLocationTypeCode type;
  UINT16 posOffset; ///< index of next location in the chain
  UINT16 negOffset; ///< index of previous location in the chain
  UINT16 areaRef;   ///< index of containing area location
  UINT16 linearRef; ///< index of containing linear location
  INT32 latitude;
  INT32 longitude;
  UINT32 roadNumberOffset;
  UINT32 roadNameOffset;
  UINT32 firstNameOffset;
  UINT32 secondNameOffset;
  TtmcDirectionName posDirectionName;
} TtmcLocationInfo;

/*
  This is the size as it appears in the binary file, not necessarily
  in memory.  TRAFFIC_FILE_SIZEOF_LOCATION_INFO may not be equal
  to sizeof(TtmcLocationInfo) due to padding for alignment.
*/
#define TRAFFIC_FILE_SIZEOF_LOCATION_INFO (     \
    2 +                                         \
    2 +                                         \
    2 +                                         \
    2 +                                         \
    2 +                                         \
    4 +                                         \
    4 +                                         \
    4 +                                         \
    4 +                                         \
    4 +                                         \
    4 +                                         \
    1 +                                         \
    0)


/// The maximum length of a text string in characters.
#define TRAFFIC_FILE_MAX_TEXT_LENGTH 100

/// The maximum length of a text string in bytes.
#define TRAFFIC_FILE_MAX_TEXT_BYTES 255

//////////////////////////////////////////////////////////////////////
// Location formatting operations

typedef enum {
  TRAFFIC_LOCATION_FORMAT_NULL,
  TRAFFIC_LOCATION_FORMAT_COALESCE,
  TRAFFIC_LOCATION_FORMAT_JOIN,
  TRAFFIC_LOCATION_FORMAT_ROAD_NUMBER,
  TRAFFIC_LOCATION_FORMAT_ROAD_NAME,
  TRAFFIC_LOCATION_FORMAT_POS_DIR_NAME,
  TRAFFIC_LOCATION_FORMAT_NEG_DIR_NAME,
  TRAFFIC_LOCATION_FORMAT_PRIM_FIRST_NAME,
  TRAFFIC_LOCATION_FORMAT_PRIM_SECOND_NAME,
  TRAFFIC_LOCATION_FORMAT_SEC_FIRST_NAME,
  TRAFFIC_LOCATION_FORMAT_SEC_SECOND_NAME,
  TRAFFIC_LOCATION_FORMAT_NUM_OPS
} TtmcLocationFormatOpType;

typedef struct TtmcLocationFormatOp {
  UINT8 opType; // TtmcLocationFormatOpType
  UINT16 nextOp;
} TtmcLocationFormatOp;

#define BASE_OP(op) ((TtmcLocationFormatOp *)op)

#define TRAFFIC_LOCATION_FORMAT_SIZEOF_BASE_OP (        \
    1 +                                                 \
    2 +                                                 \
    0)

typedef struct TtmcLocationFormatNullOp {
  TtmcLocationFormatOp parent;
} TtmcLocationFormatNullOp;

typedef struct TtmcLocationFormatArgsOp {
  TtmcLocationFormatOp parent;
  UINT8 numArgs;
} TtmcLocationFormatArgsOp;

#define ARGS_OP(op) ((TtmcLocationFormatArgsOp *)op)

#define TRAFFIC_LOCATION_FORMAT_SIZEOF_ARGS_OP (        \
    TRAFFIC_LOCATION_FORMAT_SIZEOF_BASE_OP +            \
    1 +                                                 \
    0)

typedef struct TtmcLocationFormatCoalesceOp {
  TtmcLocationFormatArgsOp parent;
} TtmcLocationFormatCoalesceOp;

typedef enum {
  TRAFFIC_LOCATION_FORMAT_JOIN_ONNULL_SKIP,
  TRAFFIC_LOCATION_FORMAT_JOIN_ONNULL_EMPTY
} TtmcLocationFormatJoinOnNull;

#define TRAFFIC_LOCATION_FORMAT_JOIN_MAX_WITH_LENGTH 5

typedef struct TtmcLocationFormatJoinOp {
  TtmcLocationFormatArgsOp parent;
  UTF16 with[TRAFFIC_LOCATION_FORMAT_JOIN_MAX_WITH_LENGTH+1];
  UINT8 onNull; // TtmcLocationFormatJoinOnNull
} TtmcLocationFormatJoinOp;

#define TRAFFIC_LOCATION_FORMAT_SIZEOF_JOIN_OP (                \
    TRAFFIC_LOCATION_FORMAT_SIZEOF_ARGS_OP +                    \
    (TRAFFIC_LOCATION_FORMAT_JOIN_MAX_WITH_LENGTH+1) * 2 +      \
    1 +                                                         \
    0)

typedef struct TtmcLocationFormatNameOp {
  TtmcLocationFormatOp parent;
} TtmcLocationFormatNameOp;

typedef union TtmcLocationFormatAnyOp {
  TtmcLocationFormatOp op;
  TtmcLocationFormatNullOp null;
  TtmcLocationFormatCoalesceOp coalesce;
  TtmcLocationFormatJoinOp join;
  TtmcLocationFormatNameOp name;
} TtmcLocationFormatAnyOp;

#define TRAFFIC_LOCATION_FORMAT_SIZEOF_OP TRAFFIC_LOCATION_FORMAT_SIZEOF_JOIN_OP

//////////////////////////////////////////////////////////////////////
// Location formatting rules

#define TRAFFIC_LOCATION_FORMAT_MAX_PROGRAM_LENGTH 10

typedef struct TtmcLocationFormatProgram {
  UINT8 numOps;
  TtmcLocationFormatAnyOp ops[TRAFFIC_LOCATION_FORMAT_MAX_PROGRAM_LENGTH];
} TtmcLocationFormatProgram;

typedef enum {
  TRAFFIC_LOCATION_FORMAT_RULE_DIR_ANY,
  TRAFFIC_LOCATION_FORMAT_RULE_DIR_POS,
  TRAFFIC_LOCATION_FORMAT_RULE_DIR_NEG,
  TRAFFIC_LOCATION_FORMAT_RULE_DIR_BIDIR
} TtmcLocationFormatRuleDir;

#define TRAFFIC_LOCATION_FORMAT_NUM_DIRS 3

typedef struct TtmcLocationFormatRules {
  TtmcLocationFormatProgram posProgram;
  TtmcLocationFormatProgram negProgram;
  TtmcLocationFormatProgram bidirProgram;
} TtmcLocationFormatRules;

typedef enum {
  TRAFFIC_LOCATION_FORMAT_ROUTE_NAME        = 0,
  TRAFFIC_LOCATION_FORMAT_ROUTE_TEXT        = 1,
  TRAFFIC_LOCATION_FORMAT_NUM_PARTS         = 2,
  TRAFFIC_LOCATION_FORMAT_LOCATION_TEXT     = 2, // not currently used
  TRAFFIC_LOCATION_FORMAT_NUM_PARTS_IN_FILE = 3
} TtmcLocationFormatPart;

typedef struct TtmcLocationFormat {
  TtmcLocationFormatRules routeNameRules;
  TtmcLocationFormatRules routeTextRules;
  TtmcLocationFormatRules locationTextRules;
} TtmcLocationFormat;

const TtmcLocationFormatProgram *locationFormat_findProgram(
  const TtmcLocationFormat *format,
  TtmcLocationFormatRuleDir dir,
  TtmcLocationFormatPart part);

//////////////////////////////////////////////////////////////////////
// Road name sorting rules

#define TRAFFIC_ROAD_NAME_MAX_NUM_RULES 32U
#define TRAFFIC_ROAD_NAME_MAX_PREFIX_LENGTH 4U

typedef struct TtmcRoadNameSortRule {
  UTF16 prefix[TRAFFIC_ROAD_NAME_MAX_PREFIX_LENGTH];
} TtmcRoadNameSortRule;

#define TRAFFIC_ROAD_NAME_SORT_SIZEOF_RULE (            \
    2 * TRAFFIC_ROAD_NAME_MAX_PREFIX_LENGTH +           \
    0)

typedef struct TtmcRoadNameSortRules {
  UINT8 numRules;
  TtmcRoadNameSortRule rules[TRAFFIC_ROAD_NAME_MAX_NUM_RULES];
} TtmcRoadNameSortRules;

//////////////////////////////////////////////////////////////////////
// Header

#define TRAFFIC_FILE_TABLE_VERSION_LENGTH 10
#define TRAFFIC_FILE_MAX_NUM_LOCATIONS 63488

typedef struct TtmcLocationFileHeader {
  char mapCountryCode[2];
  UINT8 countryCode;            // 4 bits
  UINT8 locationTableNumber;    // 6 bits
  char locationTableVersion[TRAFFIC_FILE_TABLE_VERSION_LENGTH];
  UINT16 numLocations;
  UINT8 locationFormatProgramLengths[TRAFFIC_LOCATION_FORMAT_NUM_PARTS_IN_FILE]
                                    [TRAFFIC_LOCATION_FORMAT_NUM_DIRS];
  UINT8 numRoadNameSortRules;
} TtmcLocationFileHeader;

/*
  This is the size as it appears in the binary file, not necessarily
  in memory.  TRAFFIC_FILE_SIZEOF_HEADER may not be equal to
  sizeof(TtmcLocationFileHeader) due to padding for alignment.
*/
#define TRAFFIC_FILE_SIZEOF_HEADER (                    \
    2 +                                                 \
    1 +                                                 \
    1 +                                                 \
    TRAFFIC_FILE_TABLE_VERSION_LENGTH +                 \
    2 +                                                 \
    (TRAFFIC_LOCATION_FORMAT_NUM_PARTS_IN_FILE *        \
     TRAFFIC_LOCATION_FORMAT_NUM_DIRS) +                \
    1 +                                                 \
    0)

//////////////////////////////////////////////////////////////////////
// Location code index

typedef UINT16 TtmcLocationFileLocationCodes[TRAFFIC_FILE_MAX_NUM_LOCATIONS];

#define TRAFFIC_FILE_SIZEOF_LOCATIONCODE 2U
#define TRAFFIC_FILE_MAX_SIZEOF_LOCATIONCODES ( \
    (TRAFFIC_FILE_SIZEOF_LOCATIONCODE *         \
     TRAFFIC_FILE_MAX_NUM_LOCATIONS) +          \
    0)

//////////////////////////////////////////////////////////////////////
// Generic parameters (not used)

/*
typedef enum {
  TRAFFIC_FILE_PARAMETER_UINT8,
  TRAFFIC_FILE_PARAMETER_UINT16,
  TRAFFIC_FILE_PARAMETER_UINT32,
  TRAFFIC_FILE_PARAMETER_VARIABLE
} TtmcLocationFileParameterType;

typedef struct TtmcLocationFileParameterHeader {
  TtmcLocationFileParameterType type;
  UINT16 label;
} TtmcLocationFileParameter;

typedef struct TtmcLocationFileVariableParameter {
  UINT16 length;
  UINT8 data[TRAFFIC_FILE_MAX_VARIABLE_PARAMETER_LENGTH];
} TtmcLocationFileVariableParameter;

typedef union TtmcLocationFileParameterValue {
  UINT8 uint8;
  UINT16 uint16;
  UINT32 uint32;
  TtmcLocationFileVariableParameter variable;
} TtmcLocationFileParameterValue;

typedef TtmcLocationFileParameter TtmcLocationFileParameters[TRAFFIC_FILE_MAX_NUM_PARAMETERS];
*/

//////////////////////////////////////////////////////////////////////
// Functions

BOOL tmcLocationFile_getFileName(
  const char mapCountryCode[2],
  UINT8 countryCode,
  UINT8 locationTableNumber,
  const char version[2],
  char *buffer, UINT16 bufferSize);

BOOL tmcLocation_getPosition(const TtmcLocationInfo *location, TPos *pos);

#ifdef __cplusplus
}
#endif

#endif /* TRLOCTABLEP_H */
