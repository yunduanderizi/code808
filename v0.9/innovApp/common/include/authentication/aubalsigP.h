#ifndef AUBALSIGP_H
#define AUBALSIGP_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TEnableTag {
    UINT32 associatedID;
    UINT8 *appIDString;
    BOOL8 alreadyEnabled;
} TEnableTag;

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/** Size of RSA signatures */
#define SIGSIZE                     (1024 / 8)  

/** Size of SHA1 hash */
#define HASHID_SIZE                 (160 / 8)

/** Size of signature group ID field */
#define GROUPID_SIZE                sizeof(UINT32)

/** Size of other encrypted data
 *      Expiry Date Time - UINT32
*/
#define ENCRYPTED_DATA_SIZE         sizeof(UINT32)

/*
 * Enabler Data global definitions 
 */
#define EED_GID_OFFSET_LOC     0
#define EED_ELEMENT_COUNT_LOC  4
#define EED_DATA_BLOCK_LOC     8
#define EED_INDEX_START       12

#define MAX_ENCRYPT_INPUT_LEN 112

/*
 * Key Types
 */
#define EED_KT_FEATURE            1
 
/*
 * Index entry definitions
 */
#define EED_KEY_TYPE_OFFSET_LOC      0
#define EED_KEY_LENGTH_OFFSET_LOC    1  // Maximum of 256 characters per key
#define EED_KEY_SECURITY_LEVEL_LOC   2
#define EED_KEY_RESERVED_LOC         3
#define EED_DATA_OFFSET_LOC          4
#define EED_DATA_SIZE_LOC            8
#define EED_KEY_START               12

#define EED_MAX_KEY_SIZE           256
 
#define EED_KEY_ENTRY_SIZE(KS_) (EED_KEY_START + KS_)


/** 
 * Size of signature node (all three fields)
 *   Separated into two fields as the application only has to support the new
 *   features whereas the server has to support all previous signature
 *   generation as well. For modern signatures including the encrypted data the
 *   server will add the extra storage required to the "short" node size. 
 */
#define NODE_SIZE_EDS               (SIGSIZE + HASHID_SIZE + GROUPID_SIZE + ENCRYPTED_DATA_SIZE)
#define NODE_SIZE_SVR               (SIGSIZE + HASHID_SIZE + GROUPID_SIZE)

/** Maximum number of registry locations that can be tracked - this be equal to or greater than
    the number of storage locations on the target platform (eg, one local store and two
    removable media slots = MAX_REGISTRIES defined as 3 or more). */
#define MAX_REGISTRIES              4

/** Tuning parameter - if MAX_CONCURRENT_REGISTRIES is set to less than MAX_REGISTRIES,
    the management code will discard chunks from least-recently-used registries rather
    than splitting available space to hold data from all registry files at once. */
#define MAX_CONCURRENT_REGISTRIES   4

/** MAX_FILE_PATH_LENGTH should be set to the longest possible file path length that will
    describe signature registries on the platform. */
#define MAX_FILE_PATH_LENGTH        255

/** MAX_GID_INCOMPATIBILITIES sets the size of the static buffer declared in aubalsig.c
    used to store signature group ID incompatibility links (8 bytes per incompatibility
    link will be required). This should be set to a reasonable maximum based on the number
    of defined data groups and no-simultaneous-load data type relationships. */
#define MAX_GID_INCOMPATIBILITIES   256

/*****************************************************************************
 * GLOBAL FUNCTION DECLARATIONS
 *****************************************************************************/

/** addRegLocation adds a signature registry file path to the list of available registries.
    The management code will immediately check for a file existing at that location and if
    found divide out some storage space for it and load a chunk. If no file is found at the
    location it will flag it for retrying when an authentication search fails in any other
    loaded registries.

    @param fileName Name of signature registry file to load or add to 'available registries'
    list if immediate load fails.
    @param locRemovable Set to TRUE if the signature registry is located on removable media,
    FALSE if on local storage. The tree allocation logic will give more space to removable
    media registries on the expectation that calling code will be more likely to use data
    on removable media when it is present.
    @return Returns TRUE if the registry location was added to the internal list, FALSE
    if adding this location would exceed the defined maximum registry count.
*/
BOOL8 addRegLocation(const char *fileName, BOOL8 locRemovable);

/** initSigStorage initialises the signature data storage area and the tree
    structure used to load chunks out of registry files. Memory required is:

    Tree node structure per signature: sizeof(TRegNode) bytes (which will probably be
    212)

    So the size of the buffer supplied needs to be
    ((signatures to hold in memory) * sizeof(TRegNode)) - the aubalsig functions will only
    use a buffer size divisible by sizeof(TRegNode) bytes and ignore any extra space.

    Note that no 'cleanup' function needs to be called when the calling code wants to stop
    using the aubalsig module.

    @param dataBuf Pointer to beginning of storage area available for signature data
	@param totalSizeAvail Size of buffer (in bytes) pointed to by dataBuf
*/
void initSigStorage(UINT8 *dataBuf, UINT32 totalSizeAvail);


/** searchSignature performs the tree search and chunk-swapping process to find a signature
    in the available registries. 

    @param inputHash 160-bit SHA1 hash of the authentication signature input parameters (a valid
    signature will decrypt to this value).
    @param signatureGID Pointer to a 32-bit value which, on successful return, will be loaded with
    the group ID associated with the signature. No data will be assigned to this buffer if the
    signature is not found.
	@return Returns TRUE if the signature was found in the available registries, FALSE if the signature
    was not found.
*/

/**
  getNumRegistries returns the number of sigreg.dat files that have been provided by the app
*/
UINT32 getNumRegistries( void );

/**
  getSigRegFileName - returns the filename, and whether the media is removable for that signature

  @param [in] entry - the position in the array of registries (1..getNumRegistries)
  @param [out] filename - the sigreg filename and location for that position in the arrow
  param[out] locRemovable - true if that sigreg file is on removeable media
*/
void getSigRegFileName( INT32 entry, char *fileName, UINT8 *locRemovable);

BOOL8 searchSignature(UINT8 *inputHash, UINT32 *signatureGID, UINT32 *expiryDateTime);

typedef struct TSGIDScanContext {
    UINT32 useRegIndex;
    UINT32 lastSGIDLoadIndex;
} TSGIDScanContext;

BOOL8 initSGIDSearch(TSGIDScanContext *context);

/** getNextSGID ...
    @param ID ...
    @param SGIDout ...
    @return ...
*/
BOOL8 getNextSGID(UINT32 ID, UINT32 *SGIDout, TSGIDScanContext *context);

/**
 * \brief auth_getEnablerData Gets data associated with the enabler.
 *
 * Retrieves the data associated with the enabler if the caller has the necessary
 * security level. Data at a lower security level is also returned.
 *
 * Current security levels
 *   0  Internal to authentication routines only
 *   1  Mapcore (accessed using auth_mcGetEnablerData) this function is not
 *      to be exposed to application.
 *   2  Application (accessed using auth_appGetEnablerData)
 *
 * @param[in]  featureEnabler pointer to null-terminated feature enabler
 * @param[in]  dataBufLenmaximum length of buffer
 * @param[out] dataBuf pointer to buffer in which to place data
 * @param[in]  securityLevel
 *
 * @return dataLen length of data returned; negative value indicates
 *         an error and the value provides detail
 */
UINT32 auth_getEnablerData(
        const UINT8 *featureEnabler,
        const UINT32 dataBufLen,
        UINT8 *dataBuf,
        const UINT32 securityLevel,
			  const TEnableTag enableIDList[] );


#ifdef __cplusplus
}
#endif

#endif /* AUBALSIGP_H */
