#pragma once
/** @file 
    \brief De/Serializer of Persistable objects
    
 *****************************************************************************
 *  (c) 2005 navman new zealand limited. all rights reserved.
 *
 *  module name: serialize.h
 *
 *  Author: Jordan Foo
 *
 *  The Serializer/Deserializer is a component handles serializing and
 *  deserializing of objects. The objects has to be persistable to be used
 *  with the serializer. Or else it will cause an error.
 *
 *****************************************************************************/

#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include "object.h"
#include "smart.h"
#include "xmlfile.h"


namespace navman
{

class IPersist;
class IMoniker;
  
/*****************************************************************************
 *  INTERFACE ISerialize
 *****************************************************************************/
/** \brief Interface for all persistable objects to de/serialize itself
 *
 *  The interface provides the ability to read/write interger, string and
 *  objects. Here Objects are refer to as classes. The classes serializes or
 *  deserializes themselves by using Read/Write integer. If the class being
 *  De/Serialize has sub-class, that class should call Read/Write Object.
 *  The pattern continues until everything is serialized.
 *  In order to De/Serialize an object, the object itself and its sub-classes
 *  must be serializable as well. Meaning that they all should inherit the
 *  Interface IPersist.
 *****************************************************************************/
class ISerialize : public IObject
{
public:
  /***************************************************************************/  
  /** \brief Reads an integer
   *
   *  Reads an integer that is stored in the unknown storage type. Grabs the
   *  value based on the variable name (wcIntegeName) and outputs the integer
   *  obtain via nValueOut;
   *  
   *  @param wcInteger The variable name's value to get
   *  @param [out] nValueOut The variable's integer value;
   *  @return true if success, else false
   ***************************************************************************/
  virtual bool readInteger(const WCHAR *wcIntegerName, INT32 *nValueOut) = 0;

  /***************************************************************************/  
  /** \brief Reads an null terminated string with a specified length
   *
   *  The an null terminated string that is stored in the unknown storage type.
   *  The returned string will be a null terminated string of maximum length
   *  specified by nStrLen. if the length that is being requested is longer
   *  than specified, only nStrLen characters are returned.
   *
   *  @param wcStringName The variable name's string to get
   *  @param [out] wcStringOut The string of the variable name
   *  @param nStrLen The maximum allowable string length for wcStringOut
   *  @return true if success, else false   
   ***************************************************************************/
  virtual bool readString(const WCHAR *wcStringName, WCHAR *wcStringOut, UINT32 nStrLen) = 0;

  /***************************************************************************/  
  /** \brief Reads the entire object/Class
   *
   *  Reads the object being request, re-create the object and then passes the
   *  interface being request back via ppObject.\n
   *  This will basically recreate a class that was deserialized.
   *
   *  @param wcObjectName The object/variable name for that object to be created
   *  @param rInterfaceID magic number to the interface id usually IID_*
   *  @param [out] ppObject returns the interface pointer to the object created
   *  @return true if success, else false
   ***************************************************************************/
  virtual bool readObject(const WCHAR *wcObjectName, REFIID rInterfaceID, void **ppObject) = 0;

  /***************************************************************************/  
  /** \brief Writes an Integer
   *
   *  Writes an integer to the unknown storage type. Stores the value of the
   *  integer and it's variable name
   *
   *  @param wcIntegerName The variable name that is being stored
   *  @param nInputValue The integer value the variable is holding
   *  @return true if success, else false
   ***************************************************************************/
  virtual bool writeInteger(const WCHAR *wcIntegerName, INT32 nInputValue) = 0;

  /***************************************************************************/  
  /** \brief Writes a null terminated string
   *
   *  Write a null-terminated string to the unknown storage type. Stores both
   *  the string and the string's variable name
   *
   *  @param wcStringName the string's variable name to be stored
   *  @param wcInputString the string the variable it pointing to
   ***************************************************************************/
  virtual bool writeString(const WCHAR *wcStringName, const WCHAR *wcInputString) = 0;

  /***************************************************************************/  
  /** \brief Write the whole class or an object
   *
   *  Writes the object including its members. Stores the object's member name within
   *  its container object (the member name being the wcObjectName parameter),
   *  the class name, and all the object's member data.
   *
   *  @param wcObjectName the member name of the object to serialize
   *  @param pPersistableObject any persistable object to be serialized
   ***************************************************************************/
  virtual bool writeObject(const WCHAR *wcObjectName, IObject *pPersistableObject) = 0;

  /***************************************************************************/  
  /** \brief Retrieve the license associated with this serializer object
   *
   *  @param [out] returns the license associated with the serializer
   ***************************************************************************/
  virtual bool getLicense(IObject **ppLicense) = 0;

  /***************************************************************************/  
  /** \brief Retrieve the root moniker representing the data storage
   *
   *  @param [out] returns the root moniker of the serializer
   ***************************************************************************/
  virtual bool getRootMoniker(IMoniker **ppMoniker) = 0;

  /**
  Anonymous enum used to define some constants
  */
  enum {
    ///Maximum length of the string containing the object name
    MAX_OBJECT_NAME_LEN = 260,
    ///Maximum length of the string containing the text for an object
    MAX_OBJECT_TEXT_LEN = 260
  };
};


/*****************************************************************************
 *   CLASS Serialize
 *****************************************************************************/
/** \brief Serialize Object for serializing serializable object classes
 *
 *  Creates the Serializer object so that it can de/serialize objects. The
 *  interface ISerialize's object can only be created through the following
 *  methods only
 *****************************************************************************/
class Serialize : public ISerialize
{
public:
  /***************************************************************************/  
  /** \brief Creates a serializer using a XML node for the underlying storage
   *
   *  The Xml based serializer. Hierarchical structure is implicit, and the
   *  serializer will work only the boundary given by pNode.
   *
   *  @param pNode the Xml node that sets the boundary the serializer can work
   *  @param ppSerializer the interface to the Serializer object
   *  @param true if success, else false
   ***************************************************************************/
  static bool create(XmlNode *pNode, /*out*/ ISerialize **ppSerializer);

  /***************************************************************************/  
  /** \brief Creates a serializer using a XML node for the underlying storage.
   *         Provide a license to the serializer.
   *
   *  The Xml based serializer. Hierarchical structure is implicit, and the
   *  serializer will work only the boundary given by pNode.
   *
   *  @param pLicense the license with which deserialized objects can be created
   *  @param pNode the Xml node that sets the boundary the serializer can work
   *  @param ppSerializer the interface to the Serializer object
   *  @param true if success, else false
   ***************************************************************************/
  static bool create(IObject *pLicense, XmlNode *pNode, /*out*/ ISerialize **ppSerializer);

  /***************************************************************************/  
  /** \brief Creates a serializer using a moniker for the underlying storage.
   *         Provide a license to the serializer.
   *
   *  The Xml based serializer. Hierarchical structure is implicit, and the
   *  serializer will work only the boundary given by pNode.
   *
   *  @param pLicense the license with which deserialized objects can be created
   *  @param pMoniker the root moniker from which data should be fetched or written to.
   *  @param ppSerializer the interface to the Serializer object
   *  @param true if success, else false
   ***************************************************************************/
  static bool create(IObject *pLicense, IMoniker *pMoniker, /*out*/ ISerialize **ppSerializer);
};

} // for namespace navman

#endif // _SERIALIZE_H_

