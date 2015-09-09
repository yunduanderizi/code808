#ifndef H_DIFETCHHOUSEDATA_H
#define H_DIFETCHHOUSEDATA_H
#include <innovEngine_types.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

   


/**
 @brief Fetch doorplate number data from map index and segmentID within map, and 
        put data set into buffer to render them subsequently.
 @param[In]   UINT32 mapIndex indicate map index which include pending segmentID.
 @param[In]   UINT32 name1Index is index of name for this road in road name set.
 @param[In]   UINT32 name2Index is also index of name for this road in road name set.
 @param[In]   UINT32 segmentSetIndex index for road name set. name1Index, name2Index
        and segmentSetIndex are used in checking if different segmentID belongs to the
        same road.
 @param[In]   UINT32 segmentID represent current road by number.
 @param[In]   TViewport * viewport is global display context.
 @param[In]   TPos * p1 start position of this road.
 @param[In]   TPos * p2 end position of this road.
 @return      void
*/
void FetchHouseData_byMapIndexAndSegment(UINT32 mapIndex, 
                                         UINT32  name1Index,
                                         UINT32  name2Index,
                                         UINT32  segmentSetIndex,
                                         UINT32  segmentID, 
                                         TViewport *viewport, 
                                         TPos *p1, 
                                         TPos *p2);

#ifdef __cplusplus
}
#endif

#endif
