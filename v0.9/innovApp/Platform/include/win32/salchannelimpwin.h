#ifndef SALCHANNELIMPWin_H_
#define SALCHANNELIMPWin_H_

#ifdef _WIN32_WCE

#include <windows.h>

#include "libSAL.h"
#include "SALChannelImp.h"

class SALChannelConfig;
class SALChannelConfigWin;

class SALChannelImpWin
	: public SALChannelImp
{
public:
	static SALChannelImp* alloc ( const char* name, SALChannelConfig& Config, byte_t mode );
	SALChannelImpWin ( const char* name, SALChannelConfigWin* pConfig, byte_t mode );
	virtual ~SALChannelImpWin ( void );

	//! receive packages synchronously
	bool receive ( byte_t* data, word_t* dataSize, word_t maxBytes, int32_t timeout );
	
	//! send a message synchronously
	bool send ( const byte_t* data, word_t* dataSize, int32_t timeout );

protected:
	//! protected constructors and destructor because this is an abstract class
	SALChannelImpWin ( void );

	//! create the channel
	virtual bool create ( void );
	virtual bool destroy ( void );
	
protected:
	const char* const mName;
	HANDLE mHandle;
	SALChannelConfigWin* mConfig;
	word_t mReadSize;

};

#endif
#endif
