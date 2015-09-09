#pragma once
/*****************************************************************************
 *  (c) 2005 - 2007 navman new zealand limited. all rights reserved.
 *
 *  module name: plat_paths.h
 *
 *  author(s): Tom Isaacson <tisaacson@navman.com>
 *
 *  description:
 *    This module 
 *****************************************************************************/

#ifndef _APPPATHS_H_
#define _APPPATHS_H_

#include "registry.h"
#include "plat_types.h"

BOOL sys_getPaths(TDeviceType p_eDeviceType,
                  LPWSTR p_wstrRoot, UINT p_nRootLength,
                  LPWSTR p_wstrSDCard, UINT p_nSDCardLength,
                  LPWSTR p_wstrMain, UINT p_nMainLength);
BOOL sys_isSDCardDriverName(LPCWSTR p_wstrName);

#ifdef UNICODE

// PLATFORM_DIR_SEPARATOR "\\"
#ifndef PLATFORM_DIR_SEPARATOR_W
  #ifdef _POSIX
    #define PLATFORM_DIR_SEPARATOR_W  L"/"
  #else
    #define PLATFORM_DIR_SEPARATOR_W  L"\\"
  #endif
#endif

#define SEP_W                           PLATFORM_DIR_SEPARATOR_W

#define FLASH_DIR_GSL_W                 SEP_W L"DiskOnChip"
#define FLASH_DIR_MITAC_W               SEP_W L"My Flash Disk"
#define FLASH_DIR_KINPO_W               SEP_W L"DataStorage"

#ifdef _POSIX
#define SDCARD_DIR_W                    SEP_W L"usr"
#else
#define SDCARD_DIR_W                    SEP_W L"Storage Card"
#endif
#define HDD_DIR_W                       SEP_W L"HDD"
#define PROGRAM_DIR_W                   SEP_W L"Program Files"

#define NAVMAN_W                        L"Navman"
#define SMARTST_CE_W                    L"SmartST_CE"

#define IAPPLIANCE_DIR_W                L"iAppliance"

//#define NAVMAN_DIR_W                    PROGRAM_DIR_W SEP_W NAVMAN_W
//#define APP_DIR_W                       NAVMAN_DIR_W SEP_W L"SmartST_CE"

//#define APP_NAVMAN_DIR_W                APP_DIR_W SEP_W NAVMAN_W

//#define INSTALL_FILE_NAME_W             L"newinstall.ini"
//#define APP_INSTALL_FILE_W              NAVMAN_DIR_W SEP_W INSTALL_FILE_NAME_W

#define DEFAULT_APPSTARTUPSEC_FILE_NAME_W       L"AppStartupSec.exe"
//#define APP_APPSTARTUPSEC_FILE_W        NAVMAN_DIR_W SEP_W APPSTARTUPSEC_FILE_NAME_W

#define EXE_FILE_NAME_W                 L"SmartST_CE.exe"
//#define APP_EXE_FILE_W                  APP_DIR_W SEP_W EXE_FILE_NAME_W

#ifdef DEBUG
  #define EXEDBG_FILE_NAME_W            L"SmartST_CEDbg.exe"
//  #define APP_EXEDBG_FILE_W             SEP_W EXEDBG_FILE_NAME_W
#endif

#define DEBUG_FILE_W                    L"debug.ini"

#define PATHS_XML                       L"paths.xml"

#define DEVICE_SETTINGS_INI               L"device_settings.ini"
#define DEVICE_SETTINGS_DEFAULT_SECTION   "Default"
#define DEVICE_SETTINGS_HWVERSION_SECTION "HWVersion=%d"

// Favourites Directory and paths
#define FAVOURITES_DIR_W                USER_DATA_DIR_W
#define USER_DATA_DIR_W                 L"UserData"
#define DATA_DIR_W                      L"Data"

// Setting Directory and paths
#define SETTINGS_DIR_W                  USER_DATA_DIR_W

// Defaults for paths.xml
#define DEFAULT_APPSTARTUPSEC_DIR       PROGRAM_DIR_W SEP_W NAVMAN_W
#define DEFAULT_SMARTST_DIR             PROGRAM_DIR_W SEP_W NAVMAN_W SEP_W SMARTST_CE_W
#define DEFAULT_DATABASE_DIR            SEP_W NAVMAN_W SEP_W L"database" SEP_W L"database.db"
#define DEFAULT_DATABASE_PHOTOS_DIR     SEP_W NAVMAN_W SEP_W L"photos" SEP_W
#define DEFAULT_DATABASE_ICONS_DIR      SEP_W NAVMAN_W SEP_W L"icons" SEP_W
#define DEFAULT_MILEAGE_LOG_DIR         SEP_W MY_DOCUMENTS_DIR_W SEP_W
#define DEFAULT_AUTHENTICATION_DIR      PROGRAM_DIR_W SEP_W NAVMAN_W SEP_W SMARTST_CE_W
#define DEFAULT_AVOID_AREAS_DIR         PROGRAM_DIR_W SEP_W NAVMAN_W SEP_W SMARTST_CE_W SEP_W USER_DATA_DIR_W
#define DEFAULT_FAVOURITES_DIR          PROGRAM_DIR_W SEP_W NAVMAN_W SEP_W SMARTST_CE_W SEP_W USER_DATA_DIR_W
#define DEFAULT_MULTISTOP_ROUTES_DIR    PROGRAM_DIR_W SEP_W NAVMAN_W SEP_W SMARTST_CE_W SEP_W USER_DATA_DIR_W
#define DEFAULT_POI_SETTINGS_DIR        PROGRAM_DIR_W SEP_W NAVMAN_W SEP_W SMARTST_CE_W SEP_W USER_DATA_DIR_W
#define DEFAULT_RECENTS_DIR             PROGRAM_DIR_W SEP_W NAVMAN_W SEP_W SMARTST_CE_W SEP_W USER_DATA_DIR_W
#define DEFAULT_SETTINGS_DIR            PROGRAM_DIR_W SEP_W NAVMAN_W SEP_W SMARTST_CE_W SEP_W USER_DATA_DIR_W
#define DEFAULT_ALERT_SOUNDS_DIR        PROGRAM_DIR_W SEP_W NAVMAN_W SEP_W SMARTST_CE_W SEP_W L"UI" SEP_W L"Sounds"
#define DEFAULT_MAP_DATA_DIR            SEP_W NAVMAN_W

#define DEFAULT_SDCARD_AUTO_DIR         L"Default"

#ifndef UNDER_CE
#define SDCARD_DRIVER_NAME              L"Test string for PC"
#endif // !UNDER_CE

#else // !UNICODE

#error Unicode not defined

#endif // !UNICODE

#endif // _APPPATHS_H_
