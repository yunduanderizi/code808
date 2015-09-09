#pragma once
//
// $Id: smart.h 28795 2008-05-24 19:40:33Z tom.isaacson $
// 
// smart.h
// 
// simple classes to aid in resource management
//


#ifndef _SMART_H_
#define _SMART_H_

#include "object.h"

namespace navman {

/**
   deletes the underlying array based on scoping; used in non-Local context
*/
template <class _T>
class SmartArray
{
public:
  SmartArray(_T *p) : _p(p) {}
  SmartArray() : _p(NULL) {}

  operator _T*() const { return _p; }
  _T** operator&() { ASSERT(_p == NULL); return &_p; }
  _T& operator*() const { return *_p; }

  /**
     give up our reference to the pointer without deleting. so that when
     the SmartArray goes out of scope the underlying object won't be affected.
     @return the underlying pointer
  */
  _T* detach() {
    _T* temp = _p;
    _p = NULL;
    return temp;
  }

  ~SmartArray()
  {
    if (_p) {
      delete[] _p;
      _p = NULL;
    }
  }

  _T* operator =(_T *rhs)
  {
    if (_p != NULL) {
      delete[] _p;
    }

    _p = rhs;

    return _p;
  }

  _T* operator =(SmartArray<_T>& rhs)
  {
    if (_p != NULL) {
      delete[] _p;
    }

    _p = rhs.detach();

    return _p;
  }

protected:
  _T *_p;
};


/**
   deletes the underlying array based on scoping.
*/
template <class _T>
class LocalArray
{
public:
  LocalArray(_T *p) : _p(p) {}
  LocalArray() : _p(NULL) {}

  operator _T*() const { return _p; }
  _T** operator&() { ASSERT(_p == NULL); return &_p; }
  _T& operator*() const { return *_p; }

  /**
     give up our reference to the pointer without deleting. so that when
     the LocalArray goes out of scope the underlying object won't be affected.
     @return the underlying pointer
  */
  _T* detach() {
    _T* temp = _p;
    _p = NULL;
    return temp;
  }

  ~LocalArray()
  {
    if (_p) {
      delete[] _p;
      _p = NULL;
    }
  }

  _T* operator =(_T *rhs)
  {
    if (_p != NULL) {
      delete[] _p;
    }

    _p = rhs;

    return _p;
  }

  _T* operator =(LocalArray<_T>& rhs)
  {
    if (_p != NULL) {
      delete[] _p;
    }

    _p = rhs.detach();

    return _p;
  }

protected:
  _T *_p;
};


/**
   deletes the underlying object based on scoping.
*/
template <class _T>
class LocalObject
{
public:
  LocalObject(_T *p) : _p(p) {}
  LocalObject() : _p(NULL) {}

  operator _T*() const { return _p; }
  _T** operator&() { ASSERT(_p == NULL); return &_p; }
  _T& operator*() const { return *_p; }

  /**
     give up our reference to the pointer without deleting. so that when
     the LocalObject goes out of scope the underlying object won't be affected.
     @return the underlying pointer
  */
  _T* detach() {
    _T* temp = _p;
    _p = NULL;
    return temp;
  }

  ~LocalObject()
  {
    if (_p) {
      delete _p;
      _p = NULL;
    }
  }

  _T* operator =(_T *rhs)
  {
    if (_p != NULL) {
      delete _p;
    }

    _p = rhs;

    return _p;
  }

  _T* operator =(LocalObject<_T>& rhs)
  {
    if (_p != NULL) {
      delete _p;
    }

    _p = rhs.detach();

    return _p;
  }

  _T* operator ->() { return _p; }

protected:
  _T *_p;
};

} // namespace navman

#include "platform_smartptr.h"

#endif // _SMART_H_
