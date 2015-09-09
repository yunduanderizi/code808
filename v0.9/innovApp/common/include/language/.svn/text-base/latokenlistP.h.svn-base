#ifndef LATOKENLISTP_H
#define LATOKENLISTP_H

#include <language/ConvertUTF.h>
#include <language/latokenlist.h>
#include <language/laguides.h>
#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialises the token list
extern void speechTokenList_init( TSpeechTokenList *pThis );

// Adds a token to the token list
extern void speechTokenList_add( TSpeechTokenList *pThis, 
                                INT32 tokenNum );

// Adds a string to the token list
// For limited vocabulary systems, this token will be ignored
// The optionalString parameter is the text to add when rendering 
// unlimited vocabulary systems (eg: TTS)
extern BOOL8 speechTokenList_addInterjection( TSpeechTokenList *pThis, 
                                              const UTF16* interjectionString );

// Play all the tokens in the token list
// Optional strings (added with speechTokenList_addInterjection) are ignored
extern void speechTokenList_play( TSpeechTokenList *pThis,
                                  TSpeechPriority priority,
                                  TLanguageTokenCallbackFunc tokenCallback );

// Delete a token from the token list
extern void speechTokenList_delete(TSpeechTokenList *pThis, 
                                   INT32 tokenNum );

// Get the specified token
extern INT32 speechTokenList_get(TSpeechTokenList *pThis,
                                 INT32 tokenNum);

// Get the number of tokens
extern INT32 speechTokenList_numTokens(TSpeechTokenList *pThis);

// Replace a token on the list
extern void speechTokenList_replace(TSpeechTokenList *pThis,
                                    INT32 tokenNum,
                                    INT32 newValue);

// Add a punctuation symbol to the TTS string
// The punctuation will be ignored by the token-based speech
// Note: Trying to add two commas in a row will fail
//
// @param[out] tokenList A pointer to the token list in which to add the punctuation
// @param[in]  punctuation The enumerated punctuation symbol to add
// @return TRUE if the punctuation was added, otherwise FALSE
extern BOOL8 speechTokenList_addPunctuation( TSpeechTokenList *tokenList, TPunctuation punctuation );


#ifdef __cplusplus
}
#endif

#endif /* LATOKENLISTP_H */
