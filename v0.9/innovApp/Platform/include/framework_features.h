#pragma once
/**
  @file

  Use this file to define flags that control which features are supported 
  in the franework library.

  Copyright (C) 2006, Navman New Zealand.  All rights Reserved.
*/

#ifndef _FRAMEWORK_FEATURES_H_
#define _FRAMEWORK_FEATURES_H_

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

#elif defined(DEFINED_PRODUCT_MME)

// Use SiRF's CLM (Client Location Manager) library for GPS
#define HAS_FEATURE_SIRF_CLM
#define FEATURE_SIRF_CLM  1

#elif defined(DEFINED_PRODUCT_TUI)

// Use SiRF's CLM (Client Location Manager) library for GPS
#define HAS_FEATURE_SIRF_CLM
#define FEATURE_SIRF_CLM  1

#elif defined(PRODUCT_TITAN) // LightningII (N20), Thunder (N40i) and Storm (N60i)

// Use SiRF's CLM (Client Location Manager) library for GPS
#define HAS_FEATURE_SIRF_CLM
#define FEATURE_SIRF_CLM  1

#elif defined(PRODUCT_ENDEAVOUR) // Cook (iCN 720) and Scott (iCN 750)


#elif defined(PRODUCT_GALLIUM) || defined(PRODUCT_QUICKSILVER)


#elif defined(PRODUCT_SDK500)

// Use SiRF's CLM (Client Location Manager) library for GPS
#define HAS_FEATURE_SIRF_CLM
#define FEATURE_SIRF_CLM  1

#else
#error No Product defined for target
#endif

#else // !defined(TARGET) // PC simulator

#endif // !defined(TARGET)

#endif // _FRAMEWORK_FEATURES_H_

