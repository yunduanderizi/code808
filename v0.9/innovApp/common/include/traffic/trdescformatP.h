#ifndef TRDESCFORMATP_H
#define TRDESCFORMATP_H

#include <traffic/trapi.h>
#include <traffic/trlocalP.h>

BOOL8 tmcFormatDescription(const TtmcCompleteEvent *ce,
                           UTF16  description[TRAFFIC_EVENT_TEXT_STRINGS][TRAFFIC_EVENT_TEXT_LENGTH],
                           UTF16  extraInfo[TRAFFIC_EVENT_EXTRA_STRINGS][TRAFFIC_EVENT_TEXT_LENGTH]);

#endif
