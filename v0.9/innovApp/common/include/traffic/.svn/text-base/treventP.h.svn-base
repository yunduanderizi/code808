#ifndef TREVENTP_H
#define TREVENTP_H

/* @file 
   This module contains the const definitions for extended TMC traffic events
   The basic event is encoded into an 11 bit number predefined by ISO
   specification 14819-2. The specification also contains additional info
   which is implicit in the event definition, including
     Event Class.. used for message management and display 
     Duration   .. events are classed as dynamic or long lasting. 
     Direction  .. Some events are implicitly bi-directional
     Urgency    .. There are 3 levels of urgency implicit in event type
     Nature     .. Information, forecast, or silent
   This extra information is combined into a 32 bit bitfield which becomes the 
   extended event desciptor
   It also contains the text descriptor definitions for each event. Events can have
   up to 3 text strings (with additional standard phrases) which describe them. 
   The text descriptors contain a phrase letter as well as an index number as per
   the ISO specification. */

#include <traffic/trlocalP.h>
#include <traffic/trapi.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Given an event ID direct from a traffic message, return
    an extended event descriptor
    @param ev This is a UINT16 representing the raw event ID from
    the tmc transmission

    @param exEvent This is a pointer to a TtmcExtendedEvent. The function
    will perform a lookup based on the incoming event, and populate this 
    structure with event definition information

    @return TRUE if event is a valid TMC event, FALSE if event is not included
    in ISO 14819-2
*/
BOOL8 tmcGetEvent (UINT16 ev, TtmcExtendedEvent *exEvent);

TAppTrafficEventClass  tmcGetApplicationClass (TtmcCompleteEvent *ev);

const TEventSupplementaryInfo *tmcGetSupplementaryInfo (TtmcExtendedEvent *ev);

#ifdef __cplusplus
}
#endif

#endif
