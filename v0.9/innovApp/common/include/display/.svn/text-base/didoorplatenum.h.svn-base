#ifndef H_DOOR_PLATE_NUM_H
#define H_DOOR_PLATE_NUM_H
#include <innovEngine_types.h>
#include <platform.h>
#include <language/utf16.h>
#include <math/gerect.h>
#include <logging/enlog.h>
//#include <logging/sdebugP.h>

#define MAX_DOOR_PLATE_NUM_SET  220
#define MAX_SEGMENT_NUM_SET 20
#define MAX_FRAME_COUNT     7

struct TViewport; 

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDoorPlateNumInfo 
{
    UTF16 name[16];
    TRect textRect;
    TPos  pos;
    UINT8 doorPlatePlacement;
    UINT8 frameNumber;
}TDoorPlateNumInfo;

typedef struct TOneSegmentDoorPlateSet
{
    UINT32  mapIndex;
    UINT32  name1Index;
    UINT32  name2Index;
    UINT32  segmentSetIndex;
    UINT32  segmentID;
    UINT32  count;
    UINT64 preTime;
    UINT8 currentFrame;
    UINT8 maxFrame;
    BOOL8 bframeReady;
    BOOL8 bNeedDisplay;
    TDoorPlateNumInfo   doorPlateNums[MAX_DOOR_PLATE_NUM_SET];
    UINT8 doorPlateNumCount;
}TOneSegmentDoorPlateSet;


typedef struct TDoorPlateNumSet
{
    TOneSegmentDoorPlateSet segmentSet[MAX_SEGMENT_NUM_SET];
    UINT8   segmentCount;
    TRect   drawedRect[MAX_FRAME_COUNT][MAX_DOOR_PLATE_NUM_SET * MAX_SEGMENT_NUM_SET];
    UINT16   drawedCount[MAX_FRAME_COUNT];
    struct TViewport *viewport;

    //! For debug
    UINT64  dataReadTime;
    UINT64  sectorReadTime;
    UINT32  roadNumber;
    UINT32  sectorNumber;
    UINT32  repeatTimes;

}TDoorPlateNumSet;


  

/**
 @brief       Add a doorplate number record to doorplate set for
              one road indicated by segment ID.
 @param[In]   TOneSegmentDoorPlateSet * pThis:Doorplate set for specified 
                segment associated with necessary information needed by
                rendering function.
 @param[In]   TDoorPlateNumInfo * pDoorPlateNumberInfo: single doorplate
                record data.
 @return      void
*/
void DoorPlateNum_Define( TOneSegmentDoorPlateSet *pThis,
                         TDoorPlateNumInfo *pDoorPlateNumberInfo);






/**
 @brief     Render all the doorplate element to screen frame by frame.
 @param[In]   TDoorPlateNumSet * pThis Set for all the segment lied in
                                range of screen and all the doorplate number
                                within them respectively.
 @param[In]   struct TViewport * viewport is global display context.
 @return      void
*/
void DoorPlateNumSet_Render(TDoorPlateNumSet *pThis, 
                               struct TViewport *viewport);





/**
 @brief     Calculate all the doorplate number in each frame
            before rendering.
            @param[In]   TDoorPlateNumSet * pThis Set for all the segment lied in
            range of screen and all the doorplate number
            within them respectively.
 @return      void
*/
void DoorPlateNumSet_FrameCalculate(TDoorPlateNumSet *pThis);




/**
 @brief     Check to see if new segment is already in the segment
            cache, and also alternative segmentID, which has the same 
            doorplate set, mapped to the same road.
 @param[In]   TDoorPlateNumSet * pThis Set for all the segment lied in
                        range of screen and all the doorplate number
                        within them respectively.
 @param[In]   UINT32 mapIndex indicate map index which include pending segmentID.
 @param[In]   UINT32 name1Index is index of name for this road in road name set.
 @param[In]   UINT32 name2Index is also index of name for this road in road name set.
 @param[In]   UINT32 segmentSetIndex index for road name set. name1Index, name2Index
                        and segmentSetIndex are used in checking if different segmentID belongs to the
                        same road.
 @param[In]   UINT32 segmentID represent current road by number.
 @param[Out]   UINT32 * minIndex If related segment ID existed in doorplate set,
                        return current index, if not, then get the index of segment which
                        has the smallest reference count to be replaced by new segment associated
                        with doorplate set.
 @return      BOOL8     TRUE indicates found, and FALSE not.
*/
BOOL8 DoorPlateNumSet_IsSegmentExsited(TDoorPlateNumSet *pThis, 
                                   UINT32 mapIndex,
                                   UINT32  name1Index,
                                   UINT32  name2Index,
                                   UINT32  segmentSetIndex,
                                   UINT32 segmentID,
                                   UINT32 *minIndex);





/**
 @brief     Divide all the doorplate for one segment into a frame by calculating their rectangle.
 @param[In]   TDoorPlateNumSet * pThis Set for all the segment lied in
                                range of screen and all the doorplate number
                                within them respectively.
 @param[In]   UINT8 uFrame  frame number in which doorplate number lies.
 @param[In]   TRect * pRect     Rectangle of new doorplate number will be added in current frame.
 @return      BOOL8
*/
BOOL8 DoorPlateNumSet_CheckRectCouldbeLayedOutFrame(TDoorPlateNumSet *pThis, UINT8 uFrame, TRect *pRect);

#ifdef __cplusplus
}
#endif

#endif