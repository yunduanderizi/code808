/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_gsAddressDisplay_h
#define INCLUDE_GUARD_gsAddressDisplay_h

#include <geosearch/geosearchP.h>
#include <mapdata/enmapinfoP.h>

namespace innov {
namespace geosearch {

//--------------------------------------------------------------------------------
struct TAddressDisplayDetails {
  TNumericISOCountryCode  numericISOCountryCode;
  TGenericMapInfo        *roadMapInfo;
  UTF16                   houseNumber   [GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   roadName      [GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   altRoadName   [GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   intRoadName   [GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   POIName       [GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   altPOIName    [GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   POIType       [GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   altPOIType    [GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   placeNames    [PLACE_TYPES][GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   settlementName[GEOSEARCH_MAX_NAME_SIZE];
  UTF16                   postalCode    [GEOSEARCH_MAX_NAME_SIZE];
  TGeosearchPlaceColumn   requiredPlaceColumn;
};

//--------------------------------------------------------------------------------
BOOL8 formatAddressDisplayName( const TAddressDisplayDetails *addressDisplayDetails,
                                UINT32                        maxDisplayNameLength,
                                UTF16                        *addressDisplayName );

//--------------------------------------------------------------------------------
TGeosearchPlaceColumn getPostalCodeDisplayColumn( TNumericISOCountryCode numericISOCountryCode,
                                                  TGenericMapInfo*       roadMapInfo );

//--------------------------------------------------------------------------------
} //namespace geosearch
} //namespace innov

#endif //INCLUDE_GUARD_gsAddressDisplay_h
