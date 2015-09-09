#ifndef LAGUIDES_H
#define LAGUIDES_H

#include <language/latypes.h>
#include <language/latokenlist.h>
#include <language/lalibtypes.h>
#include <mapdata/enroad.h>
#include <mapdata/enphone.h>
#include <math/geconvert.h>
#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Speech
/// @ingroup Language
/// @{ start of group

/// @brief
/// The maximum length of the buffer needed to contain the string
/// returned by the TLanguageTTSCallbackFunc.
#define TTS_MAX_BUFFER_LEN 256
/// This is the form of the callback function that is invoked for TTS.
///
/// @param[in] fullTtext Pointer to the text representation of the instruction.
/// @param[in] fullTextLength The length of the text representation.
///            It will be no longer than TTS_MAX_BUFFER_LEN-1 in length.
/// @param[in] shortTtext Pointer to the text representation of
///            the instruction.
///            This string has the interjections filtered out.
/// @param[in] shortTextLength The length of the text representation.
///            It will be no longer than TTS_MAX_BUFFER_LEN-1 in length.
/// @param[in] priority The priority of the instruction.
typedef void TLanguageTTSCallbackFunc( const UTF16 *fullText,
                                       UINT32 fullTextLength,
                                       const UTF16 *shortText,
                                       UINT32 shortTextLength,
                                       TSpeechPriority priority );

/// This is the form of the callback function that is invoked for playback
/// of tokens to form a spoken turn instruction. It is the same form as
/// the function os_playSpeechPrompt
/// 
/// @note
/// The token numbers are dependent on the currently loaded grammar.
/// 
/// @param[in] tokenList List of speech token numbers.
/// @param[in] numTokens The number of tokens in the speech token list.
/// @param[in] priority An indication of the urgency of the spoken
/// information.*/
typedef void TLanguageTokenCallbackFunc( UINT32 *tokenList,
                                         UINT32 numTokens,
                                         TSpeechPriority priority );


/// This func is used to set the output file name of token list dump, if do not set the output filename, the dump disable.
/// 
/// @param[in] pszFilename.
/// @param[in] numTokens The number of tokens in the speech token list.
/// @param[in] priority An indication of the urgency of the spoken
/// 
void TLanguageSetTokenListDump( wchar_t* pszFilename );

/// This is the log file written for token list.
/// 
/// @note
/// if szOutputFilename invalid, do not log anything.
/// 
/// @param[in] file name of log file.
/// @param[in] token list.
/// @return TRUE if the token list has dump success. Otherwise FALSE.
BOOL8 TLanguageTokenListDump( const wchar_t* szOutputFilename,
                             TSpeechTokenList* pTokenList,
							 TGrammarInterface* pGrammar,
							 TLanguageInstruction* pInstructions,
                             UINT32 nInstructionCount );

/// Render a list of instruction as spoken language.
/// e.g. "Continue three miles then turn right".
/// 
/// @param instructions Pointer to a list of instructions to render.
/// 
/// @param instructionCount The number of consecutive instructions to render.
/// 
/// @param priority The priority of these instructions.
extern MAPCORE_API
void language_speakInstruction( TLanguageInstruction* instructions,
                                UINT32 instructionCount,
                                TSpeechPriority priority );

extern MAPCORE_API
void language_speakInstructionForLMGD( TLanguageInstruction* instructions,
                                       UINT32 instructionCount,
                                       TSpeechPriority priority,
                                       UTF16 *landmarkName1,
                                       UTF16 *landmarkName2);

/// Gives a traffic instruction.
/// @param[in] data A pointer to the structure populated with data that 
/// describes the traffic event that has occured or changed

extern MAPCORE_API
void language_speakTrafficInstruction( TSpokenTrafficData* data );

/// Says a UTF-16 character.
/// What characters are supported will vary considerably by language.
/// 
/// @param c The character to speak
/// @return TRUE if the character was recognised, and spoken. Otherwise FALSE.
extern MAPCORE_API
BOOL8 language_speakCharacter( UTF16 c );

/// This registers the callback function that is invoked for TTS
/// 
/// @note You cannot use both a TTS callback function and a Token callback
/// function to deliver guidance at the same time. If a token callback has been
/// registered you cannot register a TTS callback until the token callback is
/// disabled using language_registerTokenCallback(NULL).
///
/// @param[in] func The callback to use when a spoken instruction is issued.
/// @return
///   - TRUE if the callback was registered successfully.
///   - FALSE if the callback was not registered.
extern MAPCORE_API
BOOL8 language_registerTTSCallback( TLanguageTTSCallbackFunc* func );

/// This registers the callback function that is invoked for playback of tokens.
///
/// @note You cannot use both a TTS callback function and a Token callback
/// function to deliver guidance at the same time. If a TTS callback has been
/// registered you cannot register a token callback until the TTS callback is
/// disabled using language_registerTTSCallback(NULL).
///
/// @param[in] func The callback to use when a token list is ready to be rendered.
/// @return
///   - TRUE if the callback was registered successfully.
///   - FALSE if the callback was not registered.
extern MAPCORE_API
BOOL8 language_registerTokenCallback( TLanguageTokenCallbackFunc* func );

//@MIC Start
/// for speed camera speech out.
///
/// @param[in] type of speed-camera.
/// @param[in] distance from current pos to speed-camera.
/// @param[in] units of distance.
/// @param[in] speed limit for speed-camera.
/// @param[in] units of speed limit for speed-camera.
/// @return
///   - TRUE if the speech successfully.
///   - FALSE if the speech failure.
extern MAPCORE_API
BOOL8 language_speechSpeedCamera(TPOITypeId16 speedCamType,
                                 UINT32 nDistance, 
                                 TDistanceUnits distanceUnits, 
                                 UINT32 nSpeedLimit, 
                                 TDistanceUnits speedUnits);
//@MIC End
/// @}

#ifdef __cplusplus
}
#endif

#endif
