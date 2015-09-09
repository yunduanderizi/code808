#ifndef TRTUNEP_H
#define TRTUNEP_H

#include <traffic/trlocalP.h>
#include <traffic/trapi.h>
#include <memory/allocator.h>
#include <display/diviewport.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup TrafficTuning
/// @ingroup Traffic
/// @brief Handles tuning and re-tuning of the TMC receiver, as well
///        as a copy of the current station list.
///
/// @{

// Tuning states
typedef enum TUNING_STATE
{
  tmcTuneNoHardware = 0,
  tmcTuneIdle,        // on Power Up only, collecting stationlist
  tmcTuneTuning,      // generic re-tune state (hopefully with alternates received)
  tmcTuneReceiving    // data reception state
} TUNING_STATE;

void tmcInitialiseTuner (char *rootPath);

TUNING_STATE tmcGetTuneState(void);

UINT16 tmcGetCurrentFrequency(void);

  /** 
     Process the latest status update from the TMC hardware

     @param frequency   The currently tuned station frequency
     @param blockCount  No of blocks received in last second
     @param errorRate   % blocks received in error    */

void tmcProcessStatusMessage (UINT8 hwPresent, UINT16 frequency, UINT8 blockCount, UINT8 trfCount, UINT8 errorRate);

  /** 
     Process the RDS-TMC tuning variant messages. These hold
     the information required to allow us to chage to stations
     broadcasting the same traffic data

     @param msg   Pointer to the raw traffic message data (8 bytes)
     @param result  Pointer to the processing result structure */

void tmcProcessTuningMessage (TtmcMessage *msg, TtmcResult *result);

  /** 
     Process the result of the last tuning command sent to the
     RDS-TMC hardware
     @param result   UINT8 0/1        */

void tmcProcessTuningResult (UINT8 result);

UINT8 tmcTuneStationCount (void);

BOOL8 tmcTuneStationEntry (UINT8 index, TAppTrafficStationList *station);

BOOL8 tmcTuneToStation (TAppTrafficStationList *station);

BOOL8 tmcLockToStation (TAppTrafficStationList *station);

BOOL8 tmcRetune (void);

BOOL8 tmcSetPreferredStation (UINT16 picode);

void tmcUpdateStationList (TAppTrafficStationList *station);

void tmcSetStationTMCFlag (UINT16 stationPI, TAppStationTMCStatus newStatus);

void tmcSetEventSource (UINT16 piCode, UTF16 **sourceName, TAllocatorHandle allocator);

void tmcUpdateSignalStrength (UINT16 freq, UINT16 piCode, UINT8 signal);

void tmcGetLastStats (UINT16 *rdsCount, UINT16 *tmcCount);

BOOL8 tmcBandScan (void);

//@MIC to display station frequency and strength
void tune_render(const TViewport *viewport);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* TRTUNEP_H */
