#ifndef AUUTILP_H
#define AUUTILP_H

#include <innovEngine_io.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * GLOBAL VARIABLE DECLARATIONS
 *****************************************************************************/

#ifdef NO_SECTORS
extern UINT32 shared_readbuf[READ_BUF_SIZE >> 2];
#endif

UINT32 getFileSize(const char *filename);

/** Calculate the file chunk hash of a given map file.  The hash is a 20-byte
    SHA based.

    @param pMapFileName Pointer to map file name.
    @param hash_buf Pointer to buffer area to place calculated chunk hash,
	  must be at least 20 bytes.
    @return Returns TRUE on successful calculation of chunk hash. */
UINT8 calcChunkHash(const char *pMapFileName, UINT8 *hash_buf);

/** Calculate the file chunk hash of a given map file.  The hash is a 20-byte
    SHA based and emulates the older algorithm used in V1 map authentication which
    hashed across an entire file.

    @param pMapFileName Pointer to map file name.
    @param hash_buf Pointer to buffer area to place calculated chunk hash,
	  must be at least 20 bytes.
    @return Returns TRUE on successful calculation of chunk hash. */
UINT8 calcChunkHashLongForm(const char *pMapFileName, UINT8 *hash_buf);

/** Reset the current CD key read.  Resets the internal variables and is
    used with getNextCDKey to loop through the given CD keys that a user has
    installed on their product.

    @param file File handle to CD key registry file.
    @param file_len Length of CD key registry file. */
void initCDKeyRead( IO_FILE *file, UINT32 file_len );

/** Return the next CD key iteration from the CD key registry file.  Call
    initCDKeyRead() first.

    @param file File handle to CD key registry file.
    @param buf Buffer to place CD key.
    @param key_len Pointer to integer where length of CD key returned.
    @param bufLen Length of buffer allocated for CD key.
    
    @return Returns TRUE on CD key available, FALSE if no CD keys left. */
UINT8 getNextCDKey( IO_FILE *file, UINT8 *buf, UINT32 *key_len, UINT32 buflen );

/** Reset the current auth signature read.  Resets the internal variables and
    is used with getNextSig to loop through the given signatures that a user
    has activated on their product.

    @param file File handle to signature registry file.
    @param file_len Length of signature registry file. */
void initSigRead( IO_FILE *file, UINT32 file_len );

/** Return the next signature iteration from the signature registry file.
    Call initSigRead() first.

    @param file File handle to signature registry file.
    @param buf 148 byte buffer to place signature read into.
    
    @return Returns TRUE if signature returned succesfully, FALSE if no 
    signatures available. */
UINT8 getNextSig( IO_FILE *file, UINT8 *buf );

/** Check whether a supplied group ID matches one already in the list. */
BOOL8 matchExistingGroupID(UINT32 identifier);

/** Check whether an incompatible link relationship exists between the supplied group ID 
    and one already in the list by doing a full registry scan. */
BOOL8 checkIncompatibleLink(UINT32 identifier);

/** Adds a group ID to the list of 'compatible authorised group IDs'. Returns FALSE if 
    there wasn't enough space, in which case the caller should initiate a full 
    incompatibility scan. */
BOOL8 addGroupID(UINT32 identifier);

/** Cleans out the list of 'compatible authorised group IDs'. */
void clearGroupIDList(void);

/** Retrieves a 32-bit integer from a buffer endian-independently. */
UINT32 get32(UINT8 *buf);

void put32(UINT8 *buf, UINT32 val);

#ifndef NO_SECTORS

/** Initialise the sector read buffer used in accelerated map file reading. */
void initSectorSizeBuffer( void );

/** Check to see if the file stream specified is currently the one stored in the
    accelerated sector read buffer, and if so, reset the valid flag to make
    sure data is re-read.

    @param stream File handle to check. */
void checkClearSectorBuffer( IO_FILE *stream );

/** Read in some buffered data from a file handle.  Hopefully we have already
    read in this particular sector, and if so we can returned cached sector
    data, otherwise we read in the new data.

    @param stream File handle to read.
    @param offset File offset to read from.
    @param buffer Buffer to write file data to. */
UINT32 getBufferedSectorSizeData( IO_FILE *stream, UINT32 offset, UINT8 *buffer );

/** Read the global chunk structure from an open chunk file, and validate it. If it
    appears as gibberish, assume the file is encrypted and returns TRUE. Returns FALSE
    (unencrypted file) if the data appears valid. */
BOOL8 testChunkRead(IO_FILE *fileHdl);

#endif	/* NO_SECTORS */

/** Interface to SHA-1 calculation for use in creating a user code and password 
    for the iAppliance servers

@param inputBuf     Block of data for which SHA-1 hash is to be calculated
@param inputBufLen  Length of data for which SHA-1 hash is to be calculated
@param outputBuf    Output buffer containing SHA-1 hash.
*/
void calcSHA1(UINT8* inputBuf, INT32 inputBufLen, UINT8* outputBuf);

/** Calculates MD5 sum for a file

@param fileName     Absolute name of file to have an MD5Sum calculated; null-terminated string
@param outputBuf    Output buffer containing MD5SUM; 16 bytes.
*/
BOOL8 calcFileMD5(const UINT8* fileName, UINT8* outputBuf);

#ifdef __cplusplus
}
#endif

#ifdef VSTUDIO_BUILD
#define os_fopen_direct os_fopen
#define os_fclose_direct os_fclose
#define os_fread_direct os_fread
#define os_fseek_direct os_fseek
#define os_printf printf

#include <stdio.h>

#endif

#ifdef ACTIVATION_SERVER_
#include <stdio.h>
#define os_printf printf
#endif


#endif /* AUUTILP_H */
