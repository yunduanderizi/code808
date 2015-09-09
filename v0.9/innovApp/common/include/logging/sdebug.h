/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef SDEBUG_H
#define SDEBUG_H

#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Debug
/// @ingroup Utilities
/// @{

/*----------------------------------------------------------------------*/
/// Public macros

// There are two functionally equivalent copies of the user macros.  One
// that is avaiable in a release build and one that is not.  RDBG macros
// remain in release builds.  TDBG ones are removed

// RELEASE DEBUG
#define RDBG_declareFlag( name ) SDBG_declareFlag_internal( name, NULL )
#define RDBG_useFlag( name ) SDBG_useFlag_internal( name )
#define RDBG_testFlag( name ) SDBG_testFlag_internal( name )
#define RDBG_setFlag( name, enabled )                   \
  SDBG_setFlag_internal( name, enabled, NULL, NULL )
#define RDBG( name, format ) SDBG_internal( name, format )
#define RDBG_noTerm( name, format ) SDBG_noTerm_internal( name, format )
#define RDBG_append( name, format ) SDBG_append_internal( name, format )

#define RDBG_testFlagIntValue( name, int_val )          \
  SDBG_testFlagIntValue_internal( name, int_val )
#define RDBG_testFlagStrValue( name, str_val )          \
  SDBG_testFlagStrValue_internal( name, str_val )
#define RDBG_getFlagIntValue( name, int_ptr )           \
  SDBG_getFlagIntValue_internal( name, int_ptr )
#define RDBG_getFlagStrValue( name, char_ptr_first, char_ptr_last )     \
  SDBG_getFlagStrValue_internal( name, char_ptr_first, char_ptr_last )
#define RDBG_declareFlagEx( name, argproc )             \
  SDBG_declareFlag_internal( name, argproc )
#define RDBG_setFlagEx( name, enabled, begin, end )     \
  SDBG_setFlag_internal( name, enabled, begin, end )


// TEST DEBUG
#ifdef NDEBUG
// define all these macros to nothing if this is a release build
#define TDBG_declareFlag( name )
#define TDBG_useFlag( name )
#define TDBG_testFlag( name ) (0)
#define TDBG_setFlag( name, enabled ) ((void)0)
#define TDBG( name, format ) ((void)0)
#define TDBG_noTerm( name, format ) ((void)0)
#define TDBG_append( name, format ) ((void)0)

#define TDBG_testFlagIntValue( name, int_val ) (0)
#define TDBG_testFlagStrValue( name, str_val ) (0)
#define TDBG_getFlagIntValue( name, int_ptr ) (0)
#define TDBG_getFlagStrValue( name, char_ptr_first, char_ptr_last ) (0)
#define TDBG_declareFlagEx( name, argproc )
#define TDBG_setFlagEx( name, argproc, begin, end ) ((void)0)

#else 
// this isn't a release build, so enable the test debug

/// Declare a new debug flag.  This must be done in the global namespace.
/// The flag name must be unique across the entire application
///
/// @param name The name of the debug flag to declare
#define TDBG_declareFlag( name ) SDBG_declareFlag_internal( name, NULL )

/// Bring a previously delcared debug flag into scope.  This must be done
/// in the global namespace, and the flag name must be defined somewhere
/// else.  This is useful for using a single debug flag across multiple
/// compilation units.
///
/// @param name The name of the debug flag to declare
#define TDBG_useFlag( name ) SDBG_useFlag_internal( name )

/// Used to test whether a particular debug flag is on or off.  This macro
/// is suitable for using within a conditional or to save the state of a
/// flag.  It evaluates to 0 if the flag is disabled and !0 otherwise
/// 
/// @param name The flag to test
/// @return - 0 if the flag is disabled
///         - !0 if the flag is enabled
#define TDBG_testFlag( name ) SDBG_testFlag_internal( name )

/// Set the state of a debug flag.  This allows a progam to (un)set its own
/// debug flags.  This can be useful if it is necessary to provide APIs
/// that turn flags on an off, or if you want to conditionally enable debug
/// based on another flag.
///
/// @param name The flag to set
/// @param enabled - 0 to disable the debug associated with this flag 
///                - !0 to enable it
#define TDBG_setFlag( name, enabled ) SDBG_setFlag_internal( name, enabled, NULL, NULL )

/// Output a line of debug text.  This text will be prepended with useful
/// location information (line/function/file).  A line terminator is added
/// by the macro.
///
/// @param name The flag on which this output is conditional
/// @param format A "printf" style output sequence in brackets.  
///               e.g. TDBG( myflag ( "Blah %d", 2 ) );
#define TDBG( name, format ) SDBG_internal( name, format )

/// Output a line of debug text.  This text will be prepended with useful
/// location information (line/function/file).  To allow complex, multiline
/// output, line terminators are not added automatically and are the
/// responsiblity of the callee.  This macro is designed to be used with
/// TDBG_append() below.
///
/// @param name The flag on which this output is conditional
/// @param format A "printf" style output sequence in brackets.  
///               e.g. TDBG_noTerm( myflag ( "Blah %d", 3 ) );
#define TDBG_noTerm( name, format ) SDBG_noTerm_internal( name, format )

/// Append a line of debug text to a prior (unterminated) line.  This macro
/// should only be used to append text to a debug line already started with
/// TDBG_noTerm() - It is not save to use on its own.  Line terminators are
/// the responsibility of the callee.
///
/// @param name The flag on which the parent TDBG() statment was conditional
/// @param format A "printf" style output sequence in brackets.  
///               e.g. TDBG_append( myflag ( "Blah %d\n", 6 ) );
#define TDBG_append( name, format ) SDBG_append_internal( name, format )


// ==== Macros for using and handling debug flags with values ====
// The debug flag sdbgParse will print out flag values as they are parsed

/// Test whether a particular debug flag is on or off AND whether it has
/// the specified integer value.  This macro is suitable for using within a
/// conditional.  It evaluates to 0 if the flag is disabled OR has the
/// incorrect integer value, and !0 otherwise
/// 
/// @param name The flag to test
/// @param int_value The signed int to test the value against
/// @return - 0 if the flag is disabled OR the integer value mismatches
///         - !0 if the flag is enabled AND the interger value matches
#define TDBG_testFlagIntValue( name, int_val )          \
  SDBG_testFlagIntValue_internal( name, int_val )

/// Test whether a particular debug flag is on or off AND whether it has
/// the specified string value.  This macro is suitable for using within a
/// conditional.  It evaluates to 0 if the flag is disabled OR has the
/// incorrect string value, and !0 otherwise
///
/// strncmp is used - so this is case sensitive and length sensitive
/// 
/// @param name The flag to test
/// @param str_val The const char* string to test the value against
/// @return - 0 if the flag is disabled OR the string value mismatches
///         - !0 if the flag is enabled AND the string value matches
#define TDBG_testFlagStrValue( name, str_val )          \
  SDBG_testFlagStrValue_internal( name, str_val )

/// Retrieve the integer value of a particular debug flag.
/// strtol is used to parse the integer - so dec, hex or oct will work
///
/// Note: If the flag does not have a parsable integer value, INT32_MAX
/// will be returned as that value
///
/// @param[in] name The flag whose value should be retrieved
/// @param[out] int_ptr Pointer to int for returned value
/// @return - 0 if the flag is disabled (integer value is invalid)
///         - !0 if the flag is enabled (integer value is valid)
#define TDBG_getFlagIntValue( name, int_ptr )           \
  SDBG_getFlagIntValue_internal( name, int_ptr )

/// Retrieve the string value of a particular debug flag.
/// The returned string is not null terminated, so two pointers are
/// returned, the first to the start of the string and the second to the
/// position where the terminating '\0' would normally be (one past the
/// end)
///
/// Note: If the flag does not have a string value both return pointers
/// will be equal (and possibly NULL).  This implies length 0, the empty
/// string.
///
/// @param[in] name The flag whose value should be retrieved
/// @param[out] char_ptr_first Pointer to start of returned value
/// @param[out] char_ptr_last Pointer to one past end of returned value
/// @return - 0 if the flag is disabled (string value is invalid)
///         - !0 if the flag is enabled (string value is valid)
#define TDBG_getFlagStrValue( name, char_ptr_first, char_ptr_last )     \
  SDBG_getFlagStrValue_internal( name, char_ptr_first, char_ptr_last )

/// An extended version of TDBG_declareFlag which allows an "argproc" to be
/// specified.  This is a function pointer of the form
///      void (*argproc)( const char* begin, const char* end );
/// and will be called with the flags value string whenever the flag is
/// parsed (usually only once).  This allows complex value parsing if requied.
#define TDBG_declareFlagEx( name, argproc )             \
  SDBG_declareFlag_internal( name, argproc )

/// An extended version of TDBG_setFlag which allows both the flags state
/// (enabled/disabled) and its value to be set
///
/// Note: The passed in value pointers are not copied and must remain valid
/// until either SDBG_setFlagString is called or TDBG_setFlagEx is called
/// again
///
/// @param[in] name The flag whose state should be set
/// @param[in] enabled Desired state (0 disabled, !0 enabled)
/// @param[in] begin Pointer to start of value string
/// @param[in] end Pointer to one past end of value string
#define TDBG_setFlagEx( name, enabled, begin, end )     \
  SDBG_setFlag_internal( name, enabled, begin, end )

#endif // NDEBUG

/*----------------------------------------------------------------------*/
/// Internal types, macros and functions, don't use directly

typedef struct SDBG_TFlagRec {
  int enabled;             ///< flag state - enabled or disabled?
  int initialised;         ///< is the correct flag state known?
  void (*argproc)( const char* begin, const char* end );
  int int_val;             ///< avoid reparsing the integer value
  const char* val_begin;   ///< first char in value
  const char* val_end;     ///< one past last char in value
  struct SDBG_TFlagRec* next;
} SDBG_TFlagRec;

void SDBG_parseValue( SDBG_TFlagRec* pFlag, const char* flagname,
                      const char* begin, const char* end );
int SDBG_parseKeyValueString( 
  const char* keyName,  
  const char* keyStrBegin, const char* keyStrEnd,
  const char** valueBegin, const char** valueEnd );
int  SDBG_parseFlag( SDBG_TFlagRec* pFlag, const char* flagname );
void SDBG_recordFlag( SDBG_TFlagRec* pFlag );
void SDBG_printLocation( const char* file, const char* function,
                         int line, const char* flag );

// ARM RealView and early versions of the WinCE compiler don't support the
// __FUNCTION__ predefined macro.
#if ( (defined(UNDER_CE) && defined(_MSC_VER) && _MSC_VER < 1300) ||  \
      defined(__ARMCC_VERSION) )
#define __FUNCTION__ ""
#endif

#define SDBG_declareFlag_internal( name, argproc )                    \
  SDBG_TFlagRec SDBGFlag_ ## name = { 1, 0, argproc };

#define SDBG_useFlag_internal( name )                                 \
  extern SDBG_TFlagRec SDBGFlag_ ## name;

#define SDBG_testFlag_internal( name )                                \
  ( SDBGFlag_ ## name .enabled &&                                     \
    ( SDBGFlag_ ## name .initialised                                  \
       ? SDBGFlag_ ## name .enabled                                   \
       : ( SDBG_recordFlag( &SDBGFlag_ ## name ),                     \
           SDBG_parseFlag( &SDBGFlag_ ## name, #name )) ) )

#define SDBG_setFlag_internal( name, _enabled, begin, end )           \
  { if( !SDBGFlag_ ## name .initialised ) {                           \
      SDBG_recordFlag( &SDBGFlag_ ## name );                          \
    }                                                                 \
    SDBGFlag_ ## name .initialised = 1;                               \
    SDBGFlag_ ## name .enabled = ((_enabled) ? 1 : 0);                \
    if( SDBGFlag_ ## name .enabled ) {                                \
      SDBG_parseValue( &SDBGFlag_ ## name, #name, begin, end );       \
    }                                                                 \
  }

#define SDBG_internal_internal( name, format, termaction )            \
  if( SDBGFlag_ ## name .enabled ) {                                  \
    if( !SDBGFlag_ ## name .initialised ) {                           \
      SDBG_recordFlag( &SDBGFlag_ ## name );                          \
      SDBG_parseFlag( &SDBGFlag_ ## name, #name );                    \
    }                                                                 \
    if( SDBGFlag_ ## name .enabled ) {                                \
      SDBG_printLocation( __FILE__, __FUNCTION__, __LINE__, #name );  \
      os_printf format;                                               \
      termaction                                                      \
    }                                                                 \
  }

#define SDBG_internal( name, format )                                 \
  SDBG_internal_internal( name, format, os_printf( "\n" ); )

#define SDBG_noTerm_internal( name, format )                          \
  SDBG_internal_internal( name, format, )

#define SDBG_append_internal( name, format )                          \
  if( SDBGFlag_ ## name .enabled ) { os_printf format ; }

#define SDBG_testFlagIntValue_internal( name, _int_val )              \
  ( SDBG_testFlag_internal( name )                                    \
    && ( SDBGFlag_ ## name .int_val == _int_val ) )

#define SDBG_testFlagStrValue_internal( name, _str_val )              \
  ( SDBG_testFlag_internal( name )                                    \
    && ( SDBGFlag_ ## name .val_end - SDBGFlag_ ## name .val_begin    \
         == strlen(_str_val) )                                        \
    && ( !strncmp( SDBGFlag_ ## name .val_begin, _str_val,            \
           SDBGFlag_ ## name .val_end - SDBGFlag_ ## name .val_begin ) ) )

#define SDBG_getFlagIntValue_internal( name, int_ptr )                \
  ( SDBG_testFlag_internal( name )                                    \
    && ( *int_ptr = SDBGFlag_ ## name .int_val, 1 ) )

#define SDBG_getFlagStrValue_internal(                                \
                                name, char_ptr_first, char_ptr_last ) \
  ( SDBG_testFlag_internal( name )                                    \
    && ( *char_ptr_first = SDBGFlag_ ## name .val_begin,              \
         *char_ptr_last = SDBGFlag_ ## name .val_end, 1 ) )


/*----------------------------------------------------------------------*/
/// Set debug flags in the the innovEngine switchable debug subsystem.  This
/// API should be called at startup or if the debug flag string has
/// changed.  All flags are off by default if this API is not called.
///
/// Calling this API refreshes all debug flags on or off according to the
/// string.  The string should be a comma seperated concatenated list of
/// debug flags that should be on.  All others will be turned off.
/// e.g. rtBasic,rtMapDump,diShowFPS
///
/// <b>This string is not copied, and must persist between API calls.</b>
///
/// @param flagString A list of flags to turn on
void SDBG_setFlagString( const char* flagString );

/// @} end of doxygen group

#ifdef __cplusplus
}
#endif

#endif // include guard
