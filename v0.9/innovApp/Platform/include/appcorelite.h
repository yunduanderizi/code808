#pragma once
/** @file 
  The AppcoreLite class provides a test version of the AppCore class that doesn't use the OS for timers or 
  dispatching (c) 2005 navman new zealand limited. all rights reserved.
*/
#ifndef _APPCORELITE_H_
#define _APPCORELITE_H_

#include "object.h"
#include "appcore.h"

namespace navman {

class IWorkItem;
class IAppNotify;
class ILocationEventSource;
class ILiveLocation;

/**
  The AppcoreLite class provides a test version of the Appcore class that doesn't use the OS for timers or
  dispatching.  It exposes the same IAppcore interface, but uses an internal queue for scheduling.  
  The class is intended to provide a way to unit test object that need to schedule work items or timers
  on an Appcore object, without having to run a full Appcore object and a windows event queue. 
  Using AppcoreLite to test at more than unit or basic integration level is not recommended.
  Work items and timers are added to the internal queue in order of their timeout values, and as a result 
  timers are even more approximate than usual.
  Note the class is also non-thread safe.
  
*/
class AppcoreLite : public IAppcore
{
public:
  /**
     Factory function to create an AppcoreLite object.  
     @param pNotify : pointer to an object exposing the IAppNotify interface
                      which the appcore object will use to communicate that appcore
                      events need to be dispatched
     @param ppObj new AppcoreLite object is created here
     @return true on success, false on failure
  */
  static bool create(IAppNotify *pNotify, AppcoreLite **ppObj);

  /**
     Factory function to create an AppcoreLite object and return it as an IAppCore pointer.
     @param pNotify : pointer to an object exposing the IAppNotify interface
                      which the appcore object will use to communicate that appcore
                      events need to be dispatched
     @param ppObj new AppcoreLite object is created here
     @return true on success, false on failure
  */
  static bool create(IAppNotify *pNotify, IAppcore **ppObj);
};

} // namespace navman

#endif // _APPCORELITE_H_
