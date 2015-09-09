#ifndef _SAL_DEBUGTRACE_BASE_H_
#define _SAL_DEBUGTRACE_BASE_H_

/** @file SALDebugTraceBase.h
  * @brief Definitions and functions used for debugging.
  *
  * This file should not
  * be included directly, please include SALDebugTrace.h instead.
  */

#include "SALTypes.h"
#include "SALDebugTraceSettings.h"

#define SAL_DT_MAX_APP_NAME 	64		/**< Maximum Length of Application Name */
#define SAL_DT_MAX_GROUPS		256		/**< Maximum number of groups */
#define SAL_DT_MAX_ZONES		32		/**< Maximum number of zones */
#define SAL_DT_BUFFER_SIZE		1024	/**< Size of buffer */

/** Debug Zones */
enum DebugZones {
	SAL_DT_ZONE_NONE = 	0x00000000,
	SAL_DT_ZONE_1 = 	0x00000001,
	SAL_DT_ZONE_2 = 	0x00000002,
	SAL_DT_ZONE_3 = 	0x00000004,
	SAL_DT_ZONE_4 = 	0x00000008,
	SAL_DT_ZONE_5 = 	0x00000010,
	SAL_DT_ZONE_6 = 	0x00000020,
	SAL_DT_ZONE_7 = 	0x00000040,
	SAL_DT_ZONE_8 = 	0x00000080,
	SAL_DT_ZONE_9 = 	0x00000100,
	SAL_DT_ZONE_10 = 	0x00000200,
	SAL_DT_ZONE_11 = 	0x00000400,
	SAL_DT_ZONE_12 = 	0x00000800,
	SAL_DT_ZONE_13 = 	0x00001000,
	SAL_DT_ZONE_14 = 	0x00002000,
	SAL_DT_ZONE_15 = 	0x00004000,
	SAL_DT_ZONE_16 = 	0x00008000,
	SAL_DT_ZONE_17 = 	0x00010000,
	SAL_DT_ZONE_18 = 	0x00020000,
	SAL_DT_ZONE_19 = 	0x00040000,
	SAL_DT_ZONE_20 = 	0x00080000,
	SAL_DT_ZONE_21 = 	0x00100000,
	SAL_DT_ZONE_22 = 	0x00200000,
	SAL_DT_ZONE_23 = 	0x00400000,
	SAL_DT_ZONE_24 = 	0x00800000,
	SAL_DT_ZONE_25 = 	0x01000000,
	SAL_DT_ZONE_26 = 	0x02000000,
	SAL_DT_ZONE_27 = 	0x04000000,
	SAL_DT_ZONE_28 = 	0x08000000,
	SAL_DT_ZONE_29 = 	0x10000000,
	SAL_DT_ZONE_30 = 	0x20000000,
	SAL_DT_ZONE_31 = 	0x40000000,
	SAL_DT_ZONE_32 = 	0x80000000
};

#define SAL_DT_ZONE_ERROR		SAL_DT_ZONE_1		/**< Predefined zone for Errors */
#define SAL_DT_ZONE_WARNING		SAL_DT_ZONE_2		/**< Predefined zone for Warnings */
#define SAL_DT_ZONE_INFO		SAL_DT_ZONE_3		/**< Predefined zone for Information */
#define SAL_DT_ZONE_SCOPE		SAL_DT_ZONE_4		/**< Predefined zone for debugging on scope level */

#define DTZ_ERR					SAL_DT_ZONE_ERROR		/**< Short name for predefined error zone */
#define DTZ_INF					SAL_DT_ZONE_WARNING		/**< Short name for predefined information zone */
#define DTZ_WRN					SAL_DT_ZONE_INFO		/**< Short name for predefined warning zone */
#define DTZ_SCP					SAL_DT_ZONE_SCOPE		/**< Short name for predefined scope debugging zone */


/** error and warning zones are enabled by default */
#define SAL_DT_DEFAULT_ZONES	(SAL_DT_ZONE_ERROR | SAL_DT_ZONE_WARNING)

typedef enum trace_type 
{
   _TR_ERROR_ = 0, 
   _TR_WARNING_,  
   _TR_INFO_,
   _TR_DEBUG_,   
   _TR_NUM_OF_TYPES
} trace_type_t;

/** function pointer to pointer used for flushing trace messages */
typedef void (*DbgFlushFunc) (const char*);

/** Per-group settings. See SALDebugTraceSettings for groups */
extern	dword_t g_DtGroupSettings[SAL_DT_MAX_GROUPS];

//
// Check if DebugTrace should be present.
//

#ifndef CFG_NO_DEBUG_TRACE

#define SAL_TrInit( argc, argv ) \
do { \
	DbgTr_Lock(); \
   	DbgTr_initialize(); \
   	DbgTr_Unlock(); \
} while(0)

#define SAL_TrSetApp( appID, appName ) \
do { \
	DbgTr_Lock(); \
   	DbgTr_setAppIdentifiers(appID,appName); \
   	DbgTr_Unlock(); \
} while(0)

#define SAL_TrConfigure( group, zoneMask ) \
do { \
	DbgTr_Lock(); \
	DbgTr_configureGroup(group,zoneMask); \
	DbgTr_Unlock(); \
} while(0)

#define SAL_TrEnable( group, zoneMask ) \
do { \
	DbgTr_Lock(); \
	DbgTr_enableTrace(group,zoneMask); \
	DbgTr_Unlock(); \
} while(0)

#define SAL_TrDisable( group, zoneMask ) \
do { \
	DbgTr_Lock(); \
	DbgTr_disableTrace(group,zoneMask); \
	DbgTr_Unlock(); \
} while(0)

/** @def SAL_TrErr
  * Output an error message. Arguments are printf-like.
  */
/** @def SAL_TrWrn
  * Output a warning message. Arguments are printf-like.
  */
/** @def SAL_TrInf
  * Output an informational message. Arguments are printf-like.
  */
/** @def SAL_TrMsg
  * Output a debug message. Arguments are printf-like.
  */
#if !defined(QNX) && !defined(_WIN32_WCE)
#define SAL_TrErr(...) do { DbgTr_getLineInfo(__FILE__, __LINE__, 0); DbgTr_errorMessage(__VA_ARGS__); } while(0)
#define SAL_TrWrn(...) do { DbgTr_getLineInfo(__FILE__, __LINE__, 0); DbgTr_warningMessage(__VA_ARGS__); } while(0)
#define SAL_TrInf(...) do { DbgTr_getLineInfo(__FILE__, __LINE__, 0); DbgTr_infoMessage(__VA_ARGS__); } while(0)
#define SAL_TrMsg(...) do { DbgTr_getLineInfo(__FILE__, __LINE__, 0); DbgTr_debugMessage(__VA_ARGS__); } while(0)
#else
#define SAL_TrErr DbgTr_getLineInfo(__FILE__, __LINE__, 0); DbgTr_errorMessage
#define SAL_TrWrn DbgTr_getLineInfo(__FILE__, __LINE__, 0); DbgTr_warningMessage
#define SAL_TrInf DbgTr_getLineInfo(__FILE__, __LINE__, 0); DbgTr_infoMessage
#define SAL_TrMsg DbgTr_getLineInfo(__FILE__, __LINE__, 0); DbgTr_debugMessage
#endif

#else // Do not include tracing in the output code.

#ifndef QNX
#define SAL_TrInit(argc, argv)	do {} while(0)
#define SAL_TrErr(...)	do {} while(0)
#define SAL_TrWrn(...)	do {} while(0)
#define SAL_TrInf(...)	do {} while(0)
#define SAL_TrMsg(...)	do {} while(0)
#else
#define SAL_TrInit(argc, argv)	do {} while(0)
#define SAL_TrErr DbgTr_empty1
#define SAL_TrWrn DbgTr_empty1
#define SAL_TrInf DbgTr_empty1
#define SAL_TrMsg DbgTr_empty2
#endif

#endif //CFG_NO_DEBUG_TRACE


// Debug Trace functions, should not be used directly in the code

/** @cond PRIVATE */

void DbgTr_Lock ( void ); 
void DbgTr_Unlock ( void );

void DbgTr_getLineInfo( const char *file, int line, const char* thread ); 

void DbgTr_setAppIdentifiers ( byte_t appID, char* appName );
void DbgTr_initialize ( void );
void DbgTr_errorMessage ( const char* format, ...);
void DbgTr_warningMessage ( const char* format, ...);
void DbgTr_infoMessage ( const char* format, ...);
void DbgTr_debugMessage ( /*byte_t type,*/ byte_t group, dword_t zone, const char* format, ...);

void DbgTr_configureGroup ( byte_t group, dword_t zoneMask );
void DbgTr_enableTrace ( byte_t group, dword_t zoneMask );
void DbgTr_disableTrace ( byte_t group, dword_t zoneMask );

void DbgTr_empty1 ( const char *format, ...);
void DbgTr_empty2 ( byte_t group, dword_t zone, const char *format, ...);

/** @endcond */

#endif /*_SAL_DEBUGTRACE_BASE_H_*/
