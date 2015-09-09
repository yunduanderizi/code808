#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include <assert.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/* platform_types.h provides a number of standard type definitions and       */
/* global macros.  Applications can now build against the correct            */
/* configurations (using the SDK if they choose...) without issue by         */
/* defining a preprocessor symbol.                                           */
/*                                                                           */
/* Compiler targets currently supported:                                     */
/* - gnu-sim                                                                 */
/* - visual-sim                                                              */
/* - gnu-targ                                                                */
/* - ads-targ                                                                */
/* - pocketpc-targ                                                           */
/* - wince-targ                                                              */
/* - palm-targ                                                               */
/*                                                                           */
/* If the TARGET symbol isn't defined then the compiler should throw an      */
/* error via the #error preprocessor directive.                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Windows CE 5.0 Configuration                                              */
/*---------------------------------------------------------------------------*/
typedef unsigned __int32 BITFIELD32;

typedef void *           OS_FILE;

typedef unsigned __int8  BOOL8;
typedef unsigned __int8  UINT8;
typedef unsigned __int16 UINT16;
typedef unsigned __int32 UINT32;
typedef unsigned __int64 UINT64;
typedef unsigned long    UINTP;
typedef signed __int8    INT8;
typedef signed __int16   INT16;
typedef signed __int32   INT32;
typedef signed __int64   INT64;
typedef signed long      INTP;
typedef float            FLOAT32;
typedef double           FLOAT64;

// EVC++4 uses a non-C99 syntax for 64-bit literals.

#ifndef INT64_MIN
#define INT64_MIN (-INT64_MAX-1)
#endif

#ifndef INT64_MAX
#define INT64_MAX ((INT64)0x7FFFFFFFFFFFFFFFI64)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX ((UINT64)0xFFFFFFFFFFFFFFFFUI64)
#endif

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

#define QSORT_DECL __cdecl

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

#define INLINE
#define STRUCT_PACKED struct

/**
 * DEPRECATED is used to mark API which are deprecated
 * so that compiler can warn whenever such API is still
 * used.  APIs can be marked as DEPRECATED rather than
 * removing them to remain backward compatible at least
 * during a temporary period of time to give time to
 * application to change their code without breaking
 * builds
 *
 * Compilers that do not support DEPRECATED may define
 * it as #define DEPRECATED(x) x
 *
 * See:
 * http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclang/html/vcrefDeprecatedx.asp
 */
#define DEPRECATED(x) __declspec(deprecated) x

#define PLATFORM_DIR_SEPARATOR "\\"
#define PLATFORM_DIR_SEPARATOR_W L"\\"

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_TYPES_H */

/* End of platform_types.h */
