#pragma once
#ifndef _LOADDLL_H
#define _LOADDLL_H

#ifdef __cplusplus

namespace navman {

class CLoadDll
{
public:
  CLoadDll();
  CLoadDll(LPCWSTR p_wstrFile);
  virtual ~CLoadDll();

  BOOL loadLibrary(LPCWSTR p_wstrFile);
  void *getProcAddress(LPCWSTR p_wstrFunction) const;
  void freeLibrary(void);
  const WCHAR *loadString(UINT32 p_nID) const;

  BOOL isLibraryLoaded(void) { return (m_hDll != NULL); }

private:
  WCHAR m_wstrFile[MAX_PATH];
#ifdef _POSIX
  void *m_hDll;
  const WCHAR *const *m_hStringTable;
#else
  HINSTANCE m_hDll;
#endif
};

} // namespace navman

#endif // __cplusplus

#endif // _LOADDLL_H
