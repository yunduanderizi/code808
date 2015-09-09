/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_gsParser_h
#define INCLUDE_GUARD_gsParser_h

#include <parser/parserP.h>

namespace innov {
namespace geosearch {

// Used for road address searches and intersecting road searches.
extern const parser::TParser roadAddressParser;
extern const parser::TParser simpleRoadAddressParser;

// Used for place address searches (including country and state).
extern const parser::TParser placeAddressParser;
extern const parser::TParser simplePlaceAddressParser;

// Used for postal code address searches (including detailed postal codes).
extern const parser::TParser postalCodeAddressParser;
extern const parser::TParser simplePostalCodeAddressParser;

// Used for intersection searches (road name, road name specified).
extern const parser::TParser intersectionAddressParser;
extern const parser::TParser simpleIntersectionAddressParser;

// Used for house number address searches.
extern const parser::TParser houseNumberAddressParser;
extern const parser::TParser simpleHouseNumberAddressParser;

// Used for house number searches.
extern const parser::TParser houseNumberParser;

// Used for POI address searches.
extern const parser::TParser POIAddressParser;
extern const parser::TParser simplePOIAddressParser;

// Used for POI type searches.
extern const parser::TParser POITypeNameParser;

// Used for nearest POI searches.
extern const parser::TParser nearestPOINameParser;

// Used for global address searches.
extern const parser::TParser globalAddressParser;

// Used for lat/long address searches.
extern const parser::TParser latLongAddressParser;

} //namespace geosearch
} //namespace innov

#endif //INCLUDE_GUARD_gsParser_h
