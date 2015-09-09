#pragma once

#ifndef _SERIALTHREAD_H
#define _SERIALTHREAD_H

#include <plat_serial.h>


namespace navman
{
  extern BOOL g_gpsSettingChanged;   //this may be set somewhere in the UI if settings change to force re-read

  #ifndef HAS_FEATURE_SIRF_CLM
    // Handle to the current GPS serial port
    extern class Comm g_GPSPort;
  #endif

  /*****************************************************************************
   * BOOL serial_openGPS(void)
   *
   * inputs: None
   * outputs: None
   *
   * description: Initialises the platform's GPS RS232 serial port. It must
   *              be called before using the os_readSerial function. Returns
   *              TRUE on success.
  *****************************************************************************/
  BOOL serial_openGPS(BOOL p_bRestart);


  /*****************************************************************************
   * void serial_closeGPS(void)
   *
   * inputs: None
   * outputs: None
   *
   * description: Shuts down the platform's GPS RS232 serial port.
  *****************************************************************************/
  void serial_closeGPS(void);


  /*****************************************************************************
  * BOOL serial_createGPSThread(RxThread **p_ppRxThread)
  *
  * inputs: None
  * outputs: p_ppRxThread - Pointer to GPS Rx thread created.
  *
  * description: Creates Rx thread for GPS. Returns TRUE on success.
  *****************************************************************************/
  BOOL serial_createGPSThread(BOOL p_bRestart,
                              SGPSComms *p_pCommSettings);


  /*****************************************************************************
  * BOOL serial_destroyGPSThread(RxThread **p_ppRxThread)
  *
  * inputs: p_ppRxThread - Pointer to GPS Rx thread to destroy.
  * outputs: none
  *
  * description: Destroys Rx thread for GPS. Returns TRUE on success.
  *****************************************************************************/
  void serial_destroyGPSThread(void);


  BOOL serial_shutdownAndRestartGPS(SGPSComms *p_pCommSettings);


  #ifndef UTILISE_NMEA_LOG_PLAYER
  /*****************************************************************************
   * BOOL serial_timerHandler(UINT32 p_nTimerID, LPARAM p_lParam)
   *
   * inputs: p_nTimerID - Timer ID.
   *         p_lParam - Additional data.
   * outputs: None
   *
   * description: Called when a serial timer is triggered. Returns TRUE on
   *              success.
  *****************************************************************************/
  BOOL serial_timerHandler(UINT32 p_nTimerID, LPARAM p_lParam);
  #endif // !UTILISE_NMEA_LOG_PLAYER


  /*****************************************************************************
   * void serial_resetGPS(EGpsResetType p_eResetType = GPS_COLD_START)
   *
   * inputs: p_eResetType - type of reset to be performed, default is cold boot.
   * outputs: None
   *
   * description: Resets the GPS. If real GPS is being used this sends a reset
   *              message to the GPS unit, causing it to reboot, and informs
   *              Appcore of the reset. If a logfile is being played it restarts
   *              playback from the beginning of the file. If no file is being
   *              played but one with the correct name exists playback will
   *              start, and the GPS unit will not be reset.
  *****************************************************************************/
  void serial_resetGPS(TlogGPSData *p_pLogGPSData, EGpsResetType p_eResetType = GPS_DEFAULT_START);


  /*****************************************************************************
  * void serial_setDGPS(EDGpsType p_eDGPSType)
  *
  * inputs: p_eDGPSType - type of DGPS system to use.
  * outputs: None
  *
  * description: Sets the Differential GPS type used. Returns TRUE on success.
  *****************************************************************************/
  BOOL serial_setDGPS(TlogGPSData *p_pLogGPSData, EDGpsType p_eDGPSType);


  /*****************************************************************************
   * BOOL serial_isLogFile(void)
   *
   * inputs: None
   * outputs: None
   *
   * description: Returns TRUE when the system is running a GPS or TMC log file.
   *****************************************************************************/
  BOOL serial_isLogFile(void);

} // namespace navman

#endif // _SERIALTHREAD_H
