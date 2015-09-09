#ifndef _SALCHANNELRS232CONF_H_
#define _SALCHANNELRS232CONF_H_

#include "SALTypes.h"
#include "SALChannelConfig.h"

/** @brief SAL Channel configuration for serial port channels
 */

class SALChannelConfigSerial
	: public SALChannelConfig
{
	friend class SALChannelImpSerial;
public:
	/** Create a SALChannelConfigSerial. This is passed to SALChannelFactory::registerImp()
	 */
	static SALChannelConfig* config();

	/** IDs for setValueProperty(). Note that not all architectures will support all these features */
	typedef enum {
		PORTNUMBER = 1,	//!< Port number. Defaults to 1. Some architectures may enumerate serial ports from zero.
		BAUDRATE,		//!< Baud rate. Defaults to 4800.
		DATABITS,		//!< Data bits. Defaults to 8.
		PARITY,			//!< Parity. Can be 'O', 'E', or 'N'. Defaults to 'N'.
		STOPBITS,		//!< Stopbits. 0 gives one stop bit, 1 gives 1.5 stop bits, 2 gives two stop bits. Defaults to 0.
		FLOW			//!< Flow control. Use 'N' for no flow control, 'H' for hardware flow control. Defaults to 'N'.
	} configType;

	bool setValueProperty(word_t config, dword_t value);

private:

	SALChannelConfigSerial ();

private:
	dword_t mBaudrate;
	word_t	mPortNumber;
	byte_t  mDataBits;
	byte_t  mParity;
	byte_t  mStopBits;
	byte_t	mFlow;
};

#endif /*_SALCHANNELRS232CONF_H_*/
