#ifndef LACOUNTRYTP_H
#define LACOUNTRYTP_H

#include <language/lacountryt.h>
#include <language/latypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Generate a text representation of a localised country name into
///        a string buffer.
///
/// @param numericISOCountryCode The 3-digit numeric ISO country code.
/// @param text      A pointer to a string in which to store the country name.
/// @param maxLength The maximum length of #text.
BOOL8 language_countryNameAsText( TNumericISOCountryCode numericISOCountryCode,
                                  UTF16* text,
                                  UINT32 maxLength );

#ifdef __cplusplus
}
#endif

#endif /* LACOUNTRYTP_H */
