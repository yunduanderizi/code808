/* Copyright 2014, INNOV China, All Rights reserved. */

#ifndef INCLUDE_GUARD_gsSearchFilter_h
#define INCLUDE_GUARD_gsSearchFilter_h

#include <containers/gebitset.h>
#include <geosearch/geosearchP.h>

namespace innov {
namespace geosearch {

//--------------------------------------------------------------------------------
UINT32 calcPOITypeFilter( TBitset*            POITypeFilterBitset,
                          const TPOITypeId16* POITypes,
                          UINT32              POITypeCount,
                          TGenericMapInfo*    POIMapInfo );

//--------------------------------------------------------------------------------
} //namespace geosearch
} //namespace innov

#endif //INCLUDE_GUARD_gsSearchFilter_h
