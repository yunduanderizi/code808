#pragma once

#ifndef _PLAT_ALLOC_H
#define _PLAT_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif


// defines
// DEBUG_MEMORY_FUNCS to include heap checking and total mem counters
// SIMPLE_MEMORY_FUNCS to revert to standard malloc and free
// neither to use HeapAlloc without debug
// DEBUG_MEMORY_FUNCS_DETAIL to get output for every allocation and free

#define SHARED_MEM_START_ADDRESS 0x42000000

#ifdef _WIN32_WCE
//#ifdef HAS_FEATURE_SHARED_MEM_HEAP
//  #define THREAD_GUARD_SHARED_MEM_HEAP
//#endif
#define DEBUG_MEMORY_FUNCS
//#define DEBUG_MEMORY_LOGGING
//#define SIMPLE_MEMORY_FUNCS
//#define DEBUG_MEMORY_FUNCS_DETAIL
#elif defined(_POSIX)
#define SIMPLE_MEMORY_FUNCS
#else
#ifndef PROFILER
#define DEBUG_MEMORY_FUNCS
#define DEBUG_LEAK_CHECKER
#endif
//#define SIMPLE_MEMORY_FUNCS
//#define DEBUG_MEMORY_FUNCS_DETAIL
#endif

#ifndef DEBUG_MEMORY_FUNCS

#ifdef _WIN32_WCE
  extern PLATFORM_API void *os_bigMalloc(size_t p_Size);
  extern PLATFORM_API void os_bigFree(void *p_pPtr);
#else
  #define os_bigMalloc(nSize) os_malloc(nSize, NULL)
  #define os_bigFree os_free
#endif // _WIN32_WCE

#else



#ifdef _WIN32_WCE
  extern PLATFORM_API void *os_bigMallocLocal(size_t p_Size, const char *p_strFile, unsigned int p_nLine);
  extern PLATFORM_API void os_bigFreeLocal(void *p_pPtr, const char *p_strFile, unsigned int p_nLine);
  #define os_bigMalloc(nSize) os_bigMallocLocal(nSize, __FILE__, __LINE__)
  #define os_bigFree(pPtr) os_bigFreeLocal(pPtr, __FILE__, __LINE__)
#else
  #define os_bigMalloc(nSize) os_malloc(nSize, NULL)
  #define os_bigFree os_free
#endif // _WIN32_WCE

#endif // DEBUG_MEMORY_FUNCS

  extern PLATFORM_API UINT32 os_mallocSize(void *p_pPtr);
  extern PLATFORM_API void os_printMemoryStatus(const char *msg=NULL);

#ifdef DEBUG_MEMORY_LOGGING
  //captures memory information and message.
  extern PLATFORM_API bool os_captureMemory(WCHAR *msg=NULL);

  //prints the memory captures contained in the vector
  extern PLATFORM_API void os_printMemoryLog();

  //called from printMemoryLog to print out each memory status capture
  extern PLATFORM_API void os_printMemory(const wchar_t *,MEMORYSTATUS);

  //used to determine if two memory captures are the same or different (prevents log clogging)
  extern PLATFORM_API bool os_compareMemory(MEMORYSTATUS, MEMORYSTATUS);
#endif // DEBUG_MEMORY_LOGGING

#ifdef __cplusplus
}
#endif

#endif // _PLAT_ALLOC_H
