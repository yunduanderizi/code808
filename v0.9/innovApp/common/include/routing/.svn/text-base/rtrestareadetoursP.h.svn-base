#ifndef RTRESTAREADETOURS_H
#define RTRESTAREADETOURS_H

#include <mapdata/enroad.h>

/// @file
///
/// @brief Common constants and functions used by rest area detour
///        detection code
///
/// The bulk of the rest area detection code lives in \c
/// expansion_checkAndCostRestAreaDetours() and \c
/// convergencePoint_checkRestAreaDetour().

/// Road types less than or equal to this value (i.e. of this
/// importance or greater) are considered motorways for the purposes
/// of rest area detour detection.
#define RT_REST_AREA_DETOUR_MAX_ROAD_TYPE ROAD_TYPE_NATIONAL_HIGHWAY

/// The maximum number of segments to follow when testing for a rest
/// area detour.
#define RT_REST_AREA_DETOUR_BACKTRACK_SEGMENTS 10

/// Deterimines if a segment might be part of a rest area.  Even if
/// this returns \c TRUE, the segment is probably not part of a rest
/// area, but if this returns \c FALSE then the segment definitely
/// isn't.
///
/// @param[in] segment an unpacked segment
///
/// @return \c TRUE if \p segment might be part of a rest area
BOOL8 isPossiblyRestArea(const TRoadSegment *segment);

/// Determines if a segment can be considered part of a motorway for
/// the purposes of rest area detour detection.
BOOL8 isImportantOneWayRoad(const TRoadSegment *segment);

#endif
