#ifndef DILINE_H
#define DILINE_H

#include <display/didisplayline.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

// this macro determines the maximum number of road segments that can
// be added to the display list
#ifdef PLATFORM_MAXSEGMENTS
#define DISPLAY_MAXSEGMENTS PLATFORM_MAXSEGMENTS
#else
#define DISPLAY_MAXSEGMENTS 10000
#endif

// this macro determines the maximum number of line segments (making
// up road segments) that can be stored in the display list
#define DISPLAY_MAXLINES (DISPLAY_MAXSEGMENTS << 2)



/// @addtogroup UnUsed
/// @ingroup Display
/// @{

/// @addtogroup RoadLabelling
/// @ingroup UnUsed
/// @{

/// @addtogroup DisplaySegment
/// @ingroup RoadLabelling
/// @{

/// This structure is used to store information that applies to a number of
/// road segments.  The actual shape of the road segment is determined by
/// the TDisplayLine structures to which the TDisplaySegment structure points.
/// TDisplaySegment structures are only stored if the name placement code
/// returns a non NULL pointer from the display_addRoadName function.
typedef struct TDisplaySegment
{
  // first TDisplayLine structure contained in this segment
  TDisplayLine *firstLine;
  // one after the last TDisplayLine structure contained in this segment
  TDisplayLine *lastLine;
  // points to the name used in displaying this road segment
  const UTF16 *name;
} TDisplaySegment;

//
// DisplayList functions.
//
typedef struct TDisplayList
{
  UINT32 segmentCount;
  TDisplaySegment *segments;
  UINT32 displayLabelCount;

  TDisplaySegment segmentArray[DISPLAY_MAXSEGMENTS + 1];

  // number of segments recorded in the array
  //UINT32 segmentCount;

  // this array contains all the line strings that make up each segment
  // contained in the segment array
  TDisplayLine lineArray[DISPLAY_MAXLINES + 1];

  // number of lines recorded in the array
  UINT32 lineCount;
  UINT32 oldLineCount;

} TDisplayList;

/// @}

/// @}

/// @}


#ifdef __cplusplus
}
#endif

#endif
