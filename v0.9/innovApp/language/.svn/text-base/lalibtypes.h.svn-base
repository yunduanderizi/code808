#ifndef LALIBTYPES_H
#define LALIBTYPES_H

#include <platform_types.h>
#include <language/latypes.h>
#include <language/latokenlist.h>
#include <language/ConvertUTF.h>
#include <math/geconvert.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup LanguageLibrary
/// @{

/// @typedef enum TGrammarCapabilities
/// Flags describing what a grammar is capable of.
typedef UINT32 TGrammarCapabilities;

/// #TGrammarCapabilities flag indicating no grammar loaded
#define TGRAMMARCAPS_NOGRAMMAR      0x80000000

/// @brief Stores information about a grammar.
typedef struct TGrammarInfo {
  /// The language code of this grammar
  TLanguageCode languageCode;
  /// The interface version the library was compiled with
  UINT16 intfVersion;
  /// The vocabulary version
  UINT16 vocabVersion;
  /// Flags indicating capabilities this grammar supports
  TGrammarCapabilities capabilities;
} TGrammarInfo;

/// @brief Function pointers to the support functions necessary for a grammar
///        to operate.
typedef struct TGrammarCallbacks {
  BOOL8  (*distance_isZero)                  (const TUserDistance*);
  BOOL8  (*language_isTTSEnabled)            (void);
  void   (*speechTokenList_add)              (TSpeechTokenList*, INT32);
  BOOL8  (*speechTokenList_addInterjection)  (TSpeechTokenList*, const UTF16*);
  BOOL8  (*speechTokenList_addPunctuation)   (TSpeechTokenList*, TPunctuation);
  void   (*speechTokenList_delete)           (TSpeechTokenList*, INT32);
  INT32  (*speechTokenList_get)              (TSpeechTokenList*, INT32);
  INT32  (*speechTokenList_numTokens)        (TSpeechTokenList*);
  void   (*speechTokenList_replace)          (TSpeechTokenList*, INT32, INT32);
  UTF16* (*utf16_strcat)                     (UTF16*, const UTF16*);
  UTF16* (*utf16_strcpy)                     (UTF16*, const UTF16*);
  UINT32 (*utf16_strlen)                     (const UTF16*);
  UTF16* (*utf16_strnzcat)                   (UTF16*, const UTF16*, INT32);
  UTF16  (*utf16_toupper)                    (UTF16);
  UTF16  (*utf16_tolower)                    (UTF16);
  UTF16* (*utf16_itoa)                       (INT32, UTF16 *);
  UTF16* (*utf16_strstr)                     (const UTF16 *, const UTF16 *);
} TGrammarCallbacks;

typedef BOOL8 TLangLibCharacterAsTokens (TSpeechTokenList *tokenList, UTF16 c);
typedef void TLangLibGetGrammarInfo (TGrammarInfo *info);
typedef void TLangLibGuideInstructionsAsText (const TLanguageInstruction* pInstructions,
                                              const UINT32 instructionCount,
                                              UTF16 *pUTF, const UINT32 maxLength);
typedef void TLangLibGuideInstructionsAsTokens (TSpeechTokenList *tokenList,
                                                TLanguageInstruction *instructions,
                                                UINT32 instructionCount);
typedef BOOL8 TLangLibIsSpeechFileCompatible (UINT16 speechFileVersion,
                                              TLanguageCode speechFileLangCode);
typedef BOOL8 TLangLibInit (TGrammarCallbacks *callbacks);
typedef void TLangLibTrafficInstructionAsTokens (TSpeechTokenList *tokenList,
                                                 TSpokenTrafficData *trafficData);
typedef BOOL8 TLangLibTokenAsText (UTF16* dest, UINT16 index, UINT32 maxLength);
typedef BOOL8 TLangLibPhraseAsText (const TLanguagePhrase* phrase,
                                    UTF16* text, UINT32 maxLength);
typedef BOOL8 TLangLibCountryNameAsText (TNumericISOCountryCode numericISOCountryCode,
                                         UTF16* text, UINT32 maxLength);
typedef BOOL8 TLangLibInitLookup (const UTF16* szFilepath);

typedef BOOL8 TLangLibClose (void);

/// Interface versions
#define LANGLIBINTF_V1_0_VERSION     10
#define LANGLIBINTF_V1_1_VERSION     101
/// Current version used by innovEngine
#define LANGLIBINTF_CURRENT_VERSION   LANGLIBINTF_V1_1_VERSION

/** Interface between innovEngine and a language grammar. 

    Mapcore will use the function pointers contained in this table to call
    grammar functionality for assembling instructions to be spoken or displayed
    on screen. */
typedef struct TGrammarInterface {
  TLangLibCharacterAsTokens          *characterAsTokens;
  TLangLibGetGrammarInfo             *getGrammarInfo;
  TLangLibGuideInstructionsAsText    *guideInstructionsAsText;
  TLangLibGuideInstructionsAsTokens  *guideInstructionsAsTokens;
  TLangLibIsSpeechFileCompatible     *isSpeechFileCompatible;
  TLangLibInit                       *init;
  TLangLibTrafficInstructionAsTokens *trafficInstructionAsTokens;
  TLangLibTokenAsText                *tokenAsText;
  TLangLibPhraseAsText               *phraseAsText;
  TLangLibCountryNameAsText          *countryNameAsText;
  TLangLibInitLookup                 *initLookup;
  TLangLibClose                      *close;
} TGrammarInterface;

/// @}

#ifdef __cplusplus
}
#endif

#endif // LALIBTYPES_H
