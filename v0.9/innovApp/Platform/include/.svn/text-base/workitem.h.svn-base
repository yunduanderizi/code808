#pragma once
#ifndef _WORKITEM_H_
#define _WORKITEM_H_

#include <smart.h>
#include "object.h"

namespace navman {

/**
   Work item interface.  Exported by objects that want to be dispatched by the 
   application's appcore object (i.e. when run IAppcore::dispatch() is invoked).
*/
class IWorkItem : public IObject
{
public:
  /**
     execute this work item
  */
  virtual void run() = 0;
};


/**
Functor class used to wrap a function with no arguments in an WorkItem object so it 
can be dispatched by the  application's appcore object (i.e. when run IAppcore::dispatch() is invoked).
@param _Listener class to which function that will be called belongs to
@param _Ret type of the return value of the function
@param _Cast type to cast the listener to when AddRef'ing.  Defaults to _Listener
*/
template <class _Listener, class _Ret, class _Cast = _Listener>
class Functor : public IWorkItem
{
public:
  NM_BEGIN_INTERFACE_MAP(Functor)
    NM_INTERFACE_MAP_ENTRY(IWorkItem)
    NM_END_INTERFACE_MAP

    Functor() {}
    ~Functor()
    {
      ((_Cast*)_pListener)->Release();
    }

    //
    // IWorkItem methods
    //
    virtual void run()
    {
      (_pListener->*_func)();
    }

    _Listener *_pListener;
    _Ret (_Listener::*_func)();

    static bool create(_Listener *pListener,
      _Ret (_Listener::*func)(),
      Functor<_Listener, _Ret, _Cast> **ppObj)
    {
      SmartPtr<ObjectImpl<Functor<_Listener, _Ret, _Cast> > > pItem =
        new ObjectImpl<Functor<_Listener, _Ret, _Cast> >;
      pItem->_pListener = pListener;
      ((_Cast*)pListener)->AddRef();
      pItem->_func = func;

      *ppObj = pItem.detach();
      return true;
    }
};


/**
  Functor class used to wrap a function with one argument in an WorkItem object so it 
  can be dispatched by the  application's appcore object (i.e. when run IAppcore::dispatch() is invoked).
   @param _Listener class to which function that will be called belongs to
   @param _Ret type of the return value of the function
   @param _A1 type of the first (and only) parameter of the function
   @param _Cast type to cast the listener to when AddRef'ing.  Defaults to _Listener
*/
template <class _Listener, class _Ret, class _A1, class _Cast = _Listener>
class Functor1 : public IWorkItem
{
public:

  NM_BEGIN_INTERFACE_MAP(Functor1)
    NM_INTERFACE_MAP_ENTRY(IWorkItem)
  NM_END_INTERFACE_MAP

    Functor1() {}
    ~Functor1()
    {
      ((_Cast*)_pListener)->Release();
    }

    //
    // IWorkItem methods
    //
    virtual void run()
    {
      (_pListener->*_func)(_a1);
    }

    _Listener *_pListener;
    _Ret (_Listener::*_func)(_A1);
    _A1 _a1;

    static bool create(_Listener *pListener,
      _Ret (_Listener::*func)(_A1),
      _A1 a1,
      Functor1<_Listener, _Ret, _A1, _Cast> **ppObj)
    {
      SmartPtr<ObjectImpl<Functor1<_Listener, _Ret, _A1, _Cast> > > pItem =
        new ObjectImpl<Functor1<_Listener, _Ret, _A1, _Cast> >;
      pItem->_pListener = pListener;
      ((_Cast*)pListener)->AddRef();
      pItem->_func = func;
      pItem->_a1 = a1;

      *ppObj = pItem.detach();
      return true;
    }
};


/**
  Functor class used to wrap a function with two arguments in an WorkItem object so it 
  can be dispatched by the  application's appcore object (i.e. when run IAppcore::dispatch() is invoked).
   @param _Listener class to which function that will be called belongs to
   @param _Ret type of the return value of the function
   @param _A1 type of the first parameter of the function
   @param _A2 type of the second parameter of the function
   @param _Cast type to cast the listener to when AddRef'ing.  Defaults to _Listener
*/
template <class _Listener, class _Ret, class _A1, class _A2, class _Cast = _Listener>
class Functor2 : public IWorkItem
{
public:

  NM_BEGIN_INTERFACE_MAP(Functor2)
    NM_INTERFACE_MAP_ENTRY(IWorkItem)
  NM_END_INTERFACE_MAP

    Functor2() {}
    ~Functor2()
    {
      ((_Cast*)_pListener)->Release();
    }

    //
    // IWorkItem methods
    //
    virtual void run()
    {
      (_pListener->*_func)(_a1, _a2);
    }

    _Listener *_pListener;
    _Ret (_Listener::*_func)(_A1, _A2);
    _A1 _a1;
    _A2 _a2;

    static bool create(_Listener *pListener,
      _Ret (_Listener::*func)(_A1, _A2),
      _A1 a1,
      _A2 a2,
      Functor2<_Listener, _Ret, _A1, _A2, _Cast> **ppObj)
    {
      SmartPtr<ObjectImpl<Functor2<_Listener, _Ret, _A1, _A2, _Cast> > > pItem =
        new ObjectImpl<Functor2<_Listener, _Ret, _A1, _A2, _Cast> >;
      pItem->_pListener = pListener;
      ((_Cast*)pListener)->AddRef();
      pItem->_func = func;
      pItem->_a1 = a1;
      pItem->_a2 = a2;

      *ppObj = pItem.detach();
      return true;
    }
};

} // namespace navman

#endif // _WORKITEM_H_
