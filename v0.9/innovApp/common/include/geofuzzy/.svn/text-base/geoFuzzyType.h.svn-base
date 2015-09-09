#ifndef INCLUDE_GUARD_geoFuzzyTypes_h
#define INCLUDE_GUARD_geoFuzzyTypes_h

#include <innovEngine_types.h>
#include <language/ConvertUTF.h>
#include <language/utf16.h>
#include <containers/gedlist.h>

const int   MAX_IN_LETTER        = 20;
const int   MAX_IN_CHARACTER     = 50;
const int   MAX_OUT_LETTER       = 100;
const int   MAX_OUT_CHARACTER    = 500;

const UINT8  MAX_IN_TEL          = 20;

//In_UINT8SearchLevel
const UINT8  SEARCH_LEVEL_ALL_AREA         = 0;
const UINT8  SEARCH_LEVEL_CITY             = 1;
const UINT8  SEARCH_LEVEL_DISTRICT         = 2;
const UINT8  SEARCH_LEVEL_ROAD             = 3;

//In_SearchType
const UINT8  SEARCH_FOR_INTERSECTION          = 1;//for intersection search
const UINT8  SEARCH_FOR_HOUSE_NUMBER          = 2;//for house number search

const UINT8  SEARCH_CHARACTER_INIT_FIRSTINPUT = 0;
const UINT8  SEARCH_CHARACTER_BY_CHAR         = 1;
const UINT8  SEARCH_CHARACTER_BY_PINYIN       = 2;
const UINT8  SEARCH_CHARACTER_BY_CHARACTER    = 3; 
const UINT8  SEARCH_CHARACTER_FILTER_BYCHAR   = 4;
const UINT8  SEARCH_ENGLISH_BY_CHAR           = 5;

const int MAX_SMART_ADDRESS                   = 5; // For addr_VoiceCommandSmartKeyboardSearch

typedef enum TGeofuzzyAreaListMode {

  GET_CITY_LIST_MODE,

  GET_DISTRICT_LIST_MODE,

  GET_AREA_MODE_MAX

} TGeofuzzyAreaListMode;

struct TWordRecord
{
  UTF16  wordValue[1];
  UINT8  phoneticIndex;
  char   Reserve[1];
};

struct TWordSet
{
  UTF16  wordValue[1];
  UINT8  bitset;
  char   Reserve[1];
};

struct APIS_ADDR_ITEM
{
  INT32   nCode;
  char    szName[128];
  wchar_t wPhoneticSymbol;
  INT32   nSubAddrCount;
  UINT32  dwVertexRecord_X;
  UINT32  dwVertexRecord_Y;  
};

struct APIS_SEARCH_CONDITION_IN
{
  wchar_t in_wpLetter[MAX_IN_LETTER];
  wchar_t in_wpCharacterSearched[MAX_IN_CHARACTER];
  BOOL8   in_bFirstSearch;   //first = TRUE ,back = TRUE
  UINT8   in_byteLantype;    // 1
  UINT8   in_byteSearchType; //5 + english
};

struct APIS_SEARCH_RESULT_OUT
{
  int     out_iLetterNum;     
  wchar_t out_wpLetter[MAX_OUT_LETTER];
  int     out_iChararcterNum; 
  wchar_t out_wpCharacter[MAX_OUT_CHARACTER];
  int     out_iResultNum; 
};

struct APIS_ADDR_ITEM_CITY_DISTRICT
{
  UTF16 szName[72];
};

struct APIS_ADDR_ITEM_ROAD
{
  UINT32  iRoadCode;
  UTF16   szName[72];
  UINT32  dwVertexRecord_X;
  UINT32  dwVertexRecord_Y;
  INT32   sectorIndex;
};

struct APIS_ROAD_RESULT_SET
{
  UINT8   acorssZoneCount;
  UINT32  iRoadCode[6];
  UINT32  iPlaceCode[6];
  INT32   roadNameIdx;
  UTF16   szName[72];
  UINT32  dwVertexRecord_X[6];
  UINT32  dwVertexRecord_Y[6];
  INT32   sectorIndex[6];
};

struct SMART_ROAD_RESULT_SET
{
  UINT16               resultCount;
  APIS_ROAD_RESULT_SET result[100];
};

struct SKZoneTbl
{
  UTF16  DCAName[72];
  char   path[128];//fullpath of smart keyboard index path
  char   roadMap[16];
};

struct  APIS_ADDR_SEARCH_BASE_CONFIG_IN
{
  UTF16  in_iCity[72];
  UTF16  in_iCityParent[72];
  UINT8  in_byteSearchLevel;
  UINT8  in_SearchType;
};

struct Smart_Search_Map
{
  struct TGenericMapInfo *mapInfo;
  char                   skIdx[128];
};

struct APIS_HOUSE_PARAMETER
{
  //UTF16 szState[36];
  UTF16  szCity[36];
  UTF16  szDistrict[36];
  UTF16  szRoadName[36];
  UTF16  szLaneName[15];
  UTF16  szAlleyName[15];
  UTF16  szHouseName[15];
  UINT32 segmentID;
};

struct APIS_HOUSE_RESULT
{
  UTF16  szRoadName[36];
  UTF16  szLaneName[15];
  UTF16  szAlleyName[15];
  UTF16  szHouseName[4][15];
  TPos   pos[4];
  UINT8  find;
  BOOL8  exactMatch;
};

struct HouseNumberHeader
{
  char   DESC[16]; 

  UINT8  majorVersion;
  UINT8  minorVersion;
  UINT8  Build;
  UINT8  Revision;

  UINT32 Date;
  UINT32 Time;

  UINT32 CountryCode;
  char   SystemName[16]; 

  UINT32 sectorFrameAddr;
  UINT32 segmentFrameAddr;
  UINT32 laneFrameAddr;
  UINT32 alleyFrameAddr;
  UINT32 houseNumberAddr;
  UINT32 nAddr;

};

typedef struct THNHeader
{
  char  backupFilename[256]; /* the full path of the backup map file */

  UINT32 sectorFrameAddr;
  UINT32 segmentFrameAddr;
  UINT32 laneFrameAddr;
  UINT32 alleyFrameAddr;
  UINT32 houseNumberAddr;
  UINT32 nAddr;
}THNHeader;

typedef struct THNSector
{
  UINT16 chunkIdx;
  UINT32 segmentOffset;
  UINT32 segmentCount;
}THNSector;

typedef struct THNSegment
{
  UINT8  segmentAttr;
  UINT32 segmentID;
  UINT32 nextLevelStartIndex;
  UINT16 nextLevelCount;
  UINT32 houseNumberRecordStartIndex;
  UINT16 houseNumberCount;
}THNSegment,HNSegment;

typedef struct THNLane
{
  UINT8  laneAttr;
  union {
    UINT16 number;
    UINT32 nameOffset;
  };
  UINT32 alleyStartOffset;
  UINT16 alleyCount;
  UINT32 houseNumberStartOffset;
  UINT16 houseNumberCount;
  TPos pos;
}THNLane;

typedef struct THNAlley
{
  UINT8  alleyAttr;
  union {
    UINT16 number;
    UINT32 nameOffset;
  };
  UINT32 houseNumberStartOffset;
  UINT16 houseNumberCount;
  TPos pos;
}THNAlley;

typedef struct THNHouseNumber
{
  UINT8  houseNumberAttr;
  union {
    UINT16 number;
    UINT32 nameOffset;
  };
  TPos pos;
}THNHouseNumber;

typedef struct THOUSEITEM {
  TDListObject dlink;   /* for the linked list */
  UTF16  szLaneName[15];
  UTF16  szAlleyName[15];
  UTF16  szHouseName[15];
  TPos pos;
}THOUSEITEM;

typedef TDList THOUSEITEMLIST;

struct APIS_ADDRESS_DATA
{
  UTF16     szRoadName[36];
  UTF16     szLaneAlleyName[99][12];
  UTF16     szHouseName[3000][12];
  TPos      pos[99];
  UINT16    laneCount;
  UINT16    alleyCount;
  UINT16    houseCount;
  INT16     letterLaneAlleyIdx;
  INT16     letterHouseIdx;
  UINT32    segmentID;
  HNSegment segmentST;
};

// For addr_VoiceCommandSmartKeyboardSearch
struct SMART_ADDRESS
{
  UTF16 addr_path[72];
  TPos  addr_pos;
};

struct SMART_ADDRESS_SET
{
  SMART_ADDRESS smart_addr[MAX_SMART_ADDRESS];
  UINT8         addr_count;
};

struct TGeofuzzyAreaNameList
{
  UTF16 areaName[500][16];
  UINT16 areaCount;  
};

struct TGeofuzzyDynamicAddressData
{
  UTF16     szName[16];
  TPos      pos;
};

#endif //INCLUDE_GUARD_geoFuzzyTypes_h