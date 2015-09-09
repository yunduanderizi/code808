#ifndef SALTHREAD_H_
#define SALTHREAD_H_

#include "libSAL.h"
#include "SALMutex.h"
#include "SALCondVar.h"

#if defined(WIN32) || defined(_WIN32_WCE)
typedef DWORD OSThreadRet_t;
#define THREAD_API	WINAPI
#else
typedef void *OSThreadRet_t;
#define THREAD_API
#endif

/** @brief Create a new thread which runs concurrently with the calling thread.

    As a minimum, the execute() method must be provided by a derived class.

	It is also advisable to override the destructor with a call to <tt>stop(true)</tt>
    as this ensures the thread has exited before variables it may be using
	are deleted.
 */

class SALThread
{
	DECLARE_FRIEND_SAL_THREAD_TEST_CLASSES

public:
	/** IDs for priorities of new threads. Note that not all architectures will support all these features */
	typedef enum {
		TIME_CRITICAL = 1, //!< 15
		HIGHEST, //!< 2
		ABOVE_NORMAL, //!< 1
		NORMAL, //!< 0
		BELOW_NORMAL, //!< -1
		LOWEST, //!< -2
		IDLE //!< -15
	} priorityValue;

	//! Create a new thread.
	//! @param name name for the new thread
	//! @param tid logical thread identifier
	//! @param priority priority of the newly created thread (default NORMAL)
	SALThread ( const char* name, unsigned int tid, priorityValue priority = NORMAL );
	//! Destructor does not ensure that the thread has first been stopped.
	virtual ~SALThread ( void );

	//! start running the thread.
	//! @return true if thread was started OK
	virtual bool run ( void );

	//! return logical thread identifier of this object.
	virtual unsigned int getThreadID ( void ) const;

	//! stop the running thread.
	//! The mStopProcessing variable is set to inform the thread to stop,
	//! but the thread is not signalled in any other way.
	//! @param wait Wait until the thread is terminated (default no wait)
	virtual bool stop ( bool wait = false ); 

protected:
	//! thread specific method to be provided by subclasses
	virtual void execute (void) = 0;

private:
#if defined(WIN32) || defined(_WIN32_WCE)
	//! Internal function to set the priority of the newly created thread
	bool setPriority(void);
#endif

	//! thread entrypoint function we pass to the thread creation function
	static OSThreadRet_t THREAD_API entrypoint(void* pObject);

	//! name of the active object for visualization
	const char*  mName;		

	//! logical thread identifier
	const unsigned int  mThreadID;
	
	//! Priority of this thread
	const priorityValue mPriority;

#if defined(WIN32) || defined(_WIN32_WCE)
	HANDLE mOsHandle;
	DWORD mOsThreadID;
#endif
#ifdef LINUX
	//! ID of the PThreads thread
	pthread_t mOsThreadID;
#endif

protected:
	//! set by run()
	bool mRunning;
	//! set when a call to stop() is made.
	bool mStopProcessing;

	//! barrier object for thread synchronization
	SALCondVar mCondVar;
	SALMutex mMutex;
};

#endif /*SALTHREAD_H_*/
