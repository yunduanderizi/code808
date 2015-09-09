#ifndef LAPHRASETP_H
#define LAPHRASETP_H

#include <language/laphraset.h>
#include <language/latypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Generate a text representation of a phrase into a string buffer.
///
/// @param phrase    A pointer to a structure that identifies which phrase to
///                  build and the positional arguments for that phrase.
/// @param text      A pointer to a string in which to store the phrase text.
/// @param maxLength The maximum length of #text.
void language_phraseAsText( const TLanguagePhrase* phrase,
                            UTF16* text,
                            UINT32 maxLength );

#ifdef __cplusplus
}
#endif

#endif /* LAPHRASETP_H */
