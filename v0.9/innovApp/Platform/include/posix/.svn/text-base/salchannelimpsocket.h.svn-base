#ifndef SALCHANNELSOCKETIMP_H_
#define SALCHANNELSOCKETIMP_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define SOCKET_ERROR	-1

#include "libSAL.h"
#include "SALChannelImp.h"


class SALChannelConfig;
class SALChannelConfigSocket;

class SALChannelImpSocket
	: public SALChannelImp
{
public:
	static SALChannelImp* alloc ( const char* name, SALChannelConfig& Config, byte_t mode );
	SALChannelImpSocket ( const char* name, SALChannelConfigSocket* pConfig, byte_t mode );
	virtual ~SALChannelImpSocket ( void );

	//! receive packages synchronously
	bool receive ( byte_t* data, word_t* dataSize, word_t maxBytes, int32_t timeout );
	
	//! send a message synchronously
	bool send ( const byte_t* data, word_t* dataSize, int32_t timeout );

protected:
	//! protected constructors and destructor because this is an abstract class
	SALChannelImpSocket ( void );

	//! create the channel
	virtual bool create ( void );
	virtual bool destroy ( void );
	
	//! initialize TCP/IP sockets
	static void initTcpIp ( void );
	
	//! evaluation of socket errors
	void onSocketError ( int socketFuncID, int error );
	
protected:
	const char* const mName;
	SALChannelConfigSocket* mConfig;

   	struct sockaddr_in mSrvAddr;
	int mSocket;
   	
	// flag to indicate if TCP/IP was properly initialized
	static bool mIsTcpIpInitialized;
};

#endif /*SALCHANNELSOCKET_H_*/
