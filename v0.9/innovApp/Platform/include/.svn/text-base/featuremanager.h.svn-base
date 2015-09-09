#pragma once
/** @file
\brief Contains feature settings.

(c) 2007 navman new zealand. all rights reserved.
*/

#ifndef __FEATURE_MANAGER_H__
#define __FEATURE_MANAGER_H__

#include "object.h"

namespace navman
{
  // Shared definitions
#define FEATURE_IAPPLIANCE_ONLINE   1
#define FEATURE_IAPPLIANCE_OFFLINE  2
#define FEATURE_IAPPLIANCE_ALL      3

  extern const GUID IID_FEATURE_NAVPIX;
  extern const GUID IID_FEATURE_CUSTOM_POI;
  extern const GUID IID_FEATURE_MILEAGE;
  extern const GUID IID_FEATURE_AUTO_SCREEN_CAPTURE;
  extern const GUID IID_FEATURE_TRAFFIC;
  extern const GUID IID_FEATURE_IAPPLIANCE;
  extern const GUID IID_FEATURE_HANDSFREE;
  extern const GUID IID_FEATURE_BLUETOOTH;
  extern const GUID IID_FEATURE_TTS;
  extern const GUID IID_FEATURE_DDG2P;
  extern const GUID IID_FEATURE_VOICE_CMD;
  extern const GUID IID_FEATURE_POI_ICON_CACHE;
  extern const GUID IID_FEATURE_BUTTONLESS_ID;
  extern const GUID IID_FEATURE_MEDIA_PLAYER;

  extern const GUID IID_IFeatureManager;

  /**
  Provides access to feature settings.
  */
  class IFeatureManager : public IObject
  {
  public:
    /**
    Set feature status.
    */
    virtual void setFeatureSupport(const GUID &p_GUID, bool p_bSupported) = 0;

    virtual void setFeatureSetting(const GUID &p_GUID, INT8 p_nSetting) = 0;

    /**
    Get feature status.
    @return true if feature is supported, false if not.
    */
    virtual bool isFeatureSupported(const GUID &p_GUID) const = 0;

    virtual INT8 getFeatureSetting(const GUID &p_GUID) const = 0;
  };

  /**
  IFeatureManager singleton
  */
  IFeatureManager *FeatureManager(void);

} // namespace navman 

#endif // __FEATURE_MANAGER_H__
