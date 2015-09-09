#ifndef TRSORTP_H
#define TRSORTP_H

#include <traffic/trapi.h>
#include <traffic/trlocformatP.h>
#include <traffic/trloctableP.h>

/// @brief Information used for sorting a formatted location.
typedef struct TtmcSortFmtLoc {
  TtmcFmtLoc fmtLoc;            ///< formatted location text for sorting
  BOOL8 isRoad;                 ///< does it actually name a road?
  UINT8 rank;                   ///< sort group
  TtmcLocationFormatPart part;  ///< part of location to sort
} TtmcSortFmtLoc;

void parseFmtLocSort(const TtmcRoadNameSortRules *rules,
                     TtmcSortFmtLoc *fmtLocSort);

INT32 compareFmtLocSorts(const TtmcSortFmtLoc *rn1, const TtmcSortFmtLoc *rn2);

UINT16 tmcSortEventList(TTrafficEventSortOrder  sortOrder,
                        const TJourney         *journey,
                        const TPos             *mapPos,
                        TTrafficEventId        *eventIds,
                        UINT16                  maxEvents,
                        UINT16                 *numOnJourney);

#endif
