#pragma once
#ifndef _OBJECT_H_
#define _OBJECT_H_

//----------------------------------------------------------------------------
// INCLUDES
//----------------------------------------------------------------------------

#include "frameworklib.h"

//#define OBJECT_COUNTERS
#ifdef OBJECT_COUNTERS
#  include "watch_value.h"
#endif // OBJECT_COUNTERS

namespace navman
{

/**
   Base interface for all other interfaces in the system.
*/
class IObject : public PlatformBaseInterface
{
public:
  /**
     increment the reference count of the underlying object
     @return new reference count.  A value less than 0 indicates an underrun
                                   error.
  */
  virtual ULONG PLATCALL AddRef() = 0;

  /**
     decrement the reference count of the underlying object.  When it reaches
     0, the object is deleted.
     @return the new reference count. A value less than 0 indicates an underrun error.
  */
  virtual ULONG PLATCALL Release() = 0;

  /**
    determine whether this object supports the specified interface
    @param riid GUID of interface which is being requested
    @param ppv pointer to receive the AddRef'd object if it supports the interface
    @param S_OK on success, E_NOINTERFACE otherwise.
  */
  virtual HRESULT PLATCALL QueryInterface(REFIID riid, void **ppv) = 0;
  
  /**
    Make the destructor virtual. This ensures all derived classes will have
    virtual destructors, which eliminates a certain class of errors (not to
    mention compiler warnings).
  */
  virtual ~IObject() {}
};

/**
   Multithreaded implementation of functions.  Used by the ObjectImpl template
   as an option for its thread policy.
*/
class MultiThread
{
public:
  typedef INT32 RefCountType;

  /**
     Increments the value in a thread-safe way
     @param pointer to the value to increment
     @return the new value
  */
  static RefCountType
  Increment(RefCountType *pRefCount)
  {
    return InterlockedIncrement((long *)pRefCount);
  }

  /**
     Decrements the value in a thread-safe way
     @param pointer to the value to decrement
     @return the new value
  */
  static RefCountType
  Decrement(RefCountType *pRefCount)
  {
    return InterlockedDecrement((long *)pRefCount);
  }
};

/**
   Single threaded function implementations.  Used by the ObjectImpl template as
   an option for its multithreading policy.
*/
class SingleThread
{
public:
  typedef INT16 RefCountType;

  /**
     Increments the value in an efficient, non-threadsafe manner.
     @param pRefCount pointer to the value to increment
     @return new value
  */
  static RefCountType Increment(RefCountType *pRefCount)
  {
    return ++(*pRefCount);
  }

  /**
     Decrements the value in an efficient, non-threadsafe manner.
     @param pRefCount pointer to the value to decrement
     @return new value
  */
  static RefCountType Decrement(RefCountType *pRefCount)
  {
    return --(*pRefCount);
  }
};

#ifdef OBJECT_COUNTERS
///
/// Watch value to count how much space the base displayable object uses
///
class ObjectImplWatchValue : public watch_value<long> 
{
public:
  ~ObjectImplWatchValue()
  {
    NM_LOG_INFO(L"**** ObjectImpl Mem: min=%d, max=%d, value=%d, mean=%d, accesses=%d",
        minval(), maxval(), value(), mean(),  access_count() );
  }
};

///
/// Watch value to count how much space the base displayable object uses
///
class ObjectImplCount : public watch_value<long> 
{
public:
  ~ObjectImplCount()
  {
    NM_LOG_INFO(L"**** ObjectImpl Count: min=%d, max=%d, value=%d, mean=%d, accesses=%d",
        minval(), maxval(), value(), mean(),  access_count() );
  }
};

///
/// We need to define the storage for these somewhere so we put it in appcore.cpp
///
extern ObjectImplWatchValue g_ObjectImplMem;
extern ObjectImplCount g_ObjectImplCount;
#endif // OBJECT_COUNTERS

/**
   Template class used when an object is created to add bodies for
   the methods required to be implemented by the IObject interface.
   @param _base the class that is being created and that the method
                implementations should be added to
   @param _ThreadModel threading policy used to determine how refcounting
                       and other functions should be handled
*/
template <class _base, class _ThreadModel = SingleThread>
class ObjectImpl : public _base
{
public:

  ObjectImpl() : _nRefCount(0)
  {
#ifdef OBJECT_COUNTERS
    g_ObjectImplMem += sizeof(*this);
    g_ObjectImplCount += 1;
#endif // OBJECT_COUNTERS
  }

  ~ObjectImpl()
  {
#ifdef OBJECT_COUNTERS
    g_ObjectImplMem -= sizeof(*this);
    g_ObjectImplCount -= 1;
#endif // OBJECT_COUNTERS
  }

  ULONG PLATCALL AddRef()
  {
    INT32 ref = _ThreadModel::Increment(&_nRefCount);
    ASSERT(ref > 0);

    return (ULONG)ref;
  }

  ULONG PLATCALL Release()
  {
    INT32 t = _ThreadModel::Decrement(&_nRefCount);
    ASSERT(t >= 0);

    if (t == 0) {
      delete this;
    }

    return (ULONG)t;
  }

  operator _base*() const { return (_base *)this; }

private:
  typename _ThreadModel::RefCountType _nRefCount;
};

#define QI_UNSUPPORTED \
  HRESULT PLATCALL QueryInterface(REFIID, void **) { \
    return E_NOINTERFACE; \
  }

#define NM_BEGIN_INTERFACE_MAP(_ifacecast)          \
  HRESULT PLATCALL QueryInterface(REFIID riid, void **ppv) { \
    PARAMCHECKRET(ppv != NULL, E_INVALIDARG); \
    PARAMCHECKRET(*ppv == NULL, E_INVALIDARG); \
    if (IsEqualIID(riid, IID_IObject)) { \
      *ppv = static_cast<IObject *>(static_cast<_ifacecast *>(this));     \
      (*(_ifacecast **)ppv)->AddRef(); \
      return 0; \
    }

#define NM_INTERFACE_MAP_ENTRY(_iface) \
    if (IsEqualIID(riid, IID_ ## _iface)) { \
      *ppv = static_cast<_iface *>(this); \
      (*(_iface **)ppv)->AddRef(); \
      return 0; \
    }

#define NM_INTERFACE_MAP_CHAIN(_chainclass) \
    if (_chainclass::QueryInterface(riid, ppv) != E_NOINTERFACE) { \
      return 0; \
    }

#define NM_END_INTERFACE_MAP \
    return E_NOINTERFACE; \
  }

/// use for Spirit forms when access IObject-based code needed
#define OBJECTIMPL_UNSUPPORTED  \
    ULONG PLATCALL AddRef() { return ( 0 ); }  \
    ULONG PLATCALL Release() { return ( 0 ); }

} // namespace navman

#endif  // _OBJECT_H_
