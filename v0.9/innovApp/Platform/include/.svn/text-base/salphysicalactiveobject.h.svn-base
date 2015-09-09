#ifndef _SALPHYSICALACTIVEOBJECT_H_
#define _SALPHYSICALACTIVEOBJECT_H_

#if defined(WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#endif

#include "SALThread.h"

#define MAX_LOGICAL_THREADS		10
#define MAX_PHYSICAL_THREADS	10

class SALVirtualThread;
class SALPhysicalActiveObject;
class SALMessageQueue;


struct LogicalThreadEntry
{
	LogicalThreadEntry ( void ) : mpObject(0), mRunning(false) {}
	
	SALVirtualThread* mpObject;
	bool mRunning;
};

class SALPhysicalActiveObject 
	: public SALThread
{
public:
	static SALPhysicalActiveObject* attachActiveObject ( SALVirtualThread* pObject );
	static bool detachActiveObject ( SALVirtualThread* pObject );
	
	bool attachLogicalThread ( SALVirtualThread* pObject );	
	bool detachLogicalThread ( SALVirtualThread* pObject );
	
	bool run ( SALVirtualThread* pObject );

protected:
//	SALPhysicalActiveObject ( const char* name, unsigned int queueID );
	SALPhysicalActiveObject ( void );	
   ~SALPhysicalActiveObject ( void );

	void execute (void);
		
private:
	unsigned int  mQueueID;
	SALMessageQueue*  mpQueue;

	//! table to map logical to physical threads
	static unsigned int mThreadMapping[MAX_LOGICAL_THREADS];
	
	//! repository with thread object pointers
	static SALPhysicalActiveObject* mThreadRep[MAX_PHYSICAL_THREADS];

	//! repository with logical thread object pointers
	LogicalThreadEntry mLogicalThreadRep[MAX_LOGICAL_THREADS];
	
	//! number of attached logical threads
	unsigned int mNumOfLogicalThreads;
};

#endif /*_SALPHYSICALACTIVEOBJECT_H_*/
