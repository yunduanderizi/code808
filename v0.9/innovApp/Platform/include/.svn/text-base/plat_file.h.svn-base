#pragma once
#ifndef _PLAT_FILE_H_
#define _PLAT_FILE_H_

namespace navman
{

struct CFileStatus
{
  time_t  m_ctime;          // creation date/time of file
  time_t  m_mtime;          // last modification date/time of file
  time_t  m_atime;          // last access date/time of file
  LONG    m_size;            // logical size of file in bytes
  BYTE    m_attribute;       // logical OR of CFile::Attribute enum values
  WCHAR   m_wstrFullName[MAX_PATH]; // absolute path name
};

class COSFile
{
public:
  // Constructors
  COSFile();
  COSFile(LPCSTR p_strFileName, LPCSTR p_strMode);
  COSFile(LPCWSTR p_wstrFileName, LPCWSTR p_wstrMode);
  COSFile(LPCSTR p_strFileName, UINT p_nOpenFlags);
  COSFile(LPCWSTR p_wstrFileName, UINT p_nOpenFlags);
  // Destructor
  virtual ~COSFile();

public:
  // Flag values
  enum OpenFlags
  {
    modeRead =          0x0000,
    modeWrite =         0x0001,
    modeReadWrite =     0x0002,
    shareCompat =       0x0000,
    shareExclusive =    0x0010,
    shareDenyWrite =    0x0020,
    shareDenyRead =     0x0030,
    shareDenyNone =     0x0040,
    modeNoInherit =     0x0080,
    modeCreate =        0x1000,
    modeNoTruncate =    0x2000,
    typeText =          0x4000, // typeText and typeBinary are used in
    typeBinary =   (int)0x8000 // derived classes only
  };

  enum Attribute
  {
    normal =    0x00,
    readOnly =  0x01,
    hidden =    0x02,
    system =    0x04,
    volume =    0x08,
    directory = 0x10,
    archive =   0x20
  };

  enum SeekPosition
  {
    begin =   0x0,
    current = 0x1,
    end =     0x2
  };

  // Attributes
  operator OS_FILE() const { return m_hFile; }
  BOOL isOpen() { return (m_hFile != NULL); }

  DWORD GetPosition() const;
  BOOL GetFileName(LPSTR p_strGetName, UINT p_nNoOfChars) const;
  BOOL GetFileName(LPWSTR p_wstrGetName, UINT p_nNoOfChars) const;
  BOOL GetFilePath(LPSTR p_strGetName, UINT p_nNoOfChars) const;
  BOOL GetFilePath(LPWSTR p_wstrGetName, UINT p_nNoOfChars) const;
  void SetFilePath(LPCSTR p_strNewName);
  void SetFilePath(LPCWSTR p_wstrNewName);

  // Operations
  BOOL Open(LPCSTR p_strFileName, LPCSTR p_strMode);
  BOOL Open(LPCWSTR p_wstrFileName, LPCWSTR p_wstrMode);
  BOOL Open(LPCSTR p_strFileName, UINT p_nOpenFlags);
  BOOL Open(LPCWSTR p_wstrFileName, UINT p_nOpenFlags);

  DWORD SeekToEnd() { return this->Seek(0, end); }
  void SeekToBegin() { this->Seek(0, begin); }

  LONG Seek(LONG p_lOff, SeekPosition p_eFrom);
  DWORD GetLength() const;

  UINT Read(void *p_lpBuf, UINT p_nCount);
  UINT Write(const void *p_lpBuf, UINT p_nCount);

  void Flush();
  void Close();

  BOOL AreContentsTheSame(COSFile *p_pFile);

  BOOL Rename(LPCSTR p_strNewName);
  BOOL Rename(LPCWSTR p_wstrNewName);
  BOOL Remove(void);
  BOOL GetStatus(CFileStatus& rStatus);
  BOOL SetStatus(const CFileStatus& status);

  // Static functions
  static BOOL Rename(LPCSTR p_strOldName, LPCSTR p_strNewName);
  static BOOL Rename(LPCWSTR p_wstrOldName, LPCWSTR p_wstrNewName);
  static BOOL Remove(LPCSTR p_strFileName);
  static BOOL Remove(LPCWSTR p_wstrFileName);
  static BOOL GetStatus(LPCWSTR p_wstrFileName, CFileStatus& rStatus);
  static BOOL SetStatus(LPCWSTR p_wstrFileName, const CFileStatus& status);

  static BOOL GetDiskSize(LPCWSTR p_wstrDisk, ULARGE_INTEGER *p_pnAvailable, ULARGE_INTEGER *p_pnTotal, ULARGE_INTEGER *p_pnFree);

protected:
  OS_FILE m_hFile;
  WCHAR m_wstrFile[MAX_PATH];
};


class COSBufferOutputFile : public COSFile
{
public:
  // Constructors
  COSBufferOutputFile();
  COSBufferOutputFile(LPCSTR p_strFileName, LPCSTR p_strMode, UINT p_nBufferSize = 1024);
  COSBufferOutputFile(LPCWSTR p_wstrFileName, LPCWSTR p_wstrMode, UINT p_nBufferSize = 1024);
  COSBufferOutputFile(LPCSTR p_strFileName, UINT p_nOpenFlags, UINT p_nBufferSize = 1024);
  COSBufferOutputFile(LPCWSTR p_wstrFileName, UINT p_nOpenFlags, UINT p_nBufferSize = 1024);
  // Destructor
  virtual ~COSBufferOutputFile();

  BOOL SetBufferSize(UINT p_nBufferSize);
  UINT Write(const void *p_lpBuf, UINT p_nCount);
  void Flush();

protected:
  UINT8 *m_pBuffer;
  UINT  m_nBufferSize;
  UINT  m_nBufferOffset;
};


} // namespace navman

#endif // _PLAT_FILE_H_
