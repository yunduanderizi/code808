#ifndef ENPOI_H
#define ENPOI_H

#include <innovEngine_types.h>
#include <language/lacodepage.h>

#ifdef __cplusplus
extern "C" {
#endif
/// @addtogroup MapData 
/// @{ 

/// @addtogroup POIFiles
///    This module provides a number fo API calls that allow the POI map data to be
///    accessed in a way that is reasonably independent of the underlying
///    map data format.  Typically the binary map data is unpacked into a C
///    structure where it can be more easily accessed.  Objects such as POIs,
///    POI content and settlements need to be unpacked from the map in the
///    right sequence to ensure that all the data they contain is valid.  
///    POI map data chunks are dynamically loaded and unloaded from a map cache
///    in RAM. 
/// @ingroup MapData 
/// @{ 

/** @def POI_MAX_NAME_SIZE
    POI_MAX_NAME_SIZE defines the longest length name of any type that
    the map engine POI functions can produce.  It should be used to define
    the size of character buffers that are used to contain POI or place 
    names. */
#define POI_MAX_NAME_SIZE 72

/** @def POI_PLACE_TYPES
    This is the depth of the place name hierarchy */
#define POI_PLACE_TYPES 7

/** @def POI_PLACE_TYPE_COUNTRY
    Index of the place column corresponding to a country name */
#define POI_PLACE_TYPE_COUNTRY 6

/** @def POI_PLACE_TYPE_STATE
    Index of the place column corresponding to a state / principality name */
#define POI_PLACE_TYPE_STATE 5

/** @def POI_PLACE_TYPE_LAST_MAJOR
    Last index of the place column corresponding to a city / district name */
#define POI_PLACE_TYPE_LAST_MAJOR 4

/** @def POI_PLACE_TYPE_FIRST_MAJOR
    First index of the place column corresponding to a city / district name */
#define POI_PLACE_TYPE_FIRST_MAJOR 4

/** @def POI_PLACE_TYPE_LAST_MINOR
    Last index of the place column corresponding to a town / suburb name */
#define POI_PLACE_TYPE_LAST_MINOR 4

/** @def POI_PLACE_TYPE_FIRST_MINOR
    First index of the place column corresponding to a town / suburb name */
#define POI_PLACE_TYPE_FIRST_MINOR 0

/** @def POI_PLACE_TYPE_BUA
    First index of the place column corresponding to a built up area name */
#define POI_PLACE_TYPE_BUA 0
  
/// Maximum size of the content field.
#define MAX_CONTENT_FIELD 256

/** @brief 
    The maximum number of content tags supported in
    one content field.
    See TPOIContentTags for the ones supported. */
#define MAX_CONTENT_TAGS 3
/* The number of separate POI layers that can be contained
   within a coverage area. */
typedef enum {
  POI_MAP_LAYER_0,
  POI_MAP_LAYER_1,
  POI_MAP_LAYER_2,
  POI_MAP_LAYER_3,
  // *** THIS MUST COME LAST
  POI_MAP_LAYERS
} TPOIMapLayer;
/// Tag IDs for the content field of POIs.
typedef enum TPOIContentTags {
  /// Reserved to indicate the end of content fields.
  POI_CONTENT_END = 0, /* Don't generally want to use this code for anything */
  POI_CONTENT_ROAD_NAME = 1,
  /// The house number.
  POI_CONTENT_HOUSE_NUMBER,
  POI_CONTENT_PLACE,
  POI_CONTENT_NAME,
  /// The telephone number.
  POI_CONTENT_TELEPHONE,
  POI_CONTENT_FAX,
  POI_CONTENT_EMAIL,
  POI_CONTENT_WEB,
  /// Speed information in mm/s.
  POI_CONTENT_SPEED,
  POI_CONTENT_DESCRIPTION,

  /* Used for special purposes */
  POI_CONTENT_EXTENDED = 11, /* allows extensions of up to 255 tag types */
  POI_CONTENT_LINE_BREAK = 12 /* allows some content to be formatted */

  /* extended tags can go from 32 to 255 */
} TPOIContentTags;

/// POI content unpacked, and with the tag name translated.
typedef struct TPOIContentField {
  /// The tag ID
  TPOIContentTags tagID;
  /// The tag name translated into the current language in the internal codepage.
  UTF16 translatedTagName[POI_MAX_NAME_SIZE];
  /// The raw content field excluding the tag.
  UTF16 rawContent[MAX_CONTENT_FIELD];
} TPOIContentField;
 //@TPE

/** @typedef struct TPOIMapHeader
    This keeps the display subsystem happy as it uses some private types...
    Essentially this declaration becomes an opaque type declaration int the 
    eyes of the public. */
typedef struct TPOIMapHeader TPOIMapHeader;

/// @} 
/// @} 

#ifdef __cplusplus
}
#endif

#endif /* ENPOI_H */
