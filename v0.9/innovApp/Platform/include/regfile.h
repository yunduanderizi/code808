#pragma once
#ifndef _REGFILE_H_
#define _REGFILE_H_

#include "object.h"

namespace navman {

/**
   Interface for managing getting and setting of ini file sections, keys and values.
   The RegfileManager object handles caching of the underlying ini files
*/
class IRegfileManager : public IObject
{
public:
  virtual bool getValue(const WCHAR *file,
                        const WCHAR *key,
                        const WCHAR *name,
                        WCHAR *buf,
                        UINT32 *len,
                        const WCHAR *defaultValue) = 0;

  virtual bool setValue(const WCHAR *file,
                        bool create,
                        const WCHAR *key,
                        const WCHAR *name,
                        const WCHAR *value) = 0;

  virtual bool removeValue(const WCHAR *file,
                           const WCHAR *key,
                           const WCHAR *name) = 0;

  virtual bool getNextChild(const WCHAR *file,
                            WCHAR *buf,
                            UINT32 *len,
                            bool first) = 0;

  virtual bool getNextValue(const WCHAR *file,
                            const WCHAR *key,
                            WCHAR *buf,
                            UINT32 *len,
                            bool first) = 0;

  virtual bool exists(const WCHAR *file) = 0;

  /**
     closes the file if it is the one open
     @param file path to the file to close
     @return true if the file was closed, false on
             failure or otherwise
  */
  virtual bool close(const WCHAR *file) = 0;
};

/**
 this is meant to be a singleton class and the creators
 below will only create a new instance if there isn't one
 already present in the system
*/
class RegfileManager : public IRegfileManager
{
public:
  /**
     Factory function to return the singleton RegfileManager object
     @param [out] ppObj pointer to receive the new object
     @return true on success, false otherwise
  */
  static bool create(RegfileManager **ppObj);

  /**
     Factory function to return the singleton RegfileManager object
     returned as an IRegfileManager interface.
     see RegfileManager::create(RegfileManager **ppObj);
  */
  static bool create(IRegfileManager **ppObj);

  /**
     @internal
     Forces any static instances of the regfile manager to unload
  */
  static void unload();
};

} // namespace navman

#endif // _REGFILE_H_
