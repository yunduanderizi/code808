#ifndef INTERFACE_GUIDE_H
#define INTERFACE_GUIDE_H
#include "engineinterface/engineinterface.h"
#include "guidance/gdinstructionP.h"

int guideGetInfoDirection();
int guideGetInfoStatus();
void guideGetDistanceToTurn(INT32 * distance);
INT32 guideGetDistanceToDist();
void guideGetNextTurnStreetName(UTF16 *);
bool guideIsLaneinfoAvailable();
bool guideIsJunctionAvailable();
bool guideRealGuidance();
unsigned int guideGetTurnCount();
unsigned int guideGetTurnInfo(const unsigned int guideIndex,
                                wstring& distanceFromStart,
                                wstring& roadname,
                                wstring& grade,
                                wstring& style,
                                TSegmentID &nsegmentid);
bool guideIsEndOfJourney();
bool guideIsIntoNewTurn();
bool guideCloseSegment(TSegmentID nsegmentid);
TDetourResult guideBanfrontSegments(UINT32 detourLength);
navcoreJourney::TIntersectionDescr guideGetTintersectionInDetail();
bool guideGetJunctionViewDate(unsigned char* picturedate,long *psize1 ,unsigned char * picturedate2,long *psize2);
bool guideGetSignPostString(UTF16 *pSignPostStr);
TGuideTurnDirection guideGetTurndDirection();
bool guideUpdateGuidanceInfo();

void setishavejuctionchange(int flag);
int getishavejuction();
void guidanceDateReset();

#endif // INTERFACE_GUIDE_H
