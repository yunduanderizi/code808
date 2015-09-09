#ifndef _SAL_TYPES_H_
#define _SAL_TYPES_H_

/** @file SALTypes.h
  * @brief Type definitions
  *
  * Defines simple types to be used which will remain the same
  * width on different targets.
  */

#if defined(WIN32) && !defined(_WIN32_WCE)
#include <sys/types.h>
#elif defined(LINUX) || defined(QNX)
#include <stdint.h>
#endif

typedef unsigned char  		byte_t;		/**< Eight-bit unsigned */
typedef unsigned short 		word_t;		/**< 16-bit unsigned */
typedef unsigned int		dword_t;	/**< 32-bit unsigned */

typedef unsigned char   	uint8_t;	/**< Eight-bit unsigned */
typedef unsigned short  	uint16_t;	/**< 16-bit unsigned */
typedef unsigned int    	uint32_t;	/**< 32-bit unsigned */

typedef signed char			int8_t;	/**< Eight-bit signed */
typedef short  				int16_t;	/**< 16-bit signed */
typedef int    				int32_t;	/**< 32-bit signed */

typedef byte_t*				SALRawMessage_t;	/**< Raw data in a Message */

/** Declaration of priorities */
typedef enum {
	SYS_PRIO_HIGH = 0,
	SYS_PRIO_LOW,
	APP_PRIO_VERY_HIGH,
	APP_PRIO_HIGH,
	APP_PRIO_MEDIUM,
	APP_PRIO_LOW,
	APP_PRIO_VERY_LOW,
	APP_PRIO_IDLE
} priority_t;

/** Number of different priority levels */
#define SAL_MAX_PRIORITY 8

#endif /*_SAL_TYPES_H_*/
