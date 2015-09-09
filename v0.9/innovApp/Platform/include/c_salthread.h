#ifndef C_SALTHREAD_H
#define C_SALTHREAD_H

/** @file C_SALThread.h
  * @brief Create a new thread which runs concurrently with the calling thread.
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

struct SALThread;

/** Create a new thread. The thread will not be running,
  * and the entry point needs to be set sal_threadSetEntryPoint()
  * before use.
  * @param name name for the new thread
  * @param tid logical thread identifier
  * @return new thread structure pointer
  */
struct SALThread *sal_threadInit(const char* name, unsigned int tid);

/** Destroy the thead. The thread should already be stopped before
  * this function is called.
  */
void sal_threadDestroy(struct SALThread *thread);

/** Start running the thread.
  */
int sal_threadRun(struct SALThread *thread);

/** Stop running the thread. Currently this only sets an internal
  * variable which can't be accessed by the thread.
  */
int sal_threadStop(struct SALThread *thread);

/** Return the logical thread identifier that was set when the
  * thread was created.
  */
unsigned int sal_threadGetThreadID(struct SALThread *thread);

/** Specify the entry point for the thread. This must be done before
  * the thread is started. The entry point is called with as single
  * parameter, a void pointer.
  * @param thread the thread to affect
  * @param entry pointer to the function for the thread to execute.
  * @param arg the parameter to pass to the entry-point function.
  */
void sal_threadSetEntryPoint(struct SALThread *thread, void (*entry)(void *), void *arg);

#ifdef __cplusplus
}
#endif

#endif

