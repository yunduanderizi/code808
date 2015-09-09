#ifndef LATOKENLIST_H
#define LATOKENLIST_H

#include <language/ConvertUTF.h>
#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @internal
/// The maximum number of speech tokens that may occur in a single spoken
/// phrase or sentence.
/// @todo This value is a guess. Determine the correct value to use.
#define SPEECH_MAX_TOKENS_PER_PHRASE 32

/// @internal
/// The maximum number of interjections that may occur in a single spoken
/// phrase or sentence.
#define SPEECH_MAX_INTERJECTIONS_PER_PHRASE 12

/// @internal
/// The maximum total length of the interjections that may occur 
/// in a single spoken phrase or sentence.
#define SPEECH_MAX_INTERJECTIONS_SIZE 240


typedef enum TPunctuation {
  PUNCTUATION_APOSTROPHE,
  PUNCTUATION_LEFT_PARENTHESIS,
  PUNCTUATION_RIGHT_PARENTHESIS,
  PUNCTUATION_LEFT_SQUARE_BRACKET,
  PUNCTUATION_RIGHT_SQUARE_BRACKET,
  PUNCTUATION_LEFT_BRACES,
  PUNCTUATION_RIGHT_BRACES,
  PUNCTUATION_LEFT_CHEVRON,
  PUNCTUATION_RIGHT_CHEVRON,
  PUNCTUATION_LEFT_GUILLEMET,
  PUNCTUATION_RIGHT_GUILLEMET,
  PUNCTUATION_COLON,
  PUNCTUATION_COMMA,
  PUNCTUATION_DASH,
  PUNCTUATION_ELLIPSIS,
  PUNCTUATION_EXCLAMATION,
  PUNCTUATION_PERIOD,
  PUNCTUATION_HYPHEN,
  PUNCTUATION_INTERPUNCT,
  PUNCTUATION_QUESTION_MARK,
  PUNCTUATION_QUOTATION_MARK,
  PUNCTUATION_LEFT_SINGLE_QUOTE,
  PUNCTUATION_RIGHT_SINGLE_QUOTE,
  PUNCTUATION_LEFT_DOUBLE_QUOTE,
  PUNCTUATION_RIGHT_DOUBLE_QUOTE,
  PUNCTUATION_SEMICOLON,
  PUNCTUATION_SOLIDUS
} TPunctuation;

/// @internal
/// A list of speech data file token numbers constituting a spoken phrase or
/// sentence.
typedef struct {
  /// @internal
  /// The list of speech data file token numbers.
  INT32 tokenList[SPEECH_MAX_TOKENS_PER_PHRASE];
  /// @internal
  /// The number of tokens in the list.
  INT32 numTokens;
  /// @internal
  /// Optional interjections to add to the phrase
  UTF16* interjections[SPEECH_MAX_INTERJECTIONS_PER_PHRASE];
  /// @internal
  /// The number of interjections in the list.
  INT32 numInterjections;
  /// @internal
  /// The text of the interjections
  UTF16 interjectionText[SPEECH_MAX_INTERJECTIONS_SIZE];
} TSpeechTokenList;

#ifdef __cplusplus
}
#endif

#endif /* LATOKENLIST_H */
