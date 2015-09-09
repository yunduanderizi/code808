#ifndef _SALVIRTUALTHREAD_H_
#define _SALVIRTUALTHREAD_H_


class SALPhysicalActiveObject;
class SALMessage;


class SALVirtualThread
{
	friend class SALPhysicalActiveObject;
	
public:
	SALVirtualThread ( const char* name, unsigned int tid, unsigned int qid );
	virtual ~SALVirtualThread ( void );
	
	//! start running the thread
	virtual bool run ( void );
	
	//! return logical thread identifier of this object
	virtual unsigned int getThreadID ( void ) const;
	
	//! return queue identifier of this object
	virtual unsigned int getQueueID ( void ) const;
	
	//! post a message to the active objects message queue
	bool postMessage ( SALMessage& Message );
	 
protected:
	virtual void initializeThread ( void ) {}
	 
	virtual void finalizeThread ( void ) {}
	 
	//! called in the context of the SALPhysicalActiveObject
	virtual void onProcessMessage ( SALMessage* pMessage ) = 0;
	
private:
	//! name of the active object for visualization
	const char* const mName;		
	
	//! logical thread identifier
	const unsigned int  mThreadID;
	
	//! logical queue identifier	
	const unsigned int  mQueueID;

	//! pointer to the real thread object used for performing tasks
	SALPhysicalActiveObject*  mpThread;
};


#endif /*_SALVIRTUALTHREAD_H_*/
