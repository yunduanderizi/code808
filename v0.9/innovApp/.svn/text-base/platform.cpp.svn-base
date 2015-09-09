#include <innovEngine_sdk.h>
#include <locale.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>

#ifdef __linux__
#include <stdarg.h>
#include <errno.h>
#include <sys/timeb.h>
#include <sys/io.h>
#include <assert.h>
#define os_assert(test) assert(test)
#endif 

int    g_nError;
static FILE *debug = NULL;
static UINT8 *cache = NULL;


int SCREEN_WIDTH;
int SCREEN_HEIGHT; /* physical */


#ifdef PRINTF_LOG_FORCE_FLUSH
#define LOG_OUTPUT_THRESHOLD (0)
#else
#define LOG_OUTPUT_THRESHOLD (8192)
#endif

#define SIZEOF_PRINTF_LOG    (2*8192)
#define UINT32_MAX ((UINT32)0xFFFFFFFFUL)
#define MAP_CACHE_SIZE (16*1024*1024)
BOOL8 printfLogInitialized = FALSE;
char    printf_log[SIZEOF_PRINTF_LOG];
char   *printf_log_tailp;
OS_FILE os_logFile = NULL;

UINT32 os_fakeTime = UINT32_MAX;
static void *engineMapCache = NULL;
static UINT32 engineMapCacheSize = MAP_CACHE_SIZE;

void flushPrintfLog(void);
void os_closeLogFile(void);

void initializePrintfLog(void)
{
	printf_log_tailp = printf_log;
	printfLogInitialized = TRUE;
}

extern "C" void os_printf(const char *format,...)
{
  va_list argptr;
  UINT32 ret;

  if (!printfLogInitialized) {
    initializePrintfLog();
  }

  va_start(argptr, format);
  ret = vsprintf(printf_log_tailp, format, argptr);
  va_end(argptr);
  printf_log_tailp += ret;
  assert((printf_log_tailp - printf_log) < SIZEOF_PRINTF_LOG);

  /* have we collected enough data to flush to output? */
  if ((printf_log_tailp - printf_log) >= LOG_OUTPUT_THRESHOLD) {
    // if a log file is open, write log message into it
    flushPrintfLog();
  }

}

void flushPrintfLog(void)
{
	// if a log file is open, write log message into it
	if ( os_logFile ) {
		os_fwrite(printf_log, (UINT32)(printf_log_tailp - printf_log), os_logFile);
	} else {
		fwrite(printf_log, 1, printf_log_tailp - printf_log, stderr);
	}
	printf_log_tailp = printf_log; /* reset the log's tail */
}

UINT32 os_fwrite( const void *buffer, UINT32 nbytes, OS_FILE stream ) {
#ifdef WINCE
	DWORD nWritten = 0;

	if (!WriteFile((HANDLE)stream, buffer, nbytes, &nWritten, NULL))
	{
		DWORD dwError = GetLastError();
		LOG_ERROR(0, ("os_fwrite(0x%x, %d, 0x%x) failed, error %d\n", buffer,
			nbytes, stream, dwError));
		return 0;
	}

	return nWritten;
#else
	/* not WINCE */
	return (UINT32)fwrite( buffer, 1, (long)nbytes, (FILE*)stream );
#endif /* #ifdef WINCE */
}

void os_exit_( const char *fileName, const int lineNumber, const UINT8 rc ) {
	if (rc) {
		LOG_FATAL( 0, ( "os_exit(%d): In file ('%s') at line %d\n",
			rc, fileName, lineNumber ) );
	}

	// Close all opened cached file descriptors.
	io_flushCache();
#ifdef HAS_SDL
	{
		int k;
		for( k = 0; k < MAX_FONT_SIZE + 1; k++ ) {
			if( fonts[k] ) {
				TTF_CloseFont( fonts[k] );
				fonts[k] = NULL;
			}
		}

		if( TTF_WasInit() ) { TTF_Quit(); }
	}
	SDL_Quit();
#endif

	// close log file (if it was opened)
	os_closeLogFile();

	// causes system to crash and generate a back trace
#ifdef WINCE
	if ( rc ) {
		printf( "os_exit(%d): In file ('%s') at line %d\n", rc, fileName, lineNumber );
		printf( "Please press a key to continue assert()...\n" );
		getchar();
	}
#endif
	assert( rc == 0 );
	if (rc) {
		exit(rc);
	}
}

void os_closeLogFile(void)
{
	flushPrintfLog();
	if (os_logFile) {
		os_fclose(os_logFile);
		os_logFile = NULL;
	}
}

UINT32 os_fclose(
				 OS_FILE stream )
{
#ifdef WINCE
	DWORD dwError;
	if (!CloseHandle((HANDLE)stream))
	{
		dwError = GetLastError();
		LOG_WARN(5, ("os_fclose(0x%x) failed, error %d\n", stream, dwError));
		return dwError;
	}

	return 0;
#else
	/* not WINCE */
	int status;

	if( !stream ) {
		LOG_WARN( 5, ( "warning: os_flose() called with NULL" ) );
		return 1;
	}

	status = fclose( (FILE*)stream );
	if( status ) {
		LOG_WARN( 5, ( "warning: fclose failed (%d errno=%d) (%s)",
			status, errno, strerror(errno) ) );
	}

	return (UINT32)status;
#endif /* #ifdef WINCE */
}

OS_FILE os_fopen(
				 const char *filename,
				 const char *mode )
{
#ifdef WINCE
	DWORD access;
	DWORD createDisp;
	BOOL8 seekToEnd;
	HANDLE hFile;
	UTF16 wFileName[128];

	assert( filename );

	seekToEnd = FALSE;
	access = GENERIC_READ;
	createDisp = OPEN_EXISTING;

	if ((strcmp(mode, "w") == 0) || (strncmp(mode, "wb", 2) == 0))
	{
		access |= GENERIC_WRITE;
		createDisp = CREATE_ALWAYS;
	}
	else if (strcmp(mode, "a") == 0)
	{
		seekToEnd = TRUE;
		access |= GENERIC_WRITE;
		createDisp = OPEN_ALWAYS;
	}
	else if (strncmp(mode, "r+", 2) == 0)
	{
		access |= GENERIC_WRITE;
		createDisp = OPEN_EXISTING;
	}
	else if (strncmp(mode, "w+", 2) == 0)
	{
		access |= GENERIC_WRITE;
		createDisp = CREATE_ALWAYS;
	}
	else if (strncmp(mode, "a+", 2) == 0)
	{
		seekToEnd = TRUE;
		access |= GENERIC_WRITE;
		createDisp = OPEN_ALWAYS;
	}

	language_UTF8ToUTF16(wFileName, (UTF8*)filename);
	hFile = CreateFile(wFileName,
		access,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		createDisp,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile != INVALID_HANDLE_VALUE) {
		// seek to the end of the file when the appropriate mode was specified
		if (seekToEnd) {
			if ((SetFilePointer(hFile, 0, NULL, FILE_END) == (DWORD)-1) &&
				(GetLastError() != NO_ERROR)) {
					LOG_WARN(5, ("os_fopen(%s, %s): fopen() failed, error %d\n",
						filename, mode, GetLastError()));
					CloseHandle(hFile);
					return (OS_FILE)NULL;
			}
		}
	} else {
		LOG_WARN(5, ("os_fopen(%s, %s): fopen() failed, error %d\n",
			filename, mode, GetLastError()));
		hFile = NULL;
	}

	return (OS_FILE)hFile;

#else
	/* not WINCE */
	FILE* retVal;

	errno = 0;
	assert( filename );

	retVal = fopen( filename, mode );
	if( retVal == NULL ) {
		LOG_WARN( 5, ( "Failed to open file \"%s\" with error \"%s\"",
			filename, strerror(errno) ) );
	}

	return (OS_FILE)retVal;
#endif /* #ifdef WINCE */
}

UINT32 os_fread( void *buffer, UINT32 nbytes, OS_FILE stream ) {
#ifdef WINCE
	DWORD nRead = 0;


#if 0 /* may need to be enabled for profiling */
	UINT32 _time;
	/* dump out the stack leading to the os_fread */
	{
		UINT32 nframes;
#define MAX_STACK_FRAMES 12
		CallSnapshot stack_buffer[MAX_STACK_FRAMES];

		os_printf("os_fread(%u): ", nbytes);
		nframes = os_backtrace((void **)stack_buffer, MAX_STACK_FRAMES);
		if (nframes > 1) {
			UINT32 i;
			for (i = 1; i < nframes; i++) {
				os_printf("[<%p>] ", stack_buffer[i].dwReturnAddr);
			}
		}
	}

	os_printf("os_fread(%u): ", nbytes);
	_time = os_rdtsc();
#endif /* #if 0 */

	if (!ReadFile((HANDLE)stream, buffer, nbytes, &nRead, NULL)) {
		LOG_ERROR(0, ("os_fread(0x%p, %lu, 0x%lx) failed, error %lu\n", buffer,
			nbytes, (long)stream, GetLastError()));
		return 0;
	}

#if 0 /* may need to be enabled for profiling */
	os_printf(": %u\n", os_rdtsc() - _time);
#endif

	return nRead;
#else
	/* not WINCE */
	return (UINT32)fread( buffer, 1, (long)nbytes, (FILE*)stream );
#endif /* #ifdef WINCE */
}


UINT64 os_rdtsc( void )
{
#ifdef _WIN32
	LARGE_INTEGER lInt;
	if (QueryPerformanceCounter(&lInt)) {
		return lInt.QuadPart;
	}
	return 0;
#elif (defined(__linux__) && !defined(__LP64__))
#if 0
	/* 32-bit GNU/Linux */

	// from IAPERF.H
	// Author: Michael Stoner
	// Company: Intel
	// Page: http://www.intel.com/cd/ids/developer/asmo-na/eng/microprocessors/itanium/19949.htm?page=3
	unsigned long long counter;
	__asm__ __volatile__ (
		"cpuid\n\t"
		"rdtsc\n\t"
		"leal %0, %%ecx\n\t"
		"movl %%eax, (%%ecx)\n\t"
		"movl %%edx, 4(%%ecx)\n\t"
		:
	: "m" (counter)
		: "%eax", "%ebx", "%ecx", "%edx");

	return counter;
#endif
#else
	return 0;
#endif
}

UINT32 os_fseek( OS_FILE stream, INT32 fileOffset, UINT32 seekMode ) {
#ifdef WINCE
	HANDLE  hFile;
	DWORD   pos;
	DWORD   nReturn;

	hFile = (HANDLE)stream;

	switch (seekMode)
	{
	case SEEK_CUR:
		pos = FILE_CURRENT;
		break;

	case SEEK_END:
		pos = FILE_END;
		break;

	case SEEK_SET:
		pos = FILE_BEGIN;
		break;

	default:
		assert(0);
		pos = FILE_BEGIN;
	}

	nReturn = SetFilePointer(hFile, fileOffset, NULL, pos);
	if (nReturn == (DWORD)-1)
	{
		DWORD dwError = GetLastError();
		LOG_ERROR(0, ("os_fseek(0x%x, 0x%x, %d) returned %d, error %d\n", stream,
			fileOffset, seekMode, nReturn, dwError));
		return dwError;
	}

	return 0;
#else
	/* not WINCE */
	int status;

	status = fseek( (FILE*)stream, (long)fileOffset, (int)seekMode );
	if( status ) {
		LOG_WARN( 5, ( "warning: fseek(%p %d %d) failed, errno=%d (%s)",
			stream, fileOffset, seekMode, errno, strerror(errno) ) );
	}

	return (UINT32)status;
#endif /* #ifdef WINCE */
}

UINT32 os_time(void) 
{
#ifndef WINCE
	struct timeb t;
	if ( os_fakeTime != UINT32_MAX ) {
		return os_fakeTime;
	}
	ftime(&t);
	return ( (UINT32)t.time * 1000 ) + (UINT32)t.millitm;
#else
	if ( os_fakeTime != UINT32_MAX ) {
		return os_fakeTime;
	}
	return timeGetTime();
#endif
}

BOOL8 os_getMapCache(UINT8 **cacheAddress, UINT32 *cacheSize) {
 if (engineMapCache == NULL) {
    engineMapCache = os_mallocLocal(engineMapCacheSize, NULL, NULL, 0);
  } else {
    assert(engineMapCacheSize > 0);
    memset(engineMapCache, 0, engineMapCacheSize);
  }

  if (engineMapCache == NULL) {
    return FALSE;
  }

  *cacheAddress = (UINT8*)engineMapCache;
  *cacheSize = engineMapCacheSize;

  return TRUE;
}

void * os_mallocLocal(size_t blockSize, void * hint, const char *strFile, unsigned int nLine)
{

	return malloc(blockSize);

}

void os_freeLocal(void *blockPtr, const char *strFile, unsigned int nLine)
{

	free(blockPtr);

}

void * os_reallocLocal(void * blockPtr, size_t newBlockSize, void * hint, const char *strFile, unsigned int nLine)
{

	return realloc(blockPtr, newBlockSize);

}


UINT64 os_uint64(const UINT8 *address) {
	return
		(UINT64)address[0]        |
		((UINT64)address[1] << 8)  |
		((UINT64)address[2] << 16) |
		((UINT64)address[3] << 24) |

		((UINT64)address[4] << 32) |
		((UINT64)address[5] << 40) |
		((UINT64)address[6] << 48) |
		((UINT64)address[7] << 56);
}

UINT32 os_uint32( const UINT8 *address ) {
	return
		( (UINT32)address[0]       ) |
		( (UINT32)address[1] <<  8 ) |
		( (UINT32)address[2] << 16 ) |
		( (UINT32)address[3] << 24 );
}

UINT32 os_uint24(const UINT8 *address) {
	return
		( (UINT32)address[0]       ) |
		( (UINT32)address[1] <<  8 ) |
		( (UINT32)address[2] << 16 );
}

UINT16 os_uint16(const UINT8 *address) {
	return
		( (UINT16)address[0]       ) |
		( (UINT16)address[1] <<  8 );
}

void os_pos24(
			  INT32 *x, INT32 *y,
			  POS24 *address )
{
	UINT32 temp;

	// 3 bytes of position data
	temp  =           ((UINT8*)address)[0];
	temp |= ( (UINT32)((UINT8*)address)[1] ) << 8;
	temp |= ( (UINT32)((UINT8*)address)[2] ) << 16;

	// 12 bit signed positions, assumes 32 bit little endian micro
	*x = ((INT32)(temp << 20)) >> 20;
	*y = ((INT32)(temp <<  8)) >> 20;
}

UINT64 os_rdtsf( void )
{
#ifdef _WIN32
	LARGE_INTEGER lInt;
	if (QueryPerformanceFrequency(&lInt)) {
		return lInt.QuadPart;
	}
	return 1000;
#elif (defined(__linux__) && !defined(__LP64__))
	return 2667000000UL; /* approximation of our 2.66 GHz desktops */
#else
	return 1000;
#endif
}

INT32 os_ftell( OS_FILE stream ) {
#ifdef WINCE
	HANDLE  hFile;
	DWORD dwPtr;
	DWORD dwError;

	hFile = (HANDLE)stream;
	dwPtr = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
	if ((dwPtr == 0xFFFFFFFF) && ((dwError = GetLastError()) != NO_ERROR)) {
		LOG_WARN( 5, ( "warning: ftell(%p) failed, errno=%d", stream, dwError ) );
		return 0;
	}
	return dwPtr;

#else
	/* not WINCE */
	return (INT32)ftell( (FILE*)stream );
#endif /* #ifdef WINCE */
}

void os_playSpeechPrompt(UINT32 tokenList[],
						 UINT32 numTokens,
						 TSpeechPriority priority) {
}

void os_getUnitID( UINT8 *buf, UINT32 *len, UINT32 maxlen )
{
	// hard coded unit ID at the moment
	strcpy( (char*)buf, "TEST UNIT" );
	*len = 9;
}

BOOL8 os_fIsRemoveable( const char *filename, UINT8 *buf, UINT32 *buflen ) {
	return FALSE;
}

extern "C" void platform_setScreenSize(int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

#if defined(__linux__)
void gnusim_assert(const char *file, INT32 line, const char *condition)
{
  os_printf("Mapcore assertion fail: File %hs, Line %d: %hs\n", file, line, condition);
  os_assert(0);
}
#endif
