#ifndef _SALCHANNELFACTORY_H_
#define _SALCHANNELFACTORY_H_

#include "SALChannelDefs.h"

class SALChannel;
class SALChannelConfig;

/**    @brief Communication Channel Factory Interface

		Communication channels provide a unified concept for inter-process 
		communication, for inter-processor communication and for communication 
		with external devices like GPS or RDS/TMC receivers. A major goal is to 
		hide the concrete channel implementation from the user depending on the 
		configuration settings and on the underlying RTOS. 
		
		To hide as much information as possible about the concrete channel 
		implementation channel objects cannot be created directly. Instead the 
		channel factory SALChannelFactory is used, which is a singleton class. 
		Channels are implemented using the handle/body or bridge design pattern. 
		That means the SALChannel class used by clients provides a common 
		interface to a channel and works as a delegator to the operating system 
		dependent implementation class. SALChannel itself only knows the abstract
		class SALChannelImp and is configured with a concrete channel 
		implementation object during construction by the SALChannelFactory. The 
		channel configuration is separated from the channel implementation. All 
		concrete channel configurations are derived from the base class 
		SALChannelConfig. Channel configuration might be created and passed to 
		the factory by a client or the client might provide an unique identifier 
		to the factory to select a certain configuration from a set of 
		preconfigured configuration objects. It is the responsibility of 
		SALChannelFactory to ensure that configuration and channel implementation 
		objects fit together when a new channel object is created.

		The set of channel implementation may encompass different physical channels
		(serial ports, sockets or shared memory based solutions) for different operating 
		systems (currently Win32, WinCE, Linux and QNX) with specialized behaviour
		(e.g. synchronous, asynchronous, one way or duplex). The different protocols
		must be registered in the channel factory before use, except for the cpSAL
		protocol. This allows users to add to the provided channel implementions
		without having to change any parts of SAL.

		SALChannelFactory is a singleton and controls creation of channels and 
		might also maintain some channel statistics.

*/

class SALChannelFactory
{
public:
	//! get a pointer to the factory instance (will be created if not existing)
	static SALChannelFactory* getInstance ( void );

	//! delete the singleton object
	static void deleteInstance ( void );

	/** Register a new implementation of a channel.
	  * Note that the serial and socket protocols are not registered by default,
	  * and must be added with a call such as:
	  * @code
	SALChannelFactory::getInstance()->registerImp(cpTCP, SALChannelImpSocket::alloc, SALChannelConfig::config);
	  * @endcode
	  * @param protocol ID of the protocol (see the enum eChannelProtocol)
	  * @param creator a function which creates a new channel for the protocol.
	  */
	void registerImp ( eChannelProtocol protocol, SALChannelImpCreator creator, SALConfigCreator config);

	//! create a new channel object
	//! @param appID application ID of this application
	//! @param remoteAppID application ID of the remote application
	//! @param name currently unused
	//! @param protocol Except for cpTCP, this must be registered before use.
	//! @param config A previously created and configured channel configuration
	//! @param mode Either CHANNEL_IO_READ, or CHANNEL_IO_WRITE. Some channel implementations may allow these to be OR'd
	//! @return Newly created communication channel
	SALChannel* createChannel (
		word_t appID, 
		word_t remoteAppID,
		char* name, 
		eChannelProtocol protocol, 
		SALChannelConfig& config, 
		byte_t mode
		);
//		read|write|duplex|sync );

	//! create a channel configuration suitable for the chosen protocol
	//! @param protocol ID of the protocol (see the enum eChannelProtocol)
	//! @return a sub-class of SALChannelConfig
	SALChannelConfig* createChannelConfig (eChannelProtocol protocol);
	
	
protected:
	//! protected default constructor
	SALChannelFactory ( void );
	
	//! protected class destructor
	virtual ~SALChannelFactory ( void );

protected:
	//! pointer to the singleton object
	static SALChannelFactory* mpInstance;

	static struct protocols {
		SALChannelImpCreator imp;
		SALConfigCreator config;
	} channelCreators[MAX_PROTO];
};

#endif /*_SALCHANNELFACTORY_H_*/
