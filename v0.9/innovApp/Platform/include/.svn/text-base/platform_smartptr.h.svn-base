#pragma once
#ifndef _PLATFORM_SMARTPTR_H_
#define _PLATFORM_SMARTPTR_H_
#include "appcore_platform.h"

namespace navman {

//#ifdef _POSIX // No COM SmartPtr available on POSIX
// COM removed from WinCE, so use this for everything
#define USE_OWN_SMARTPTR
//#endif

#ifdef USE_OWN_SMARTPTR

template <class ObjectType>
class CObjectPtr {

public:
  ObjectType* p;
  
  CObjectPtr() {
    p = NULL;
  }
  
  CObjectPtr( const CObjectPtr& objPtr ) {
    p = objPtr.p;
    if (p != NULL) {
      p->AddRef();
    }
  }
  
  CObjectPtr( ObjectType* ptr ) {
    p = ptr;
    if (p != NULL) {
      p->AddRef();
    }
  }
  
  ~CObjectPtr() {
    if (p != NULL) {
      p->Release();
      p = NULL;
    }
  }
  
  CObjectPtr& operator=( const CObjectPtr& objPtr ) {
    ObjectType* ptr = objPtr.p;
    if (ptr != NULL) {
      ptr->AddRef();
    }
    if (p != NULL) {
      p->Release();
    }
    p = ptr;
    return (*this);
  }
  
  CObjectPtr& operator=( ObjectType* ptr ) {
    if (ptr != NULL) {
      ptr->AddRef();
    }
    if (p != NULL) {
      p->Release();
    }
    p = ptr;
    return (*this);
  }
  
  operator ObjectType*() const {
    return (p);
  }
  
  ObjectType* operator->() const {
    return (p);
  }
  
  ObjectType** operator&() {
    ASSERT(p == NULL);
    return (&p);
  }
  
  ObjectType* Detach() {
    ObjectType* ptr = p;
    p = NULL;
    return (ptr);
  }

  void Attach(ObjectType *ptr) {
    if (p != NULL) {
      p->Release();
    }
    p = ptr;
  }
};


#define SMART_PTR_PARENT CObjectPtr
#else
#define SMART_PTR_PARENT CComPtr
#endif // USE_OWN_SMARTPTR

template <class _PType, class _Cast = _PType>
class SmartPtr : public SMART_PTR_PARENT<_Cast>
{
public:
  SmartPtr() : SMART_PTR_PARENT<_Cast>() {}
  SmartPtr(_PType *ptr) : SMART_PTR_PARENT<_Cast>(ptr) {}

  _PType *detach()
  {
    return static_cast<_PType *>(SMART_PTR_PARENT<_Cast>::Detach());
  }

  _PType* operator->() const
  {
    return static_cast<_PType *>(SMART_PTR_PARENT<_Cast>::p);
  }

  _PType* ptr() const
  {
      return static_cast<_PType *>(SMART_PTR_PARENT<_Cast>::p);
  }

};

// Use this template when creating STL containers of SmartPtrs.
// e.g. use vector<SuperSmartPtr<object> > instead of
//          vector<SmartPtr<object> >
// This is because STL uses operator& which does the wrong thing
template<class T>
    class SuperSmartPtr : public SmartPtr<T>
{
  public:
    SuperSmartPtr() : SmartPtr<T>() {}
    SuperSmartPtr(T* ptr) : SmartPtr<T>(ptr) {}
    SuperSmartPtr(SmartPtr<T>& t) : SmartPtr<T>(t) {}
    SuperSmartPtr* operator&() {
      return this;
    }
};

}

#endif // _PLATFORM_SMARTPTR_H_
