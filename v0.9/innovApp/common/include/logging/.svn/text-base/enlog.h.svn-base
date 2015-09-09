/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef ENLOG_H
#define ENLOG_H

#include <logging/sdebug.h>
#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Logging
/// @ingroup Utilities
/// @{

// Logging is now implemented using the switchable debug system.  Import
// the appropriate flags from enlog.c.  The mc_INFO, mc_WARN, mc_ERROR and 
// mc_FATAL flags are forced always on.  The LOG_DEBUG macro will only produce
// output if the mcDebug flags is enabled using SDBG_setFlagString().
RDBG_useFlag( mc_INFO );
RDBG_useFlag( mc_WARN );
RDBG_useFlag( mc_ERROR );
RDBG_useFlag( mc_FATAL );
RDBG_useFlag( mc_Debug );

#define LOGTHROTTLEMSG \
  "No further messages logged...  Throttle count (%d) exceeded."

/// The LOG_RAW macro produces a "raw" log message with no prefix.  This is
/// equivalent to os_printf and should be avoided unless both the LOG_* and
/// SDBG systems are unsuitable.  A possible useage would be raw xml debug
/// output.
#define LOG_RAW( content ) os_printf content

/// LOG_INFO is for always on information messages - e.g. "map loading
/// complete" or similar
#define LOG_INFO( _throttleCount_, _messagelist_ )              \
  { static int _count_ = 0;                                     \
    if( !_throttleCount_ || _count_ < _throttleCount_ ) {       \
      if( !SDBGFlag_mc_INFO.initialised ) {                     \
        SDBGFlag_mc_INFO.enabled = 1;                           \
        SDBGFlag_mc_INFO.initialised = 1;                       \
      }                                                         \
      RDBG( mc_INFO, _messagelist_ );                           \
      if( ++_count_ == _throttleCount_ ) {                      \
        RDBG( mc_INFO, ( LOGTHROTTLEMSG, _throttleCount_) );    \
      }                                                         \
    }                                                           \
  }

/// LOG_DEBUG is <b>off</b> by default - this macro will only print
/// messages if the "mcDebug" SDBG flag is enabled.
#define LOG_DEBUG( _throttleCount_, _messagelist_ )             \
  { static int _count_ = 0;                                     \
    if( !_throttleCount_ || _count_ < _throttleCount_ ) {       \
      RDBG( mc_Debug, _messagelist_ );                          \
      if( ++_count_ == _throttleCount_ ) {                      \
        RDBG( mc_Debug, ( LOGTHROTTLEMSG, _throttleCount_) );   \
      }                                                         \
    }                                                           \
  }

/// LOG_WARN is for always on warning messages - e.g. for non-fatal,
/// non-severe errors - "file myfavs.dat not found"
#define LOG_WARN( _throttleCount_, _messagelist_ )              \
  { static int _count_ = 0;                                     \
    if( !_throttleCount_ || _count_ < _throttleCount_ ) {       \
      if( !SDBGFlag_mc_WARN.initialised ) {                     \
        SDBGFlag_mc_WARN.enabled = 1;                           \
        SDBGFlag_mc_WARN.initialised = 1;                       \
      }                                                         \
      RDBG( mc_WARN, _messagelist_ );                           \
      if( ++_count_ == _throttleCount_ ) {                      \
        RDBG( mc_WARN, ( LOGTHROTTLEMSG, _throttleCount_) );    \
      }                                                         \
    }                                                           \
  }

/// LOG_ERROR is for always on non-fatal error messages.  These document
/// events that should never occur, but are none-the-less non-fatal.
#define LOG_ERROR( _throttleCount_, _messagelist_ )             \
  { static int _count_ = 0;                                     \
    if( !_throttleCount_ || _count_ < _throttleCount_ ) {       \
      if( !SDBGFlag_mc_ERROR.initialised ) {                    \
        SDBGFlag_mc_ERROR.enabled = 1;                          \
        SDBGFlag_mc_ERROR.initialised = 1;                      \
      }                                                         \
      RDBG( mc_ERROR, _messagelist_ );                          \
      if( ++_count_ == _throttleCount_ ) {                      \
        RDBG( mc_ERROR, ( LOGTHROTTLEMSG, _throttleCount_) );   \
      }                                                         \
    }                                                           \
  }

/// LOG_FATAL if for non-recoverable fatal errors.
/// the throttleCount is ignored for fatal error messages.
#define LOG_FATAL( _throttleCount_, _messagelist_ )     \
  { if( !SDBGFlag_mc_FATAL.initialised ) {              \
      SDBGFlag_mc_FATAL.enabled = 1;                    \
      SDBGFlag_mc_FATAL.initialised = 1;                \
    }                                                   \
    RDBG( mc_FATAL, _messagelist_ );                    \
  }

/// Declares a variable used for measuring times.
#define TIMINGS_DECLARE( var )                          \
  UINT64 var

/// Sets a timing variable to 0.
#define TIMINGS_CLEAR( accTime )                        \
  accTime = 0

/// Stores the current PC time in a timing variable. This should be done for
/// measuring the start time.
#define TIMINGS_START( startTime )                      \
  startTime = os_rdtsc()

/// Determins the time difference between the start and the current PC time
/// and adds it to the accumulated timing variable.
#define TIMINGS_ACCUMULATE( startTime, accTime )        \
  accTime += os_rdtsc() - startTime

/// Converts the a timing value into milliseconds.
#define TIMINGS_MS( accTime )                           \
  ( (UINT32)( accTime / ( os_rdtsf() / 1000 ) ) )


// Deprecated macros - these no longer have any affect whatsoever
#define LOG_ENABLECOMPONENT( component )
#define LOG_DISABLECOMPONENT( component )
#define LOG_ISLOGGING( severity ) (FALSE)
#define LOG_SETSEVERITY( component, severity )
#define LOG_CLEARSEVERITY( component, severity )
#define LOG_RESETSEVERITY( component )

/// @} end of doxygen group 

#ifdef __cplusplus
}
#endif

#endif // include guard
