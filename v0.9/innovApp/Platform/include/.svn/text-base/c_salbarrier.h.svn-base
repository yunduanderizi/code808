#ifndef C_SALBARRIER_H
#define C_SALBARRIER_H

/** @file C_SALBarrier.h
  * @brief C API to Barrier rendezvous mechanism.

	The barrier function allows participating threads at synchronize at a barrier.
    The calling thread will block until the required number of threads have called
	join().

    When the required number of threads have called sal_barrierJoin(), all threads
    can continue execution. The barrier can be used again by the same threads to
    synchronize at another point.
*/


#ifdef __cplusplus
extern "C" {
#endif

struct SALBarrier;

/** Create a barrier.
 * @param numOfThreads Number of threads which must call sal_barrierJoin() before execution
 * continues.
 */
struct SALBarrier *sal_barrierInit ( uint8_t numOfThreads );
/** Destroy a barrier.
  * No threads should be waiting on this barrier when it is destroyed.
  */
void sal_barrierDestroy(struct SALBarrier *barrier);
	
/** Join the synchronization barrier. Execution is blocked until all threads have
  * joined.
  */
void sal_barrierJoin (struct SALBarrier *barrier);
	

#ifdef __cplusplus
}
#endif

#endif

