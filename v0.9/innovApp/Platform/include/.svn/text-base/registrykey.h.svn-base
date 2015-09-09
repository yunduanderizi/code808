#pragma once
#ifndef __REGISTRYKEY_H__
#define __REGISTRYKEY_H__

#include "object.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace navman {

class IRegistryKey : public IObject
{
public:
  /**
     Get a value by name from the current key.
     @param name [in] name of the value
     @param pValue [out] buffer to be populated with value
     @param pValueLen [in/out] size of the output buffer
     @param pType [out] buffer to be populated with type
     @return true on success, otherwise false
   */
  virtual bool getValue(const WCHAR *name, BYTE *pValue, UINT32 *pValueLen, UINT32 *pType) = 0;
  /**
     Set a value in the current key.
     @param name [in] name of the value
     @param value [in] value of the value
     @param type [in] type of the value
     @return true on success, otherwise false
   */
  virtual bool setValue(const WCHAR *name, const BYTE *value, UINT32 valueLen, UINT32 type) = 0;

  /**
     Remove a value from the key
     @param name value to remove
     @return true on success, false otherwise
  */
  virtual bool removeValue(const WCHAR *name) = 0;

  /**
     Get a value by name from the current key.
     @param pName [out] buffer for the child's name
     @param pNameLen [in/out] size of the pName buffer
     @param pHKey [out] populated with an HKEY (unless NULL)
     @param first [in] true resets the enumeration to first element
     @return true on success, otherwise false
   */
  virtual bool getNextChild(WCHAR *pName, UINT32 *pNameLen, HKEY *pHKey, bool first = false) = 0;
  /**
     Get the next value in the enumeration.
     @param pName [out] buffer for the value's name
     @param pNameLen [in/out] size of the pName buffer
     @param pValue [out] buffer for the value's value
     @param pValueLen [in/out] size of the pValue buffer
     @param first [in] true resets the enumeration to first element
     @return true on success, otherwise false
   */
  virtual bool getNextValue(WCHAR *pName, UINT32 *pNameLen, WCHAR *pValue, UINT32 *pValueLen, UINT32 *pType, bool first = false) = 0;
};

class RegistryKey : public IRegistryKey
{
public:
  /**
     Factory function to create a RegistryKey object
     @param url path to required registry key
     @param create if true, key will be created if it doesn't already exist
     @param readonly if true, key will be opened read-only
     @param [out] ppReg pointer to receive the new RegistryKey
  */
  static bool create(const WCHAR *url,
                     bool create,
                     bool readonly,
                     IRegistryKey **ppReg);
  /**
     Factory function to create a RegistryKey object
     @param url path to required registry key
     @param create if true, key will be created if it doesn't already exist
     @param readonly if true, key will be opened read-only
     @param [out] ppReg pointer to receive the new RegistryKey
  */
  static bool create(const WCHAR *url,
                     bool create,
                     bool readonly,
                     RegistryKey **ppReg);
};

}

#endif // __REGISTRYKEY_H__
