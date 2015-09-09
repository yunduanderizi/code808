#ifndef _LIBSAL_H_
#define _LIBSAL_H_

/** @mainpage SAL Reference Manual
  *
  * @section Intro Introduction
  * The System Abstraction Layer (SAL) provides fundamental architecture
  * concepts in an operating system independent manner. It ensures
  * portability to different operating systems.
  * Therefore it is a precondition to allow licensing of a Navigation SDK by
  * OEM suppliers. It comprises thread support, synchronization
  * mechanisms, communication models for intra- and inter-process as well as
  * inter-processor communication, error handling, memory management and
  * logging of trace messages.
  *
  * @section Classes C++ Classes
  *    	- Application Skeleton
  * 		- SALApplication:: (@link Application/public/SALApplication.h @endlink)
  *    	- Communication Channels (including inter-process)
  *    		- SALChannel:: (@link Communication/public/SALChannel.h @endlink)
  *    		- SALChannelImp:: (@link Communication/public/SALChannelImp.h @endlink)
  * 		- SALChannelFactory:: (@link Communication/public/SALChannelFactory.h @endlink)
  * 		- SALChannelConfig::
  *    	- Message Passing
  * 		- SALMessage:: (@link Communication/public/SALMessage.h @endlink)
  * 		- SALMessageQueue:: (@link Communication/public/SALMessageQueue.h @endlink)
  * 		- SALPriorityQueue:: (@link Communication/public/SALPriorityQueue.h @endlink)
  *    	- Thread support
  * 		- SALThread::
  *    	- Synchronization mechanisms
  * 		- SALMutex::
  * 		- SALScopeLock::
  * 		- SALCondVar::
  * 		- SALBarrier::
  *    	- Process skeleton
  * 		- SALApplication::
  *
  * Please see @ref comms "Events, Messages, Queues and Channels" for information on using the inter-process message passing
  * and Application framework.
  *
  * @section CAPI C API
  * The SAL library can be used from C as well as from C++. The header files
  * which are specific to the C API have a "C_" prefix. The documentation can
  * be found in the following files:
  *    - Thread support
  *       - @link Concurrency/public/C_SALThread.h @endlink
  *    - Synchronization mechanisms
  *       - @link Concurrency/public/C_SALMutex.h @endlink
  *       - @link Concurrency/public/C_SALCondVar.h @endlink
  *       - @link Concurrency/public/C_SALBarrier.h @endlink
  *
  * Some files are common to both the C++ and C implementation. These include:
  *   - @link Common/public/SALTime.h @endlink
  *   - @link Common/public/libSAL.h @endlink
  *   - @link Common/public/SALTypes.h @endlink
  *   - @link Common/public/SALAppIDs.h @endlink
  *   - @link Common/public/SALEventIDs.h @endlink
  *   - @link DebugTrace/public/SALDebugTrace.h @endlink
  *   - @link Common/public/SALCustomize.h @endlink
  *
  * @page comms Events, Messages, Queues and Channels
  * 
  * @section Channel
  * 
  * A channel is an inter-application datagram-based communication channel.
  * When a channel is opened for writing, the remote application ID is used
  * to identify where messages will be sent. When a channel is opened for
  * reading, it will accept messages from any process.
  * 
  * There are different types of channel that can be used depending on what
  * the underlying operating system offers. Three types of channel are
  * currently defined: cpSAL, cpTCP and cpSerial. The cpSAL channel is used
  * for sending messages between applications. Under WindowsXP and Linux,
  * these channels are implented as TCP Sockets. WindowsCE uses the MsgQueue
  * mechanism, and QNX uses its own channel implementation. Serial channels
  * are also implemented which allow communication to/from GPS or other
  * devices.
  * 
  * The type of channel to be created is specified as a parameter when
  * creating the channel with SALChannelFactory::createChannel(). A ChannelConfig::
  * object should be created first and configured for the type of channel
  * that is being opened. This is done with SALChannelFactory::createChannelConfig().
  * 
  * The SALApplication:: uses a channel to send messages between different
  * applications (but not within an application). This channel is of type
  * cpSAL. If SALApplication:: is used, it is normally not necessary to
  * create your own instance of a cpSAL channel.
  * 
  * An example of creating and using a cpSAL-type channel is as follows:
  *
@code
#include "SALChannel.h"
#include "SALChannelDefs.h"

main(int argc, char *argv[])
{

	// Register the TCP communication channel.
	// This isn;t necessary in this example as the cpSAL channel will be used
	SALChannelFactory::getInstance()->registerImp(cpTCP, SALChannelImpSocket::alloc, SALChannelConfigSocket::config);

	// Create the configuration for a SAL channel.
	// Note that this is OS-dependent
	SALChannelConfig* settings = SALChannelFactory::getInstance()->createChannelConfig(cpSAL);
#if defined(_WIN32_WCE)
	settings->setStringProperty(SALChannelConfigWin::NAME, "SALChannel");
#elif defined(QNX)
	// Nothing to configure
#else
	settings->setStringProperty(SALChannelConfigSocket::LOCAL_IPADDR, "127.0.0.1");
	settings->setStringProperty(SALChannelConfigSocket::REMOTE_IPADDR, "127.0.0.1");
#endif

	// Now create a channel to receive data
	SALChannel* pChannel = SALChannelFactory::getInstance()->createChannel ( 
		mAppID,		// this is our application ID
		appID,		// ID of the destination application, not used in this case
		"",			// Name of the channel 
		cpSAL, 		// type of the channel, e.g. serial, socket,...
		*settings, 
		CHANNEL_IO_READ
	);


	// Receive some data from the channel

	const int MAXLENGTH = 100;
	byte_t data[MAXLENGTH];
	word_t dataSize;
	pChannel->receive(data, &dataSize, MAXLENGTH, 10000);
	// Process is blocked until data is available, or a timeout occurs (10s)
}
@endcode
  * Transmitting data down the channel is a similar process.
  *
  * @section Queues
  * 
  * A SALMessageQueue:: is used to receive messages. An application can
  * create as many queues as required; each can be used to receive a
  * different type of message. Messages can be sent directly to a queue
  * within the same application by using SALAbstractQueue::postMessage().
  * If SALApplication:: is used, sal_post() or sal_publish() are normally
  * used to send messages either to queues within the current application,
  * or over a channel to a remote application. The queueID is used to
  * identify the destination application.
  * 
  * Queues are referred to via an ID. This ID is a 32-bit quantity, where
  * 16 bits define the application, and the other 16 bits define the queue
  * within the application. Application IDs can be assigned as desired,
  * except that 0 is reserved to mean "this application".
  * Normally they are assigned starting from one.
  * The Queue IDs are assigned in a similar way.
  * 
  * @code
	// Construct a queue with queueID of 1
	SALMessageQueue myQueue(1);

	// In another thread....
	// Create a message to send to queue 1
	SALMessage msg(EVENT_CODE, 1);
	// Set some data in the message
	msg.setStream("Hello", 5);
	msg.setDataLen(5);
	// Send it. The virtual queueID is unused at present.
	myQueue.postMessage(msg, 0);

	// Somewhere else in the code...
	// Retrieve message from queue
	SALMessage msg(false);
	myQueue.getMessage(msg);

  * @endcode
  * Note that in this example the queueIDs are unused, since the message is
  * sent directly to the queue.
  *
  * @section Using SALApplication
  *
  * The SALApplication:: framework makes sending and receiving messages
  * between different applications transparent, automatically creating a
  * channel to receive messages, and creates a channel to send messages
  * when necessary. The above example can be changed to use this mechanism
  * as follows:
  * @code
	// Create a message to send to remote application 3, queue 2
	SALMessage msg(EVENT_CODE, (3<<16)|2);
	// Set some data in the message
	msg.setStream("Hello", 5);
	msg.setDataLen(5);

	// Sending a message using mechanism provided by SALApplication
	// Note that the QueueID must be specified
	sal_post(msg, msg.getReceiver());

	// Create a message to broadcast to all queues listening for EVENT_CODE
	SALMessage msg(EVENT_CODE);
	// Set some data in the message
	msg.setStream("Hello", 5);
	msg.setDataLen(5);

	// Send a message to all queues on this application listening for EVENT_CODE
	sal_publish(msg, PUB_LOCAL);

	// Send a message to all queues on all applications listening for EVENT_CODE
	sal_publish(msg, PUB_GLOBAL);
  * @endcode
  */

#if defined(WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#else
#include <pthread.h>
#endif

/** @file libSAL.h
  * This file does not include any definitions or function prototypes, but
  * simply includes a list of commonly used SAL include files, so that the
  * application doesn't need to include them individually.
  *
  * The included files are:
  *   - @link Common/public/SALTypes.h @endlink
  *   - @link Common/public/SALAppIDs.h @endlink
  *   - @link Common/public/SALEventIDs.h @endlink
  *   - @link DebugTrace/public/SALDebugTrace.h @endlink
  *   - @link Common/public/SALCustomize.h @endlink
  *
  * If compiling with C++, the following are also included:
  *   - @link Common/public/SALApi.h @endlink
  */

#include "SALTypes.h"
//#include "SALAppIDs.h"
#include "SALEventIDs.h"

// Support for debug and trace messages
#include "SALDebugTrace.h"

// Customization (project and platform dependencies)
#include "SALCustomize.h"

#ifdef __cplusplus
//
#include "SALApi.h"

// unit test support
#include "SALUnitTest.h"

#endif /* __cplusplus */

#endif /*_LIBSAL_H_*/
