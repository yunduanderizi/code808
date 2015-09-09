#ifndef LALIB_H
#define LALIB_H

#if defined (WIN32) || defined (WINCE)
  #ifdef LANGLIB_EXPORTS
    #define LANGLIB_API __declspec(dllexport)
  #else
    #define LANGLIB_API __declspec(dllimport)
  #endif
#else
  #define LANGLIB_API
#endif

#include <language/lalibtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup LanguageLibrary
/// @{

/// @brief Return the token(s) to say the provided character.
///
/// @param tokenList A pointer to the speech token list to add the tokens to.
/// @param c         The character to get tokens for.
///
/// @return TRUE if the character was pronouncable (in which case #tokenList
///         will contain the necessary token(s)), FALSE if this grammar cannot
///         pronounce the requested token.
LANGLIB_API BOOL8
langlib_characterAsTokens (TSpeechTokenList *tokenList,
                           UTF16 c);

/// @brief Get the grammar info for this grammar.
///
/// @param info Pointer to a #TGrammarInfo structure which will be filled with
///             the information about this grammar.
LANGLIB_API void
langlib_getGrammarInfo (TGrammarInfo *info);

/// @brief Get a brief text summary of a guidance instruction.
///
/// This function will fill #pUTF with a summarised instruction. Although it
/// will accept a list of instructions, only the first instruction will be
/// used in the result.
///
/// @param pInstructions    A pointer to a list of instructions.
/// @param instructionCount The number of instructions in the list.
/// @param pUTF             A pointer to a string in which to store the result.
/// @param maxLength        The length of the string pointed to by #pUTF.
LANGLIB_API void
langlib_guideInstructionsAsText (const TLanguageInstruction* pInstructions,
                                 const UINT32 instructionCount,
                                 UTF16 *pUTF,
                                 const UINT32 maxLength);

/// @brief Get a list of tokens to say some guidance instructions.
///
/// This function will fill #tokenList with the token indices that can be used
/// with a speech file or TTS voice from the same language to speak the
/// instructions. All instructions in the provided list will be used.
///
/// @param tokenList        A token list in which to store the result.
/// @param instructions     A list of instructions to speak.
/// @param instructionCount The number of instructions in the list.
LANGLIB_API void
langlib_guideInstructionsAsTokens (TSpeechTokenList *tokenList,
                                   TLanguageInstruction *instructions,
                                   UINT32 instructionCount);

/// @brief Checks if a speech file is supported by the grammar.
///
/// This function will check if the speech file version and language code that
/// are passed in are supported by the grammar. A grammar will support a single
/// language code, but may support a range of speech file versions.
///
/// Supported speech file version is separate from the grammar version.
///
/// @param speechFileVersion  The version of the speech file.
/// @param speechFileLangCode The language code of the speech file.
///
/// @return TRUE if the speech file is compatible, FALSE otherwise.
LANGLIB_API BOOL8
langlib_isSpeechFileCompatible (UINT16 speechFileVersion,
                                TLanguageCode speechFileLangCode);

/// @brief Initialises the grammar before first use.
///
/// Gives the grammar to perform any initialisation it needs to do before it
/// is used. Also sets the callbacks to support functions used by the grammar.
///
/// @param callbacks A TGrammarCallbacks containing the callback addresses.
///
/// @return TRUE if the callbacks were successfully set, FALSE otherwise.
LANGLIB_API BOOL8
langlib_init (TGrammarCallbacks *callbacks);

/// @brief Gets a list of tokens to say a traffic instruction.
///
/// Turns a traffic instruction structure into a list of token indices that can
/// be used with a speech file or TTS voice from the same language to speak the
/// instruction.
///
/// @param tokenList   A token list in which to store the result.
/// @param trafficData The traffic information to say.
LANGLIB_API void
langlib_trafficInstructionAsTokens (TSpeechTokenList *tokenList,
                                    TSpokenTrafficData *trafficData);

/// @brief Concatenate the text representation of a token onto a string.
///
/// Each token index corresponds to both a voice sample from a speech file and
/// a text string stored in the grammar library. This function will retrieve
/// the text string for a given token index and concatenate it onto the string
/// stored in dest.
///
/// @param dest      A pointer to a string in which to store the result.
/// @param index     The index of the token string to retrieve.
/// @param maxLength The maximum length of #dest.
///
/// @return TRUE if the token was found and copied into #dest, FALSE otherwise.
LANGLIB_API BOOL8
langlib_tokenAsText (UTF16* dest,
                     UINT16 index,
                     UINT32 maxLength);

/// @brief Generate a text representation of a phrase into a string buffer.
///
/// @param phrase    A pointer to a structure that identifies which phrase to
///                  build and the positional arguments for that phrase.
/// @param text      A pointer to a string in which to store the phrase text.
/// @param maxLength The maximum length of #text.
///
/// @return TRUE if the phrase is supported by the language library, FALSE otherwise.
LANGLIB_API BOOL8
langlib_phraseAsText (const TLanguagePhrase* phrase,
                      UTF16* text,
                      UINT32 maxLength);

/// @brief Generate a text representation of a localised country name into
///        a string buffer.
///
/// @param numericISOCountryCode The 3-digit numeric ISO country code.
/// @param text      A pointer to a string in which to store the country name.
/// @param maxLength The maximum length of #text.
///
/// @return TRUE if the country name is supported by the language library,
///         FALSE otherwise.
LANGLIB_API BOOL8
langlib_countryNameAsText( TNumericISOCountryCode numericISOCountryCode,
                           UTF16* text,
                           UINT32 maxLength );


/// @brief Lood a lookup.bin file for local language's grammar
///
/// @param szFilepath file path of lookup.bin file.
///
/// @return TRUE when the lookup.bin file has been loaded,
///         FALSE otherwise.
LANGLIB_API BOOL8
langlib_initLookup (const UTF16* szFilepath);

/// @release the loaded lookup.bin
///
/// @return TRUE
LANGLIB_API BOOL8
langlib_close (void);


/// @}

#ifdef __cplusplus
}
#endif

#endif // LALIB_H
