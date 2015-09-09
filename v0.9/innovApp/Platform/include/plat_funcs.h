#pragma once
#ifndef _PLAT_FUNCS_H
#define _PLAT_FUNCS_H

/** @file
This file contains the interface to the platform support layer that
sits underneath both the map engine and NAVMAN embedded GUI applications.
The support layer has a different implementation for each target
platform. */

#ifndef _POSIX
#include <wince_funcs.h>
#else
#include <platform_posix.h>
#endif // ndef _POSIX
#include <plat_types.h>

#ifndef ASSERT
#  define ASSERT _ASSERT
#endif // ndef ASSERT

//
// prevent runtime type conversion errors in desktop
//
#undef GetRValue
#undef GetGValue
#undef GetBValue

#define GetRValue(rgb)      ((BYTE)((rgb) & 0x0000ff))
#define GetGValue(rgb)      ((BYTE)(((WORD)((rgb) & 0x00ff00)) >> 8))
#define GetBValue(rgb)      ((BYTE)(((rgb) & 0xff0000)>>16))

#ifndef MAX_PATH
  #define MAX_PATH          260
#endif


extern UINT8  *g_mapCache;
extern UINT32 g_mapCacheSize;
extern UINT8  *g_mapRouteBuffer;
extern UINT32 g_mapRouteBufferSize;
#if defined(PRODUCT_ENDEAVOUR)
extern BOOL   g_bSDCardChanged; // from platform.cpp
#endif

BOOL sys_init(void);

#ifdef __cplusplus
namespace navman {
  class Comm;
}
extern class navman::Comm *g_pDebugPort;
extern INT32 g_nDebugPort;
extern INT32 g_nDebugBaud;

BOOL sys_openDebugSerial(void);
void sys_closeDebugSerial(void);

#endif // __cplusplus

BOOL sys_suspendDevice(void);
BOOL sys_rebootDevice(void);
BOOL sys_resetDevice(void);
BOOL sys_shutdownDevice(void);

void sys_getScreenSize(UINT32 *p_pnWidth, UINT32 *p_pnHeight);
void sys_setScreenSize(UINT32 p_nWidth, UINT32 p_nHeight);

/** Returns the position (in pixels) of the top left of the main window
on the platforms display unit. 

@param p_pnX Pointer to a 32-bit integer that, on return, will 
contain the x position (in pixels) of the display.

@param p_pnY Pointer to a 32-bit integer that, on return, will 
contain the y position (in pixels) of the display. */
void sys_getScreenOffset(UINT32 *p_pnX, UINT32 *p_pnY);
void sys_setScreenOffset(UINT32 p_nX, UINT32 p_nY);

TDeviceType sys_getDeviceTypeFromName(LPCWSTR p_wstrDeviceName);
TDeviceType sys_getDeviceType(void);

BOOL sys_decodeVersionString(LPCWSTR p_wstrVersion, UINT32 *pMajor, UINT32 *pMinor, UINT32 *pBuild);

BOOL sys_getKernelVersion(UINT32 *p_pnMajor, UINT32 *p_pnMinor, UINT32 *p_pnBuild);
BOOL sys_getBootloaderVersion(UINT32 *p_pnMajor, UINT32 *p_pnMinor, UINT32 *p_pnBuild);
BOOL sys_getBootloaderChecksum(DWORD *p_pdwChecksum);

BOOL sys_enableBatteryCharging(BOOL bEnable);

// This function tells us if the system clock time has been reset.
BOOL sys_isClockValid(void);

// Sets the system language 
BOOL sys_setSystemLocale(int localeId);

void sys_printVersions(void);

BOOL sys_setKeypadBacklight(BOOL p_bOn);

void sys_createMapCache(unsigned int p_nCacheBufferSizeMax,
                          unsigned int p_nCacheBufferSizeMin,
                          unsigned int p_nStoragePageMin);
void sys_getMemorySettings(DWORD *pMapCacheSizeMax, 
                             DWORD *pMapCacheSizeMin, 
                             DWORD *pMinStoragePage,
                             DWORD *pBigMallocBound);

BOOL sys_getHardwareMemory(UINT32 *p_pnHWMemory);
BOOL sys_getHardwareFlashSize(LPCWSTR p_wstrDir, UINT32 *p_pnHWFlashSize);
BOOL sys_getHardwareFlashType(DWORD *p_pdwFlashType);

#ifdef __cplusplus
extern "C" {
#endif

BOOL8 os_testSerialPort(int nPort);


/** This function creates a directory

@param dir Directory to create */
UINT32 os_fmkdir(const char *dir);

BOOL8 os_fmove(const char *dest, const char *source);

UINT32 os_frmdir(const char *dir);

BOOL os_fflush(OS_FILE stream);

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------*/

#endif // _PLAT_FUNCS_H

