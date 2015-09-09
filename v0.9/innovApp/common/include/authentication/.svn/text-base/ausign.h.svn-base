/** Interface to signature creation functions for back-end authentication 
    processing.

    author: Daniel Sauer <dsauer@navman.com> 
    author: Colin Robinson <colin.robinson@navman.com> */

#ifndef AUSIGN_H
#define AUSIGN_H

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

#if defined(AUTHFAKE_APP_BUILD) && !defined(_POSIX)
unsigned char TMC_TEST_ENABLER[] = { 'T', 'M', 'C', 'T', 'T', 'N' };

unsigned char TMC_TEST_DATA[] = { 
0x00, 0x00, 0x00, 0xad, 0x00, 0x02, 0x22, 0x55, 0x53, 0x01, 0x03, 0x02, 0x55, 0x53, 0x01, 0x03, 0x04, 0x55, 0x53, 0x01, 0x03, 0x08, 0x55, 0x53, 0x01, 0x03, 0x0C, 0x55, 0x53,
0x01, 0x03, 0x10, 0x55, 0x53, 0x01, 0x03, 0x15, 0x55, 0x53, 0x01, 0x03, 0x1a, 0x55, 0x53, 0x01, 0x03, 0x1b, 0x55, 0x53, 0x01, 0x03, 0x1c, 0x55, 0x53, 0x01, 0x03, 0x1d, 0x55,
0x53, 0x01, 0x03, 0x20, 0x55, 0x53, 0x01, 0x03, 0x21, 0x55, 0x53, 0x01, 0x03, 0x22, 0x55, 0x53, 0x01, 0x04, 0x0d, 0x55, 0x53, 0x01, 0x04, 0x0f, 0x55, 0x53, 0x01, 0x04, 0x12,
0x55, 0x53, 0x01, 0x04, 0x14, 0x55, 0x53, 0x01, 0x04, 0x16, 0x55, 0x53, 0x01, 0x04, 0x19, 0x55, 0x53, 0x01, 0x04, 0x1D, 0x55, 0x53, 0x01, 0x04, 0x23, 0x55, 0x53, 0x01, 0x05,
0x01, 0x55, 0x53, 0x01, 0x05, 0x03, 0x55, 0x53, 0x01, 0x05, 0x05, 0x55, 0x53, 0x01, 0x05, 0x07, 0x55, 0x53, 0x01, 0x05, 0x0B, 0x55, 0x53, 0x01, 0x05, 0x17, 0x55, 0x53, 0x01,
0x05, 0x18, 0x55, 0x53, 0x01, 0x05, 0x1F, 0x55, 0x53, 0x01, 0x06, 0x06, 0x55, 0x53, 0x01, 0x06, 0x09, 0x55, 0x53, 0x01, 0x06, 0x0a, 0x55, 0x53, 0x01, 0x06, 0x11, 0x55, 0x53,
0x01, 0x06, 0x13 };
#endif
 
/*****************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <platform.h>
#include <authentication/cryptlib/rsa.h>

/*****************************************************************************
 * GLOBAL FUNCTION DECLARATIONS
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
    generateSig takes parameters representing the four Map Auth V2 inputs (SHA1 hash of
    the input .chk file, 'software key' ASCII string in the form "XXX-YYY-NNNNN-NNNNN",
    arbitrary-length application ID data and hardware serial ID data, concatenates them,
    calculates a SHA1 hash over the concatenated data, adds data to be encrypted if required
    and encrypts the hash with the Navman 'map data' 1024-bit RSA private key to generate
    an authentication signature for the .chk file in question.

    @param cryptoProvider Which provider to use to generate the RSA encrypted data (Cryptographic
    accelerator, OpenSSL or Navman Reference routines).
    @param privateKeyID Which private key to use for encryption.
    @param addEncryptedData Flag to tell this function whether to add data to the hash to be encrypted.
    @param expiryDateTime The expiry date of the resource. This will be encrypted into the extra data
    associated with the signature.
    @param chunkHash Pointer to 16-bit SHA1 hash of input .chk file
    @param chunkHashLen Length of data (in bytes) pointed to by chunkHash. This should always
    be set to 20, unless a 'feature-enable' signature is being generated with no chunk hash
    parameter, in which case it should be zero
    @param softwareKey Pointer to a null-terminated ASCII string
    @param appID Pointer to a buffer containing the application ID data (binary, any length)
    @param appIDLen Length (in bytes) of the application ID data
    @param hardwareID Pointer to a buffer containing the hardware serial data (binary, any length)
    @param hardwareIDLen Length (in bytes) of the hardware ID data
    @param inputHashBuf Pointer to a 20-byte buffer which will be loaded on return with the
    160-bit SHA1 hash created as input to the digital signature out of the input parameters
    @param signatureBuf Pointer to a 128-byte buffer which will be loaded with the digital
    signature created from the input parameters
*/

INT32 generateSig(
        UINT32 cryptoProvider, 
        UINT8 privateKeyID, 
        UINT8 addEncryptedData, 
        UINT32 expiryDateTime, 
        UINT8 *chunkHash, 
        UINT32 chunkHashLen, 
        char *softwareKey, 
        UINT32 softwareKeyLen, 
        UINT8 *appID, 
        UINT32 appIDLen, 
        UINT8 *hardwareID, 
        UINT32 hardwareIDLen, 
        UINT8 *inputHashBuf, 
        UINT8 *signatureBuf );

/** Initialise the private key variable from our RSA signature.  Needed by 
    database to initialise local variable to avoid static local variable
    clashes from multiprocess database engine.

    @param pk           Pointer to array of private key structures.
    @param privateKeyID Which private key to use (now have three; 1 each for legacy, tui+ and wireless)
*/
R_RSA_PRIVATE_KEY * getPrivateKey(UINT32 privateKeyID);

INT32 initEnablerDataArea(UINT8 *image);

INT32 addEnablerData(
        UINT8 *image, 
        UINT32 imageInLength,
        UINT8 newKeyType, 
        UINT8 newKeySecurityLevel,
        UINT8 newKeyLength, 
        UINT8 *newKeyValue, 
        UINT32 enablerDataLength, 
        UINT8 *enablerData );

INT32 encryptEnablerData(
        UINT8 *image, 
        UINT32 imageLength, 
        UINT8 *hardwareIdentifier,
        UINT32 hardwareIdentifierLength,
        UINT8 *featureEnabler,
        UINT32 featureEnablerLength,
        UINT8 *encryptedImage,
        UINT32 encryptedImageLength,
        UINT32 cryptoProvider, 
        UINT32 privateKeyID );
        
void initGIDArea(UINT8 *image, UINT32 *imageOutLen, BOOL8 addEnablerData);

/** Add a GID pair to a gid memory image.  This function is used iteratively by
    the database to add individual GID pairs retrieved via. cursor from the
    database, into an image compatible with the mapcore GID pair sigreg
    implementation.

    @param gidleft Map GID.
    @param gidright Map GID.
    @param image Pointer to data pointer big enough to accomodate full GID image.
    @param imageInLen Current length of the data image of GIDs.  Note this is
           not the total size of the image array, just the used size.
    @param imageOutLen Image after GID pair was added written on function 
           success.
    @return Returns TRUE on success, FALSE on failure.
*/
BOOL8 addToGIDImage(UINT32 gidleft, UINT32 gidright, UINT8 *image, UINT32 imageInLen, UINT32 *imageOutLen, BOOL8 addEnablerData);

/** Add a GID to an SGID's membership group. This function should be used iteratively to 
    add GIDs under each SGID in sequence.

    @param input_sgid SGID to add member to
    @param input_gid GID to add as a member under input_sgid
    @param image Pointer to start of registry data previously initialised with addToGIDImage()
    @param imageInLen Length of image as previously created
    @param imageOutLen Pointer to integer which will, on return, be loaded with the output
           size of the image as modified by addSGIDMember
    @return Returns TRUE on success, FALSE on failure.
*/
void initSGIDArea(UINT8 *image, UINT32 imageInLen, UINT32 *imageOutLen);

BOOL8 addSGIDMember(UINT32 input_sgid, UINT32 input_gid, UINT8 *image, UINT32 imageInLen, UINT32 *imageOutLen, BOOL8 addEnablerData);

/** Add a signature to a sig memory image.  This function is used iteratively
    by the database to add an individual lookup hash and signature generated
    by the generateSig() function to an in-memory image of the signature 
    portion of the sigreg.dat file.

    @param hashData Lookup hash data.
    @param sigData Actual signature data for this lookup.
    @param groupID GID for this map.
    @param image Pointer to data pointer big enough to accomodate full sig image.
    @param imageInLen Current length of the sig data image. Note this is not 
           the total size of the image array, just the used size.
    @param imageOutLen Set to after image size once addition of signature is
           complete.
    @return Returns TRUE on success, FALSE on failure.
*/
BOOL8 initSigArea(UINT8 *image, UINT32 imageInLen, UINT32 *imageOutLen, BOOL8 addEnablerData);

BOOL8 addToSigImage(UINT8 *hashData, UINT8 *sigData, UINT32 groupID, UINT8 addEncryptedData, UINT32 expiryDateTime, BOOL8 addEnablerData, UINT8 *image, UINT32 imageInLen, UINT32 *imageOutLen);

BOOL8 addSequenceData(UINT8 *image, UINT32 sequenceHigh, UINT32 sequenceLow, UINT32 imageInLen, UINT32 *imageOutLen, BOOL8 addEnablerData);

#ifdef __cplusplus
}
#endif

#endif /* AUSIGN_H */
