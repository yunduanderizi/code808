#ifndef _SALCHANNELDEFS_H_
#define _SALCHANNELDEFS_H_

#include "SALTypes.h"

class SALChannelConfig;
class SALChannelImp;

/** @file SALChannelDefs.h
  * Various definitions relating to channels
  */

// channel modi (might be combined)
#define CHANNEL_IO_READ			((byte_t) 0x01)
#define CHANNEL_IO_WRITE		((byte_t) 0x02)
#define CHANNEL_IO_DUPLEX		((byte_t) 0x03)
#define CHANNEL_IO_SYNC			((byte_t) 0x04)


/** Maximum number of protocols. SAL defines protocol cpSAL as 0,
  * and cpSerial and cpTCP are normally 1 and 2. This leaves 3..9 available. */

#define MAX_PROTO	10

/** type definition of a function to create a channel. Used as
  * the argument type to SALChannelFactory::registerImp()
  */
typedef SALChannelImp* (*SALChannelImpCreator)(const char* name, SALChannelConfig& config, byte_t mode);

/** type definition of a function to create a channel configuration. Used
  * as the argument type to SALChannelFactory::registerImp()
  */
typedef SALChannelConfig* (*SALConfigCreator)(void);

/** Definitions of built-in channel protocols */
enum eChannelProtocol
{
	cpSAL = 0,			//!< predefined communication mechanism used for IPC between SAL application 
	cpSerial, 			//!< serial port communication
	cpTCP,				//!< tcp/ip socket communication
	cpNumOfProtocols
};

#endif /*_SALCHANNELDEFS_H_*/
