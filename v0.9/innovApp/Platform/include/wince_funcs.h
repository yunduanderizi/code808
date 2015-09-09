#pragma once
#ifndef _WINCE_FUNCS_H_
#define _WINCE_FUNCS_H_

#if defined(_NAVMAN_PLAT_KINPO)
#include "plat_kinpo.h"
#endif

#ifdef _WIN32_WCE
void wince_organiseMemory(unsigned int p_nCacheBufferSizeMax,
                          unsigned int p_nCacheBufferSizeMin,
                          unsigned int p_nStoragePageMin);

void wince_reportMemoryUse(void);
#endif // _WIN32_WCE

/* a word of warning regarding the following functions with a p_nBufferSize parameter:
this is the length in characters (wide chars) and not the size of the buffer in bytes.
so don't pass in the size of the buffer as in wince_getDeviceName(my_device, sizeof(my_device));
because you will get a buffer overrun when wcsnzcpy() sets the p_nBufferSize - 1 char to /0.
*/
#if defined(_NAVMAN_PLAT_GSL)
BOOL wince_getDeviceSerialNumber(UINT8 *p_pSerialNumber, int p_nBufferSize, int *p_pnLength);
#endif

BOOL wince_getKernelVersionRaw(LPWSTR p_wstrVersion, int p_nBufferSize);
BOOL wince_getBootloaderVersionRaw(LPWSTR p_wstrVersion, int p_nBufferSize);

BOOL gsl_getIPLVersionRaw(LPWSTR p_wstrVersion, int p_nBufferSize);
BOOL gsl_getSPLVersionRaw(LPWSTR p_wstrVersion, int p_nBufferSize);
BOOL gsl_getLogoVersionRaw(LPWSTR p_wstrVersion, int p_nBufferSize);
BOOL gsl_getSysInfoVersionRaw(LPWSTR p_wstrVersion, int p_nBufferSize);
BOOL gsl_getSelftestVersionRaw(LPWSTR p_wstrVersion, int p_nBufferSize);
BOOL gsl_getSPLBitmapVersionRaw(LPWSTR p_wstrVersion, int p_nBufferSize);

BOOL gsl_getIPLVersion(UINT32 *p_pnMajor, UINT32 *p_pnMinor, UINT32 *p_pnBuild);
BOOL gsl_getSPLVersion(UINT32 *p_pnMajor, UINT32 *p_pnMinor, UINT32 *p_pnBuild);
BOOL gsl_getLogoVersion(UINT32 *p_pnMajor, UINT32 *p_pnMinor, UINT32 *p_pnBuild);
BOOL gsl_getSysInfoVersion(UINT32 *p_pnMajor, UINT32 *p_pnMinor, UINT32 *p_pnBuild);
BOOL gsl_getSelftestVersion(UINT32 *p_pnMajor, UINT32 *p_pnMinor, UINT32 *p_pnBuild);
BOOL gsl_getSPLBitmapVersion(UINT32 *p_pnMajor, UINT32 *p_pnMinor, UINT32 *p_pnBuild);

BOOL wince_setProcessorSpeeds(void);

BOOL wince_setWakeup(BOOL p_bExtPowerOn, BOOL p_bExtPowerOff);

BOOL debug_decodeUnitID(UINT8 *p_serial, UINT32 p_nLength);

#if defined(_WIN32_WCE) && defined(TARGET)
BOOL wince_findProcess(LPWSTR p_strProcess, DWORD *p_pdwProcessID = NULL);
#endif

#endif // _WINCE_FUNCS_H_

