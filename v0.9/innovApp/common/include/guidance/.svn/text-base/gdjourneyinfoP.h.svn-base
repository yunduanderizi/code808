#ifndef GDJOURNEYINFOP_H
#define GDJOURNEYINFOP_H

#include <guidance/gdinstruction.h>
#include <containers/gedlist.h>
#include <mapdata/enroadP.h>
#include <language/lacodepageP.h>
#include <routing/rtpath.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TOKEN_LIST_MAX_TOKENS 4

#define TOKEN_LIST_STRING_BUFFER_SIZE ROAD_MAX_NAME_SIZE

/// @internal
/// Route numbers on signposts often do not exactly match route numbers on
/// roads, even though the strings refer to the same physical road. E.g. the
/// signpost string "HWY 29" might match the road "WI-29", or the signpost
/// string "US 141" might match the road "US-141". To perform these kind of
/// "fuzzy" string matches, we split route numbers up into alpha/numeric tokens
/// and match the tokens independently.
typedef struct {
  /// @internal
  /// The list of alpha/numeric tokens comprising the route number. This is a
  /// list of offsets into the tokenBuffer. We do not use pointers so that this
  /// structure is relocatable in memory.
  UINT16 tokens[TOKEN_LIST_MAX_TOKENS];
  /// @internal
  /// The number of tokens in the tokens buffer.
  UINT32 numTokens;
  /// @internal
  /// Buffer used to store the token strings.
  UTF16 tokenBuffer[TOKEN_LIST_STRING_BUFFER_SIZE];
  /// @internal
  /// Number of bytes used in the token buffer
  UINT32 usedBufferSize;
#ifdef SIMULATOR
  /// @internal
  /// Store an exact copy of the original string to aid debugging.
  UTF16 routeNumStr[ROAD_MAX_NAME_SIZE];
#endif
} TStringTokenList;


/// @internal
/// The maximum distance in metres at which to include route numbers in
/// instructions. e.g. If a sign post says "M5" we would say "Follow the signs
/// to the M5" if the route joins the M5 in less than 50000 metres.
#define GD_ROUTE_NUMBER_MAX_DISTANCE 50000

/// @internal
/// Store up to 8 route name changes.
#define GD_ROUTE_NUMBER_LIST_SIZE 8

typedef struct {
  /// TDListObject previous object pointer.
  void *previous;
  /// TDListObject next object pointer.
  void *next;
  /// Driving distance in metres from the point at which the route number
  /// begins to the end of the journey.
  UINT32 distance;
  /// Route number e.g. "M5". The route number is split into a list of tokens
  /// to make comparisons easier.
  TStringTokenList routeNumber;
  /// Route directional (North, South, East, West, or undefined)
  TTokenRouteDir direction;
} TGuideRouteNumber;


typedef struct {
  /// TDListObject previous object pointer.
  void *previous;
  /// TDListObject next object pointer.
  void *next;
  /// Driving distance in metres from the point at which the route number
  /// begins to the end of the journey.
  UINT32 distance;
  /// Street name. Only take name1 for comparison
  TRoadStringID name1;
} TGuideStreetName;

/// The TGuideJourneyInfo object provides all of the information we would
/// like to be able to obtain from #TJourney itself, but is not provided. e.g.
/// route numbers along the journey, the length of the journey, and the
/// distance to the nearest intersection.
typedef struct {
  /// Linked list of allocated #TGuideRouteNumber objects.
  TDList usedList;
  /// Linked list of unallocated #TGuideRouteNumber objects.
  TDList freeList;
  /// Route number list storage space.
  TGuideRouteNumber numbers[GD_ROUTE_NUMBER_LIST_SIZE];
  /// Linked list of allocated #TGuideRouteNumber objects.
  TDList streetUsedList;
  /// Linked list of unallocated #TGuideRouteNumber objects.
  TDList streetFreeList;
  /// Route number list storage space.
  TGuideStreetName streets[GD_ROUTE_NUMBER_LIST_SIZE];
  /// Cumulative length of the journey in metres.
  UINT32 length;
  /// Cumulative travel time of the journey in seconds.
  UINT32 time;
  /// Number of intermediate right turns to the next instruction.
  INT32 leftTurnCount;
  /// Number of intermediate left turns to the next instruction.
  INT32 rightTurnCount;
} TGuideJourneyInfo;

extern void gdJourneyInfo_init(TGuideJourneyInfo *pThis);

extern void gdJourneyInfo_updateSeg(TGuideJourneyInfo *pThis,
                                    TRoadSegment *pSegment,
                                    const TPath *pPath,
                                    const INT32 pathIndex);

extern void gdJourneyInfo_getDistance(const TGuideJourneyInfo *pThis,
                                      UINT32 *distance,
                                      UINT32 *time);

extern BOOL8 gdJourneyInfo_addRouteNumber(TGuideJourneyInfo *pThis,
                                          const UTF16 *number,
                                          TTokenRouteDir direction);

extern BOOL8 gdJourneyInfo_addStreet(TGuideJourneyInfo *pThis,
                                     const TRoadStringID name1);

extern void gdJourneyInfo_reconstructRouteAndStreetName( TGuideJourneyInfo *pThis,
                                                         TSegmentID        segmentID,
                                                         UINT32            distance );

extern UINT32 gdJourneyInfo_findRouteNumber(const TGuideJourneyInfo *pThis,
                                            const UTF16 *number,
                                            TTokenRouteDir direction);

extern UINT32 gdJourneyInfo_findStreet(const TGuideJourneyInfo *pThis,
                                       const TRoadStringID name1);

extern void gdJourneyInfo_updateIntermediateTurns(TGuideJourneyInfo *pThis,
                                                  INT32 leftTurns,
                                                  INT32 rightTurns);

extern void gdJourneyInfo_resetIntermediateTurns(TGuideJourneyInfo *pThis);

extern INT32 gdJourneyInfo_getIntermediateTurns(const TGuideJourneyInfo *pThis,
                                                TGuideTurnDirection direction);

#ifdef __cplusplus
}
#endif

#endif /* GDJOURNEYINFOP_H */
