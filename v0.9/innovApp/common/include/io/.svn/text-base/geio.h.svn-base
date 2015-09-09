#ifndef GEIO_H
#define GEIO_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup IO
/// This library provides a generic I/O API layer which sits on top of
/// OS layer.  Its goal is to provides:
/// - I/O statistics per handle of global statistics
/// - Keep file opened and reuse previously opened handles
/// even when user access files with following pattern:
/// io_fopen/io_fread/io_fclose, io_fopen/io_fread/io_fclose (etc)
/// - To avoid overhead of calling os_*() functions.  os_*()
/// functions may have overhead on some platforms.
/// - Coalesce consecutive seeks without intermediate I/O
/// to again minimize calls to os level (os_fseek).
/// @ingroup Utilities
/// @{

/** Handle of a file used by io_*() api */
typedef void *IO_FILE;

/** Force-ably flush (i.e call os_fclose) all file descriptors that had
    previously been closed by io_fclose() but there were still cached (i.e. 
    not closed yet at os_ level by io lib).  This function can be called
    when using removable file systems (SD card for example) to avoid stale
    open file handles on a file system that no longer exists.  It is 
    harmless to call this function when no file system is unmounted other 
    than a performance cost since all cached opened file handle will be 
    flushed and further io_fopen() of these files will require a call to 
    os_fopen(). */
extern MAPCORE_API void io_flushCache( void );

/** Opens a file from the non-volatile storage filesystem.

    @param filename Pointer to a null-terminated ASCII string containing
    the name of the file to be opened.

    @param mode Type of access permitted. Can be any one of the following:
    - "r" Opens for reading.  If the file does not exist or cannot be 
    found, the os_fopen call fails.
    - "w" Opens an empty file for writing. If the given file exists, its 
    contents are destroyed.
    - "a" Opens for writing at the end of the file (appending); creates the 
    file first if it doesn't exist.
    - "r+" Opens for both reading and writing. (The file must exist.)
    - "w+" Opens an empty file for both reading and writing. If the given 
    file exists, its contents are destroyed.
    - "a+" Opens for reading and appending; creates the file first if it 
    doesn't exist.
    When a file is opened with the "a" or "a+" access type, all write 
    operations occur at the end of the file. The file pointer can be 
    repositioned using io_fseek, but is always moved back to the end of 
    the file before any write operation is carried out.  Thus, existing 
    data cannot be overwritten.

    @return Pointer to the open file structure (IO_FILE). If the file open
    operation failed, a NULL pointer is returned. */
extern MAPCORE_API IO_FILE *io_fopen( const char *filename,
                                      const char *mode );
  
/** Closes an opened file.

    @param fp Pointer to the IO_FILE structure containing the previously
    opened file handle.

    @return Zero if the file was successfully closed, nonzero if an error
    occurred. */
extern MAPCORE_API UINT32 io_fclose( IO_FILE *fp );

/** Reads a specified number of bytes from an opened file, starting at
    the current file pointer position.

    @param buffer Pointer to a buffer which will contain the data read from 
    the file upon return.

    @param n 32-bit integer containing the number of bytes the application
    wants to read out of the file.

    @param fp Pointer to an IO_FILE structure containing an opened file 
    handle.

    @return Number of bytes read from the file and placed into the supplied 
    buffer.  This may be less than the number specified in the nbytes 
    parameter. */
extern MAPCORE_API UINT32 io_fread( void *buffer,
                                    UINT32 n,
                                    IO_FILE *fp );

/** Writes a specified number of bytes from a buffer to an opened file at 
    the current file pointer position.

    @param buffer Pointer to a buffer containing the data to be written 
    to the file.

    @param nBytes Number of bytes to write starting at the byte pointed 
    to by the buffer parameter.

    @param fp Pointer to an IO_FILE structure containing the previously 
    opened file handle.

    @return Number of bytes actually written to the file (this may be less 
    than the amount requested in the nbytes parameter). */
extern MAPCORE_API UINT32 io_fwrite( const void *buffer,
                                     UINT32 nBytes,
                                     IO_FILE *fp );
  
/** Moves the file pointer in an opened file to a specified position.
    This function behaves like fseek().
    
    @param fp Pointer to the IO_FILE structure containing the opened
    file handle.

    @param offset 32-bit integer containing the offset (in bytes) to 
    move the file pointer. 

    @param seekMode Can be set to either of the following values:
    - OS_SEEK_SET The fileOffset parameter will be interpreted as an
    offset from the beginning of the file
    - OS_SEEK_END The fileOffset parameter is ignored and the file pointer 
    is placed at the end of the file.

    @note CURRENTLY ONLY OS_SEEK_SET MODE IS IMPLEMENTED (OTHER MODES ARE
    NOT USED YET ANYWAY)

    @return Zero if the file pointer was moved successfully, otherwise 
    nonzero. */
extern MAPCORE_API INT32 io_fseek( IO_FILE *fp,
                                   INT32 offset,
                                   UINT32 seekMode );

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif
