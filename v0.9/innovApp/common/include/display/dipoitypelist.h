#ifndef DIPOITYPELIST_H
#define DIPOITYPELIST_H

#include <mapdata/englobal.h>
#include <mapdata/enpoi.h>
#include <mapdata/enpoitype.h>
#include <language/lacodepage.h>
#include <containers/gebitset.h>

#ifdef __cplusplus
extern "C" {
#endif

//@MIC Start
//To get the map layer
extern MAPCORE_API
TPOIMapLayer GetPOIMapLayer(UINT16 typeId);
//@MIC End

/// @addtogroup Display
/// @{

///
/// @addtogroup PointsOfInterest
/// @ingroup Display
/// @{

// Clear all the bits in the type filter
BOOL8 POITypeFilter_init( TPOITypeFilter *typeFilter );

UINT32 POITypeFilter_getTotalTypeCount( const TPOITypeFilter *typeFilter );

// Enable all types in the type filter
void POITypeFilter_setAllFlags( TPOITypeFilter *typeFilter,
                                BOOL8          enable );

// Enable or disable a specific POI type 
void POITypeFilter_setFlag( TPOITypeFilter *typeFilter, 
                            UINT16         typeId,
                            BOOL8          enable );

// Check if a given POI type is enabled or not
BOOL8 POITypeFilter_testFlag( const TPOITypeFilter *typeFilter, 
                              UINT16               typeId );


///
/// @}
///


///
/// @}
///

#ifdef __cplusplus
}
#endif

#endif
