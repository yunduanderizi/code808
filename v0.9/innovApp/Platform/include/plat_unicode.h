#pragma once
#ifndef __PLAT_UNICODE_H
#define __PLAT_UNICODE_H

BOOL sys_findNextFilename(LPWSTR p_wstrFile, UINT8 p_nMaxFilenumber = 99);
BOOL sys_dumpPrintLog(LPCWSTR p_wstrFile);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _POSIX
#define FILE_ATTRIBUTE_DIRECTORY	1
#define FILE_ATTRIBUTE_HIDDEN		2
#endif

/** This structure is used by the os_findfirst, os_findnext and 
    os_finddone functions.  IT IS A DIRECT MAPPING OF THE 
    WINDOWS 'ffblk' STRUCTURE AND IT IS USED IN THE SAME WAY.
    
    @todo THIS STRUCTURE SHOULD REALLY BE ELIMINATED AND THE 
    PLATFORM LAYER CHANGED TO ENUMERATE FILES IN A MUCH TIDIER 
    WAY.  THE NAME OF THIS STRUCTURE DOES NOT FOLLOW THE TXyz 
    NAMING CONVENTION. */
typedef struct OS_FILE_DATA_W
{
   INT32  reserved;
   /** file size */
   INT32  size;
   /** a file-attribute byte used in selecting eligible files for 
       the search. attrib should be selected from the following 
       constants defined in dos.h:
       - FA_RDONLY: Read-only attribute
       - FA_HIDDEN: Hidden file
       - FA_SYSTEM: System file
       - FA_LABEL: Volume label
       - FA_DIREC: Directory
       - FA_ARCH: Archive
       
       @par
       A combination of constants can be ORed together.
       @par
       FOR MORE DETAILED INFORMATION ABOUT THESE ATTRIBUTES REFER 
       TO YOUR OPERATING SYSTEM DOCUMENTATION. */
   UINT32 attrib;
   /** contains bit fields for referring to the current time:
       - Bits 0 to 4: The result of seconds divided by 2 (for example 
       10 here means 20 seconds)
       - Bits 5 to 10: Minutes
       - Bits 11 to 15: Hours */
   UINT32 time;
   /** contains bit fields for referring to the current date:
       - Bits 0-4: Day
       - Bits 5-8: Month
       - Bits 9-15: Years since 1980 (for example 9 here means 1989) */
   UINT32 date;
   /** found file name */
   WCHAR  filename[256];
} OS_FILE_DATA_W;


void os_wprintf(LPCWSTR p_strFormat, ... );
OS_FILE os_wfopen(LPCWSTR p_strFilename, LPCWSTR p_strMode);
UINT32 os_wfdelete(LPCWSTR p_strFilename);
UINT32 os_wfindfirstfilter(LPCWSTR p_strPath, LPCWSTR p_strFilter, OS_FILE_DATA_W *p_pFileData);
UINT32 os_wfindnextfilter(LPCWSTR p_strFilter, OS_FILE_DATA_W *p_pFileData);
void os_wfinddone(OS_FILE_DATA_W *p_pFileData);

#define os_wfindfirst(p_strPath, p_pFileData) os_wfindfirstfilter(p_strPath, NULL, p_pFileData)
#define os_wfindnext(p_pFileData)             os_wfindnextfilter(NULL, p_pFileData)

BOOL8 os_wFileExists(LPCWSTR p_strFilename);
UINT32 os_getFileSizeFromHandle(OS_FILE p_hFile);
UINT32 os_wGetFileSize(LPCWSTR p_strFilename);
BOOL8 os_wfcopy(LPCWSTR p_strDest, LPCWSTR p_strSource, BOOL8 p_bReplace);
BOOL8 os_wMoveFile(LPCWSTR p_strSource, LPCWSTR p_strDest);

#ifdef __cplusplus
}
#endif

#endif // __PLAT_UNICODE_H




