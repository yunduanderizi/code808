#pragma once
#ifndef _XMLDEVICEFILE_H_
#define _XMLDEVICEFILE_H_

typedef struct TUpgradeSettings
{
  UINT32  nMajor, nMinor, nBuild;
  BOOL    bForceUpgrade;
  WCHAR   wstrHWType[MAX_PATH];
  WCHAR   wstrHWVersion[MAX_PATH];
  WCHAR   wstrMemorySize[MAX_PATH];
  WCHAR   wstrStorageSize[MAX_PATH];
  WCHAR   wstrStorageType[MAX_PATH];
  BOOL    bFormatFlash;
  UINT32  nFlashMajor, nFlashMinor, nFlashBuild;
  WCHAR   wstrProjectCode[MAX_PATH];
  WCHAR   wstrSKUID[MAX_PATH];
  WCHAR   wstrMapVersion[MAX_PATH];
  WCHAR   wstrMapRegion[MAX_PATH];
} TUpgradeSettings;

INT32 getIntValue(LPCWSTR p_wstrValue);

BOOL createDeviceXMLFile(LPCWSTR p_wstrXmlFile);
BOOL createSDCardXMLFile(LPCWSTR p_wstrXmlFile);
BOOL createXMLFileSignature(LPCWSTR p_wstrXmlFile, LPCWSTR p_wstrSigFile, const BYTE *p_pbyPrivateKey, const DWORD p_dwPrivateKeySize);
BOOL getKernelXMLFileSettings(const WCHAR *p_wstrFile, TUpgradeSettings *p_pUpgradeSettings);
BOOL getUBootXMLFileSettings(const WCHAR *p_wstrFile, TUpgradeSettings *p_pUpgradeSettings);

#endif // !_XMLDEVICEFILE_H_
