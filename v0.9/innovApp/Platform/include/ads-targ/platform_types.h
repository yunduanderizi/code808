#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

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
/*                                                                           */
/* XScale/StrongARM Embedded Configuration (for the ARM Realview 2.x         */
/* compiler)                                                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
typedef unsigned int BITFIELD32;

typedef void *   OS_FILE;

typedef unsigned char BOOL8;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef uintptr_t UINTP;
typedef signed char INT8;
typedef signed short INT16;
typedef signed int INT32;
typedef signed long long INT64;
typedef intptr_t INTP;
typedef float FLOAT32;
typedef double FLOAT64;

#ifndef NULL
#define NULL ((void *)0)
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

#define INLINE __inline
#define STRUCT_PACKED __packed struct

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
 */
#define DEPRECATED(x) x

#define PLATFORM_DIR_SEPARATOR "\\"

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_TYPES_H */

/* End of platform_types.h */
