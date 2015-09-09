#pragma once
#ifndef _PROPERTYBAG_H_
#define _PROPERTYBAG_H_

#include "object.h"

namespace navman {

class IXmlNode;

/**
   Interface to a bag (i.e. unordered set) of name-value pairs
*/
class IPropertyBag : public IObject
{
public:
  /**
     Get the underlying property as a 16 bit signed number.  Fails if the
     underlying property cannot be converted.
     @param name name of property to get
     @param pValue pointer to value to return
     @param devfal default value to set
     @return true on success, false otherwise
   */
  virtual bool getProperty(const WCHAR *name,  INT16 *pValue, INT16 defval = 0) = 0;

  /**
     Get the underlying property as a 16 bit unsigned number.  Fails if the
     underlying property cannot be converted.
     @param name name of property to get
     @param pValue pointer to value to return
     @param defval default value to set
     @return true on success, false otherwise
   */
  virtual bool getProperty(const WCHAR *name, UINT16 *pValue, UINT16 defval = 0) = 0;

  /**
     Get the underlying property as a 32 bit signed number.  Fails if the
     underlying property cannot be converted.
     @param name name of property to get
     @param pValue pointer to value to return
     @param devfal default value to set
     @return true on success, false otherwise
   */
  virtual bool getProperty(const WCHAR *name,  INT32 *pValue, INT32 defval = 0) = 0;

  /**
     Get the underlying property as a 32 bit unsigned number.  Fails if the
     underlying property cannot be converted.
     @param name name of property to get
     @param pValue pointer to value to return
     @param defval default value to set
     @return true on success, false otherwise
   */
  virtual bool getProperty(const WCHAR *name, UINT32 *pValue, UINT32 defval = 0) = 0;

  /**
     Get the underlying property as a 64 bit unsigned number.  Fails if the
     underlying property cannot be converted.
     @param name name of property to get
     @param pValue pointer to value to return
     @param defval default value to set
     @return true on success, false otherwise
   */
  virtual bool getProperty(const WCHAR *name, UINT64 *pValue, UINT64 defval = 0) = 0;

  /**
     Get the underlying property as a float.  Fails if the
     underlying property cannot be converted.
     @param name name of property to get
     @param pValue pointer to value to return
     @param defval default value to set
     @return true on success, false otherwise
   */
  virtual bool getProperty(const WCHAR *name, float *pValue, float defval = 0.0) = 0;

  /**
     Get the underlying property as a nul terminated WCHAR *.  Fails if the
     underlying property cannot be converted.
     @param name name of property to get
     @param pValue pointer to buffer to copy data into
     @param len length of buffer
     @param defval default value to set if the key doesn't exist
     @return true on success, false otherwise
   */
  virtual bool getProperty(const WCHAR *name, WCHAR *pValue, UINT32 len, const WCHAR *defval = 0) = 0;

  /**
     Set the underlying property as a 32 bit signed number.  Fails if the
     underlying property cannot be converted.
     @param name name of property to get
     @param pValue pointer to value to return
     @return true on success, false otherwise
   */
  virtual bool setProperty(const WCHAR *name,  INT32 value) = 0;

  /**
     Set the underlying property as a 32 bit unsigned number.
     @param name name of property to get
     @param pValue pointer to value to return
     @return true on success, false otherwise
   */
  virtual bool setProperty(const WCHAR *name, UINT32 value) = 0;

  /**
     Set the underlying property as a 32 bit unsigned number.
     @param name name of property to get
     @param pValue pointer to value to return
     @return true on success, false otherwise
   */
  virtual bool setProperty(const WCHAR *name, UINT64 value) = 0;

  /**
     Set the underlying property as a float.
     @param name name of property to get
     @param value value to set
     @return true on success, false otherwise
   */
  virtual bool setProperty(const WCHAR *name, float value) = 0;

  /**
     Set the underlying property as a nul terminated WCHAR *.
     @param name property to get
     @param value pointer to value to set
     @return true on success, false otherwise
   */
  virtual bool setProperty(const WCHAR *name, const WCHAR *value) = 0;

  /**
     remove the property from the bag
     @param name property to remove
     @return true on successful removal, false on error or if the
                  property didn't exist
  */
  virtual bool removeProperty(const WCHAR *name) = 0;

  /**
     Gets the next child of a property bag.
     @param pChild buffer to copy the name of the child into
     @param pChildLen points to the length of the buffer, if the buffer isn't big
                      enough, it is set to the size the buffer should be (and getNextChild
                      returns false without incrementing to the next child)
     @param first true if the iteration should be reset, false if the current child
                       should be returned
     @return true if the nth value exists, false if it doesn't or if the size
                  pointed to by pChildLen isn't big enough
  */
  virtual bool getNextChild(WCHAR *pChild, UINT32 *pChildLen, bool first = false) = 0;

  /**
     Get the next value of a property bag
     @param pValue buffer to copy value name into
     @param pValuelen size of the buffer, if the buffer isn't big enough it gets set to
            the size the buffer should be (and getValue returns false)
     @param first true if the iteration should be reset, false if the current value
                       should be returned
     @return true if the next value exists, false if it doesn't or if the size
                  pointed to by pValueLen isn't big enough
  */
  virtual bool getNextValue(WCHAR *pValue, UINT32* pValueLen, bool first = false) = 0;

  /**
     Sets the default properties
     @param pDefaults : propertyBag representing the default set of properties.
                        if a property exists in the underlying property bag the value for that is
                        taken, otherwise if it is in the defaults propertybag, the value for that is used,
                        otherwise the value passed in as the parameter to the method is used
     @return true on success, false otherwise                   
  */
  virtual bool setDefaults(IPropertyBag *pDefaults) = 0;
  
  /**
     Gets the defaults property bag
     @param [out] ppObj receives the property bag
     @return true on success, false on error or if the default properties doesn't exist
  */
  virtual bool getDefaults(IPropertyBag **ppDefaults) = 0;

  /**
     Return extended error code
     @return extended error information
  */
  virtual UINT32 getError() = 0;

  /**
     Create a copy of this property bag, and set ppBag to point to it.
     @param url url to create.  see PropertyBag::create. if NULL a non-persistent
                propertybag is created.
     @param ppBag clone out-paramater
     @return true on success, false otherwise
   */
  virtual bool clone(IPropertyBag **ppBag) = 0;

  /**
     Copy the keys of this bag to another
     @param pBag [in] bag to receive new settings
     @return true on success, false otherwise
  */
  virtual bool copyTo(IPropertyBag *pBag) = 0;
};

/**
   PropertyBag object
*/
class PropertyBag : public IPropertyBag
{
public:

  /**
     Factory function to create a non-persistent, in-memory-only property bag
     @param ppObj will point to a new property bag, already addref'd
     @return true on success, false otherwise
  */
  static bool create(PropertyBag **ppObj);

  /**
     Factory function to create a non-persistent, in-memory-only property bag returned as an
     IPropertyBag pointer.
     @param ppObj will point to a new property bag, already addref'd
     @return true on success, false otherwise
  */
  static bool create(IPropertyBag **ppObj);

  /**
    Factory function to create a property bag from an xmlnode. Will find first propertybag
    occuring within the node. 
    @param pNode will contain a property bag
    @param ppBag will point to a new property bag
    @return true on success, false otherwise
  */  
  static bool create(IXmlNode *pNode, PropertyBag **ppBag);
  /**
     Factory function to create a property bag backed by the appropriate store
     @param path to url (or file path with optional key path).  For .ini files,
                 the file and the key are ':' separated.
                 e.g. c:\settings.ini:section1.  
                 For .xml files, the file and the path to the node are ':' separated.
                 The path to the node is given using only the names of propertybags and '\\'.
                 e.g. c:\test.xml:root\child1
                 where 'root' and 'child1' are the names of two propertybags.
                 The currently recognized protocols are
                 http://, file://, reg://.  If no protocol is specified, it defaults to
                 file://
     @param url specifying defaults.  defaults work as follows: if the property is present
                 in the underlying propertybag (specified by the url) then that value is used,
                 otherwise if the property is present in the defaults that value is used, otherwise
                 the default passed in as a parameter to the getProperty method is used.  NULL if
                 no defaults are to be set (or you are going to use setDefaults after the object is
                 created).
     @param create true if the underlying object (file, registry key, whatever) should
                   should be created if it doesn't exist, false if not
     @param readonly true if the bag should be created without allowing changes.  Applications
                          should set this to true unless changes are desired since the underlying
                          object may be stored in rom or some other readonly store that cannot
                          be opened with write permissions.
     @param ppObj will point to a new property bag, already addref'd
     @return true on success, false otherwise
  */
  static bool create(const WCHAR *url, const WCHAR *defaultsUrl, bool create, bool readonly, PropertyBag **ppObj);

  /**
     Factory function to create a propertybag backed by the appropriate store, returned as an IPropertyBag.
     see create(const WCHAR *url, const WCHAR *defaultsUrl, bool create, bool readonly, PropertyBag **ppObj)
  */
  static bool create(const WCHAR *url, const WCHAR *defaultsUrl, bool create, bool readonly, IPropertyBag **ppObj);
};

} // namespace navman

#endif // _PROPERTYBAG_H_
