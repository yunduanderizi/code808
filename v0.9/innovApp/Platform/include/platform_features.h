#pragma once
/**
  @file

  Use this file to define flags that control which features are supported 
  in the Platform library.

  Copyright (C) 2006, Navman New Zealand.  All rights Reserved.
*/

#ifndef _PLATFORM_FEATURES_H_
#define _PLATFORM_FEATURES_H_

#include "product_features.h"

/* 
  Each Feature has two defines 
  One with no value that is tested using #ifdef 
  e.g  #define HAS_FEATURE_WIDGET 
  #ifdef HAS_FEATURE_WIDGET 
  or 
  #ifnded HAS_FEATURE_WIDGET 

  One with a value that is tested using #if
  e.g. #define FEATURE_WIDGET 1
  #if FEATURE_WIDGET
  or 
  #if (FEATURE_WIDGET==3)
  The value should represent the version of the feature 
  while zero means that the feature is not present

  If you are writing code that should be included or should be excluded by the feature
  use #ifdef HAS_FEATURE_WIDGET

  If you are writing code that depends on a certain version of the feature 
  use #if (FEATURE_WIDGET==n)

  or both
  #if ( defined(HAS_FEATURE_WIDGET) && (FEATURE_WIDGET==5) )

  Both flags are used so you can control elements of the feature and also toggle features on and off.
*/

#ifdef TARGET

#if defined(DEFINED_PRODUCT_SPIRIT)

// Use new Tui SD card event
#define HAS_FEATURE_TUI_SDCARD_EVENT
#define FEATURE_TUI_SDCARD_EVENT  1

// Has USB Mass Storage
#define HAS_FEATURE_USB_MASS_STORAGE
#define FEATURE_USB_MASS_STORAGE  1

// Uses new MME USB driver
#define HAS_FEATURE_MME_USB_DRIVER
#define FEATURE_MME_USB_DRIVER  1

// Create hash signature for device.xml
#define HAS_FEATURE_HASH_SIGNATURE
#define FEATURE_HASH_SIGNATURE  1

#elif defined(DEFINED_PRODUCT_MME)

// Use new Tui SD card event
#define HAS_FEATURE_TUI_SDCARD_EVENT
#define FEATURE_TUI_SDCARD_EVENT  1

// Has USB Mass Storage
#define HAS_FEATURE_USB_MASS_STORAGE
#define FEATURE_USB_MASS_STORAGE  1

// Uses new MME USB driver
#define HAS_FEATURE_MME_USB_DRIVER
#define FEATURE_MME_USB_DRIVER  1

#elif defined(DEFINED_PRODUCT_TUI)

//// Use new Tui SD card event
#define HAS_FEATURE_TUI_SDCARD_EVENT
#define FEATURE_TUI_SDCARD_EVENT  1

// Has USB Mass Storage
#define HAS_FEATURE_USB_MASS_STORAGE
#define FEATURE_USB_MASS_STORAGE  1

#ifdef PRODUCT_TILAPIA
// Uses new MME USB driver
#define HAS_FEATURE_MME_USB_DRIVER
#define FEATURE_MME_USB_DRIVER  1
#endif

#elif defined(PRODUCT_TITAN) // LightningII (N20), Thunder (N40i) and Storm (N60i)


#elif defined(PRODUCT_ENDEAVOUR) // Cook (iCN 720) and Scott (iCN 750)


#elif defined(PRODUCT_GALLIUM) || defined(PRODUCT_QUICKSILVER)


#elif defined(PRODUCT_SDK500)


#else
#error No Product defined for target
#endif

#else // !TARGET // PC simulator, POSIX

#endif // !TARGET

#endif // _PLATFORM_FEATURES_H_

