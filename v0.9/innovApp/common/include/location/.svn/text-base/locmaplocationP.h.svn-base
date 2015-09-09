#ifndef LOCMAPLOCATIONP_H
#define LOCMAPLOCATIONP_H

#include <geosearch/geosearch.h>
#include <location/locmaplocation.h>
#include <mapdata/enmapinfoP.h>
#include <mapdata/entokenP.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TLocationUtteredName {
  TGenericMapInfo *mapInfo;
  TTokenDataType   type;
  UINT32           nameId;
  UINT32           houseNumber;
} TLocationUtteredName;

BOOL8 maplocation_initMovingLocation( TMapLocation *mapLocation, 
                                      const TPos   *userPos,
                                      const UTF16  *name,
                                      UINT32        orientation,
                                      UINT32        heading,
                                      TRoadType     visibleRoadTypes );

UINT32 maplocation_getLocationResultTypes( const TMapLocation          *mapLocation, 
                                           const TGeosearchResultType **resultTypes );

BOOL8 maplocation_resolveOnNetwork( TMapLocation *mapLocation,
                                    TRect        *snapBounds );

//@MIC Start
BOOL8 maplocation_resolveOnNetworkbyRange( TMapLocation *mapLocation,
                                           UINT32        snapRange );

BOOL8 maplocation_resolveOnNetwork2( TMapLocation *mapLocation,
                                     TRect        *snapBounds );

BOOL8 maplocation_resolveOnNetwork3( TMapLocation *mapLocation,
                                     TRect        *snapBounds );

BOOL8 maplocation_resolveOnNetworkforSS( TMapLocation *mapLocation,
                                         TRect        *snapBounds,
                                         UINT32        iRoadCode);
//@MIC End

BOOL8 maplocation_getNetworkLocation( TMapLocation        *mapLocation,
                                      TMapLocationNetwork *outNetwork );

BOOL8 maplocation_getNetworkDistToEndNode( TMapLocation *mapLocation,
                                           UINT32       *distToEndNode );

INT32 maplocation_getNetworkLayer( TMapLocation *mapLocation );

void maplocation_dumpDetails( TMapLocation *mapLocation );

BOOL8 maplocation_getMovementDestNodeID( TMapLocation *mapLocation, 
                                         TNodeID      *nodeID );

TRoadSide maplocation_getNetworkSideOfLine( TMapLocation *mapLocation );

BOOL8 maplocation_getLocationPoiID( const TMapLocation *mapLocation, 
                                    UINT32             *poiID );

BOOL8 maplocation_initNetworkData( TMapLocation *mapLocation, 
                                   const TPos   *netPos,
                                   TSegmentID   segmentId,
                                   INT32        proportion,
                                   TRoadSide    sideOfLine );

BOOL8 maplocation_getUtteredName( TMapLocation         *mapLocation, 
                                  TLocationUtteredName *utteredName );



#ifdef __cplusplus
}
#endif

#endif // LOCMAPLOCATIONP_H
