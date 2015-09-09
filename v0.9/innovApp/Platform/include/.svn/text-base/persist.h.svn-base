#pragma once
/** @file 
    \brief the interface any serializable/persistable class must inherit
    
 *****************************************************************************
 *  (c) 2005 navman new zealand limited. all rights reserved.
 *
 *  module name: persist.h
 *
 *  Author: Jordan Foo
 *
 *  The full description
 *
 *****************************************************************************/

#ifndef _PERSIST_H_
#define _PERSIST_H_
 
#include "object.h"

namespace navman
{

class ISerialize;

/*****************************************************************************
 *  INTERFACE IPersist
 *****************************************************************************/
/** \brief Labels a class persistable (serializable)
 *
 *  This works in relation with ISerialize. Inheriting this interface will
 *  allow your class to be serializable using ISerialize. The methods list
 *  in this interface are all implemented by the persistable/serializable
 *  class.\n\n
 *  The class handles how and what is stored when serializing and this is
 *  done via SaveTo() method call and hardcodes it's class name through
 *  GetClassName
 *****************************************************************************/
class IPersist : public IObject
{
public:
  /***************************************************************************/  
  /** \brief Serialise the class to a serial data
   *
   *  The class that inherits IPersist handles how or what to store when it is
   *  being serialized. Here it will have access to ISerialize's read and
   *  write methods but here you WILL only use write, as reading just makes no
   *  sense when serializing an object.\n\n
   *  Note: This method is called by the serializer, not by the class
   *
   *  @param pISerialize Interface to the serializtion methods
   *  @return true if success, else false
   ***************************************************************************/
  virtual bool saveTo(ISerialize *pSerializer) = 0;
  
  /***************************************************************************/  
  /** \brief Gets the name of the class that will be serialized
   *
   *  The serializer needs to know what class is it serializing and the class
   *  name is known through this call here. \n
   *  Here it is up to programmer to fill in the class name when the serializer
   *  request for it. The class name returned is normally just the a hardcoded
   *  string and it is the same name as the class contructor. Although it does
   *  not need to be, but that would be unwise.
   *
   *  @param [out] wcClassName the class name
   *  @param nStrLen the maximum length of the class name to be returned
   ***************************************************************************/
  virtual void getClassName(WCHAR *wcClassName, UINT32 nStrLen) = 0;
};

}

#endif // _PERSIST_H_

