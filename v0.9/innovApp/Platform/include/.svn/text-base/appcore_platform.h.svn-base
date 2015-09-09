#pragma once
#ifndef _APPCORE_PLATFORM_H_
#define _APPCORE_PLATFORM_H_

#ifndef _POSIX

#define WIN32_LEAN_AND_MEAN

// Don't define min() and max() macros (they conflict with the STL versions)
//#define NOMINMAX // min() & max() still needed for WTL, consider removing later

// must be defined before atlbase and other windows headers are included
// so we can pick up CoInitializeEx, et al
#ifndef _WIN32_WCE
#define _WIN32_DCOM
#endif

#include <windows.h>

#ifndef _WIN32_WCE
#include <crtdbg.h>
#endif

#ifndef ASSERT
#define ASSERT _ASSERT
#endif

#ifndef MAX_TEXT
#define MAX_TEXT 256
#endif

#include "warnings.h"

#if defined(PRODUCT_MANTAS) \
  || defined(PRODUCT_PLATINUM) \
  || defined(PRODUCT_MERCURY) \
  || defined(PRODUCT_TITANIUM) \
  || defined(PRODUCT_ZINC) \
  || defined(PRODUCT_COPPER) \
  || defined(PRODUCT_NICKEL) \
  || defined(PRODUCT_MANTAS) \
  || defined(PRODUCT_TILAPIA)
  #define DEFINED_PRODUCT_TUI 1
#endif

#if defined(PRODUCT_COBIA_300) \
  || defined(PRODUCT_COBIA_300_NA) \
  || defined(PRODUCT_COBIA_300_AU) \
  || defined(PRODUCT_COBIA_T300) \
  || defined(PRODUCT_COBIA_T300_NA) \
  || defined(PRODUCT_COBIA_400) \
  || defined(PRODUCT_COBIA_400_NA) \
  || defined(PRODUCT_COBIA_T400) \
  || defined(PRODUCT_COBIA_T400_NA) \
  || defined(PRODUCT_COBIA_410) \
  || defined(PRODUCT_COBIA_420) \
  || defined(PRODUCT_COBIA_420_AU) \
  || defined(PRODUCT_COBIA_T420) \
  || defined(PRODUCT_COBIA_T420_NA) \
  || defined(PRODUCT_COBIA_T420_AU) \
  || defined(PRODUCT_S35) \
  || defined(PRODUCT_S55) \
  || defined(PRODUCT_S75)
  #define DEFINED_PRODUCT_MME 1
#endif

#if defined(PRODUCT_MERMAID)
#define DEFINED_PRODUCT_SPIRIT  1
#endif

#ifdef _WIN32_WCE
#include <Storemgr.h>
#ifdef TARGET
#ifndef PRODUCT_SDK500
#include <Pkfuncs.h>
#include <ras.h>
#endif // !PRODUCT_SDK500
#include <Tlhelp32.h>
#endif // TARGET
#include <Notify.h>
#include <Pm.h>
#endif // _WIN32_WCE

#else
#include <plat_funcs.h>
#endif // !_POSIX

#ifndef MAX_TEXT
#define MAX_TEXT 256
#endif

/**
   This macro will return the number of entries in a statically declared
   array.
*/
#if defined(WIN32) || defined(_WIN32_WCE)
// Copied from atldef.h
extern "C++"
{
  template <typename _CountofType, size_t _SizeOfArray>
  char (*__countof_helper(UNALIGNED _CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
#define _countof(_Array) sizeof(*__countof_helper(_Array))
}

#define ARRAYLENGTH(x) _countof(x)
#else
#define ARRAYLENGTH(x) (sizeof(x)/sizeof(x[0]))
#endif

#include <platform.h>
#include "plat_unicode.h"
#include "cfuncs.h"

#ifndef _POSIX
#ifndef _WIN32_WCE
// platform_types defines a version of this for some reason on desktop windows
#undef offsetof
#endif // !_WIN32_WCE

#include <objbase.h>

#ifdef _WIN32_WCE
#include <coguid.h>
#else
#include <cguid.h>
#endif // _WIN32_WCE

#if defined(WIN32) && !defined(_WIN32_WCE)
#include <atlbase.h>

// This is required for various windows structures.
#include <atlwin.h>
#endif

// Copied from atlosapice.h
#ifndef SWP_NOREDRAW
#define SWP_NOREDRAW            0
#endif

#ifdef _WIN32_WCE
#include <imgguids.h>
#endif // _WIN32_WCE

#endif // !_POSIX

#ifdef _POSIX
#include <stdarg.h>
#endif // _POSIX
#include <plat_string.h>

#define PlatformBaseInterface IUnknown
#ifdef _POSIX
#define PLATCALL
#else
#define PLATCALL __stdcall
#endif // _POSIX

#ifndef _POSIX
/**
Comment out to log full paths with file names whenever available;
uncomment to always strip paths and output only file names.
*/
#ifndef DEBUG
#define LOG_ONLY_FILE_NAMES
#endif
#endif
//#ifdef LOG_ONLY_FILE_NAMES
  #include <string.h>

  // strip leading L"..." from wide string macros
  #define NO_L
  #define W2N_( wideStrExpanded ) NO_##wideStrExpanded
  #define W2N( wideStr )          W2N_( wideStr )
//#endif // LOG_ONLY_FILE_NAMES

//
// This macro/function returns true if _name is not an absolute filesystem path.
// This is typically a platform function.
//
#ifdef _POSIX
#define ISRELATIVEPATH(_name) (!(_name[0] == L'/'))
#else
#define ISRELATIVEPATH(_name) (!(_name[0] == PLATFORM_DIR_SEPARATOR_W[0] || _name[1] == L':'))
#endif // _POSIX

// max length of log file line
#define MAX_LOG_LINE_BUFFER_SIZE    1280  // 1280 = 256 for path and error info + 1024 for MAX_SQL_COMMAND_LENGTH


// typedef HRESULT on non-win32 platforms
// typedef iid/clsid/guid macros on non-win32 platforms

/*  For macros to delete string pointers
 */
//#pragma message("Warning: del is deprecated please use SAFE_DELETE or SAFE_DELETE_ARRAY, see WA-475")
//#define del(x) {if(x!=NULL) delete x; x=NULL;}

#define SAFE_DELETE(x) {if(x!=NULL) delete x; x=NULL;}
#define SAFE_DELETEARRAY(x) {if(x!=NULL) delete [] x; x=NULL;}

namespace navman {

/**
   The possible debug levels.
 */
typedef enum {
  /// Specify to disable logging
  LOG_NONE = 0,
  /// Log only errors (the default)
  LOG_ERROR = 1,
  /// Log errors and warnings
  LOG_WARNING = 2,
  /// Log errors, warnings, and informational messages
  LOG_INFO = 3,
  /// Log errors, warnings, informational, and debug messages
  LOG_DEBUG = 4,
  /// Log errors, warnings, informational, debug, and trace level messages
  LOG_TRACE = 5
} DebugLevel;

/**
The possible debug modules.
*/
typedef enum {
  MODULE_NONE     = 0x0000,
  MODULE_ROOT     = 0x0001,
  MODULE_TRAFFIC  = 0x0002,
  MODULE_GPS      = 0x0004,
  MODULE_UI       = 0x0008,
  MODULE_SOUND    = 0x0010,
  MODULE_PLATFORM = 0x0020,
  MODULE_ALL      = 0xffff
} DebugModules;

/**
   Set this variable to the appropriate debug level to enable logging
*/
extern DebugLevel g_DebugLevel;

/**
Set this variable to the appropriate debug modules to enable logging
*/
extern DebugModules g_DebugModules;


/**
   @internal
   This class shouldn't be used directly, rather use the logging macros
   provided.

   This class outputs a string via os_printf adding some additional formatting
   and debug information.
*/
class NMTrace
{
public:
  
  /**
     The constructor, meant to be called from a macro so that
     file and line information can be set correctly
  */
  NMTrace(const char *file, const int line, const char *level, UINT32 time)
    : _file(file)
    , _level(level)
    , _module(NULL)
    , _line(line)
    , _time(time)
  {
  }

  NMTrace(const char *file, const int line, const char *level, const char *module, UINT32 time)
    : _file(file)
    , _level(level)
    , _module(module)
    , _line(line)
    , _time(time)
  {
  }

  /**
     once the object is constructed, this method is called
     to output a printf-style string using the extra
     information passed in the constructor.
  */
  void
  printf(const WCHAR *fmt, ...)
  {
    va_list args;

    // do NOT replace malloc with 'new' or 'os_malloc' -- they hook into debug 
    // which we do not want as failure there may cause infinite loop!!!
    _buffer = ( WCHAR* )malloc( MAX_LOG_LINE_BUFFER_SIZE * sizeof ( WCHAR ) );
    if ( _buffer )
    {
      UINT32 len;

      va_start(args, fmt);

      if (_module != NULL)
      {
        if (strncmp("MODULE_", _module, strlen("MODULE_")) == 0)
          _module += strlen("MODULE_");
      }

#ifdef LOG_ONLY_FILE_NAMES
      // file name starts after last dir separator
      const char *fileNameStart = strrchr( _file, PLATFORM_DIR_SEPARATOR[0] );
      fileNameStart = ( fileNameStart       // found last dir separator?
                      ? fileNameStart + 1   // yes -- skip it to get to file name
                      : _file               // no dir separators found -- use the whole string
                      );
#else // !LOG_ONLY_FILE_NAMES
      const char *fileNameStart = _file;
#endif // !LOG_ONLY_FILE_NAMES
      // Keep the "%hs(%d):" for VS2005, so double clicking on debug output takes you to the line in question
      if (_module != NULL)
        len = snwprintf(_buffer, MAX_LOG_LINE_BUFFER_SIZE, L"%hs(%d):, %u, %hs-%hs, ", fileNameStart, _line, _time, _module, _level);
      else
        len = snwprintf(_buffer, MAX_LOG_LINE_BUFFER_SIZE, L"%hs(%d):, %u, %hs, ", fileNameStart, _line, _time, _level);
      INT32 count = vsnwprintf(_buffer + len, MAX_LOG_LINE_BUFFER_SIZE - len, fmt, args);
      // count == -1 indicates that output has been truncated
      len = ( ( count < 0 )
            ? MAX_LOG_LINE_BUFFER_SIZE
            : len + count
            );

      va_end(args);

      if (len > MAX_LOG_LINE_BUFFER_SIZE - 2) 
      {
        len = MAX_LOG_LINE_BUFFER_SIZE - 2;
      }

      // Detect already existing newlines
      if (_buffer[len - 1] != '\n')
      {
        _buffer[len] = '\n';
        _buffer[len + 1] = 0;
      }
      else
      {
        _buffer[len] = 0;
      }

      os_wprintf(_buffer);

      free(_buffer);
    }
  }

  WCHAR *_buffer;
  const char *_file;
  const char *_level;
  const char *_module;
  int _line;
  UINT32 _time;
};

#ifdef _I386_
inline void
DumpStackTrace(WCHAR *buffer, UINT32 len)
{
  OutputDebugString(L"\nStack Trace\n");

  UINT32 framep;
  UINT32 instrp = 0;
  __asm {
    mov framep, ebp
  }

  while (1) {
    instrp = *((UINT32 *)framep + 1);
    framep = *((UINT32 *)framep);

    if (instrp == 0) {
      break;
    }

    snwprintf(buffer, len, L"0x%08x\n", instrp);
    OutputDebugString(buffer);
  }
}
#else
inline void
DumpStackTrace(WCHAR *buffer, UINT32 len)
{
}
#endif // _I386_

/**
  The NM_LOG_* macros are used to output messages according to the current
  debug level set in g_DebugLevel.
*/

/**
  Sets the current log level.
*/
#  define NM_LOG_SETLEVEL(_level) g_DebugLevel = (_level)

/**
  Gets the current log level
*/
#  define NM_LOG_GETLEVEL() g_DebugLevel

/**
  Checks if the log level is set
*/
#  define NM_LOG_ISLEVELSET(_level) (BOOL)(g_DebugLevel >= _level)

/**
  Sets the current log modules.
*/
#  define NM_LOG_SETMODULES(_modules) g_DebugModules = (_modules)

/**
  Gets the current log modules
*/
#  define NM_LOG_GETMODULES() g_DebugModules

/**
  Checks if the log module is set
*/
#  define NM_LOG_ISMODULESET(_module) (BOOL)((g_DebugModules & _module) > 0)

#ifdef DEBUG
/**
   Call to output a TRACE-level log message.
*/
#define NM_LOG_TRACE(...) do { \
    if (g_DebugLevel >= LOG_TRACE) NMTrace(__FILE__, __LINE__, "TRACE", os_time()).printf(__VA_ARGS__); \
} while (0)
#define NM_LOGMODULE_TRACE(LogModule, ...) do { \
  if ((g_DebugLevel >= LOG_TRACE)&&((g_DebugModules & LogModule) > 0)) NMTrace(__FILE__, __LINE__, "TRACE", #LogModule, os_time()).printf(__VA_ARGS__); \
} while (0)
#else
#define NM_LOG_TRACE(...)
#define NM_LOGMODULE_TRACE(...)
#endif // DEBUG

/**
   Call to output a DEBUG-level log message
*/
#define NM_LOG_DEBUG(...) do { \
    if (g_DebugLevel >= LOG_DEBUG) NMTrace(__FILE__, __LINE__, "DEBUG", os_time()).printf(__VA_ARGS__); \
} while (0)
#define NM_LOGMODULE_DEBUG(LogModule, ...) do { \
  if ((g_DebugLevel >= LOG_DEBUG)&&((g_DebugModules & LogModule) > 0)) NMTrace(__FILE__, __LINE__, "DEBUG", #LogModule, os_time()).printf(__VA_ARGS__); \
} while (0)

/**
   Call to output an INFO-level (informational) log message.
*/
#define NM_LOG_INFO(...) do { \
    if (g_DebugLevel >= LOG_INFO) NMTrace(__FILE__, __LINE__, "INFO", os_time()).printf(__VA_ARGS__); \
} while (0)
#define NM_LOGMODULE_INFO(LogModule, ...) do { \
  if ((g_DebugLevel >= LOG_INFO)&&((g_DebugModules & LogModule) > 0)) NMTrace(__FILE__, __LINE__, "INFO", #LogModule, os_time()).printf(__VA_ARGS__); \
} while (0)

/**
   Call to output a WARNING-level log message
*/
#define NM_LOG_WARNING(...) do { \
    if (g_DebugLevel >= LOG_WARNING) NMTrace(__FILE__, __LINE__, "WARNING", os_time()).printf(__VA_ARGS__); \
} while (0)
#define NM_LOGMODULE_WARNING(LogModule, ...) do { \
  if ((g_DebugLevel >= LOG_WARNING)&&((g_DebugModules & LogModule) > 0)) NMTrace(__FILE__, __LINE__, "WARNING", #LogModule, os_time()).printf(__VA_ARGS__); \
} while (0)

/**
   Call to output an ERROR-level log message
*/
#define NM_LOG_ERROR(...) do { \
    if (g_DebugLevel >= LOG_ERROR) NMTrace(__FILE__, __LINE__, "ERROR", os_time()).printf(__VA_ARGS__); \
} while (0)
#define NM_LOGMODULE_ERROR(LogModule, ...) do { \
  if ((g_DebugLevel >= LOG_ERROR)&&((g_DebugModules & LogModule) > 0)) NMTrace(__FILE__, __LINE__, "ERROR", #LogModule, os_time()).printf(__VA_ARGS__); \
} while (0)

#ifdef DEBUG
/**
   The PARAMCHECK macros are used to check that function parameters conform
   to expectations.
*/
/**
   ASSERT the expression and return false
*/
#  define PARAMCHECK(expr) do { \
    if (!(expr)) { NM_LOG_ERROR(L"PARAMCHECK(%hs)", #expr); ASSERT(FALSE); return false; } \
   } while (0)
/**
   ASSERT the expression, returning ret if false
*/
#  define PARAMCHECKRET(expr, ret) do { \
    if (!(expr)) { NM_LOG_ERROR(L"PARAMCHECKRET(%hs)", #expr); ASSERT(FALSE); return (ret); } \
   } while (0)
/**
   ASSERT the expression, returning if false.
*/
#  define PARAMCHECKVOID(expr) do { \
    if (!(expr)) { NM_LOG_ERROR(L"PARAMCHECKVOID(%hs)", #expr); ASSERT(FALSE); return; } \
   } while (0)

/**
   ASSERTs the expression in debug, guarantees the expression is still evaluated
   in retail builds.  This is a convenience for ASSERT.
   The value logged in brackets at the end of the message is a platform specific error code,
   e.g. GetLastError on Windows CE.
*/
#  define NMVERIFY(expr) do { \
   if (!(expr)) { NM_LOG_ERROR(L"NMVERIFY(%hs) (%u)", #expr, GetLastError()); ASSERT(FALSE); } \
   } while (0)

#else

#  define PARAMCHECK(expr)
#  define PARAMCHECKRET(expr, ret)
#  define PARAMCHECKVOID(expr)

/**
   ASSERTs the expression in debug, guarantees the expression is still evaluated
   in retail builds.  This is a convenience for ASSERT.
*/
#  define NMVERIFY(expr) do { \
   if (!(expr)) { NM_LOG_ERROR(L"NMVERIFY(%hs)", #expr); os_exit_(__FILE__, __LINE__, 1); } \
   } while (0)

#endif // DEBUG

#ifdef _WIN32_WCE
static BOOL l_bUTF8NotSupported = FALSE;
#endif

/**
   Converts a wide (UTF16) string to a narrow (UTF8) string
*/
class NarrowStr
{
public:
  /**
    empty constructor
  */
  NarrowStr() : _dest(NULL) { }

  /**
     Construct the narrow string from the wide
     @param src the wide string to narrow
  */
  explicit NarrowStr(const WCHAR *src)
  {
    (void)initWithWideString(src);
  }

  /**
     Construct the narrow string from the wide, stopping at len characters
     @param src the wide string to narrow
     @param len the number of characters in src to include in the narrow string.
  */
  NarrowStr(const WCHAR *src, size_t len)
  {
    wchar_t *temp = new wchar_t[len + 1];
    wcsncpy(temp, src, len);
    temp[len] = L'\0';
    (void)initWithWideString(temp);
    delete[] temp;
  }

  ~NarrowStr()
  {
    if (_dest != NULL) {
      delete[] _dest;
      _dest = NULL;
    }
  }

  /**
     This operator allows a NarrowStr to be evaluated in a regular string
     context.
  */
  operator const char *() const { return _dest; }

  /**
     A more explicit version of the const char * operator.
  */
  const char *toString() const { return _dest; }

  /**
    Assigns a new string to this object.
    @param src the string to assign
    @return this
  */
  const NarrowStr& operator=(const WCHAR *src)
  {
    if (_dest != NULL) {
      delete[] _dest;
    }
    
    (void)initWithWideString(src);

    return *this;
  }

protected:
  bool initWithWideString(const wchar_t *src)
  {
#ifdef _WIN32_WCE
    UINT32 len = wcslen(src);
    UINT32 destLen = 0;
    if (!l_bUTF8NotSupported)
      destLen = WideCharToMultiByte(CP_UTF8, 0, src, len + 1, NULL, 0, NULL, FALSE);
    if (destLen == 0)
    {
      l_bUTF8NotSupported = TRUE;
      destLen = WideCharToMultiByte(CP_ACP, 0, src, len + 1, NULL, 0, NULL, FALSE);
      _dest = new char[destLen];
      destLen = WideCharToMultiByte(CP_ACP, 0, src, len + 1, _dest, destLen, NULL, FALSE);
    }
    else
    {
      _dest = new char[destLen];
      destLen = WideCharToMultiByte(CP_UTF8, 0, src, len + 1, _dest, destLen, NULL, FALSE);
    }
    return true;
#else // !_WIN32_WCE
    const WCHAR *p;
    mbstate_t mbstate;
    memset(&mbstate, 0, sizeof(mbstate));
    p = src;
    size_t len = wcsrtombs(NULL, &p, 0, &mbstate); // Find length of MBS
    if (len == (size_t)-1) {
      NM_LOG_ERROR(L"Cannot convert wide string \"%ls\" to multi-byte string.", src);
      NMVERIFY(FALSE);
      _dest = new char[1];
      *_dest = '\0';
      return false;
    }
    _dest = new char[len + 1];
    p = src;
    wcsrtombs(_dest, &p, len + 1, &mbstate);
    _dest[len] = '\0';
    return true;
#endif // !_WIN32_WCE
  }

  char *_dest;
};

/**
   Converts a narrow (UTF8) string to the wide (UTF16) equivalent.
*/
class WideStr
{
public:
  /**
    empty constructor
  */
  WideStr() : _dest(NULL) {}

  /**
     Construct a wide string from a narrow one.
     @param src the narrow string
  */
  explicit WideStr(const char *src)
  {
    (void)initWithMultiByteString(src);
  }

  /**
     Construct a wide string from a narrow one, stopping at len chars
     @param src the narrow string
     @param len the number of chars in src to include in the wide string
  */
  WideStr(const char *src, size_t len)
  {
    char *temp = new char[len + 1];
    strncpy(temp, src, len);
    temp[len] = '\0';
    (void)initWithMultiByteString(temp);
    delete[] temp;
  }

  ~WideStr()
  {
    if (_dest != NULL) {
      delete[] _dest;
      _dest = NULL;
    }
  }

  /**
     Allows WideStr to be evaluated in a WCHAR * context
  */
  operator const WCHAR *() const { return _dest; }

  /**
     A more explicit version of the const WCHAR * operator
  */
  const WCHAR *toString() const { return _dest; }

  /**
    Assigns a new string to this object.
    @param src the string to assign
    @return this
  */
  const WideStr& operator=(const char *src)
  {
    if (_dest != NULL) {
      delete[] _dest;
    }
    
    (void)initWithMultiByteString(src);

    return *this;
  }

protected:
  bool initWithMultiByteString(const char *src)
  {
#ifdef _WIN32_WCE
    UINT32 len = strlen(src);
    UINT32 destLen = 0;
    if (!l_bUTF8NotSupported)
      destLen = MultiByteToWideChar(CP_UTF8, 0, src, len + 1, NULL, 0);
    if (destLen == 0)
    {
      l_bUTF8NotSupported = TRUE;
      destLen = MultiByteToWideChar(CP_ACP, 0, src, len + 1, NULL, 0);
      _dest = new WCHAR[destLen];
      destLen = MultiByteToWideChar(CP_ACP, 0, src, len + 1, _dest, destLen);
    }
    else
    {
      _dest = new WCHAR[destLen];
      destLen = MultiByteToWideChar(CP_UTF8, 0, src, len + 1, _dest, destLen);
    }
    return true;
#else // _WIN32_WCE
    mbstate_t mbstate;
    const char *p;

    memset(&mbstate, 0, sizeof(mbstate));
    p = src;
    size_t len = mbsrtowcs(NULL, &p, 0, &mbstate); // Find length of wide string
    if (len == (size_t)-1) {
      NM_LOG_ERROR(L"String \"%hs\" contains invalid multi-byte sequence.", src);
      NMVERIFY(FALSE);
      _dest = new WCHAR[1];
      *_dest = L'\0';
      return false;
    }
    _dest = new WCHAR[len + 1];
    p = src;
    mbsrtowcs(_dest, &p, len + 1, &mbstate);
    _dest[len] = L'\0';
    return true;
#endif // _WIN32_WCE
  }

  WCHAR *_dest;
};

#ifdef WIN32
// wide chars
//#define wcslen _wcslen
//#define wcscpy _wcscpy
//#define wcscmp _wcscmp
//#define wcsncmp _wcsncmp
#define wcsnicmp _wcsnicmp
#define snwprintf _snwprintf
//#define wcschr _wcschr
#define wtoi _wtoi
#define wtof(x) atof(NarrowStr(x))
#define wtoi64 _wtoi64 
#define itow _itow

// narrow chars
#define strnicmp _strnicmp
#define snprintf _snprintf
#define atoi64 _atoi64
#define itoa _itoa
#endif // WIN32

} // namespace navman
#endif // _APPCORE_PLATFORM_H_
