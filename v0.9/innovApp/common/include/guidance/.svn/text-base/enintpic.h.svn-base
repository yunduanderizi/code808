/* enIntpic contains the API for intersection pictorials. The application
   should query this module to find information about the simplified
   representation of the intersection. A callback function is supplied
   to notify the app when the pictorial should be rendered.

   NOTE: all headings are on a scale of 0-255, and are relative to the
   source (intersection entry) segment. This segment is implicit.
   Similarly, the subsequent intersection headings are relative to
   that intersection's entry. */

#ifndef ENINTPIC_H
#define ENINTPIC_H

#include <guidance/gdlist.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup IntersectionPictorial
/// @ingroup Guidance
/// @{

/// The default minimum angle allowed between roads heading out of a node.
#define DEFAULT_MINIMUM_ANGLE 12

/// The maximum angle that can be passed to #intersectionPic_setMinimumAngle.
#define MAXIMUM_MINIMUM_ANGLE 32

/// @internal
/// The maximum number of roads connected to an intersection
#define GUIDE_MAX_ROADS_PER_INTERSECTION 30

#define GUIDE_MAX_LANES_PER_PICT 10

/// @internal
/// An outgoing road connected to an intersection.
typedef struct {
  TSegmentID segmentID;
  INT16 roadForm;
  INT16 roadFow;
  /// the heading direction of this road as it leaves the intersection
  BITFIELD32 heading:8;
  /// has this road been combined with another one?
  /// eg. both lanes of a dual carriageway are combined to one road
  BITFIELD32 flagRoadsCombined:1;
  /// bits which are still available for allocation to new flags
  BITFIELD32 spareBits:23;

} TIntersectionRoad;

/// @internal
/// Structure that contains the reduces lane data, needed for drawing the lane
/// pictorial.
typedef struct {
  // Number of lanes
  UINT8   laneCount; 
  // Lane direction information for all the lanes we care about
  UINT16  laneDir[ROAD_MAX_LANES];
  // Lane direction information for the lanes required for travel
  UINT16  reqLaneDir[ROAD_MAX_LANES];
} TIntersectionLanePic;

/// Simplified representation of a guided intersection, suitable for use
/// in drawing a pictorial of the intersection.
typedef struct {
  /// @internal
  /// The minimum angle between roads heading out of a node before they
  /// are merged into one road.
  UINT8 minimumAngle;
  /// @internal
  /// Headings of the roads out of each node. Headings are relative to the
  /// intersection source (entry road).
  TIntersectionRoad roads[GUIDE_MAX_ROADS_PER_INTERSECTION];
  /// @internal
  /// The number of roads connected to the intersection (excluding the source).
  UINT32 roadCount;
  /// @internal
  /// Index in the heading list of the destination.
  UINT32 destRoadIndex;
  /// @internal
  /// TRUE if the node is a roundabout.
  BOOL8 isRoundabout;
  /// @internal
  /// TRUE if we have a lane pictorial for this instruction.
  BOOL8 hasLanePic;
  /// @internal
  /// Stores the lane pictorial data for this intersection
  TIntersectionLanePic lanePic;
  /// @internal
  /// Handedness (driving side) of the road network.
  TRoadHandedness handedness;
} TIntersectionPic;


/*----------------------------------------------------------------*/

extern MAPCORE_API
void intersectionPic_init(TIntersectionPic *pThis);

extern MAPCORE_API
INT32 intersectionPic_setGuideIndex(TIntersectionPic *pThis,
                                    const TGuideList *pGuideList,
                                    INT32 index);

extern MAPCORE_API
BOOL8 intersectionPic_hasLanePictorial(const TIntersectionPic *pThis );

extern MAPCORE_API
BOOL8 intersectionPic_getLanePictorial( const TIntersectionPic *pThis, 
                                        TIntersectionLanePic   *lanePic );

extern MAPCORE_API
UINT8 intersectionPic_getRoadCount(const TIntersectionPic *pThis,
                                   UINT8 unused);

extern MAPCORE_API
UINT8 intersectionPic_getRoadHeading(const TIntersectionPic *pThis,
                                     UINT32 roadNumber,
                                     UINT8 unused);

extern MAPCORE_API
UINT8 intersectionPic_getExitIndex(const TIntersectionPic *pThis,
                                   UINT8 unused);

extern MAPCORE_API
BOOL8 intersectionPic_isRoundabout(const TIntersectionPic *pThis,
                                   UINT8 unused);

extern MAPCORE_API
TRoadHandedness intersectionPic_getRoadHandedness(const TIntersectionPic *pThis);

DEPRECATED(
extern MAPCORE_API
UINT8 intersectionPic_getNodeCount(const TIntersectionPic *pThis)
);

DEPRECATED(
extern MAPCORE_API
void intersectionPic_new(TIntersectionPic *pThis)
);

DEPRECATED(
extern MAPCORE_API
void intersectionPic_setMinimumAngle(TIntersectionPic *pThis,
                                     UINT8 angle)
);

/// @}

#ifdef __cplusplus
}
#endif

#endif
