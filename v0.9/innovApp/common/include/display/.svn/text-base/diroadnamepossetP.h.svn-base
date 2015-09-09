#ifndef DIROADNAMEPOSSETP_H
#define DIROADNAMEPOSSETP_H

#include <mapdata/enmapinfoP.h>
#include <display/diroadnameposset.h>

#ifdef __cplusplus
extern "C" {
#endif

extern MAPCORE_API
TRoadNamePos* RoadNamePosSet_FindPos(TRoadNamePosSet *pThis,
                                     const UTF16 *roadName,
                                     const TGenericMapInfo *const mapInfo);

extern MAPCORE_API
void RoadNamePosSet_AddPos(TRoadNamePosSet *pThis,
                           const UTF16 *roadName,
                           const TPos *latlon, 
                           UINT8 place,
                           TRoadType roadType,
                           UINT16 routeType,
                           TRect *textBounds,
                           const struct TGenericMapInfo *const mapInfo);


#ifdef __cplusplus
}
#endif

#endif /* DIROADNAMEPOSSETP_H */
