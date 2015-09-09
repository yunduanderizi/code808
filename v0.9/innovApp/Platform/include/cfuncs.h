#pragma once

#ifndef __CFUNCS_H
#define __CFUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

extern BOOL g_bExit;

extern BOOL8 g_bDebugToFile;
extern BOOL8 g_bIncrementDebugFile;
extern char g_strDebugFile[MAX_PATH];
extern BOOL8 g_bDebugToASCII;
extern BOOL8 g_bDebugToConsole;
extern BOOL8 g_bDebugOverDebugPort;
extern BOOL8 g_bGpsOverDebugPort;
extern BOOL8 g_bRdsOverDebugPort;
extern BOOL8 g_bRunWarmStartTest;
extern BOOL8 g_bWarmStartIncrementLog;
extern char g_strWarmStartLogFile[MAX_PATH];
extern INT32 g_nGpsOnPeriod;
extern INT32 g_nGpsOffPeriod;
extern INT32 g_nInitialFixOffset;
extern char g_strResetType[MAX_PATH];
extern BOOL8 g_bTrafficDebugScreen;
extern INT32 g_bMemPrintInterval;
extern BOOL8 g_bCloseSerialPort;

extern DWORD dwError;
extern WCHAR wstrOutput[MAX_PATH];
extern char  strOutput[MAX_PATH];

#ifdef DEBUG
#define os_assert(test) assert(test)
#define os_assertmsg(msg, test) if (!(test)) os_printf(msg)
#define os_verify(test) assert(test)
#define os_abort()
#else
#define os_assert(test)
#define os_assertmsg(msg, test)
#define os_verify(test) if (!(test)) os_exit_(__FILE__, __LINE__, 2)
#define os_abort() TerminateProcess(GetCurrentProcess(), 0)
#endif // DEBUG

char *strnzcpy(char *s1, const char *s2, size_t n);
char *strnzcat(char *s1, const char *s2, size_t n);
int snzprintf(char *buffer, int bufferLength, const char *format, ...);

#ifdef UNICODE
WCHAR *wcsnzcpy(WCHAR *s1, const WCHAR *s2, size_t n);
WCHAR *wcsnzcat(WCHAR *s1, const WCHAR *s2, size_t n);
int swnzprintf(LPTSTR lpOut, int nBufferLength, LPCTSTR lpFmt, ...);
#endif // UNICODE

#ifdef _WIN32_WCE
BOOL ReadMemoryDivision(
  DWORD *p_pdwStoragePages,
	DWORD *p_pdwRamPages,
	DWORD *p_pdwPageSize);
BOOL SetMemoryDivision(DWORD p_dwStoragePages);
BOOL SetBestMemoryDivision(
  DWORD p_dwMinStorage,
  DWORD p_dwMaxStorage,
  signed long p_lStep);
#endif // _WIN32_WCE

int xtoi(char x);
int wxtoi(WCHAR x);
char itox_char(UINT8 i);
void itox(UINT8 i, char *c1, char *c2);
void witox(UINT8 i, WCHAR *c1, WCHAR *c2);

#ifdef __cplusplus
}
#endif

#endif // __CFUNCS_H
