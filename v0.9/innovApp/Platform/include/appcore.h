#pragma once
#ifndef _APPCORE_H_
#define _APPCORE_H_

#include "object.h"

namespace navman {

class IWorkItem;
class IAppNotify;
class ILocationEventSource;
class ILiveLocation;

/**
   Appcore interface.  There should be only 1 appcore object in the system and
   it will expose the interface which is consumed by various other components
   and used for some base functionality like event broadcasting and receiving,
   and settings management.
*/
class IAppcore : public IObject
{
public:

  /**
     Setup the appcore structures, start the thread, etc
     @return true on success, false on failure
  */
  virtual bool start() = 0;

  /**
     Stop the appcore thread, cleanup any structures, etc
     @return true on success, false on failure
  */
  virtual bool cleanUp() = 0;

  /**
     Add a work item to the appcore thread
     @param [in] pItem pointer to item to add
     @param [in] delayMS number of milliseconds to delay before executing the item,
                         0 indicates no delay
     @return true on success, false on failure
  */
  virtual bool addCoreItem(IWorkItem *pItem, UINT32 delayMs = 0) = 0;

  /**
     Dispatch any pending applicaton event callbacks.  This is meant to be run on the application's
     main (winmain) thread
  */
  virtual bool dispatch() = 0;

  /**
     Add an item to the external dispatch queue (i.e. to the list of items run
     by dispatch();
     @param pDispatch the item to add
     @param delayMs number of milliseconds to delay before executing the item,
                    0 indicates no delay
     @return true on success, false otherwise
  */
  virtual bool addAppItem(IWorkItem *pDispatch, UINT32 delayMs = 0) = 0;

  /**
     remove an item from the dispatch queue.
     if the item is a timer, then kill the timer.
     @param pItem pointer the the workitem to remove
     @return true on success, false otherwise
  */
  virtual bool cancelAppItem( IWorkItem *pItem ) = 0;

  /**
     cancel a timer on the appcore thread and manually run it.
     @param pItem the workitem to cancel and run
     @return true on success, false otherwise
  */
  virtual bool manualWorkItemRun( IWorkItem *pItem ) = 0;

  /**
     Gets the id of the appcore thread
     @return identifier of the thread, only valid after start() has been called.
  */
  virtual UINT32 getThreadId() = 0;
};

/**
   Appcore object.  There should be only when present in the system at any time.
*/
class Appcore : public IAppcore
{
public:
  /**
     Factory function to create an appcore object.  There should only be one instance
     per application.

     @param pNotify : pointer to an object exposing the IAppNotify interface
                      which the appcore object will use to communicate that appcore
                      events need to be dispatched
     @param ppObj new appcore object is created here
     @return true on success, false on failure
  */
  static bool create(IAppNotify *pNotify, Appcore **ppObj);
};

} // namespace navman

#endif // _APPCORE_H_
