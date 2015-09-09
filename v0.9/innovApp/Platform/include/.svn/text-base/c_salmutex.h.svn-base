#ifndef C_SALMUTEX_H
#define C_SALMUTEX_H

/** @file C_SALMutex.h
  * @brief C API to Mutual Exlusion interface.

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

*/


#ifdef __cplusplus
extern "C" {
#endif

#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

struct SALMutex;

#ifndef __cplusplus
enum {
	MODE_RECURSIVE = 0,
	MODE_NOT_RECURSIVE = 1
};
#endif

/** Create a mutex. The mutex is initialized in its unlocked state and available for use.
  * @param mode set to MODE_RECURSIVE for a recursive mutex, or MODE_NOT_RECURSIVE
  * for a non-recursive mutex. Must be deleted after use to free resources.
  */
struct SALMutex *sal_mutexInit(int mode);

/** Destroy the mutex. The mutex must be in an unlocked
  * state before it is destroyed.
  */
void sal_mutexDestroy(struct SALMutex *mutex);

   /** Get a lock on the mutex. A thread attempting to lock a mutex that is already
     * locked by another thread is suspended until the owning thread unlocks the mutex.
     * If called by the thread that already has a lock on this mutex and the mutex is
     * recursive, a reference count is incremented, and an equal number of calls to sal_mutexUnlock()
     * are required to unlock the mutex. If the mutex is non-recursive, calling sal_mutexLock() on
     * an already locked mutex causes the thread to deadlock until another thread calls
     * unlock().
     * @return true on success, false otherwise. */
int sal_mutexLock(struct SALMutex *mutex);

   /** Unlock a mutex. If a recursive mutex is locked more than once by the calling thread,
     * an equal number of calls is required before the mutex is unlocked. */
int sal_mutexUnlock(struct SALMutex *mutex);

#ifdef __cplusplus
}
#endif

#endif

