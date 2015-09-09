#ifndef LOCUTILP_H
#define LOCUTILP_H

#include <innovEngine_types.h>
#include <mapdata/enroadP.h>
#include <mapdata/enpoiP.h>
#include <location/locmaplocation.h>

#ifdef __cplusplus
extern "C" {
#endif

// Default 'safe' distance for generic snapping
#define SNAP_DEFAULT_SEARCH_DISTANCE 5000

typedef struct TLocationSnapSegment {
  TSegmentID segmentID;
  UINT32     segmentLength;
  TPos       snapPos;
  TRoadSide  sideOfLine;
  UINT32     distance;
  UINT32     distanceToEndNode;
  BOOL8      nameMatched;
} TLocationSnapSegment;  

typedef struct TLocationSnapPOI {
  UINT32     poiID;
  UINT32     distance;
  BOOL8      nameMatched;
} TLocationSnapPOI;

void location_invalidatePos( TPos *pos );

BOOL8 location_posIsValid( const TPos *pos );

void location_lockHeadingOnSegment( 
  TSegmentID segmentID, 
  INT32      heading,
  TNodeID    *destNodeID,
  BOOL8      *destTowardsEnd );

//TODO comments
void location_calculatePosOnSegment( 
  TSegmentID segment,
  UINT32     proportion,
  TPos      *outputPos,
  UINT32    *distToEnd );

// User supplied callback to validate the sectors being considered during a 
// snap operation
typedef BOOL8 TLocationSnapSectorCallback (
  void              *context,
  TRoadSectorHeader *sector ); 

// User supplied callback to validate the segments being considered during a 
// snap operation
typedef BOOL8 TLocationSnapSegmentCallback (
  void              *context,
  TRoadSegment      *segment ); 

UINT16 location_getSnapProportion( const TLocationSnapSegment *snapSegment );

BOOL8 location_snapToRoad( const TPos                   *pos, 
                           INT32                         orientation,
                           const UTF16                  *hintedIdentity,
                           UINT32                        maxSnapDistance,
                           TRoadMapLayer                 layerToSnapTo,
                           TRoadType                     maxRoadType,
                           TLocationSnapSectorCallback  *sectorCallback,
                           TLocationSnapSegmentCallback *segmentCallback,
                           void                         *context,                      
                           TLocationSnapSegment         *snapOutput );
//@MIC Start
BOOL8 location_snapToRoad2( const TPos                   *pos, 
                            INT32                         orientation,
                            const UTF16                  *hintedIdentity,
                            UINT32                        maxSnapDistance,
                            TRoadMapLayer                 layerToSnapTo,
                            TRoadType                     maxRoadType,
                            TLocationSnapSectorCallback  *sectorCallback,
                            TLocationSnapSegmentCallback *segmentCallback,
                            void                         *context,                      
                            TLocationSnapSegment         *snapOutput );

BOOL8 location_snapToRoad3( const TPos                   *pos, 
                            INT32                         orientation,
                            const UTF16                  *hintedIdentity,
                            UINT32                        maxSnapDistance,
                            TRoadMapLayer                 layerToSnapTo,
                            TRoadType                     maxRoadType,
                            TLocationSnapSectorCallback  *sectorCallback,
                            TLocationSnapSegmentCallback *segmentCallback,
                            void                         *context,                      
                            TLocationSnapSegment         *snapOutput );

BOOL8 location_snapToRoad4( const TPos                   *pos, 
                            INT32                         orientation,
                            const UTF16                  *hintedIdentity,
                            UINT32                        maxSnapDistance,
                            TRoadMapLayer                 layerToSnapTo,
                            TRoadType                     maxRoadType,
														UINT32                        filterSegmentID,
                            TLocationSnapSectorCallback  *sectorCallback,
                            TLocationSnapSegmentCallback *segmentCallback,
                            void                         *context,
                            TLocationSnapSegment         *snapOutput );

BOOL8 location_snapToRoadforSS( const TPos                   *pos, 
                                INT32                         orientation,
                                const UTF16                  *hintedIdentity,
                                UINT32                        maxSnapDistance,
                                TRoadMapLayer                 layerToSnapTo,
                                TRoadType                     maxRoadType,
                                TLocationSnapSectorCallback  *sectorCallback,
                                TLocationSnapSegmentCallback *segmentCallback,
                                void                         *context,                      
                                TLocationSnapSegment         *snapOutput,
                                UINT32                        iRoadCode);

//@MIC End

BOOL8 location_snapToPOI( const TPos                    *pos,
                          const UTF16                   *hintedIdentity,
                          INT32                          globalTypeID,
                          UINT32                         maxSnapDistance,
                          TPOIMapLayer                   layerToSnapTo,
                          TLocationSnapPOI              *snapOutput );


#ifdef __cplusplus
}
#endif

#endif // LOCUTILP_H
