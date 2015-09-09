#ifndef _SALAPPINFO_H_
#define _SALAPPINFO_H_

#include <unistd.h>
#include "libSAL.h"
#include "SALChannelConfig.h"

class SALChannelConfigQNX
	: public SALChannelConfig
{
	friend class SALChannelImpQNX;
public:
	static SALChannelConfig* config();

	typedef enum {
		CHAN_ID=1,
		PROC_ID,
		CONN_ID
	} configType;

	bool setValueProperty(word_t config, dword_t value);
private:
	//! constructor 
	SALChannelConfigQNX ( void );
	
#if 0
	//! class destructor
	~SALChannelConfigQNX ( void );
	
	pid_t getProcessID( void );
	int getChannelID( void );
	int getConnectionID( void );
		
	void setChannelID(int id);
	void setProcessID(pid_t id);
	void setConnectionID(int conID);
#endif
	
private:
	int mCONID;
	pid_t	mPID;
	int	mCHID;
};




#endif //_SALAPPINFO_
