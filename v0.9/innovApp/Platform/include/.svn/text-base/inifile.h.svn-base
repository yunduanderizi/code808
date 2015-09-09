#pragma once
/*****************************************************************************
 *  (c) 2002 navman new zealand limited. all rights reserved.
 *
 *  module name: inifile.h
 *
 *  author(s): Paul Sinclair <psinclair@navman.com>
 *
 *  description:
 *    this module read and writes to ini files
 *****************************************************************************/

#ifndef _APPS_INIFILE_H_
#define _APPS_INIFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

  /** @file
  This module provides an API for reading from and writing to ini files.
  */

/*****************************************************************************
 * TYPEDEFS, STRUCTS, UNIONS
 *****************************************************************************/

typedef enum {
  INIFILE_READ,
  INIFILE_WRITE
} EIniFileOpenMode;

typedef enum {
  PARAMTYPE_UNKNOWN,
  PARAMTYPE_STRING,
  PARAMTYPE_INT,
  PARAMTYPE_BOOL
} EIniFileParamType;

/*****************************************************************************
 * GLOBAL FUNCTION DECLARATIONS
 *****************************************************************************/

void inifile_setPath(const char *p_cstrPath);
void inifile_setWPath(const WCHAR *p_wstrcPath);
BOOL8 inifile_open(EIniFileOpenMode p_eMode);
void inifile_close(void);

BOOL8 inifile_getSection(UINT16 p_nSectionIndex, char *p_strSectionName, UINT p_nSectionNameLength);

BOOL8  inifile_getParamType(const char *p_cstrSection,
                            const char *p_cstrParamName,
                            EIniFileParamType *p_eParamType);

BOOL8  inifile_getString(const char *p_cstrSection,
                          const char *p_cstrParamName,
                          char *p_strParamValue,
                          int p_nParamLength);
BOOL8  inifile_getInt(const char *p_cstrSection,
                      const char *p_cstrParamName,
                      INT32 *p_nParamValue);
BOOL8  inifile_getBool(const char *p_cstrSection,
                        const char *p_cstrParamName,
                        BOOL8 *p_bParamValue);

BOOL8  inifile_putString(const char *p_cstrSection,
                          const char *p_cstrParamName,
                          const char *p_cstrParamValue);
BOOL8  inifile_putInt(const char *p_cstrSection,
                      const char *p_cstrParamName,
                      const INT32 p_nParamInt);
BOOL8  inifile_putBool(const char *p_cstrSection,
                        const char *p_cstrParamName,
                        const BOOL8 p_bParamValue);

INT32 inifile_getIntValue(const char *p_cstrValue);

#define inifile_getPrivateProfileString(p_cstrSection, p_cstrParamName, p_strParamValue, p_nParamLength)  inifile_getString(p_cstrSection, p_cstrParamName, p_strParamValue, p_nParamLength)
#define inifile_getPrivateProfileInt(p_cstrSection, p_cstrParamName, p_nParamValue) inifile_getInt(p_cstrSection, p_cstrParamName, p_nParamValue)
#define inifile_getPrivateProfileBool(p_cstrSection, p_cstrParamName, p_bParamValue)  inifile_getBool(p_cstrSection, p_cstrParamName, p_bParamValue)
#define inifile_putPrivateProfileString(p_cstrSection, p_cstrParamName, p_cstrParamValue) inifile_putString(p_cstrSection, p_cstrParamName, p_cstrParamValue)
#define inifile_putPrivateProfileInt(p_cstrSection, p_cstrParamName, p_nParamInt) inifile_putInt(p_cstrSection, p_cstrParamName, p_nParamInt)
#define inifile_putPrivateProfileBool(p_cstrSection, p_cstrParamName, p_bParamValue)  inifile_putBool(p_cstrSection, p_cstrParamName, p_bParamValue)

/******************************************************************************/

  /*----------------------------------------------------------------*/
  /** @addtogroup inif
  This is an alternative interface for reading ini files in a much more
  efficient and often more intuitive fashion than the Windows style
  'GetPrivateProfileXXX' API.
  @par
  The idea here is to enumerate through the sections and keys of the ini
  file in the order that they occur in the file. This method utilises
  callbacks to handle the sections and keys in the file.
  @par
  Observe the following example code:
  @code
    BOOL8 key_callback(TIniFile *ini, const char *section, const char *key, const char *value) {
      // todo: process this key
    }
    BOOL8 section_callback(TIniFile *ini, const char *section) {
      // todo: initialise data structure to hold data for this section
      inif_enumKeys(ini, section, key_callback);
      // todo: perform action on data for this section
    }
    void test_inif(char *filename) {
      TIniFile ini;
      if(inif_openIniFile(&ini, filename)) {
        inif_enumSections(&ini, section_callback);
        inif_closeIniFile(&ini);
      } else {
        printf("File not found: %hs\n", filename);
      }
    }
  */
  /*----------------------------------------------------------------*/
  /**@{ start of group */

  /** This structure defines an instance of an ini file enumerator class. All
  fields should be considered private and unavailable to the appliacation.*/
typedef struct TIniFile {
  OS_FILE fid; // file handle for the open inifile
#ifdef _VS7_SIM
  unsigned // vs7 asserts if c functions like strcmp see negative ascii codes
#endif
  char buf[2048];     // internal file cache - unsigned would please vs
  int buf_ptr;        // cursor position within cache
  int buf_left;       // how much data is in the cache
  char section[128];  // current section name
} TIniFile;

  /** This is the form of the section callback function that must be supplied
  to inif_enumSections.
  @param ini Pointer to an ini file enumerator object.
  @param section Name of this section.
  @return Return TRUE if you are happy to continue with the enumeration,
  otherwise return FALSE (e.g. if you have run out of memory for storing the
  data). */
typedef BOOL8 TIniFileSectionCallback(TIniFile *p_pIni,
                                     const char *p_cstrSection);

  /** This is the form of the key callback function that must be supplied
  to inif_enumKeys.
  @param ini Pointer to an ini file enumerator object.
  @param section Name of the section currently being enumerated for keys.
  @param key Name of this key.
  @param value Value of this key.
  @return Return TRUE if you are happy to continue with the enumeration,
  otherwise return FALSE (e.g. if you have found the key we were looking for,
  or if you have run out of memory for storing the data). */
typedef BOOL8 TIniFileKeyCallback(TIniFile *p_pIni,
                                 const char *p_cstrSection,
                                 const char *p_cstrKey,
                                 const char *p_cstrValue);

  /** Open an ini file for reading.
  @param ini Pointer to the ini file enumerator object.
  @param filename Name of the ini file to be read.
  @return TRUE if the ini file was opened successfully. FALSE if the file
  could not be found, or for any other reason could not be opened. */
BOOL8 inif_openIniFile(TIniFile *p_pIni, const char *p_cstrFilename);

  /** This function enumerates all sections in the ini file from start to
  finish.
  @param ini Pointer to the ini file enumerator object.
  @param callback This callback function is called once for each section
  found in the file.
  @return TRUE if the ini file sections were successfully enumerated. Will
  probably never return FALSE. */
BOOL8 inif_enumSections(TIniFile *p_pIni, TIniFileSectionCallback *p_pCallback);

  /** This function enumerates all keys in the current section. This
  function should only ever be called from inside the section callback
  handler.
  @param ini Pointer to the ini file enumerator object.
  @param section The name of the current section.
  @param callback This callback function is called once for each key found
  in the section.
  @return TRUE if the section's keys were successfully enumerated. Will
  probably never return FALSE. */
BOOL8 inif_enumKeys(TIniFile *p_pIni, const char *p_cstrSection, TIniFileKeyCallback *p_pCallback);

  /** Close the ini file after we have finished enumerating it's contents.
  @param ini Pointer to the ini file enumerator object.
  @return TRUE if the ini file was closed successfully. Probably will never
  return FALSE. */
BOOL8 inif_closeIniFile(TIniFile *p_pIni);

  /**@} end of group */
  /*----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif // _APPS_INIFILE_H_
