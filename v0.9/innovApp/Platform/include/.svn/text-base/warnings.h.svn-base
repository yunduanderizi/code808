#pragma once
/** @file 
    \brief Adds macros for pragma message and remove warnings
    
 *****************************************************************************
 *  (c) 2005 navman new zealand limited. all rights reserved.
 *
 *  module name: Warnings.h
 *
 *  Author: Jordan Foo
 *
 *****************************************************************************/

 
// PRAGMA Message Define extra
/*  This allows the user to put a pragma message that they can double click
 *  on to jump directly to the source code line. The usage is as follows:
 *    #pragma message( __FIXME__ "The message" );
 *  JORDAN::COMMENTS: AVOID USING THESE MACROS AS THEY ARE DEPRECATED
 *  They have been put back for backward compatibility
 */
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __FIXME__ __FILE__ "("__STR1__(__LINE__)") : warning FIXME: "
#define __TODO__ __FILE__ "("__STR1__(__LINE__)") : warning TODO: "


#if defined(WIN32) || defined(_WIN32_WCE)
/*
Current:
 * C4800 - 'type' : forcing value to bool 'true' or 'false' (performance warning)
 * C4786 - 'identifier' : identifier was truncated to 'number' characters in the debug information. 
 * C4530 - C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
 * C4099 - 'identifier' : type name first seen using 'objecttype1' now seen using 'objecttype2'
 * C4049 - compiler limit : terminating line number emission
 * C4005 - 'identifier' : macro redefinition. TOM: Disable for SYS-153
 * C4355 - 'this' : used in base member initializer list. TOM: Temporarily disable for SST-4255

Old:
 * C4996 - 'function': was declared deprecated
*/
#pragma warning(disable : 4800 4786 4530 4099 4049 4005)
#endif // WIN32 || _WIN32_WCE
