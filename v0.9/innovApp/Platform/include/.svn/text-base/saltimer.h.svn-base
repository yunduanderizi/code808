#ifndef SALTIMER_H_
#define SALTIMER_H_

#include "libSAL.h"

#if defined(WIN32) || defined(_WIN32_WCE)
#include "SALThread.h"
#elif defined(LINUX)
#include <signal.h>
#elif defined(QNX)
#include <signal.h>
typedef sigval sigval_t;
#endif

#define TM_SINGLE			0x00
#define TM_CYCLIC			0x01
#define TM_INVALID_ID		-1
#define SAL_MAX_TIMERS		100


class SALMutex;

/** SAL Timer Support
  *
  * SAL supports a pool of timers which can be allocated by the various threads. A
  * thread can either be single shot, or periodic. All times are specified in
  * milliseconds. When a timer expires, it posts an event to a queue.
  * Timers can be stopped, and the type and period can be changed when they are stopped.
  */

class SALTimer
#if defined(WIN32) || defined(_WIN32_WCE)
	: public SALThread
#endif
{
public:
	//! spawn a new Timer
	static SALTimer* createTimer ( uint32_t timerBase, uint8_t timerType = TM_SINGLE );
	
	//! start the timer if it was not activated before
	bool startTimer ( int32_t eventID, int32_t queueID, void* cookie1, void* cookie2 );
	
	//! stop the timer if it was activated before
	bool stopTimer ( void );	
	
	//! return the timer's unique identifier
	int32_t getTimerID ( void ) const;
	
	//! set the timer base (only possible if the timer is not activated)
	bool setTimerBase ( uint32_t value );

	//! set the timer type (only possible if the timer is not activated)
	bool setTimerType ( uint8_t type );
	
protected:
	SALTimer ( void );
	SALTimer ( uint32_t timerBase, uint8_t timerType );
	virtual ~SALTimer ( void );
	
	//! setup the timer internally
	void setupTimer ( void );

	//! object specific signal handling method
	virtual void timerElapsed ( void );

private:
#if defined(WIN32) || defined(_WIN32_WCE)
	//! entrypoint for thread (from SALThread)
	void execute(void);
#else
	//! entrypoint for the OS thread	
	static void threadEntryPoint ( sigval_t value );
#endif
	
protected:
	//! unique timer ID
	int32_t  mID;
					
	//! timer value in milliseconds
	uint32_t  mTimerBase;		
	
	//! single shot/periodic
	uint8_t  mTimerType; 		
	
	//! event ID of the timer message to deliver
	int32_t  mEventID;
	
	//! event ID of the timer message to deliver
	int32_t  mQueueID;
	
	//! event ID of the timer message to deliver
	void*  mCookie[2];
	
	//! flag to indicate if the timer is activated
	bool mIsActive;
	
	//! flag to indicate if the timer was properly created
	bool mIsValid;
	
	//! ring buffer with IDs of empty slots
    static int16_t  mFreeList[SAL_MAX_TIMERS];
    
    //! first free slot in the repository
    static int16_t  mHead;
    
    //! last free slot in the repository
    static int16_t  mTail;
	
    //! number of empty slots in the repository
    static int16_t  mFreeSlots;
    
    //! number of used slots in the repository
    static int16_t  mNumOfTimers;
    
	//! timer repository
	static SALTimer* mTimerLookupTable[SAL_MAX_TIMERS];	

	//! mutex for concurrent access to the timer repository
	static SALMutex* mMutex;

	//! SALTimer is working with signals. This flags indicates if the handler is installed
	static bool mIsSignalHandlerInstalled;
	
	//
#if defined(WIN32) || defined(_WIN32_WCE)
	DWORD mSysTime;
	HANDLE mHandle;
#else
	struct sigevent   mSigEvent;
	struct itimerspec mTimerSpec;
	timer_t mTimerID;
#endif
};

#endif /*SALTIMER_H_*/

/*
A timer shall be identified by a unique ID ... done

Timer values shall be specified in milliseconds

Single-shot and periodic timers shall be supported

It shall be possible to stop a timer

The timer shall send a notification when the time is elapsed

The timer shall pass back at least two parameters in notifications (one parameter for an object pointer in an object oriented design and one parameter for a client specific cookie). The parameters shall be passed to the timer during construction or client registration.

It shall be possible that the timer notifies via a callback function (function pointer is passed to the timer during construction or client registration)

It shall be possible that the timer notifies by sending a message (message is passed to the timer during construction or client registration)

It shall be possible that the timer broadcasts a timer event to subscribed client
*/
