#pragma once
/** @file
\brief Provides access to settings and paths.

(c) 2007 navman technology new zealand ltd. all rights reserved.
*/

#ifndef __APPPATHS_H__
#define __APPPATHS_H__

#include "plat_types.h"
#include "plat_paths.h" // for now

namespace navman
{
  /**
  Provides access to settings and paths shared with Desktop.
  */
  class CAppPaths
  {
    public:
      bool construct(void);

      CAppPaths(void);
      ~CAppPaths(void);

#ifndef TARGET
      void SetDeviceName(LPCWSTR p_wstrDevice, int p_nHWVersion = 0);
      void SetDeviceType(TDeviceType p_eDevice, int p_nHWVersion = 0);
#endif
      void SetMainPath(LPCWSTR p_wstrMainPath, LPCWSTR p_wstrRoot);
      void SetSDCardPath(LPCWSTR p_wstrPath);

      LPCWSTR GetDevicePath(void);
      TDeviceType GetDeviceType(void);

      void GetRoot(LPWSTR p_wstrBuffer, int p_nBufferLength) const;
      void GetMainPath(LPWSTR p_wstrBuffer, int p_nBufferLength) const;
      void GetSDCardPath(LPWSTR p_wstrBuffer, int p_nBufferLength) const;
      void GetMainMapPath(LPWSTR p_wstrBuffer, int p_nBufferLength);
      void GetSDCardMapPath(LPWSTR p_wstrBuffer, int p_nBufferLength) const;

      LPCWSTR GetRoot(void) const;
      LPCWSTR GetMainPath(void) const;
      LPCWSTR GetSDCardPath(void) const;
      LPCWSTR GetMainMapPath(void);
      LPCWSTR GetSDCardMapPath(void) const;

      int GetRoot(LPWSTR p_wstrBuffer, int p_nBufferLength, LPCWSTR p_wstrFormat, ...) const;
      int GetMainPath(LPWSTR p_wstrBuffer, int p_nBufferLength, LPCWSTR p_wstrFormat, ...) const;
      int GetSDCardPath(LPWSTR p_wstrBuffer, int p_nBufferLength, LPCWSTR p_wstrFormat, ...) const;
      int GetMainMapPath(LPWSTR p_wstrBuffer, int p_nBufferLength, LPCWSTR p_wstrFormat, ...);
      int GetSDCardMapPath(LPWSTR p_wstrBuffer, int p_nBufferLength, LPCWSTR p_wstrFormat, ...) const;

      BOOL IsSDCardDriverName(LPCWSTR p_strName) const;
      BOOL IsCameraPresent(void) const;

    private:
      WCHAR m_wstrDeviceName[20];
      TDeviceType m_eDeviceType;
      int   m_nHWVersion;
      WCHAR m_wstrInternalPath[MAX_PATH];
      WCHAR m_wstrExternalPath[MAX_PATH];
      WCHAR m_wstrMainPath[MAX_PATH];
      WCHAR m_wstrMainMapPath[MAX_PATH];
      WCHAR m_wstrSDCardMapPath[MAX_PATH];

      LPCWSTR GetDeviceName(void);
  }; // class CAppPaths

  /**
  CAppPaths singleton
  */
  CAppPaths *AppPaths(void);

} // namespace navman 


#define MAIN_PATH_W                     AppPaths()->GetMainPath()
#define SDCARD_PATH_W                   AppPaths()->GetSDCardPath() !! not used anywhere

#define USER_SETTINGS_XML               L"user_settings.xml"
#define DEFAULT_SETTINGS_XML            L"default_settings.xml"
#define SKU_SETTINGS_XML                L"sku_settings.xml"
#define LOCALE_SETTINGS_XML             L"_settings.xml" // locale fills in prefix lang code
#define MANIFEST_PROFILE_XML            L"manifest.xml"
#define DEVICE_IMAGE_XML                L"splash_image.xml"
#define DEFAULT_LOCALE_SETTINGS_XML     L"en-gb_settings.xml" // locale fills in prefix lang code
#define SOUND_SETTINGS_XML              L"sound_settings.xml"

#define MY_FAV_LOCATIONS_XML            L"MyFavouriteLocations.xml"
#define MY_RECENT_LOCATIONS_XML         L"MyRecentLocations.xml"
#define MY_IAPP_LOCATIONS_XML           L"MySavedLocations.xml"
#define MY_MULTISTOP_LOCATION_XML       L"MyMultiStopLocations.xml"
#define MY_AVOIDAREAS_XML               L"MyAvoidAreas.xml"
#define MY_ROUTE_XML                    L"MyRoute.xml"
#define MY_POIDATA_XML                  L"poitypes.xml"
#define MY_POIALERTS_XML                L"poialerts.xml"

#define SHOP_DEMO_XML                   L"shop_demo.xml"
#define SUBSCRIPTION_PREF_XML           L"subscription_preferences.xml"

#define SOUND_DIR_W                     L"Sound"
#define SOUND_FILE_EXT_W                L"*.dat"

#define MAPCOLS_DIR_W                   L"MapCols"

#define MASTERKEY_FILE_NAME_W           L"masterkey.dat"    // it is the combine cdkeys from all the different sources of cdkey
#define CDKEYS_FILE_NAME_W              L"cdkeys.dat"
#define SIGREG_FILE_NAME_W              L"sigreg.dat"
#define CUSTOMPOI_FILE_NAME_W           L"custompoi.xml"

#define BITMAPS_DIR_W                   L"Bitmaps"                                                        !! not used anywhere
#define MK_BMPFILE(file)                AppPaths()->GetMainPath( BITMAPS_DIR_W SEP_W L## #file L".bmp" )  !! not used anywhere

#define FILENAME_SAVED_DEST_FIELDS_W    USER_DATA_DIR_W SEP_W L"destdata.dat"
#define DESTWIZ_RECENT_ROAD_SEARCH      USER_DATA_DIR_W SEP_W L"dwrecentroad.xml"
#define DESTWIZ_RECENT_PLACE_SEARCH     USER_DATA_DIR_W SEP_W L"dwrecentplace.xml"
#define DESTWIZ_RECENT_POCODE_SEARCH    USER_DATA_DIR_W SEP_W L"dwrecentpocode.xml"
#define DESTWIZ_RECENT_AREA_SEARCH      USER_DATA_DIR_W SEP_W L"dwrecentarea.xml"
#define DESTWIZ_CURRENT_CONTINENT       USER_DATA_DIR_W SEP_W L"dwcurrentcontinent.xml"

#define MY_DOCUMENTS_DIR_W              SEP_W L"My Documents" SEP_W
#define PICTURES_DIR_W                  SEP_W L"My Documents" SEP_W L"My Pictures" SEP_W
#define NEW_PICTURES_DIR_NAME_W         L"NavPix"
#define NEW_PICTURES_DIR_W              PICTURES_DIR_W NEW_PICTURES_DIR_NAME_W
#define PHOTO_LOCATION_LIST_FILENAME_W  L"photolocationlist.xml"
#define PICTURES_STORE_C_W              AppPaths()->GetRoot()
#define PICTURES_STORE_A_W              AppPaths()->GetSDCardPath()
#define DEF_PICTURESTORE                NEW_PICTURES_DIR_W SEP_W L"ICN"
#define DEF_FAKEPHOTO                   L"debug" SEP_W L"test_photo.jpg"

#define PICTURES_FILTER_BMP_W           L"*.bmp"
#define PICTURES_FILTER_JPG_W           L"*.jpg"
#define PICTURES_FILTER_BM_W            L"*.bm"

#define PICTURES_TEMPLATE_W             L"template.jpg"

// This thumbnail string is determined by SmartST Desktop
#define THUMBNAIL_SEARCH_STRING         L"_thumb.jpg"

#define FAVOURITE_IMAGE_DIR_W           L"image" SEP_W

#define PLATFORM_LANGUAGE_INI_FILE      L"language.ini"
#define GPS_LOG_INI_W                   L"GPSLog.ini"

#define TIMEZONE_DATABASE_FILE_W        L"date_time_zonespec.csv"

#define TUTORIAL_FILE_W                 Locale()->getString( ID_STRING_TUTORIAL_XMLPATH )
#define ABOUT_FILE_W                    Locale()->getString( ID_STRING_ABOUT_XMLPATH )

#define STN_LIST_FILE_W                 L"stnlist.dat"

#define CONCAT_3_STRINGS                L"%ls%ls%ls"

#define TMC_STATION_FILE_W              CONCAT_3_STRINGS, DATA_DIR_W, SEP_W, STN_LIST_FILE_W

#define MILEAGE_LOG_FILE_NAME           L"MyMileage.xml"

#define TTS_DATA_DIR                    L"TTSData"

#define SOUND_SETTINGS_FILE_W           CONCAT_3_STRINGS, USER_DATA_DIR_W, SEP_W, SOUND_SETTINGS_XML

#endif // __APPPATHS_H__
