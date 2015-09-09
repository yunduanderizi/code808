#pragma once
#ifndef _APPNOTIFY_H_
#define _APPNOTIFY_H_

#include "object.h"

namespace navman {

/**
   Interface exposed by an object passed to Appcore::create.  The appcore object
   uses this to tell the application that there are outstanding events that need to
   be dispatched.
*/
class IAppNotify : public IObject
{
public:
  /**
     called when there are outstanding dispatches.  On receiving this event
     the application should call IAppcore::dispatch at the next convenient time.
  */
  virtual bool notifyDispatchesPending() = 0;

  /**
     Get context to handle dispatched notifications.
     @return pointer to contextual data for notify object.
  */
  virtual void* getContext() = 0;
};

} // namespace navman

#endif // _APPNOTIFY_H_
