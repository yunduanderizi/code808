#ifndef TRRECEIVEP_H
#define TRRECEIVEP_H

#include <traffic/trreceive.h>
#include <locationprovider/gpparse.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup TrafficReceiver
/// @ingroup Traffic
/// @brief Interface to the mythical TMC receiver. At present unsure
///        of what goes in here.
///
/// @{

BOOL8 tmcCheckLocationFile(void);

void processTrafficMessage (TtmcMessageType msg);

  /** Format and transmit a command string to the TMC receiver
      hardware (proprietary NMEA message)

      @param cmd  A pointer to the command string to be transmitted
                  (does not include NMEA header or checksum)  */

void tmcSendCommand (const char *cmd);

// return age in minutes of last new or updated message
UINT16 tmcGetLastUpdateAge (void);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* TRRECEIVEP_H */
