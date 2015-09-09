#pragma once
/** 
  @file
  Defines the IPersistableManager interface. 
  Copyright (C) 2005 - 2006, Navman New Zealand. All rights reserved.
*/

#ifndef _PERSISTABLE_H_
#define _PERSISTABLE_H_

namespace navman
{

class IObject;
class ISerialize;
class IPersist;

/**
  Interface for the persistable manager
*/
class IPersistableManager : public IObject
{
public:
  typedef bool TCreateFunc(ISerialize*, IPersist**);

  /**
    Registers a class of persistable objects with the persistable manager
    A persistable object must inherit from the IPersist interface class
    and register itself with the persistable manager.
    @param name specifies the name of the class to register
    @param pCreateFunc points to the creator method of the class
  */
  virtual void add(const WCHAR *name, TCreateFunc *pCreateFunc) = 0;

  /**
    Gets an object given the class name. This effectively creates an object
    of the class and returns it via the IPersist interface
    @param name specifies the name of the class
    @param serializer points to the serializer used for unmarshalling
    @param[out] ppObj points to the buffere receiving the object
    @return true if successful, false otherwise
  */
  virtual bool getObject(const WCHAR *name, ISerialize *pSerializer, IPersist **ppObj) = 0;

};

/**
  Implements the IPersistableManager interface
*/
class PersistableManager : public IPersistableManager
{
public:
  /**
    Factory method to create a persistable manager object
    @param[out] ppObj points to the buffer receiving the created object
    @return true if successful, false otherwise
  */
  static bool create(PersistableManager **ppObj);

  /**
    Factory method to create a persistable manager object
    see static bool create(PersistableManager **ppObj);
  */
  static bool create(IPersistableManager **ppObj);

};

} // namespace navman

#endif // PERSISTABLE
