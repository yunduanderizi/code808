#pragma once
/**
  @file

  Use this file to define flags that control which features to be included 
  in the various product builds.

  Features that are common to multiple libraries should be defined in 
  this file.
  Features that are used only within one library should be defined in 
  the features file within that library.

  Copyright (C) 2006, 2007 Navman Technology New Zealand.  All rights Reserved.
*/

#ifndef _PRODUCT_FEATURES_H_
#define _PRODUCT_FEATURES_H_

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

// Shared definitions
#define FEATURE_IAPPLIANCE_ONLINE   1
#define FEATURE_IAPPLIANCE_OFFLINE  2
#define FEATURE_IAPPLIANCE_ALL      3


#ifdef TARGET

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_COBIA_300_NA
// ****************************************************************************
#if defined(PRODUCT_COBIA_300_NA) // 64 MB RAM, NA SKU

// tells the software that the device does not map hardware map and menu buttons and 
// should provide them on the map screen.
#define HAS_FEATURE_BUTTONLESS_ID
#define FEATURE_BUTTONLESS_ID 1

// TTS
#define HAS_FEATURE_TTS
#define FEATURE_TTS 1

// TRAFFIC
#define HAS_FEATURE_TRAFFIC
#define FEATURE_TRAFFIC 1

// Enable this definition to include code to prevent features that are memory hungry from
// running at the same time and so exhausting memory.
#define HAS_FEATURE_FORCED_MEMORY_SHARING
#define FEATURE_FORCED_MEMORY_SHARING 1

// Use dual memory heaps - one existing in process, the other in shared memory space.
// For devices with more than 32Mb of RAM.
#define HAS_FEATURE_SHARED_MEM_HEAP
#define FEATURE_SHARED_MEM_HEAP 1

// ****************************************************************************
// * End of Feature Flags for PRODUCT_COBIA_300_NA
// ****************************************************************************

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_COBIA_300_AU
// ****************************************************************************
#elif defined(PRODUCT_COBIA_300_AU) // 64 MB RAM, AU SKU

// tells the software that the device does not map hardware map and menu buttons and 
// should provide them on the map screen.
#define HAS_FEATURE_BUTTONLESS_ID
#define FEATURE_BUTTONLESS_ID 1

// NAVPIX -- requires Custom POI!!!
#define HAS_FEATURE_NAVPIX          // define this to get Image browser views
#define FEATURE_NAVPIX 1	

// CUSTOM POI
#define HAS_FEATURE_CUSTOM_POI
#define FEATURE_CUSTOM_POI 1

// Mileage Recording
#define HAS_FEATURE_MILEAGE
#define FEATURE_MILEAGE 1

// TTS
#define HAS_FEATURE_TTS
#define FEATURE_TTS 1

// TRAFFIC
#define HAS_FEATURE_TRAFFIC
#define FEATURE_TRAFFIC 1

// iAppliance 
#define HAS_FEATURE_IAPPLIANCE

// Offline iAppliance
#define FEATURE_IAPPLIANCE FEATURE_IAPPLIANCE_OFFLINE

// Enable this definition to include code to prevent features that are memory hungry from
// running at the same time and so exhausting memory.
#define HAS_FEATURE_FORCED_MEMORY_SHARING
#define FEATURE_FORCED_MEMORY_SHARING 1

// Use dual memory heaps - one existing in process, the other in shared memory space.
// For devices with more than 32Mb of RAM.
#define HAS_FEATURE_SHARED_MEM_HEAP
#define FEATURE_SHARED_MEM_HEAP 1

// ****************************************************************************
// * End of Feature Flags for PRODUCT_COBIA_300_AU
// ****************************************************************************

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_COBIA_T300_NA
// ****************************************************************************
#elif defined(PRODUCT_COBIA_T300_NA) // 64 MB RAM, NA SKU

// TRAFFIC
#define HAS_FEATURE_TRAFFIC
#define FEATURE_TRAFFIC 1

// tells the software that the device does not map hardware map and menu buttons and 
// should provide them on the map screen.
#define HAS_FEATURE_BUTTONLESS_ID
#define FEATURE_BUTTONLESS_ID 1

// TTS
#define HAS_FEATURE_TTS
#define FEATURE_TTS 1

//// DDG2P
//#if defined( HAS_FEATURE_TTS )
//#define HAS_FEATURE_DDG2P
//#define FEATURE_DDG2P 1
//#endif // HAS_FEATURE_TTS

// Enable this definition to include code to prevent features that are memory hungry from
// running at the same time and so exhausting memory.
#define HAS_FEATURE_FORCED_MEMORY_SHARING
#define FEATURE_FORCED_MEMORY_SHARING 1

// Use dual memory heaps - one existing in process, the other in shared memory space.
// For devices with more than 32Mb of RAM.
#define HAS_FEATURE_SHARED_MEM_HEAP
#define FEATURE_SHARED_MEM_HEAP 1

// ****************************************************************************
// * End of Feature Flags for PRODUCT_COBIA_T300_NA
// ****************************************************************************

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_COBIA_400_NA
// ****************************************************************************
#elif defined(PRODUCT_COBIA_400_NA) // 64 MB RAM, NA SKU

// tells the software that the device does not map hardware map and menu buttons and 
// should provide them on the map screen.
#define HAS_FEATURE_BUTTONLESS_ID
#define FEATURE_BUTTONLESS_ID 1

// TTS
#define HAS_FEATURE_TTS
#define FEATURE_TTS 1

// TRAFFIC
#define HAS_FEATURE_TRAFFIC
#define FEATURE_TRAFFIC 1

// Enable this definition to include code to prevent features that are memory hungry from
// running at the same time and so exhausting memory.
#define HAS_FEATURE_FORCED_MEMORY_SHARING
#define FEATURE_FORCED_MEMORY_SHARING 1

// Use dual memory heaps - one existing in process, the other in shared memory space.
// For devices with more than 32Mb of RAM.
#define HAS_FEATURE_SHARED_MEM_HEAP
#define FEATURE_SHARED_MEM_HEAP 1

// ****************************************************************************
// * End of Feature Flags for PRODUCT_COBIA_400_NA
// ****************************************************************************

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_COBIA_T400_NA
// ****************************************************************************
#elif defined(PRODUCT_COBIA_T400_NA) // 64 MB RAM, NA SKU

// TRAFFIC
#define HAS_FEATURE_TRAFFIC
#define FEATURE_TRAFFIC 1

// tells the software that the device does not map hardware map and menu buttons and 
// should provide them on the map screen.
#define HAS_FEATURE_BUTTONLESS_ID
#define FEATURE_BUTTONLESS_ID 1

// TTS
#define HAS_FEATURE_TTS
#define FEATURE_TTS 1

//// DDG2P
//#if defined( HAS_FEATURE_TTS )
//#define HAS_FEATURE_DDG2P
//#define FEATURE_DDG2P 1
//#endif // HAS_FEATURE_TTS

// Enable this definition to include code to prevent features that are memory hungry from
// running at the same time and so exhausting memory.
#define HAS_FEATURE_FORCED_MEMORY_SHARING
#define FEATURE_FORCED_MEMORY_SHARING 1

// Use dual memory heaps - one existing in process, the other in shared memory space.
// For devices with more than 32Mb of RAM.
#define HAS_FEATURE_SHARED_MEM_HEAP
#define FEATURE_SHARED_MEM_HEAP 1

// ****************************************************************************
// * End of Feature Flags for PRODUCT_COBIA_T400_NA
// ****************************************************************************

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_COBIA_T420_NA
// ****************************************************************************
#elif defined(PRODUCT_COBIA_T420_NA) // 64 MB RAM, NA SKU

// NAVPIX -- requires Custom POI!!!
#define HAS_FEATURE_NAVPIX          // define this to get Image browser views
#define FEATURE_NAVPIX 1

// CUSTOM POI
#define HAS_FEATURE_CUSTOM_POI
#define FEATURE_CUSTOM_POI 1

// Mileage Recording
#define HAS_FEATURE_MILEAGE
#define FEATURE_MILEAGE 1

// TRAFFIC
#define HAS_FEATURE_TRAFFIC
#define FEATURE_TRAFFIC 1

// iAppliance 
// Disabled for NA SKUs
//#define HAS_FEATURE_IAPPLIANCE

// Online and offline
// Disabled for NA SKUs
//#define FEATURE_IAPPLIANCE FEATURE_IAPPLIANCE_ALL

// Handsfree 
#define HAS_FEATURE_HANDSFREE
#define FEATURE_HANDSFREE 1

// Bluetooth
#if defined(HAS_FEATURE_HANDSFREE) || defined(HAS_FEATURE_IAPPLIANCE)
#define HAS_FEATURE_BLUETOOTH
#define FEATURE_BLUETOOTH 1
#endif //defined(HAS_FEATURE_HANDSFREE) || defined(HAS_FEATURE_IAPPLIANCE)

// tells the software that the device does not map hardware map and menu buttons and 
// should provide them on the map screen.
#define HAS_FEATURE_BUTTONLESS_ID
#define FEATURE_BUTTONLESS_ID 1

// Include ability to launch and switch to AudioPlayer
//#define HAS_FEATURE_AUDIOPLAYER
//#define FEATURE_AUDIOPLAYER 1

// TTS
// Enabled for NA SKUs
#define HAS_FEATURE_TTS
#define FEATURE_TTS 1

//// DDG2P
//#if defined( HAS_FEATURE_TTS )
//#define HAS_FEATURE_DDG2P
//#define FEATURE_DDG2P 1
//#endif // HAS_FEATURE_TTS

// Enable this definition to include code to prevent features that are memory hungry from
// running at the same time and so exhausting memory.
#define HAS_FEATURE_FORCED_MEMORY_SHARING
#define FEATURE_FORCED_MEMORY_SHARING 1

// Use dual memory heaps - one existing in process, the other in shared memory space.
// For devices with more than 32Mb of RAM.
#define HAS_FEATURE_SHARED_MEM_HEAP
#define FEATURE_SHARED_MEM_HEAP 1

// ****************************************************************************
// * End of Feature Flags for PRODUCT_COBIA_T420_NA
// ****************************************************************************

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_COBIA_T420_AU
// ****************************************************************************
#elif defined(PRODUCT_COBIA_T420_AU) // 64 MB RAM, AU SKU

// NAVPIX -- requires Custom POI!!!
#define HAS_FEATURE_NAVPIX          // define this to get Image browser views
#define FEATURE_NAVPIX 1

// CUSTOM POI
#define HAS_FEATURE_CUSTOM_POI
#define FEATURE_CUSTOM_POI 1

// Mileage Recording
#define HAS_FEATURE_MILEAGE
#define FEATURE_MILEAGE 1

// TRAFFIC
#define HAS_FEATURE_TRAFFIC
#define FEATURE_TRAFFIC 1

// TTS
#define HAS_FEATURE_TTS
#define FEATURE_TTS 1

// iAppliance
#define HAS_FEATURE_IAPPLIANCE

// Online and offline
#define FEATURE_IAPPLIANCE FEATURE_IAPPLIANCE_ALL

// Handsfree 
#define HAS_FEATURE_HANDSFREE
#define FEATURE_HANDSFREE 1

// Bluetooth
#if defined(HAS_FEATURE_HANDSFREE) || defined(HAS_FEATURE_IAPPLIANCE)
#define HAS_FEATURE_BLUETOOTH
#define FEATURE_BLUETOOTH 1
#endif //defined(HAS_FEATURE_HANDSFREE) || defined(HAS_FEATURE_IAPPLIANCE)

// tells the software that the device does not map hardware map and menu buttons and 
// should provide them on the map screen.
#define HAS_FEATURE_BUTTONLESS_ID
#define FEATURE_BUTTONLESS_ID 1

// Include ability to launch and switch to AudioPlayer
#define HAS_FEATURE_AUDIOPLAYER
#define FEATURE_AUDIOPLAYER 1

// Enable this definition to include code to prevent features that are memory hungry from
// running at the same time and so exhausting memory.
#define HAS_FEATURE_FORCED_MEMORY_SHARING
#define FEATURE_FORCED_MEMORY_SHARING 1

// Use dual memory heaps - one existing in process, the other in shared memory space.
// For devices with more than 32Mb of RAM.
#define HAS_FEATURE_SHARED_MEM_HEAP
#define FEATURE_SHARED_MEM_HEAP 1

// ****************************************************************************
// * End of Feature Flags for PRODUCT_COBIA_T420_AU
// ****************************************************************************

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_PLATINUM
// ****************************************************************************
#elif defined(PRODUCT_PLATINUM) // S90i

/*
// CAMERA
#define HAS_FEATURE_CAMERA_HARDWARE // define this to get Camera Capture views.
#define FEATURE_CAMERA_HARDWARE 2   // Mitac 2mp camera	
*/

// ****************************************************************************
// * End of Feature Flags for PRODUCT_PLATINUM
// ****************************************************************************

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_TITAN
// ****************************************************************************
#elif defined(PRODUCT_TITAN) // LightningII (N20), Thunder (N40i) and Storm (N60i)

/*
// CAMERA
#define HAS_FEATURE_CAMERA_HARDWARE // define this to get Camera Capture views.
#define FEATURE_CAMERA_HARDWARE 1	  // Mitac version 1 camera
*/

// One2One Parrot module
#define HAS_FEATURE_ONE2ONE
#define FEATURE_ONE2ONE 1

// ****************************************************************************
// * End of Feature Flags for PRODUCT_TITAN
// ****************************************************************************

// ****************************************************************************
// * Start of Feature Flags for PRODUCT_ENDEAVOUR
// ****************************************************************************
#elif defined(PRODUCT_ENDEAVOUR) // Cook (iCN 720) and Scott (iCN 750)

/*
// CAMERA
#define HAS_FEATURE_CAMERA_HARDWARE // define this to get Camera Capture views.
#define FEATURE_CAMERA_HARDWARE 1	
*/

// ****************************************************************************
// * End of Feature Flags for PRODUCT_ENDEAVOUR
// ****************************************************************************

#else

//#pragma message("Product settings moved to device.dll")

#endif


#else // !TARGET

// ****************************************************************************
// * Start of Feature Flags for LINUX SIMULATOR
// ****************************************************************************
#ifdef _POSIX

#define HAS_FEATURE_NAVPIX
#define FEATURE_NAVPIX 1	

// Has Camera
#define HAS_FEATURE_CAMERA          // define this to get Image browser views
#define FEATURE_CAMERA 1	

// CUSTOM POI
#define HAS_FEATURE_CUSTOM_POI
#define FEATURE_CUSTOM_POI 1

// POI ICON CACHE
#define HAS_FEATURE_POI_ICON_CACHE
#define FEATURE_POI_ICON_CACHE 1

// Mileage Recording
#define HAS_FEATURE_MILEAGE
#define FEATURE_MILEAGE 1

// Automatic screen capture on the PC simulator
#define HAS_AUTO_SCREEN_CAPTURE
#define AUTO_SCREEN_CAPTURE 1

/*
// TRAFFIC
#define HAS_FEATURE_TRAFFIC
#define FEATURE_TRAFFIC 1
*/

// BUTTONLESS_ID
#define HAS_FEATURE_BUTTONLESS_ID
#define FEATURE_BUTTONLESS_ID 1

// ****************************************************************************
// * End of Feature Flags for LINUX SIMULATOR
// ****************************************************************************
#endif // _POSIX

#endif // !TARGET

#endif // _PRODUCT_FEATURES_H_
