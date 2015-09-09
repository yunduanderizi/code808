#ifndef _SALAPPLICATION_H_
#define _SALAPPLICATION_H_

#include "libSAL.h"
#include "SALDefaults.h"

#define PUB_LOCAL	0	// denotes intra-process communication only
#define PUB_GLOBAL	1

class SALMessage;
class SALChannel;
class SALServiceProvider;

/**    @brief Application Skeleton interface.

		Purpose of the application skeleton is to provide a framework component 
		that allows easy creation of new applications fitting into the system 
		architecture of projects based on SAL.
		
		SALApplication is designed to be a singleton class so its class 
		constructors and destructor are protected and static class methods are 
		provided to create, delete or access the singleton object. SALApplication 
		sets up the basic infrastructure needed by any SAL application to 
		perform intra- and inter-process communication. An instance of a 
		SALChannel is created as input channel in order to receive SAL messages
		from other applications. Note that it is intended to have only one input
		channel per application in order to better control the flow of messages 
		(e.g. in special scenarios like shutdown incoming messages can easily be
		deferred). A message pool is created for storing the binary representation
		of SAL messages and event registries (local and global) are built up to 
		maintain event subscriptions. The global event registry is placed into 
		shared memory. The application skeleton itself is passive, that means no
		threads are started inside. Instead the application skeleton takes over 
		control of the thread that calls its run() method. Typically this is the
		application’s main thread that starts listening to incoming messages after
		the application’s subsystem was created and initialized properly. 
		
		It is intended that the application skeleton sets up the default debug 
		trace settings for an application. DebugTrace configuration via command 
		line parameters at startup time shall be supported in future. Currently 
		debug trace configuration at run-time is supported (see the debugtool in
		the examples section of the SAL/Software folder)

		A skeleton application is as follows:
@code
int main(int argc, char *argv[])
{
	// Initialize DebugTrace
	SAL_TrSetApp(APP_ID, "Application Name");
	SAL_TrInit(argc,argv);
	SAL_TrConfigure(DT_GLOBAL,SAL_DT_ZONE_INFO);	
	SAL_TrConfigure(DT_SAL,SAL_DT_ZONE_INFO | SAL_DT_ZONE_SCOPE);
	SAL_TrConfigure(DT_SAL,DTZ_SAL_THREADS);


	// Create the application instance.
	// This sets up the message pool, and registers the application
	// in the shared application registry.
	SALApplication::createInstance(APP_ID);

	// Start up all threads here to get real work done...

	// run the thread that sends and receive messages
	// to/from other applications
	SALApplication::getInstance()->run();
}
@endcode
*/

class SALApplication
{
public:
	//! singleton constructor
	static SALApplication* createInstance ( 
		word_t appID,
		const char* appName = 0, 
		word_t msgPoolSize=SAL_DEFAULT_MSGPOOL, 
		word_t maxQueueNumber=SAL_DEFAULT_QUEUES,
		word_t maxApps = SAL_DEFAULT_APPS,
		word_t maxEvents = SAL_DEFAULT_EVENTS
		);
	
	//! singleton destructor
	static void deleteInstance ( void );
	
	//! return the pointer to the singleton instance
	static SALApplication* getInstance ( void );

	//! return the application ID
	word_t getAppID ( void ) const;	
	
	//! run the application's event loop
	void run ( void );
	
	//! publish a message
	bool publish ( SALMessage& msg, byte_t mode=PUB_GLOBAL );
	
	/** post a message to the specified queue
	  * @param msg message to post
	  * @param queueID Queue to post message to. This may be
	  * a queue within the current process, or a different
	  * application.
	  */

	bool post ( SALMessage& msg, dword_t queueID );
		
	//! subscribe a queue to the specified set of events
	bool subscribeEvents ( int numOfEvents, const int eventList[], dword_t queueID );
	
	//! unsubscribe a queue to the specified set of events
	bool unsubscribeEvents ( int numOfEvents, const int eventList[], dword_t queueID );
	
protected:
	//! class constructor
	SALApplication ( 
		word_t appID,
		const char* appName,
		word_t msgPoolSize,
		word_t maxQueueNumber,
		word_t maxApps,
		word_t maxEvents
		);

	//! class destructor
	virtual ~SALApplication ( void );
	
	//! signal handling function
	static void onSignal ( int signo );

public:
	static bool mClearShareMemoryFlag;
		
protected:
	//! pointer to the singleton instance
	static SALApplication* mpInstance;
	
	//! Unique identifier for the application
	const word_t mAppID;
	
	//! application name
	char*  mAppName;
	
	//! Maximum number of queues for this application
	const word_t mMaxQueueNumber;

	//! Maximum number of applications
	const word_t mMaxAppNumber;

	//! Maximum number of events
	const word_t mMaxEventNumber;
	
	//! Pointer to service 
	SALServiceProvider* mpServiceProvider;
	
	//! application internal event registry. maps events to queues
	dword_t** mpEventRegistry;
	
	//! Repository of channels to other applications which are already created
	SALChannel** mpChannelRep;
	
	//! input channel of the application
	SALChannel* mpInputChannel;
};

#endif /*_SALAPPLICATION_H_*/
