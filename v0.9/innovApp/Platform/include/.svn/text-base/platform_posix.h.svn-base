#pragma once
#ifndef _PLATFORM_POSIX_H_
#define _PLATFORM_POSIX_H_

#include <wchar.h>
#include <wctype.h>
#include <string.h>         // for memcmp()
#include <stdio.h>

#include <platform_types.h>

// Prevent microwindows redefining UINT
// Mapcore defines UINT in gnu-targ, but not in gnu-sim.
#ifdef TARGET
#define NOUINTTYPE
#endif
#include <windows.h>
#undef NOUINTTYPE
#define _HRESULT_DEFINED

#include <assert.h>
#define _ASSERT(X) assert(X)

typedef union {
  struct {
    DWORD LowPart;
    DWORD HighPart;
  };
  struct {
    DWORD LowPart;
    DWORD HighPart;
  } u;
  unsigned long long QuadPart;
} ULARGE_INTEGER;

#define INVALID_HANDLE_VALUE ((HANDLE)-1)

#define FAILED(STATUS) ((STATUS) < 0)
#define SUCCEEDED(STATUS) (!FAILED(STATUS))

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;
typedef GUID IID;
#define GUID_DEFINED

#define E_NOINTERFACE ((HRESULT)-1)
#define E_INVALIDARG  ((HRESULT)-2)

#ifdef UNICODE
void inline OutputDebugString(const WCHAR *str) {
  printf("%ls", str);
}
#else
void inline OutputDebugString(const char *str) {
  printf("%hs", str);
}
#endif

#ifdef __cplusplus

#define REFGUID const GUID &
#define REFIID const IID &
#define _REFIID_DEFINED

BOOL inline IsEqualIID(REFIID r1, REFIID r2) {
  return ::memcmp(&r1, &r2, sizeof(REFIID)) == 0;
}

class IUnknown {
public:
  /** Add a reference. */
  virtual ULONG AddRef() = 0;
  
  /** Free a reference. */
  virtual ULONG Release() = 0;

  /** Test for interface. */
  virtual HRESULT QueryInterface( REFIID riid, void ** ppv ) = 0;
  
  /* Stop GCC complaining about non-virtual destructor */
  virtual ~IUnknown() {}
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
long InterlockedIncrement(long *v);
long InterlockedDecrement(long *v);
#ifdef __cplusplus
}
#endif

#include <SALTime.h>
#define GetTickCount sal_tickCount

#include <errno.h>
static int __inline__ GetLastError(void) { return errno;}

#define __forceinline __inline__

#endif
