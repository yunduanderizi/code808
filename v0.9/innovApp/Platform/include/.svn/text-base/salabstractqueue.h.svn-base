#ifndef _SALABSTRACTQUEUE_H_
#define _SALABSTRACTQUEUE_H_

#include "SALTypes.h"
#include "SALMutex.h"
#include "SALCondVar.h"

#define  SAL_MAX_QUEUESIZE		50
#define  SAL_MAX_QUEUES			50

#define	QUEUE_APPID(key)		((word_t)((key) >> 16))
#define	QUEUE_QID(key)			((word_t)((key) & 0xFFFF))
#define MAKE_QUEUEID(app, id)	(((dword_t)(app) << 16) | ((dword_t)(id)))

class SALMessage;

/** @brief Generic Message Queue Interface.
 *
 *  This class in an abstract class which is subclassed to define actual
 *  message queues. The only functions the subclass needs to define
 *  are append() and pop(), which add and remove items to the queue.
 *
 *  Two implementations of queues are currently defined: SALMessageQueue::
 *  and SALPriorityQueue::. SALMessageQueue:: implements a FIFO queue,
 *  whereas SALPriorityQueue:: implements a priority queue.
 *
 *  Pointers to each created queue are stored in a globally accessible
 *  lookup table.
 */

class SALAbstractQueue
{
public:
	//! lookup table to find queue in the application
	static SALAbstractQueue* mQueueLookupTable[SAL_MAX_QUEUES];

	//! append a message to the internal queue
	//! @return true if message was posted successfully
	bool postMessage ( const SALMessage& pMessage, word_t virtualQueueID, bool blocking = false, dword_t timeout = 0 );
	
	//! extract the next message from the queue with a timeout.
	//! If no message is on the queue, the thread is suspended until
	//! a message is available. If blocking is set to false, the
	//! call will return after the timeout is reached.
	//! @param[out] message has data set from queue
	//! @param blocking if set, waits forever for a message
	//! @param timeout number of milliseconds to wait for message if not blocking
	//! @return true if successful, false on timeout
	bool getMessage( SALMessage& message, bool blocking = true, dword_t timeout = 0 );

	//! Extract a message from the queue if one is available.
	//! If no message is available, returns false and does not block.
	//! This call is identical to getMessageTimeout() without blocking and a zero timeout.
	//! @param[out] message has data set from queue
	//! @return true if successful, false if no messages are available
	bool peekMessage( SALMessage& message) {
		return getMessage(message, false);
	}
	
	//! check if the queue is full.
	bool isFull ( void ) const { 
		return (mSize == mMaxSize);
	}

	//! check if the queue is empty.
	bool isEmpty ( void ) const {
		return (mSize == 0);
	}

	//! find the maximum number of messages that have been in the queue
	//! and reset the peak level
	word_t getPeakLevel( void );

protected:
	SALAbstractQueue ( dword_t queueID, uint8_t size );
	virtual ~SALAbstractQueue ( void );

	//! unique queue identifier
	const dword_t  mQueueID;
	
private:
    //! return the first message from the queue. message is removed
    virtual int32_t pop ( void ) = 0;

    //! append a new message to the queue
    virtual bool append ( int32_t key, int pPriority ) = 0;

	//! append a message (raw format --> byte stream) to the queue
	//! and signal any process waiting on this queue.
	bool postMessage ( int32_t key, int pPriority, bool blocking, dword_t timeout );
	
	//! mutex for queue access (read/write) synchronization
	SALMutex  mMutex;
	
	//! conditional variable to signal that messages are in the queue
	SALCondVar  mFetcher;

	//! conditional variable to signal that space is in the queue
	SALCondVar mPoster;

	//! maximum size of this queue
	const uint8_t mMaxSize;

	//! number of pending messages in the queue
	uint8_t  mSize;

	//! maximum number of messages in the queue (for diagnostics)
	uint8_t  mPeakLevel;

};

#endif
