#ifndef DIICONRANKLISTP_H
#define DIICONRANKLISTP_H

#include <display/diiconranklist.h>
#include <display/di3Dengi.h>

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

/**
 * @brief Populates a \c TIconDisplayRank structure
 *
 * @param[in] pThis a pointer to the icon display rank to populate
 * @param[in] type the type of icon
 * @param[in] importance the relative importance of the icon within its
 *                       type; smaller numbers indicate greater
 *                       importance
 * @param[in] pos_s a pointer to the screen coordinates, including z for
 *                  3D, of the centre of the icon
 * @param[in] mapPos_w a pointer to the map coordinates of the centre of
 *                     the icon
 * @param[in] getBitmapFunc a pointer to the function to be used to
 *                          obtain the bitmap for the icon if it is to
 *                          be displayed
 */
extern void IconDisplayRank_Define(TIconDisplayRank   *pThis,
                                   TIconType           type,
                                   UINT8               importance,
                                   const TPos3d       *pos_s,
                                   const TPos         *mapPos_w,
                                   UINT32              scaleFactor,
                                   INT16               alpha,
                                   TIconGetBitmapFunc *getBitmapFunc,
                                   const TRect        *bitmapRect);

/**
 * @brief Retrieves the extra info stored in an icon display rank
 *
 * @par Example
 *
 * @code
 * TIconDisplayRank *rank;
 * TSomeIconInfo *info;
 *
 * info = (TSomeIconInfo *)IconDisplayRank_GetExtraInfo(rank));
 * @endcode
 *
 * @param[in] pThis a pointer to the icon display rank
 * @return a pointer to the extra info stored in the icon display rank
 */
extern const void *IconDisplayRank_GetExtraInfo(const TIconDisplayRank *pThis);

/**
 * @brief Stores extra info in an icon display rank
 *
 * The supplied info is copied into the extra info storage area of the
 * icon display rank.
 *
 * @par Example
 *
 * @code
 * TIconDisplayRank rank;
 * TSomeIconInfo info;
 *
 * IconDisplayRank_StoreExtraInfo(&rank, &info, sizeof(info));
 * @endcode
 *
 * @param[in] pThis a pointer to the icon display rank in which to store the
 *              info
 * @param[in] extraInfo a pointer to the extra info to store
 * @param[in] infoSize the size of the extra info.  Must be <= \c
 *                 ICON_DISPLAY_RANK_EXTRA_INFO_SIZE
 */
extern void IconDisplayRank_StoreExtraInfo(TIconDisplayRank *pThis,
                                           void *extraInfo,
                                           UINT32 infoSize);

// END PRIVATE

// BEGIN PRIVATE

/**
 * @brief Renders the icons in an icon rank set to a viewport
 * Must be called once for each icon type.
 *
 * @param[in] pThis a pointer to the icon display rank set
 * @param[in] viewport the viewport defining the visible part of the map
 * @param[in] iconType specifies which type of icon to render
 */
extern void IconDisplayRankSet_Render(TIconRankSet *pThis,
                                      struct TViewport *viewport,
                                      TIconType iconType);

// for enclosure function
extern void IconDisplayRankSet_Render2(TIconRankSet *pThis,
                                      struct TViewport *viewport,
                                      TIconType iconType);

/**
 * @brief Empties an icon rank set
 *
 * @param[in] pThis a pointer to the icon display rank set
 */
extern void IconDisplayRankSet_Reset(TIconRankSet *pThis);

/**
 * @brief Considers an icon for addition to an icon rank set
 *
 * The icon will only actually be added if it is not obscured by a
 * higher priority icon.  Also, if the icon is added, any lower priority
 * icons that it obscures will be removed from the set.
 *
 * @param[in] pThis a pointer to the icon display rank set
 * @param[in] displayRank a pointer to the icon to consider for
 *                        addition.  If the icon is added then its
 *                        display rank structure is copied into the set.
 */
extern void IconDisplayRankSet_addToRankList(TIconRankSet *pThis,
                                             TIconDisplayRank *displayRank);

/**
* @brief Considers an icon for addition to an icon rank set
*
* The icon will only actually be added if it is not obscured by a
* higher priority icon.  Also, if the icon is added, any lower priority
* icons that it obscures will be removed from the set.
* Edison add at 2008.8.21
*
* @param[in] pThis a pointer to the icon display rank set
* @param[in] displayRank a pointer to the icon to consider for
*                        addition.  If the icon is added then its
*                        display rank structure is copied into the set.
*/
extern void IconDisplayRankSet_addToRankListEx(TIconRankSet *pThis,
                                               TIconDisplayRank *displayRank);
/**
 * @brief Retrieves the icon with the given index from the set
 *
 * @param[in] pThis a pointer to the icon display rank set
 * @param[in] index the zero-based index of the icon to retrieve; must
 *                  be stricly less than the value returned by \c
 *                  IconDisplayRankSet_GetCount
 * @return a pointer to the indexth icon
 */
extern const TIconDisplayRank *IconDisplayRankSet_GetVisibleIcon(
  const TIconRankSet *pThis, UINT32 index);

/**
 * @brief Finds the icon, if there is one, that is visible and that
 *        overlaps the given screen position.
 *
 * There can only ever be zero or one icons visible at any screen
 * position due to the nature of the icon rank set data structure.
 *
 * @todo How does this work in 3D given that the approximation used for
 *       the icon's bounds allows near icons to overlap?
 *
 * @param[in] pThis a pointer to the icon display rank set
 * @param[in] pos_s a pointer to the screen position at which to find an
 *                  icon
 * @return a pointer to the icon that is visible at \p pos_s, or \c NULL
 *         if there is no such icon
 */
extern const TIconDisplayRank *IconDisplayRankSet_FindVisibleIconAtScreenPos(
  const TIconRankSet *pThis, const TPos *pos_s);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DIICONRANKLISTP_H */
