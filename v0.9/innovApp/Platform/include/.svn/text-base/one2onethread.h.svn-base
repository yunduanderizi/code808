#pragma once

#ifndef _ONE2ONETHREAD_H
#define _ONE2ONETHREAD_H

//#ifdef HAS_FEATURE_ONE2ONE

#include <plat_serial.h>

namespace navman
{
  //class One2OneRxThread;

  //extern class navman::Comm g_One2OnePort;

  /*****************************************************************************
   * BOOL serial_openOne2One(void)
   *
   * inputs: None
   * outputs: None
   *
   * description: Initialises the platform's One2One RS232 serial port.
  *****************************************************************************/
  //BOOL serial_openOne2One(void);


  /*****************************************************************************
   * void serial_closeOne2One(void)
   *
   * inputs: None
   * outputs: None
   *
   * description: Shuts down the platform's One2One RS232 serial port.
  *****************************************************************************/
  //void serial_closeOne2One(void);

  BOOL serial_createOne2OneThread(SOne2OneComms *p_pOne2OneComms);

  void serial_destroyOne2OneThread(void);


  /*****************************************************************************
   * BOOL serial_handleOne2One(char *p_strData, int p_nLength)
   *
   * inputs: p_strData - One2One data sentence.
   *         p_nLength - Length of sentence.
   * outputs: None
   *
   * description: 
  *****************************************************************************/
  BOOL serial_handleOne2One(char *p_strData, int p_nLength);

  //#endif // HAS_FEATURE_ONE2ONE

} // namespace navman

#endif // _ONE2ONETHREAD_H
