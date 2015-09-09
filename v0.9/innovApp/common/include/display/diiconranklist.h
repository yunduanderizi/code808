#ifndef DIICONRANKLIST_H
#define DIICONRANKLIST_H

#include <innovEngine_types.h>
#include <math/gerect.h>
#include <mapdata/enpoi.h>
#include <graphics/grbitmap.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Icons
/// @ingroup Display
///
/// @brief Interface to the IconDisplayRank and IconDisplayRankSet
///        classes.
///
/// These classes provide a way for a set of potentially overlapping
/// icons to be displayed as a single layer.  When icons overlap,
/// those that are more important obscure those that are less
/// important.  Obscured icons are not rendered.
///
/// @{

struct TViewport;               /* forward declaration */


/**
 * @brief Identifies the different types of icons that may appear in an
 * icon rank list.
 *
 * The types that appear earlier in this enumeration have priority over
 * types that appear later, i.e. icons of lower-numbered types will
 * obscure icons of higher-numbered types.
 */
typedef enum {
  ICON_TYPE_CAR,    ///< the car icon
  ICON_TYPE_SAFETY, ///< icons representing speed cameras
  ICON_TYPE_TMC,    ///< icons representing TMC events
  ICON_TYPE_FLAG,   ///< icons representing journey flags
  ICON_TYPE_FAV,    ///< icons representing favourites
  ICON_TYPE_POI,    ///< icons representing POIs
  ICON_TYPE_CLUSTER,///< icons representing Cluster Edison add at  2008.8.21
  ICON_TYPE_GROUP,  ///< icons representing Group
  ICON_TYPE_25D_POI,///< icons representing 2.5D POIs
  ICON_TYPE_SEARCH_RESULT ///< icons representing search result @TPE
} TIconType;

// Forward declaration used in the following typedef
struct TIconDisplayRank;

/**
 * @internal
 * The type of functions that are called to obtain the bitmap for an
 * icon that is about to be rendered.
 */
typedef const TBitmap *TIconGetBitmapFunc(struct TIconDisplayRank *);

/**
 * @internal
 * @brief The number of bytes that specific types of icon can store in
 *        the TIconDisplayRank::extraInfo field.
 */
#define ICON_DISPLAY_RANK_EXTRA_INFO_SIZE 13

/**
 * @brief Holds information about an icon that is used to determine
 * whether it is obscured by other icons.
 *
 * @internal
 * When comparing icons to decide which is to obscure another, the
 * fields of each structure are compared in order, with the first to
 * have a lower valued byte being declared the winner.  This means that
 * all icons of a lower-numbered type will obscure all icons of a
 * higher-numbered type and that within a type, icons of lower
 * importance obscure those of higher importance.
 */
typedef struct TIconDisplayRank
{
  /// what type of icon it is
  TIconType type; 

  /// @internal
  /// within its type, how important it is (lower is more important)
  UINT8 iconImportance;
  /// @internal
  /// the screen coordinates that will be covered by the icon if it is shown
  TRect bounds;
  /// @internal
  /// the map coordinates of the centre of the icon
  TPos pos;
  /// @internal
  /// the z coordinate for 3D display, ignored for 2D
  INT32 depth;
  /// @internal
  /// function to get the bitmap for the icon
  TIconGetBitmapFunc *getBitmapFunc;
  UINT32 scaleFactor;
  /// @internal
  /// extra storage space for information specific to a particular type
  /// of icon NOTE: MUST BE ON 4 BYTE BOUNDARY FOR WINCE
  UINT32 extraInfo[(ICON_DISPLAY_RANK_EXTRA_INFO_SIZE + 3) >> 2];
  /// @internal
  /// ****** Placed after the extraInfo so extraInfo is on a 4 byte boundary
  UINT16 alpha;
  /// @internal
  /// the dimensions of the UN-SCALED bitmap around its (0, 0) anchor point
  TRect bitmapRect;
  /// @MIC Neo - to know POI is hide or not
  BOOL8 bVisible;
} TIconDisplayRank;
///
typedef struct TExplorePOI
{
  UTF16 poiName[POI_MAX_NAME_SIZE];
  TPos pos;
  UINT16 typeID;
  UINT32 poiID;
  UINT16 mapIndex;
  UINT16 brandID;
  char vendor[2];
} TExplorePOI;
///@TPE
// The following internal declarations are here because the definition
// of TViewport depends on them.

/**
 * @internal
 * @brief The maximum number of icons that can be displayed at once
 */
#define MAX_ICON_DISPLAY_RANKS 120

/**
 * @internal
 * @brief Holds visible icons, sorted by rank
 */
typedef struct TIconRankSet
{
  TIconDisplayRank iconRanks[MAX_ICON_DISPLAY_RANKS];
  UINT32 iconRankCount;
} TIconRankSet;

// declare global cluster add by Edison at 2008.9.19
extern TIconRankSet s_ClusterRankSet;

///@TPE

typedef struct TExploreSet
{
  TExplorePOI ePOIArray[MAX_ICON_DISPLAY_RANKS];
  UINT32 exploreCount;
} TExploreSet;

/**
 * @brief Returns the number of visible icons in the set
 *
 * @param[in] pThis a pointer to the icon display rank set
 * @return the number of icons in the set
 */
extern MAPCORE_API
UINT32 IconDisplayRankSet_GetCount(const TIconRankSet *pThis);
///@TPE

extern MAPCORE_API
TExploreSet Viewport_GetExplorePOI(struct TViewport *viewport);

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif
