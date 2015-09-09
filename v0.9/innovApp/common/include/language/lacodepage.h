#ifndef LACODEPAGE_H
#define LACODEPAGE_H

#include <innovEngine_types.h>
#include <language/ConvertUTF.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Language
/// @{

/** Type for string latin1 code page ( actually Windows-1252 code page ) */
typedef char TLatin1Char;

extern MAPCORE_API  void language_UTF8ToUTF16(UTF16       *utf16,
                                              const UTF8  *utf8 );

extern MAPCORE_API void language_UTF16ToUTF8(UTF8        *utf8,
                                             const UTF16 *utf16);

extern MAPCORE_API void language_latin1ToUTF16( 
  UTF16 *dest,
  const TLatin1Char *src );

extern MAPCORE_API void language_UTF16ToLatin1(
  TLatin1Char *dest,
  const UTF16 *utf16 );


extern MAPCORE_API const wchar_t *language_UTF16ToWchar(const UTF16* s);

#ifndef U2W
#define U2W(x) language_UTF16ToWchar(x)
#endif

extern MAPCORE_API const UTF16 *language_wcharToUTF16(const wchar_t* s);

#ifndef W2U
#define W2U(x) language_wcharToUTF16(x)
#endif

#define LANGUAGE_FIRST_CLUSTER_KEY (0xE020)
#define LANGUAGE_LAST_CLUSTER_KEY  (0xE027)

/// @}

#ifdef __cplusplus
}
#endif

#endif /* LACODEPAGE_H */
