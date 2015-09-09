#pragma once
/** @file
\brief Provides access to settings and paths (e.g. those contained in
       "paths.xml") that are shared between device and desktop.

(c) 2007 navman new zealand. all rights reserved.
*/

#ifndef __DESKTOP_INTEGRATION_H__
#define __DESKTOP_INTEGRATION_H__

#include "object.h"

namespace navman
{

  extern const GUID IID_IDesktopIntegration;

  /**
  Provides access to settings and paths shared with Desktop.
  */
  class IDesktopIntegration : public IObject
  {

  public:
    virtual const WCHAR* getRootPath( void ) const = 0;
    virtual int getRootPath(LPWSTR p_wstrBuffer, int p_nBufferLength, LPCWSTR p_wstrFormat, ...) const = 0;

    virtual const WCHAR* getAppStartupSecPath( void ) const = 0;
    virtual const WCHAR* getAppStartupSecFile( void ) const = 0;

    virtual const WCHAR* getSmartSTPath( void ) const = 0;
    virtual const WCHAR* getSmartSTFile( void ) const = 0;

    /**
    Fetch standard database access path.  It will contain database
    file name but will not contain storage device.  This path can
    be used to open or attach main database on any storage device.
    This path cannot be used to access a database embedded within
    NMF bundle.
    @return Database access path if successful; empty string if not.
    */
    virtual const WCHAR* getDatabasePath( void ) const = 0;

    /**
    Fetch standard access path for Custom POI icons.  It will not 
    contain storage device.  This path can be used to access directory
    with Custom POI icons on any storage device.  This path cannot be
    used to access Custom POI icons embedded within NMF bundle.
    @return String containing Custom POI icons access path if successful;
            empty string if not.
    */
    virtual const WCHAR* getDatabaseIconsPath( void ) const = 0;

    /**
    Fetch standard access path for categorized photos.  It will not 
    contain storage device.  This path can be used to access directory
    with categorized photos on any storage device.  This path cannot be
    used to access categorized photos embedded within NMF bundle.
    @return String containing categorized photos access path if successful;
            empty string if not.
    */
    virtual const WCHAR* getDatabasePhotosPath( void ) const = 0;

    /**
    Retrieve the mileage log file path
    @return String containing the mileage log file path if successful;
            empty string if not.
    */
    virtual const WCHAR *getMileageLogPath( void ) const = 0;

    virtual const WCHAR *getAuthenticationPath( void ) const = 0;
    virtual const WCHAR *getAvoidAreasPath( void ) const = 0;
    virtual const WCHAR *getFavouritesPath( void ) const = 0;
    virtual const WCHAR *getMultistopRoutesPath( void ) const = 0;
    virtual const WCHAR *getPOISettingsPath( void ) const = 0;
    virtual const WCHAR *getRecentsPath( void ) const = 0;
    virtual const WCHAR *getSettingsPath( void ) const = 0;
    virtual const WCHAR *getAlertSoundsPath( void ) const = 0;
    virtual const WCHAR *getMapDataPath( void ) const = 0;

  }; // class IDesktopIntegration

  /**
  IDesktopIntegration singleton
  */
  IDesktopIntegration* DesktopIntegration( void );

} // namespace navman 

#endif // __DESKTOP_INTEGRATION_H__
