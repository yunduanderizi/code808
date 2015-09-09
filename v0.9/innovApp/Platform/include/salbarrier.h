#ifndef _SALBARRIER_H_
#define _SALBARRIER_H_

#include "libSAL.h"

#if defined(LINUX) || defined(QNX)
typedef pthread_barrier_t SALBarrierHandle_t;
#endif

/** @brief Barrier interface

	The barrier function allows participating threads at synchronize at a barrier.
    The calling thread will block until the required number of threads have called
	join().

    When the required number of threads have called join(), all threads can continue
    execution. The barrier can be used again by the same threads to synchronize at
    another point.
*/

class SALBarrier
{
public:
	/** Create a barrier.
	 * @param numOfThreads Number of threads which must call join() before execution
	 * continues.
	 */
	SALBarrier ( uint8_t numOfThreads );
	virtual ~SALBarrier ( void );
	
	/** Join the synchronization barrier. Execution is blocked until all threads have
     * joined.
     */
	void join ( void );
	
protected:
#if defined(WIN32) || defined(_WIN32_WCE)
	HANDLE	mEvent[2];
	LONG	mCount[2];
	uint16_t	mStep;
	const uint16_t mTrigger;
#else
	SALBarrierHandle_t  mHandle;
#endif
};

#endif /*_SALBARRIER_H_*/
