#ifndef DIPOIRANKLIST_H
#define DIPOIRANKLIST_H

#include <display/diiconranklist.h>
#include <mapdata/enpoi.h>
#include <location/locmaplocation.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Icons
/// @ingroup Display
/// @{

/// @addtogroup POI
/// @ingroup Icons
/// @{

/// @brief Gets the map location of the POI represented by a displayed icon
///
/// @param[in]  pThis       the displayed icon
/// @param[out] mapLocation The TMapLocation structure that will be
///                         populated
///
/// @return \c TRUE if \p pThis is a POI icon and its details can be
///         retrieved
extern MAPCORE_API
BOOL8 IconDisplayRank_GetPOIMapLocation( const TIconDisplayRank *pThis,
                                         TMapLocation           *mapLocation );


extern MAPCORE_API
void IconDisplayRankSet_GetVisiblePOIPosAndID(const TIconRankSet *pThis,
                                              UINT32 index,
                                              TPos *pos_w,
                                              UINT32 *id);

extern MAPCORE_API
BOOL8 IconDisplayRankSet_HighlightPOI( const TIconRankSet *pThis,
                                       TMapLocation       *mapLocation,
                                       const TPos         *pos_s,
                                       UINT32             *highlightedPoiId);

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif
