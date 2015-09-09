#ifndef LAINIT_H
#define LAINIT_H

#include <language/lalibtypes.h>
#include <language/latypes.h>
#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Language
/// These functions allow the map engine to utilise maps that use
/// different map encodings, and that contain multiple languages.
/// The language encoding and spoken language can easily be
/// selected.
/// @{

/** This is the callback function that updates the text strings
    within the application.  It should rely on querying the
    language module ( using language_getLanguageCode ) to find
    the current language, and load the appropriate language
    strings based on that. */
typedef void TStringsCallbackFunc( void );

/*----------------------------------------------------------------*/

/** Initialise the language engine.  Until a grammar is loaded, and when no 
    grammar is loaded, the language code is set to UK English.

    @return TRUE if language initialisation successful. */
extern MAPCORE_API
BOOL8 language_init(void);

/** @brief Checks if a speech file is supported by the currently-loaded 
           grammar.

    This function will check if the speech file version and language code that
    are passed in are supported by the currently-loaded grammar. A grammar 
    will support a single language code, but may support a range of speech 
    file versions.

    Supported speech file version is separate from the grammar version.

    @param speechFileVersion  The version of the speech file.
    @param speechFileLangCode The language code of the speech file.

    @return TRUE if the speech file is compatible, FALSE otherwise. */
extern MAPCORE_API BOOL8
language_isSpeechFileCompatible (UINT16 speechFileVersion,
                                 TLanguageCode speechFileLangCode);

/** @brief Gets information about the currently-loaded grammar.

    @note If no grammar is currently loaded, the flag TGRAMMARCAPS_NOGRAMMAR
          will be set in the grammar capabilities value.

    @note This information will be provided by the grammar library, but if
          suitable checks for compatibility have been made (i.e version and
          language code match), should also apply to the speech data file.

    @param grammarInfo Pointer to a #TGrammarInfo structure to fill with
                       information. */
extern MAPCORE_API void
language_getGrammarInfo (TGrammarInfo *grammarInfo);

/** Returns the current language code.

    @return The language code which is currently set. */
extern MAPCORE_API
TLanguageCode language_getLanguageCode( void );

/** @brief Loads a language library into innovEngine.

    The current grammar used by innovEngine is set by this function. It uses the
    table of function pointers to set the grammar.

    @note To unload a grammar, pass NULL into this function.

    @note This will also set the language code returned by
          #language_getLanguageCode to the language code of this grammar.

    @param newGrammarInterface A pointer to a #TGrammarInterface structure
                               containing the necessary function pointers.

    @return TRUE if the library was successfully loaded, FALSE otherwise. */
extern MAPCORE_API BOOL8
language_loadGrammar (TGrammarInterface *newGrammarInterface, const UTF16* szLookupTable);

/***
release the memory for lookup table.
**/
extern MAPCORE_API BOOL8
language_closeGrammar (void);

/** Sets a callback function to load the menu strings when the language
    code is changed. */
extern MAPCORE_API
void language_setMenuStringsCallback (TStringsCallbackFunc* func);


BOOL8 isCHNTWNSKU(void);
/// @}

#ifdef __cplusplus
}
#endif

#endif
