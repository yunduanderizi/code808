#pragma once
/**
 * @file plat_string.h
 * Redefines string function names to appropriate functions for the target
 * platform.
 * @author Zane Bitter
 */

#ifndef _PLAT_STRING_H_
#define _PLAT_STRING_H_

#ifdef _POSIX
// POSIX functions
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// wide chars
#define snwprintf   swprintf
#define vsnwprintf  vswprintf
#define wcsicmp     wcscasecmp
#define wcsnicmp    wcsncasecmp
#define wtoi(X)     atoi(navman::NarrowStr(X))
#define wtoi64(X)   atoll(navman::NarrowStr(X))
#define wcstok_s    wcstok

// narrow chars
#define stricmp     strcasecmp
#define strnicmp    strncasecmp

// wide or narrow chars
#ifdef UNICODE
#define tcslen      wcslen
#define tcsncat     wcsncat
#else // !UNICODE
#define tcslen      strlen
#define tcsncat     strncat
#endif // !UNICODE

#else // !_POSIX
// Win32 functions

// wide chars
#define wcsicmp     _wcsicmp
#define wcsnicmp    _wcsnicmp
#define snwprintf   _snwprintf
#define vsnwprintf  _vsnwprintf
#define wtoi        _wtoi
#define wtoi64      _wtoi64
#define itow        _itow

// narrow chars
#define stricmp     _stricmp
#define strnicmp    _strnicmp
#define snprintf    _snprintf
#define atoi64      _atoi64
#define itoa        _itoa

// wide or narrow chars
#define tcslen      _tcslen
#define tcsncat     _tcsncat

#endif // _POSIX


#define wtof(X)     atof(navman::NarrowStr(X))


#endif // _PLAT_STRING_H_
