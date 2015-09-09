#pragma once
// TrafficThread.h : The thread that manages the traffic COM port
//
#ifndef _TRAFFICTHREAD_H
#define _TRAFFICTHREAD_H

#include <plat_serial.h>


//class TrafficRxThread;

namespace navman
{
  extern STrafficComms g_trafficComms;
  extern class navman::Comm g_TrafficPort;


  // The entry point for Mapcore character transmit to traffic port
  // this was initially designed to be a serial transmit, however
  // the high level NMEA processing is now done in this driver
  UINT8 traffic_driverSendChar(char ch);

  /*****************************************************************************
   * UINT8 serial_openTraffic( void )
   *
   * inputs: none
   * outputs: returns TRUE on successful open of Traffic serial port
   *
   * description: open the Traffic serial port.
  *****************************************************************************/
  BOOL serial_openTraffic(void);

  /*****************************************************************************
   * UINT8 serial_closeTraffic( void )
   *
   * inputs: none
   * outputs: none
   *
   * description: close the traffic serial port connection.
   *****************************************************************************/
  void serial_closeTraffic(void);

  /*****************************************************************************
   * void trafficPortWakeup( void )
   *
   * inputs: none
   * outputs: none
   *
   * description: detects sleep of > 1 hour 
   *****************************************************************************/
  void trafficPortWakeup(void);

  /*****************************************************************************
   * BOOL traffic_createThread(STrafficComms *p_pTrafficComms)
   *
   * inputs: Pointer to traffic comms.
   * outputs: returns TRUE on success
   *
   * description: starts the traffic thread
   *****************************************************************************/
  BOOL traffic_createThread(STrafficComms *p_pTrafficComms);

  /*****************************************************************************
   * BOOL traffic_destroyThread()
   *
   * inputs: Pointer to traffic thread handle.
   * outputs: returns TRUE on success
   *
   * description: stops the traffic thread
   *****************************************************************************/
  BOOL traffic_destroyThread(void);

  /*****************************************************************************
  * void traffic_startTRFLogPlayer(void)
  *
  * inputs: None.
  * outputs: None.
  *
  * description: starts the TRF log player
  *****************************************************************************/
  void traffic_startTRFLogPlayer(void);

  /*****************************************************************************
  * BOOL traffic_timerHandler(UINT32 p_nTimerID, LPARAM p_lParam)
  *
  * inputs: p_nTimerID - Timer ID.
  *         p_lParam - Additional data.
  * outputs: None
  *
  * description: Called when a traffic timer is triggered. Returns TRUE on
  *              success.
  *****************************************************************************/
  BOOL traffic_timerHandler(UINT32 p_nTimerID, LPARAM p_lParam);

  /**
    Sets the traffic update time
    @param newValue Specifies the new traffic update time (in seconds)
  */
  void traffic_setUpdateTime(UINT32 newValue);

} // namespace navman

#endif // _TRAFFICTHREAD_H
