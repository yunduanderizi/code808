#ifndef LAUTIL_H
#define LAUTIL_H

#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Language
/// @{

/// @brief Compare two strings (including numerical comparisons) ignoring accents
///        and case for primary ordering.  Accents and case are considered only if
///        two strings are equivalent when accents and case are ignored.
///
/// @param[in]   string1              A pointer to the left UTF16 string to compare.
///
/// @param[in]   string2              A pointer to the right UTF16 string to compare.
///
/// @param[in]   ignoreSecondaryDiff  A flag indicating whether or not to apply
///                                   secondary compare criteria (accents and case)
///                                   if two strings compare equal when ignoring the
///                                   secondary criteria.
///
/// @return  A signed value indicating the relative ordering of the strings:
///           <0 if string1 < string2,
///            0 if string1 == string2,
///           >0 if string1 > string2.
///
INT32 language_strcmp( const UTF16* string1,
                       const UTF16* string2,
                       BOOL8        ignoreSecondaryDiff );

/// @}

#ifdef __cplusplus
}
#endif

#endif /* LAUTIL_H */
