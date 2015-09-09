/** @file
@internal Contains a class that provides a thread-safe internal string type.

(c) 2008 navman new zealand. all rights reserved.
*/


#pragma once

#ifndef OSSTRING_H
#define OSSTRING_H



/*****************************************************************************
*  Dependencies
*****************************************************************************/

#include "appcore_platform.h"


namespace navman
{

  /**
     Provides a thread-safe internal string type.

     Implementation: wraps a WHCAR* string.

     In the future, we may want to expand this type to also wrap the
     types CHAR* (for ASCII strings) and UTF16* (for UTF16-encoded
     strings), with automatic conversions among the types.
  */
  class COSString
  {
  public:

    /**
       Zero-parameter constructor.
    */
    COSString();

    /**
       Construct a COSString from a wide string, i.e., a string of type WCHAR*.
       The parameter can also be a string of types NWPSTR, LPWSTR, or PWSTR, which
       are defined as null-terminated WCHAR*, or of types LPCWSTR or PCWSTR, which
       are defined as null-terminated CONST WCHAR*.

       @param string the wide string
    */
    explicit COSString(const WCHAR* wstr);

    /**
       Construct a COSString from a narrow string, i.e., a string of type CHAR*.
       The parameter can also be a string of types NPSTR, LPSTR, or PSTR, which
       are defined as null-terminated CHAR*, or of types LPCSTR or PCSTR, which
       are defined as null-terminated CONST CHAR*.

       @param string the wide string
    */
    explicit COSString::COSString(const CHAR* str);

    ~COSString();

    /**
       Interpret the COSString object as a constant wide string, i.e., of type
       const WCHAR*.  The return value can be automatically cast as a string
       of type PWCHAR, LPWCH, or PWCH, which are defined as WCHAR* (?can it?),
       or of type LPCWCH or PCWCH, which are defined as CONST WCHAR*.
    */
    operator const COSString::WCHAR*();

    /**
       Returns a non-constant WCHAR* string.  (Prefer the constant version
       of this implicit conversion function if possible.)  A NULL may be
       returned.  To prevent memory leaks, remember to delete the returned
       string.
    */
  /*
    operator WCHAR*()
    {
      if (_wide_string == NULL) return NULL;
      UINT32 len = wcslen(_wide_string);
      WCHAR* out_string = new WCHAR[len+1]; // the +1 character is to accommodate the terminator
      wcscpy(out_string, _wide_string);
      return out_string;
    }
  */

    /**
       Assigns a new wide string to this object.

       @param str The string to assign.
       @return this
    */
    COSString& COSString::operator=(const WCHAR* wstr);


  protected: // functions

    /**
       Copies the wide string contained in the parameter to _wide_string.
    */
    void COSString::initialiseWithWideString(const WCHAR* wstr);

    /**
       Copies the narrow string contained in the parameter to _wide_string.
    */
    void COSString::initialiseWithNarrowString(const CHAR* str);


  protected: // members

    WCHAR* _wide_string;

  }; // class COSString


} // namespace navman


#endif // OSSTRING_H