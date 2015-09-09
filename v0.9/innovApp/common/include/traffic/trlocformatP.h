#ifndef TRLOCFORMATP_H
#define TRLOCFORMATP_H

#include <traffic/trapi.h>
#include <traffic/trloctableP.h>
#include <memory/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TtmcFmtLoc {
  UTF16 *text[TRAFFIC_LOCATION_FORMAT_NUM_PARTS];
  UTF16 *fromLocation;
  UTF16 *toLocation;
  TtmcLocationTypeCode locationType;
} TtmcFmtLoc;

BOOL8 locationFormat_format(const TtmcLocationFormat *format,
                            TtmcLocationFormatRuleDir dir,
                            const UTF16 *roadNumber,
                            const UTF16 *roadName,
                            const UTF16 *posEndName,
                            const UTF16 *negEndName,
                            const UTF16 *primFirstName,
                            const UTF16 *primSecondName,
                            const UTF16 *secFirstName,
                            const UTF16 *secSecondName,
                            TtmcFmtLoc *output,
                            TAllocatorHandle allocator);

#ifdef __cplusplus
}
#endif

#endif
