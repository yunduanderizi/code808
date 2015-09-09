#ifndef _SALPRIORITYQUEUE_
#define _SALPRIORITYQUEUE_

#include "SALAbstractQueue.h"
#include "SALPriorityList.h"

//! Priority Message Queue Interface
class SALPriorityQueue : public SALAbstractQueue
{
public:
	//! class constructor
	//! @param queueID unique queue ID in the range 0..SAL_MAX_QUEUES-1
	SALPriorityQueue(dword_t queueID, uint8_t size = SAL_MAX_QUEUESIZE);
	
	//! class destructor
	virtual ~SALPriorityQueue();

private:
    //! return the first message from the queue. This is the highest priority
	//! message available. Messages of equal priority are queued in FIFO order.
    int32_t pop ( void );

	//! add a new message to the queue in a position dependent on priority
	bool append ( int32_t key, int pPriority );

	//! The priority list itself
	SALPriorityList mPriorityList;
};

#endif //_SALPRIORITYQUEUE_

