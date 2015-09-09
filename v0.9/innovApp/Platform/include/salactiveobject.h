#ifndef _SALACTIVEOBJECT_H_
#define _SALACTIVEOBJECT_H_

#if defined(WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "SALThread.h"
#include "SALMessageQueue.h"

class SALMessage;

typedef bool (*CBFunc_PollingLoop) ( void* );


class SALActiveObject
	: public SALThread
{
	struct PollModeData
	{
		PollModeData ( void ) 
			: pObject(0), pCBFunc(0) 
		{}
		
		void* pObject;
		CBFunc_PollingLoop pCBFunc;
	};
	
public:
	//! class constructor
	SALActiveObject ( const char* name, unsigned int tid, unsigned int qid );
	
	//! class destructor
	virtual ~SALActiveObject ( void );
	
	//! start running the thread
	virtual bool run ( void );

	//! enable polling mode
	void enablePollingMode ( void* pObject, CBFunc_PollingLoop pCBFunc );
		
	//! return logical thread identifier of this object
//	virtual unsigned int getThreadID ( void ) const;
	
	//! return queue identifier of this object
	virtual unsigned int getQueueID ( void ) const;
	
	//! post a message to the active objects message queue
	bool postMessage ( SALMessage& pMessage );
	 
protected:
	virtual void initializeThread ( void ) {}
	 
	virtual void finalizeThread ( void ) {}
	 
	//! called in the context of the SALPhysicalActiveObject
	virtual void onProcessMessage ( SALMessage* pMessage ) = 0;

	//this is the threads main loop
	virtual void execute (void);
		
private:
	//! queue identifier
	const unsigned int mQueueID;
	
	//! pointer to the queue
	SALMessageQueue mpQueue;

	//! flag to indicate if the thread work in polling mode
	bool mPollMode;
	
	//! information needed to iterate the polling loop
	PollModeData mPollInfo;
};

#endif /*_SALACTIVEOBJECT_H_*/
