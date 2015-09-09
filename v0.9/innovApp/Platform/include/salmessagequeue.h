#ifndef _SALMESSAGEQUEUE_H_
#define _SALMESSAGEQUEUE_H_

#include "SALTypes.h"
#include "SALMutex.h"
#include "SALCondVar.h"
//#include "SALPriorityQueue.h"

#include "SALAbstractQueue.h"

//! FIFO Message queue interface
class SALMessageQueue : public SALAbstractQueue
{
public:
	//! class constructor
	//! @param queueID unique queue ID in the range 0..SAL_MAX_QUEUES-1
	SALMessageQueue ( dword_t queueID, uint8_t size = SAL_MAX_QUEUESIZE );
	
	//! class destructor
	virtual ~SALMessageQueue ( void );

private:
    //! return the first message from the queue. message is removed
    int32_t pop ( void );

    //! append a new message to the queue
    bool append ( int32_t key, int pPriority );

	//! queue of pending messages (works like a ring buffer)
	int32_t* const mpQueue;
	
	//! position of the first element in the queue
	uint8_t  mHead;
	
	//! position of the last element in the queue
	uint8_t  mTail;
	
};

#endif /*_SALMESSAGEQUEUE_H_*/
