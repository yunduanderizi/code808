#pragma once
#ifndef _SDCARDFUNCS_H
#define _SDCARDFUNCS_H

#ifdef __cplusplus

BOOL sdcard_wait(LPCWSTR p_wstrSDCardPath, UINT8 p_nCount = 10, UINT8 p_nSleep = 100);
BOOL sdcard_runExecutable(LPCWSTR p_wstrCalledFrom, LPCWSTR p_wstrSDCardPath, LPCWSTR p_wstrDevicePath);

namespace navman {

#if defined(_WIN32_WCE) && defined(TARGET)

class SDCardThread;

BOOL sdcard_createThread(const HWND p_hRootWindow, const UINT p_nMessage, SDCardThread **p_ppSDCardThread);
BOOL sdcard_destroyThread(SDCardThread **p_ppSDCardThread);
BOOL sdcard_onDeviceChange(UINT p_uMsg, WPARAM p_wParam, LPARAM p_lParam, HWND p_hRootWindow, UINT p_nMessage);

#endif // _WIN32_WCE && TARGET

} // namespace navman

#endif // __cplusplus

#endif // _SDCARDFUNCS_H
