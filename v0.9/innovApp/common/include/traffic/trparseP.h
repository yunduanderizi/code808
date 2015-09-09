#ifndef TRPARSEP_H
#define TRPARSEP_H

#include <mapdata/enroadP.h>
#include <traffic/trlocalP.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup TrafficParsing
/// @ingroup Traffic
/// @brief RDS-TMC message parsing
///
/// Takes as input an 8 byte RDS message and dissects it to see if it is worth
/// having. Passes the results on to whatever section of code is interested.
///
/// @{

  /** Analyse the raw RDS message from the hardware / NMEA stream

    @param msg  A pointer to the raw 8 byte RDS message

    @param result A pointer to the standard result structure

  */

void tmcProcessTMCMessage (TtmcMessage *msg, TtmcResult *result);

BOOL8 tmcIsEncrypted (void);
BOOL8 tmcCanDecrpyt (void);

/// @brief Perform any required translations of a location table
///        number
///
/// This takes account of stations using the TMC Forum's interim
/// encryption method, i.e. transmitting a different location table
/// number from that required to decode their messages.
///
/// @param countryCode the country code in which to interpret the
///                    location table number
/// @param         ltn the location table number as broadcase
///
/// @return the location table number to use for decoding messages
///
UINT8 tmcTranslateLTN(UINT8 countryCode, UINT8 ltn);

/// @brief Retrieve the translated LTN of the current transmission
///
/// This location table number is suitable for decoding TMC messages,
/// i.e. any necessary translations have already been performed.
///
/// @return the location table number to use for decoding messages
///
UINT8 tmcCurrentLTN (void);
UINT8 tmcCurrentExtendedCountryCode(void);
const TCountryCode *tmcCurrentMapCountryCode(void);
UINT8 tmcCurrentCountryCode(void);
UINT16 tmcCurrentpiCode (void);

/// @brief Extract the country code from a European PI code
///
/// Note that this will return nonsense if passed a US PI code.
///
/// @param pi the PI code
///
/// @return the country code (the most significant four bits)
///
UINT8 tmcCountryCodeFromPI(UINT16 pi);

void tmcSetLocationTableFlag (BOOL8 flagValue);
//@MIC
//To get location table flag
BOOL8  tmcGetLocationTableFlag (void);
void tmcClearServiceParameters (void);

void tmcWakeup (void);

UINT8 routeLengthKm (UINT8 rawValue);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* TRPARSEP_H */
