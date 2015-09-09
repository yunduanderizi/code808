#ifndef ENPHONE_H
#define ENPHONE_H

#include <platform_types.h>
#include <innovEngine_types.h>
#include <language/latypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Phonetics
/// @ingroup MapData 
/// @{ 

typedef enum {
  PHONE_DATA_TYPE_ROAD,
  PHONE_DATA_TYPE_PLACE,
  PHONE_DATA_TYPE_POI,
  PHONE_DATA_TYPE_SIGNPOST,
  /* Total number of phonetic data types, 
   * must come last */
  PHONE_DATA_TYPES  
} TPhoneticDataType;


/* Phonetic Callback which can be used as an additional source for phonetics 
 * if no data is available in the maps 
 *
 * */  
typedef UINT32 TPhoneticSourceCallback( const UTF16*      sourceName,
                                        TPhoneticDataType dataType,
                                        TLanguageCode     langCode,
                                        UTF16*            outputPhonetics ); 

void phone_registerAlternateCallback( TPhoneticSourceCallback *callback );

/// @} 

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------*/

#endif
