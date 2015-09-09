#pragma once
#ifndef __PLAT_SERIAL_H
#define __PLAT_SERIAL_H

#ifdef __cplusplus

#ifdef _POSIX
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif // _POSIX

namespace navman {

class Comm {
public:

#ifdef _POSIX
  enum EventFlags {
    RXFLAG = TIOCM_DTR, // Since DTR is an output, this is OK
    CTS = TIOCM_CTS,
    DSR = TIOCM_DSR,
    RLSD = TIOCM_CD,
    RING = TIOCM_RNG
  };
  enum ModemLines {
    DTR = TIOCM_DTR,
    RTS = TIOCM_RTS
  };

#else // !_POSIX
  enum EventFlags {
    BREAK = EV_BREAK,     // A break was detected on input.
    CTS = EV_CTS,         // The CTS (clear-to-send) signal changed state.
    DSR = EV_DSR,         // The DSR (data-set-ready) signal changed state.
    ERR = EV_ERR,         // A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY.
    RING = EV_RING,       // A ring indicator was detected.
    RLSD = EV_RLSD,       // The RLSD (receive-line-signal-detect) signal changed state.
    RXCHAR = EV_RXCHAR,   // A character was received and placed in the input buffer.
    RXFLAG = EV_RXFLAG,   // The event character was received and placed in the input buffer. The event character is specified in the device's DCB structure, which is applied to a serial port by using the SetCommState function.
    TXEMPTY = EV_TXEMPTY, // The last character in the output buffer was sent.
  };
  enum ModemLines {
    DTR = 1,
    RTS = 2
  };

#endif // !_POSIX

#ifdef _POSIX
  Comm() : m_fd(-1) {}
#else
  Comm() : m_hPort(INVALID_HANDLE_VALUE),
           m_dwEvtMask(EV_RXFLAG | EV_CTS | EV_DSR | EV_RLSD | EV_RING),
           m_bEnableHandshaking(false)
  {
    memset(m_PortName, 0, sizeof(m_PortName));
  }
#endif
  virtual ~Comm() { close(); }

  /** Open port ready for reading/writing.
   * Will fail if the port is already open.
   * @param bEnableHandshaking turns hardware handshaking on.
   * @return true if port opened successfully.
   */
#ifdef _POSIX
  bool open(int port, UINT32 baudRate);
  bool open(const WCHAR* portName, UINT32 baudRate);
#else
  bool open(int port, UINT32 baudRate, void *timeouts = NULL, bool bEnableHandshaking = false);
  bool open(const WCHAR* portName, UINT32 baudRate, void *timeouts = NULL, bool bEnableHandshaking = false);
#endif
  /** Close the port. It is safe to call this more than once.
   * The destructor calls this automatically.
   */
  virtual bool close();
  /** Test to see whether the port is opened.
  */
#ifdef _POSIX
  bool isOpen() { return m_fd >= 0; }
#else
  bool isOpen() { return m_hPort != INVALID_HANDLE_VALUE; }
#endif
  /** Find number of characters available to be read.
  */
  UINT32 avail();
  /** Empty all transmit and receive queues.
  */
  void purge();
  /** Write characters to the port. If an error occurs
   * zero is returned, otherwise the number of characters
   * written is returned.
   */
  DWORD write(const void *data, DWORD length);
  /** Read characters from the port. If an error occurs
   * zero is returned, otherwise the number of characters
   * read is returned.
   */
  DWORD read(void *data, DWORD length);
  /** Wait for either a change in the modem status lines
   * or available data. If setCanon() is called, waits
   * for a line of data (terminated by a linefeed).
   */
  EventFlags waitEvent();
  /** Set canonical input mode. This makes waitEvent()
   * wait for a line of data before returning.
   */
  void setCanon();
  void setCanon(EventFlags mask);
  EventFlags getCanon();
  /** Set Modem status lines. Lines specified in mask
   * will be set to the value specified in value.
   */
  void setModemCtrl(ModemLines mask, ModemLines value);
  /** return the port name
   */
  WCHAR const * const getPortName() {return m_PortName;}
  /** set the baud rate
  */
#ifdef _POSIX
  void setBaudRate(UINT32 baudRate) { }
#else
  void setBaudRate(UINT32 baudRate) { setUpPort(baudRate, NULL);}
#endif

protected:
  #define SERIAL_PORT_NAME_LENGTH 10
  WCHAR   m_PortName[SERIAL_PORT_NAME_LENGTH];
#ifdef _POSIX
  int m_fd;
  int m_modemStatus;
#else
  // this functionality separated so that it can be overridden - see open()
  virtual bool createPort(const WCHAR *portName, bool bEnableHandshaking);
  virtual bool setUpPort(UINT32 baudRate, void *timeouts);
  HANDLE  m_hPort;
  DWORD   m_dwEvtMask;
  bool    m_bEnableHandshaking;
private:
  Comm(Comm const &);       // prevent copy ctor
  Comm & operator=(Comm &); // prevent assignment op

#endif
};

} // navman

#endif // __cplusplus

#endif // __PLAT_SERIAL_H

