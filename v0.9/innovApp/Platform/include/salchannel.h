#ifndef _SALCHANNEL_H_
#define _SALCHANNEL_H_

#include "SALTypes.h"


class SALChannelImp;

/**    @brief Communication Channel

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
		objects fit together when a new channel object is created. The set of 
		channel implementation may encompass different physical channels (serial 
		ports, sockets or shared memory based solutions) for different operating 
		systems (currently Win32, WinCE, Linux and QNX) with specialized behaviour
		(e.g. synchronous, asynchronous, one way or duplex).

		The abstract base class SALChannel defines the basic interface that is
		expected from a communication channel. We need to know if the channel is
		properly initialized and if so we need methods to send and receive binary
		data via the channel.

 */
class SALChannel
{
	friend class SALChannelFactory;
	
public:
	//! check if the channel is initialized properly
	virtual bool isInitialized ( void ) const;

	//! get ID of application that has created the channel
	virtual word_t getAppID ( void ) const;

	//! get ID of remote application that will receive messages via the channel
	virtual word_t getRemoteAppID ( void ) const;
	
	//! receive packages synchronously
	virtual bool receive ( byte_t* data, word_t* dataSize, word_t maxBytes, int32_t timeout=-1);
	
	//! send a message synchronously
	virtual bool send ( byte_t* data, word_t* dataSize, int32_t timeout=-1);
	
protected:
	//! protected constructor because this is an abstract class
	SALChannel ( word_t appID, word_t remoteAppID, SALChannelImp* pBody );

public: // should be changed to protected 

	//! protected class destructor
	virtual ~SALChannel ( void );

private:
	//! deprecated default constructor. Shall not be used by subclasses
	SALChannel ( void );

protected:
	//! unique identifier of the application
	word_t	mAppID;
	//! unique identifier of the remote application
	word_t	mRemoteAppID;
	//! pointer to the concrete channel implementation
	SALChannelImp* mpBody;
};
 
#endif /*_SALCHANNEL_H_*/
