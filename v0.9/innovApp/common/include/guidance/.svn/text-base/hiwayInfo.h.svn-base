#ifndef HIWAYINFO_H
#define HIWAYINFO_H

#include <innovEngine_types.h>
#include <routing/rtpath.h>
#include <guidance/gdlist.h>
#include <mapdata/enmfile.h>
//#include <mapdata/enroadP.h>
//#include <math/mtangleP.h>

#ifdef __cplusplus
extern "C" {
#endif


//#define MIN_VERTEX_SEPARATION 3

//#define MIN_VERTEX_SEPARATION_FREEWAY 5
#define MAX_HIGHWAY_INFO_STR		64
#define HIGHWAY_INFO_KIND_IC		0
#define HIGHWAY_INFO_KIND_JC		1
#define HIGHWAY_INFO_KIND_REST		2
#define HIGHWAY_INFO_KIND_TOLL	 	3
#define HIGHWAY_INFO_KIND_PARK	 	4
#define HIGHWAY_INFO_KIND_MILE		5
#pragma pack(1)

typedef struct {
  wchar_t   szText[MAX_HIGHWAY_INFO_STR];
  INT32     pathIdx;
  INT32     kind;	
  INT32	    distFromStart;
  INT32	    distFromSegment;
} THighWayInfo;

typedef struct {
  THighWayInfo  *pHighwayInfo;
  UINT32        numHighwayInfo;
} THighWayInfoList;

typedef struct {
  THighWayInfoList list[ITINERARY_MAX_DESTINATIONS];
  UINT32           bufferSize[ITINERARY_MAX_DESTINATIONS];
} THighWayList;

typedef struct LINK_MAPPING_HEADER {
  UINT32   magic:16;
  UINT32   bundleMap:4;
  UINT32   mapType:4;
  UINT32   layoutVersion:8;
  UINT8    majorVersion;
  UINT8    minorVersion;
  UINT16   spare;
  UINT32   epoch;
  UINT32   langNum;
  UINT32   dcaOff;
  UINT32   sectorOff;
  UINT32   segmentOff;
  UINT32   facilityOff;
} LINK_MAPPING_HEADER;

typedef struct DCA_Frame {
  char     DCAName[16];
  UINT32   sectorOff;
  UINT32   sectorNum;
} DCA_Frame;

typedef struct Sector_Frame {
  UINT32   sectorID;
  UINT32   segOff;
  UINT32   segNum;
} Sector_Frame;

typedef struct Segment_Info {
  UINT16   dirFlag   :1;
  UINT16   pathID    :15;
} Segment_Info;

typedef struct Segment_Frame {
  UINT32          segID;
  Segment_Info    segInfo;
  UINT32          facilityOff;
  UINT16          nextDist;
  UINT16          segtype;
} Segment_Frame;

typedef struct Highway_Path_Frame {
  UINT32   facilityStartadd;
  UINT32   facilityEndadd;
} Highway_Path_Frame;

typedef struct Toll_Gate_Info {
  UINT16   flagHeavyMotor  :1;
  UINT16   flagMinicar     :1;
  UINT16   flagCompactcar  :1;
  UINT16   flagMincar      :1;
  UINT16   flagOsCar       :1;
  UINT16   flagOsCargo     :1;
  UINT16   flagSpeCargo    :1;
  UINT16   reserved_1      :1;
  UINT16   flagETC         :1;
  UINT16   flagTicket      :1;
  UINT16   flagDollar      :1;
  UINT16   reserved_2      :5;
} Toll_Gate_Info;

typedef struct Service_Area_Info {
  UINT16   flagParklot     :1;
  UINT16   flagGasStation  :1;
  UINT16   flagRestaurant  :1;
  UINT16   flagATM         :1;
  UINT16   flagShop        :1;
  UINT16   flagSouvenir    :1;
  UINT16   flagMotel       :1;
  UINT16   flagChildren    :1;
  UINT16   flagInfo        :1;
  UINT16   flagOther       :1;
  UINT16   reserve         :6;
} Service_Area_Info;

typedef struct Facility_Name_Info {
  char     languageCode;
  char     characterNum;
  //char*      nameString; ???
} Facility_Name_Info;

typedef struct Facilities_Header {
  UINT16   flagInterChangeJunction :1;
  UINT16   flagTollStation         :1;
  UINT16   flagServiceArea         :1;
  UINT16   flagParkingArea         :1;
  UINT16   flagIsGlueNode          :1;
  UINT16   flagIsPathEnd           :1;
  UINT16   flagPositive            :1;
  UINT16   flagNegative            :1;
  UINT16   langNum                 :4;
  UINT16   tollGateNum             :4;
} Facilities_Header;

typedef struct Highway_Facilities_Partial_Data {
  Facilities_Header    header;
  UINT16               preFacilitySize;
  UINT16               recentFacilitySize;
  UINT32               positiveMeter;
  UINT32               negativeMeter;
  UINT64               glueNodeID;
  //WORD               tollGateInfo;//???
  //WORD               serviceAreainfo;
  //WORD               facilityName;//???
} Highway_Facilities_Partial_Data;
#pragma pack()

/*----------------------------------------------------------------*/
#define HIWAYINFO_MAJOR_VERSION 140
#define HIWAYINFO_MINOR_VERSION   1

TMapfileStatus hiwayinfo_load(const char *const mapName);

TMapfileStatus HiwayInfo_CheckVersion(const char *sourceFile);

BOOL8 IsCurrDCA_HiwayInfoOpen(char *DCAname);

void SetDCAname_forHiwayInfo(char *DCAname);

extern MAPCORE_API
BOOL8 HiwayInfo_OpenEx(const char *sourceFile);

extern MAPCORE_API
void HiwayInfo_Shutdown(void);

extern MAPCORE_API
BOOL8 Get_HighWay_InfoEx(char* dca_name, UINT32 sectorID, UINT32 segmentID, THighWayInfo* hiway_info);

extern MAPCORE_API
BOOL8 Get_Next_HighWay_InfoEx(THighWayInfo* hiway_info);

extern MAPCORE_API
BOOL8 Get_Highway_Info_First(THighWayInfo *pHighwayInfo);

extern MAPCORE_API
BOOL8 Get_Highway_Info_Next(THighWayInfo *pHighwayInfo);

extern MAPCORE_API
UINT32 Get_Highway_Info_Num(const THighWayList *pThis, TJourneyLegNumber *leg);

extern MAPCORE_API
UINT32 Get_Current_Highway_Info_Idx(const THighWayList *pThis, const TJourneyLegNumber leg);

extern MAPCORE_API
BOOL8 Get_Highway_Info(const THighWayList *pThis, const TJourneyLegNumber leg, const UINT32 _index, THighWayInfo *pHighwayInfo);

extern MAPCORE_API
BOOL8 guideList_HighwayInfo_isFull(const THighWayList *pThis, const TJourneyLegNumber leg);

extern MAPCORE_API
void Highway_Info_Update(TGuideList *pThis, THighWayList *pHiwayList);

extern MAPCORE_API
void Make_Highway_Info(TGuideList *pThis, THighWayList *pHiwayList);

extern MAPCORE_API
void guideList_HighwayInfo_init(THighWayList *pThis, const TJourneyLegNumber leg, void *highwayInfoBuffer, UINT32 bufferSize);

extern MAPCORE_API
BOOL8 Highway_Info_Refresh_Navigation(THighWayList *pHiwayList);

extern MAPCORE_API
BOOL8 Highway_Info_Refresh_NonNavigation();

extern MAPCORE_API
BOOL8 Get_Highway_Info_By_Kind(THighWayInfo *HighwayInfo, int kind);

extern MAPCORE_API
BOOL8 IsDrivingOnFreeway();
/*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* HIWAYINFO_H */
