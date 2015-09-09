#pragma once
/*****************************************************************************
 *  (c) 2005 navman new zealand limited. all rights reserved.
 *
 *  module name: Registry.h
 *
 *  author(s): Tom Isaacson <tisaacson@navman.com>
 *
 *  description:
 *    This module initialises the touchscreen
 *****************************************************************************/

#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include "plat_types.h"

BOOL getNMRegistryString(LPCWSTR p_wstrName, LPWSTR p_wstrValue, UINT p_nLength);
BOOL getNMRegistryDWORD(LPCWSTR p_wstrName, PDWORD p_pdwValue);

#ifdef _WIN32_WCE

TDeviceType wincereg_getDeviceType(void);
DEPRECATED(BOOL wincereg_getDeviceName(LPWSTR p_wstrDeviceName, UINT p_nLength));
DEPRECATED(BOOL wincereg_getDeviceDisplayName(LPWSTR p_wstrDeviceDisplayName, UINT p_nLength));
BOOL wincereg_getDevicePath(LPWSTR p_wstrDevicePath, UINT p_nLength);
DEPRECATED(BOOL wincereg_getBasePath(LPWSTR p_wstrBasePath, UINT p_nLength));
BOOL wincereg_getInternalPath(LPWSTR p_wstrBasePath, UINT p_nLength);
BOOL wincereg_getExternalPath(int p_nExternal, LPWSTR p_wstrBasePath, UINT p_nLength);
BOOL wincereg_getBaseHardware(LPWSTR p_wstrBaseHardware, UINT p_nLength);
BOOL wincereg_getKernelVersion(LPWSTR p_wstrVersion, UINT p_nLength);
BOOL wincereg_getHardwareVersion(LPWSTR p_wstrVersion, UINT p_nLength);
DEPRECATED(BOOL wincereg_getMioSKUSettings(
  LPWSTR p_wstrProjectCode, UINT p_nProjectCodeSize,
  LPWSTR p_wstrSKUID, UINT p_nSKUIDSize,
  LPWSTR p_wstrMapVersion, UINT p_nMapVersionSize,
  LPWSTR p_wstrMapRegion, UINT p_nMapRegionSize));

#endif // _WIN32_WCE

#endif // _REGISTRY_H_
