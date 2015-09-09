#pragma once
#ifndef _EVENTSOURCEMULTICASTER_H_
#define _EVENTSOURCEMULTICASTER_H_

#include "object.h"
#include "workitem.h"
#include "appcore.h"
#include "nmthread.h"

#include <list>

namespace navman
{

//
// classes for wrapping listener callbacks in workitems
//

template <class _Listener>
class ListenerWorkItem : public IWorkItem
{
public:
  NM_BEGIN_INTERFACE_MAP(ListenerWorkItem)
    NM_INTERFACE_MAP_ENTRY(IWorkItem)
  NM_END_INTERFACE_MAP

  ListenerWorkItem() {}
  ~ListenerWorkItem() { _pListener->Release(); }

  //
  // IWorkItem methods
  //
  virtual void run()
  {
    (_pListener->*_func)();
  }
  
  _Listener *_pListener;
  void (_Listener::*_func)();

  static bool create(_Listener *pListener, void (_Listener::*func)(), ListenerWorkItem<_Listener> **ppObj)
  {
    SmartPtr<ObjectImpl<ListenerWorkItem<_Listener> > > pItem =
      new ObjectImpl<ListenerWorkItem<_Listener> >;
    pItem->_pListener = pListener;
    pListener->AddRef();
    pItem->_func = func;

    *ppObj = pItem.detach();
    return true;
  }
};

template <class _Listener, class _A1>
class ListenerWorkItem1 : public IWorkItem
{
public:
  NM_BEGIN_INTERFACE_MAP(ListenerWorkItem1)
    NM_INTERFACE_MAP_ENTRY(IWorkItem)
  NM_END_INTERFACE_MAP

  ListenerWorkItem1() {}
  ~ListenerWorkItem1()
  {
    _pListener->Release();
    handleRelease(_a1);
  }

  //
  // don't addref arguments that don't inherit from IObject
  //
  template<class _A1m>
  static void handleAddRef(const _A1m& a)
  {
  }

  //
  // addref arguments that inherit from IObject
  //
  template<IObject *>
  static void handleAddRef(IObject *a)
  {
    a->AddRef();
  }

  //
  // don't release arguments that don't inherit from IObject
  //
  template<class _A1m>
  static void handleRelease(const _A1m& a)
  {
  }

  //
  // release objects that inherit from IObject
  //
  template<IObject *>
  static void handleRelease(IObject *a)
  {
    a->Release();
  }

  //
  // IWorkItem methods
  //
  virtual void run()
  {
    (_pListener->*_func)(_a1);
  }
  
  _Listener *_pListener;
  void (_Listener::*_func)(_A1);
  _A1 _a1;

  static bool create(_Listener *pListener,
                     void (_Listener::*func)(_A1),
                     _A1 a1,
                     ListenerWorkItem1<_Listener, _A1> **ppObj)
  {
    SmartPtr<ObjectImpl<ListenerWorkItem1<_Listener, _A1> > > pItem =
      new ObjectImpl<ListenerWorkItem1<_Listener, _A1> >;
    pItem->_pListener = pListener;
    pListener->AddRef();
    pItem->_func = func;
    pItem->_a1 = a1;
    handleAddRef(a1);

    *ppObj = pItem.detach();
    return true;
  }

};

template <class _Listener, class _A1, class _A2>
class ListenerWorkItem2 : public IWorkItem
{
public:
  NM_BEGIN_INTERFACE_MAP(ListenerWorkItem2)
    NM_INTERFACE_MAP_ENTRY(IWorkItem)
  NM_END_INTERFACE_MAP

  ListenerWorkItem2() {}
  ~ListenerWorkItem2()
  {
    if (_pListener != NULL) {
      _pListener->Release();
    }

    handleRelease(_a1);
    handleRelease(_a2);
  }

  //
  // don't addref arguments that don't inherit from IObject
  //
  template<class _A1m>
  static void handleAddRef(const _A1m& a)
  {
  }

  //
  // addref arguments that inherit from IObject
  //
  template<IObject *>
  static void handleAddRef(IObject *a)
  {
    a->AddRef();
  }

  //
  // don't release arguments that don't inherit from IObject
  //
  template<class _A1m>
  static void handleRelease(const _A1m& a)
  {
  }

  //
  // release objects that inherit from IObject
  //
  template<IObject *>
  static void handleRelease(IObject *a)
  {
    a->Release();
  }

  //
  // IWorkItem methods
  //
  virtual void run()
  {
    (_pListener->*_func)(_a1, _a2);
  }
  
  _Listener *_pListener;
  void (_Listener::*_func)(_A1, _A2);
  _A1 _a1;
  _A2 _a2;

  static bool create(_Listener *pListener,
                     void (_Listener::*func)(_A1, _A2),
                     _A1 a1,
                     _A2 a2,
                     ListenerWorkItem2<_Listener, _A1, _A2> **ppObj)
  {
    SmartPtr<ObjectImpl<ListenerWorkItem2<_Listener, _A1, _A2> > > pItem =
      new ObjectImpl<ListenerWorkItem2<_Listener, _A1, _A2> >;
    pItem->_pListener = pListener;
    pListener->AddRef();
    pItem->_func = func;
    pItem->_a1 = a1;
    handleAddRef(a1);
    pItem->_a2 = a2;
    handleAddRef(a2);

    *ppObj = pItem.detach();
    return true;
  }
};


template <class _Listener, class _A1, class _A2, class _A3>
class ListenerWorkItem3 : public IWorkItem
{
public:
  NM_BEGIN_INTERFACE_MAP(ListenerWorkItem3)
    NM_INTERFACE_MAP_ENTRY(IWorkItem)
  NM_END_INTERFACE_MAP

  ListenerWorkItem3() {}
  ~ListenerWorkItem3()
  {
    if (_pListener != NULL) {
      _pListener->Release();
    }

    handleRelease(_a1);
    handleRelease(_a2);
    handleRelease(_a3);
  }

  //
  // don't addref arguments that don't inherit from IObject
  //
  template<class _A1m>
  static void handleAddRef(const _A1m& a)
  {
  }

  //
  // addref arguments that inherit from IObject
  //
  template<IObject *>
  static void handleAddRef(IObject *a)
  {
    a->AddRef();
  }

  //
  // don't release arguments that don't inherit from IObject
  //
  template<class _A1m>
  static void handleRelease(const _A1m& a)
  {
  }

  //
  // release objects that inherit from IObject
  //
  template<IObject *>
  static void handleRelease(IObject *a)
  {
    a->Release();
  }

  //
  // IWorkItem methods
  //
  virtual void run()
  {
    (_pListener->*_func)(_a1, _a2, _a3);
  }
  
  _Listener *_pListener;
  void (_Listener::*_func)(_A1, _A2, _A3);
  _A1 _a1;
  _A2 _a2;
  _A3 _a3;

  static bool create(_Listener *pListener,
                     void (_Listener::*func)(_A1, _A2, _A3),
                     _A1 a1,
                     _A2 a2,
                     _A3 a3,
                     ListenerWorkItem3<_Listener, _A1, _A2, _A3> **ppObj)
  {
    SmartPtr<ObjectImpl<ListenerWorkItem3<_Listener, _A1, _A2, _A3> > > pItem =
      new ObjectImpl<ListenerWorkItem3<_Listener, _A1, _A2, _A3> >;
    pItem->_pListener = pListener;
    pListener->AddRef();
    pItem->_func = func;
    pItem->_a1 = a1;
    handleAddRef(a1);
    pItem->_a2 = a2;
    handleAddRef(a2);
    pItem->_a3 = a3;
    handleAddRef(a3);

    *ppObj = pItem.detach();
    return true;
  }
};



template <class _Listener, class _A1, class _A2, class _A3, class _A4>
class ListenerWorkItem4 : public IWorkItem
{
public:
  NM_BEGIN_INTERFACE_MAP(ListenerWorkItem4)
    NM_INTERFACE_MAP_ENTRY(IWorkItem)
  NM_END_INTERFACE_MAP

  ListenerWorkItem4() {}
  ~ListenerWorkItem4()
  {
    if (_pListener != NULL) {
      _pListener->Release();
    }

    handleRelease(_a1);
    handleRelease(_a2);
    handleRelease(_a3);
    handleRelease(_a4);
  }

  //
  // don't addref arguments that don't inherit from IObject
  //
  template<class _A1m>
  static void handleAddRef(const _A1m& a)
  {
  }

  //
  // addref arguments that inherit from IObject
  //
  template<IObject *>
  static void handleAddRef(IObject *a)
  {
    a->AddRef();
  }

  //
  // don't release arguments that don't inherit from IObject
  //
  template<class _A1m>
  static void handleRelease(const _A1m& a)
  {
  }

  //
  // release objects that inherit from IObject
  //
  template<IObject *>
  static void handleRelease(IObject *a)
  {
    a->Release();
  }

  //
  // IWorkItem methods
  //
  virtual void run()
  {
    (_pListener->*_func)(_a1, _a2, _a3, _a4);
  }
  
  _Listener *_pListener;
  void (_Listener::*_func)(_A1, _A2, _A3, _A4);
  _A1 _a1;
  _A2 _a2;
  _A3 _a3;
  _A4 _a4;

  static bool create(_Listener *pListener,
                     void (_Listener::*func)(_A1, _A2, _A3, _A4),
                     _A1 a1,
                     _A2 a2,
                     _A3 a3,
                     _A4 a4,
                     ListenerWorkItem4<_Listener, _A1, _A2, _A3, _A4> **ppObj)
  {
    SmartPtr<ObjectImpl<ListenerWorkItem4<_Listener, _A1, _A2, _A3, _A4> > > pItem =
      new ObjectImpl<ListenerWorkItem4<_Listener, _A1, _A2, _A3, _A4> >;
    pItem->_pListener = pListener;
    pListener->AddRef();
    pItem->_func = func;
    pItem->_a1 = a1;
    handleAddRef(a1);
    pItem->_a2 = a2;
    handleAddRef(a2);
    pItem->_a3 = a3;
    handleAddRef(a3);
    pItem->_a4 = a4;
    handleAddRef(a4);

    *ppObj = pItem.detach();
    return true;
  }
};


//
// Impl class for event sources.  This implements the methods to broadcast listener
// callbacks as events to the application thread and to the appcore thread
//
template <class _Listener>
class EventSourceMulticaster
{
public:
  ~EventSourceMulticaster()
  {
    typename std::list<_Listener *>::iterator iter;
    for (iter = _listeners.begin(); iter != _listeners.end(); iter++) {
      _Listener *pListener = *iter;
      ASSERT(pListener != NULL);
      pListener->Release();
    }
  }

  typedef std::pair<_Listener *, UINT32> ListenerPair;

  /** 
    Find out how many listeners this object has.
    @return Number of listeners.
  */
  UINT32 numListeners() 
  {
    return _listeners.size();
  }

  /** Add a listener to the event source list.  This listener will be
      called whenever the event is fired.
        
      @param pListener Pointer to object that implements the listening
      interface for this event.
  */
  bool
  addListenerInternal(_Listener* pListener)
  {
    pListener->AddRef();
    _listeners.push_back(pListener);

    return true;
  }
    
  /** Remove a listener from this event source caster list.  This
      listner will not be called on future event broadcasts.
     
      @param pListener Pointer to listener function to remove.
  */
  bool
  removeListenerInternal(_Listener* pListener)
  {
    if (pListener == NULL) {
      NM_LOG_DEBUG(L"listener not found, not removed");
      return false;
    }
    
    for (typename std::list<_Listener *>::iterator i = _listeners.begin();
         i != _listeners.end();
         i++) {
      if (*i == pListener) {
        _listeners.erase(i);
        pListener->Release();
        return true;
      }
    }

    return false;
  }
  
  /**
     broadcastCoreEvent variations allow the caller to call listeners on
     the thread that they were added from (i.e. the thread that called addListener)
     @param func the function to call
  */

  void
  broadcastEvent(IAppcore *pCore, void (_Listener::*func)())
  {
    typedef ListenerWorkItem<_Listener> LWI;
    for (typename std::list<_Listener *>::iterator iter = _listeners.begin();
         iter != _listeners.end();
         iter++) {

      _Listener *pListener = *iter;

      SmartPtr<LWI> pItem;
      LWI::create(pListener, func, &pItem);

      pCore->addAppItem(pItem.p);
    }
  }
    
  void
  sendEvent(void (_Listener::*func)())
  {
    typename std::list<_Listener *>::iterator iter = _listeners.begin();
    while (iter != _listeners.end()) {
      _Listener *pListener = *iter;
      ++iter;
      (pListener->*func)();
    }
  }

  template <class _A1>
  void
  sendEvent1(void (_Listener::*func)(_A1), _A1 a1)
  {
    typename std::list<_Listener *>::iterator iter = _listeners.begin();
    while (iter != _listeners.end()) {
      _Listener *pListener = *iter;
      ++iter;
      (pListener->*func)(a1);
    }
  }

  template <class _A1, class _A2>
  void
  sendEvent2(void (_Listener::*func)(_A1, _A2), _A1 a1, _A2 a2)
  {
    typename std::list<_Listener *>::iterator iter = _listeners.begin();
    while (iter != _listeners.end()) {
      _Listener *pListener = *iter;
      ++iter;
      (pListener->*func)(a1, a2);
    }
  }

  template <class _A1, class _A2, class _A3>
  void
  sendEvent3(void (_Listener::*func)(_A1, _A2, _A3), _A1 a1, _A2 a2, _A3 a3)
  {
    typename std::list<_Listener *>::iterator iter = _listeners.begin();
    while (iter != _listeners.end()) {
      _Listener *pListener = *iter;
      ++iter;
      (pListener->*func)(a1, a2, a3);
    }
  }

  template <class _A1, class _A2, class _A3, class _A4>
  void
  sendEvent4(void (_Listener::*func)(_A1, _A2, _A3, _A4), _A1 a1, _A2 a2, _A3 a3, _A4 a4)
  {
    typename std::list<_Listener *>::iterator iter = _listeners.begin();
    while (iter != _listeners.end()) {
      _Listener *pListener = *iter;
      ++iter;
      (pListener->*func)(a1, a2, a3, a4);
    }
  }

  /**
     broadcastCoreEvent variations allow the caller to call listeners on
     the application's main thread
     @param func the function to call
     @param a1 first argument to pass to func
  */
  template<class _A1>
  void
  broadcastEvent1(IAppcore *pCore, void (_Listener::*func)(_A1), _A1 a1)
  {
    typedef ListenerWorkItem1<_Listener, _A1> LWI1;
    for (typename std::list<_Listener *>::iterator iter = _listeners.begin();
         iter != _listeners.end();
         iter++) {

      _Listener *pListener = *iter;

      SmartPtr<LWI1> pItem;
      LWI1::create(pListener, func, a1, &pItem);
      pCore->addAppItem(pItem.p);
    }
  }

  /**
     broadcastCoreEvent variations allow the caller to call listeners on
     the application's main thread
     @param func the function to call
     @param a1 first argument to pass to func
     @param a2 second argument to pass to func
  */
  template<class _A1, class _A2>
  void
  broadcastEvent2(IAppcore *pCore, void (_Listener::*func)(_A1, _A2), _A1 a1, _A2 a2)
  {
    typedef ListenerWorkItem2<_Listener, _A1, _A2> LWI2;

    for (typename std::list<_Listener *>::iterator iter = _listeners.begin();
         iter != _listeners.end();
         iter++) {
      _Listener *pListener = *iter;

      SmartPtr<LWI2> pItem;
      LWI2::create(pListener, func, a1, a2, &pItem);

      pCore->addAppItem(pItem.p);
    }
  }
 

  /**
     broadcastCoreEvent variations allow the caller to call listeners on
     the application's main thread
     @param func the function to call
     @param a1 first argument to pass to func
     @param a2 second argument to pass to func
     @param a3 third argument to pass to func
  */
  template<class _A1, class _A2, class _A3>
  void
  broadcastEvent3(IAppcore *pCore, void (_Listener::*func)(_A1, _A2, _A3), _A1 a1, _A2 a2, _A3 a3)
  {
    typedef ListenerWorkItem3<_Listener, _A1, _A2, _A3> LWI3;

    for (typename std::list<_Listener *>::iterator iter = _listeners.begin();
         iter != _listeners.end();
         iter++) {
      _Listener *pListener = *iter;

      SmartPtr<LWI3> pItem;
      LWI3::create(pListener, func, a1, a2, a3, &pItem);

      pCore->addAppItem(pItem.p);
    }
  }

  template<class _A1, class _A2, class _A3, class _A4>
  void
  broadcastEvent4(IAppcore *pCore, void (_Listener::*func)(_A1, _A2, _A3, _A4), _A1 a1, _A2 a2, _A3 a3, _A4 a4)
  {
    typedef ListenerWorkItem4<_Listener, _A1, _A2, _A3, _A4> LWI4;

    for (typename std::list<_Listener *>::iterator iter = _listeners.begin();
         iter != _listeners.end();
         iter++) {
      _Listener *pListener = *iter;

      SmartPtr<LWI4> pItem;
      LWI4::create(pListener, func, a1, a2, a3, a4, &pItem);

      pCore->addAppItem(pItem.p);
    }
  }

  /**
     sends an event (synchronously) to a single target
     @param func the function to call
     @param a1 first argument to pass to func
     @param a2 second argument to pass to func
  */
  template<class _A1, class _A2>
  void
  sendEventToTarget2(_Listener *pListener, void (_Listener::*func)(_A1, _A2), _A1 a1, _A2 a2)
  {
    typedef ListenerWorkItem2<_Listener, _A1, _A2> LWI2;
    (pListener->*func)(a1, a2);
  }

private:
  std::list<_Listener *> _listeners;
};

} // namespace

#endif  // _EVENTSOURCEMULTICASTER_H_
