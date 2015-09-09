#ifndef SALAPI_H_
#define SALAPI_H_

#include "libSAL.h"

//extern "C" {

class SALMessage;

//! publish a message (broadcast) 
bool sal_publish ( SALMessage& msg );
//void sal_publish ( word_t eventID, void* msgData, word_t msgLen );

//! publish an event (broadcast)
bool sal_publishEvent ( uint32_t event, dword_t p1 = 0, dword_t p2 = 0 );

//! post a message to the specified queue
bool sal_post ( SALMessage& msg, dword_t queueID );

//! subscribe for a list of events
bool sal_subscribeEvents ( int numOfEvents, const int eventList[], dword_t queueID );

//! unsubscribe for a list of events
bool sal_unsubscribeEvents ( int numOfEvents, const int eventList[], dword_t queue_ID );
	
//}

#endif /*SALAPI_H_*/
