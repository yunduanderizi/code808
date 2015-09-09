#ifndef SALCHANNELCONFIGSOCKET_H_
#define SALCHANNELCONFIGSOCKET_H_

#include "libSAL.h"
#include "SALChannelConfig.h"

/** @brief SAL Channel configuration for TCP sockets. */

class SALChannelConfigSocket
	: public SALChannelConfig
{
	friend class SALChannelImpSocket;

public:
	/** Create a SALChannelConfigSocket. This is pased to SALChannelFactory::registerImp() */
	static SALChannelConfig* config();

	/** IDs for setStringProperty() and setValueProperty(). */
	typedef enum {
		LOCAL_IPADDR = 1,	/**< Local IP address to bind to. Defaults to NULL which allows incoming connections from anwhere.
							     Set to "127.0.0.1" to allow connections only from the current machine. */
		REMOTE_IPADDR,		//!< Remote IP address to connect to. Defaults to "127.0.0.1"
		PORT				//!< Base port number to use. Defaults to 2500.
	} configType;

	bool setStringProperty(word_t config, const char* value);
	bool setValueProperty(word_t config, dword_t value);

private:
	//! constructor
	SALChannelConfigSocket(); 
	
private:
	//! ip address of the local machine
	const char* mIpAddr;
	
	//! ip address of the machine listening on the socket
	const char* mIpAddrRemote;
	
	//! port to be used
	word_t mPort;
};

#endif /*SALCHANNELCONFIGSOCKET_H_*/
