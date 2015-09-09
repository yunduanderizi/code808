#include "interface_guide.h"
#include "interface_route.h"
#include <guidance/gdinstructionP.h>
#include <stdlib.h>
#include <stdio.h>
#include "engineinterface/filenamelist.h"
#include "guidance/gdroundaboutP.h"
//#include "engineinterface/engineinterface.h"
navcoreJourney::TIntersectionDescr g_intersectiondescr;
TGuideInstruction *g_guideInstruction=NULL;
bool isnewturn=false;
int njuctionchange= 0;
int njuctioncount= 0;
INT32 g_distancetoturn;
UINT8 g_degree;
INT32 g_guideIndex =GUIDE_LIST_DEFAULT;


//////////////////////////////////////////////////////////////////////////////
///use by interface_guide
static int getexitnumber()
{
    return g_intersectiondescr.exitIndex;
}

static bool getRoundAboutExitIndex(INT32 *roundaboutexitindex)
{
    TGuideRoundabout roundaboutJourney;
    if(gdinters_roundaboutStats( &roundaboutJourney, g_navRoute->getGuideList()->pJourney, g_guideInstruction))
    {
        if(roundaboutJourney.journeyExitSegmentIndex>0)
        {
            *roundaboutexitindex = roundaboutJourney.journeyExitSegmentIndex;
            return true;
        }
    }
    return false;
}

static void setguidanceinstr()
{
    TGuideList *guidanceList = NULL;

    guidanceList = g_navRoute->getGuideList();
    g_guideInstruction = guideList_get(guidanceList, g_guideIndex);
}

static int setdistancetoturn()
{
    INT32 timeToTurn;
    TGuideContext * guidance_context = NULL;
    TGuideList *guidanceList = NULL;


    if( g_guideIndex==GUIDE_LIST_ERROR )
    {
        return -1;
    }
    guidance_context = g_navRoute->getGuideContext();
    if(guidance_context==NULL )
    {
        return -1;
    }
    guidanceList = g_navRoute->getGuideList();
    guideContext_timeAndDistanceToTurn(guidance_context,g_guideIndex,&g_distancetoturn,&timeToTurn);
    return 0;
}

static void set_Tintersectionindetail()
{
    if(g_guideIndex!=-1)
    {
        if (g_navRoute->initialiseIntersectionPictorial(g_guideIndex)) {
            g_navRoute->retrieveIntersectionPictorialAttributes(g_intersectiondescr);
        }
    }
    return ;
}

//follow are use by others
void guideGetDistanceToTurn(INT32 * distance)
{
    *distance = g_distancetoturn;
}

INT32 guideGetDistanceToDist()
{
    return g_guideInstruction->distanceToEnd;
}

int guideGetInfoStatus()
{
    int status = 0;
    if(routeIsSimulating())
        status = 2;
    else if(g_navRoute->ismanulSimulating())
        status = 1;
    else status = 0;
    return status;
}


int guideGetInfoDirection()
{
    TGuideTurnDirection direction = g_guideInstruction->direction;
    UINT8 degree = guideInstr_getTurnDegree(g_guideInstruction);

    INT32 exitindex;

    int newDir = 0xFF;
    if(getRoundAboutExitIndex(&exitindex))
    {
        newDir = 0x33+exitindex;
        return newDir;
    }

    if(guideIsEndOfJourney())
    {
        newDir = 0x56;
    }
    else
    {
        switch (direction)
        {
        {
        case TURNDIRECTION_LEFT:
//                if(guideInstr_isRoundaboutEntry(g_guideInstruction))
//                {
//                    newDir = 0x43;
//                }
//                else
//                {
                    switch(degree)
                    {
                    case TURN_SLIGHT:
                        newDir = 0x27;
                        break;
                    case TURN_MEDIUM:
                        newDir = 0x26;
                        break;
                    case TURN_SHARP:
                        newDir = 0x25;
                        break;
                    default:
                        newDir = 0x20;
                        break;
                    }
//                }
                break;
        }

        case TURNDIRECTION_STRAIGHT:
//            if(guideInstr_isRoundaboutEntry(g_guideInstruction))
//            {
//                newDir = 0x41;
//            }
//            else
//            {
                newDir = 0x20;
//            }
            break;

        case TURNDIRECTION_RIGHT:
//            if(guideInstr_isRoundaboutEntry(g_guideInstruction))
//            {
//                newDir = 0x45;
//            }
//            else if(guideInstr_isRoundaboutExit(g_guideInstruction))
//            {
//                newDir = 0x45;
//            }
//            else
//            {
                switch(degree)
                {
                case TURN_SLIGHT:
                    newDir = 0x21;
                    break;
                case TURN_MEDIUM:
                    newDir = 0x22;
                    break;
                case TURN_SHARP:
                    newDir = 0x23;
                    break;
                default:
                    newDir = 0x20;
                    break;
                }
//            }
            break;
        case TURNDIRECTION_CURRENT_ROAD:
            newDir = 0x0;
            break;
        default:
            newDir = 0x0;
            break;
        }
    }
    return newDir;
}

bool guideRealGuidance()
{
    if(maplock_isOnRoute())  //TODO
    {
        if(guideIsEndOfJourney())
        {
            INT32 distance;
            guideGetDistanceToTurn(&distance);
            if(distance<10)
            {
                g_navRoute->setmanulsimulationfinished(true);
                return false;
            }
        }
    }

    TPos gpslocation;

    routeGetGpsLocation(gpslocation);
    g_navRoute->driveCar(gpslocation);
    return guideUpdateGuidanceInfo();
}


void guideGetNextTurnStreetName(UTF16 *fullRoadName)
{
    UINT8 maxNameLen=ROAD_MAX_NAME_SIZE;
    UINT32 usegmentID = 0;
    UTF16 routeType[maxNameLen];
    UTF16 simpleRouteName[maxNameLen];

    if(guideIsEndOfJourney())
    {
        return ;
    }

    usegmentID = g_guideInstruction->roadNameSegment;
    if (usegmentID != 0 && road_getRouteNumberOrNameBySegmentIDOrPos(usegmentID,
                                        0,
                                        routeType,
                                        simpleRouteName,
                                        fullRoadName,
                                        maxNameLen,
                                        ROAD_ONLY_NAME)) {
   } else {

    }
}


TGuideTurnDirection guideGetTurndDirection()
{
    return g_guideInstruction->direction;
}


bool guideUpdateGuidanceInfo()
{
    TGuideContext * guidance_context = NULL;
    int guideIndex;

    if(!maplock_isOnRoute())
    {
        return false;
    }
    guidance_context = g_navRoute->getGuideContext();
    guideIndex = guideContext_getNextTurnIndex(guidance_context);
    if( guideIndex!=g_guideIndex )
    {
        isnewturn = true;
        g_guideIndex = guideIndex;
        set_Tintersectionindetail();
        setguidanceinstr();
        setstreetname();
        setdistancetoturn();
    }
    else
    {
        if(isnewturn)
        {
            isnewturn = false;
        }
        setdistancetoturn();
    }
    return true;
}


unsigned int guideGetTurnCount()
{
    return g_navRoute->getNumberOfTurns();
}

unsigned int guideGetTurnInfo(const unsigned int guideIndex,
                                wstring& distanceFromStart,
                                wstring& roadname,
                                wstring& grade,
                                wstring& style,
                                TSegmentID &nsegmentid)
{
    return g_navRoute->getGuidanceInfo(guideIndex, distanceFromStart, roadname, grade, style, nsegmentid);
}

bool guideIsEndOfJourney()
{
    if(g_guideInstruction==NULL)
    {
        return false;
    }
    if(guideInstr_isEndOfJourney(g_guideInstruction))
    {
        return true;
    }
    return false;
}

bool guideIsLaneinfoAvailable()
{
    if(!routeIsDefiningNewJourney()||
            routeIsSimulationFinished()||
            routeIsManulSimulationFinished())
    {
        return false;
    }

    if(maplock_isOnRoute())
    {
        if(g_intersectiondescr.lanePicValid)
        {
            return true;
        }
    }
    return false;
}

bool guideIsJunctionAvailable()
{
    if(!routeIsDefiningNewJourney()||
        routeIsSimulationFinished()||
        routeIsManulSimulationFinished())
    {
        return false;
    }

    if(maplock_isOnRoute())
    {
        if(getishavejuction() == 1)
        {
            return true;
        }
    }
    return false;
}

navcoreJourney::TIntersectionDescr guideGetTintersectionInDetail()
{
    return g_intersectiondescr;
}

bool guideIsIntoNewTurn()
{
    return isnewturn;
}

TDetourResult guideBanfrontSegments(UINT32 detourLength)
{
    banSegments_clearDetourBans();
    return banSegments_addForImmediateDetour(detourLength);
}



bool guideCloseSegment(TSegmentID nsegmentid)
{

    //banSegments_addMapLocation()
    return true;
}

bool guideGetSignPostString(UTF16 *pSignPostStr)
{
    TGuideList *pguidelist = NULL;
    pguidelist = g_navRoute->getGuideList();
    if((pguidelist==NULL)||!(g_guideIndex>=0))
        return false;
    if(guideList_getSignpost(pguidelist,
                                 g_guideIndex,
                                 pSignPostStr,
                                 ROAD_MAX_NAME_SIZE))
    {
        return true;
    }
    return false;
}

bool guideGetJunctionViewDate(unsigned char* picturedate,long *psize1 ,unsigned char * picturedate2,long *psize2)
{
    if(g_guideInstruction==NULL)
    {
        return false;
    }

    if(g_overlaySet.count>0)                                 // should be at least 2
    {
        assert(g_overlaySet.count <= MAX_JUNCTION_IMAGE_TYPES);
        engineGetPicInfo(picturedate,g_overlaySet.entries[0].offset,g_overlaySet.entries[0].size);
        *psize1 = g_overlaySet.entries[0].size;
        engineGetPicInfo(picturedate2,g_overlaySet.entries[1].offset,g_overlaySet.entries[1].size);
        *psize2 = g_overlaySet.entries[1].size;
        return true;
    }
    return false;
}

void setishavejuctionchange(int flag)
{
    njuctionchange = flag;
}

int getishavejuction()
{
    return njuctionchange;
}

void guidanceDateReset()
{
    g_intersectiondescr.lanePicValid = false;
    g_guideInstruction=NULL;
    isnewturn=false;
    njuctionchange= 0;
    njuctioncount= 0;
    g_guideIndex =GUIDE_LIST_DEFAULT;
}
