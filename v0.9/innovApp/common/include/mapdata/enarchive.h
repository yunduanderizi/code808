#ifndef ENARCHIVE_H
#define ENARCHIVE_H

#include <mapdata/enmfile.h>
#include <guidance/gdcontext.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_OVERLAY_PATH 260

typedef struct TArchiveOverlay{
  long offset;
  long size;
  char path[MAX_OVERLAY_PATH];
} TArchiveOverlay;

typedef struct TArchiveOverlaySet{
  int count;
  TArchiveOverlay entries[MAX_JUNCTION_IMAGE_TYPES];
} TArchiveOverlaySet;
/// Fills in a list of filename, offset pairs representing embedded PNG files to be used for the 
/// construction of junction view pictorials.
///
/// @param pDest [in] A TArchiveOverlaySet structure
///
/// @param pJunctionViewInfo [in] The TJunctionViewInfo object supplied by the mapcore routing API
///                  
/// @param isNightTime [in] Set to TRUE for a night time pictorial.
///                        
/// @return TRUE if function successfully renders the specified JunctionView
///

extern MAPCORE_API
BOOL8 archive_resolveJunctionViewInfo(TArchiveOverlaySet *pDest, TJunctionViewInfo * pJunctionViewInfo, BOOL8 isNightTime);

/// Fills in a list of filename, offset pairs representing embedded PNG files to be used for the 
/// construction of junction view pictorials.
///
/// @param destOverlays [in] A TArchiveOverlaySet structure
///
/// @param opaqueEventData [in] The TJunctionViewData object supplied by the mapcore routing API
///                  
/// @param isNightTime [in] Set to TRUE for a night time pictorial.
///                        
/// @return TRUE if function successfully renders the specified JunctionView
///

extern MAPCORE_API
BOOL8 archive_resolveJunctionView( TArchiveOverlaySet *destOverlays, TJunctionViewData opaqueEventData, BOOL8 isNightTime );

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ENARCHIVEP_H */
