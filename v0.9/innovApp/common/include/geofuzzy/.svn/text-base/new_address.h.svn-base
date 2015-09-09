/////////////////////////////////////////////////////////////////////////////////
////// address.h
#ifndef __NEW_ADDRESS_H_
#define __NEW_ADDRESS_H_

#define  _USE_BODY_MM_  1

#include <innovEngine_types.h>
#include <innovEngine_mapdata.h>
#include <io/geio.h>
#include <language/utf16.h>
#include <language/ConvertUTF.h>
#include <language/lainit.h>
#include <mapdata/enmapinfoP.h>
#include <mapdata/enroadP.h>
#include <location/locmaplocationP.h>
#include <math/gemath.h>
#include <query/fwd.h>
#include <query/object.h>
#include <query/objectBase.h>
#include <query/objectImpl.h>

#include "geoFuzzyType.h"

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING     1

#include <vector>

#define MAX_SEARCH_MAP_COUNT        3

#define _FOR_EUP                    1

#define SymbolRecSize               12
#define IndexListSize               8
#define IndexNextCharacterSize      8

#define RESULT_POS_ARRAY_SIZE       792//99*8
#define RESULT_LANE_ALLEY_SIZE      2376//99*12*2
#define RESULT_HOUSE_NUMBER_SIZE    72000//3000*12*2
#define READ_DATA_BUFFER_SIZE       40000
#define NEXTCHAR_DATA_BUFFER_SIZE   20480//20K

#if _FOR_EUP

using namespace innov;

#pragma pack(1) 

typedef struct _tagT_SYMBOL_HEAD_
{
  UINT8  DESC[16]; 

  UINT8  majorVersion;
  UINT8  minorVersion;
  UINT8  Build;
  UINT8  Revision;

  UINT32 Date;
  UINT32 Time;

  UINT32 CountryCode;
  UINT8  SystemName[16]; 
  UINT32 DCACode;
  UINT8  DCAName[32];  

  UINT32 cityCount;
  UINT32 cityStartOffset;
  UINT32 byteSymbol_cnt;
  UINT32 dwSymbol_StartOffset;
  UINT32 dwIndexList_cnt;
  UINT32 dwIndexList_StartOffset;
  UINT32 dwIndexNextCharacter_cnt;
  UINT32 dwIndexNextCharacter_StartOffset;
  UINT32 dwRowNumber_cnt;
  UINT32 dwRowNumber_StartOffset;

  UINT32 enhanced_info_cnt;
  UINT32 enhanced_info_offset;
  UINT32 enhanced_name_cnt;
  UINT32 enhanced_name_offset;

}T_SYMBOL_HEAD;

typedef struct _tagT_RANGE_REC_
{
  UINT32 wNameOffset;
  UINT32 startRowNumIndex;
  UINT32 endRowNumIndex;
  UINT8  areaLevel;
  UINT16 parentRow;

}T_RANGE_REC;

typedef struct _tagT_SYMBOL_REC_
{
  wchar_t  wPhonetic_Symbol;
  UINT32   dwStart_offset;
  UINT16   wLevel_State_cnt;
  UINT16   wLevel_City_cnt;
  UINT16   wLevel_District_cnt;
  UINT32   wLevel_Road_cnt;

}T_SYMBOL_REC;


typedef struct _tagT_INDEX_LIST_ 
{
  UINT8    byteLevel;
  wchar_t  wWord_value;
  UINT32   dwStartIdx;
  UINT8    reserve;

}T_INDEX_LIST;

typedef struct _tagT_INDEX_NEXTCHARACTER_ 
{  
  wchar_t  wNext_word_value;
  UINT32   dwRecord_idx;
  UINT8    byteWord_idx;
  UINT8    byteNext_symbol_idx;

}T_INDEX_NEXTCHARACTER;

typedef struct _tagADDRESS_IDX_
{
  UINT32   dwRecord_idx;
  wchar_t  wNext_word_value;
  UINT8    byteWord_idx;

  _tagADDRESS_IDX_(){}
  _tagADDRESS_IDX_(UINT32 _dwRecord_idx, wchar_t _wNext_word_value, UINT8  _byteWord_idx)
  {
    dwRecord_idx     = _dwRecord_idx;
    wNext_word_value = _wNext_word_value; 
    byteWord_idx     = _byteWord_idx;
  }

  bool operator < (_tagADDRESS_IDX_& x)
  {
    return wNext_word_value < x.wNext_word_value;
  }

}ADDRESS_IDX;

typedef struct _tagADDRESS_LE_CHAR
{
  wchar_t wLetter;
  wchar_t wCharacter;

  _tagADDRESS_LE_CHAR(){}
  _tagADDRESS_LE_CHAR(wchar_t _wLetter,wchar_t _wCharacter)
  {
    wLetter    = _wLetter;
    wCharacter = _wCharacter;
  }

  bool operator < (_tagADDRESS_LE_CHAR& x)
  {
    return wLetter < x.wLetter;
  }

}ADDRESS_LET_CHAR;

typedef struct _SMAPC_
{
  wchar_t wWord_value;
  UINT8   symbol_idx;

}SMAPC;

typedef struct TGeofuzzyTmpLetterData
{
  UTF16 szName[12];
  TPos  tPos;

} TGeofuzzyTmpLetterData;

template <class DataType>
class TObject: public query::CObjectBase,
               public query::IObject {
public:
public:
  struct TParameters {
    DataType *_object;
  };

public:
  DataType _object;

public:
  TObject<DataType>( IAllocator*        allocator,
                     const TParameters& parameters,
                     query::TStatus&    status )
    : CObjectBase( allocator ) {
      if (parameters._object) {
        _object = *(parameters._object);
      } else {
        memset(&_object, 0, sizeof(DataType));
      }
  }

public:
  static
  query::TStatus
  make( IAllocator*          allocator,
        DataType*            data,
        TObject<DataType> ** object)
  {
    typedef query::CObjectImpl <TObject<DataType>> ObjectType;

    ObjectType::TParameters parameters = { data };

    return ObjectType::make( allocator,
                             parameters,
                             object );
  }
};

typedef TObject<THNHeader> CHNHeader;
typedef TObject<THNSector> CHNSector;
typedef TObject<THNSegment> CHNSegment;
typedef TObject<THNLane> CHNLane;
typedef TObject<THNAlley> CHNAlley;
typedef TObject<THNHouseNumber> CHNHouseNumber;

typedef struct THNHandle
{
  UINT8 flag;  /*
               0 - header
               1 - sector
               2 - segment
               3 - lane
               4 - alley
               5 - housenumber
               */
  CHNHeader *hnHeader;
  CHNSector *hnSector;
  CHNSegment *hnSegment;
  CHNLane *hnLane;
  CHNAlley *hnAllay;
  CHNHouseNumber *hnHouse;

  THNHandle()
    : hnHeader(0),
    hnSector(0),
    hnSegment(0),
    hnLane(0),
    hnAllay(0),
    hnHouse(0) {}

  void operator=(const THNHandle & obj) {
    flag = obj.flag;
    hnHeader = obj.hnHeader;
    if (hnHeader) {
      hnHeader->AddRef();
    }
    hnSector = obj.hnSector;
    if (hnSector) {
      hnSector->AddRef();
    }
    hnSegment = obj.hnSegment;
    if (hnSegment) {
      hnSegment->AddRef();
    }
    hnLane = obj.hnLane;
    if (hnLane) {
      hnLane->AddRef();
    }
    hnAllay = obj.hnAllay;
    if (hnAllay) {
      hnAllay->AddRef();
    }
    hnHouse = obj.hnHouse;
    if (hnHouse) {
      hnHouse->AddRef();
    }
  }

  ~THNHandle() {
    if (hnHeader) {
      hnHeader->Release();
    }
    if (hnSector) {
      hnSector->Release();
    }
    if (hnSegment) {
      hnSegment->Release();
    }
    if (hnLane) {
      hnLane->Release();
    }
    if (hnAllay) {
      hnAllay->Release();
    }
    if (hnHouse) {
      hnHouse->Release();
    }
  }
}THNHandle;

typedef TObject<THNHandle> CHNHandle;

#pragma pack() 

#endif

class CAddress
{
public:
  FILE                  *m_fmSmartKeyboard;
  UINT8                 m_pBegin[READ_DATA_BUFFER_SIZE];
  UINT8                 *m_pNextChar;

  Smart_Search_Map      ssMap[MAX_SEARCH_MAP_COUNT];//the smart search map data will be search
  UINT8                 ssMapCount;
  UINT8                 searchingIndex;
  UTF16                 searchCityName[72];
  UTF16                 searchCityParent[72];

  T_SYMBOL_HEAD         m_index_Symbol_Head;
  T_SYMBOL_REC          *m_index_Symbo_Rec;
  T_INDEX_LIST          *m_index_List;
  T_INDEX_NEXTCHARACTER *m_index_Nextcharacter;


  UINT8                 m_byteSymbolCount;  
  INT32                 m_index_List_Num;
  INT32                 m_index_Nextcharacter_Num;

  INT32                 m_iLetterNum[MAX_SEARCH_MAP_COUNT];
  INT32                 m_iSearchCharNum[MAX_SEARCH_MAP_COUNT];
  INT32                 m_iSearchAddressNum[MAX_SEARCH_MAP_COUNT];

  wchar_t               m_wpLetter_Out[MAX_SEARCH_MAP_COUNT][MAX_OUT_LETTER];
  wchar_t               m_wpChar_Out[MAX_SEARCH_MAP_COUNT][MAX_OUT_CHARACTER][1];
  wchar_t               m_wpSearchChar_In[MAX_IN_CHARACTER];

  INT32                 iNum[MAX_SEARCH_MAP_COUNT];
  UINT16                searchState;
  UINT8                 m_bFirstSearchC;

public:
  CAddress();

  ~CAddress();

  BOOL8   LoadSmartKeyboardTbl();

  BOOL8   CloseTbl();

  void    ResetConf();

  void    SetWcharName(wchar_t wsearchWChr);//Set first letter
  
  //set level: SEARCH_LEVEL_DISTRICT , SEARCH_LEVEL_ROAD
  void    SetSearchLevel(APIS_ADDR_SEARCH_BASE_CONFIG_IN &in_BaseConfig);

  void    SetisFirstSearch(BOOL8 bFirstSearch);

  BOOL8   SetSearchAdress(UTF16 *in_iCity);//for search road

  UINT8   GetSearchLevel();//get level: SEARCH_LEVEL_DISTRICT , SEARCH_LEVEL_ROAD

  INT32   GetAllLetter();//Get all first letter 

  INT32   GetChrName();//get Character by first letter

  INT32   StartSearchByName();// start search

  INT32   SearchByName_AllArea();//search all place

  INT32   SearchByName_City();//search city

  INT32   SearchByName_District();//search district

  void    SearchNextCharacter_District(INT32 iCityStartIdx,
                                       INT32 iCityEndIdx,
                                       INT32 iDistrictStartIdx,
                                       INT32 iDistrictEndIdx, 
                                       BOOL8 bRecordBodyIdx);

  INT32   SearchByName_Road();//search Road

  void    SearchNextCharacter_Road(INT32 iRoadStartIdx,
                                   INT32 iRoadEndIdx, 
                                   BOOL8 bRecordBodyIdx);

  BOOL8   FilterCharacter_byLetter();//filter character searched by first leter.

  BOOL8   GetItemCityDistrict(APIS_ADDR_ITEM_CITY_DISTRICT &_info, BOOL8 bFlag);

  BOOL8   GetItemRoad(APIS_ADDR_ITEM_ROAD &_info, BOOL8 bFlag);

  UINT16  GetRoadResultSet(SMART_ROAD_RESULT_SET &_info);

  UINT32  CreateZoneTbl();//create a table of every dca's smart keyboard idx

  BOOL8   SearchSKMap(UTF16 *stateName);//search the idx from zone table by state name

  BOOL8   checkSKZoneTbl();

  BOOL8   GetSearchRoadRange(UTF16 *aCity, UINT32 &startRow, UINT32 &endRow);

  BOOL8   GetSearchPlaceRange(UTF16 *aCity, UINT8 searchLevel);

  INT16   getBracketPosition(UTF16 *aName);

  UINT8   TransferInput(UTF16 input, UTF16 *output);

  BOOL8   GetSearchAllAreaRange();

  //for house number search----------------------------------------------------------
  BOOL8   startPointHouseNumberSearch(APIS_HOUSE_PARAMETER &parameter, 
                                      APIS_HOUSE_RESULT    &result);

  BOOL8   GetHouseNumber(APIS_HOUSE_PARAMETER &parameter, 
                         APIS_HOUSE_RESULT    &result, 
                         UINT32               houseNumberStartAddr, 
                         UINT16               houseNumberCount);

  BOOL8   GetSimilarHouseNumber(APIS_HOUSE_PARAMETER &parameter, 
                                APIS_HOUSE_RESULT    &result, 
                                UINT32               offset, 
                                UINT16               houseNumberCount);

  UINT8   DirectGet4HouseNumber(APIS_HOUSE_PARAMETER &parameter,
                                APIS_HOUSE_RESULT    &result, 
                                UINT32               houseNumberStartAddr, 
                                UINT16               houseNumberCount);

  UINT8   isLetterType(UTF16 *name);//0:digit / 1:letter

  UINT8   GetLetterName(UINT32 offset, UTF16 *letter);

  void    SetMin(INT32 cmp,INT32 index);

  BOOL8   GetRoadPos(UINT32 roadOffset, TPos &pos);

  BOOL8   checkAddressData(APIS_HOUSE_PARAMETER &parameter); 

  BOOL8   getRoadAddressData(APIS_HOUSE_PARAMETER &parameter, 
                             APIS_ADDRESS_DATA    &result, 
                             BOOL8                anyInput);

  BOOL8   clearData(APIS_ADDRESS_DATA &result);

  INT8    SearchHNMap(UTF16 *stateName);//search the idx by state name, search faile if return -1

  BOOL8   openHouseNumberMapdata(UINT8 *basePath1, UINT8 *basePath2);

  void    parseHouseNumberMapHeader(void);

  void    getPosfromHouseNnumberMap(UINT32 startAddress, TPos &pos);

  void    setHNRoadMapIdx(UINT32 mapIdx);

  BOOL8   getAddressDataCount(APIS_HOUSE_PARAMETER     parameter, 
                              BOOL8                    charData,
                              UINT16                  *laneCount,
                              UINT16                  *alleyCount,
                              UINT16                  *houseCount);

  BOOL8   getAddressData(UINT16                        count,  
                         UINT8                         dataType,
                         TGeofuzzyDynamicAddressData   data[]);

  void    clearAddressData(void);

  INT8    scanHNMapbyPlace(UTF16 *cityName, UTF16 *districtName);
  //--------------------------------------------------------------------------------

  BOOL8   VoiceCommandSmartKeyboardSearch(const UTF16       *queryString, 
                                          SMART_ADDRESS_SET *addrSet);

  BOOL8   getAreaNameList(UTF16                        *stateName,
                          UTF16                        *parentAreaName,
                          TGeofuzzyAreaListMode         getAreaMode,
                          struct TGeofuzzyAreaNameList *areaList);

  /************************************************************************/
  /* for show house number on map view                                                                      */
  /************************************************************************/
  INT32 getHouseMapIdx(const UINT32 roadMapIdx);
  BOOL8 getHouseNumberHeader(const char* filename, CHNHeader &header);
  BOOL8 getSector(const CHNHeader *hnHeader, const UINT32 roadMapIdx, const UINT32 roadSegmentID, CHNSector &sector);
  BOOL8 getSegment(const CHNHeader *hnHeader, const CHNSector *hnSector, const UINT32 segmentOffset, CHNSegment &segment);
  BOOL8 getLaneFromSegment(const CHNHeader *hnHeader, const CHNSegment *hnSegment, std::vector<CHNLane*>&lanes);
  BOOL8 getAlleyFromSegment(const CHNHeader *hnHeader, const CHNSegment *hnSegment, std::vector<CHNAlley*>&alleys);
  BOOL8 getAlleyFromLane(const CHNHeader *hnHeader, const CHNLane *hnLane, std::vector<CHNAlley*>&alleys);
  BOOL8 getAlley(const CHNHeader *hnHeader, const UINT32 alleyCnt, const UINT32 alleyStartOffset, std::vector<CHNAlley*>&alleys);
  BOOL8 getHouseNumberFromSegment(const CHNHeader *hnHeader, const CHNSegment *hnSegment, std::vector<CHNHouseNumber*>&houseNumbers);
  BOOL8 getHouseNumberFromLane(const CHNHeader *hnHeader, const CHNLane *hnLane, std::vector<CHNHouseNumber*>&houseNumbers);
  BOOL8 getHouseNumberFromAlley(const CHNHeader *hnHeader, const CHNAlley *hnAlley, std::vector<CHNHouseNumber*>&houseNumbers);
  BOOL8 getHouseNumber(const CHNHeader *hnHeader, const UINT32 houseNumberCnt, const UINT32 houseNumberStartOffset, std::vector<CHNHouseNumber*>&houseNumbers);
  BOOL8 getHouseNumber(const CHNHandle *hnHandle, const TRect searchBounds, THOUSEITEMLIST &results);
  BOOL8 getLaneName(const CHNLane *hnLane, UTF16 *str, const UINT32 size);
  BOOL8 getAlleyName(const CHNAlley *hnAlley, UTF16 *str, const UINT32 size);
  BOOL8 getHouseName(const CHNHouseNumber *hnHouse, UTF16 *str, const UINT32 size);
  BOOL8 getNameFromOffset(const UINT32 nameOffset, UTF16 *name, const UINT32 size);
  BOOL8 getHouseNumberItems(const UINT32 roadMapIdx, const UINT32 roadSegmentID, TRect searchBounds, THOUSEITEMLIST &results);
  BOOL8 freeHouseNumberItems(const THOUSEITEMLIST &items);
  const THOUSEITEM* getHouseItemListBegin(const THOUSEITEMLIST &items);
  const THOUSEITEM* getHouseItemListEnd(const THOUSEITEMLIST &items);
  const THOUSEITEM* getHouseItemNext(const THOUSEITEM* item);
  const THOUSEITEM* getHouseItemPrevious(const THOUSEITEM* item);

private:
  wchar_t         m_searchWChr;//first letter
  UINT8           m_byteSearchLevel;//SEARCH_LEVEL_DISTRICT , SEARCH_LEVEL_ROAD
  UINT8           m_SearchType;//SEARCH_FOR_INTERSECTION , SEARCH_FOR_HOUSE_NUMBER
  BOOL8           m_bFirstSearch;

  UINT32          m_iMaxIdx;   //for search Road   count by m_byteState,m_iCity,m_iDistrict
  UINT32          m_iMinIdx;   //for search Road   count by m_byteState,m_iCity,m_iDistrict
  UINT32          m_iMaxIdxArea;
  UINT32          m_iMinIdxArea;

  INT32           m_iCurrentItem_District;  
  INT32           m_iCurrentItem_Road;

  INT32           c1, c2, c3, c4, c5, c6;

  INT32           m_iSearchNum_City;
  INT32           m_iSearchNum_District;//plus district
  INT32           m_iSearchNum_Road;

  UINT32          m_dwDistrictMin_X;
  UINT32          m_dwDistrictMin_Y;

  TGenericMapInfo *mapInfo;
  UINT32          mapCount;
  UINT8           skIdx[128];
  UTF16           cityName[72];
  BOOL8           iCreateTbl;
  UINT32          resultRecord[100][2];
  UINT32          resultCount;

  BOOL8           flagState;
  BOOL8           voiceMode;//blake test

  BOOL8           mergeRoadResult;
  UINT8           cityLevel;
  UTF16           hnState[72];
  UINT8           smartMinorVersion;

  //for house number search
  HouseNumberHeader iHNHeader;
  FILE              *houseNumber;
  FILE              *houseNameTbl;
  INT32             similarMin[4];
  INT32             similarMinIndex[4];
  UINT32            houseNumberTotalSize;
  UINT32            houseNameTblTotalSize;
  INT8              houseMapdataIdx;
  UINT32            hnRoadMapIdx;

  TGeofuzzyDynamicAddressData  *dLaneData;
  TGeofuzzyDynamicAddressData  *dAlleyData;
  TGeofuzzyDynamicAddressData  *dHouseData;
  UINT16                        dLaneCount;
  UINT16                        dAlleyCount;
  UINT16                        dHouseCount;

  BOOL8             numberSign;

private:
  //GetChrName() will call  FindChar_New() get all character by first letter
  INT32  FindChar_New();
  
  //find start index and end index in m_index_Nextcharacter by Character selected.
  BOOL8  FindSelectedChar(wchar_t wFirstCharacter,
                          INT32   &iCityStartIdx,
                          INT32   &iCityEndIdx,
                          INT32   &iDistrictStartIdx,
                          INT32   &iDistrictEndIdx,
                          INT32   &iRoadStartIdx,
                          INT32   &iRoadEndIdx);
        
  //find the index pos in m_index_List by Character
  INT32  FindCharPostion(wchar_t wFirstCharacter, INT32 iStartIdx, INT32 iEndIdx);
        
  //if the index of Symbol and Character is known,call it can get find start index and end index in m_index_Nextcharacter.
  BOOL8  FindSelectedCharbyIndex_New(UINT8   byteIndex,
                                     wchar_t wFirstCharacter,
                                     INT32   &iCityStartIdx,
                                     INT32   &iCityEndIdx,
                                     INT32   &iDistrictStartIdx,
                                     INT32   &iDistrictEndIdx,
                                     INT32   &iRoadStartIdx,
                                     INT32   &iRoadEndIdx);
        
  UINT16 GetAreaRow(UTF16 *aArea, TGeofuzzyAreaListMode getAreaMode, BOOL8 loadMap);

  BOOL8  GetSearchAreaRange(UINT16 areaRow, UINT8 searchLevel, UINT32 &startRow, UINT32 &endRow);

  UINT8  GetDistrictName(UINT32 recordIdx, 
                         UTF16  *aName, 
                         INT32  &placeIdx, 
                         INT32  &parentRow);

  INT32  GetSegmentName(UINT32 segment, UTF16 *segmentName);
};
#endif
