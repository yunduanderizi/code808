/* This file contains the interface to the authentication subsystem.  This 
   system provides a secure mechanism to distribute content.

   author: Daniel Sauer <dsauer@21com.com>
   author: Ben Bodley <ben_bodley@hotmail.com> */

#ifndef AUAPI_H
#define AUAPI_H

#include <innovEngine_io.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Authentication
    The authentication provides services for the reading of encrypted 
    data. */
/**@{ start of group */

#define AUTH_ERROR_NO_SIGNATURE         0
#define AUTH_ERROR_INCOMPATIBLE         1
#define AUTH_SUCCESS                    2
#define AUTH_ERROR_BAD_PIN              3
#define AUTH_ERROR_EXPIRED              4

#define AUTH_COMPATIBLE                 0
#define AUTH_INCOMPATIBLE               1
#define AUTH_OBLIGATED_A                2
#define AUTH_OBLIGATED_B                3

#define AUTH_OBLIGATION_BIT             (1ul << 30)

#define PIN_LENGTH                      16

#define RESOURCE_ALREADY_EXPIRED_DATETIME  0UL
#define RESOURCE_NEVER_EXPIRES_DATETIME    4102444799UL
#define MAXIMUM_DRIFT_TIME                 86400UL

/** Initialise the authentication subsystem.
    
    @param cdkey_file Full path to cd key registry file for this platform.
    
    @param signatureBuffer Pointer to storage buffer for signature.
    
    @param sigBufSize The size of the buffer use to store the signature.
    
    @return Returns TRUE on successful initialisation, FALSE on failure. */
extern  MAPCORE_API UINT8 auth_init( const char *cdkey_file, 
                                     UINT8 *signatureBuffer, 
                                     UINT32 sigBufSize );
  
 /** Initialises the authentication buffer. If the system has multiple cdkeys.dat files, this function
      will require a path to where to store the contents of the different cdkeys.dat files.  
      
      @param cdkeyStorageFile Absolute file path to where a temporary cdkey storage file is to 
                             be created. This should be on permanent storage, i.e. the device. 
                             If there is only one cdkeys.dat file this paramter may be NULL, and this file will not 
                             be created
      
      @param signatureBuffer Pointer to storage buffer for signature.
      
      @param sigBufSize The size of the buffer use to store the signature.
      
      @return Returns TRUE on successful initialisation, FALSE on failure. */
  MAPCORE_API UINT8 auth_initBuffer( const char *cdkeyStorageLoc, 
			       UINT8 *signatureBuffer, 
			       UINT32 sigBufSize );
  
  /** Call this function for every cdkeys.dat file present.
      Append the contents of the cdkey file into a storage file as specified by auth_initBuffer.
      This function can only be called after auth_initBuffer has been called.
      If there is only one cdkeys file, use auth_init instead.

      @param cdkeys_file Full path to cdkeys file. 
      
      @param isRemovable Should be TRUE if registry is located on removable 
      storage, FALSE if registry file is on local (permanent) storage. 
      
      @return Returns TRUE if the file was read successfully and could write to the storage
      file*/
  MAPCORE_API BOOL8 auth_addCDKeyFile( const char *cdKey_file, 
				     BOOL8 isRemovable );
  

/** Add a signature registry file to the internal list which will be 
    searched for data authentication signatures.

    @param sigreg_file Full path to signature registry file. If the file is 
    not immediately visible the authentication subsystem will silently check 
    for its presence when searching for a signature that is not found in 
    visible registry files before returning failure.
    
    @param isRemovable Should be TRUE if registry is located on removable 
    storage, FALSE if registry file is on local (permanent) storage. This 
    affects the prioritisation of allocated signature storage memory in 
    the internal authentication functions.
    
    @return Returns TRUE if registry file was added to internal list, FALSE 
    if the internal maximum number of registry files was exceeded and this 
    one was not added. */
extern  MAPCORE_API BOOL8 auth_add_sigreg( const char *sigreg_file, 
				     BOOL8 isRemovable );

/** Shutdown the authentication subsystem. */
extern  MAPCORE_API void auth_shutdown( void );

/** Add a PIN

    @param newPIN Personal Idenfitication Number supplied by the customer
    if set or the default PIN. Maximum length of PIN_LENGTH. Null terminated. */
extern void auth_set_pin(const UINT8 *newPIN);

/**
 * \brief Loads the traffic service to feature enabler mappings.
 *
 * @param[out] products pointer to buffer to store product data
 * @param[in]  maxProducts the maximum number of products to store
 *
 * @return the number of products stored
 */
//UINT32 auth_loadTrafficProducts(TTrafficProduct *products, UINT32 maxProducts);

/**
 * \brief auth_appGetEnablerData Gets innovEngine data associated with the enabler.
 *
 * Retrieves the data associated with the enabler that innovEngine is
 * allowed to see. Data that is at a higher security level than
 * innovEngine is not returned. Data at a lower security level is also
 * returned.
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
 *
 * @return dataLen length of data returned; negative value indicates
 *         an error and the value provides detail
 */
UINT32 auth_appGetEnablerData(
  const UINT8 *featureEnabler,
  const UINT32 dataBufLen,
  UINT8 *dataBuf);

/** Check to see if the particular map file is verified to work on the 
    unit. Maintains an internal list of files that have been 'verified'.  
    This list can be cleared by a call to auth_clearVerifyHistory().

    @param pMapFileName Map file name
    
    @param groupIdentifier Pointer to a 32-bit variable which, on return 
    with a '1' code, will be loaded with the group compatibility identifier 
    of the verified file. This identifier can be used (matched against 
    identifiers returned against other files by auth_verify()) as input to 
    auth_areFilesCompatibleOrObligated() when generating 'loadability' 
    information.
    
    @param alwaysAllowCleartext Boolean value which, when true, will cause
    auth_verify to return AUTH_SUCCESS without checking for signature data
    against a file *if* that file is not block-encrypted (this is to support
    differentiation of user-created custom POI data from block-encrypted 
    OEM POI data, and calling application code should set this input 
    parameter accordingly).
    
    @return Returns AUTH_ERROR_NO_SIGNATURE if no signature authorising the 
    file is present; AUTH_ERROR_INCOMPATIBLE if a signature is present, but 
    loading the file would conflict with currently loaded data (as stored 
    in the history list created by previous calls to auth_verify());
    AUTH_SUCCESS if a signature is present and the file should be loaded. */
extern  MAPCORE_API UINT32 auth_verify( 
        const char *pMapFileName, 
        UINT32 *groupIdentifier,
        BOOL8 alwaysAllowCleartext,
        UINT32 currentDateTime, 
        UINT32 *expiryDateTime );

/** Clears any outstanding list of map files and load-compatibility 
    information generated by a previous sequence of calls to auth_verify. */
extern  MAPCORE_API void auth_clearVerifyHistory( void );

/** Associates an application ID string with a feature-enable ID.

    @param levelID 32-bit integer to associate with an application ID 
    string.
    
    @param appIDString Pointer to null-terminated application ID string.

    @return TRUE if enable ID was successfully added to internal auth list,
    FALSE if there were too many IDs already registered. */
extern MAPCORE_API BOOL8 auth_setEnabler( UINT32 levelID, 
                                          UINT8 *appIDString );

/** Checks whether a feature-enable (zero-length chunk hash) signature 
    exists for a given pre-registered (via auth_setEnabler) feature
    enable ID.

    @param levelID 32-bit feature-enable identifier to check. Level ID of 0 
    is reserved for the PIN signature. This only uses the Hardware Identifier
    and the user or default PIN.
    
    @return TRUE if application ID associated with specified level ID 
    exists in a feature-enable signature, otherwise FALSE. */
extern MAPCORE_API BOOL8 auth_isEnabled( UINT32 levelID );

/** Uses the group ID compatibility matrix data from loaded signature 
    registry files to determine whether the specified pair of IDs can be 
    loaded simultaneously.

    @param identifierA Group identifier returned for a map or chunk file by 
    a successful call to auth_verify.
    
    @param identifierB Group identifier returned for a map or chunk file by 
    a successful call to auth_verify.
    
    @return AUTH_COMPATIBLE if files with the supplied group identifiers 
    can be safely loaded simultaneously, AUTH_INCOMPATIBLE if not, 
    AUTH_OBLIGATED_A if the identifierA parameter should always be loaded 
    instead of identifierB, AUTH_OBLIGATED_B if the identifierB
    parameter should always be loaded instead of identifierA. */
extern  MAPCORE_API UINT32 auth_areFilesCompatibleOrObligated( UINT32 identifierA,
                                                               UINT32 identifierB );

/** Uses the group ID compatibility matrix data from loaded signature 
    registry files to determine whether the specified pair of IDs can be 
    loaded simultaneously.

    @param identifierA Group identifier returned for a map or chunk file by 
    a successful call to auth_verify.
    
    @param identifierB Group identifier returned for a map or chunk file by 
    a successful call to auth_verify.
    
    @return TRUE if files with the supplied group identifiers can be safely 
    loaded simultaneously, FALSE if not. */
extern MAPCORE_API BOOL8 auth_areFilesCompatible( UINT32 identifierA,
                                                  UINT32 identifierB );

/** Clears the internal history maintained by the auth_verify function 
    which causes auth_verify to return a failure code when a map files is 
    passed in subsequent to a previous file with which it has an 
    incompatible load' link. */
extern  MAPCORE_API void auth_clearCompatibilityHistory( void );

/** Decrypts block of data using the public key
    First use is for protecting the URL of the iAppliance Redirector from
	being tampered with.
	
	@param encryptedBuffer    Block of data to be decrypted 
	@param encryptedBufferLen Length of data to be decrypted (<= size of buffer)
	@param decryptBuffer      Buffer into which decrypted data to be placed
	@param decryptBufferLen   Length of decrypt buffer
	
	@return Length of the decrypted data - negative if failed */
extern  MAPCORE_API BOOL8 auth_decryptBlock( const UINT8* encryptedBuffer,
                                             UINT32 encryptedBufferLen,
                                             UINT8* decryptBuffer,
                                             UINT32* decryptBufferLen );

/** Exposes SHA-1 calculation to application for use in creating a user code
    and password for the iAppliance servers

	@param inputBuf     Block of data for which SHA-1 hash is to be calculated
	@param inputBufLen  Length of data for which SHA-1 hash is to be calculated
	@param outputBuf    Output buffer containing SHA-1 hash; 20 bytes.
	*/
extern  MAPCORE_API void auth_calcSHA1( UINT8* inputBuf, 
								                        INT32 inputBufLen, 
								                        UINT8* outputBuf);

/** Exposes MD5Sum calculation to application for use in checking that files have

@param fileName     Absolute name of file to have an MD5Sum calculated; null-terminated string
@param outputBuf    Output buffer containing MD5SUM; 16 bytes.
*/
extern  MAPCORE_API BOOL8 auth_calcFileMD5(const UINT8* fileName,
                                           UINT8* outputBuf);

/**@} end of group */

#ifdef __cplusplus
}
#endif

#endif	/* AUAPI_H */
