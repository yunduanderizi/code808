#ifndef LAGUIDETP_H
#define LAGUIDETP_H

#include <language/laguidet.h>

#ifdef __cplusplus
extern "C" {
#endif

// Check a couple of options to see if we can find a road name
// Check for a route name first, then for a road name
// return as a internal string
void language_findRoadName( const TGuideInstruction *pInstr, UTF16* roadName );

// Concatenate the place name ( with parenthesies ) to the given string location
// No place name -> Nothing happens
// Return as an internal string
void language_findPlaceName( const TGuideInstruction *pInstr, UTF16* placeName );


#ifdef __cplusplus
}
#endif

#endif /* LAGUIDETP_H */
