#ifndef UTF16_H
#define UTF16_H

#include <innovEngine_types.h>
#include <language/ConvertUTF.h>

#ifdef __cplusplus
extern "C" {
#endif

static const UTF16 UTF16_NULL[] = { 0 };  /* a const empty string */

#define UTF16_FIELD_SEP ((UTF16)0x0001)

extern BOOL8 utf16_ispunct(UTF16 c);
extern BOOL8 utf16_isspace(UTF16 c);
extern BOOL8 utf16_iswordbreak(UTF16 c);
extern BOOL8 utf16_isalnum(UTF16 c);
extern BOOL8 utf16_isalpha(UTF16 c);
extern UTF16 utf16_normalise(UTF16 c);
extern UTF16 utf16_toupper( UTF16 c );
extern UTF16 utf16_tolower( UTF16 c );
extern UTF16 utf16_dropAccent( UTF16 c );
extern UTF16 utf16_getSecondSpecialChar(UTF16 c);

extern UTF16 *utf16_strcat(UTF16 *dest, const UTF16 *src);
extern INT32  utf16_strcmp(const UTF16 *s1, const UTF16 *s2);
extern UTF16 *utf16_strcpy(UTF16 *s1, const UTF16 *s2);
extern UINT32 utf16_strlen(const UTF16 *s);
extern UINT32 utf16_strnlen(const UTF16 *s, INT32 n);
extern UTF16 *utf16_strncat(UTF16 *s1, const UTF16 *s2, INT32 n);
extern UTF16 *utf16_strnzcat(UTF16 *s1, const UTF16 *s2, INT32 n);
extern INT32  utf16_strncmp(const UTF16 *s1, const UTF16 *s2, INT32 n);
extern INT32  utf16_strncasecmp(const UTF16 *s1, const UTF16 *s2, INT32 n);
extern UTF16 *utf16_strncpy(UTF16 *s1, const UTF16 *s2, INT32 n);
extern UTF16 *utf16_strnzcpy(UTF16 *s1, const UTF16 *s2, INT32 n);
extern UTF16 *utf16_strnzcpy_many(UTF16 *s1, INT32 n, ...);
extern UTF16 *utf16_strchr(const UTF16 *s, UTF16 c);
extern UTF16 *utf16_strcasechr(const UTF16 *s, UTF16 c);
extern UTF16 *utf16_strstr(const UTF16 *s1, const UTF16 *s2);
extern UTF16 *utf16_strcasestr(const UTF16 *s1, const UTF16 *s2);

extern UTF16 *utf16_itoa(INT32 value, UTF16 * str);
extern INT32 utf16_atoi(const UTF16 *s);

#define utf16_isdigit(c) ((c >= '0' && c <= '9'))

#ifdef __cplusplus
}
#endif

#endif
