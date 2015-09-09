#ifndef SALCHANNELSOCKETIMPQNX_H_
#define SALCHANNELSOCKETIMPQNX_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define SOCKET_ERROR	-1

#include "libSAL.h"
#include "SALChannelImp.h"


class SALChannelConfig;
class SALChannelConfigQNX;

class SALChannelImpQNX
	: public SALChannelImp
{
public:
	static SALChannelImp* alloc ( const char* name, SALChannelConfig& Config, byte_t mode );
	SALChannelImpQNX ( const char* name, SALChannelConfigQNX* pConfig, byte_t mode );
	virtual ~SALChannelImpQNX ( void );

	//! receive packages synchronously
	bool receive ( byte_t* data, word_t* dataSize, word_t maxBytes );
	
	//! send a message synchronously
	bool send ( const byte_t* data, word_t* dataSize );

protected:
	//! protected constructors and destructor because this is an abstract class
	SALChannelImpQNX ( void );

	//! create the channel
	virtual bool create ( void );
	virtual bool destroy ( void );
	
protected:
	const char* const mName;
	SALChannelConfigQNX* mConfig;

	// flag to indicate if TCP/IP was properly initialized
	static bool mIsInitialized;
};

#endif /*SALCHANNELSOCKET_H_*/
