#ifndef LACODEPAGEP_H
#define LACODEPAGEP_H

#include <ctype.h>
#include <stdio.h>
#include <wchar.h>

#include <language/lacodepage.h>
#include <language/latypesP.h>

#ifdef __cplusplus
extern "C" {
#endif

extern TLanguageCode language_normaliseLanguageCode( TLanguageCode langCode );

extern void language_concatlLatin1AsUTF16( UTF16 *dest, const TLatin1Char *src );

extern void language_UTF8nzcpyAsUTF16( UTF16 *dest, const UTF8 *src, UINT32 bufferSize );

extern void language_UTF8nzcatAsUTF16( UTF16 *dest, const UTF8 *src, UINT32 bufferSize );

extern int utf16_snprintf(UTF16 *ubuf, size_t len, const UTF16 *ufmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* LACODEPAGEP_H */
