#ifndef LAGUIDESP_H
#define LAGUIDESP_H

#include <language/laguides.h>
#include <language/latokenlist.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL8 language_tokenListAsText (const TSpeechTokenList *pTokenList,
                                BOOL8 filter,
                                UTF16* textResult,
                                UINT32 maxLength,
                                UINT32 *pResultLength);

BOOL8 language_tokenListAsTextForLMGD (const TSpeechTokenList *pTokenList,
                                BOOL8 filter,
                                UTF16* textResult,
                                UINT32 maxLength,
                                UINT32 *pResultLength,
                                UTF16* landmarkName1,
                                UTF16* landmarkName2);

void language_prepareTextForTTS( const UTF16 *inputText,
                                 UINT32      inputTextLength,
                                 UTF16       *outputText,
                                 UINT32      *pOutputTextLength );

/// Returns TRUE if TTS output is currently enabled.
///
/// @return TRUE if TTS output is current enabled on the system.
BOOL8 language_isTTSEnabled( void );

#ifdef __cplusplus
}
#endif

#endif /* LAGUIDESP_H */
