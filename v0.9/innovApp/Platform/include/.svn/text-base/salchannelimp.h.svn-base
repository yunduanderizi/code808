#ifndef _SALCHANNELIMP_H_
#define _SALCHANNELIMP_H_

#include "SALTypes.h"


class SALChannel;

/**    @brief Communication Channel Implementation Interface

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

		SALChannelImp is an abstract base class that defines the interface for 
		all different kinds of concrete channel implementations.
 */
class SALChannelImp
{
public:
	//! check if the channel is initialized properly
	virtual bool isInitialized ( void ) const;
	
	//! receive packages synchronously
	virtual bool receive ( byte_t* data, word_t* dataSize, word_t maxBytes, int32_t timeout = -1 );
	
	//! send a message synchronously
	virtual bool send ( const byte_t* data, word_t* dataSize, int32_t timeout = -1 );

	//! assign the channel object that uses this channel implementation
	virtual bool setOwner ( SALChannel* pChannel );
	
	//! template method. only sub classes itself will know how they have to be created
	virtual bool create ( void ) = 0;
	 	 
protected:
	//! protected default constructor to prevent creation of object (this is an abstract class)
	SALChannelImp ( void );
	
public:
	//! class destructor
	virtual ~SALChannelImp (void );
		
protected:
	//! flag to indicate whether the channel is initialized
	bool mIsInitialized;	
	
	//! channel object that uses this channel implementation
	SALChannel* mpChannel;
	
	//! data transmission modes (bit mask)
	byte_t mMode;
};

#endif /*_SALCHANNELIMP_H_*/
