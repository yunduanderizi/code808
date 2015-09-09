/******************************************************************************
 *  (c) 2003 INNOV China Limited, All Rights Reserved.
 *
 *  Module Name: gevers.c
 *
 *  Author: Derek Wong <DWong@21com.com>
 *
 *  Description:
 *    Simple Map Core library versioning with API for enquiry.
 *****************************************************************************/

#ifndef GEVERS_H
#define GEVERS_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Initialisation
/// @{

/** The current Map Core release */
#define MAPCORE_VERSION 130

/** Return a pointer to a C-style null terminated string that contains
    versioning information for the Map Core library.  The information string
    takes the following form:
    
    @par
    T-core-[vendor]-vXX_yy_nnnn yyyy/mm/dd-HH:mm:ss
    - [vendor] is an optional vendor tag used for vendor specific releases.
    - XX is the major revision number.
    - yy is the minor revision number.
    - nnnn is the build release number.
    - dd/mm/yyyy is the date of the build.
    - HH:mm:ss is the approximate time the library was generated.
    
    @par
    If the source used to build T-core-XX_yy_nnnn happens to be at the 
    head of CVS then "HEAD" is inserted as the build tag for the
    library, e.g. HEAD yyyy/mm/dd-HH:mm:ss.
    
    @par
    The version number XX.yy.nnnn is used as a release tag under CVS so
    if problems occur then the Map Core library version can be quoted so
    that the problem can be investigated and fixed against the relevant
    build.
    
    @return
    A pointer to a C-style null terminated string. */
extern MAPCORE_API
const char * version_getLibraryIdString( void );

/// @}

#ifdef __cplusplus
}
#endif

#endif /* GEVERS_H */
