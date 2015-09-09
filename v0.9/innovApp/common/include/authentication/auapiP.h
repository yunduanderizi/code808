#ifndef AUAPIP_H
#define AUAPIP_H

#include <authentication/auapi.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

#define SECTOR_SIZE							512

#ifdef NO_SECTORS
/** if the application is compiled for no sectors, we change to read up to 16kB
    if data in one do, and dont use sector aligned reads and decrypt the data 
	directly to the map cache. we try a big read buffer, much biffer than 512
	bytes, to keep OS overhead low.  also word align read buffer for
	efficiency */
#define READ_BUF_SIZE						(16 * 1024) 
#endif


/*****************************************************************************
 * GLOBAL VARIABLE DECLARATIONS
 *****************************************************************************/

#ifdef NO_SECTORS
extern UINT32 shared_readbuf[READ_BUF_SIZE >> 2];
#endif


/*****************************************************************************
 * GLOBAL FUNCTION DECLARATIONS
 *****************************************************************************/

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

#define EDT_SECURITY_LEVEL_AUTH         0
#define EDT_SECURITY_LEVEL_MAPCORE      1
#define EDT_SECURITY_LEVEL_APP          2

/**
 * \brief auth_mcGetEnablerData Gets innovEngine data associated with the enabler.
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
UINT32 auth_mcGetEnablerData(
  const UINT8 *featureEnabler,
  const UINT32 dataBufLen,
  UINT8 *dataBuf );
  
#ifdef __cplusplus
}
#endif

#endif /* AUAPIP_H */
