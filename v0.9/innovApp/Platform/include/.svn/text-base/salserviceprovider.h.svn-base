#ifndef SALSERVICEPROVIDER_H_
#define SALSERVICEPROVIDER_H_

#if defined(WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#else
#include <sys/types.h>
#include <semaphore.h>
#endif
#include "libSAL.h"
#include "SALDefaults.h"
#include "SALChannelConfig.h"

// forward declarations
class SALSharedMemory;

/** @brief Maintain a list of global services
  * 
  * SALServiceProvider (SP) is a module that is globally available to all SAL modules and which provides 
  * global services and information to them. For example: SP maintains a global table of all defined events 
  * and subscriptions to these events. With this mechanism it is possible for a module to subscribe to an 
  * event without knowing where the server is located that publishes the event.
  */

class SALServiceProvider
{
	friend class SALApplication;

private:
	struct SharedServiceData {
		SALChannelConfig* appRegistry;
		uint8_t  eventRegistry[0];
#if 0
	private:
		SharedServiceData(SharedServiceData&);
		SharedServiceData& operator=(SharedServiceData&);
#endif
	};

	struct SharedServiceData* getAppData(int appID);

public:
	//! singleton constructor or accessor
	static SALServiceProvider* getInstance ( word_t appID=0, int maxEvents=SAL_DEFAULT_EVENTS, bool eraseMode=false);
	
	//! singleton destructor
	static void deleteInstance ( void );
	
	//! add events to the event registry
	bool addEventSubscription ( int numOfEvents, const int eventList[] );

	//! remove events events from the event registry
	bool removeEventSubscription ( int numOfEvents, const int eventList[] );

protected:
	//! class constructor
	SALServiceProvider ( word_t appID, int maxEvents, bool eraseMode );
	
	//! class destructor
	virtual ~SALServiceProvider ( void );
	
protected:
	//! ID of the application
	const word_t mAppID;

	//! Number of events
	const word_t mNumEvents;

	//! pointer to the singleton object
	static SALServiceProvider* mpInstance;

	//!	pointer to the shared memory object
	static SALSharedMemory* mpSharedMemory;

	//! pointer to the mapped shared memory area
	static SharedServiceData* mpSharedData;

#if defined(WIN32) || defined(_WIN32_WCE)
	//! name for the shared memory object
	static const TCHAR* mSharedMemoryName;
	
	//! name for the semaphore used to access control to the shared memory object
	static const TCHAR* mSharedMemorySemaphoreName;
	
	// named semaphore
	HANDLE	mServiceDataSemaphore;
#else
	//! name for the shared memory object
	static const char* mSharedMemoryName;
	
	//! name for the semaphore used to access control to the shared memory object
	static const char* mSharedMemorySemaphoreName;
	
	// named semaphore
	sem_t*	mServiceDataSemaphore;
#endif
};

#endif /*SALSERVICEPROVIDER_H_*/
