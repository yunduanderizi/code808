#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int BITFIELD32;

typedef void * OS_FILE;

typedef unsigned char      BOOL8;
typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;
typedef unsigned long long UINT64;
typedef uintptr_t          UINTP;
typedef signed char        INT8;
typedef signed short       INT16;
typedef signed int         INT32;
typedef long long          INT64;
typedef intptr_t           INTP;
typedef float              FLOAT32;
typedef double             FLOAT64;

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* #ifndef NULL */

#ifndef FALSE
#define FALSE               0
#endif /* #ifndef FALSE */

#ifndef TRUE
#define TRUE                1
#endif /* #ifndef TRUE */

#define QSORT_DECL

/* The following are defines for platform-dependent text functions */
#define TXT_CHAR char
#define txtcpy strcpy
#define makestr
#define txtcat strcat
#define txtlen strlen
#define txt_fromPlatform(a,b,c) strlen(strcpy(a,b))
#define txt_toPlatform(a, b, c) strcpy(a, b)

/*
 * Some platforms (PalmOS) can't dereference function pointers
 * directly.  So whenever a function pointer is used, we meed to
 * use the GET_FUNCTION_ADDRESS() wrapper.  It is just a stub for
 * targets other than PalmOS.
 */
#define GET_FUNCTION_ADDRESS(f) f

#define GENERIC_API
#define PLATFORM_API
#define ENGINE_API
#define GRAPHICS_API
#define GPS_API
#define DISPLAY_API
#define LANGUAGE_API
#define AUTH_API
#define MAPCORE_API

#ifndef INLINE
#define INLINE inline
#endif

#define STRUCT_PACKED struct


#define DEPRECATED(x) x __attribute__((__deprecated__))

/*
 * Support building for Windows with MinGW
 */
#ifdef _WIN32
#define PLATFORM_DIR_SEPARATOR "\\"
#define PLATFORM_DIR_SEPARATOR_W L"\\"
#else
#define PLATFORM_DIR_SEPARATOR "/"
#define PLATFORM_DIR_SEPARATOR_W L"/"
#endif

#undef assert
#ifdef NDEBUG
#define assert(cond) ((void)0)
#else
extern void gnusim_assert(const char *, INT32, const char *);
#define assert(cond) ((cond) ? (void)0 : gnusim_assert(__FILE__, __LINE__, #cond))
#endif

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_TYPES_H */

/* End of platform_types.h */
