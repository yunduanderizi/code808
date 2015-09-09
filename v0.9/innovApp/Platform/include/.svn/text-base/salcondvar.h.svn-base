#ifndef _SALCONDVAR_H_
#define _SALCONDVAR_H_

#include "SALTypes.h"

#if defined(WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#else
#include <pthread.h>
#endif

class SALMutex;

/**    @brief Operations on conditions.

       A  condition variable is a synchronization
       device that allows threads to suspend execution and relinquish the
       processors until some predicate on shared data is satisfied. The basic
       operations on conditions are: signal the condition (when the predicate
       becomes true), and wait for the condition, suspending the thread
       execution until another thread signals the condition.

       A condition variable must always be associated with a mutex, to avoid
       the race condition where a thread prepares to wait on a condition
       variable and another thread signals the condition just before the first
       thread actually waits on it.
*/

class SALCondVar
{
public:
   	SALCondVar ( void );
   	virtual ~SALCondVar ( void );

        /** atomically unlocks the mutex and waits for the condition variable to be
           signaled. The thread execution is suspended and does not consume any
           CPU time until the condition variable is signaled. The mutex must be
           locked by the calling thread on entrance.  Before returning to the
           calling thread,  the mutex is re-acquired.
           @param mutex required mutual exclusion semaphore.
        */
   	bool wait ( SALMutex* mutex );

		/** similar to wait(), but with a timeout. The mutex is always locked
		    before returning, regardless of whether the call timed out or not.
		    @param mutex required mutual exclusion semaphore.
			@param timeout number of milliseconds to wait.
		    @return true on success, false if timeout or other error.
		 */

	bool waitTimeout ( SALMutex* mutex, uint32_t timeout );

        /** restarts one of the threads that are waiting on the
           condition variable.  If no threads are waiting, nothing
           happens. If several threads are waiting, exactly one is
           restarted, but it is not specified which.
        */
	bool signal (void );

        /** restarts all of the threads that are waiting on the
           condition variable.  If no threads are waiting, nothing
           happens.
        */
	bool broadcast ( void );
	
private:
   	/* Prevent assignment and copy on this class */
   	SALCondVar( const SALCondVar& );
   	SALCondVar& operator = ( const SALCondVar& );

#if defined(WIN32) || defined(_WIN32_WCE)
	CRITICAL_SECTION critList;

	struct Waiter *head;
	struct Waiter *tail;

	static const int CACHE_SIZE = 2;
	HANDLE eventCache[CACHE_SIZE];
	int eventCacheLen;
#else
	pthread_cond_t mCondVar;
#endif
   	bool mIsInitialized;
};

#endif /*_SALCONDVAR_H_*/
