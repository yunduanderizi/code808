#pragma once
/*****************************************************************************
 *  (c) 2005 navman new zealand limited. all rights reserved.
 *
 *  module name: InitFuncs.h
 *
 *  author(s): Tom Isaacson <tisaacson@navman.com>
 *
 *  description:
 *    This module 
 *****************************************************************************/

#ifndef _INITFUNCS_H_
#define _INITFUNCS_H_

//#include "platform_features.h"

BOOL init_readSettingFromPathsFile(LPSTR p_strReturn,
  LPCWSTR p_wstrRoot,
  LPCSTR p_strValue,
  LPCSTR p_strKeyOpen1,
  LPCSTR p_strKeyClose1,
  LPCSTR p_strKeyOpen2_1,
  LPCSTR p_strKeyClose2_1,
  LPCSTR p_strKeyOpen2_2,
  LPCSTR p_strKeyClose2_2);


class CDeviceSettings
{
  public:
    bool construct(void);

    CDeviceSettings();
    ~CDeviceSettings();

    // Memory
    INT32 m_nMapCacheSizeMax;
    INT32 m_nMapCacheSizeMin;
    INT32 m_nMinStoragePage;
    INT32 m_nBigMallocBound;
    BOOL  m_bSharedMemHeap;
    // GPS
    BOOL  m_bGPS;
    char  m_strGPSPort[10];
    INT32 m_nNMEAOldBaudRate;
    INT32 m_nNMEABaudRate;
    BOOL  m_bSiRFCLMStorageOpenClose;
    BOOL  m_bDefaultProtocolIsNMEA;
    BOOL  m_bUseSiRFBinaryProtocol;
    INT32 m_nSiRFBinaryBaudRate;
    BOOL  m_bSwitchProtocol;
    BOOL  m_bUseSiRFCLM;
    BOOL  m_bGPSColdStart;
    // Traffic
    BOOL  m_bTMC;
    char  m_strTMCPort[10];
    INT32 m_nTMCBaudRate;
    BOOL  m_bTMCDuplexMode;
    BOOL  m_bTMCGNSModule;
    INT32 m_nTMCCommandTimeout;
    INT32 m_nTMCCommandRetry;
    // Battery
    INT32 m_nLowWarningBatteryPercentage;
    INT32 m_nCriticalWarningBatteryPercentage;
    // FM Transmitter
    BOOL  m_bFMTransmitterAvailable;
    // Kernel
    BOOL  m_bNoSDCardXMLFile;
};

CDeviceSettings *DeviceSettings(void);

struct SThreadParams
{
  HWND  hWnd;
  UINT  nMessage;
};


extern BOOL      g_bShutdown;
extern HANDLE    g_hRS232Event;
//#if defined(DEFINED_PRODUCT_TUI) && !defined(HAS_FEATURE_MME_USB_DRIVER)
extern HANDLE    g_hUSBConnectEvent;
extern HANDLE    g_hUSBDisconnectEvent;
//#endif // DEFINED_PRODUCT_TUI && !HAS_FEATURE_MME_USB_DRIVER
extern HANDLE    g_hWakeupEvent;

BOOL OpenDebugIniFile(void);
BOOL ReadDebugIniFileDbgSettings(const char *p_strSection);

BOOL DoesFileExist(const WCHAR *p_strFile, long *p_plBytes = NULL);
BOOL DoesFileNameExist(WCHAR *p_strPath, WCHAR *p_strFilename, long *p_plBytes);
//BOOL IsDestFileOlder(WCHAR *p_strDestFile, WCHAR *p_strSrcFile);
BOOL AddToFileList(WCHAR *p_strDestFile, WCHAR *p_strSrcFile, long p_lFileSize);
int DoesFileExistInDirectory(WCHAR *p_strPath, WCHAR *p_strFileExt);
int AddDirToFileList(WCHAR *p_strDestPath, WCHAR *p_strSrcPath, WCHAR *p_strFileExt, BOOL p_bRecursive, WCHAR *p_strCopyPath);
//BOOL IsDestDirFileOlder(WCHAR *p_strDestPath, WCHAR *p_strSrcPath, WCHAR *p_strFileExt, BOOL p_bRecursive);
BOOL IsDestDirFileMissing(WCHAR *p_strDestPath, WCHAR *p_strSrcPath, WCHAR *p_strFileExt, BOOL p_bRecursive);
BOOL CreateDirectoryLocal(WCHAR *p_strDir);
BOOL DoesDirectoryExist(WCHAR *p_strDir);
BOOL RemoveDirectoryLocal(WCHAR *p_strDir);
BOOL DeleteFileLocal(WCHAR *p_strFile);

#ifdef _WIN32_WCE
DWORD WINAPI RS232EventThreadFunc(LPVOID p_lpParam);
//#if defined(DEFINED_PRODUCT_TUI) && !defined(HAS_FEATURE_MME_USB_DRIVER)
DWORD WINAPI USBConnectEventThreadFunc(LPVOID p_lpParam);
DWORD WINAPI USBDisconnectEventThreadFunc(LPVOID p_lpParam);
//#endif // DEFINED_PRODUCT_TUI && !HAS_FEATURE_MME_USB_DRIVER
DWORD WINAPI WakeupEventThreadFunc(LPVOID p_lpParam);

BOOL IsActiveSyncConnected(void);
//#ifdef HAS_FEATURE_USB_MASS_STORAGE
BOOL IsUSBMassStorageConnected(void);
//#endif
BOOL IsUSBConnected(BOOL p_bUsingActivesync); // = FALSE
BOOL StartUSBScreen(LPWSTR p_wstrCommandLine);
BOOL StartAppStartupPri(LPWSTR p_wstrCommandLine);

BOOL KillAllApplications(void);
void OrganiseMemory(BOOL8 p_bMaximiseAppMem);
#endif

#endif // _INITFUNCS_H_
