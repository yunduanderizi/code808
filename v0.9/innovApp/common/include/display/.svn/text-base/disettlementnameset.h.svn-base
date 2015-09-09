#ifndef DISETTLEMENTNAMESET_H
#define DISETTLEMENTNAMESET_H

#include <innovEngine_types.h>
#include <display/didisplaynameset.h>
#include <language/utf16.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BEGIN HACK */
struct TViewport;
/* END HACK */

#define MAX_SETTLEMENT_NAMES  30

/// @addtogroup Display
/// @{

/// @addtogroup UnUsed
/// @ingroup Display
/// @{

/// @addtogroup RoadLabelling
/// @ingroup UnUsed
/// @{

/// @addtogroup SettlementNamePos
/// @ingroup RoadLabelling
/// @{

///
/// @typedef struct TSettlementNamePos
///
typedef struct TSettlementNamePos {
  UTF16 *name;
  TPos pos;
  UINT8 placement;
  UINT8 importance;
} TSettlementNamePos;

/// @}

/// @addtogroup SettlementNameSet
/// @ingroup RoadLabelling
/// @{

///
/// @typedef struct TSettlementNameSet
///
typedef struct TSettlementNameSet
{
  TSettlementNamePos settlementNames[MAX_SETTLEMENT_NAMES];
  UINT8 settlementNameCount;
  UINT8 settlementPlaces[MAX_SETTLEMENT_NAMES];
  UINT8 settlementPlacesCount;
  UTF16 settlementNameText[DISPLAY_MAXTEXT];
  UTF16 *currentSettlementName;
} TSettlementNameSet;

/// @}

/// @}

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif
