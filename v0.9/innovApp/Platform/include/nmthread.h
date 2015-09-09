#pragma once
#ifndef _NMTHREAD_H_
#define _NMTHREAD_H_

#include "appcore_platform.h"
#include "object.h"
#include "smart.h"
#include "workitem.h"
#include <queue>

namespace navman {

struct WorkerListElement
{
  IWorkItem *_pItem;
  UINT32 _time;

  WorkerListElement()
  {
  }

  WorkerListElement(const WorkerListElement& other)
  {
    _pItem = other._pItem;
    _time = other._time;
  }

  WorkerListElement& operator=(const WorkerListElement& rhs)
  {
    _pItem = rhs._pItem;
    _time = rhs._time;

    return *this;
  }

  bool operator <(const WorkerListElement& rhs)
  {
    // we want the priority queue sorted in reverse order (i.e. smallest to largest)
    return (INT32)(_time - rhs._time) > 0;
  }

};

inline bool
operator <(const WorkerListElement& lhs, const WorkerListElement& rhs)
{
    // we want the priority queue sorted in reverse order (i.e. smallest to largest)
    return (INT32)(lhs._time - rhs._time) > 0;
}

//
// creates a timer to be run on the current thread
//
extern bool scheduleWorkItem(IWorkItem *pItem, UINT32 delayMS);

//
// cancels a timer running on the current thread
//
extern bool cancelWorkItem( IWorkItem *pItem );

extern bool cancelAllTimers();

} // namespace navman

#endif // _NMTHREAD_H_
