#pragma once
/*****************************************************************************
*  (c) 2008 navman new zealand limited. all rights reserved.
*
*  module name: common.h
*
*  author(s): Tom Isaacson <tom.isaacson@navman.co.nz>
*
*  description:
*    common library interface
*****************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#define UTILISE_NMEA_LOG_PLAYER // Use the NMEA Log Player in Navigation to play the GPS and TMC log files

#include "loaddll.h"
#include "sdcardfuncs.h"
#include "../../../common/device/include/device.h"


namespace navman
{
  typedef enum EGpsResetType { GPS_DEFAULT_START, GPS_COLD_START, GPS_WARM_START, GPS_HOT_START };
  typedef enum EDGpsType { DGPS_NONE, DGPS_WAAS, DGPS_EGNOS, DGPS_MSAS };

  struct SComms
  {
    HWND  hRootWindow;
    BOOL  bPortOpen;
    UINT  nMessage;
    WCHAR wstrPort[10];
    INT32 nBaudRate;
  };

  typedef BOOL ThandleGPS(char *p_strGPSData, int p_nLength);
  typedef void TlogGPSData(LPCSTR p_strGPSData, UINT p_nLength);

  struct SGPSComms
  {
    SComms  comms;
    INT32   nNMEAOldBaudRate;
    BOOL    bSiRFCLMStorageOpenClose;
    BOOL    bDefaultProtocolIsNMEA;
    BOOL    bUseSiRFBinaryProtocol;
    INT32   nSiRFBinaryBaudRate;
    BOOL    bSwitchProtocol;
    BOOL    bUseSiRFCLM;
    BOOL    bGPSColdStart;
    ThandleGPS  *pHandleGPS;
    TlogGPSData *pLogGPSData;
  };

  struct STrafficComms
  {
    SComms  comms;
    UINT    nTrafficTimer;
    UINT    nReplayTRFLogTimer;
    BOOL    bFullDuplex;
    BOOL    bGNSModule;
    INT32   nTimeout;
    INT32   nRetry;
  };

  struct SOne2OneComms
  {
    SComms  comms;
  };

  struct SCommonMessages
  {
    // Root window
    HWND  hRootWindow;
    // SD card
    UINT  nSDCardMsg;

    SGPSComms     gps;
    STrafficComms traffic;
    SOne2OneComms one2one;
  };

  class CCommon
  {
    public:
      bool construct(void);

      CCommon(void);
      ~CCommon(void);

      void setMessages(SCommonMessages *p_pMessages);

      BOOL handleMessage(UINT p_uMsg, WPARAM p_wParam, LPARAM p_lParam);
      BOOL handleTimer(UINT32 p_nTimerID, LPARAM p_lParam);

      BOOL getDeviceName(LPWSTR p_wstrDeviceName, UINT p_nBufferLength) const;
      BOOL getDeviceDisplayName(LPWSTR p_wstrDeviceDisplayName, UINT p_nBufferLength) const;
      BOOL getHardwareVersion(INT32 *p_pnHWVersion) const;
      BOOL getHardwareVersionString(LPWSTR p_wstrVersion, UINT p_nBufferLength) const;
      BOOL getSKUSettings(
        LPWSTR p_wstrProjectCode, UINT p_nProjectCodeLength,
        LPWSTR p_wstrSKUID, UINT p_nSKUIDLength,
        LPWSTR p_wstrMapVersion, UINT p_nMapVersionLength,
        LPWSTR p_wstrMapRegion, UINT p_nMapRegionLength) const;

      BOOL setDevice(TDeviceType p_eDevice) const;
      BOOL getApplicationID(LPWSTR p_wstrKey, UINT8 p_nKeyLength) const;
      BOOL isFeatureEnabled(CDeviceInterface::TFeature p_eFeature, BOOL *p_pbEnabled) const;
      BOOL getFeatureSetting(CDeviceInterface::TFeature p_eFeature, INT8 *p_pnSetting) const;
      BOOL setBrightness(UINT8 p_nBrightness, const UINT8 p_nMin, const UINT8 p_nMax);
      BOOL getBrightness(UINT8 *p_pnBrightness, const UINT8 p_nMin, const UINT8 p_nMax) const;
      BOOL setContrast(UINT8 p_nContrast);
      BOOL getContrast(UINT8 *p_pnContrast) const;
      BOOL setScreenBacklight(BOOL p_bBacklight);
      BOOL getScreenBacklight(BOOL *p_pbBacklight) const;
      BOOL setTouchScreenLock(BOOL p_bLock);
      BOOL getTouchScreenLock(BOOL *p_pbLock) const;
      BOOL getTemperature(SSystemTemp *p_pTempReading);

      BOOL setTSCalibration(LPCWSTR p_wstrTSCalibration, UINT8 p_nLength);
      BOOL getTSCalibration(LPWSTR p_wstrTSCalibration, UINT8 *p_pnLength) const;

      BOOL setLastTime(const LPSYSTEMTIME p_pTime);
      BOOL getLastTime(LPSYSTEMTIME p_pTime) const;
      BOOL isLastTimeMoreRecent(const LPSYSTEMTIME p_pTime) const;
      BOOL setFirstUseTime(const LPSYSTEMTIME p_pTime);
      BOOL getFirstUseTime(LPSYSTEMTIME p_pTime) const;

      BOOL setUSB(BOOL p_bOn);
      BOOL getLCID(DWORD *p_pdwLCID) const;
      BOOL getRegionVersion(DWORD *p_pdwLCID) const;
      BOOL getBaseHardware(LPWSTR p_wstrBaseHardware, UINT p_nLength) const;

      BOOL getPowerStatus(
        EExtPowerStatus *p_pExtPowerStatus,
        EBatteryStatus *p_pBatteryStatus,
        INT8 *p_pnBatteryPercent);

      BOOL openGPS(BOOL p_bRestart);
      BOOL closeGPS(void);

      BOOL resetGPS(EGpsResetType p_eResetType);
      BOOL setDGPS(EDGpsType p_eDGPSType);

      BOOL shutdownAndRestartGPS(void);

      BOOL createThreads(BOOL p_bStartTraffic);
      BOOL destroyThreads(void);

      // SD card
      BOOL  m_bTuiSDCardEvent;
      // USB
      BOOL  m_bUSBMassStorage;
      BOOL  m_bMMEUSBDriver;

    private:
      SCommonMessages   m_msg;
      CLoadDll          m_deviceDll;
#ifdef _POSIX
      void              *m_pDevice;
#else
      CDeviceInterface  *m_pDevice;
#endif
#ifdef _WIN32_WCE
      SDCardThread      *m_pSDCardThread;
#endif
  };

  CCommon *Common(void);
} // namespace navman

#endif // _COMMON_H_
