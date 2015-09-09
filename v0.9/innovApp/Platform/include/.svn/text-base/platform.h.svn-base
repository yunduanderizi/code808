#ifndef PLATFORM_H
#define PLATFORM_H

/*--------------------------------------------------------------------------*/
/*
  This file contains the interface to the platform support layer that
  sits underneath both the map engine and NAVMAN embedded GUI applications.
  The support layer has a different implementation for each target
  platform. */
/*--------------------------------------------------------------------------*/

#include "platform_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------*/
/** @addtogroup Platform
    These functions abstract away map engine accesses to system
    resources such as memory, and sound.  These functions are
    required in order to make the map core operate - i.e. the
    bare minimum platform support requirement.  The implementations
    of the functions are user supplied and are platform specific. */
/*----------------------------------------------------------------*/
/**@{ start of group */

#ifndef INT16_MAX
/** the greatest value representable by an #INT16 */
#define INT16_MAX ((INT16)0x7FFF)
#endif

#ifndef INT16_MIN
/** the least value representable by an #INT16 */
#define INT16_MIN (-INT16_MAX-1)
#endif

#ifndef UINT16_MAX
/** the greatest value representable by a #UINT16 */
#define UINT16_MAX ((UINT16)0xFFFFU)
#endif

#ifndef INT32_MAX
/** the greatest value representable by an #INT32 */
#define INT32_MAX ((INT32)0x7FFFFFFFL)
#endif

#ifndef INT32_MIN
/** the least value representable by an #INT32 */
#define INT32_MIN (-INT32_MAX-1)
#endif

#ifndef UINT32_MAX
/** the greatest value representable by a #UINT32 */
#define UINT32_MAX ((UINT32)0xFFFFFFFFUL)
#endif

// The following 64-bit constants are valid C99 but certain compilers
// won't accept them.  Appropriate versions of the 64-bit constants
// should be defined in platform_types.h for these compilers.

#ifndef INT64_MAX
/** the greatest value representable by an #INT64 */
#define INT64_MAX ((INT64)0x7FFFFFFFFFFFFFFFLL)
#endif

#ifndef INT64_MIN
/** the least value representable by an #INT64 */
#define INT64_MIN (-INT64_MAX-1)
#endif

#ifndef UINT64_MAX
/** the greatest value representable by a #UINT64 */
#define UINT64_MAX ((UINT64)0xFFFFFFFFFFFFFFFFULL)
#endif

/** this value can be used in the seekMode parameter of the
    os_fseek function. */
#define OS_SEEK_SET 0
/** this value can be used in the seekMode parameter of the
    os_fseek function. */
#define OS_SEEK_CUR 1
/** this value can be used in the seekMode parameter of the
    os_fseek function. */
#define OS_SEEK_END 2

/** Packed position data contained in 24 bits. */
typedef UINT8 POS24[3];

/** @deprecated
    This structure is not used. */
typedef struct OS_FILE_DATA {
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
   char  filename[256];
} OS_FILE_DATA;


/** Speech prompt priority. This is used to indicate the urgency of the
    information contained in a speech prompt. In the event that an attempt is
    made to speak a prompt while a previous one is still playing, the previous
    one may be suspended or allowed to finish based on the priority of each
    prompt. */
typedef enum {
  /** The prompt requires no action. E.g. "Continue for 20 kilometres" or
      "There is slow traffic ahead". */
  SPEECH_PRIORITY_LOW,
  /** The prompt requires action. E.g. Short/medium range guidance such as
      "In 600 metres turn left". */
  SPEECH_PRIORITY_MEDIUM = 50,
  /** The prompt requires immediate action. E.g. close range guidance such as
      "Turn Left" or "Exit Right". */
  SPEECH_PRIORITY_HIGH = 100
} TSpeechPriority;


/** OS_POS24 is an optimised version of os_pos24.  os_pos24 is
    performance critical so a macro OS_POS24 is also provided which
    avoids function call overhead and should also perform less memory
    accesses than os_pos24() function because x, y don't need to be
    passed by address when using a macro.  Since it is a macro,
    optimiser can also optimise code better in the caller (x, y
    can be registers for example). */

#ifndef PLATFORM_BIG_ENDIAN
/* Little endian implementation */
#define OS_POS24(x, y, address)                               \
  { UINT32 _tmp;                                              \
    UINT8  *_address = (UINT8 *)address;                      \
    _tmp = ((UINT32)(_address[0])      ) |                    \
           ((UINT32)(_address[1]) <<  8) |                    \
           ((UINT32)(_address[2]) << 16);                     \
    /* Shift bits with signed integer to preserve sign bit */ \
    x = ((INT32)(_tmp << 20)) >> 20;                          \
    y = ((INT32)(_tmp <<  8)) >> 20;                          \
  }
#else
#define OS_POS24(x, y, address) os_pos24(&(x), &(y), (address))
#endif

/** OS_UINT24 is an optimised version of os_uint24.  os_uint24 is
    performance critical so a macro OS_UINT24 is also provided which
    avoids function call overhead and should also perform less memory
    accesses than os_uint24() function because x, y don't need to be
    passed by address when using a macro.  Since it is a macro,
    optimiser can also optimise code better in the caller (x, y
    can be registers for example). */

#ifndef PLATFORM_BIG_ENDIAN
#define OS_UINT24(address)                       \
  /* Little endian implementation */             \
  ( ((UINT32)(((UINT8 *)(address))[0])      ) |  \
    ((UINT32)(((UINT8 *)(address))[1]) <<  8) |  \
    ((UINT32)(((UINT8 *)(address))[2]) << 16)    \
  )
#else
#define OS_UINT24(address) os_uint24(address)
#endif

/** OS_UINT32 is an optimised version of os_uint32.  os_uint32 is
    performance critical so a macro OS_UINT32 is also provided which
    avoids function call overhead.

    Since it is a macro, avoid passing expressions as argument
    because it can cause side effects or might be inefficient. */

#ifndef PLATFORM_BIG_ENDIAN
/* Little endian implementation */
#define OS_UINT32(address)                       \
  ( ((UINT32)(((UINT8 *)(address))[0])      ) |  \
    ((UINT32)(((UINT8 *)(address))[1]) <<  8) |  \
    ((UINT32)(((UINT8 *)(address))[2]) << 16) |  \
    ((UINT32)(((UINT8 *)(address))[3]) << 24)    \
  )
#else
#define OS_UINT32(address) os_uint32(address)
#endif

/** OS_UINT16 is an optimised version of os_uint16.  os_uint16 is
    performance critical so a macro OS_UINT16 is also provided which
    avoids function call overhead.

    Since it is a macro, avoid passing expressions as argument
    because it can cause side effects or might be inefficient. */

#ifndef PLATFORM_BIG_ENDIAN
/* Little endian implementation */
#define OS_UINT16(address)                       \
  ( ((UINT16)(((UINT8 *)(address))[0])      ) |  \
    ((UINT16)(((UINT8 *)(address))[1]) <<  8)    \
  )
#else
#define OS_UINT16(address) os_uint16(address)
#endif

/*----------------------------------------------------------------*/


/** This function returns a 16-bit integer stored at a specified memory
    address (the address does not have to be on a 16 bit boundary).

    @param address Pointer to a byte address in memory that the desired
    16-bit value is stored at.

    @return 16-bit integer stored at the specified address. */
extern MAPCORE_API UINT16 os_uint16( const UINT8 *address );

/** This function returns a 64-bit integer stored at a specified memory
    address (the address does not have to be on a 64 bit boundary).

    @param address Pointer to a byte address in memory that the desired
    64-bit value is stored at.

    @return 64-bit integer stored at the specified address. */
extern MAPCORE_API UINT64 os_uint64( const UINT8 *address );

/** This function returns a 32-bit integer stored at a specified memory
    address (the address does not have to be on a 32 bit boundary).

    @param address Pointer to a byte address in memory that the desired
    32-bit value is stored at.

    @return 32-bit integer stored at the specified address. */
extern MAPCORE_API UINT32 os_uint32( const UINT8 *address );


/** This function returns a 24-bit integer stored at a specified memory
    address (the address does not have to be on a 16 or 32 bit boundary).

    @param address Pointer to a byte address in memory that the
    desired 24-bit value is stored at.

    @return 24-bit integer stored at the specified address. */
extern MAPCORE_API UINT32 os_uint24( const UINT8 *address );


/** This function unpacks 24-bit position coordinate data into two
    32-bit integers representing the x, y position stored in the
    packed coordinate data structure.

    @param x Pointer to a 32-bit integer which will contain the x
    coordinate of the packed position data pointed to by the
    address parameter.

    @param y Pointer to a 32-bit integer which will contain the y
    coordinate of the packed position data pointed to by the
    address parameter.

    @param address Pointer to a POS24 structure containing packed
    position data to be decoded. The POS24 structure is three bytes
    in size and is defined as follows (in platform_types.h):
    typedef UINT8 POS24[3]; */
extern MAPCORE_API void os_pos24( INT32 *x,
                                  INT32 *y,
                                  POS24 *address );


/** This macro returns the a byte contents of the specified
    location.

    @param x a Pointer to the location containing the required data.

    @return UINT8 value contained at the specified location. */
#define os_uint8(x) ( *((UINT8*)(x)) )

//@TPE get the size of available physical memory(KB)
extern MAPCORE_API UINT32 Get_AvailPhys( void );

/** This function retrieves the current value of the internal 32-bit
    OS counter converted to milliseconds.

    @return 32-bit counter value. */
extern MAPCORE_API UINT32 os_time( void );

/** This function (shortnamed for read-timestamp-counter) retrieves
    the current value of the internal 64-bit high-resolution hardware
    performance counter. The frequency of the counter may be obtained
    from os_rdtsf().

    This functionality is only implemented for platforms used for
    benchmarking and internal testing. A reference implementation can
    be obtained from Mapcore's internal test framework.

    @return 64-bit counter value. */
extern MAPCORE_API UINT64 os_rdtsc( void );

/** This function retrieves the frequency of the internal 64-bit high
    resolution hardware counter. The frequency can be used to
    translate differences in the counter's values into real time.

    This functionality is only implemented for platforms used for
    benchmarking and internal testing. A reference implementation can
    be obtained from Mapcore's internal test framework.

    @return 64-bit counter frequency. */
extern MAPCORE_API UINT64 os_rdtsf( void );

#ifdef PROFILE_ENABLED
#define PROFILE_FUNCTION_ENTER UINT32 _time = os_rdtsc();
#define PROFILE_FUNCTION_EXIT                                   \
  os_printf("%s: %d\n", __FUNCTION__, os_rdtsc() - _time);
#else  /* PROFILE_ENABLED */
#define PROFILE_FUNCTION_ENTER /* empty */
#define PROFILE_FUNCTION_EXIT  /* empty */
#endif /* PROFILE_ENABLED */

/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API void os_init( void )
);


/** This function terminates all ongoing operations and closes down the
    system. Specific behaviour is platform-dependent.
    @param fileName The source filename in which the fault occurred.
    @param lineNumber The source line number at which os_exit() was invoked.
    @param rc Return code that will be posted to any underlying operating
    system on program termination.

    @return (the map engine does not expect this function to return, although
    on some platforms specific reason codes may cause os_exit to return). */
extern MAPCORE_API void os_exit_( const char *fileName,
                                  const int lineNumber,
                                  const UINT8 rc );


/** This macro invokes the os_exit_() platform function call with the
    name of the source file and the line number at which the the os_exit()
    was invoked.
    @param rc The exit code to be returned. */
#define os_exit(rc) os_exit_(__FILE__,__LINE__,(rc))


/** This function requests the allocation of an object with at least blockSize
    bytes of storage available.  If the space is available, os_malloc returns
    a pointer to the newly allocated block as its result.

    @note A blockSize of 0 bytes is possible.

    @param blockSize Specifies the size of the memory block required in bytes.

    @param strFile The name of the file in which this allocation occurred.

    @param nLine The line number at which this allocation occurred.

    @return A 32 bit aligned pointer to some memory allocated from the system
    heap otherwise NULL if the allocation was not possible. */
extern MAPCORE_API void *os_mallocLocal(size_t blockSize, void *hint, const char *strFile, unsigned int nLine);


/** This macro invokes the #os_mallocLocal platform function call with the
    name of the source file and the line number at which the the #os_malloc
    was invoked.

    @param blockSize Specifies the size of the memory block required in bytes.
    
    @param hint      Specifies a hint address. */
#define os_malloc(blockSize, hint) os_mallocLocal(blockSize, hint, __FILE__, __LINE__);


/** This function requests more or less space for an object previously 
    allocated by #os_malloc.

    @param blockPtr A pointer to the original block allocated using #os_malloc.

    @param newBlockSize The size of the new block to be reallocated.

    @param strFile The name of the file in which this reallocation occurred.

    @param nLine The line number at which this reallocation occurred.

    @return A 32 bit aligned pointer to the reallocated memory block that
    guarantees the integrity of the contents of the beginning of the original
    object.  NULL is returned if the requested newBlockSize cannot be allocated
    from the system heap. */
extern MAPCORE_API void *os_reallocLocal(void *blockPtr, size_t newBlockSize, void *hint, const char *strFile, unsigned int nLine);


/** This macro invokes the #os_reallocLocal platform function call with the
    name of the source file and the line number at which the the #os_realloc
    was invoked.

    @param blockSize Specifies the size of the memory block required in bytes.
    
    @param hint      Specifies a hint address. */
#define os_realloc(blockPtr, newBlockSize, hint) os_reallocLocal(blockPtr, newBlockSize, hint, __FILE__, __LINE__);


/** This function returns memory previously allocated via #os_malloc to the system
    heap.

    @note The blockPtr must have been allocated with #os_malloc.  The behaviour
    of an attempt to free memory not allocated via #os_malloc using os_free 
    is implementation specific.

    @param blockPtr A pointer to a memory block previously allocated via #os_malloc.

    @param strFile The name of the file in which this free occurred.

    @param nLine The line number at which this free occurred.
*/
extern MAPCORE_API void os_freeLocal(void *blockPtr, const char *strFile, unsigned int nLine);


/** This macro invokes the #os_freeLocal platform function call with the
    name of the source file and the line number at which the the #os_free
    was invoked.

    @param blockSize Specifies the size of the memory block required in bytes. */
#define os_free(blockPtr) os_freeLocal(blockPtr, __FILE__, __LINE__);


/** This function retrieves a pointer to the start address of the map
    cache area together with the allocated size.

    @note
    The map cache needs to be at least 1 MByte in size, the larger the
    cache, the better performance will be.

    @param cacheAddress Pointer to a pointer to a byte that, on return
    will contain the address of the first byte in the map cache
    memory area.

    @param cacheSize Pointer to a 32-bit integer that, on return,
    will contain the size (in bytes) currently allocated to the
    map cache.

    @return TRUE if a map cache area is available, FALSE if an
    allocation operation failed or other errors were encountered. */
extern MAPCORE_API BOOL8 os_getMapCache( UINT8 **cacheAddress,
                                         UINT32 *cacheSize );


/** Opens a file from the non-volatile storage filesystem.

    @param filename Pointer to a null-terminated ASCII string containing
    the name of the file to be opened.

    @param mode Type of access permitted. Can be any one of the
    following:
    - "r" Opens for reading.  If the file does not exist or cannot be
    found, the os_fopen call fails.
    - "w" Opens an empty file for writing.  If the given file exists, its
    contents are destroyed.
    - "a" Opens for writing at the end of the file (appending); creates
    the file first if it doesn't exist.
    - "r+" Opens for both reading and writing.  The file must
    exist.
    - "w+" Opens an empty file for both reading and writing.  If the
    given file exists, its contents are destroyed.
    - "a+" Opens for reading and appending; creates the file first if
    it doesn't exist.  When a file is opened with the "a" or "a+" access
    type, all write operations occur at the end of the file. The file
    pointer can be repositioned using os_fseek, but is always moved back
    to the end of the file before any write operation is carried out.
    Thus, existing data cannot be overwritten.

    @return Pointer to the open file structure (OS_FILE). If the file open
    operation failed, a NULL pointer is returned. */
extern MAPCORE_API OS_FILE os_fopen( const char *filename,
                                     const char *mode );


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API UINT32 os_fdelete( const char *filename )
);


/** Closes an opened file.

    @param stream Pointer to the OS_FILE structure containing the previously
    opened file handle.

    @return Zero if the file was successfully closed, nonzero if an error
    occurred. */
extern MAPCORE_API UINT32 os_fclose( OS_FILE stream );


/** Moves the file pointer in an opened file to a specified position.

    @param stream Pointer to the OS_FILE structure containing the opened
    file handle.

    @param fileOffset 32-bit integer containing the offset (in bytes) to
    move the file pointer. A negative value will cause the file pointer
    to be moved backward.

    @param seekMode Can be set to either of the following values:
    - OS_SEEK_SET The fileOffset parameter will be interpreted as an
    offset from the beginning of the file
    - OS_SEEK_END The fileOffset parameter is ignored and the file
    pointer is placed at the end of the file.

    @return Zero if the file pointer was moved successfully, otherwise
    nonzero. */
extern MAPCORE_API UINT32 os_fseek( OS_FILE stream,
                                    INT32 fileOffset,
                                    UINT32 seekMode );


/** Retrieve the current position of the file pointer.

    @note On some operating systems depending on how the file was
    originally opened, os_ftell may not reflect the physical byte offset
    for streams opened in text mode because of CR/LF translation.

    @param stream Pointer to the OS_FILE structure containing the opened
    file handle.

    @return Returns the current file position.  On error -1 is returned,
    the error could have been caused by the fact that the stream was not
    a valid OS_FILE or that an invalid stream was passed in. */
extern MAPCORE_API INT32 os_ftell( OS_FILE stream );


/** Reads a specified number of bytes from an opened file, starting at
    the current file pointer position.

    @param buffer Pointer to a buffer which will contain the data read
    from the file upon return.

    @param nbytes 32-bit integer containing the number of bytes the
    application wants to read out of the file.

    @param stream Pointer to an OS_FILE structure containing an opened
    file handle.

    @return Number of bytes read from the file and placed into the
    supplied buffer.  This may be less than the number specified in the
    nbytes parameter. */
extern MAPCORE_API UINT32 os_fread( void *buffer,
                                    UINT32 nbytes,
                                    OS_FILE stream );


/** Writes a specified number of bytes from a buffer to an opened file
    at the current file pointer position.

    @param buffer Pointer to a buffer containing the data to be written
    to the file.

    @param nbytes Number of bytes to write starting at the byte pointed
    to by the buffer parameter.

    @param stream Pointer to an OS_FILE structure containing the
    previously opened file handle.

    @return Number of bytes actually written to the file (this may be
    less than the amount requested in the nbytes parameter). */
extern MAPCORE_API UINT32 os_fwrite( const void *buffer,
                                     UINT32 nbytes,
                                     OS_FILE stream );


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API UINT32 os_findfirst( const char *path,
                                        OS_FILE_DATA *fileData )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API UINT32 os_findnext( OS_FILE_DATA *fileData )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API void os_finddone( OS_FILE_DATA *fileData )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API UINT32 os_getFileSize( const char *filename )
);


/** This function checks to see if a given map file is currently on a
    removable device.  If the device is found to be removable it
    will attempt to obtain a unique identifier for the removable
    storage unit, and return that information.

    @param filename Filename of full path to mapfile.

    @param buf Buffer to place unique removable ID if found.

    @param buflen Length of buffer allocated.  Will be set to 0 if
    ID not found. */
extern MAPCORE_API BOOL8 os_fIsRemoveable( const char *filename,
                                           UINT8 *buf,
                                           UINT32 *buflen );


/** This function checks to see if a given map file is allowed for
    use by the platform's secure 'offline authentication'
    regionalised system. Platforms that do not support secure offline
    regionalised authentication should stub this function out to
    always return TRUE.

    @param chunkHash 20-byte SHA1 identifying hash of a .chk file.

    @return TRUE if file is usable, FALSE if not. */
extern MAPCORE_API BOOL8 os_regionVerifyChunkHash( UINT8 *chunkHash );


/** This function gets the unique unit ID of the platform device.

    @param buf Pointer to pre-allocated buffer to store the serial
    number of the hardware device.

    @param len Pointer to UINT32 object to store the returned length
    of the serial number.

    @param maxlen Maximum size of the buffer to place the serial
    number in to. */
extern MAPCORE_API void os_getUnitID( UINT8 *buf,
                                      UINT32 *len,
                                      UINT32 maxlen );


/** Outputs formatted text. The actual output destination is
    platform-dependent (e.g., on-screen, logfile output, etc).

    @param format Pointer to a printf-style null-terminated ASCII string
    containing format specifiers to be matched against the other
    parameters provided to os_printf.

    @param ... Variable list of parameters to be matched against the
    format specifiers in the string pointed to by the format
    parameter. */
extern MAPCORE_API void os_printf( const char *format, ... )
#ifdef __GNUC__
     /*
      * gcc makes it possible to check that the format matches
      * argument types at compilation time so use that feature
      * to catch bugs at compilation time
      */
     __attribute__ ((format(printf, 1, 2)))
#endif
     ;


/** @deprecated
    This should be implemented as a stub function. */
DEPRECATED(
extern MAPCORE_API UINT8 os_getScreenDepth( void )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API BOOL8 os_setScreenDepth( UINT8 depth )
);


/** @deprecated
    This should be implemented as a stub function. */
DEPRECATED(
extern MAPCORE_API UINT8 *os_getScreenMem( void )
);


/** @deprecated
    This should be implemented as a stub function. */
DEPRECATED(
extern MAPCORE_API void os_getScreenSize( UINT32* width,
                                          UINT32* height )
);


/** Play a speech prompt in the currently selected language. A prompt consists
    of one or more speech tokens combined to form a meaningful phrase. For
    example, the token list 66, 1, 29, 4, 31 might represent the spoken
    instruction "Continue / Two / Point / Five / Kilometres".

    Speech tokens are usually stored in speech data files. Speech data files
    may be decoded using the #TSpeechFile utility API.

    @note
    The token numbers are dependent on the current language code and
    version settings. These must be set using #language_setSpeechVersion and
    #language_setLanguageCode.

    @param[in] tokenList List of speech token numbers.

    @param[in] numTokens The number of tokens in the speech token list.

    @param[in] priority An indication of the urgency of the spoken
    information.*/
extern MAPCORE_API void os_playSpeechPrompt( UINT32 tokenList[],
                                             UINT32 numTokens,
                                             TSpeechPriority priority );


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API BOOL8 os_soundInit( const char *fname,
                                       UINT16 *desired_language,
                                       UINT16 *version )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API void os_soundShutDown( void )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API BOOL8 os_getSpeechInfo( const char *fname,
                                           UINT16 *version,
                                           UINT16 *language,
                                           char *description )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API void os_soundAddBuffer( const char *data, UINT32 size )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API void os_speechAddBuffer( UINT16 sampleNum )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API void os_soundClearBuffer( void )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API void os_soundPlayBuffer( void )
);


/** @deprecated
    This function is not used. */
DEPRECATED(
extern MAPCORE_API UINT8 os_writeTrafficSerial (char ch)
);


/**@} end of group */

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------------------------------*/

#endif /* PLATFORM_H */

/* End of platform.h */
