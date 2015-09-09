#ifndef RTJOURNEYP_H
#define RTJOURNEYP_H

#include <routing/rtjourney.h>
#include <containers/gestaticheapP.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/* JOURNEY FUNCTIONS */
/*----------------------------------------------------------------*/
// Determine the maximum number of legs a journey can hold
//
// @param pThis Journey to query
// @return The max number of legs possible
TJourneyLegNumber journey_getMaximumLegs( 
  const TJourney* pThis );

// Set the path of a particular leg to the free segment path
//
// @param[in] pThis Journey containing the leg
// @param[in] legNumber Leg index of leg whose path should be set
// @return    FALSE if legNumber is invalid, TRUE otherwise
BOOL8 journey_setLegPathToFreeSegmentPath(
  TJourney* pThis,
  TJourneyLegNumber legNumber );

// Retrieve the free segment path.  A special path containing all the
// free space in a journeys segment stores
//
// @param[in]  pThis Journey containing the path
// @param[out] path Pointer to retrieved path
void journey_getFreeSegmentPath(
  TJourney* pThis,
  TPath** path );

/*----------------------------------------------------------------*/
/// Retrieve the path for a particular leg of the journey
///
/// NOTE: just because a path exists, that does not mean the path has a
/// valid set of path segments.  The path result needs to be check to
/// determine that.
///
/// NOTE: This is a convenience API only to simplify access to new APIs.
///
/// @param[in]  pThis Journey containing the path
/// @param[in]  legNumber Leg index of path to retrieve
/// @param[out] path Pointer to retrieved path
/// @return     TRUE if legNumber was valid, FALSE otherwise
BOOL8 journey_getLegPath(
  const TJourney* pThis,
  TJourneyLegNumber legNumber,
  const TPath** path );

/*----------------------------------------------------------------*/
/// Get the previous leg number of a journey if a valid one exists
///
/// Given a "current" leg number, it returns the number of the previous
/// one if
/// -- the journey exists
/// -- the current leg number is not the first leg (i.e., legNumber > 0)
/// -- the current leg is valid
/// -- the current leg is not a reroute, which would indicate the previous
///    leg is not contiguous with the current one.
/// -- the previous leg is valid
///
/// If any of these conditions is false, then the routine returns
/// FALSE, and previousLegNumber is set to legNumber,
///
/// If all of these conditions are true, then the routine returns
/// TRUE, and previousLegNumber is set to legNumber - 1
/// 
/// @param[in]  pThis Journey containing the legs in question
/// @param[in]  legNumber Leg index into the journey
/// @param[out] previousLegNumber returns the previous
///             leg number if such a leg exists, is valid, and
///             is contiguous with the current leg. If not,
///             legNumber is returned. Note that previousLegNumber
///             will be set if the routine returns TRUE.
///             
/// @return     TRUE if the routine determines that a previous
///             leg exists and is contiguous with the
///             current leg, FALSE otherwise.
BOOL8 journey_getPreviousLeg(
  const TJourney* pThis,
  TJourneyLegNumber legNumber,
  TJourneyLegNumber *previousLegNumber);

/*----------------------------------------------------------------*/
/// @brief Initialises the per-journey foreign storage system.
///
/// This must be called before any foreign storage is registered.
/// Calling this a second and subsequent times will remove any
/// registered foreign storage and render all existing journey objects
/// unusable until they are reinitialised.
///
/// @return \c TRUE if the foreign storage system was successfully
///         initialised, otherwise \c FALSE
BOOL8 journey_initForeignStorage(void);

/*----------------------------------------------------------------*/
/// @brief Registers some per-journey foreign storage.
///
/// Once a foreign store is registered, each journey object will have
/// some storage corresponding to the static heap block returned by
/// this function.  Given the static heap block and a journey, the
/// storage can be retrieved using #staticHeap_resolveBlock, like so:
///
/// @code
/// typedef struct TStuff {
///   // ...
/// } TStuff;
///
/// // ...
///
/// TStaticHeapBlock *shb;
///
/// shb = journey_registerForeignStore("something", sizeof(TStuff),
///                                    NULL, NULL);
/// // ...
///
/// TStuff *stuff = (TStuff *) staticHeap_resolveBlock(
///   &journey->foreignStorage, shb);
/// @endcode
///
/// @param owner as for #staticHeapTemplate_add
/// @param size  as for #staticHeapTemplate_add
/// @param clear as for #staticHeapTemplate_add
/// @param clone as for #staticHeapTemplate_add
///
/// @return a static heap block that can be used in conjunction with a
///         journey's #foreignStorage field to obtain the per-journey
///         foreign store
TStaticHeapBlock *journey_registerForeignStore(
  const char *owner,
  UINT32 size,
  TStaticHeapBlockClearFunc *clear,
  TStaticHeapBlockCloneFunc *clone);

#ifdef __cplusplus
}
#endif

#endif // RTJOURNEYP_H
