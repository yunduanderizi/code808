#ifndef C_SALCONDVAR_H
#define C_SALCONDVAR_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

struct SALCondVar;
struct SALMutex;

/** @file C_SALCondVar.h
    @brief C API to SALCondVar functions.

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


/** Create a Condition Variable. The pointer returned contains no user-visible
  * members. sal_condVarDestroy() must be called after the condition variable is
  * no longer needed, to deallocate any resources used.
  * @return pointer to the condition variable.
  */
struct SALCondVar *sal_condVarInit(void);

/** Destroy a Condition Variable. If any threads are waiting for to be signalled
  * the result is undefined.
  * @param cv condition variable created with SALCondVar_init().
  */
void sal_condVarDestroy(struct SALCondVar *cv);

/** atomically unlocks the mutex and waits for the condition variable to be
   signaled. The thread execution is suspended and does not consume any
   CPU time until the condition variable is signaled. The mutex must be
   locked by the calling thread on entrance.  Before returning to the
   calling thread,  the mutex is re-acquired.
   @param cv condition variable.
   @param mutex required mutual exclusion semaphore.
   @return TRUE on success, FALSE otherwise.
*/

int sal_condVarWait(struct SALCondVar *cv, struct SALMutex *mutex);

/** similar to wait(), but with a timeout. The mutex is always locked
    before returning, regardless of whether the call timed out or not.
    @param cv condition variable.
    @param mutex required mutual exclusion semaphore.
	@param timeout number of milliseconds to wait.
    @return true on success, false if timeout or other error.
 */
int sal_condVarWaitTimeout(struct SALCondVar *cv, struct SALMutex *mutex, uint32_t timeout);

/** restarts one of the threads that are waiting on the
   condition variable.  If no threads are waiting, nothing
   happens. If several threads are waiting, exactly one is
   restarted, but it is not specified which.
   @param cv condition variable.
   @return TRUE on success.
*/
int sal_condVarSignal(struct SALCondVar *cv);

/** restarts all of the threads that are waiting on the
   condition variable.  If no threads are waiting, nothing
   happens.
   @param cv condition variable.
   @return TRUE on success.
*/
int sal_condVarBroadcast(struct SALCondVar *cv);

#ifdef __cplusplus
}
#endif

#endif

