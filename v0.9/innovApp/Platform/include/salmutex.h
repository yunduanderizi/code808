#ifndef _SALMUTEX_H_
#define _SALMUTEX_H_

#if defined(WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#else
#include <pthread.h>
#endif

enum {
	MODE_RECURSIVE = 0,
	MODE_NOT_RECURSIVE = 1
};

/**    @brief Mutual Exclusion interface.

       A mutex is a MUTual EXclusion device, and is useful for protecting
       shared data structures from concurrent modifications, and implementing
       critical sections and monitors.

       A mutex has two possible states: unlocked (not owned by any thread),
       and locked (owned by one thread). A mutex can never be owned by two
       different threads simultaneously. A thread attempting to lock a mutex
       that is already locked by another thread is suspended until the owning
       thread unlocks the mutex first.

       Two types of mutex are available: recursive, and non-recursive. A thread
       that already has locked a recursive mutex can lock the same mutex again,
       and the call will return immediately. Trying this with a non-recursive
       mutex will cause the thread to deadlock. The recursive mutex needs to
       be unlocked once for each time it is locked before it returns to its
       unlocked state.

       A recursive mutex can only be unlocked by the thread that locked it.
       A non-recursive mutex can be unlocked by any thread.
       
       In general the usage of recursive mutexes is recommended in order to
       prevent dead-lock situations. Thus "recursive" is the default setting when
       a mutex object is created. But there are situation where a non-recursive 
       mutex is require, e.g. for condition variables. 

 */
class SALMutex
{
	friend class SALCondVar;
	
public:
   /** Create a mutex. The mutex is initialized in its unlocked state and available for use.
     * @param mode set to MODE_RECURSIVE for a recursive mutex, or MODE_NOT_RECURSIVE
     * for a non-recursive mutex. The defuault is recursive. */
   SALMutex ( int mode=MODE_RECURSIVE );

   /** Destroy the mutex. The mutex must be in an unlocked state before it is destroyed. */
   virtual ~SALMutex ( void );

   /** Get a lock on the mutex. A thread attempting to lock a mutex that is already
     * locked by another thread is suspended until the owning thread unlocks the mutex.
     * If called by the thread that already has a lock on this mutex and the mutex is
     * recursive, a reference count is incremented, and an equal number of calls to unlock()
     * are required to unlock the mutex. If the mutex is non-recursive, calling lock() on
     * an already locked mutex causes the thread to deadlock until another thread calls
     * unlock().
     * @return true on success, false otherwise. */
   bool lock ( void );
   /** Unlock a mutex. If a recursive mutex is locked more than once by the calling thread,
     * an equal number of calls is required before the mutex is unlocked. */
   bool unlock ( void );

private:
   /* Prevent assignment and copy on this class */
   SALMutex( const SALMutex& );
   SALMutex& operator = ( const SALMutex& );

   bool mIsInitialized;

#if defined(WIN32) || defined(_WIN32_WCE)
	const int mMode;
	union {
		// Use a critical section for a recursive mutex
		struct {
			CRITICAL_SECTION mCritical;
			DWORD mOwner;
			DWORD mCount;
		};
		// Use an event for a non-recursive ("fast") mutex
		struct {
			HANDLE mEvent;
		};
	};
#else
	pthread_mutex_t  mMutex;
#endif
};

#endif /*_SALMUTEX_H_*/
